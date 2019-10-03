/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_NAGRASDR_NSSS_DECODE_IMPL_H
#define INCLUDED_NAGRASDR_NSSS_DECODE_IMPL_H

#include <nbiot/nsss_decode.h>
#include "global.h"

namespace gr {
  namespace nbiot {

    // nb OFDM symbol per NSSS signal
    #define NSSS_NB_OFDM_SYMBOLS        (11)

    // nb samples per NSSS signal
    #define NSSS_NB_SAMPLE              (NSSS_NB_OFDM_SYMBOLS*NB_SUBCARRIERS) // 132  

    // nb of different cell id 
    #define NSSS_CELLS_ID               (504)

    class nsss_decode_impl : public nsss_decode
    {
     private:

      // Uses to create a dump file
      #define DUMP_FILE "dump_nsss_sync.log"

      // Used to pre-compute all 2016 sequences
      void compute_sequence(int frame_number, int cell_number, gr_complex* seq_array);

      // Used to correlate
      gr_complex cross_ceorrelate(const gr_complex* in, int &cell_number, int &frame_number);

      // Keep pre-compute sequences
      gr_complex zadoff_seq[NSSS_CELLS_ID][4][NSSS_NB_SAMPLES];
      
      bool detect_nsss_frame;     // Used to inform detection of NSSS subframe
      bool buffering = false;     // Used to bufferinf if NSSS sequence is not complete in input stream

      int nsss_offset;            // Keep nsss offset absolute offset
      int nsss_delay_in_stream;   // keep nsss offset in current input stream
      int nb_samples_buffered;

      float average_correlation;    // keep average correlation magnitude
      int nb_correlation;           // keep the nb of correlation comput

      FILE *fdump;    // keep reference of log file
      
      gr_complex* buffer; // Used to buffered NSSS frame if needed

     public:
      nsss_decode_impl();
      ~nsss_decode_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace nagrasdr
} // namespace gr

#endif /* INCLUDED_NAGRASDR_NSSS_DETECT_IMPL_H */

