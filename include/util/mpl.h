/****************************************************************************
 *
 *   Copyright 2012-2016 University of Utah
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
****************************************************************************/

#pragma once

namespace nitro {
namespace mpl {

template<bool enable>
struct copy_if {

  template<typename T>
  static void impl(const std::vector<T>& src,
                   std::vector<T>& dest) {}
};

template<>
struct copy_if<true> {

  template<typename T>
  static void impl(const std::vector<T>& src,
                   std::vector<T>& dest) {
    dest.clear();
    std::copy(src.begin(), src.end(), std::back_inserter(dest));
  }
};

} // namespace mpl
} // namespace nitro
