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

import svm
import os
from svmutil import *
from nitro.util.system_utils import *
from nitro.util.prettyprint import *

class svm_classifier:
  """Support Vector Machine Classifier"""
  m_name = "svm"
  feature_scaling = True
  m_params = svm.svm_parameter()
  m_prob = None

  def convert_raw_training_data(self, raw_data):
    t_data = ""
    for line in raw_data.split('\n'):
      if line == "":
        break
      x = [l.strip() for l in line.split(':')]
      t_data += x[0] + " "
      i = 1
      for feature in x[1].split():
        t_data += str(i) + ":" + feature + " "
        i += 1
      t_data += "\n"
    return t_data

  def scale_features(self, variant_name, models_folder):
    command = "svm-scale -s %s %s > %s" % (models_folder + variant_name + ".range",\
                                          variant_name + ".t",\
                                          variant_name + ".t.scale")
    try:
      result = subprocess.check_output(command, shell=True)
    except subprocess.CalledProcessError as e:
      print_error("Scaling Command Failed")
      quit()

  def generate_model(self, variant_name, models_folder):
    training_file = variant_name + ".t"
    if self.feature_scaling:
      self.scale_features(variant_name, models_folder)
      training_file += ".scale"
    (y, x) = svm_read_problem(training_file)
    self.m_prob = svm.svm_problem(y, x, self.m_params.kernel_type == PRECOMPUTED)

    libsvm_path = os.environ['LIBSVM_PATH']
    scaled_filename = os.path.abspath(training_file)
    cp = "python grid.py " + scaled_filename
    curdir = os.getcwd()
    os.chdir(libsvm_path + "/tools/")
    result = call_process(cp)
    os.chdir(curdir)
    C,g,rate = [float(l) for l in result.split("\n")[-2].split(" ")]

    print "C: %.8f, gamma: %.8f\n" % (C,g)

    self.m_params.C = C
    self.m_params.gamma = g

    print "\n-----------------------------"
    model = svm.svm_train(self.m_prob, self.m_params)
    print "-----------------------------\n"

    svm_save_model(models_folder + variant_name + ".model", model)

