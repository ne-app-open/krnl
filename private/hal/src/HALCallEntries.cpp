// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

/// For Entries Support.
#include <ArchKit/ArchKit.h>
#include <hal/HAL/HAL.h>

using Ne::Kernel::HAL::kRegisteredSystemCalls;

EXTERN_C SInt32 hali_add_entry(HAL::hal_proc_type proc, const UInt64& level, const SInt64& hash) {
  if (!hash) return -1;

  STATIC BOOL kLocked = NO;

  while (kLocked);

  kLocked = YES;

  for (Ne::Kernel::SizeT i = 0UL; i < kMaxDispatchCallCount; ++i) {
    if (kRegisteredSystemCalls[i].fActive) continue;

    kRegisteredSystemCalls[i].fHash      = hash;
    kRegisteredSystemCalls[i].fProc      = proc;
    kRegisteredSystemCalls[i].fActive    = YES;
    kRegisteredSystemCalls[i].fAuthLevel = level;

    kLocked = NO;

    return i;
  }

  kLocked = NO;

  return -1;
}

EXTERN_C Void hali_remove_entry(const SInt64& hash) {
  if (!hash) return;

  STATIC BOOL kLocked = NO;

  while (kLocked);

  kLocked = YES;

  for (Ne::Kernel::SizeT i = 0UL; i < kMaxDispatchCallCount; ++i) {
    if (kRegisteredSystemCalls[i].fHash != hash) continue;

    kRegisteredSystemCalls[i].fProc      = nullptr;
    kRegisteredSystemCalls[i].fHash      = 0;
    kRegisteredSystemCalls[i].fActive    = NO;
    kRegisteredSystemCalls[i].fAuthLevel = 0;

    kLocked = NO;

    return;
  }

  kLocked = NO;
}
