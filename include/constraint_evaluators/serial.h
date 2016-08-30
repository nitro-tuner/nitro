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

/*! \file serial.h
 *  \brief Sequential Constraint Evaluator
 */

#pragma once

#include <detail/constraint/constraint_type.h>

namespace nitro {
namespace constraint_evaluators {

struct serial_constraint_evaluator {

  static inline bool impl(const std::vector<constraint_type *>& constraints,
                          const std::vector<double>& features) {
    bool run = true;
    for(size_t i=0; i<constraints.size(); ++i) {
      run &= (*constraints[i])(features);
    }
    return run;
  }
};

} // namespace constraint_evaluators
} // namespace nitro
