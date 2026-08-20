// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_REF_H
#define FW_HEADERS_CF_REF_H

#include <CoreFoundation/headers/Object.h>
#include <SystemKit/System.h>

namespace CF {
template <class T>
class CFRef;

/// ================================================================================
/// @brief CFRef class for reference counting.
/// ================================================================================
template <class T>
class CFRef _FINAL CF_OBJECT {
 public:
  CFRef() = default;

  ~CFRef() {
    if (MmGetHeapFlags(fClass) != -1) delete fClass;
  }

 public:
  CFRef(T* cls) : fClass(cls) {}

  CFRef(T cls) : fClass(&cls) {}

  CFRef& operator=(T ref) {
    if (!fClass) return *this;

    fClass = &ref;
    return *this;
  }

 public:
  T operator->() const {
    MUST_PASS(*fClass);
    return *fClass;
  }

  T& Leak() { return *fClass; }

  T& TryLeak() const {
    MUST_PASS(*fClass);
    return *fClass;
  }

  T operator*() { return *fClass; }

  operator bool() { return fClass; }

 private:
  T* fClass{nullptr};
};

/// ================================================================================
/// @brief CFNonNullRef class for non-null reference counting.
/// ================================================================================
template <typename T>
class CFNonNullRef final CF_OBJECT {
 public:
  CFNonNullRef()        = delete;
  CFNonNullRef(nullPtr) = delete;

  CFNonNullRef(T* ref) : fRef(ref) { MUST_PASS(ref); }
  CFNonNullRef(CFRef<T> ref) : fRef(ref) { MUST_PASS(ref); }

  CFRef<T>& operator->() {
    MUST_PASS(fRef);
    return fRef;
  }

  CFNonNullRef& operator=(const CFNonNullRef<T>& ref) = default;
  CFNonNullRef(const CFNonNullRef<T>& ref)            = default;

 private:
  CFRef<T> fRef{nullptr};
};
}  // namespace CF

#endif
