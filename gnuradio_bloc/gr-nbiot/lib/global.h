#pragma once

namespace gr {
  namespace nbiot {
  	#define NB_IOT_FREQ 		(1920000)		// Hz

    #define NB_SLOT_PER_SUBFRAME    (2)
    #define FFT_LEN                 (128)
    #define CP_NORMAL               (9)
    #define CP_FIRST                (10)
    #define SAMPLES_PER_PSS_SYMBOL  (CP_NORMAL+128) // 137


    #define NB_SUBCARRIERS          (12)

    #define USED_SAMPLES_PER_SYMBOL		(128)
    #define SAMPLES_PER_FULL_SYMBOL		(CP_NORMAL + USED_SAMPLES_PER_SYMBOL)	//137
    #define SAMPLES_PER_FIRST_SYMBOL	(CP_FIRST + USED_SAMPLES_PER_SYMBOL)		//138
    #define SYMBOL_PER_SLOT				(7)
    #define SAMPLES_PER_SLOT			((1*SAMPLES_PER_FIRST_SYMBOL)+(6*SAMPLES_PER_FULL_SYMBOL)) // 960
    #define SLOT_PER_SUBFRAME			(2)	
    #define SYMBOLS_PER_SUBFRAME        (SYMBOL_PER_SLOT * SLOT_PER_SUBFRAME)		
    #define SAMPLES_PER_SUBFRAME		(SLOT_PER_SUBFRAME*SAMPLES_PER_SLOT)	// 1920
    #define SUBFRAMES_PER_FRAME 		(10)
    #define SAMPLES_PER_FRAME 			(SUBFRAMES_PER_FRAME*SAMPLES_PER_SUBFRAME) // 19200

    #define NPSS_FRAME_INDEX            (5)
    #define ELEMENTS_PER_SUBFRAME       (NB_SUBCARRIERS * SYMBOLS_PER_SUBFRAME)  // 168
    #define START_NPSS_OFFSET           (413)   // 138 + 2*137

    #define NSSS_NB_SYMBOLS             (11)
    #define NSSS_NB_SAMPLES             (NSSS_NB_SYMBOLS*NB_SUBCARRIERS)            //132

    #define START_NSSS_OFFSET           (36)
    #define NSSS_FRAME_INDEX            (9)

    #define NSSS_MAGNITUDE_THRESHOLD    (50)

    #define SAMPLES_PER_SUBFRAME_WITHOUT_CP        (USED_SAMPLES_PER_SYMBOL*SYMBOLS_PER_SUBFRAME) //1792

	}
}


// -620011.0