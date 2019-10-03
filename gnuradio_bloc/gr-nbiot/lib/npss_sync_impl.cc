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
#include "global.h"
#include "npss_sync_impl.h"
#include <volk/volk.h>
#include <gnuradio/fft/fft.h>


// #define MORE_LOG 

namespace gr {
  namespace nbiot {

    npss_sync::sptr
    npss_sync::make()
    {
      return gnuradio::get_initial_sptr
        (new npss_sync_impl());
    }

    /*
     * The private constructor
     */
    npss_sync_impl::npss_sync_impl()
      : gr::sync_block("npss_sync",
              gr::io_signature::make( 1 , 1 , sizeof(gr_complex)),
              gr::io_signature::make( 2 , 2 , sizeof(gr_complex))),
              alert_generator(0),
              base_freq(-620000.0)
    {
      // private variables initialization
      first_loop = 1;
      nb_items = 0;
      average_abs_correlation = 0;
      fine_mode = 0;
      step = 1;
      last_nb_items = 0;
      last_diff_nb_items = 0;
      npss_start_samples = 0;
      counter = 1;
      set_tag = 0;

      // Create a dump file
      fdump = fopen( DUMP_FILE,"a+");

      // This is used to create a message port for this block
      message_port_register_out(pmt::mp("npss_sync"));
      message_port_register_out(pmt::mp("freq"));
    }

    /*
     * Our virtual destructor.
     */
    npss_sync_impl::~npss_sync_impl()
    {

      // close file if any
      if (fdump) {
        fclose(fdump);
        fdump = nullptr;
      }
    }

    /*
      This method is use to decimate input stream
      param [in] k        : lag-constant parameter
      param [in] pss_seq  : vector of gr_complex values
      param [in] sample_per_symbol : tell how many samples there is for a symbol (17 for decimate / 137 for fine mode)
      return gr_complex   : current correlation value Ak(t)
    */
    gr_complex npss_sync_impl::auto_corelate( int k,const gr_complex* pss_seq, int sample_per_symbol)
    {
      gr_complex current_correlation(0,0);
      gr_complex conjugate_res;

      for(int m = 0; m < PSS_SYMBOLS-k; m++){
        // compute conjugate
        volk_32fc_x2_conjugate_dot_prod_32fc(&conjugate_res,&pss_seq[m*sample_per_symbol],&pss_seq[(m+k)*sample_per_symbol],sample_per_symbol);

        // apply code coverage
        if (CODE_COVER_NPSS[m] != CODE_COVER_NPSS[m+k]){
          current_correlation -= conjugate_res;
        }else{
          current_correlation += conjugate_res;
        }

      }
      current_correlation /= (PSS_SYMBOLS-k);
      
      return current_correlation;
    }



    int
    npss_sync_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Get reference to in/out streams
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = output_items.size()?( gr_complex*) output_items[0]:nullptr;
      gr_complex *out_clean = output_items.size()>1?( gr_complex*) output_items[1]:nullptr;

