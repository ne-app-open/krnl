// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_LAUNCH_FOUNDATION_H
#define FW_HEADERS_LAUNCH_FOUNDATION_H

#include <CoreFoundation/headers/Foundation.h>
#include <CoreFoundation/headers/String.h>

namespace LaunchHelpers {
struct LHLaunchInfo;

inline constexpr auto kMaxPath = 4096;
inline constexpr auto kMaxArgs = 256;

/// @brief Launch information structure.
/// @note This structure is read-only. Modyfing its members wo't have any effect.
struct LHLaunchInfo _FINAL {
  CF::CFString            fExecutablePath{kMaxPath};
  CF::CFString            fWorkingDirectory{kMaxPath};
  CF::CFRef<CF::CFString> fArguments[kMaxArgs];
  CF::CFString            fEnvironment{kMaxPath};
  CF::CFInteger64         fUID{0};
  CF::CFInteger64         fGID{0};

  LHLaunchInfo()  = default;
  ~LHLaunchInfo() = default;

  LIBSYS_COPY_DELETE(LHLaunchInfo)

  explicit operator bool() { return fUID && fGID; }

  CF::CFRef<CF::CFString>* begin() { return fArguments; }
  CF::CFRef<CF::CFString>* end() { return fArguments + kMaxArgs; }

  SizeT size() { return kMaxArgs; }
};

using LHLaunchInfoPtr = LHLaunchInfo*;

/// @brief Get launch information.
/// @return the launch information structure.
CF::CFRef<LHLaunchInfo> LHGetLaunchInfo(Void);
}  // namespace LaunchHelpers

#endif
