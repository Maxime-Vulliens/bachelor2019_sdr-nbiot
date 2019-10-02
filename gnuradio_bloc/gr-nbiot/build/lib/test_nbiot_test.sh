#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/gnuradio/Desktop/gnubloc/gr-nbiot/lib
export PATH=/home/gnuradio/Desktop/gnubloc/gr-nbiot/build/lib:$PATH
export LD_LIBRARY_PATH=/home/gnuradio/Desktop/gnubloc/gr-nbiot/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-nbiot 
