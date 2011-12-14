#ifndef _GFASCHANNEL_GET_SET_REG_H_INCLUDE_
#define _GFASCHANNEL_GET_SET_REG_H_INCLUDE_

/*
  These functions are used to deliver register values directly to the user
  space.

  API is the following:
  1 param -- statics table

  2 param -- ioctl argument in predefined format:
             Massive of 3 elements, each is 4 bytes long.
             [0] - user-space address
             [1] - number of elements to r/w
             [2] - element index, starting from zero

             In case of service registers -- ioctl arguments can vary.
             Their amount depends on specific ioctl number.
             See service routines (those are with __SRV__ subword)
             for more details on parameter amount.

             For example, if this is a repetitive r/w request
             (ioctl number is SRV__REP_REG_RW) then we should have 4 arguments,
             that are packed as follows:

             [0] -- ioctl number
             [1] -- user-space address
             [2] -- number of elements to r/w
             [3] -- element index, starting from zero

  3 param -- check r/w bounds (1 - yes, 0 - no)
             valid only in case of Lynx
  4 param -- repeatedly read register (1 - yes, 0 - no)


  Bear in mind, that r/w operation results goes diretly to the user space.
  If you want to operate on the HW registers inside the driver -- use
  low-level port operation functions from port_ops_[linux/lynx].h like:
  __inb      -- read a byte from a port
  __inw      -- read a word from a port
  __in       -- lread a long from a port
  __outb     -- write a byte to a port
  __outw     -- write a word to a port
  __outl     -- write a long to a port
  __rep_inb  -- read multiple bytes from a port into a buffer
  __rep_inw  -- read multiple words from a port into a buffer
  __rep_inl  -- read multiple longs from a port into a buffer
  __rep_outb -- write multiple bytes to a port from a buffer
  __rep_outw -- write multiple words to a port from a buffer
  __rep_outl -- write multiple longs to a port from a buffer

  These functions are used to r/w HW registers inside the driver.
  Never access registers directly. Use this function to do this.
*/

/* Service register operations */
int get___SRV__DEBUG_FLAG(register GFASCHANNELStatics_t*, char*, int, int);
int set___SRV__DEBUG_FLAG(register GFASCHANNELStatics_t*, char*, int, int);
int get___SRV__DEVINFO_T(register GFASCHANNELStatics_t*, char*, int, int);
int get___SRV__DRVR_VERS(register GFASCHANNELStatics_t*, char*, int, int);
int get___SRV__DAL_CONSISTENT(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function1 */
int get_VECTORS1(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS1(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function2 */
int get_VECTORS2(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS2(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function3 */
int get_VECTORS3(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS3(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function4 */
int get_VECTORS4(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS4(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function5 */
int get_VECTORS5(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS5(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function6 */
int get_VECTORS6(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS6(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function7 */
int get_VECTORS7(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS7(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function8 */
int get_VECTORS8(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS8(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function9 */
int get_VECTORS9(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS9(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function10 */
int get_VECTORS10(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS10(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function11 */
int get_VECTORS11(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS11(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function12 */
int get_VECTORS12(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS12(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function13 */
int get_VECTORS13(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS13(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function14 */
int get_VECTORS14(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS14(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function15 */
int get_VECTORS15(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS15(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function16 */
int get_VECTORS16(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS16(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function17 */
int get_VECTORS17(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS17(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function18 */
int get_VECTORS18(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS18(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function19 */
int get_VECTORS19(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS19(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function20 */
int get_VECTORS20(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS20(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function21 */
int get_VECTORS21(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS21(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function22 */
int get_VECTORS22(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS22(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function23 */
int get_VECTORS23(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS23(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function24 */
int get_VECTORS24(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS24(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function25 */
int get_VECTORS25(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS25(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function26 */
int get_VECTORS26(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS26(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function27 */
int get_VECTORS27(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS27(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function28 */
int get_VECTORS28(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS28(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function29 */
int get_VECTORS29(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS29(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function30 */
int get_VECTORS30(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS30(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function31 */
int get_VECTORS31(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS31(register GFASCHANNELStatics_t*, char*, int, int);

/* Vector sequence for function32 */
int get_VECTORS32(register GFASCHANNELStatics_t*, char*, int, int);
int set_VECTORS32(register GFASCHANNELStatics_t*, char*, int, int);

/* Mask of functions to be converted (bit map) */
int get_FUNC_CONVERT(register GFASCHANNELStatics_t*, char*, int, int);
int set_FUNC_CONVERT(register GFASCHANNELStatics_t*, char*, int, int);

/* Number of the function to be generated (selected) */
int get_FUNC_SELECT(register GFASCHANNELStatics_t*, char*, int, int);
int set_FUNC_SELECT(register GFASCHANNELStatics_t*, char*, int, int);

/* Mode [Normal/DAC/Low Jitter/Recurrent/Low Jitter Recurrent/Test1/Test2/No Func] */
int get_CONFIG(register GFASCHANNELStatics_t*, char*, int, int);
int set_CONFIG(register GFASCHANNELStatics_t*, char*, int, int);

/* Slope */
int get_SLOPE(register GFASCHANNELStatics_t*, char*, int, int);
int set_SLOPE(register GFASCHANNELStatics_t*, char*, int, int);

/* DAC value [0: -10V, 65535: +10V] */
int get_DAC_VAL(register GFASCHANNELStatics_t*, char*, int, int);
int set_DAC_VAL(register GFASCHANNELStatics_t*, char*, int, int);

/* Number of recurrent cycles (signed) */
int get_RECURR_CYCLES(register GFASCHANNELStatics_t*, char*, int, int);
int set_RECURR_CYCLES(register GFASCHANNELStatics_t*, char*, int, int);

/* Mask of functions to be disabled (bit map) */
int get_FUNC_DISABLE(register GFASCHANNELStatics_t*, char*, int, int);
int set_FUNC_DISABLE(register GFASCHANNELStatics_t*, char*, int, int);

/* Force conversions (boolean) */
int get_FORCE_CONVERT(register GFASCHANNELStatics_t*, char*, int, int);
int set_FORCE_CONVERT(register GFASCHANNELStatics_t*, char*, int, int);

/* Convert functions (boolean) */
int get_CONVERT_FUNC(register GFASCHANNELStatics_t*, char*, int, int);
int set_CONVERT_FUNC(register GFASCHANNELStatics_t*, char*, int, int);

/* Select function (boolean) */
int get_SELECT_FUNC(register GFASCHANNELStatics_t*, char*, int, int);
int set_SELECT_FUNC(register GFASCHANNELStatics_t*, char*, int, int);

/* Soft main start (boolean) */
int get_SOFT_START(register GFASCHANNELStatics_t*, char*, int, int);
int set_SOFT_START(register GFASCHANNELStatics_t*, char*, int, int);

/* Soft event start (boolean) */
int get_SOFT_EV_START(register GFASCHANNELStatics_t*, char*, int, int);
int set_SOFT_EV_START(register GFASCHANNELStatics_t*, char*, int, int);

/* Accepted functions (bit map) */
int get_ACCEPT_STATUS(register GFASCHANNELStatics_t*, char*, int, int);

/* Status (bit map) */
int get_STATUS(register GFASCHANNELStatics_t*, char*, int, int);

/* Disabled functions (bit map) */
int get_DISABLE_STATUS(register GFASCHANNELStatics_t*, char*, int, int);

#endif /* _GFASCHANNEL_GET_SET_REG_H_INCLUDE_ */
