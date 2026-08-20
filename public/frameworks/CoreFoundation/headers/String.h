// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_STRING_H
#define FW_HEADERS_CF_STRING_H

#include <CoreFoundation/headers/Object.h>
#include <CoreFoundation/headers/Ref.h>

namespace CF {
class CFString;
class CFStringBuilder;

/// ================================================================================
/// @brief CFString class for string manipulations.
/// ================================================================================
class CFString _FINAL CF_OBJECT {
 public:
  CFString() = delete;

  explicit CFString(const SizeT sz);
  explicit CFString(const Char* str);

  ~CFString();

  const char* asBytes() const;

  LIBSYS_COPY_DELETE(CFString);

 private:
  Char* mStr{nullptr};
};

/// ================================================================================
/// @brief CFStringBuilder class for string manipulations.
/// ================================================================================
class CFStringBuilder final {
 public:
  static CFRef<CFString> Construct(const Char*);
  static const Char*     FromBool(const Char*, BOOL);
  static const Char*     Format(const Char*, const Char*);

  static BOOL Equals(const Char, const Char*);
  static BOOL Equals(const Char*, const Char*);
};
}  // namespace CF

#endif
