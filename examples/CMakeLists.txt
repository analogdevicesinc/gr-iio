# Copyright 2011-2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

INSTALL(
    FILES
    attr-sink-source.grc
    cyclic-sine.grc
    cyclic-sine_pluto.grc
    fm_radio_pluto.grc
    fm_transmitter_pluto.grc
    DESTINATION ${GR_PKG_EXAMPLES_DIR}
    COMPONENT "iio_python"
)
