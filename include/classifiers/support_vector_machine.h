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

#include <vector>
#include <map>
#include <sstream>
#include <util/svm.h>

#include <util/error.h>

namespace nitro {
namespace classifiers {

template<bool feature_scaling=true>
struct svm {
  svm_model *m_model;
  //std::vector<std::pair<double, double> > m_ranges;
  std::map<size_t, std::pair<double, double> > m_ranges;
  double m_scale_lower, m_scale_upper;

  svm() : m_model(NULL) {}

  ~svm() {
    //if(m_model) { svm_free_and_destroy_model(&m_model); }
  }

  // XXX: Dirty workaround for nvcc.
  // Second argument should not be there
  template<typename feature_scaler>
  size_t query(std::vector<double>& features, feature_scaler *fs=0) {
    if(m_ranges.size() > 0) {
      feature_scaler::impl(features, m_ranges, m_scale_lower, m_scale_upper);
    }

    struct svm_node *x = new svm_node[features.size()+1];
    size_t i;

    for(i=0; i<features.size(); ++i) {
      x[i].index = i+1;
      x[i].value = features[i];
    }
    x[i].index = -1;

    size_t predicted_idx = (size_t)(svm_predict(m_model, x));
    delete x;

    return predicted_idx;
  }

  void load_from_file(const std::string& model_file,
                      const std::string& scaling_file) {
    m_model = svm_load_model(model_file.c_str());
    if(!m_model) {
      fprintf(stderr,
          "[Nitro] Fatal Error: Classification model from %s could not be loaded\n",
          model_file.c_str());
      exit(-1);
    }

    FILE *fp_scaling = fopen(scaling_file.c_str(), "r");
    if(!fp_scaling) {
      fprintf(stderr, "[Nitro] Warning: No Scaling File Found\n");
      return;
    }

    _read_scaling_file(fp_scaling);
    fclose(fp_scaling);
  }

  void load_model_from_source(const char *model_string,
                              const char *range_string) {
    m_model = svm_load_model_from_string(model_string);
    _read_range_from_string(range_string);
  }

private:
  void _read_scaling_file(FILE *fp_scaling) {
    char c; unsigned idx;
    double lower, upper;

    if(fscanf(fp_scaling, "%c", &c) < 1)
      NITRO_FATAL("C value could not be read.");

    if(fscanf(fp_scaling, "%lf %lf", &m_scale_lower, &m_scale_upper) < 2)
      NITRO_FATAL("Could not read scaling bounds.")

    while(fscanf(fp_scaling, "%u %lf %lf", &idx, &lower, &upper) != EOF) {
      //m_ranges.push_back(std::make_pair(lower, upper));
      m_ranges[idx-1] = std::make_pair(lower, upper);
    }
  }

  void _read_range_from_string(const char *range_string) {
    using namespace std;

    char c; unsigned idx;
    double lower, upper;

    // TODO: stringstream can be quite slow. Speed this up!
    stringstream ss(range_string);
    ss >> c >> m_scale_lower >> m_scale_upper;

    while(ss >> idx >> lower >> upper) {
      //m_ranges.push_back(std::make_pair(lower, upper));
      m_ranges[idx-1] = std::make_pair(lower, upper);
    }
  }
};

template<>
struct svm<false> {
  svm_model *m_model;

  svm() : m_model(NULL) {}

  ~svm() {
    //if(m_model) { svm_free_and_destroy_model(&m_model); }
  }

  // XXX: Dirty workaround for nvcc.
  // Second argument should not be there
  template<typename feature_scaler>
  size_t query(std::vector<double>& features, feature_scaler *fs=0) const {
    struct svm_node *x = new svm_node[features.size()+1];
    size_t i;

    for(i=0; i<features.size(); ++i) {
      x[i].index = i+1;
      x[i].value = features[i];
    }
    x[i].index = -1;

    size_t predicted_idx = (size_t)(svm_predict(m_model, x));
    delete x;

    return predicted_idx;
  }

  void load_from_file(const std::string& model_file,
                      const std::string& scaling_file) {
    m_model = svm_load_model(model_file.c_str());
    if(!m_model) {
      fprintf(stderr,
          "[Nitro] Fatal Error: Classification model from %s could not be loaded\n",
          model_file.c_str());
      exit(-1);
    }
  }

  void load_model_from_source(const char *model_string,
                              const char *range_string) {
    m_model = svm_load_model_from_string(model_string);
  }

private:
};

} // namespace classifiers
} // namespace nitro
