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

#include <svm.h>

namespace nitro {

// FIXME: This is the most braindead implementation of this function
// possible. But then again the libsvm 'svm_load_model' code isn't that easy to
// translate either.
svm_model *svm_load_model_from_string(const char *model_string) {
  const char *tmp_file_name = "__temp_model.svm";
  FILE *fp_temp = fopen(tmp_file_name, "w");

  fprintf(fp_temp, "%s", model_string);
  fclose(fp_temp);

  return svm_load_model(tmp_file_name);
}

} // namespace nitro
