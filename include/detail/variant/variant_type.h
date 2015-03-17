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

/*! \file variant_type.h
 *  \brief Variant Functor Base Class
 */

#pragma once

namespace nitro {
namespace detail {

template<typename T>
struct variant_type_base {
  typedef std::string result_type;
};

template<typename T0>
struct variant_type_base<thrust::tuple<T0> > {
  typedef std::string result_type;
  virtual std::string operator()(T0) = 0;
};

template<typename T0, typename T1>
struct variant_type_base<thrust::tuple<T0, T1> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1) = 0;
};

template<typename T0, typename T1, typename T2>
struct variant_type_base<thrust::tuple<T0, T1, T2> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2) = 0;
};

template<typename T0, typename T1, typename T2, typename T3>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4, T5) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4, T5, T6) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4, T5, T6, T7) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7, typename T8>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4, T5, T6, T7, T8) = 0;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7, typename T8, typename T9>
struct variant_type_base<thrust::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
  typedef std::string result_type;
  virtual std::string operator()(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) = 0;
};

} // namespace detail

  template<typename... Args>
  struct variant_type :
    detail::variant_type_base
    <decltype(thrust::make_tuple(Args{}...))> {};

} // namespace nitro
