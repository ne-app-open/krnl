// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_KT_SOURCELOCATION_H
#define FW_HEADERS_KT_SOURCELOCATION_H

#include <KernelTest/headers/Foundation.h>

class KTSourceLocation;

/// ================================================================================
/// @brief SourceLocation class for Ne::Kernel Test Framework.
/// ================================================================================
class KTSourceLocation _FINAL CF_OBJECT {
 public:
  KTSourceLocation()           = delete;
  ~KTSourceLocation() override = default;

  LIBSYS_COPY_DELETE(KTSourceLocation);

 public:
  KTSourceLocation(const Char*, const SInt32 = 0UL);

  CF::CFString& File() { return mFile; }
  SInt32        Line() { return mLine; }

  CF::CFString operator()();

 private:
  CF::CFString mFile{4096};
  SInt32       mLine{0U};
};

#endif
