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

/*! \file constraint_type.h
 *  \brief Constraint Type Definitions
 */

#pragma once

namespace nitro {

struct constraint_type {
  virtual bool operator()(const std::vector<double>&) = 0;
};

template<bool enable, typename evaluator>
struct evaluate_constraints {

  static inline bool impl(const std::vector<constraint_type*>& constraints,
                          const std::vector<double>& features)
  { return evaluator::impl(constraints, features); }
};

template<typename evaluator>
struct evaluate_constraints<false, evaluator> {

  static inline bool impl(void *, const std::vector<double>& features)
  { return true; }
};

namespace detail {

template<bool enable, typename CVType>
struct constraint_wrapper {
  typedef std::map<CVType*, std::vector<constraint_type *> > map_type;

  static void add(CVType *cv, constraint_type *constraint, map_type& cmap)
  { cmap[cv].push_back(constraint); }

  static const std::vector<constraint_type *>& get(map_type& cmap, CVType *cv)
  { return cmap[cv]; }
};

template<typename CVType>
struct constraint_wrapper<false, CVType> {
  typedef void *map_type;

  static void add(CVType *, constraint_type *, map_type&) {}
  static void *get(map_type&, CVType *) { return nullptr; }
};

} // namespace detail
} // namespace nitro
