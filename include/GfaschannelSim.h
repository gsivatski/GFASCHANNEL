#ifndef _GFASCHANNEL_SIM_H_INCLUDE_
#define _GFASCHANNEL_SIM_H_INCLUDE_

#ifdef __LYNXOS
/* only for kernel */
#include <dldd.h>
#include <kernel.h>
#include <kern_proto.h>

#include <errno.h>
#include <sys/types.h>
#include <conf.h>
#include <io.h>
#include <sys/ioctl.h>
#include <time.h>
#include <param.h>
#include <string.h>
#include <proto.h>
#include <proto_int.h>
#include <fcntl.h>
#include <dir.h>
#include <string.h>
#include <unistd.h>
#include <param.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <sys/file.h>
#include <termio.h>
#include <termios.h>
#include <ces/vmelib.h>	  /* for 'pdparam_xxx' structure */
#include <inode.h>

/* system thread */
#include <conf.h>
#include <st.h>
#endif	/* __LYNXOS */

#include "dg/vme_am.h" /* for VMEbus Address Modifier Codes */
#include "dg/ServiceRegIoctl.h"
#include "dg/ModuleHeader.h"

#if defined(__linux__) && defined (__KERNEL__)
#undef sel
#define sel poll_table_struct	/* for select entry point */
/* only for Linux kernel */
#include <cdcm/cdcm.h>
#include "dg/swab-extra-linux.h"
#endif

/* for kernel only */
#if defined(__LYNXOS) || defined (__KERNEL__)
/* to be able to use CDCM utils inside the driver */
#include <cdcm/cdcmBoth.h>
#endif

/* Provides debug information printing. */
#define DBG_BEG(f_nm)							\
do {									\
  if (s->info->debugFlag & DBGIOCTL) {					\
    swait(&sem_drvr, SEM_SIGIGNORE); /* lock calling thread */		\
    cprintf("(pid %d) (tid %d) %s() ", getpid(), st_getstid(), f_nm);	\
  }									\
} while (0)

/* information about the address that will be read/write */
#define DBG_ADDR(ra, idx, en)					   \
do {								   \
  if (s->info->debugFlag & DBGIOCTL) {				   \
    unsigned addr = (unsigned)ra;				   \
    cprintf("[(elIdx %d) (elAm %d) @ 0x%x] ", idx, (int)en, addr); \
  }								   \
} while (0)

/* ioctl completion tag */
#define DBG_OK()							\
do {									\
	if (s->info->debugFlag & DBGIOCTL) {				\
		cprintf(" - OK.\n");					\
		ssignal(&sem_drvr);	/* wake up waiting thread */	\
	}								\
 } while (0)

/* ioctl completion tag */
#define DBG_FAIL()					\
do {							\
  if (s->info->debugFlag & DBGIOCTL) {			\
    cprintf(" - FAIL.\n");				\
    ssignal(&sem_drvr);	/* wake up waiting thread */	\
  }							\
} while (0)

/* start timing measurements. Different in case of Lynx/Linux */
#ifdef __Lynx__

#define START_TIME_STAT()								\
do {											\
  if (s->info->debugFlag & DBGTIMESTAT) {						\
    /* because of the spatial locality reference phenomenon, fill in cache		\
       memory so that it will keep recently accessed values (in our case		\
       it is 'nanotime' sys function stuff), thus trying to avoid additional		\
       time loss induced by cache miss.							\
       Normally, '__builtin_prefetch' gcc function should be used to minimize		\
       cache-miss latency by moving data into a cache before it is accessed.		\
       But with currently used Lynx gcc (version 2.95.3 20010323 (Lynx) as		\
       of 24.04.06) it's not possible, as it's probably not supported.			\
       For more info see gcc 4.0.1 reference manual at:					\
       http://gcc.gnu.org/onlinedocs/gcc-4.0.1/gcc/Other-Builtins.html#Other-Builtins	\
       So let's wait until newer gcc will be installed and then try the new		\
       approach. */									\
    GlobStartN = nanotime(&GlobStartS);							\
											\
    /* wait for the better times to implement... */					\
    /*__builtin_prefetch(&GlobStartN, 1, 1); */						\
											\
    /* now we can start real timing measurements */					\
    GlobStartN = nanotime(&GlobStartS);							\
  }											\
} while (0)

