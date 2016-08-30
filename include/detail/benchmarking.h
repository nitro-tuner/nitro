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
namespace detail {

template<typename VariantType, typename CVType, bool ENABLE>
struct benchmarker {
  typedef void * VariantIndicesType;

  static void report_stats(context& cx,
                           VariantType *variant,
                           size_t index,
                           const std::vector<double>& fv) {}
  static void report_stats(context& cx,
                           VariantType *variant,
                           VariantIndicesType variant_indices,
                           CVType *selected_variant,
                           const std::vector<double>& fv) {}
  static void update_indices(VariantIndicesType variant_indices,
                             CVType *variant,
                             size_t index) {}
};

template<typename VariantType, typename CVType>
struct benchmarker<VariantType, CVType, true> {
  typedef std::map<CVType *, size_t> VariantIndicesType;

  static void report_stats(context& cx,
                           VariantType *variant,
                           size_t index,
                           const std::vector<double>& fv) {
    cx.report_stats(variant, index, fv);
  }
  static void report_stats(context& cx,
                           VariantType *variant,
                           VariantIndicesType variant_indices,
                           CVType *selected_variant,
                           const std::vector<double>& fv) {
    cx.report_stats(variant, variant_indices[selected_variant], fv);
  }
  static void update_indices(VariantIndicesType variant_indices,
                             CVType *variant,
                             size_t index) {
    variant_indices[variant] = index;
  }
};

} // namespace detail
} // namespace nitro
