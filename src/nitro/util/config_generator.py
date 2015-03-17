#############################################################################
#
#   Copyright 2012-2015 University of Utah
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#############################################################################

from string import Template

header = """// WARNING: This is an auto-generated tuning configuration file.
// Please don't modify unless you know what you are doing. To
// generate/modify this file, please run your tuning script

#pragma once

#include <nitro_modes.h>
#include <classifiers/support_vector_machine.h>
#include <feature_evaluators/serial.h>
#include <constraint_evaluators/serial.h>

namespace nitro {
namespace tuning_policies {

struct __global_policy {
  typedef void *feature_evaluator_init;
};
"""

variant = """
struct ${__variant_name} {
  static const char *variant_name;
  static const int tuning_mode = ${__tuning_mode};
  static const bool enable_constraints = ${__enable_constraints};
  static const bool load_model_from_source = ${__load_model_from_source};
  static const bool benchmark_mode = ${__benchmark_mode};
  typedef nitro::classifiers::${__classifier_type} classifier_type;
  typedef nitro::feature_evaluators::${__feature_evaluator} feature_evaluator;
  typedef nitro::feature_scalers::${__feature_scaler} feature_scaler;
  typedef nitro::constraint_evaluators::${__constraint_evaluator} constraint_evaluator;
};

const char *${__variant_name}::variant_name = \"${__variant_name}\";

"""

footer = """
} // namespace tuning_policies
} // namespace nitro
"""

def BooleanCPP(value):
  if(value == True):
    return "true"
  else:
    return "false"

def generate_variant_config(v, tuning_mode):
  global variant

  s = Template(variant)
  s = s.substitute(__variant_name=v.m_name,
      __tuning_mode=tuning_mode,
      __enable_constraints=BooleanCPP(v.constraints),
      __load_model_from_source=BooleanCPP(v.load_model_from_source),
      __benchmark_mode=BooleanCPP(v.enable_benchmarking),
      __classifier_type=v.classifier.m_name + ("<true>" if v.classifier.feature_scaling else "<false>"),
      __feature_evaluator="serial_feature_evaluator",
      __feature_scaler="serial_feature_scaler",
      __constraint_evaluator="serial_constraint_evaluator")
  return s

def generate_codepath(variant_list):
  global header, footer

  config_file_text = header
  for v in variant_list:
    config_file_text = config_file_text + generate_variant_config(v, "RUN_CODE_PATH")

  config_file_text = config_file_text + footer
  return config_file_text

def generate_tuned(variant_list):
  global header, variant, footer

  config_file_text = header
  for v in variant_list:
    config_file_text = config_file_text + generate_variant_config(v, "RUN_TUNED")

  config_file_text = config_file_text + footer
  return config_file_text

def generate_cp_file(filename, num):
  f = open(filename, "w")
  f.write(str(num))
  f.close()

