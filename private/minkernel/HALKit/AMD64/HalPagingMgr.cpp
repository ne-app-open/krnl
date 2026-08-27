// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <HALKit/AMD64/Paging.h>
#include <HALKit/AMD64/Processor.h>
#include <KernelKit/DebugOutput.h>
#include <HALKit/Generic/PhysicalMemory.h>

namespace Ne::Kernel::HAL {
namespace Detail {
  /// @brief Page Table Entry for AMD64.
  struct PTE {
    UInt64 Present : 1;
    UInt64 Wr : 1;
    UInt64 User : 1;
    UInt64 Pwt : 1;  // Page-level Write-Through
    UInt64 Pcd : 1;  // Page-level Cache Disable
    UInt64 Accessed : 1;
    UInt64 Dirty : 1;
    UInt64 Pat : 1;  // Page Attribute Table (or PS for PDE)
    UInt64 Global : 1;
    UInt64 Ignored1 : 3;          // Available to software
    UInt64 PhysicalAddress : 40;  // Physical page frame address (bits 12–51)
    UInt64 Ignored2 : 7;          // More software bits / reserved
    UInt64 ProtectionKey : 4;     // Optional (if PKU enabled)
    UInt64 Nx : 1;                // No Execute, bit 63
  };
}  // namespace Detail

/***********************************************************************************/
/// @brief Build page tables of our own and run on them.
/// @param limit highest physical address that has to be reachable.
/// @return the new PML4's physical address, 0 on failure.
/***********************************************************************************/
EXTERN_C UIntPtr mm_init_kernel_tables(UIntPtr limit) {
  constexpr UIntPtr kEntries   = 512;
  constexpr UIntPtr kLargePage = 0x200000;    // 2 MiB
  constexpr UIntPtr kGiB       = 0x40000000;  // one PDPT entry's reach
  constexpr UIntPtr kTablePerm = 0x3;         // present + writable
  constexpr UIntPtr kLeafPerm  = 0x83;        // present + writable + PS

  auto pml4_pa = pmmi_alloc_frame();

  if (!pml4_pa) return 0UL;

  auto pml4 = reinterpret_cast<UInt64*>(pml4_pa);

  for (UIntPtr base = 0UL; base < limit; base += kGiB) {
    auto pml4_idx = (base >> 39) & (kEntries - 1);

    if (!(pml4[pml4_idx] & 1)) {
      auto pdpt_pa = pmmi_alloc_frame();

      if (!pdpt_pa) return 0UL;

      pml4[pml4_idx] = pdpt_pa | kTablePerm;
    }

    auto pdpt  = reinterpret_cast<UInt64*>(pml4[pml4_idx] & ~(kPageSize - 1));
    auto pd_pa = pmmi_alloc_frame();

    if (!pd_pa) return 0UL;

    pdpt[(base >> 30) & (kEntries - 1)] = pd_pa | kTablePerm;

    auto pd = reinterpret_cast<UInt64*>(pd_pa);

    for (UIntPtr i = 0UL; i < kEntries; ++i) {
      pd[i] = (base + (i * kLargePage)) | kLeafPerm;
    }
  }

  return pml4_pa;
}

/***********************************************************************************/
/// \brief Retrieve the page status of a PTE.
/// \param pte Page Table Entry pointer.
/***********************************************************************************/
STATIC Void mmi_page_status(Detail::PTE* pte) {
  NE_UNUSED(pte);

#ifdef __NE_VERBOSE_BITMAP__
  (Void)(kout << "Flag: " << (pte->Present ? "Present" : "Not Present") << kendl);
  (Void)(kout << "Flag: " << (pte->Wr ? "W/R" : "Not W/R") << kendl);
  (Void)(kout << "Flag: " << (pte->Nx ? "NX" : "Not NX") << kendl);
  (Void)(kout << "Flag: " << pte->User ? "User" : "Not User") << kendl);
  (Void)(kout << "Flag: " << (pte->Pcd ? "Not Cached" : "Cached") << kendl);
  (Void)(kout << "Flag: " << (pte->Accessed ? "Accessed" : "Not Accessed") << kendl);
  (Void)(kout << "Flag: " << (pte->ProtectionKey ? "Protected" : "Not Protected/PKU Disabled")
              << kendl);
  (Void)(kout << "Physical Address: " << hex_number(pte->PhysicalAddress) << kendl);
#endif
}

