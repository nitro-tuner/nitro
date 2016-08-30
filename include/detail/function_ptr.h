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

template<typename result_type, typename T>
struct function_ptr {
  typedef void type;
};

template<typename result_type, typename T0>
struct function_ptr<result_type, thrust::tuple<T0> > {
  typedef result_type (*type)(T0);
};

template<typename result_type, typename T0, typename T1>
struct function_ptr<result_type, thrust::tuple<T0,T1> > {
  typedef result_type (*type)(T0, T1);
};

template<typename result_type, typename T0, typename T1, typename T2>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2> > {
  typedef result_type (*type)(T0, T1, T2);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3> > {
  typedef result_type (*type)(T0, T1, T2, T3);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4,T5> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4, T5);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4,T5,T6> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4, T5, T6);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4,T5,T6,T7> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4, T5, T6, T7);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7, typename T8>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
};

template<typename result_type, typename T0, typename T1, typename T2, typename T3,
         typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
struct function_ptr<result_type, thrust::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> > {
  typedef result_type (*type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
};

} // namespace detail
} // namespace nitro

