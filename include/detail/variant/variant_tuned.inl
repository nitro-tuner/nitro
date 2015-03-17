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

/*! \file variant_tuned.inl
 *  \brief code_variant specialization for tuned variant execution
 */

#pragma once

#include <cassert>
#include "detail/benchmarking.h"
#include "util/mpl.h"

namespace nitro {

template<typename Policy, typename T, typename D>
struct code_variant<Policy, T, D, RUN_TUNED> {
  //typedef Policy tuning_policy;
  typedef std::string result_type;
  typedef detail::variant_type_base<T> cv_type;
  typedef detail::input_feature_type_base<T> if_type;
  typedef typename Policy::feature_evaluator feature_evaluator_type;
  typedef typename Policy::constraint_evaluator constraint_evaluator_type;
  typedef detail::benchmarker<code_variant<Policy, T, D>, cv_type, Policy::benchmark_mode> bench;
  typedef detail::constraint_wrapper<Policy::enable_constraints, cv_type> constraint_wrapper;

  typename Policy::classifier_type m_classifier;

  const std::string m_name;
  typename bench::VariantIndicesType m_variant_indices;
  std::vector<cv_type *> m_variants;
  std::vector<if_type *> m_input_features;
  std::vector<double> m_cached_fv;
  std::vector<double> m_unscaled_fv;
  typename constraint_wrapper::map_type m_constraints;
  cv_type *m_default_variant;
  T m_fixed_args;
  context& m_context;

  code_variant(context& cx) : m_name(Policy::variant_name),
                              m_default_variant(NULL),
                              m_context(cx) {
    m_context.register_variant(this, m_name);
    m_classifier = m_context.load_model<typename Policy::classifier_type>(this);
  }

  ~code_variant() {
    m_context.unregister_variant(this);
  }

  template<typename VT>
  void add_variant(VT *variant) {
    bench::update_indices(m_variant_indices, variant, m_variants.size());
    m_variants.push_back(static_cast<cv_type*>(variant));
    if(m_default_variant == NULL) m_default_variant = variant;
  }

  void set_default(cv_type *variant) {
    m_default_variant = variant;
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

    std::vector<double> feature_values;
    feature_evaluator_type::impl(m_input_features, args, feature_values);

    mpl::copy_if<Policy::benchmark_mode || Policy::enable_constraints>::impl(feature_values, m_unscaled_fv);
    cv_type *selected_variant;

    // XXX: Dirty workaround for nvcc.
    // Ideally should be query<...>()
    size_t idx = m_classifier.query(feature_values, static_cast<typename Policy::feature_scaler*>(0));
    selected_variant = m_variants[idx];

    bool run = evaluate_constraints<Policy::enable_constraints,
                 constraint_evaluator_type>::impl
                   (constraint_wrapper::get(m_constraints, selected_variant),
                    m_unscaled_fv);

    if(run) {
      bench::report_stats(m_context, this, idx, m_unscaled_fv);
    } else {
      selected_variant = m_default_variant;
      bench::report_stats(m_context, this, m_variant_indices, selected_variant, m_unscaled_fv);
    }
    return nitro::apply_from_tuple(*selected_variant, args);
  }
};

} // namespace nitro