      // Local variables initialization
      const int k=3;                    // lag parameter
      const float pi = std::acos(-1);   // pi required to compute cfo offset
      int remaining_samples = noutput_items;
      int nb_generated_out = 0;       
      gr_complex current_correlation; // resulting correlation 

      
      // We need at least 1500 sample to execute the algorithm 
      while (remaining_samples >= 1500){

        // Check if we have detect NPSS and we are aligned with a subframe
        if (set_tag && (nitems_read(0) + nb_generated_out) == npss_start_samples + SAMPLES_PER_SUBFRAME * counter ){ 

          // Update tag counter
          if (counter == SUBFRAMES_PER_FRAME)
            counter = 1;
          else
            counter++;

#ifdef _DEBUG
              std::cout << "NPSS2: Add tag : " << counter << " npsss_start_samples : " << npss_start_samples << 
                "  offset set in stream : " << nitems_read(0) + nb_generated_out << "\n";
#endif

            // Add subframe tags to the stream, 2 types are generated
            //  frame used for subframe 0 / sub_frame used for others
            // the counter have an offset of 4. (it set to 1 when npss is detected)
            if (counter == 6){
              add_item_tag(1,
                            nitems_read(0) + nb_generated_out,
                            pmt::string_to_symbol("frame"), 
                            pmt::from_long((counter + 4) % 10),
                            pmt::string_to_symbol(this->name() ));
            }else if (counter == 1 ){
              add_item_tag(1,
                            nitems_read(0) + nb_generated_out,
                            pmt::string_to_symbol("sub_frame"), 
                            pmt::from_long(5),
                            pmt::string_to_symbol(this->name() ));      
            }else{
              add_item_tag(1,
                            nitems_read(0) + nb_generated_out,
                            pmt::string_to_symbol("sub_frame"), 
                            pmt::from_long((counter + 4) % 10),
                            pmt::string_to_symbol(this->name() ));
            }     
        }


        // compute correlation
        current_correlation = auto_corelate(k,in,SAMPLES_PER_PSS_SYMBOL);

        // compute new smooth correlation (keep a weight on last correlation to avoid big glitches)
        if (first_loop){
          smooth_correlation = current_correlation;
          first_loop = 0;
        }else{
          smooth_correlation = (CORR_WEIGHT*smooth_correlation)+((1-CORR_WEIGHT)*current_correlation);
        }

        // fine_mode is when current correllation is above the treshold value
        if (fine_mode){

          float magnitude = abs(smooth_correlation);
          // Memorize peak for this frame
          if ( magnitude > max_magnitude ) {
            max_magnitude = magnitude;
            arg_at_peak = arg(smooth_correlation);
            offset_at_peak = nitems_read(0) + nb_generated_out;
          }

          // Detect if current correlation is under the treshold value
          // In this case change working mode and add a tag
          if (abs(smooth_correlation) / average_abs_correlation < THRESHOLD_VALUE){

            fine_mode = 0;
            step = 1;

            add_item_tag(0,
                          nitems_read(0) + nb_generated_out,
                          pmt::string_to_symbol("tresholddwn"), 
                          pmt::from_long(1),
                          pmt::string_to_symbol(this->name() ));
          
            // compute npss start sample by taking the half between threshold go and down the limit
            npss_start_samples = offset_at_peak - START_NPSS_OFFSET; //(nb_sample_treshold_detect + ((nitems_read(0) + nb_generated_out - nb_sample_treshold_detect)/2)) - START_NPSS_OFFSET;

            float freq_offset = (float)64/19200.0/pi*arg_at_peak*15000.0;
            base_freq += freq_offset; ///30.0 ; // smooth frequency shift

            // If the freq_offset has changed : send it through message port
            if (base_freq != freq_sent){
              message_port_pub(pmt::mp("freq"),pmt::mp(freq_offset));
              freq_sent = base_freq;
            }

#ifdef MORE_LOG
            // Generate more log
            std::cout << "max_correlation : " << max_magnitude << "normalized correlation : " << max_magnitude / average_abs_correlation << "  offest : " << offset_at_peak << "\n";
#endif

            // keep offset of last peak and the difference between 2 last peaks
            last_diff_nb_items =  offset_at_peak - last_nb_items;
            last_nb_items = offset_at_peak;

            // Initialize private variable for nexgt frame
            counter = 1;
            alert_generator = 0;
            max_magnitude = 0; 
            arg_at_peak = 0; 
           }

        }else{

          // Decide to activate fine_mode if current correlation is above the treshold
          if (abs(smooth_correlation) / average_abs_correlation > THRESHOLD_VALUE){

            // We can start to set frame tag if we fine 3 NPSS samples with the same spacing between
            if ( last_diff_nb_items != 0 && npss_start_samples != 0 && (last_diff_nb_items - SAMPLES_PER_FRAME >= -1 &&
                        (last_diff_nb_items - SAMPLES_PER_FRAME <= 1))){
              // Frame numbering
              set_tag = 1;
            }

            // Update variables
            //last_diff_nb_items = nb_items - last_nb_items;
            fine_mode = 1;
            step = 1;
            //last_nb_items = nb_items;

            // Add tag to visualize when we change operating mode
            add_item_tag(0,
                          nitems_read(0) + nb_generated_out,
                          pmt::string_to_symbol("tresholdup"), 
                          pmt::from_long(2),
                          pmt::string_to_symbol(this->name() ));

            // Keep sample offset of threshold detect to compute start of npss
            nb_sample_treshold_detect = nitems_read(0) + nb_generated_out;
          }
        }

        /******************************************************************************************************
        NPSS Processing
        ******************************************************************************************************/

        // update out stream with new correalaiton value
        // On 2nd output, the stream is keep as the input stream with some tags added
        for (int i = 0; i < step; ++i)
        {
          *out++ = smooth_correlation;
          *out_clean++ = *(in + i);
        }

        // update loop variables
        remaining_samples -= step;
        nb_generated_out += step;
        in += step;

        // Update alert generator when we are in mode "set_tag"
        if (set_tag)
          alert_generator++;

        // If the space between two NPSS occurence is greater than 19200
        // We start to generate alert on the message port each missing NPSS
        if (alert_generator>SAMPLES_PER_FRAME && alert_generator%SAMPLES_PER_FRAME == 0){
           message_port_pub(pmt::mp("npss_sync"),pmt::mp(alert_generator));
           if (fdump){
              fprintf(fdump, "Alert, npsss not find after %d samples, it's the %d occurence missed \n",
                      alert_generator, alert_generator/SAMPLES_PER_SUBFRAME ); 
           }
         }

        // comput new abs(correlation) average
        if (nb_items == 0){
          average_abs_correlation = abs(smooth_correlation);
          nb_items++ ;
        }else{
          average_abs_correlation = ((average_abs_correlation*nb_items) + abs(smooth_correlation)) / (nb_items+1);
          nb_items++ ;
        }
      }

      // Tell runtime system how many output items we produced.
      return nb_generated_out;
    }

  } /* namespace nbiot */
} /* namespace gr */

