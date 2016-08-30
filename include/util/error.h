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

#define NITRO_FATAL(str)                               \
  do {                                                 \
    fprintf(stderr, "[Nitro Fatal Error]: %s\n", str); \
    exit(-1);                                          \
  } while(0);

#ifndef NITRO_NDEBUG
  template<typename... Args>
  void NITRO_DBG(const char *msg, Args&&... args) {
    fprintf(stderr, "[Nitro]: ");
    fprintf(stderr, msg, args...);
    fprintf(stderr, "\n");
  }
#else
  #define NITRO_DBG(...)
#endif

} // namespace nitro
