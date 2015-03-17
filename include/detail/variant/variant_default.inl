/****************************************************************************
 *
 *   Copyright 2012-2015 University of Utah
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

/*! \file variant_default.inl
 *  \brief code_variant specialization for default variant execution
 */

#pragma once

#include <cassert>
#include "util/apply_from_tuple.h"

namespace nitro {

template<typename Policy, typename T, typename D>
struct code_variant<Policy, T, D, RUN_DEFAULT> {
  typedef std::string result_type;
  typedef detail::variant_type_base<T> cv_type;
  typedef detail::input_feature_type_base<T> if_type;

  const context& m_context;
  T m_fixed_args;
  cv_type *m_default_variant;
  const char *m_name;

  code_variant(const context& cx) : m_name(Policy::variant_name),
                                    m_default_variant(NULL),
                                    m_context(cx) {}

  void add_variant(cv_type *variant) {
    if(m_default_variant == NULL) {
      m_default_variant = variant;
    }
  }

  void set_default(cv_type *variant) {
    m_default_variant = variant;
  }

  void add_input_feature(if_type *) {}

  void add_constraint(cv_type *, constraint_type *) {}

  template<typename Arg0, typename... Args>
  result_type operator()(Arg0 arg0, Args... args)
  { return _dispatch(thrust::make_tuple(arg0, args...)); }

private:
  template<typename Args>
  result_type _dispatch(const Args& args) {
    return nitro::apply_from_tuple(*m_default_variant, args);
  }

  //code_variant() : m_name("") {}
};
} // namespace nitro

