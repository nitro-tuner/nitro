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

#pragma once

#include <thrust/tuple.h>
#include <type_traits>

namespace nitro {

template<typename F>
struct extract_result_type {
  typedef typename F::result_type type;
};

template<typename R, typename... Args>
struct extract_result_type<R (*)(Args...)>
{ typedef R type; };

template<typename R, typename... Args>
struct extract_result_type<R (*const)(Args...)>
{ typedef R type; };

template<typename F, typename T0>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0>& t) {
  return fn(thrust::get<0>(t));
}

template<typename F, typename T0, typename T1>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t));
}

template<typename F, typename T0, typename T1, typename T2>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4, T5>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t),
            thrust::get<5>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4, T5, T6>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t),
            thrust::get<5>(t),
            thrust::get<6>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t),
            thrust::get<5>(t),
            thrust::get<6>(t),
            thrust::get<7>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7, typename T8>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t),
            thrust::get<5>(t),
            thrust::get<6>(t),
            thrust::get<7>(t),
            thrust::get<8>(t));
}

template<typename F, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7, typename T8,
         typename T9>
inline typename extract_result_type<F>::type
apply_from_tuple(F& fn,
                 const thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& t) {
  return fn(thrust::get<0>(t),
            thrust::get<1>(t),
            thrust::get<2>(t),
            thrust::get<3>(t),
            thrust::get<4>(t),
            thrust::get<5>(t),
            thrust::get<6>(t),
            thrust::get<7>(t),
            thrust::get<8>(t),
            thrust::get<9>(t));
}

} // namespace nitro
