// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_DICTIONARY_H
#define FW_HEADERS_CF_DICTIONARY_H

#include <SystemKit/System.h>

namespace CF {

/// @brief The following class makes a dictionary of values, based on their keys.
template <class Key, class Value>
class CFDictionary _FINAL {
 public:
  explicit CFDictionary() = default;
  ~CFDictionary()         = default;

  CFDictionary& operator=(const CFDictionary&) = default;
  CFDictionary(const CFDictionary&)            = default;

  Value& operator[](Key& at) { return this->Find(at); }

  Bool Empty() { return this->fCount > 0; }

  Bool Find(Key& key);
  Bool Find(const Key& key);

  operator bool() { return !this->Empty(); }

 private:
  SizeT fCount{0UL};
};

template <typename KeyType, typename ValueType>
inline auto make_dict() {
  return CFDictionary<KeyType, ValueType>{};
}

template <typename K, V>
using CFDict = CFDictionary<K, V>;

}  // namespace CF

#endif