#else  /* linux */

#define START_TIME_STAT() \
	if (s->info->debugFlag & DBGTIMESTAT) getnstimeofday(&_t_beg)

#endif /* defined __Lynx__ */

/* printout measured time statistics */
#define FINISH_TIME_STAT()						\
do {									\
	if (s->info->debugFlag & DBGTIMESTAT) {				\
		unsigned int retN; /* for timing measurements */	\
		retN = ComputeTime(GlobTimeStatS);			\
		if (retN == -1)						\
			cprintf("Done in %s\n", GlobTimeStatS);		\
		else							\
			cprintf("Done in %dns (%s)\n", retN, GlobTimeStatS); \
	}								\
 } while (0)


#define GFASCHANNEL_MAX_NUM_CARDS 21 /* VME crates have 21 slots max */

/* GFASCHANNEL module ioctl numbers */
#define GFASCHANNEL_IOCTL_MAGIC 'G'

#define GFASCHANNEL_IO(nr)      _IO(GFASCHANNEL_IOCTL_MAGIC, nr)
#define GFASCHANNEL_IOR(nr,sz)  _IOR(GFASCHANNEL_IOCTL_MAGIC, nr, sz)
#define GFASCHANNEL_IOW(nr,sz)  _IOW(GFASCHANNEL_IOCTL_MAGIC, nr, sz)
#define GFASCHANNEL_IOWR(nr,sz) _IOWR(GFASCHANNEL_IOCTL_MAGIC, nr, sz)

