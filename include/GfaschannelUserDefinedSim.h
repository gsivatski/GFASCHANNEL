#ifndef _GFASCHANNEL_USER_DEFINED_SIM_H_INCLUDE_
#define _GFASCHANNEL_USER_DEFINED_SIM_H_INCLUDE_

#include "GfaschannelSim.h"

struct sel; /* preliminary structure declaration to supress warnings during
	       user code compilation */

/* user-defined statics data table for GFASCHANNEL module */
struct GFASCHANNELUserStatics_t {
  /*
    +=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+
    |                     insert your new members here                        |
    | NOTE! All that will be placed here should be managed by you, and you    |
    | alone!!! All the pointers-related memory allocation and deallocation    |
    | should be done by you. It will NOT be done automatically! If the        |
    | pointer is declared - it should be initialized properly by allocating   |
    | a new memory (using sysbrk) or by assigning it a correct and valid      |
    | address. Don't forget to free allocated memory (if any) in the          |
    | uninstallation procedure! Have fun (-;                                  |
    +=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+
  */
};



/*
  +-------------------------------+
  | INSERT USER-DEFINED CODE HERE |
  +-------------------------------+
*/


int GfaschannelUserOpen(int*, register GFASCHANNELStatics_t*, int, struct file*);
int GfaschannelUserClose(int*, register GFASCHANNELStatics_t*, struct file*);
int GfaschannelUserRead(int*, register GFASCHANNELStatics_t*, struct file*, char*, int);
int GfaschannelUserWrite(int*, register GFASCHANNELStatics_t*, struct file*, char*, int);
int GfaschannelUserSelect(int*, register GFASCHANNELStatics_t*, struct file*, int, struct sel*);
int GfaschannelUserIoctl(int*, register GFASCHANNELStatics_t*, struct file*, int, int, char*);
char* GfaschannelUserInst(int*, register DevInfo_t*, register GFASCHANNELStatics_t*);
int GfaschannelUserUnInst(int*, GFASCHANNELStatics_t*);

#endif /* _GFASCHANNEL_USER_DEFINED_SIM_H_INCLUDE_ */
