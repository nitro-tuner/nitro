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

/*! \file wrappers.h
 *  \brief Binders for variant and feature function pointers
 */

#pragma once

#include <detail/function_ptr.h>

namespace nitro {
namespace detail {

  template<typename base_t,
           typename T0,
           typename T1=thrust::null_type,
           typename T2=thrust::null_type,
           typename T3=thrust::null_type,
           typename T4=thrust::null_type,
           typename T5=thrust::null_type,
           typename T6=thrust::null_type,
           typename T7=thrust::null_type,
           typename T8=thrust::null_type,
           typename T9=thrust::null_type>
  struct wrapper_base : base_t {
    typedef thrust::tuple<T0, T1, T2, T3, T4,
                          T5, T6, T7, T8, T9> arg_tuple;
    typedef typename base_t::result_type result_type;
    typedef typename detail::function_ptr<result_type, arg_tuple>::type F;

    F m_fn;

    wrapper_base(F f) : m_fn(f) {}

    result_type operator()(T0 t0)
    { return _dispatch(thrust::make_tuple(t0)); }

    result_type operator()(T0 t0, T1 t1)
    { return _dispatch(thrust::make_tuple(t0, t1)); }

    result_type operator()(T0 t0, T1 t1, T2 t2)
    { return _dispatch(thrust::make_tuple(t0, t1, t2)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4,
                           T5 t5)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4, t5)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4,
                           T5 t5, T6 t6)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4, t5, t6)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4,
                           T5 t5, T6 t6, T7 t7)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4, t5, t6, t7)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4,
                           T5 t5, T6 t6, T7 t7, T8 t8)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4, t5, t6, t7, t8)); }

    result_type operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4,
                           T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    { return _dispatch(thrust::make_tuple(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9)); }

  private:
    template<typename T>
    result_type _dispatch(const T& args)
    { return nitro::apply_from_tuple(m_fn, args); }
  };

} // namespace detail

template<typename... Args>
struct wrap_variant : detail::wrapper_base<
                      variant_type<Args...>, Args...> {

  typedef detail::wrapper_base<variant_type<Args...>, Args...> parent;
  typedef typename parent::F F;

  wrap_variant(F f) : parent(f) {}
};

template<typename... Args>
struct wrap_feature : detail::wrapper_base<
                      input_feature_type<Args...>, Args...> {

  typedef detail::wrapper_base<input_feature_type<Args...>, Args...> parent;
  typedef typename parent::F F;

  wrap_feature(F f) : parent(f) {}
};

} // namespace nitro