/***********************************************************************************/
/// @brief Gets a physical address from a virtual address.
/// @param virt a valid virtual address.
/// @return Physical address.
/***********************************************************************************/
EXTERN_C UIntPtr mm_get_page_addr(VoidPtr virt) {
  const UInt64 kVMAddr         = (UInt64) virt;
  const UInt64 kMask9Bits      = 0x1FFULL;
  const UInt64 kPageOffsetMask = 0xFFFULL;

  UInt64 cr3 = (UInt64) hal_read_cr3() & ~kPageOffsetMask;

  // Level 4
  auto   pml4  = reinterpret_cast<UInt64*>(cr3);
  UInt64 pml4e = pml4[(kVMAddr >> 39) & kMask9Bits];

  if (!(pml4e & 1)) return 0;

  // Level 3
  auto   pdpt  = reinterpret_cast<UInt64*>(pml4e & ~kPageOffsetMask);
  UInt64 pdpte = pdpt[(kVMAddr >> 30) & kMask9Bits];

  if (!(pdpte & 1)) return 0;

  constexpr UInt64 kPageSizeBit = 0x80;
  constexpr UInt64 kAddrMask    = 0x000FFFFFFFFFF000ULL;

  // A 1 GiB leaf stops the walk here.
  if (pdpte & kPageSizeBit) {
    constexpr UInt64 kGiBOffset = (1ULL << 30) - 1;

    return (pdpte & kAddrMask & ~kGiBOffset) | (kVMAddr & kGiBOffset);
  }

  // Level 2
  auto   pd  = reinterpret_cast<UInt64*>(pdpte & ~kPageOffsetMask);
  UInt64 pde = pd[(kVMAddr >> 21) & kMask9Bits];

  if (!(pde & 1)) return 0;

  // A 2 MiB leaf, the identity map is built out of these.
  if (pde & kPageSizeBit) {
    constexpr UInt64 kLargeOffset = (1ULL << 21) - 1;

    return (pde & kAddrMask & ~kLargeOffset) | (kVMAddr & kLargeOffset);
  }

  // Level 1
  auto   pt  = reinterpret_cast<UInt64*>(pde & ~kPageOffsetMask);
  UInt64 pte = pt[(kVMAddr >> 12) & kMask9Bits];

  if (!(pte & 1)) return 0;

  return (pte & kAddrMask) | (kVMAddr & kPageOffsetMask);
}

/***********************************************************************************/
/// @brief clflush+mfence helper function.
/***********************************************************************************/
EXTERN_C Int32 mm_memory_fence(VoidPtr virtual_address) {
  if (!virtual_address || !mm_get_page_addr(virtual_address)) return kErrorInvalidData;

  asm volatile("clflush (%0)" : : "r"(virtual_address) : "memory");
  asm volatile("mfence" ::: "memory");

  return kErrorSuccess;
}

