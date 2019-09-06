/******************************************************************************
 *
 *  Copyright 2019 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include "variable.h"

#include <stdio.h>
#include <sstream>

namespace bluetooth {
namespace packet {
namespace parser {
namespace test {

Variable::Variable(const std::string& str) : data(str) {}

void Variable::Serialize(BitInserter& bi) const {
  if (data.size() > 255) {
    fprintf(stderr, "data.size() > 255: (%zu)", data.size());
    abort();
  }
  bi.insert_byte((uint8_t)data.size());
  for (auto byte : data) {
    bi.insert_byte(byte);
  }
}

size_t Variable::size() const {
  return data.size() + 1;
}

Iterator<true> Variable::Parse(Variable* instance, Iterator<true> it) {
  if (it.NumBytesRemaining() < 1) {
    return it;
  }
  size_t data_length = it.extract<uint8_t>();
  if (data_length > 255) {
    return it + it.NumBytesRemaining();
  }
  if (it.NumBytesRemaining() < data_length) {
    return it + it.NumBytesRemaining();
  }
  std::stringstream ss;
  for (size_t i = 0; i < data_length; i++) {
    ss << it.extract<char>();
  }
  *instance = ss.str();
  return it;
}
}  // namespace test
}  // namespace parser
}  // namespace packet
}  // namespace bluetooth
