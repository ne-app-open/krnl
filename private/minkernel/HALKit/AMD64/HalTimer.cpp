// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <ArchKit/ArchKit.h>
#include <KernelKit/Timer.h>
#include <modules/ACPI/ACPIFactoryInterface.h>

/// ================================================================================
/// @note timer slot 0
/// ================================================================================

#define kHPETSignature ("HPET")

#define kHPETCounterRegValue (0x00)
#define kHPETConfigRegValue (0x20)
#define kHPETCompRegValue (0x24)
#define kHPETInterruptRegValue (0x2C)

/// ================================================================================
///! BUGS: 0
///! @file HalTimer.cc
///! @brief Hardware Timer (HPET)
/// ================================================================================

namespace Ne::Kernel::Detail {
struct HPET_BLOCK : public Ne::Kernel::SDT {
  Ne::Kernel::UInt8  hardware_rev_id;
  Ne::Kernel::UInt8  comparator_count : 5;
  Ne::Kernel::UInt8  counter_size : 1;
  Ne::Kernel::UInt8  reserved : 1;
  Ne::Kernel::UInt8  legacy_replacement : 1;
  Ne::Kernel::UInt16 pci_vendor_id;
  ACPI_ADDRESS   address;
  Ne::Kernel::UInt8  hpet_number;
  Ne::Kernel::UInt16 minimum_tick;
  Ne::Kernel::UInt8  page_protection;
} PACKED;
}  // namespace Ne::Kernel::Detail

using namespace Ne::Kernel;

HardwareTimer::HardwareTimer(UInt64 ms) : fWaitFor(ms) {
  auto power = PowerFactoryInterface(kHandoverHeader->f_HardwareTables.f_VendorPtr);

  auto hpet = (Detail::HPET_BLOCK*) power.Find(kHPETSignature).Leak().Leak();
  MUST_PASS(hpet);

  fDigitalTimer = (UInt8*) hpet->address.Address;

  if (hpet->page_protection) {
    HAL::mm_map_page((VoidPtr) fDigitalTimer, (VoidPtr) fDigitalTimer,
                     HAL::kMMFlagsWr | HAL::kMMFlagsPCD | HAL::kMMFlagsPwt);
  }

  // if not enabled yet.
  if (!(*((volatile UInt64*) ((UInt8*) fDigitalTimer + kHPETConfigRegValue)) & (1 << 0))) {
    *((volatile UInt64*) ((UInt8*) fDigitalTimer + kHPETConfigRegValue)) =
        *((volatile UInt64*) ((UInt8*) fDigitalTimer + kHPETConfigRegValue)) | (1 << 0) |
        (1 << 3);  // enable timer & one shot conf
  }
}

HardwareTimer::~HardwareTimer() {
  if (fDigitalTimer) fDigitalTimer = nullptr;
  fWaitFor      = 0;
}

/***********************************************************************************/
/// @brief Wait for the timer to stop spinning.
/***********************************************************************************/

BOOL HardwareTimer::Wait() {
  if (fWaitFor < 1) return NO;
  if (fWaitFor > 1'000'000) return NO;  // max 1000s = 16 minutes

  volatile UInt64* timer = (volatile UInt64*) (fDigitalTimer + kHPETCounterRegValue);

  if (!timer) return NO;

  UInt64 hpet_cap              = *((volatile UInt64*) (fDigitalTimer));
  UInt64 femtoseconds_per_tick = (hpet_cap >> 32);

  if (femtoseconds_per_tick == 0) return NO;

  UInt64 now = *timer;

  UInt64 fs_wait = fWaitFor * 1'000'000'000'000ULL;
  UInt64 stop_at = now + (fs_wait / femtoseconds_per_tick);

  while (*timer < (stop_at)) asm volatile("pause");

  return YES;
}
