# Copyright 2011-2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# The presence of this file turns this directory into a Python package

'''
This is the GNU Radio IIO module.
'''

import os

# import pybind11 generated symbols into the iio namespace
try:
    from .iio_python import *
except ModuleNotFoundError:
    pass
