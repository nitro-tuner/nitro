#############################################################################
#
#   Copyright 2012-2016 University of Utah
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

import subprocess, shutil

from code_variant import code_variant
from util.prettyprint import *
from util import config_generator, parsers

class autotuner:
  """Nitro: Main Autotuning Class"""

  m_app_name = ""
  m_build_command = "make"
  m_clean_command = "make clean"
  m_training_args = []
  m_testing_args = []
  m_record_benchmark_data = True

  # User options
  record_commands = False
  log_errors = True

  def __init__(self, app_name):
    self.m_app_name = app_name
    print_header("Nitro Automatic Performance Tuning System")
    print_header("Copyright (C) 2012-2016 University of Utah")

  def set_training_args(self, args):
    self.m_training_args = args

  def set_testing_args(self, args):
    self.m_testing_args = args

  def set_build_command(self, command):
    self.m_build_command = command

  def set_clean_command(self, command):
    self.m_clean_command = command

  def tune(self, variant_list, benchmark = False):
    mode_string = "BENCHMARK" if benchmark else "TUNING"
    print_header("\n-------------------------------")
    print_header(mode_string + " MODE")
    print_header("-------------------------------\n")

    if benchmark != True:
      print_header("Cleaning Up Directory")
      self.cleanup()

    print_header("Building Code Path Executable")
    self.build_executable_codepath(variant_list)

    print_status("Enumerating Code Paths")
    code_paths = self.enumerate_code_paths(variant_list)
    print_ok()
    indent()
    print_info(str(len(code_paths)) + " Code Paths Found")
    deindent()

    print_header("Collecting Training Data")
    if benchmark:
      self.run_paths(variant_list, code_paths, self.m_testing_args)
      self.train_model(variant_list, benchmark)
    else:
      self.run_paths(variant_list, code_paths, self.m_training_args)
      self.train_model(variant_list, benchmark)

    if benchmark != True:
      print_header("Building Classification Model")
      self.build_model(variant_list)

    print_header("Creating Tuned Version")
    self.build_executable_tuned(variant_list)

    print_header("\nAll Done!")

  def itune(self, variant_list):
    print_header("Nitro Automatic Performance Tuning System")
    print_header("Copyright (C) 2012-2013 University of Utah\n")

  def benchmark(self, variant_list):
    for v in variant_list:
      v.enable_benchmarking = True;
      # Clear Perf Files
      f = open(v.m_name + ".perf", "w")
      f.close()

    self.tune(variant_list, True)

    for v in variant_list:
      print_header("Calculating Prediction Accuracy for " + v.m_name)
      print_header("Collecting Tuned Performance")
      indent()
      for t in self.m_testing_args:
        command = "./" + self.m_app_name + "_tuned " + t
        print_status(command)
        # Run the app and let it record perf data
        self.run_command(command)
        print_ok()

      deindent()
      print_header("Prediction Accuracies:")
      average_perf = self.compute_avg_performance(v, self.m_record_benchmark_data)
      print_status("Average Performance w.r.t Best: ")
      print_ok(str(average_perf) + " %")

  def compute_avg_performance(self, variant, record=False):
    indent()
    tuned_perf_file = open(variant.m_name + ".tsperf", "r")
    best_perf_file = open(variant.m_name + ".ts", "r")
    timings_file = open(variant.m_name + ".perf", "r")

    if record:
      bm_file = open(variant.m_name + ".acc", "w")

    timings = [l.strip() for l in timings_file]
    abs_perf = dict()
    for t in timings:
      idx, fv, perf = [l.strip() for l in t.split(':')]
      if fv in abs_perf:
        abs_perf[fv][idx] = perf
      else:
        abs_perf[fv] = dict()
        abs_perf[fv][idx] = perf

    tuned_perf = [l.strip() for l in tuned_perf_file]
    best_perf = [l.strip() for l in best_perf_file]

    if len(tuned_perf) != len(best_perf):
      print_error("File size mismatch b/w tuned and best perf files")
      quit()

    bp_dict = dict()

    for bp in best_perf:
      bp_idx, bp_fv = [l.strip() for l in bp.split(':')]
      bp_dict[bp_fv] = bp_idx

    sum = 0.0
    for tp in tuned_perf:
      tp_idx, tp_fv = [l.strip() for l in tp.split(':')]

      tp_num = float(abs_perf[tp_fv][tp_idx])
      bp_num = float(abs_perf[tp_fv][bp_dict[tp_fv]])

      if bp_num < 0 and tp_num < 0:
        perc = tp_num / bp_num * 100.0
      else:
        perc = bp_num / tp_num * 100.0
      sum += perc
      print_status("[%s]: \t%s%.2f%%%s [Best(%s): %.4f, Tuned(%s): %.4f]\n" % \
          (tp_fv, bcolors.OKBLUE, perc, bcolors.ENDC, bp_dict[tp_fv], bp_num, tp_idx, tp_num))
      if record:
        bm_file.write(str(perc) + "\n")

    tuned_perf_file.close()
    best_perf_file.close()
    timings_file.close()
    if record:
      bm_file.close()
    deindent()

    return sum / float(len(tuned_perf))

  def build_model(self, variant_list):
    for v in variant_list:
      v.classifier.generate_model(v.m_name, "models/")

  def run_paths(self, variant_list, code_paths, inputs):
    indent()
    print_info("Collecting Performance Data")
    indent()
    total = len(inputs) * len(code_paths)
    count = 0

    if self.record_commands:
      command_logs = dict()
      for v in variant_list:
        f = open(v.m_name + '_commands.log', 'w')
        command_logs[v.m_name] = f

    for t in inputs:
      for path in code_paths:
        for v in path.keys():
          # Generate CodePath Config File
          config_generator.generate_cp_file(v.m_name + ".cp", path[v])
          command = "./" + self.m_app_name + "_codepath " + t
          status = "[" + v.m_name + ":" + str(path[v]) + "] " + command
          print_status(status)
          # Run the app and let it record perf data
          self.run_command(command)
          if self.record_commands:
            command_logs[v.m_name].write(status + '\n')
          perc = float(count) / float(total) * 100.0
          print_ok(str(int(perc)) + " %")
          count += 1
    deindent()

    if self.record_commands:
      for f in command_logs.keys():
        command_logs[f].close()

  def train_model(self, variant_list, benchmark):
    print_status("Calculating Best Variants")
    training_data = dict()
    for v in variant_list:
      training_data[v] = parsers.parse_training_data(v.m_name + ".perf")
    print_ok()

    if benchmark != True:
      print_status("Converting to Correct Formats")
      for v in variant_list:
        training_data[v] = v.classifier.convert_raw_training_data(training_data[v])
      print_ok()

    print_status("Generating Training Files")
    suffix = ".ts" if benchmark else ".t"
    for v in variant_list:
      f = open(v.m_name + suffix, "w")
      f.write(training_data[v])
      f.close()
    print_ok()
    deindent()

  def generate_cpp_config_codepath(self, variant_list):
    indent()
    print_status("Generating nitro_config.h")
    file_text = config_generator.generate_codepath(variant_list)
    f = open("nitro_config.h", "w")
    f.write(file_text)
    f.close()
    print_ok()
    deindent()

  def generate_cpp_config_tuned(self, variant_list):
    indent()
    print_status("Generating nitro_config.h")
    file_text = config_generator.generate_tuned(variant_list)
    f = open("nitro_config.h", "w")
    f.write(file_text)
    f.close()
    print_ok()
    deindent()

  def run_command(self, cmd, error_msg = None):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = p.communicate()[0]
    if p.returncode != 0:
      if error_msg == None:
        error_msg = "Failed to execute command: " + cmd.split()[0]
      print_error(error_msg + ' (Please check nitro_errors.log)', False)
      if self.log_errors:
        # XXX: Open as append?
        f = open('nitro_errors.log', 'w')
        cmd_line = 'Command: ' + cmd
        f.write(cmd_line + '\n')
        f.write('-' * len(cmd_line) + '\n')
        f.write(output + '\n')
        f.close()
      quit()

  def cleanup(self):
    self.run_command(self.m_clean_command, "Clean command failed")

  def build(self, exec_name):
    indent()
    print_status("Building " + self.m_app_name)
    self.run_command(self.m_build_command, "Build Command Failed")
    print_ok()
    print_status("Moving to " + exec_name)
    shutil.move(self.m_app_name, exec_name)
    print_ok()
    deindent()

  def build_executable_codepath(self, variant_list):
    self.generate_cpp_config_codepath(variant_list)
    self.build(self.m_app_name + "_codepath")

  def build_executable_tuned(self, variant_list):
    self.generate_cpp_config_tuned(variant_list)
    self.build(self.m_app_name + "_tuned")

  def calc_tuning_order(self, variant_list):
    if len(variant_list) == 1:
      return variant_list
    return variant_list

  def enumerate_code_paths(self, variant_list):
    sizes = [l.m_num_variants for l in variant_list]
    maxlen = max(sizes)

    path_list = []
    for i in range(maxlen):
      paths = dict()
      for v in variant_list:
        if(i < v.m_num_variants):
          paths[v] = i
        else:
          paths[v] = -1
      path_list.append(paths)

    return path_list

