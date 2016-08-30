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

/*! \file input_feature_type.h
 *  \brief Feature Functor Base Class
 */

#pragma once

namespace nitro {
namespace detail {

template<typename T>
struct input_feature_type_base {
  typedef double result_type;
};

template<typename T0>
struct input_feature_type_base<thrust::tuple<T0> > {
  typedef double result_type;
  virtual double operator()(T0) = 0;
};

template<typename T0, typename T1>
struct input_feature_type_base<thrust::tuple<T0, T1> > {
  typedef double result_type;
  virtual double operator()(T0, T1) = 0;
};

template<typename T0, typename T1, typename T2>
struct input_feature_type_base<thrust::tuple<T0, T1, T2> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2) = 0;
};

template<typename T0, typename T1, typename T2, typename T3>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4, T5) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4, T5, T6) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4, T5, T6, T7) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7, typename T8>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4, T5, T6, T7, T8) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7, typename T8, typename T9>
struct input_feature_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
  typedef double result_type;
  virtual double operator()(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) = 0;
};

} // namespace detail

  template<typename... Args>
  struct input_feature_type :
    detail::input_feature_type_base
    <decltype(thrust::make_tuple(Args{}...))> {};

} // namespace nitro
