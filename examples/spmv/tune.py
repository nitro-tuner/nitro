from nitro.autotuner import *
from nitro.code_variant import *

import glob

# Set tuning properties for the spmv code variant
spmv = code_variant("spmv", 4)
spmv.constraints = True

tuner = autotuner("spmv")

# Set global tuning properties
tuner.record_commands = True
tuner.set_training_args(glob.glob("inputs/training/*.mtx"))

# Tune
tuner.tune([spmv])