#define GFASCHANNEL_GET_VECTORS1 (_FIRST__IOCTL_ + 0)
#define GFASCHANNEL_SET_VECTORS1 (_FIRST__IOCTL_ + 1)
#define GFASCHANNEL_GET_VECTORS2 (_FIRST__IOCTL_ + 2)
#define GFASCHANNEL_SET_VECTORS2 (_FIRST__IOCTL_ + 3)
#define GFASCHANNEL_GET_VECTORS3 (_FIRST__IOCTL_ + 4)
#define GFASCHANNEL_SET_VECTORS3 (_FIRST__IOCTL_ + 5)
#define GFASCHANNEL_GET_VECTORS4 (_FIRST__IOCTL_ + 6)
#define GFASCHANNEL_SET_VECTORS4 (_FIRST__IOCTL_ + 7)
#define GFASCHANNEL_GET_VECTORS5 (_FIRST__IOCTL_ + 8)
#define GFASCHANNEL_SET_VECTORS5 (_FIRST__IOCTL_ + 9)
#define GFASCHANNEL_GET_VECTORS6 (_FIRST__IOCTL_ + 10)
#define GFASCHANNEL_SET_VECTORS6 (_FIRST__IOCTL_ + 11)
#define GFASCHANNEL_GET_VECTORS7 (_FIRST__IOCTL_ + 12)
#define GFASCHANNEL_SET_VECTORS7 (_FIRST__IOCTL_ + 13)
#define GFASCHANNEL_GET_VECTORS8 (_FIRST__IOCTL_ + 14)
#define GFASCHANNEL_SET_VECTORS8 (_FIRST__IOCTL_ + 15)
#define GFASCHANNEL_GET_VECTORS9 (_FIRST__IOCTL_ + 16)
#define GFASCHANNEL_SET_VECTORS9 (_FIRST__IOCTL_ + 17)
#define GFASCHANNEL_GET_VECTORS10 (_FIRST__IOCTL_ + 18)
#define GFASCHANNEL_SET_VECTORS10 (_FIRST__IOCTL_ + 19)
#define GFASCHANNEL_GET_VECTORS11 (_FIRST__IOCTL_ + 20)
#define GFASCHANNEL_SET_VECTORS11 (_FIRST__IOCTL_ + 21)
#define GFASCHANNEL_GET_VECTORS12 (_FIRST__IOCTL_ + 22)
#define GFASCHANNEL_SET_VECTORS12 (_FIRST__IOCTL_ + 23)
#define GFASCHANNEL_GET_VECTORS13 (_FIRST__IOCTL_ + 24)
#define GFASCHANNEL_SET_VECTORS13 (_FIRST__IOCTL_ + 25)
#define GFASCHANNEL_GET_VECTORS14 (_FIRST__IOCTL_ + 26)
#define GFASCHANNEL_SET_VECTORS14 (_FIRST__IOCTL_ + 27)
#define GFASCHANNEL_GET_VECTORS15 (_FIRST__IOCTL_ + 28)
#define GFASCHANNEL_SET_VECTORS15 (_FIRST__IOCTL_ + 29)
#define GFASCHANNEL_GET_VECTORS16 (_FIRST__IOCTL_ + 30)
#define GFASCHANNEL_SET_VECTORS16 (_FIRST__IOCTL_ + 31)
#define GFASCHANNEL_GET_VECTORS17 (_FIRST__IOCTL_ + 32)
#define GFASCHANNEL_SET_VECTORS17 (_FIRST__IOCTL_ + 33)
#define GFASCHANNEL_GET_VECTORS18 (_FIRST__IOCTL_ + 34)
#define GFASCHANNEL_SET_VECTORS18 (_FIRST__IOCTL_ + 35)
#define GFASCHANNEL_GET_VECTORS19 (_FIRST__IOCTL_ + 36)
#define GFASCHANNEL_SET_VECTORS19 (_FIRST__IOCTL_ + 37)
#define GFASCHANNEL_GET_VECTORS20 (_FIRST__IOCTL_ + 38)
#define GFASCHANNEL_SET_VECTORS20 (_FIRST__IOCTL_ + 39)
#define GFASCHANNEL_GET_VECTORS21 (_FIRST__IOCTL_ + 40)
#define GFASCHANNEL_SET_VECTORS21 (_FIRST__IOCTL_ + 41)
#define GFASCHANNEL_GET_VECTORS22 (_FIRST__IOCTL_ + 42)
#define GFASCHANNEL_SET_VECTORS22 (_FIRST__IOCTL_ + 43)
#define GFASCHANNEL_GET_VECTORS23 (_FIRST__IOCTL_ + 44)
#define GFASCHANNEL_SET_VECTORS23 (_FIRST__IOCTL_ + 45)
#define GFASCHANNEL_GET_VECTORS24 (_FIRST__IOCTL_ + 46)
#define GFASCHANNEL_SET_VECTORS24 (_FIRST__IOCTL_ + 47)
#define GFASCHANNEL_GET_VECTORS25 (_FIRST__IOCTL_ + 48)
#define GFASCHANNEL_SET_VECTORS25 (_FIRST__IOCTL_ + 49)
#define GFASCHANNEL_GET_VECTORS26 (_FIRST__IOCTL_ + 50)
#define GFASCHANNEL_SET_VECTORS26 (_FIRST__IOCTL_ + 51)
#define GFASCHANNEL_GET_VECTORS27 (_FIRST__IOCTL_ + 52)
#define GFASCHANNEL_SET_VECTORS27 (_FIRST__IOCTL_ + 53)
#define GFASCHANNEL_GET_VECTORS28 (_FIRST__IOCTL_ + 54)
#define GFASCHANNEL_SET_VECTORS28 (_FIRST__IOCTL_ + 55)
#define GFASCHANNEL_GET_VECTORS29 (_FIRST__IOCTL_ + 56)
#define GFASCHANNEL_SET_VECTORS29 (_FIRST__IOCTL_ + 57)
#define GFASCHANNEL_GET_VECTORS30 (_FIRST__IOCTL_ + 58)
#define GFASCHANNEL_SET_VECTORS30 (_FIRST__IOCTL_ + 59)
#define GFASCHANNEL_GET_VECTORS31 (_FIRST__IOCTL_ + 60)
#define GFASCHANNEL_SET_VECTORS31 (_FIRST__IOCTL_ + 61)
#define GFASCHANNEL_GET_VECTORS32 (_FIRST__IOCTL_ + 62)
#define GFASCHANNEL_SET_VECTORS32 (_FIRST__IOCTL_ + 63)
#define GFASCHANNEL_GET_FUNC_CONVERT (_FIRST__IOCTL_ + 64)
#define GFASCHANNEL_SET_FUNC_CONVERT (_FIRST__IOCTL_ + 65)
#define GFASCHANNEL_GET_FUNC_SELECT (_FIRST__IOCTL_ + 66)
#define GFASCHANNEL_SET_FUNC_SELECT (_FIRST__IOCTL_ + 67)
#define GFASCHANNEL_GET_CONFIG (_FIRST__IOCTL_ + 68)
#define GFASCHANNEL_SET_CONFIG (_FIRST__IOCTL_ + 69)
#define GFASCHANNEL_GET_SLOPE (_FIRST__IOCTL_ + 70)
#define GFASCHANNEL_SET_SLOPE (_FIRST__IOCTL_ + 71)
#define GFASCHANNEL_GET_DAC_VAL (_FIRST__IOCTL_ + 72)
#define GFASCHANNEL_SET_DAC_VAL (_FIRST__IOCTL_ + 73)
#define GFASCHANNEL_GET_RECURR_CYCLES (_FIRST__IOCTL_ + 74)
#define GFASCHANNEL_SET_RECURR_CYCLES (_FIRST__IOCTL_ + 75)
#define GFASCHANNEL_GET_FUNC_DISABLE (_FIRST__IOCTL_ + 76)
#define GFASCHANNEL_SET_FUNC_DISABLE (_FIRST__IOCTL_ + 77)
#define GFASCHANNEL_GET_FORCE_CONVERT (_FIRST__IOCTL_ + 78)
#define GFASCHANNEL_SET_FORCE_CONVERT (_FIRST__IOCTL_ + 79)
#define GFASCHANNEL_GET_CONVERT_FUNC (_FIRST__IOCTL_ + 80)
#define GFASCHANNEL_SET_CONVERT_FUNC (_FIRST__IOCTL_ + 81)
#define GFASCHANNEL_GET_SELECT_FUNC (_FIRST__IOCTL_ + 82)
#define GFASCHANNEL_SET_SELECT_FUNC (_FIRST__IOCTL_ + 83)
#define GFASCHANNEL_GET_SOFT_START (_FIRST__IOCTL_ + 84)
#define GFASCHANNEL_SET_SOFT_START (_FIRST__IOCTL_ + 85)
#define GFASCHANNEL_GET_SOFT_EV_START (_FIRST__IOCTL_ + 86)
#define GFASCHANNEL_SET_SOFT_EV_START (_FIRST__IOCTL_ + 87)
#define GFASCHANNEL_GET_ACCEPT_STATUS (_FIRST__IOCTL_ + 88)
#define GFASCHANNEL_GET_STATUS (_FIRST__IOCTL_ + 89)
#define GFASCHANNEL_GET_DISABLE_STATUS (_FIRST__IOCTL_ + 90)

