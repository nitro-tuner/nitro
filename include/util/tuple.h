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

template<typename T>
struct is_reference {
  static const bool value = false;
};

template<typename _T>
struct is_reference<_T&> {
  static const bool value = true;
};

template<typename SHT, typename STT>
struct contains_ref {
  static const bool value = is_reference<SHT>::value ||
    contains_ref<typename STT::head_type, typename STT::tail_type>::value;
};

template<typename T>
struct contains_ref<T, thrust::null_type> {
  static const bool value = is_reference<T>::value;
};

template<typename T>
struct tuple_ref_check {
  typedef typename thrust::detail::eval_if<
    contains_ref<typename T::head_type, typename T::tail_type>::value,
    thrust::detail::identity_<void *>,
    thrust::detail::identity_<T> >::type type;
};

} // namespace nitro
