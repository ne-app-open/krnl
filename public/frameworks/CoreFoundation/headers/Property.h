// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_PROPERTY_H
#define FW_HEADERS_CF_PROPERTY_H

#include <CoreFoundation/headers/Foundation.h>
#include <CoreFoundation/headers/Ref.h>
#include <SystemKit/System.h>

#define kCFMaxPropLen (256U)

namespace CF {
class CFString;
class CFProperty;

template <class Cls, SizeT N>
class CFArray;

/// ================================================================================
/// @brief handle to anything (number, ptr, string...)
/// ================================================================================
using CFPropertyId = UIntPtr;

/// ================================================================================
/// @brief User property class.
/// @note /prop/foo or /prop/bar are properties.
/// ================================================================================
class CFProperty _FINAL CF_OBJECT {
 public:
  CFProperty(CFRef<CFGuid> guid, CFString& name, CFPropertyId value);
  ~CFProperty() override = default;

 public:
  CFProperty& operator=(const CFProperty&) = default;
  CFProperty(const CFProperty&)            = default;

  Bool          StringEquals(CFString& name);
  CFPropertyId& GetValue();
  CFString&     GetKey();

 private:
  CFString*     fName{nullptr};
  CFPropertyId  fValue{0UL};
  CFRef<CFGuid> fGUID{};
};

template <SizeT N>
using CFPropertyArray = CFArray<CFProperty, N>;
}  // namespace CF

#endif
