title: gr-iio
brief: Analog Devices' IIO blocks for GNU Radio
tags:
  - IIO
  - FMCOMMS
  - Pluto
author:
  - Travis Collins
  - Paul Cercueil
  - Lars-Peter Clausen
copyright_owner:
  - Analog Devices Inc.
dependencies:
  - gnuradio
repo: https://github.com/analogdevicesinc/gr-iio.git
stable_release: HEAD
icon:
---

The Industrial Input/Output (IIO) framework has been in the upstream Linux kernels since 2011 and is responsible for handling sensors, converters, integrated transceivers and other real-world  I/O  devices. It provides a hardware abstraction layer with a consistent API for the user space applications. The IIO framework supports discrete components as well as integrated transceivers like the Analog Devices AD9361, a 2x2 RF Agile Transceiver, found in many SDR products.

gr-iio contains several new blocks for GNU Radio that can be used to stream samples from and to high-speed IIO devices. These new GNU Radio blocks are built around ADI’s libiio library, which concentrates most of the complexity, supports all IIO devices and has built-in support for data streaming over the network. This data streaming feature has several benefits: it allows the GNURadio flowgraphs to run on operating systems other than Linux (including Windows), and on more capable computers than the development board where the IIO devices are attached, which is usually not suitable for high-performance and real-time data processing.
