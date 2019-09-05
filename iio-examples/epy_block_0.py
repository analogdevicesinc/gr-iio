"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr
import libm2k

import time

class blk(gr.basic_block):
   def __init__(self, uri='ip:192.168.2.1',):
       gr.basic_block.__init__(
           self,
           name='M2k Init',
           in_sig=None,
           out_sig=None
       )
       libm2k.m2kOpen(uri)