/* First allowed number for user-defined ioctl */
#define GFASCHANNEL_FIRST_USR_IOCTL (_FIRST__IOCTL_ + 91)

/* keeps last written value of the 'write only' registers */
typedef  struct {
} GFASCHANNELWriteonly_t;


/* user-defined extraneous registers */
typedef  struct {
} GFASCHANNELExtraneous_t;


/* Blk[#0]@addr[#1] Offs 0x0. Sz 98304 bytes. 32 reg(s). 0 gap(s) */
typedef volatile struct {
  /* [0x0 - 0xbfe] */ unsigned short VECTORS1[1536];
  /* [0xc00 - 0x17fe] */ unsigned short VECTORS2[1536];
  /* [0x1800 - 0x23fe] */ unsigned short VECTORS3[1536];
  /* [0x2400 - 0x2ffe] */ unsigned short VECTORS4[1536];
  /* [0x3000 - 0x3bfe] */ unsigned short VECTORS5[1536];
  /* [0x3c00 - 0x47fe] */ unsigned short VECTORS6[1536];
  /* [0x4800 - 0x53fe] */ unsigned short VECTORS7[1536];
  /* [0x5400 - 0x5ffe] */ unsigned short VECTORS8[1536];
  /* [0x6000 - 0x6bfe] */ unsigned short VECTORS9[1536];
  /* [0x6c00 - 0x77fe] */ unsigned short VECTORS10[1536];
  /* [0x7800 - 0x83fe] */ unsigned short VECTORS11[1536];
  /* [0x8400 - 0x8ffe] */ unsigned short VECTORS12[1536];
  /* [0x9000 - 0x9bfe] */ unsigned short VECTORS13[1536];
  /* [0x9c00 - 0xa7fe] */ unsigned short VECTORS14[1536];
  /* [0xa800 - 0xb3fe] */ unsigned short VECTORS15[1536];
  /* [0xb400 - 0xbffe] */ unsigned short VECTORS16[1536];
  /* [0xc000 - 0xcbfe] */ unsigned short VECTORS17[1536];
  /* [0xcc00 - 0xd7fe] */ unsigned short VECTORS18[1536];
  /* [0xd800 - 0xe3fe] */ unsigned short VECTORS19[1536];
  /* [0xe400 - 0xeffe] */ unsigned short VECTORS20[1536];
  /* [0xf000 - 0xfbfe] */ unsigned short VECTORS21[1536];
  /* [0xfc00 - 0x107fe] */ unsigned short VECTORS22[1536];
  /* [0x10800 - 0x113fe] */ unsigned short VECTORS23[1536];
  /* [0x11400 - 0x11ffe] */ unsigned short VECTORS24[1536];
  /* [0x12000 - 0x12bfe] */ unsigned short VECTORS25[1536];
  /* [0x12c00 - 0x137fe] */ unsigned short VECTORS26[1536];
  /* [0x13800 - 0x143fe] */ unsigned short VECTORS27[1536];
  /* [0x14400 - 0x14ffe] */ unsigned short VECTORS28[1536];
  /* [0x15000 - 0x15bfe] */ unsigned short VECTORS29[1536];
  /* [0x15c00 - 0x167fe] */ unsigned short VECTORS30[1536];
  /* [0x16800 - 0x173fe] */ unsigned short VECTORS31[1536];
  /* [0x17400 - 0x17ffe] */ unsigned short VECTORS32[1536];
} GFASCHANNELBlock00_t;

