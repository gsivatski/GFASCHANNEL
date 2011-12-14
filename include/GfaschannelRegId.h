#ifndef _GFASCHANNEL_REG_ID_H_INCLUDE_
#define _GFASCHANNEL_REG_ID_H_INCLUDE_

#include "dg/ServiceRegId.h"

/* GFASCHANNEL module registers ID. Used by DAL */
typedef enum _tag_GFASCHANNEL_rid {
  VECTORS1_ID = LAST_SRV_REG, /* Vector sequence for function1 (RWMODE rw) */
  VECTORS2_ID, /* Vector sequence for function2 (RWMODE rw) */
  VECTORS3_ID, /* Vector sequence for function3 (RWMODE rw) */
  VECTORS4_ID, /* Vector sequence for function4 (RWMODE rw) */
  VECTORS5_ID, /* Vector sequence for function5 (RWMODE rw) */
  VECTORS6_ID, /* Vector sequence for function6 (RWMODE rw) */
  VECTORS7_ID, /* Vector sequence for function7 (RWMODE rw) */
  VECTORS8_ID, /* Vector sequence for function8 (RWMODE rw) */
  VECTORS9_ID, /* Vector sequence for function9 (RWMODE rw) */
  VECTORS10_ID, /* Vector sequence for function10 (RWMODE rw) */
  VECTORS11_ID, /* Vector sequence for function11 (RWMODE rw) */
  VECTORS12_ID, /* Vector sequence for function12 (RWMODE rw) */
  VECTORS13_ID, /* Vector sequence for function13 (RWMODE rw) */
  VECTORS14_ID, /* Vector sequence for function14 (RWMODE rw) */
  VECTORS15_ID, /* Vector sequence for function15 (RWMODE rw) */
  VECTORS16_ID, /* Vector sequence for function16 (RWMODE rw) */
  VECTORS17_ID, /* Vector sequence for function17 (RWMODE rw) */
  VECTORS18_ID, /* Vector sequence for function18 (RWMODE rw) */
  VECTORS19_ID, /* Vector sequence for function19 (RWMODE rw) */
  VECTORS20_ID, /* Vector sequence for function20 (RWMODE rw) */
  VECTORS21_ID, /* Vector sequence for function21 (RWMODE rw) */
  VECTORS22_ID, /* Vector sequence for function22 (RWMODE rw) */
  VECTORS23_ID, /* Vector sequence for function23 (RWMODE rw) */
  VECTORS24_ID, /* Vector sequence for function24 (RWMODE rw) */
  VECTORS25_ID, /* Vector sequence for function25 (RWMODE rw) */
  VECTORS26_ID, /* Vector sequence for function26 (RWMODE rw) */
  VECTORS27_ID, /* Vector sequence for function27 (RWMODE rw) */
  VECTORS28_ID, /* Vector sequence for function28 (RWMODE rw) */
  VECTORS29_ID, /* Vector sequence for function29 (RWMODE rw) */
  VECTORS30_ID, /* Vector sequence for function30 (RWMODE rw) */
  VECTORS31_ID, /* Vector sequence for function31 (RWMODE rw) */
  VECTORS32_ID, /* Vector sequence for function32 (RWMODE rw) */
  FUNC_CONVERT_ID, /* Mask of functions to be converted (bit map) (RWMODE rw) */
  FUNC_SELECT_ID, /* Number of the function to be generated (selected) (RWMODE rw) */
  CONFIG_ID, /* Mode [Normal/DAC/Low Jitter/Recurrent/Low Jitter Recurrent/Test1/Test2/No Func] (RWMODE rw) */
  SLOPE_ID, /* Slope (RWMODE rw) */
  DAC_VAL_ID, /* DAC value [0: -10V, 65535: +10V] (RWMODE rw) */
  RECURR_CYCLES_ID, /* Number of recurrent cycles (signed) (RWMODE rw) */
  FUNC_DISABLE_ID, /* Mask of functions to be disabled (bit map) (RWMODE rw) */
  FORCE_CONVERT_ID, /* Force conversions (boolean) (RWMODE rw) */
  CONVERT_FUNC_ID, /* Convert functions (boolean) (RWMODE rw) */
  SELECT_FUNC_ID, /* Select function (boolean) (RWMODE rw) */
  SOFT_START_ID, /* Soft main start (boolean) (RWMODE rw) */
  SOFT_EV_START_ID, /* Soft event start (boolean) (RWMODE rw) */
  ACCEPT_STATUS_ID, /* Accepted functions (bit map) (RWMODE r) */
  STATUS_ID, /* Status (bit map) (RWMODE rc) */
  DISABLE_STATUS_ID, /* Disabled functions (bit map) (RWMODE r) */
} GFASCHANNEL_rid_t;

#endif /* _GFASCHANNEL_REG_ID_H_INCLUDE_ */
