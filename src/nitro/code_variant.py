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

from classifiers.svm import svm_classifier

class code_variant:
  """Nitro Code Variant Class"""

  m_name = ""
  m_num_variants = 0

  def __init__(self, name, num):
    self.m_name = name
    self.m_num_variants = num

  classifier = svm_classifier()
  load_model_from_source = False
  constraints = True
  enable_benchmarking = False

