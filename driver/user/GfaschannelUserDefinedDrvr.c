#include "GfaschannelUserDefinedDrvr.h"

#ifdef __powerpc__
#include <vme.h>
#endif

/* for __inb, __outb etc... */
#ifdef __Lynx__
#include "dg/port_ops_lynx.h"
#else  /* __linux__ */
#include "dg/port_ops_linux.h"
#include "dg/swab-extra-linux.h"
#endif


/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  User Entry Points into Driver/Simulator are located here.
  This functions are called in an appropriate driver/simulator routines.
  All user-defined driver/simulator code should be added in this functions.
  Also Interrupt Service Routine is located here, as it's implementation is
  _completely_ up to the user.
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/



#if defined (__LYNXOS)
/* from linux/irqreturn.h */
/**
 * enum irqreturn
 * @IRQ_NONE            interrupt was not from this device
 * @IRQ_HANDLED         interrupt was handled by this device
 * @IRQ_WAKE_THREAD     handler requests to wake the handler thread
 */
enum irqreturn {
	IRQ_NONE,
	IRQ_HANDLED,
	IRQ_WAKE_THREAD,
};
#endif
/**
 * @brief Interrupt Service Routine.
 *
 * @param stPtr -- Statics table pointer
 *
 * Implementation is @b completely up to the user.
 *
 * @return IRQ_NONE        --  interrupt was not from this device
 * @return IRQ_HANDLED     --  interrupt was handled by this device
 * @return IRQ_WAKE_THREAD --  handler requests to wake the handler thread
 */
static int __attribute__((unused)) GfaschannelISR(GFASCHANNELStatics_t *ptr)
{
	GFASCHANNELStatics_t *stPtr;
	stPtr = (GFASCHANNELStatics_t *)ptr;

	return IRQ_HANDLED;
}


/**
 * @brief User entry point in driver/simulator open routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param d       --
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function\n
 *         OK     - if succeed.\n
 *         SYSERR - in case of failure.
 */
int GfaschannelUserOpen(int *proceed, register GFASCHANNELStatics_t *sptr,
			int d, struct file *f)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return OK; /* succeed */
}

/**
 * @brief User entry point in driver/simulator close routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function\n
 *         OK     - if succeed.\n
 *         SYSERR - in case of failure.
 */
int GfaschannelUserClose(int *proceed, register GFASCHANNELStatics_t *sptr,
			 struct file *f)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return OK; /* succeed */
}

/**
 * @brief User entry point in driver/simulator read routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param buff    -- character buffer pointer
 * @param count   -- number of bytes to copy
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function.\n
 *         number of bytes actually copied, including zero - if succeed.\n
 *         SYSERR                                          - if fails.
 */
int GfaschannelUserRead(int *proceed, register GFASCHANNELStatics_t *sptr,
			struct file *f, char *buff, int count)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return count; /* number of read bytes */
}

/**
 * @brief User entry point in driver/simulator write routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param buff    -- char buffer pointer
 * @param count   -- the number of bytes to copy
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function.\n
 *         number of bytes actually copied, including zero - if succeed.\n
 *         SYSERR                                          - if fails.
 */
int GfaschannelUserWrite(int *proceed, register GFASCHANNELStatics_t *sptr,
			 struct file *f, char *buff, int count)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return count; /* number of written bytes */
}

/**
 * @brief User entry point in driver/simulator select routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param which   -- condition to monitor. Not valid in case of Linux!\n
 *                   <b> SREAD, SWRITE, SEXCEPT </b> in case of Lynx.
 * @param ffs     -- select data structure in case of Lynx.\n
 *                   struct poll_table_struct in case of Linux.
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function\n
 *         OK     - if succeed.\n
 *         SYSERR - in case of failure.
 */
int GfaschannelUserSelect(int *proceed, register GFASCHANNELStatics_t *sptr,
			  struct file *f, int which, struct sel *ffs)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return OK; /* succeed */
}

