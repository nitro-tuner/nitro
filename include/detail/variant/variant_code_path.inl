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

/*! \file variant_code_path.inl
 *  \brief code_variant specialization for code path execution
 */

#pragma once

#include <cassert>
#include <limits>

namespace nitro {

template<typename Policy, typename T, typename D>
struct code_variant<Policy, T, D, RUN_CODE_PATH> {
  typedef std::string result_type;
  typedef detail::variant_type_base<T> cv_type;
  typedef detail::input_feature_type_base<T> if_type;
  typedef typename Policy::feature_evaluator feature_evaluator_type;
  typedef typename Policy::constraint_evaluator constraint_evaluator_type;
  //typedef feature_evaluators::async_feature_evaluator<Policy> async_evaluator_type;
  typedef detail::constraint_wrapper<Policy::enable_constraints, cv_type> constraint_wrapper;

  context& m_context;
  std::vector<cv_type *> m_variants;
  std::vector<if_type *> m_input_features;
  std::vector<double> m_cached_fv;
  typename constraint_wrapper::map_type m_constraints;
  T m_fixed_args;
  const std::string m_name;
  size_t m_idx;

  code_variant(context& cx) : m_name(Policy::variant_name),
                              m_variants(),
                              m_input_features(),
                              m_context(cx) {
    m_context.register_variant(this, m_name);
    m_idx = m_context.get_variant_index(this);
  }

  ~code_variant() {
    m_context.unregister_variant(this);
  }

  template<typename VT>
  void add_variant(VT *variant) {
    m_variants.push_back(static_cast<cv_type*>(variant));
  }

  void set_default(cv_type *variant) {
  }

  void add_input_feature(if_type *feature) {
    m_input_features.push_back(feature);
  }

  void add_constraint(cv_type *variant, constraint_type *constraint) {
    constraint_wrapper::add(variant, constraint, m_constraints);
  }

  template<typename Arg0, typename... Args>
  result_type operator()(Arg0 arg0, Args... args)
  { return _dispatch(thrust::make_tuple(arg0, args...)); }

private:
  template<typename Args>
  result_type _dispatch(const Args& args) {
    assert(m_idx < m_variants.size());

    cv_type *selected_variant = m_variants[m_idx];

    std::vector<double> feature_values;
    feature_evaluator_type::impl(m_input_features, args, feature_values);

    bool run = evaluate_constraints<Policy::enable_constraints,
                 constraint_evaluator_type>::impl
                   (constraint_wrapper::get(m_constraints, selected_variant),
                    feature_values);

    if(!run) {
      fprintf(stderr, "[Nitro] Input constraint(s) violated\n");
      std::stringstream ss;
      double perf = (double)std::numeric_limits<int>::max();
      ss << perf;
      m_context.report_stats(this, m_idx, feature_values, ss.str());
      return ss.str();;
    }

    result_type perf = nitro::apply_from_tuple(*selected_variant, args);
    m_context.report_stats(this, m_idx, feature_values, perf);

    return perf;
  }
};
} // namespace nitro

