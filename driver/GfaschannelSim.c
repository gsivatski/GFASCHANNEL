#ifdef __Lynx__
/* for __inb, __outb etc... */
#include "dg/port_ops_lynx.h"
#else  /* __linux__ */
#include "dg/port_ops_linux.h"
#include "dg/swab-extra-linux.h"
#endif
#include "GfaschannelSim.h"
#include "GfaschannelUserDefinedSim.h"
#include "GfaschannelVersionSim.h"

int sem_drvr = 1; /* initial semaphore value */

/* next are for timing measurements */
#define TSTR_LEN 32 /* time string lengths */
/* time statistic string */
char GlobTimeStatS[TSTR_LEN] = { [0 ... sizeof(GlobTimeStatS)-1] = 0 };
#ifdef __Lynx__
unsigned long GlobStartS = 0, GlobEndS = 0; /* seconds */
unsigned int  GlobStartN = 0, GlobEndN = 0; /* nanoseconds */
unsigned int  GlobCalVal = 0; /* timing measurements calibration value */
#else /* __linux__ */
static struct timespec _t_beg, _t_end, _t_delta;
static s64 _ac_etime;
#endif	/* __Lynx__ */

/* -------> GFASCHANNEL Simulator version info <------- */
/* driver compilation date and time */
static char Gfaschannel_compile_date[]  = __DATE__;
static char Gfaschannel_compile_time[]  = __TIME__;

/* which driverGen version was used to generate current code */
static const char Gfaschannel_version[] = "v2.7.78";

/* generation date in hex and human representation */
static const char Gfaschannel_generation_time_str[] = "Wed Dec 14 17:41:30 2011";
#define GFASCHANNEL_GENERATION_TIME_HEX 0x4ee8d1ba
/* ------------------------------------------------------------------------- */

/* to suppress implisit declaration warnings */
extern int nanotime(unsigned long *);
extern int ksprintf(char *, char *, ...);
extern int unmapVmeAddr(DevInfo_t*) __attribute__ ((weak));

/**
 * @brief Current driver/simulator version.
 *
 * @param vp -- if not @e NULL - driver version string will be placed here
 *
 * If @b vp is @e NULL, then driver compilation date and @e driverGen version,
 * by which current code was generated are printed out to the terminal output.
 * If @b vp is not null, then it is set to the address with version info.
 * No terminal printout is performed in this case.
 *
 * @return current driver version number
 */
static int DisplayVersion(char **vp)
{
	static char vers_info_str[MAX_STR] =
		{ [0 ... sizeof(vers_info_str)-1] = 0 };


	if (!vers_info_str[0]) { /* not initialized yet */
		Gfaschannel_compile_date[11] = 0;
		Gfaschannel_compile_time [9] = 0;

		/* build-up version info string */
		ksprintf(vers_info_str, "Compiled on: %s %s."
			 " Generated by driverGen <%s> on [ %s ]",
			 Gfaschannel_compile_date, Gfaschannel_compile_time,
			 Gfaschannel_version, Gfaschannel_generation_time_str);
	}

	if (vp)
		*vp = vers_info_str;
	else
		kkprintf("\nGFASCHANNEL Simulator <Vers. %d>\n%s",
			 GFASCHANNEL_SIM_CURRENT_VERSION,
			 vers_info_str);

	return GFASCHANNEL_SIM_CURRENT_VERSION;
}

#ifdef __Lynx__

/**
 * @brief Calibration for timing measurements
 *
 * @param info -- dev info table
 *
 * Find out how much time does @e timing @e measurements algorithm itself
 * takes. This time inaccuracy will be taken into account during timing
 * measurements computation. Consider it like timing collibration mechanism.
 *
 * For Linux - calibration is always zero.
 *
 * @return rough estimation for timing measurement in nanoseconds for Lynx
 * @return 0 - in case of Linux
 */
static unsigned int CalibrateTime(DevInfo_t *info)
{
	int cntr;
	unsigned int nsS, nsF, calVal; /* ns */
	unsigned long sS, sF;	         /* s */
	unsigned long long sum = 0;

	/* 100 sampling */
	for (cntr = 0; cntr < 0x64; cntr++) {
		/* hope that is less then 4s (if no - then you are fucked!) */
		nsS = nanotime(&sS);
		nsF = nanotime(&sF);
		calVal = ((sF-sS) && nsF < nsS) ? nsF + 1000000000 - nsS :
			nsF - nsS;
		sum += calVal;
	}

	calVal = sum/0x64;
	return calVal;
}

#else  /* __linux__ */

