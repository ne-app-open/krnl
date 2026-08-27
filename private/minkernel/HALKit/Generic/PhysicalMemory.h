// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_PHYSICALMEMORY_H
#define KERNELKIT_PHYSICALMEMORY_H

#include <NeKit/Config.h>
#include <hint/CompilerHint.h>

#ifdef __NE_AMD64__
#include <HALKit/AMD64/Paging.h>
#endif  // __NE_AMD64__

#ifdef __NE_ARM64__
#include <HALKit/ARM64/Paging.h>
#endif  // __NE_AMD64__

namespace Ne::Kernel::HAL {

/// @brief Hand a physical region to the frame allocator.
Void pmmi_init(UIntPtr base, SizeT sz);

/// @brief Take one zeroed frame, 0 when out of memory.
_Output UIntPtr pmmi_alloc_frame(Void);

/// @brief Give a frame back.
Void pmmi_free_frame(UIntPtr frame);

/// @brief Frames still available.
_Output SizeT pmmi_free_frames(Void);

/// @brief Frames handed out.
_Output SizeT pmmi_used_frames(Void);

}  // namespace Ne::Kernel::HAL

#endif /* ifndef KERNELKIT_PHYSICALMEMORY_H */
