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

#ifndef INCLUDED_NAGRASDR_NPSS_SYNC_H
#define INCLUDED_NAGRASDR_NPSS_SYNC_H

#include <nbiot/npss_sync.h>
#include <list>
#include "global.h"

namespace gr {
  namespace nbiot {

    // Nb OFDM symbol per PSS signals
    #define PSS_SYMBOLS (11)

    class npss_sync_impl : public npss_sync
    {
     private:

      // Used to create a log file
      #define DUMP_FILE "dump_npss_sync.log"

      // Code-cover for Zadoff-Chu sequence
      const int CODE_COVER_NPSS[PSS_SYMBOLS]={1,1,1,1,-1,-1,1,1,1,-1,1};
      const float CORR_WEIGHT = 0;  // used to smooth correlation function / o = no smoothing
      const int THRESHOLD_VALUE = 20; // treshold value to detect a NPSS peak

      // Keep  average of correlation magnitude
      float average_abs_correlation;


      int first_loop;         // different behaviour on first loop
      int nb_items;           // keep nb items processed to compute average correlation
      int fine_mode;          // true when correlation is above the threshold, false otherwise
      int step;               // This parameter can be used to make biggest step when we are far from NPSS nignal - not used for now
      int last_nb_items;      // Keep the last offset where we detect a peak
      int last_diff_nb_items;       // Compute number of samples between 2 lasts peaks
      int nb_sample_treshold_detect;  // keep the offset when we pass the threshold limit
      int npss_start_samples;         // Keep the offset of npss start to generate tags
      int counter;                    // used to count the subframe number
      int set_tag;                    // Used to start tag generation
      int alert_generator;            // Used to generate an alarm when we miss an NPSS signal

      // these variables are used to save the offset where correlation has max magnitude
      float max_magnitude;   // max magnitude find
      float arg_at_peak;     // keep argument at peak to compute cfo offset
      int offset_at_peak;    // keep offset at peak

      float base_freq;       // Used to correct cfo
      float freq_sent;       // Used to remember last cfo value send

      gr_complex smooth_correlation;  // Used to smooth the correlation

      FILE *fdump;  // reference to the log file

      // THis method is used to compute the correlation
      gr_complex auto_corelate( int k,const gr_complex* pss_seq, int sample_per_symbol);

     public:
      npss_sync_impl();
      ~npss_sync_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace nbiot
} // namespace gr

#endif /* INCLUDED_NAGRASDR_NPSS_FINE_MODE_IMPL_H */



