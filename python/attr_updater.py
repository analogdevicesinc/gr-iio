#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 Analog Devices Inc.
# Author: Travis Collins <travis.collins@analog.com>
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

import numpy
from gnuradio import gr
import threading
import pmt
from time import sleep

class attr_updater(gr.basic_block):
    """
    attr_updater: Utility for creating PMT dictionaries in a format for interfacing
    with IIO Attribute blocks.
    """
    def __init__(self, attribute, value, interval):
        gr.basic_block.__init__(self,
            name="attr_updater",
            in_sig=None,
            out_sig=None)

        self.attribute = attribute
        self.value = value
        self.interval = interval
        self.port = pmt.intern("out")
        self.message_port_register_out(self.port)
        self.lock = threading.Lock()
        self.run = True
        self.thread = threading.Thread(target=self.sender, args=())
        self.thread.start()

    def __del__(self):
        self.lock.acquire()
        self.run = False
        self.lock.release()
        self.thread.join()

    def update_value(self, value):
        self.value = value

    def sender(self):
        while 1:
            self.lock.acquire()
            if not self.run:
                self.lock.release()
                return
            self.lock.release()

            key0 = pmt.intern(str(self.attribute))
            val0 = pmt.intern(str(self.value))
            msg_dic = pmt.make_dict()
            msg_dic = pmt.dict_add(msg_dic, key0, val0)
            try:
                self.message_port_pub(self.port, msg_dic)
            except:
                print("Error: Failed to publish message")
                return

            sleep(self.interval/1000)
