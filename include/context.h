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

/*! \file context.h
 *  \brief Nitro Context Class
 */
#pragma once

#include <string>
#include <vector>
#include <map>
#include "nitro_modes.h"
#include "util/error.h"

namespace nitro {
namespace detail {

template<typename T, typename D, bool ENABLE>
struct tuned_variant_reporter {
  static void impl(const std::string& name,
                   size_t index,
                   std::vector<double>& fv) {
  }
};

template<typename T, typename D>
struct tuned_variant_reporter<T, D, true> {
  static void impl(const std::string& name,
                   size_t index,
                   std::vector<double>& fv) {

    FILE *fp_perf;
    if((fp_perf = fopen((name + ".tsperf").c_str(), "a")) == NULL) {
      fprintf(stderr, "Fatal Error: Could not open perf file %s\n",
          ((name + ".tsperf").c_str()));
      exit(-1);
    }
    fprintf(fp_perf, "%lu : ", index);
    for(size_t i=0; i<fv.size(); ++i) {
      fprintf(fp_perf, "%f ", fv[i]);
    }
    fprintf(fp_perf, "\n");
    fclose(fp_perf);
  }
};

} // namespace detail

template<typename Policy, typename T,
         typename Device, int Mode>
struct code_variant;

template<int tuning_mode>
struct variant_info {};

template<>
struct variant_info<RUN_CODE_PATH> {
  std::string m_name;
  size_t m_idx;
  std::vector<std::vector<double> > m_feature_vec;
  std::vector<std::string> m_perf;
};

template<>
struct variant_info<RUN_TUNED> {
  std::string m_name;
  size_t m_idx;
  std::vector<double> m_feature_vec;
};

template<typename Policy, typename Classifier,
         bool in_source = Policy::load_model_from_source>
struct model_loader {
  static Classifier impl(const std::string& model_file,
                         const std::string& scaling_file) {
    Classifier classifier;
    classifier.load_model_from_source(Policy::model, Policy::range);

    return classifier;
  }
};

template<typename Policy, typename Classifier>
struct model_loader<Policy, Classifier, false> {
  static Classifier impl(const std::string& model_file,
                         const std::string& scaling_file) {
    Classifier classifier;
    classifier.load_from_file(model_file, scaling_file);

    return classifier;
  }
};

struct context {
  typedef nitro::tuning_policies::__global_policy GlobalPolicy;

  std::map<void*, variant_info<RUN_CODE_PATH>> m_path_variants;
  std::map<void*, variant_info<RUN_TUNED>> m_tuned_variants;
  // For TBB
  typename GlobalPolicy::feature_evaluator_init m_fe_init;

  template<typename Policy, typename T, typename D>
  void register_variant(code_variant<Policy, T, D, RUN_CODE_PATH> *variant,
                        const std::string name) {
    variant_info<RUN_CODE_PATH> info;

    FILE *fp_idx;
    if((fp_idx = fopen((name + ".cp").c_str(), "r")) == NULL) {
      fprintf(stderr, "Fatal Error: Could not open code path file %s\n",
          ((name + ".cp").c_str()));
      exit(-1);
    }
    if(fscanf(fp_idx, "%lu", &info.m_idx) < 1)
      NITRO_FATAL("Error reading variant index from code path file.");

    if(info.m_idx < 0)
      NITRO_FATAL("Negative variant index specified")

    fclose(fp_idx);

    info.m_name = name;
    m_path_variants[variant] = info;
  }

  template<typename Policy, typename T, typename D>
  void register_variant(code_variant<Policy, T, D, RUN_TUNED> *variant,
                        const std::string name) {
    variant_info<RUN_TUNED> info;

    info.m_name = name;
    m_tuned_variants[variant] = info;
  }

  template<typename Policy, typename T, typename D>
  void unregister_variant(code_variant<Policy, T, D, RUN_CODE_PATH> *variant) {
    FILE *fp_perf;

    if((fp_perf = fopen((m_path_variants[variant].m_name + ".perf").c_str(), "a")) == NULL) {
      fprintf(stderr, "Fatal Error: Could not open perf file %s\n",
          ((m_path_variants[variant].m_name + ".perf").c_str()));
      exit(-1);
    }

    for(size_t i=0; i<m_path_variants[variant].m_perf.size(); ++i) {
      fprintf(fp_perf, "%lu : ", m_path_variants[variant].m_idx);
      for(size_t j=0; j<m_path_variants[variant].m_feature_vec[i].size(); ++j) {
        fprintf(fp_perf, "%f ", m_path_variants[variant].m_feature_vec[i][j]);
      }
      fprintf(fp_perf, ": %s\n", m_path_variants[variant].m_perf[i].c_str());
    }
    fclose(fp_perf);
  }

  template<typename Policy, typename T, typename D>
  void unregister_variant(code_variant<Policy, T, D, RUN_TUNED> *variant) {
    detail::tuned_variant_reporter<T, D, Policy::benchmark_mode>::impl(
        m_tuned_variants[variant].m_name,
        m_tuned_variants[variant].m_idx,
        m_tuned_variants[variant].m_feature_vec);
  }

  template<typename Policy, typename T, typename D>
  size_t get_variant_index(code_variant<Policy, T, D, RUN_CODE_PATH> *variant) {
    return m_path_variants[variant].m_idx;
  }

  template<typename Policy, typename T, typename D>
  void report_stats(code_variant<Policy, T, D, RUN_CODE_PATH> *variant,
                    const size_t index,
                    std::vector<double> features,
                    std::string perf) {
    m_path_variants[variant].m_feature_vec.push_back(features);
    m_path_variants[variant].m_perf.push_back(perf);
  }

  template<typename Policy, typename T, typename D>
  void report_stats(code_variant<Policy, T, D, RUN_TUNED> *variant,
                    const size_t index,
                    const std::vector<double>& fv) {
    m_tuned_variants[variant].m_idx = index;
    std::copy(fv.begin(), fv.end(),
              std::back_inserter(m_tuned_variants[variant].m_feature_vec));
  }

  template<typename Classifier, typename Policy, typename T, typename D>
  Classifier load_model(code_variant<Policy, T, D, RUN_TUNED> *variant) {
    std::string model_file_name = std::string("models/") + m_tuned_variants[variant].m_name + std::string(".model");
    std::string scaling_file_name = std::string("models/") + m_tuned_variants[variant].m_name + std::string(".range");

    return model_loader<Policy, Classifier>::impl(model_file_name, scaling_file_name);
  }
};

} // namespace nitro
