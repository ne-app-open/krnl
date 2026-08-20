// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_FOUNDATION_H
#define FW_HEADERS_CF_FOUNDATION_H

#include <SystemKit/System.h>

namespace CF {
class CFString;
union CFGuid;
class CFProperty;
class CFObject;

template <class T>
class CFRef;
class CFFont;
struct CFPoint;
struct CFRect;
struct CFColor;

#ifndef __CF_64BIT__
typedef SInt32 CFInteger;
typedef float  CFReal;
#else
typedef SInt64 CFInteger;
typedef double CFReal;
#endif

typedef SInt32 CFInteger32;
typedef SInt64 CFInteger64;

typedef Char     CFChar8;
typedef char16_t CFChar16;

struct CFPoint _FINAL {
  CFInteger64 x_1{0UL};
  CFInteger64 y_1{0UL};

  CFInteger64 x_2{0UL};
  CFInteger64 y_2{0UL};
  CFReal      ang{0UL};

  explicit operator bool();

  /// @brief Check if point is within the current CFPoint.
  /// @param point the current point to check.
  /// @retval true if point is within this point.
  /// @retval validations failed.
  bool IsWithin(CFPoint& point);
};

struct CFColor _FINAL {
  CFInteger64 r, g, b, a{0};
};

struct CFRect _FINAL {
  CFInteger64 x{0UL};
  CFInteger64 y{0UL};

  CFInteger64 width{0UL};
  CFInteger64 height{0UL};

  explicit operator bool();

  BOOL SizeMatches(CFRect& rect);
  BOOL PositionMatches(CFRect& rect);
};

union CFGuid _FINAL {
  alignas(8) UInt16 fU8[16];
  alignas(8) UInt16 fU16[8];
  alignas(8) UInt32 fU32[4];
  alignas(8) UInt64 fU64[2];

  struct {
    alignas(8) UInt32 fMs1;
    UInt16 fMs2;
    UInt16 fMs3;
    UInt8  fMs4[8];
  } fUuid;
};
}  // namespace CF

#endif
