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

#include <map>
#include "util/apply_from_tuple.h"

namespace nitro {
namespace feature_evaluators {

struct serial_feature_evaluator {

  template<typename T, typename feature_function_type>
  static inline void impl(const std::vector<feature_function_type *>& features,
                          const T& args,
                          std::vector<double>& feature_values) {
    for(size_t i=0; i<features.size(); ++i) {
      feature_values.push_back(nitro::apply_from_tuple(*features[i], args));
    }
  }
};

} // namespace feature_evaluators

namespace feature_scalers {

struct serial_feature_scaler {

  static inline void impl(std::vector<double>& features,
                          //const std::vector<std::pair<double, double> >& ranges,
                          std::map<size_t, std::pair<double, double> >& ranges,
                          double scale_lower, double scale_upper) {
    for(size_t i=0; i<features.size(); ++i) {
      if(ranges.count(i) == 0)
        continue;
      if(ranges[i].first == features[i]) {
        features[i] = scale_lower;
      } else if(ranges[i].second == features[i]) {
        features[i] = scale_upper;
      } else {
        features[i] = scale_lower + (scale_upper - scale_lower) *
          (features[i] - ranges[i].first) /
          (ranges[i].second - ranges[i].first);
      }
    }
  }
};

} // namespace feature_scalers
} // namespace nitro
