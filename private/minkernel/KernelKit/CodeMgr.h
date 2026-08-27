// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_CODEMGR_H
#define KERNELKIT_CODEMGR_H

#include <KernelKit/CoreProcessScheduler.h>
#include <KernelKit/IPEFDylibObject.h>
#include <KernelKit/PE32CodeMgr.h>
#include <KernelKit/PEFCodeMgr.h>

/// @file CodeMgr.h
/// @brief Code Manager header file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

namespace Ne::Kernel {

/// @brief Image process entrypoint.
typedef void (*rtl_start_kind)(void);

typedef void (*rtl_kstart_kind)(VoidPtr);

/// @brief C++ Constructor entrypoint.
typedef void (*rtl_cxx_ctor_kind)(void);

/// @brief C++ Destructor entrypoint.
typedef void (*rtl_cxx_dtor_kind)(void);

/// @brief Executes a new process from a function. Ne::Kernel code only.
/// @note This sets up a new stack, anything on the main function that calls the Ne::Kernel will not
/// be accessible.
/// @param main the start of the process.
/// @return The team's process id.
BOOL rtl_create_kernel_task(KernelTask& main, const KID& kid);

/// @brief Executes a new process from a function. User code only.
/// @note This sets up a new stack, anything on the main function that calls the Ne::Kernel will not
/// be accessible.
/// @param main the start of the process.
/// @return The team's process id.
ProcessID rtl_create_user_process(rtl_start_kind main, const Char* process_name);

}  // namespace Ne::Kernel

#endif
