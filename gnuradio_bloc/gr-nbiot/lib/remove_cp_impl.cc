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
#include "remove_cp_impl.h"

// Use this define to activate debug printf
//#define _DEBUG

namespace gr {
  namespace nbiot {

    remove_cp::sptr
    remove_cp::make()
    {
      return gnuradio::get_initial_sptr
        (new remove_cp_impl());
    }

    /*
     * The private constructor
     */
    remove_cp_impl::remove_cp_impl()
      : gr::block("remove_cp",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        buffer_in_index(0),
        buffer_out_size(0),
        frame_number(0),
        subframe_number(-1),
        counter(0)
    {
      // do not propagate TAG as sample rate will change
      set_tag_propagation_policy(TPP_DONT);

      // set buffer in to 0
      memset(buffer_in,0,sizeof(gr_complex)*SAMPLES_PER_SUBFRAME);
    }

    /*
     * Our virtual destructor.
     */
    remove_cp_impl::~remove_cp_impl()
    {
    }

    // Use this method to inform GNU Radio scheduling how many items in input stream
    // are necessary to generate noutput_items. 
    // Also used this method to not ask input stream if output buffer is already full
    void
    remove_cp_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      // if buffer out already contains number of output need, we not need any input stream
      if (buffer_out_size > noutput_items){
        ninput_items_required[0] = 0;
      }else{
        // Otherwise the input size needed is 960/896 more than output to generate
        // This is due to the CP extract process 
        ninput_items_required[0] = (noutput_items >= 1024) ? int(noutput_items * 960 / 896) : 1097 ;
      }
    }

    int
    remove_cp_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // Get references on in/out stream
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Local variables
      int remaining_samples = ninput_items[0];
      int delayed_sample  = 0;
      int generated_samples = 0;
      int consumed_samples = 0;
      int i = 0;

      // Get tag in range
      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + remaining_samples);

      // Keep trace of subframes
      // Add offset of each enf of subframe in subrames_end queue
      for ( std::vector<tag_t>::iterator itag=tags.begin(); itag!=tags.end(); ++itag ){
        
        subframes_end.push(itag->offset + SAMPLES_PER_SUBFRAME);

#ifdef _DEBUG
        std::cout << " Tags addes in queue : " << itag->offset + SAMPLES_PER_SUBFRAME << " remaining_samples :  " << remaining_samples
         << " buffer oout size  : " << buffer_out_size << "  nitem read : " << nitems_read(0)
         << "  tag value : " << pmt::to_long(itag->value) << "\n";
#endif 

         // Update the subframe number with the first tag
         // subframe_number is initialize to -1 only once at constructor time
        if (subframe_number == -1){
          subframe_number = pmt::to_long(itag->value);
        }
      }

      // complete the buffer in
      while (remaining_samples > 0){

        // check if current element offset is equal to the front of the queue
        if (!subframes_end.empty() && subframes_end.front() == nitems_read(0) + consumed_samples){

#ifdef _DEBUG
          std::cout << "buffer in index " << buffer_in_index << "  buffer out size : " << buffer_out_size << "\n";
#endif
          // Copy buffer in into buffer out
          if (subframes_end.front() != 0){
            copy_samples_from_in_to_out(buffer_out+buffer_out_size, buffer_in);
            buffer_out_size += SAMPLES_PER_SUBFRAME_WITHOUT_CP;
          }

#ifdef _DEBUG
          std::cout << " Tags removes from queue : " << subframes_end.front() << "  queue size :  " << subframes_end.size() << "\n";
#endif 

          // Remove offset used from the queue
          subframes_end.pop();
        }

        // Update buffer in and local variable
        buffer_in[buffer_in_index] = *in++;
        buffer_in_index = (buffer_in_index + 1) % SAMPLES_PER_SUBFRAME;
        remaining_samples--;
        consumed_samples++;
      }

      // Tell runtime system how many samples were used
      // Send buffer
      consume_each(consumed_samples);

