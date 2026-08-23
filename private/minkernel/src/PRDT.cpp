// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/DebugOutput.h>
#include <NeKit/KString.h>
#include <StorageKit/PRDT.h>

namespace Ne::Kernel {
  
/***********************************************************************************/
/// @brief constructs a new PRD.
/// @param prd PRD reference.
/// @note This doesnt construct a valid, please fill it by yourself.
/***********************************************************************************/
Void sk_construct_prdt(Ref<PRDT>& prd) {
  if (!prd.Leak().fSectorCount) return;
  
  prd.Leak().fPhysAddress = 0x0;
  prd.Leak().fSectorCount = 0x0;
  prd.Leak().fEndBit      = 0x0;
}

}  // namespace Ne::Kernel
