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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "nsss_decode_impl.h"
#include <volk/volk.h>

namespace gr {
  namespace nbiot {

    nsss_decode::sptr
    nsss_decode::make()
    {
      return gnuradio::get_initial_sptr
        (new nsss_decode_impl());
    }

    /*
     * The private constructor
     */
    nsss_decode_impl::nsss_decode_impl()
      : gr::sync_block("nsss_decode",
              gr::io_signature::make( 1 , 1 , sizeof(gr_complex)),
              gr::io_signature::make( 1 , 1 , sizeof(gr_complex))),
        detect_nsss_frame(false),
        nsss_offset(-1),
        nsss_delay_in_stream(-1),
        nb_correlation(0),
        average_correlation(0)
    {

      // Create a dump file
      fdump = fopen( DUMP_FILE,"a+");

      // compute all possible zadoff chu sequence
      for (int cell_id = 1 ; cell_id < NSSS_CELLS_ID; cell_id++){    // cell id
        for (int frame_nb = 0 ; frame_nb < 4; frame_nb++){  // frame number
          compute_sequence(frame_nb,cell_id,(gr_complex*) &zadoff_seq[cell_id][frame_nb]);
        }
      }

      // set a minimun nb input needs to call this block
      set_min_noutput_items( 1024 );

      // create buffer to keep a complete subframe
      // This is used when the stream id provide only a part of NSSS subframe
      buffer = new gr_complex[ ELEMENTS_PER_SUBFRAME ];
      memset( buffer, 0, sizeof(buffer) );  // Not needed, just to be clean
    
    }

    /*
     * Our virtual destructor.
     */
    nsss_decode_impl::~nsss_decode_impl()
    {
      delete[] buffer;

      if (fdump) {
        fclose(fdump);
        fdump = nullptr;
      }
    }

    void nsss_decode_impl::compute_sequence(int frame_number, int cell_number, gr_complex* seq_array)
    {
      // Static binary sequence define by the NB-IoT Standard
      static const float b[4][128] = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
          1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
          1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
          1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1 },
        { 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
         -1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,
          1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
         -1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1 },
        { 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1,
         -1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,
         -1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,
          1,-1,-1, 1,-1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1 }
      };
      static const float pi = acos(-1); // Comput pi value once
      const gr_complex i(0,1);
      const int q = cell_number/126;

      // Local variables initialization
      int u =  (cell_number % 126) + 3;
      float n_prime = 0;
      int m = 0;
      float theta = ((float)33/132) * frame_number;
      gr_complex frame_shift = -i * (float)2 * pi * theta;
      gr_complex root_index = -i * pi * (float)u / (float)131;

