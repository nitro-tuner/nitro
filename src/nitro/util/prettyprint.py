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

import sys,re

indentation = 0

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OKGREEN = ''
        self.WARNING = ''
        self.FAIL = ''
        self.ENDC = ''

def print_status(s, color=bcolors.ENDC):
  sys.stdout.write(color + ' ' * indentation + s)

def print_warning(s):
  print(bcolors.WARNING + s + bcolors.ENDC)

def print_error(s, _quit = True):
  print(bcolors.FAIL + ' ' + "[ERROR: " + s + "]" + bcolors.ENDC)
  if _quit:
    quit()

def print_ok(s = "OK"):
  print(bcolors.OKBLUE + ' [' + s + ']' + bcolors.ENDC)

def print_header(s):
  print(s)

def print_info(s):
  print(' ' * indentation + s)

def indent():
  global indentation
  indentation = indentation + 4

def deindent():
  global indentation
  indentation = indentation - 4

try:
    # Win32
    from msvcrt import getch
except ImportError:
    # UNIX
    def getch():
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            return sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)

