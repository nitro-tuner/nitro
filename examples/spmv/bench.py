from nitro.autotuner import *
from nitro.code_variant import *

import glob

# Set tuning properties for the spmv code variant
spmv = code_variant("spmv", 4)
spmv.constraints = True

tuner = autotuner("spmv")

# Set global tuning properties
tuner.set_build_command("make")
tuner.set_clean_command("make clean")

inputs = glob.glob('inputs/testing/*.mtx')

tuner.set_testing_args(inputs)
tuner.benchmark([spmv])