/* form kernel/lib/div64.c */
uint32_t __attribute__((weak)) __div64_32(uint64_t *n, uint32_t base)
{
	uint64_t rem = *n;
	uint64_t b = base;
	uint64_t res, d = 1;
	uint32_t high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (uint64_t) high << 32;
		rem -= (uint64_t) (high*base) << 32;
	}

	while ((int64_t)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

#endif


/**
 * @brief Process timing data, that was measured during driver code execution.
 *
 * @param tStr   -- measured time in %E format
 *
 * As currently @e LynxOS doesn't support %E output format, time string in
 * seconds (%E) representation is build-up and store in @e tStr parameter.
 * Note, that accuracy is limited up to 4 sec MAX. So if exec time is more,
 * than buildup string will contain notification message saying, that time is
 * more then 4 secs.
 *
 * @return Computed time in ns, @b LIMITED up to 4s (i.e. 4E+9ns) If time
 *         exeeds 4s, then -1 (i.e. 0xffffffff) is returnded. @e tStr string
 *         is set to the measured time in sec (%E format), or if it exeeds 4
 *         sec, then string will contain appropriate message.
 */
static unsigned int ComputeTime(char tStr[TSTR_LEN])
{
	int tseCntr, power;
	unsigned int  execTN = 0; /* total execution time in ns */
	unsigned long execTS = 0; /* total execution time in sec */

	static int e[10] = { /* all possible exp values */
		1,	   /* s  */
		10,	   /* ms */
		100,	   /* ms */
		1000,	   /* ms */
		10000,	   /* us */
		100000,	   /* us */
		1000000,   /* us */
		10000000,  /* ns */
		100000000, /* ns */
		1000000000 /* ns */
	};

	/* finish time measurements */
#ifdef __Lynx__
	GlobEndN = nanotime(&GlobEndS);
	execTS = GlobEndS - GlobStartS; /* total exec time in seconds */
#else
	getnstimeofday(&_t_end);
	_t_delta = timespec_sub(_t_end, _t_beg);
	_ac_etime = timespec_to_ns(&_t_delta);
	execTN = __div64_32(&_ac_etime, e[9]);
	execTS = _ac_etime;
#endif

	/* exec time is more or equal 4 sec. printout and return */
	if (execTS >= 4) {
		ksprintf(tStr, "> 4.00000E+00s");
		return -1;
	}

	/* init time string */
	bzero(tStr, sizeof(tStr));

	/* printout in XXs XXns format */
	/*
	cprintf("\nDone in %ds %dns.\n",
		execTS, (execTS && GlobEndN < GlobStartN) ?
		GlobEndN + e[9] - GlobStartN : GlobEndN - GlobStartN);
	*/

	/* convert seconds to nanoseconds */
	execTS *= e[9];

#ifdef __Lynx__
	/* compute total exec time in nanosec */
	execTN = execTS + GlobEndN - GlobStartN - GlobCalVal;
#else
	execTN += execTS;
#endif

	/* get current time dimension */
	for (power = 9; power >= 0; power--)
		if (execTN/e[power])/* we've got our dimension */
			break;

	/* build-up time string tail. precision will be */
	/* limited to 5 digits after the decimal point  */
	ksprintf(tStr, "%u", execTN);
	for (tseCntr = power; tseCntr >= 1; tseCntr--)
		/* shift one position to the right */
		tStr[tseCntr+1] = tStr[tseCntr];
	tStr[1] = '.';
	tStr[7] = 'E';
	tStr[8] = (9-power) ? '-' : '+';
	ksprintf(&tStr[9], "%02ds", 9-power);
	return execTN;
}


/* Standart getting/setting register operations for GFASCHANNEL module
   are located in included file */
#include "GfaschannelGetSetReg.inc.c"


/**
 * @brief Entry point function. Initializes minor devices.
 *
 * @param statPtr -- statics table pointer
 * @param devno   -- contains major/minor dev numbers
 * @param flp     -- file pointer
 *
 * @return OK     -- if succeed.
 * @return SYSERR -- in case of failure.
 */
int Gfaschannel_open(register GFASCHANNELStatics_t *statPtr,
		     int devno, struct file *flp)
{
	int minN = minor(devno); /* get minor device number */
	int proceed;	/* if standard code execution should be
			   proceed after call to
			   user entry point function  */
	int usrcoco;	/* completion code of user entry point function */

	if (minN >= GFASCHANNEL_MAX_NUM_CARDS) {
		kkprintf("%sMinor number (%d) exceeds MAX allowed"
			 " cards number (%d)\n",
			 ERR_MSG, minN, GFASCHANNEL_MAX_NUM_CARDS);
		pseterr(ENXIO);
		return SYSERR; /* -1 */
	}

	kkprintf("Gfaschannel: Open Logical Unit %d.\n", statPtr->info->mlun);

	/* user entry point function call */
	usrcoco = GfaschannelUserOpen(&proceed, statPtr, devno, flp);
	if (!proceed) /* all done by user */
		return usrcoco;

	kkprintf("Gfaschannel: Done\n\n");

	return OK; /* 0 */
}


/**
 * @brief Entry point function.Called to close open file descriptor.
 *
 * @param statPtr -- statics table pointer
 * @param flp     -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 *
 * @return OK     - if succeed.
 * @return SYSERR - in case of failure.
 */
int Gfaschannel_close(register GFASCHANNELStatics_t *statPtr, struct file *flp)
{
	int minN;    /* minor device number */
	int proceed; /* if standard code execution should be proceed
			after call to user entry point function  */
	int usrcoco; /* completion code of user entry point function */

#ifdef __Lynx__
	minN = minor(flp->dev);
#else
	minN = iminor(flp->f_path.dentry->d_inode);
#endif

	/* user entry point function call */
	usrcoco = GfaschannelUserClose(&proceed, statPtr, flp);
	if (!proceed) /* all done by user */
		return usrcoco;

	return OK; /* 0 */
}


/**
 * @brief Entry point function. Reads data from the device.
 *
 * @param statPtr -- statics table pointer
 * @param flp     -- file pointer
 * @param buff    -- character buffer pointer
 * @param count   -- number of bytes to copy
 *
 * @return number of bytes actually copied, including zero - if succeed.
 * @return SYSERR                                          - if fails.
 */
int Gfaschannel_read(register GFASCHANNELStatics_t *statPtr,
		     struct file *flp, char *buff, int count)
{
	int minN;    /* minor device number */
	int proceed; /* if standard code execution should be proceed
			after call to user entry point function  */
	int usrcoco; /* completion code of user entry point function */

#ifdef __Lynx__
	minN = minor(flp->dev);
#else
	minN = iminor(flp->f_path.dentry->d_inode);
#endif

	/* user entry point function call */
	usrcoco = GfaschannelUserRead(&proceed, statPtr, flp, buff, count);
	if (!proceed) /* all done by user */
		return usrcoco;

	return usrcoco; /* number of read bytes */
}


/**
 * @brief Entry point function. Sends data to the device.
 *
 * @param statPtr -- statics table pointer
 * @param flp     -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param which   -- condition to monitor. Not valid in case of Linux!\n
 * @param buff    -- char buffer pointer
 * @param count   -- the number of bytes to copy
 *
 * @return number of bytes actually copied, including zero - if succeed.
 * @return SYSERR                                          - if fails.
 */
int Gfaschannel_write(register GFASCHANNELStatics_t *statPtr,
		      struct file *flp, char *buff, int count)
{
	int minN; /* minor device number */
	int proceed;	/* if standard code execution should be proceed after
			   call to user entry point function  */
	int usrcoco;	/* completion code of user entry point function */

#ifdef __Lynx__
	minN = minor(flp->dev);
#else  /* __linux__ */
	minN = iminor(flp->f_path.dentry->d_inode);
#endif

	/* user entry point function call */
	usrcoco = GfaschannelUserWrite(&proceed, statPtr, flp, buff, count);
	if (!proceed) /* all done by user */
		return usrcoco;

	return usrcoco; /* number of written bytes */
}


/**
 * @brief Entry point function. Supports I/O polling or multiplexing.
 *
 * @param statPtr -- statics table pointer
 * @param flp     -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param which   -- condition to monitor. Not valid in case of Linux!\n
 *                   <b> SREAD, SWRITE, SEXCEPT </b> in case of Lynx.
 * @param ffs     -- select data structure in case of Lynx.\n
 *                   struct poll_table_struct in case of Linux.
 *
 * @return OK     - if succeed.
 * @return SYSERR - in case of failure.
 */
int Gfaschannel_select(register GFASCHANNELStatics_t *statPtr,
		       struct file *flp, int which, struct sel *ffs)
{
	int minN;    /* minor device number */
	int proceed; /* if standard code execution should be proceed after
			call to user entry point function  */
	int usrcc; /* completion code of user entry point function */

#ifdef __Lynx__
	minN = minor(flp->dev);
#else  /* __linux__ */
	minN = iminor(flp->f_path.dentry->d_inode);
#endif

	/* user entry point function call */
	usrcc = GfaschannelUserSelect(&proceed, statPtr, flp, which, ffs);
	if (!proceed) /* all done by user */
		return usrcc;

	return OK; /* 0 */
}


/**
 * @brief GFASCHANNEL ioctl entry point
 *
 * @param s   -- statics table
 * @param f   -- file pointer. Lynx/Linux specific.
 *               See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param com -- ioctl number
 * @param arg -- arguments
 *
 * By default - we do check r/w bounds while accessing the register.
 *
 * @return OK     - if succeed.
 * @return SYSERR - in case of failure.
 */
int Gfaschannel_ioctl(register GFASCHANNELStatics_t *s,
		      struct file *f, int com, char *arg)
{
	static int c_rwb = 1; /* check r/w bounds (1 - yes, 0 - no)
				 Valid _ONLY_ for Lynx! */
	int minN;     /* minor device number (LUN) */
	int proceed;  /* if standard code execution should be proceed after
			 call to user entry point function  */
	int rc;  /* return code */
	int r_rw = 0; /* repetitive r/w (1 - yes, 0 - no) */

	START_TIME_STAT(); /* timing measurements */

#ifdef __Lynx__
	minN = minor(f->dev);
#else
	minN = iminor(f->f_path.dentry->d_inode);
#endif

	/* user entry point function call */
	rc = GfaschannelUserIoctl(&proceed, s, f, minN, com, arg);
	if (!proceed) /* all done by user */
		goto out_ioctl;

 rep_ioctl:
	switch (com) { /* default 'ioctl' driver operations */
	case SRV_GET_DEBUG_FLAG:
		rc = get___SRV__DEBUG_FLAG(s, arg, c_rwb, r_rw);
		break;
	case SRV_SET_DEBUG_FLAG:
		rc = set___SRV__DEBUG_FLAG(s, arg, c_rwb, r_rw);
		break;
	case SRV_GET_DEVINFO_T:
		rc = get___SRV__DEVINFO_T(s, arg, c_rwb, r_rw);
		break;
	case SRV_GET_DRVR_VERS:
		rc = get___SRV__DRVR_VERS(s, arg, c_rwb, r_rw);
		break;
	case SRV_GET_DAL_CONSISTENT:
		rc = get___SRV__DAL_CONSISTENT(s, arg, c_rwb, r_rw);
		break;
	case SRV__REP_REG_RW:
		rc = srv_func___SRV__REP_REG_RW(s, arg, c_rwb, r_rw);
		break;
	case SRV__RW_BOUNDS:
		rc = srv_func___SRV__RW_BOUNDS(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS1:
		rc = get_VECTORS1(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS1:
		rc = set_VECTORS1(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS2:
		rc = get_VECTORS2(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS2:
		rc = set_VECTORS2(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS3:
		rc = get_VECTORS3(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS3:
		rc = set_VECTORS3(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS4:
		rc = get_VECTORS4(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS4:
		rc = set_VECTORS4(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS5:
		rc = get_VECTORS5(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS5:
		rc = set_VECTORS5(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS6:
		rc = get_VECTORS6(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS6:
		rc = set_VECTORS6(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS7:
		rc = get_VECTORS7(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS7:
		rc = set_VECTORS7(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS8:
		rc = get_VECTORS8(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS8:
		rc = set_VECTORS8(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS9:
		rc = get_VECTORS9(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS9:
		rc = set_VECTORS9(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS10:
		rc = get_VECTORS10(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS10:
		rc = set_VECTORS10(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS11:
		rc = get_VECTORS11(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS11:
		rc = set_VECTORS11(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS12:
		rc = get_VECTORS12(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS12:
		rc = set_VECTORS12(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS13:
		rc = get_VECTORS13(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS13:
		rc = set_VECTORS13(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS14:
		rc = get_VECTORS14(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS14:
		rc = set_VECTORS14(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS15:
		rc = get_VECTORS15(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS15:
		rc = set_VECTORS15(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS16:
		rc = get_VECTORS16(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS16:
		rc = set_VECTORS16(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS17:
		rc = get_VECTORS17(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS17:
		rc = set_VECTORS17(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS18:
		rc = get_VECTORS18(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS18:
		rc = set_VECTORS18(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS19:
		rc = get_VECTORS19(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS19:
		rc = set_VECTORS19(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS20:
		rc = get_VECTORS20(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS20:
		rc = set_VECTORS20(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS21:
		rc = get_VECTORS21(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS21:
		rc = set_VECTORS21(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS22:
		rc = get_VECTORS22(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS22:
		rc = set_VECTORS22(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS23:
		rc = get_VECTORS23(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS23:
		rc = set_VECTORS23(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS24:
		rc = get_VECTORS24(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS24:
		rc = set_VECTORS24(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS25:
		rc = get_VECTORS25(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS25:
		rc = set_VECTORS25(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS26:
		rc = get_VECTORS26(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS26:
		rc = set_VECTORS26(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS27:
		rc = get_VECTORS27(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS27:
		rc = set_VECTORS27(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS28:
		rc = get_VECTORS28(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS28:
		rc = set_VECTORS28(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS29:
		rc = get_VECTORS29(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS29:
		rc = set_VECTORS29(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS30:
		rc = get_VECTORS30(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS30:
		rc = set_VECTORS30(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS31:
		rc = get_VECTORS31(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS31:
		rc = set_VECTORS31(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_VECTORS32:
		rc = get_VECTORS32(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_VECTORS32:
		rc = set_VECTORS32(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_FUNC_CONVERT:
		rc = get_FUNC_CONVERT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_FUNC_CONVERT:
		rc = set_FUNC_CONVERT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_FUNC_SELECT:
		rc = get_FUNC_SELECT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_FUNC_SELECT:
		rc = set_FUNC_SELECT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_CONFIG:
		rc = get_CONFIG(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_CONFIG:
		rc = set_CONFIG(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_SLOPE:
		rc = get_SLOPE(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_SLOPE:
		rc = set_SLOPE(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_DAC_VAL:
		rc = get_DAC_VAL(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_DAC_VAL:
		rc = set_DAC_VAL(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_RECURR_CYCLES:
		rc = get_RECURR_CYCLES(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_RECURR_CYCLES:
		rc = set_RECURR_CYCLES(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_FUNC_DISABLE:
		rc = get_FUNC_DISABLE(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_FUNC_DISABLE:
		rc = set_FUNC_DISABLE(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_FORCE_CONVERT:
		rc = get_FORCE_CONVERT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_FORCE_CONVERT:
		rc = set_FORCE_CONVERT(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_CONVERT_FUNC:
		rc = get_CONVERT_FUNC(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_CONVERT_FUNC:
		rc = set_CONVERT_FUNC(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_SELECT_FUNC:
		rc = get_SELECT_FUNC(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_SELECT_FUNC:
		rc = set_SELECT_FUNC(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_SOFT_START:
		rc = get_SOFT_START(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_SOFT_START:
		rc = set_SOFT_START(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_SOFT_EV_START:
		rc = get_SOFT_EV_START(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_SET_SOFT_EV_START:
		rc = set_SOFT_EV_START(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_ACCEPT_STATUS:
		rc = get_ACCEPT_STATUS(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_STATUS:
		rc = get_STATUS(s, arg, c_rwb, r_rw);
		break;
	case GFASCHANNEL_GET_DISABLE_STATUS:
		rc = get_DISABLE_STATUS(s, arg, c_rwb, r_rw);
		break;
	default:
		pseterr(EINVAL);
		rc = SYSERR; /* -1 */
                break;
	} /* end of 'ioctl' operations switch */

 out_ioctl:
	return rc;
}


/**
 * @brief Cleanup and deallocation of the Simulator statics table.
 *
 * @param statPtr - deallocate this
 *
 * @return void
 */
static void GfaschannelMemCleanup(GFASCHANNELStatics_t *statPtr)
{
  DevInfo_t *dip = statPtr->info; /* device info pointer */

  if (statPtr->card != NULL) {
    kkprintf("Gfaschannel: Cleaning up Logical Unit %d\n", dip->mlun);

    if (statPtr->card->wo != NULL) {
      kkprintf("Gfaschannel: Writeonly structure deallocation - ");
      sysfree((char*)statPtr->card->wo, (long)sizeof(GFASCHANNELWriteonly_t));
      kkprintf("OK\n");
    }

    if (statPtr->card->ex != NULL) {
      kkprintf("Gfaschannel: Extraneous structure deallocation - ");
      sysfree((char*)statPtr->card->ex, (long)sizeof(GFASCHANNELExtraneous_t));
      kkprintf("OK\n");
    }
    if (statPtr->card->block00 != NULL) {
      kkprintf("Gfaschannel: Block 0 deallocation - ");
      sysfree((char*)statPtr->card->block00, (long)sizeof(GFASCHANNELBlock00_t));
      kkprintf("OK\n");
    }

    if (statPtr->card->block01 != NULL) {
      kkprintf("Gfaschannel: Block 1 deallocation - ");
      sysfree((char*)statPtr->card->block01, (long)sizeof(GFASCHANNELBlock01_t));
      kkprintf("OK\n");
    }

    if (statPtr->card->block02 != NULL) {
      kkprintf("Gfaschannel: Block 2 deallocation - ");
      sysfree((char*)statPtr->card->block02, (long)sizeof(GFASCHANNELBlock02_t));
      kkprintf("OK\n");
    }

    if (statPtr->card->block03 != NULL) {
      kkprintf("Gfaschannel: Block 3 deallocation - ");
      sysfree((char*)statPtr->card->block03, (long)sizeof(GFASCHANNELBlock03_t));
      kkprintf("OK\n");
    }

    kkprintf("Gfaschannel: Topology structure deallocation - ");
    sysfree((char*)statPtr->card, (long)sizeof(GFASCHANNELTopology_t));
    kkprintf("OK\n");

    kkprintf("Gfaschannel: All Logical Unit specific resources have been deallocated.\n");
  }

  if (statPtr->usrst != NULL) {
    kkprintf("Gfaschannel: User-defined statics data table deallocation - ");
    sysfree((char*)statPtr->usrst, (long)sizeof(GFASCHANNELUserStatics_t));
    kkprintf("OK\n");
  }

  kkprintf("Gfaschannel: Device info table structure deallocation - ");
  sysfree((char*)statPtr->info, (long)sizeof(DevInfo_t));
  kkprintf("OK\n");

  kkprintf("Gfaschannel: Statics structure deallocation - ");
  sysfree((char*)statPtr, (long)sizeof(GFASCHANNELStatics_t));
  kkprintf("OK\n");
}




/**
 * @brief Entry point function.
 *
 * @param info --
 *
 * Initializes the hardware and allocates shared memory buffers. Invoked each
 * time the device driver is installed for a major device (i.e. each time
 * when @b cdv_install() or @b bdv_install() functions are called).
 *
 * @return pointer to a statics data structure - if succeed.
 * @return SYSERR                              - in case of failure.
 */
#define INST_OK()						\
do {								\
  kkprintf("Gfaschannel: Simulator (LUN %d) ", info->mlun);	\
  kkprintf(" installation Complete. ");				\
  DisplayVersion(NULL);						\
  kkprintf("\n\n");						\
} while (0)

#define INST_FAIL()						\
do {								\
  kkprintf("Gfaschannel: Simulator (LUN %d) ", info->mlun);	\
  kkprintf(" installation Fails. ");				\
  DisplayVersion(NULL);						\
  kkprintf("\n\n");						\
} while (0)

char* Gfaschannel_install(void *infofile)
{
	register GFASCHANNELStatics_t *statPtr;
	char *usrcoco; /* completion code of user entry point function */
	int cntr;
	AddrInfo_t *aiptr;
	DevInfo_t *info;
	int res __attribute__((unused));

	kkprintf("Gfaschannel: Device info table structure allocation - ");
	info = (DevInfo_t *)sysbrk(sizeof(DevInfo_t));
	if (!info) {
		/* Horrible failure */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return (char *)SYSERR; /* -1 */
	}
	kkprintf("OK\n");

	kkprintf("Gfaschannel: Copying info table from user space - ");
	memcpy(info, infofile, sizeof(DevInfo_t));
	kkprintf("OK\n");


  if (info->debugFlag & DBGINSTL) {	/* only in case of debugging */
    aiptr = &(info->addr1);
    kkprintf("\n\nSimulator install procedure parameters are:\n");
    kkprintf("+-----------------------------------------\n");
    /* info about 2 address spaces */
    for (cntr = 0; cntr < 2; cntr++) {
      kkprintf("|    Addr%d info:\n", cntr+1);
      kkprintf("|    -----------\n");
      if (aiptr->baseAddr != ADCG__NO_ADDRESS) {
	kkprintf("|     Base address      => 0x%x\n", aiptr->baseAddr);
	kkprintf("|     Address range     => 0x%x\n", aiptr->range);
	kkprintf("|     Address increment => 0x%x\n", aiptr->increment);
	kkprintf("|     Dataport size     => %d\n", aiptr->dpSize);
	kkprintf("|     Address modifier  => %s (0x%x)\n|\n", (aiptr->addrModif == DG_AM_SH)?"SH":(aiptr->addrModif == DG_AM_ST)?"ST":(aiptr->addrModif == DG_AM_EX)?"EX":(aiptr->addrModif == DG_AM_CR)?"CR":"UNKNOWN", aiptr->addrModif);
      } else kkprintf("|     Addr%d NOT defined.\n|\n", cntr+1);
      aiptr++;
    }

    kkprintf("| Debug flag                 => 0x%x\n", info->debugFlag);
    kkprintf("| Opaque parameter           => '%s'\n", info->opaque);
    kkprintf("| Interrupt level            => %d\n", info->iLevel);
    kkprintf("| Interrupt vector           => %d\n", info->iVector);
    kkprintf("| Interrupt vector increment => %d\n", info->iVectorInc);
    kkprintf("| Number of channels         => %d\n", info->chan);
    kkprintf("+-----------------------------------------\n\n");

    if (info->debugFlag & DBGALLDI) {
      /* user wants really everything, so let him have it! */
      REGDESC *rdp = info->regDesc;
      BLKDESC *bdp = info->blkDesc;
      aiptr = &(info->addr1);

      /* 2 address space info */
      for (cntr = 0; cntr < 2; cntr++) {
	kkprintf("+-----------------------------\n");
	kkprintf("| Addr[%d].baseAddr  => 0x%X\n", cntr, aiptr->baseAddr);
	kkprintf("| Addr[%d].range     => 0x%X\n", cntr, aiptr->range);
	kkprintf("| Addr[%d].increment => 0x%X\n", cntr, aiptr->increment);
	kkprintf("| Addr[%d].dpSize    => 0x%d\n", cntr, aiptr->dpSize);
	kkprintf("| Addr[%d].addrModif => 0x%d\n", cntr, aiptr->addrModif);
	kkprintf("+-----------------------------\n\n");
	aiptr++;
      }

      /* general info */
      kkprintf("+-----------------------------\n");
      kkprintf("| 'mtn'        => %d\n",   info->mtn);
      kkprintf("| 'debugFlag'  => 0x%X\n", info->debugFlag);
      kkprintf("| 'opaque'     => '%s'\n", info->opaque);
      kkprintf("| 'iLevel'     => %d\n",   info->iLevel);
      kkprintf("| 'iVector'    => %d\n",   info->iVector);
      kkprintf("| 'iVectorInc' => %d\n",   info->iVectorInc);
      kkprintf("| 'chan'       => %d\n",   info->chan);
      kkprintf("| 'chrindex'   => %d\n",   info->chrindex);
      kkprintf("| 'gen_date'   => %ld\n",  info->gen_date);
      kkprintf("| 'dg_vers'    => '%s'\n", info->dg_vers);
      kkprintf("| 'regAmount'  => %d\n",   info->regAmount);
      kkprintf("| 'maxRegSz'   => %d\n",   info->maxRegSz);
      kkprintf("| 'checksum'   => %d\n",   info->checksum);
      kkprintf("+-----------------------------\n\n");

      /* all block info */
      for (cntr = 0; cntr < info->blkAmount; cntr++) {
	kkprintf("+---------------------------------------------\n");
	kkprintf("| Blk[%d] 'block'       => %d\n", cntr, bdp[cntr].block);
	kkprintf("| Blk[%d] 'blkBaseAddr' => %d\n", cntr,
		 bdp[cntr].blkBaseAddr);
	kkprintf("| Blk[%d] 'offset'      => 0x%lX\n", cntr, bdp[cntr].offset);
	kkprintf("| Blk[%d] 'blksz_drvr'  => %d\n", cntr, bdp[cntr].blksz_drvr);
	kkprintf("| Blk[%d] 'blksz_sim'   => %d\n", cntr, bdp[cntr].blksz_sim);
	kkprintf("| Blk[%d] 'reg_am'      => %d\n", cntr, bdp[cntr].reg_am);
	kkprintf("+---------------------------------------------\n\n");
      }

      /* all registers info */
      for (cntr = 0; cntr < info->regAmount; cntr++) {
	kkprintf("+---------------------------------------------\n");
	kkprintf("| Reg[%d] 'ID'           => %d\n", cntr, rdp[cntr].regId);
	kkprintf("| Reg[%d] 'regName'      => %s\n", cntr, rdp[cntr].regName);
	kkprintf("| Reg[%d] 'busType'      => %s\n", cntr, rdp[cntr].busType);
	kkprintf("| Reg[%d] 'regType'      => %s (%d)\n", cntr, (rdp[cntr].regType == RT_REAL)?"RT_REAL":(rdp[cntr].regType == RT_EXTR)?"RT_EXTR":"RT_SRVS", rdp[cntr].regType);
	kkprintf("| Reg[%d] 'regSize'      => %d\n", cntr, rdp[cntr].regSize);
	kkprintf("| Reg[%d] 'b_a'          => %s\n", cntr, rdp[cntr].b_a);
	kkprintf("| Reg[%d] 'bid'          => %d\n", cntr, rdp[cntr].bid);
	kkprintf("| Reg[%d] 'regOffset'    => 0x%X\n", cntr, rdp[cntr].regOffset);
	kkprintf("| Reg[%d] 'regDepth'     => %d\n", cntr, rdp[cntr].regDepth);
	kkprintf("| Reg[%d] 'regtl'        => %d\n", cntr, rdp[cntr].regtl);
	kkprintf("| Reg[%d] 'regar'        => %d\n", cntr, rdp[cntr].regar);
	kkprintf("| Reg[%d] 'rwIoctlOpNum' => 0x%x\n", cntr, rdp[cntr].rwIoctlOpNum);
	kkprintf("+---------------------------------------------\n\n");
      }
    }
  }

  kkprintf("Gfaschannel: Simulator (LUN %d) installation begins...\n", info->mlun);

#ifdef __Lynx__
  GlobCalVal = CalibrateTime(info); /* do timing calibration */
  if (info->debugFlag & DBGTIMESTAT) /* time statistics is enabled */
     kkprintf("Gfaschannel: Calibrated timing is %dns per access.\n",
	      GlobCalVal);
#endif

  /*
     Should we disable interrupts here?
     int ps;
     disable(ps);
  */

	/* For device driver resources, resource allocation must be done during
	   the installation routine (e.g.: decriptor of system call interface:
	   semaphore, system thread, etc.) */

	/* 0x0 */
	kkprintf("Gfaschannel: Statics data structure allocation - ");
	if(!(statPtr = (GFASCHANNELStatics_t*)
	     sysbrk((long)sizeof(GFASCHANNELStatics_t)))) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return (char*)SYSERR; /* -1 */
	}
	kkprintf("OK\n");

	/* 0x1 */
	kkprintf("Gfaschannel: Hardware topology structure allocation - ");
	if( !(statPtr->card = (GFASCHANNELTopology_t*)
	      sysbrk((long)sizeof(GFASCHANNELTopology_t))) ) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return (char*)SYSERR; /* -1 */
	}
	bzero((char*)statPtr->card, sizeof(GFASCHANNELTopology_t));
	kkprintf("OK\n");

	/* 0x2 */
	if (sizeof(GFASCHANNELWriteonly_t) > 0) {
		kkprintf("Gfaschannel: Writeonly structure allocation - ");
		if ( !(statPtr->card->wo = (GFASCHANNELWriteonly_t*)
		       sysbrk((long)sizeof(GFASCHANNELWriteonly_t))) ) {
			/* Horrible, impossible failure. */
			kkprintf("FAILED\n");
			pseterr(ENOMEM);
			INST_FAIL();
			return (char*)SYSERR;	/* -1 */
		}
		bzero((char*)statPtr->card->wo, sizeof(GFASCHANNELWriteonly_t));
		kkprintf("OK\n");
	} else {
		statPtr->card->wo = NULL;
	}

	/* 0x3 */
	if (sizeof(GFASCHANNELExtraneous_t) > 0) {
		kkprintf("Gfaschannel: Extraneous structure allocation - ");
		if ( !(statPtr->card->ex = (GFASCHANNELExtraneous_t*)
		       sysbrk((long)sizeof(GFASCHANNELExtraneous_t))) ) {
			/* Horrible, impossible failure. */
			kkprintf("FAILED\n");
			pseterr(ENOMEM);
			INST_FAIL();
			return (char*)SYSERR;	/* -1 */
		}
		bzero((char*)statPtr->card->ex, sizeof(GFASCHANNELExtraneous_t));
		kkprintf("OK\n");
	} else {
		statPtr->card->ex = NULL;
	}

	/* 0x4 */
	kkprintf("Gfaschannel: Block 0 allocation - ");
	if ( !(statPtr->card->block00 = (GFASCHANNELBlock00_t *)sysbrk((long)sizeof(GFASCHANNELBlock00_t))) ) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return((char*)SYSERR); /* -1 */
	}

	bzero((char*)statPtr->card->block00, sizeof(GFASCHANNELBlock00_t));
	kkprintf("OK\n");

	kkprintf("Gfaschannel: Block 1 allocation - ");
	if ( !(statPtr->card->block01 = (GFASCHANNELBlock01_t *)sysbrk((long)sizeof(GFASCHANNELBlock01_t))) ) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return((char*)SYSERR); /* -1 */
	}

	bzero((char*)statPtr->card->block01, sizeof(GFASCHANNELBlock01_t));
	kkprintf("OK\n");

	kkprintf("Gfaschannel: Block 2 allocation - ");
	if ( !(statPtr->card->block02 = (GFASCHANNELBlock02_t *)sysbrk((long)sizeof(GFASCHANNELBlock02_t))) ) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return((char*)SYSERR); /* -1 */
	}

	bzero((char*)statPtr->card->block02, sizeof(GFASCHANNELBlock02_t));
	kkprintf("OK\n");

	kkprintf("Gfaschannel: Block 3 allocation - ");
	if ( !(statPtr->card->block03 = (GFASCHANNELBlock03_t *)sysbrk((long)sizeof(GFASCHANNELBlock03_t))) ) {
		/* Horrible, impossible failure. */
		kkprintf("FAILED\n");
		pseterr(ENOMEM);
		INST_FAIL();
		return((char*)SYSERR); /* -1 */
	}

	bzero((char*)statPtr->card->block03, sizeof(GFASCHANNELBlock03_t));
	kkprintf("OK\n");

	/* 0x5 save info table pointer */
	statPtr->info = (void*)info;

	/* 0x6 */
	if (sizeof(GFASCHANNELUserStatics_t) > 0) { /* only if it's not empty */
		kkprintf("Gfaschannel: User-defined statics data table"
			 " allocation - ");
		if( !(statPtr->usrst = (GFASCHANNELUserStatics_t*)
		      sysbrk( (long)sizeof(GFASCHANNELUserStatics_t) )) ) {
			/* Horrible, impossible failure. */
			kkprintf("FAILED\n");
			pseterr(ENOMEM);
			INST_FAIL();
			return (char*)SYSERR; /* -1 */
		}
		bzero((char*)statPtr->usrst, sizeof(GFASCHANNELUserStatics_t));
		kkprintf("OK\n");
	} else {
		statPtr->usrst = NULL;
	}

	/* user entry point function call */
	usrcoco = GfaschannelUserInst(NULL, info, statPtr);
	if (usrcoco == (char*)SYSERR ) {
		INST_FAIL();
		GfaschannelMemCleanup(statPtr); /* free allocated memory */
		return (char*)SYSERR; /* -1 */
	}

	INST_OK();
	return (char*)statPtr;
}


/**
 * @brief Driver uninstallation.
 *
 * @param statPtr -- statics table
 *
 * Deallocates shared memory and clears used interrupt vectors.
 *
 * @return OK     - if succeed.
 * @return SYSERR - in case of failure.
 */
int Gfaschannel_uninstall(GFASCHANNELStatics_t *statPtr)
{
	int rc;

	/*
	  Should we disable interrupts?
	  int ps;
	  disable(ps);
	*/

	kkprintf("Gfaschannel: Uninstall...\n");

	/* user entry point function call */
	rc = GfaschannelUserUnInst(NULL, statPtr);
	if (rc == SYSERR )
		return rc; /* -1 */

	/* cleanup and release statics table */
	GfaschannelMemCleanup(statPtr);

	/*
	  Enable interrupts.
	  restore(ps);
	*/

	kkprintf("Gfaschannel: Done\n\n");

	return OK;
}

/* GFASCHANNEL entry points */
#ifdef __linux__
struct dldd entry_points = {
	(int(*)(void*, int, struct cdcm_file*))
	/* open      */ Gfaschannel_open,
	 (int(*)(void*, struct cdcm_file*))
	/* close     */ Gfaschannel_close,
	 (int(*)(void*, struct cdcm_file*, char*, int))
	/* read      */ Gfaschannel_read,
	 (int(*)(void*, struct cdcm_file*, char*, int))
	/* write     */ Gfaschannel_write,
	 (int(*)(void*, struct cdcm_file*, int, struct cdcm_sel*))
	/* select    */ Gfaschannel_select,
	 (int(*)(void*, struct cdcm_file*, int, char*))
	/* ioctl     */ Gfaschannel_ioctl,
	 (char* (*)(void*))
	/* install   */ Gfaschannel_install,
	 (int(*)(void*))
	/* uninstall */ Gfaschannel_uninstall,
	/* memory-mapped devices access */ NULL
};
#else
struct dldd entry_points = {
  /* open      */ Gfaschannel_open,
  /* close     */ Gfaschannel_close,
  /* read      */ Gfaschannel_read,
  /* write     */ Gfaschannel_write,
  /* select    */ Gfaschannel_select,
  /* ioctl     */ Gfaschannel_ioctl,
  /* install   */ Gfaschannel_install,
  /* uninstall */ Gfaschannel_uninstall,
  /* memory-mapped devices access */ NULL
};
#endif

