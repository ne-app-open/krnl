// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_ARRAY_H
#define FW_HEADERS_CF_ARRAY_H

#include <SystemKit/System.h>

namespace CF {
template <class T, SizeT N>
class CFArray _FINAL {
 public:
  explicit CFArray() = default;
  ~CFArray()         = default;

  CFArray& operator=(const CFArray&) = default;
  CFArray(const CFArray&)            = default;

  T& operator[](SizeT at) {
    MUST_PASS(at < this->Count());
    return fArray[at];
  }

  Bool Empty() { return this->Count() > 0; }

  SizeT Capacity() { return N; }

  SizeT Count() {
    auto cnt = 0UL;

    for (auto i = 0UL; i < N; ++i) {
      if (fArray[i]) ++cnt;
    }

    return cnt;
  }

  const T* CData() { return fArray; }

  operator bool() { return !Empty(); }

 private:
  T fArray[N] = {nullptr};
};

template <typename ValueType>
inline auto make_array(ValueType val) {
  return CFArray<ValueType, ARRAY_SIZE(val)>{val};
}
}  // namespace CF

#endif
