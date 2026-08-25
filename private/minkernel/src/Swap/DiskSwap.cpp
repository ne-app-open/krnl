// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/FileMgr.h>
#include <SwapKit/DiskSwap.h>

namespace Ne::Kernel {

STATIC constexpr UInt32 kSwapDiskHeaderMagic = 0x44535750;  // 'DSWP'

/***********************************************************************************/
/// @brief Write memory chunk onto disk.
/// @param data the data packet.
/***********************************************************************************/
Int64 IDiskSwap::Write(SwapDiskHdrPtr data, const Char* name) {
  if (!data || data->fMagic != kSwapDiskHeaderMagic) return 0UL;
  if (!name || *name == 0) return 0LL;

  FileStream file(name, kRestrictWRB);

  if (!file.Leak()) return 0LL;

  Ref<Int64> ret = file.Write(data->fOffset, data, sizeof(SwapDiskHdr) + data->fBlobSz);

  return ret.Leak();
}

/***********************************************************************************/
/// @brief Read memory chunk from disk.
/// @param data the data packet length.
/// @return Whether the swap was fetched to disk, or not.
/***********************************************************************************/
SwapDiskHdrPtr IDiskSwap::Read(const UIntPtr& offset, SizeT data_len, const Char* name) {
  if (data_len == 0UL) return nullptr;
  if (!name || *name == 0) return nullptr;

  FileStream file(name, kRestrictRB);

  if (!file.Leak()) return nullptr;

  ErrorOrAny blob = file.Read(offset, sizeof(SwapDiskHdr) + data_len);

  if (blob.HasError() || !blob.Leak().Leak() ||
      (static_cast<SwapDiskHdr*>(blob.Leak().Leak()))->fMagic != kSwapDiskHeaderMagic) {
    if (!blob.HasError()) mm_free_ptr(blob.Leak().Leak());
    return nullptr;
  }

  return reinterpret_cast<SwapDiskHdr*>(blob.Leak().Leak());
}

}  // namespace Ne::Kernel