/**
 * @brief User entry point in driver/simulator ioctl routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 * @param f       -- file pointer. Lynx/Linux specific.
 *                   See (sys/file.h) for Lynx and (linux/fs.h) for Linux.
 * @param lun     -- minor number (LUN)
 * @param com     -- ioctl number
 * @param arg     -- ioctl arguments
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function\n
 *         OK     - if succeed.\n
 *         SYSERR - in case of failure.
 */
int GfaschannelUserIoctl(int *proceed, register GFASCHANNELStatics_t *sptr,
			 struct file *f, int lun, int com, char *arg)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;


	/*
	  +-------------------------------+
	  | INSERT USER-DEFINED CODE HERE |
	  +-------------------------------+
	*/


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return OK; /* succeed */
}

/**
 * @brief User entry point in driver/simulator installation routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param info    -- driver info table
 * @param sptr    -- statics table
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function.\n
 *         pointer to a statics data structure - if succeed.\n
 *         SYSERR                              - in case of failure.
 */
char* GfaschannelUserInst(int *proceed, register DevInfo_t *info,
			  register GFASCHANNELStatics_t *sptr)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */
	int iVec = 0;			/* interrupt vector */

	usp  = sptr->usrst;
	iVec = info->iVector;		/* set up interrupt vector */

	/* Uncomment the following code to register ISR */
#if 0
	if (iVec > 0) {
		int cc = 0; /* completion code */
		kkprintf("ISR ( vector number [%d] ) installation - ", iVec);
#ifdef __Lynx__
#ifdef __powerpc__ /* in this case we are using CES BSP */
		cc = vme_intset(iVec, (int (*)())GfaschannelISR,
				  (char*)sptr, 0);
#else  /* use standard system call otherwise */
		cc = iointset(iVec, (int (*)())GfaschannelISR, (char*)sptr);
#endif
#else  /* __linux__ */
		cc = vme_request_irq(iVec,
				     (int (*)(void *))GfaschannelISR,
				     (char *)sptr,
				     "GfaschannelD");
#endif /* __Lynx__ */
		if (cc < 0) {
			kkprintf("Failed.\n");
			pseterr(EFAULT); /* TODO. what error to set? */
			return (char*)SYSERR;	/* -1 */
		}
		kkprintf("interrupt vector managed.\n");
	}
#endif

	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return (char *)sptr; /* succeed */
}

/**
 * @brief User entry point in driver/simulator unistall routine.
 *
 * @param proceed -- if standard code execution should be proceed
 * @param sptr    -- statics table pointer
 *
 * It's up to user to set kernel-level errno (by means of @e pseterr call).
 * @e proceed parameter denotes if further standard actions should be proceed
 * after function returns. @b FALSE - means that user-desired operation done
 * all that user wants and there is no further necessaty to perfom any standard
 * operations that follow function call. @b TRUE - means that code that follows
 * function call will be executed.
 *
 * @return return value is the same as in entry point function.\n
 *         OK     - if succeed.\n
 *         SYSERR - in case of failure.
 */
int GfaschannelUserUnInst(int *proceed, GFASCHANNELStatics_t *sptr)
{
	GFASCHANNELUserStatics_t *usp; /* user statistics table */

	usp = sptr->usrst;

	/* Uncomment the following code to unregister ISR */
#if 0
	kkprintf("Gfaschannel: Interrupt routine managment"
		 " cleanup ( vector number [%d] ) - ", sptr->info->iVector);
#ifdef __Lynx__
#ifdef __powerpc__
	/* in this case we are using CES BSP */
	vme_intclr(sptr->info->iVector, 0);
#else
	iointclr(sptr->info->iVector);
#endif
#else  /* __linux__ */
	vme_free_irq(sptr->info->iVector);
#endif	/* __Lynx__ */

	kkprintf("OK\n");
#endif


	if (proceed)
		*proceed = TRUE; /* continue standard code execution */

	return OK; /* succeed */
}