/* Blk[#1]@addr[#1] Offs 0x18000. Sz 18 bytes. 7 reg(s). 0 gap(s) */
typedef volatile struct {
  /* [0x0 - 0x2] */ unsigned short FUNC_CONVERT[2];
  /* 0x4 */ unsigned short FUNC_SELECT;
  /* 0x6 */ unsigned short CONFIG;
  /* 0x8 */ unsigned short SLOPE;
  /* 0xa */ unsigned short DAC_VAL;
  /* 0xc */ unsigned short RECURR_CYCLES;
  /* [0xe - 0x10] */ unsigned short FUNC_DISABLE[2];
} GFASCHANNELBlock01_t;

/* Blk[#2]@addr[#1] Offs 0x1a000. Sz 10 bytes. 5 reg(s). 0 gap(s) */
typedef volatile struct {
  /* 0x0 */ unsigned short FORCE_CONVERT;
  /* 0x2 */ unsigned short CONVERT_FUNC;
  /* 0x4 */ unsigned short SELECT_FUNC;
  /* 0x6 */ unsigned short SOFT_START;
  /* 0x8 */ unsigned short SOFT_EV_START;
} GFASCHANNELBlock02_t;

/* Blk[#3]@addr[#1] Offs 0x1c000. Sz 10 bytes. 3 reg(s). 0 gap(s) */
typedef volatile struct {
  /* [0x0 - 0x2] */ unsigned short ACCEPT_STATUS[2];
  /* 0x4 */ unsigned short STATUS;
  /* [0x6 - 0x8] */ unsigned short DISABLE_STATUS[2];
} GFASCHANNELBlock03_t;

/* GFASCHANNEL module master topology */
typedef struct {
  GFASCHANNELWriteonly_t  *wo; /* write only registers last history */
  GFASCHANNELExtraneous_t *ex; /* extraneous registers */
  GFASCHANNELBlock00_t *block00;
  GFASCHANNELBlock01_t *block01;
  GFASCHANNELBlock02_t *block02;
  GFASCHANNELBlock03_t *block03;
} GFASCHANNELTopology_t;

/* compiler should know variable type. So get rid of incomplete type */
typedef struct GFASCHANNELUserStatics_t GFASCHANNELUserStatics_t;

/* GFASCHANNEL statics data table */
typedef struct {
  GFASCHANNELTopology_t *card; /* hardware module topology */
  DevInfo_t *info; /* device information table */
  GFASCHANNELUserStatics_t *usrst; /* user-defined statics data table */
} GFASCHANNELStatics_t;

#endif /* _GFASCHANNEL_SIM_H_INCLUDE_ */