      // loop to compute all of 132 elements of 1 sequence (fixed cell_id and frame_nb)
      for (int n = 0; n < NSSS_NB_SAMPLES; ++n)
      {
        n_prime = n % 131;
        m = n % 128;
        seq_array[n] = std::exp(frame_shift * (float)n) * std::exp(root_index * n_prime * (n_prime + 1)) * b[q][m];
      }

    }



    gr_complex nsss_decode_impl::cross_ceorrelate(const gr_complex* in, int &cell_number, int &frame_number)
    {

      gr_complex a[NSSS_CELLS_ID][4];

      for (int cell=1; cell<NSSS_CELLS_ID; cell++ ) {
        for (int frame=0; frame<4; frame++) {
          volk_32fc_x2_conjugate_dot_prod_32fc( &a[cell][frame], in, zadoff_seq[cell][frame], NSSS_SYMBOLS );
          //std::cout << "CORREL - cell " << cell << "  frame " << frame << " correlation :" << a[cell][frame] << "\n";
        }
      }

      // Parse results to see if we have only 1 single correlation spike
      float magnitude;
      float magnitude_max=0;
      for (int cell=1; cell< NSSS_CELLS_ID; cell++ ) {
        for (int frame=0; frame<4; frame++) {
          magnitude = abs(a[cell][frame]);
          if ( magnitude > magnitude_max ) {
            magnitude_max = magnitude;
            cell_number = cell;
            frame_number = frame;
          }
        }
      }

      if (magnitude_max > NSSS_MAGNITUDE_THRESHOLD){
        nb_correlation++;
        average_correlation = ((average_correlation * (nb_correlation-1)) + magnitude_max ) / nb_correlation;
        std::cout << "Decode nsss - frame : " << frame_number << "  cell : " << cell_number << " correaltion magnitude : " << magnitude_max <<
         " average correaltion : " << average_correlation << "\n";
         if (fdump){
          fprintf(fdump, "Decode NSSS - frame nb : %d , cell_id : %d , magnitude max : %f , average_magnitude : %f \n", 
                frame_number, cell_number, magnitude_max,average_correlation );
         }
      }

      return a[cell_number][frame_number];
    }

    int
    nsss_decode_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Get refrences to in/out stream
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out_0 = (gr_complex *) output_items[0];

      // local variables
      gr_complex conjugate_res;
      gr_complex default_corr(0,0);
      gr_complex seq_array[NSSS_NB_SAMPLES];
      gr_complex max_conjugate_res;

      float max_abs_correlation = 0;

      int current_frame = 0;
      int nb_generated_out = 0;
      int remaining_samples = noutput_items;
      int cell_number = 0;
      int frame_number = 0;
      

      // Check if we are in buffering mode (this mean that a part of the NSSS subframe is already buffered)
      if (buffering){

        if (remaining_samples >= (ELEMENTS_PER_SUBFRAME - nb_samples_buffered)){
          // we reach enf of NSSS subframe, we can compute correlation

          // copy end of NSSS in the buffer
          memcpy(buffer+nb_samples_buffered, in, (ELEMENTS_PER_SUBFRAME-nb_samples_buffered)*sizeof(gr_complex));
          // Compute correlation
          conjugate_res = cross_ceorrelate(buffer+START_NSSS_OFFSET,cell_number,frame_number);
          memset( buffer, 0, sizeof(buffer) );  // Not needed, just to be clean

          // update out stream
          *out_0++ = conjugate_res;

          // Update loop variables
          buffering = false;
          nb_samples_buffered = 0;
          detect_nsss_frame = false;
        }else{
          // continue buffering we not reach end of NSSS
          memcpy(buffer+nb_samples_buffered, in, remaining_samples*sizeof(gr_complex));
          // Keep number of samples buffered
          nb_samples_buffered += remaining_samples;

          // Update out stream
          for(int i = 0; i < remaining_samples; i++){
            *out_0++ = default_corr;
          }
          return noutput_items;
        }
      }

      // Detect when there is a start of frame 9
      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + remaining_samples);

      if (!detect_nsss_frame){
        for ( std::vector<tag_t>::iterator itag=tags.begin(); itag!=tags.end(); ++itag ) {
          if ( pmt::to_long(itag->value) == NSSS_FRAME_INDEX){
            detect_nsss_frame = true;
            nsss_offset = itag->offset;
            nsss_delay_in_stream = nsss_offset - nitems_read(0);
          }
        }
      }

      // Check if there is a nss frame
      if(detect_nsss_frame){
        
        // Check if there is all nsss sequence in input stream
        if(nsss_offset+ELEMENTS_PER_SUBFRAME <= nitems_read(0)+remaining_samples){
          // All NSSS subframe is in input stream, we can compute correlation
          // we can cross correlate
          conjugate_res = cross_ceorrelate( in+nsss_delay_in_stream+START_NSSS_OFFSET, cell_number, frame_number);

          // Update out stream with correlation value
          *out_0++ = conjugate_res;

          // update loops variable
          remaining_samples--;
          detect_nsss_frame = false;

        }else{
          // There is only a part of NSSS frame in the input stream
          // Buffering this part and activate buffering mode
          buffering = true;
          nb_samples_buffered = (nitems_read(0)+remaining_samples) - nsss_offset;
          memcpy(buffer, in + nsss_delay_in_stream, nb_samples_buffered*sizeof(gr_complex));
        }
      }

      // Correaltion is 0 when there isn't any nsss frame
      // update out stream
      for(int i = 0; i < remaining_samples; i++){
        *out_0++ = default_corr;
      }
      
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace nbiot */
} /* namespace gr */