/***********************************************************************************/
/// @brief Maps or allocates a page from virtual_address.
/// @param virtual_address a valid virtual address.
/// @param phys_addr point to physical address.
/// @param flags the flags to put on the page.
/// @return Status code of page manipulation process.
/***********************************************************************************/
EXTERN_C UInt64* mm_walk_page(UIntPtr root, UIntPtr virtual_address, Bool alloc) {
  constexpr UInt64 kMask9       = 0x1FF;
  constexpr UInt64 kPageMask    = 0xFFF;
  constexpr UInt64 kPageSizeBit = 0x80;
  constexpr UInt64 kEntries     = 512;

  /// @note privileges are ANDed down the walk, so every table on the way has to be
  /// permissive and the leaf carries the real policy.
  constexpr UInt64 kTablePerm = 0x7;  // present + writable + user

  if (!root) return nullptr;

  auto table = reinterpret_cast<UInt64*>(root & ~kPageMask);

  /// PML4 -> PDPT -> PD, splitting anything mapped as a large page on the way down.
  for (Int32 shift = 39; shift > 12; shift -= 9) {
    auto  idx   = (virtual_address >> shift) & kMask9;
    auto& entry = table[idx];

    if (!(entry & 1)) {
      if (!alloc) return nullptr;

      auto frame = pmmi_alloc_frame();

      if (!frame) return nullptr;

      entry = frame | kTablePerm;
    } else if (entry & kPageSizeBit) {
      if (!alloc) return nullptr;

      auto frame = pmmi_alloc_frame();

      if (!frame) return nullptr;

      auto step = 1ULL << shift;
      auto base = entry & ~(step - 1);

      /// @note carry the leaf's own attributes, never PS itself nor its PAT bit.
      auto perm = (entry & 0x1F) | (entry & 0x100) | (entry & (1ULL << 63));

      auto split = reinterpret_cast<UInt64*>(frame);

      for (UInt64 i = 0UL; i < kEntries; ++i) {
        split[i] = (base + (i * (step >> 9))) | perm | ((shift > 21) ? kPageSizeBit : 0);
      }

      entry = frame | kTablePerm;
    } else {
      entry |= kTablePerm;
    }

    table = reinterpret_cast<UInt64*>(entry & ~kPageMask);
  }

  return &table[(virtual_address >> 12) & kMask9];
}

/***********************************************************************************/
/// @brief Maps a page into a specific address space.
/***********************************************************************************/
EXTERN_C Int32 mm_map_page_in(UIntPtr root, VoidPtr virtual_address, VoidPtr physical_address,
                              UInt32 flags) {
  if (physical_address == 0) return kErrorInvalidData;

  /// @note the PTE only holds bits 12..51, an unaligned frame would silently map
  /// the page containing it and shift the whole view.
  if ((UIntPtr) physical_address & (kPageSize - 1)) return kErrorInvalidData;

  auto slot = mm_walk_page(root, (UIntPtr) virtual_address, Yes);

  if (!slot) return kErrorInvalidData;

  Detail::PTE* pte = reinterpret_cast<Detail::PTE*>(slot);

  *slot = 0UL;

  pte->Present = !!(flags & kMMFlagsPresent);
  pte->Wr      = !!(flags & kMMFlagsWr);
  pte->User    = !!(flags & kMMFlagsUser);
  pte->Nx      = !!(flags & kMMFlagsNX);
  pte->Pcd     = !!(flags & kMMFlagsPCD);
  pte->Pwt     = !!(flags & kMMFlagsPwt);

  pte->PhysicalAddress = ((UIntPtr) (physical_address)) >> 12;

  hal_invl_tlb(virtual_address);

  mmi_page_status(pte);

  return kErrorSuccess;
}

/***********************************************************************************/
/// @brief Maps a page into the address space we are running on.
/***********************************************************************************/
EXTERN_C Int32 mm_map_page(VoidPtr virtual_address, VoidPtr physical_address, UInt32 flags,
                           UInt32 level) {
  NE_UNUSED(level);

  return mm_map_page_in((UIntPtr) hal_read_cr3(), virtual_address, physical_address, flags);
}

/***********************************************************************************/
/// @brief Unmaps a page, returns the frame it pointed at, 0 when there was none.
/***********************************************************************************/
EXTERN_C UIntPtr mm_unmap_page(VoidPtr virtual_address) {
  auto slot = mm_walk_page((UIntPtr) hal_read_cr3(), (UIntPtr) virtual_address, No);

  if (!slot || !(*slot & 1)) return 0UL;

  constexpr UInt64 kAddrMask = 0x000FFFFFFFFFF000ULL;

  auto frame = *slot & kAddrMask;

  *slot = 0UL;

  hal_invl_tlb(virtual_address);

  return frame;
}
}  // namespace Ne::Kernel::HAL
