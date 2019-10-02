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


#ifndef INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_H
#define INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_H

#include <nbiot/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace nbiot {

    /*!
     * \brief <+description of block+>
     * \ingroup nbiot
     *
     */
    class NBIOT_API npss_sync_advanced : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<npss_sync_advanced> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of nbiot::npss_sync_advanced.
       *
       * To avoid accidental use of raw pointers, nbiot::npss_sync_advanced's
       * constructor is in a private implementation
       * class. nbiot::npss_sync_advanced::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace nbiot
} // namespace gr

#endif /* INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_H */

