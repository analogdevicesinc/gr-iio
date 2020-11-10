/* -*- c++ -*- */

/* The PlutoSDR block is in a separate file, because Swig chokes on it
 * and produces incorrect Python code if it is present in iio_swig.i... */

#define IIO_API

%include "gnuradio.i"

%{
#include "iio/pluto_sink.h"
%}

%include "iio/pluto_sink.h"

GR_SWIG_BLOCK_MAGIC2(iio, pluto_sink);
