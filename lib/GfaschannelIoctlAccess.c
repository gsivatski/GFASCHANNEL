#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <stdlib.h>
#include "GfaschannelDrvr.h"

#define _DRVR_NM_ "GFASCHANNEL"

#define matchEndian(bytes, size, depth) { }

/**
 * @brief Open device driver node
 *
 * @param lun   -- Logical Unit Number assigned to the module. Negative in case
 *                 of driver simulator
 * @param chanN -- Minor Device Number. There can be several entry points for
 *                 current Logical Unit Number (ChannelNumber).
 *
 * @return Open file decriptor (normally >= 3) - if success
 * @return -1                                  - if fails
 *                                               Error message is printing out.
 */
int GfaschannelEnableAccess(int lun, int chanN)
{
	int  fd;			/* open file descriptor */
	char fileName[0x100];		/* device file name */
	char *tmp;

	if (!MODULE_NAME_OK(_DRVR_NM_)) {
		fprintf(stderr, "Spurious Module Name '%s'.\n"
			"Normally _should not_ contain any lowercase"
			" letters!\n",  _DRVR_NM_);
		return -1;
	}

	tmp = _ncf(_DRVR_NM_);
	sprintf(fileName, "/dev/" NODE_NAME_FMT,
		tmp, (lun < 0)?_SIML_:_DRVR_, abs(lun), chanN);
	free(tmp);
	if ((fd = open(fileName, O_RDWR)) < 0) { /* error */
		perror(NULL);
		fprintf(stderr, "Error [%s] in GfaschannelEnableAccess()"
			" while opening '%s' file.\nCheck if '%s' module is"
			" installed.\n", strerror(errno), fileName, _DRVR_NM_);
		return -1;
	}

	return fd;
}

/**
 * @brief  Close driver file descriptor.
 *
 * @param fd -- open file descriptor, retuned by GfaschannelEnableAccess call
 *
 * @return void
 */