      // export buffer out to out stream
      // generate number of sample required (noutput_items)
      while (generated_samples < noutput_items && buffer_out_size){

        // Cizbter is 0 when we reach enf of subframe without CP
        if (counter == 0){

#ifdef _DEBUG
          std::cout << "Add tag " << subframe_number << "  offset : " << nitems_written(0) + generated_samples 
              << "frame number " << frame_number << "\n";
          std::cout << "buffer in index " << buffer_in_index << "  buffer out size : " << buffer_out_size 
              << "\n";
#endif 
          // Add specifig tags to the stream
          if (subframe_number != -1){
            if (subframe_number == NPSS_FRAME_INDEX){
              add_item_tag(0,
                nitems_written(0) + generated_samples,
                pmt::string_to_symbol("start_npss"), 
                pmt::from_long(subframe_number),
                pmt::string_to_symbol(this->name() ));  
            }else if (subframe_number == 0){
              add_item_tag(0,
                nitems_written(0) + generated_samples,
                pmt::string_to_symbol("frame"), 
                pmt::from_long(frame_number),
                pmt::string_to_symbol(this->name() ));
              frame_number++;
            }else{
              add_item_tag(0,
                nitems_written(0) + generated_samples,
                pmt::string_to_symbol("sub_frame"), 
                pmt::from_long(subframe_number),
                pmt::string_to_symbol(this->name() ));
            }
            // increment subframe number
            subframe_number = (subframe_number+1)%SUBFRAMES_PER_FRAME;
          }

        }

        // update out stream and local variable
        counter = (counter+1)%SAMPLES_PER_SUBFRAME_WITHOUT_CP;
        *out++ = buffer_out[generated_samples];
        generated_samples++;
        buffer_out_size--;
      }

      // remove the part that has been copy from output buffer
      memcpy(buffer_out,buffer_out+generated_samples,buffer_out_size*sizeof(gr_complex));

      // Tell runtime system how many output items we produced.
      return generated_samples;
    }


    /*
      This method is used to copy input buffer to the output buffer by removing CP

      param [out] out  : output buffer, must have a size of 1792 complex values
      param [in] in    : input buffer, must have 1920samples
    
      Description         : Subframe = 1920 Samples
                            Slot = 960 sampless
                            For each slot :  
                            --------------------------------------------------------------------
                            | CP1   | OFDM Symbol |  CP | OFDM Symbol |   *5 CP + OFDM Symbol  |
                            --------------------------------------------------------------------
                             CP1 = 10samples |  OFDM Symbol = 128samples |  CP = 9samples

                             This method is complex because the input buffer work as a circular buffer
                             the index give is not necesssary the start of the array. So I need to 
                             track the end of the input buffer to loop through
    */
    void remove_cp_impl::copy_samples_from_in_to_out(gr_complex* out, const gr_complex* in)
    {
      // get copy of in/out stream pointer
      gr_complex* in_bis = (gr_complex*)in;
      gr_complex* out_bis = out;

      // local variable
      int remaining_size = 0;
      int cp_length = 0;
      int symbol_length = 0;

      // Loop for each OFDM symbol
      for (int j = 0; j < SLOT_PER_SUBFRAME ; ++j){
        for (int i = 0; i < SYMBOL_PER_SLOT; ++i){

          remaining_size = SAMPLES_PER_SUBFRAME - buffer_in_index;
          cp_length = (i == 0) ? CP_FIRST : CP_NORMAL;
          symbol_length = USED_SAMPLES_PER_SYMBOL + cp_length;
        
          if (buffer_in_index + symbol_length >= SAMPLES_PER_SUBFRAME){
            if (buffer_in_index + cp_length >= SAMPLES_PER_SUBFRAME){
              // In this case end of buffer contains only the cycle prefix
              // skip cp part at first and then copy the OFDM symbol samples
              buffer_in_index = (buffer_in_index + cp_length) % SAMPLES_PER_SUBFRAME;
              memcpy(out_bis, in+buffer_in_index, USED_SAMPLES_PER_SYMBOL*sizeof(gr_complex));
              buffer_in_index = (buffer_in_index + USED_SAMPLES_PER_SYMBOL) % SAMPLES_PER_SUBFRAME;

            }else{
              // In this case the buffer start at the end and continue at the begining
              // do the copy in 2 times
              memcpy(out_bis, in+buffer_in_index+cp_length, (remaining_size-cp_length)*sizeof(gr_complex));
              buffer_in_index = (buffer_in_index+remaining_size) % SAMPLES_PER_SUBFRAME;   // should be 0
              out_bis = (out_bis + remaining_size - cp_length);

              memcpy(out_bis, in+buffer_in_index, (USED_SAMPLES_PER_SYMBOL - remaining_size + cp_length)*sizeof(gr_complex));
              out_bis = out_bis + USED_SAMPLES_PER_SYMBOL - remaining_size + cp_length;
              buffer_in_index = (buffer_in_index + symbol_length - remaining_size) % SAMPLES_PER_SUBFRAME;
            }

          }else{
            // First CP is 10 samples / useful OFDM symbol is 128 long
            memcpy(out_bis, in+buffer_in_index+cp_length, USED_SAMPLES_PER_SYMBOL*sizeof(gr_complex));
            buffer_in_index = (buffer_in_index + symbol_length) % SAMPLES_PER_SUBFRAME;
            out_bis += USED_SAMPLES_PER_SYMBOL;
          }       

                
        }
      }
    }
      

  } /* namespace nagrasdr */
} /* namespace gr */

