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

def parse_training_data(filename):
  perf_string = ""

  f = open(filename, "r")
  variant_perf = dict()
  for line in f:
    v_info = [li.strip() for li in line.split(':')]
    if v_info[1] in variant_perf:
      variant_perf[v_info[1]].append((v_info[0], v_info[2]))
    else:
      variant_perf[v_info[1]] = [(v_info[0], v_info[2])]

  for p in variant_perf.keys():
    min = (variant_perf[p][0][0], variant_perf[p][0][1])
    for li in variant_perf[p]:
      if float(li[1]) < float(min[1]):
        min = li
    perf_string = perf_string + min[0] + " : " + p + "\n"
  f.close()
  return perf_string