void GfaschannelDisableAccess(int fd)
{
	close(fd);
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS1(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS1, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS1(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS1(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS1(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS1, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS1(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS1(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS2(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS2, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS2(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS2(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS2(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS2, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS2(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS2(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS3(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS3, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS3(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS3(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS3(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS3, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS3(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS3(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS4(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS4, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS4(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS4(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS4(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS4, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS4(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS4(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS5(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS5, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS5(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS5(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS5(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS5, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS5(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS5(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS6(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS6, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS6(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS6(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS6(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS6, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS6(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS6(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS7(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS7, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS7(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS7(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS7(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS7, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS7(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS7(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS8(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS8, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS8(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS8(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS8(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS8, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS8(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS8(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS9(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS9, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS9(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS9(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS9(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS9, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS9(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS9(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS10(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS10, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS10(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS10(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS10(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS10, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS10(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS10(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS11(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS11, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS11(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS11(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS11(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS11, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS11(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS11(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS12(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS12, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS12(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS12(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS12(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS12, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS12(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS12(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS13(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS13, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS13(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS13(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS13(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS13, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS13(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS13(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS14(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS14, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS14(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS14(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS14(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS14, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS14(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS14(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS15(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS15, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS15(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS15(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS15(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS15, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS15(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS15(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS16(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS16, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS16(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS16(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS16(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS16, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS16(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS16(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS17(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS17, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS17(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS17(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS17(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS17, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS17(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS17(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS18(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS18, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS18(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS18(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS18(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS18, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS18(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS18(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS19(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS19, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS19(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS19(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS19(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS19, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS19(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS19(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS20(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS20, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS20(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS20(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS20(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS20, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS20(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS20(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS21(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS21, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS21(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS21(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS21(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS21, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS21(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS21(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS22(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS22, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS22(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS22(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS22(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS22, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS22(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS22(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS23(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS23, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS23(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS23(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS23(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS23, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS23(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS23(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS24(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS24, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS24(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS24(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS24(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS24, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS24(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS24(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS25(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS25, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS25(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS25(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS25(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS25, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS25(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS25(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS26(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS26, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS26(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS26(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS26(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS26, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS26(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS26(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS27(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS27, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS27(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS27(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS27(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS27, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS27(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS27(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS28(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS28, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS28(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS28(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS28(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS28, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS28(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS28(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS29(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS29, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS29(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS29(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS29(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS29, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS29(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS29(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS30(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS30, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS30(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS30(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS30(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS30, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS30(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS30(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS31(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS31, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS31(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS31(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS31(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS31, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS31(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS31(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowVECTORS32(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_VECTORS32, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 1536);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetVECTORS32(
			     int fd,
			     unsigned short result[1536])
{
	return GfaschannelGetWindowVECTORS32(fd, 0, 1536, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowVECTORS32(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 1536);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_VECTORS32, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetVECTORS32(
			     int fd,
			     unsigned short arg[1536])
{
	return GfaschannelSetWindowVECTORS32(fd, 0, 1536, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowFUNC_CONVERT(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_FUNC_CONVERT, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 2);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetFUNC_CONVERT(
			     int fd,
			     unsigned short result[2])
{
	return GfaschannelGetWindowFUNC_CONVERT(fd, 0, 2, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowFUNC_CONVERT(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 2);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_FUNC_CONVERT, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetFUNC_CONVERT(
			     int fd,
			     unsigned short arg[2])
{
	return GfaschannelSetWindowFUNC_CONVERT(fd, 0, 2, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetFUNC_SELECT(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_FUNC_SELECT, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetFUNC_SELECT(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_FUNC_SELECT, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetCONFIG(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_CONFIG, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetCONFIG(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_CONFIG, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetSLOPE(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_SLOPE, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetSLOPE(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_SLOPE, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetDAC_VAL(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_DAC_VAL, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetDAC_VAL(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_DAC_VAL, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetRECURR_CYCLES(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_RECURR_CYCLES, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetRECURR_CYCLES(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_RECURR_CYCLES, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowFUNC_DISABLE(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_FUNC_DISABLE, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 2);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetFUNC_DISABLE(
			     int fd,
			     unsigned short result[2])
{
	return GfaschannelGetWindowFUNC_DISABLE(fd, 0, 2, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to set
 * @param arg    -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetWindowFUNC_DISABLE(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) arg; /* where to take data from */
	arguments[1] = depth;               /* number of elements write */
	arguments[2] = elOffs;              /* element index */

	/* handle endianity */
	matchEndian((char*)arg, sizeof(unsigned short), 2);

	/* driver call */
	return ioctl(fd, GFASCHANNEL_SET_FUNC_DISABLE, (char *)arguments);
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- buffer holds new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetFUNC_DISABLE(
			     int fd,
			     unsigned short arg[2])
{
	return GfaschannelSetWindowFUNC_DISABLE(fd, 0, 2, arg);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetFORCE_CONVERT(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_FORCE_CONVERT, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetFORCE_CONVERT(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_FORCE_CONVERT, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetCONVERT_FUNC(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_CONVERT_FUNC, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetCONVERT_FUNC(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_CONVERT_FUNC, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetSELECT_FUNC(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_SELECT_FUNC, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetSELECT_FUNC(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_SELECT_FUNC, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetSOFT_START(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_SOFT_START, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetSOFT_START(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_SOFT_START, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetSOFT_EV_START(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_SOFT_EV_START, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd  -- driver node descriptor
 * @param arg -- new values
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelSetSOFT_EV_START(
			     int fd,
			     unsigned short arg)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) &arg; /* where to take data from */
	arguments[1] = 1;		     /* number of elements write */
	arguments[2] = 0;		     /* element index */

	/* handle endianity */
	matchEndian((char*)&arg, sizeof(unsigned short), 0);

	return ioctl(fd, GFASCHANNEL_SET_SOFT_EV_START, (char *)arguments);

}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowACCEPT_STATUS(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_ACCEPT_STATUS, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 2);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetACCEPT_STATUS(
			     int fd,
			     unsigned short result[2])
{
	return GfaschannelGetWindowACCEPT_STATUS(fd, 0, 2, result);
}


/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetSTATUS(
			     int fd,
			     unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result; /* where to put results */
	arguments[1] = 1;		       /* number of elements to read */
	arguments[2] = 0;		       /* element index */

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_STATUS, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 0);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param elOffs -- element offset (expressed in elements)
 * @param depth  -- number of elemets to get
 * @param result -- buffer for the results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetWindowDISABLE_STATUS(
				   int fd,
				   unsigned int elOffs,
				   unsigned int depth,
				   unsigned short *result)
{
	unsigned long arguments[3];

	/* pack ioctl args */
	arguments[0] = (unsigned long) result;
	arguments[1] = depth;
	arguments[2] = elOffs;

	/* driver call */
	if (ioctl(fd, GFASCHANNEL_GET_DISABLE_STATUS, (char*)arguments))
		return -1;

	/* handle endianity */
	matchEndian((char*)result, sizeof(unsigned short), 2);

	return 0;
}

/**
 * @brief
 *
 * @param fd     -- driver node descriptor
 * @param result -- buffer to put results
 *
 * @return  0 - on success.
 * @return -1 - if error occurs. errno is set appropriately.
 */
int GfaschannelGetDISABLE_STATUS(
			     int fd,
			     unsigned short result[2])
{
	return GfaschannelGetWindowDISABLE_STATUS(fd, 0, 2, result);
}


