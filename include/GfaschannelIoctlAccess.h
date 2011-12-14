#ifndef _GFASCHANNEL_IOCTL_ACCESS_H_INCLUDE_
#define _GFASCHANNEL_IOCTL_ACCESS_H_INCLUDE_

#include "GfaschannelUserDefinedAccess.h"

/* this API is obsolete! */
#warning WARNING! Deprecated library. Use DrvrAccess library instead.

#ifdef __cplusplus
extern "C" {
#endif

/* see GfaschannelIoctlAccess.c for precise parameter description */

int  GfaschannelEnableAccess(int, int); /* open  Device driver */
void GfaschannelDisableAccess(int);     /* close Device driver */

int  GfaschannelGetWindowVECTORS1(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS1(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS1(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS1(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS2(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS2(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS2(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS2(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS3(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS3(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS3(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS3(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS4(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS4(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS4(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS4(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS5(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS5(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS5(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS5(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS6(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS6(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS6(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS6(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS7(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS7(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS7(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS7(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS8(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS8(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS8(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS8(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS9(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS9(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS9(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS9(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS10(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS10(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS10(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS10(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS11(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS11(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS11(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS11(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS12(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS12(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS12(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS12(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS13(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS13(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS13(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS13(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS14(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS14(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS14(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS14(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS15(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS15(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS15(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS15(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS16(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS16(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS16(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS16(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS17(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS17(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS17(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS17(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS18(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS18(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS18(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS18(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS19(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS19(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS19(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS19(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS20(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS20(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS20(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS20(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS21(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS21(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS21(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS21(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS22(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS22(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS22(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS22(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS23(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS23(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS23(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS23(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS24(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS24(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS24(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS24(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS25(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS25(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS25(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS25(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS26(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS26(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS26(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS26(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS27(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS27(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS27(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS27(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS28(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS28(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS28(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS28(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS29(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS29(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS29(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS29(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS30(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS30(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS30(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS30(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS31(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS31(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS31(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS31(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowVECTORS32(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetVECTORS32(int fd, unsigned short result[1536]);
int  GfaschannelSetWindowVECTORS32(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetVECTORS32(int fd, unsigned short arg[1536]);
int  GfaschannelGetWindowFUNC_CONVERT(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetFUNC_CONVERT(int fd, unsigned short result[2]);
int  GfaschannelSetWindowFUNC_CONVERT(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetFUNC_CONVERT(int fd, unsigned short arg[2]);
int  GfaschannelGetFUNC_SELECT(int fd, unsigned short *result);
int  GfaschannelSetFUNC_SELECT(int fd, unsigned short arg);
int  GfaschannelGetCONFIG(int fd, unsigned short *result);
int  GfaschannelSetCONFIG(int fd, unsigned short arg);
int  GfaschannelGetSLOPE(int fd, unsigned short *result);
int  GfaschannelSetSLOPE(int fd, unsigned short arg);
int  GfaschannelGetDAC_VAL(int fd, unsigned short *result);
int  GfaschannelSetDAC_VAL(int fd, unsigned short arg);
int  GfaschannelGetRECURR_CYCLES(int fd, unsigned short *result);
int  GfaschannelSetRECURR_CYCLES(int fd, unsigned short arg);
int  GfaschannelGetWindowFUNC_DISABLE(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetFUNC_DISABLE(int fd, unsigned short result[2]);
int  GfaschannelSetWindowFUNC_DISABLE(int fd, unsigned int elOffs, unsigned int depth, unsigned short *arg);
int  GfaschannelSetFUNC_DISABLE(int fd, unsigned short arg[2]);
int  GfaschannelGetFORCE_CONVERT(int fd, unsigned short *result);
int  GfaschannelSetFORCE_CONVERT(int fd, unsigned short arg);
int  GfaschannelGetCONVERT_FUNC(int fd, unsigned short *result);
int  GfaschannelSetCONVERT_FUNC(int fd, unsigned short arg);
int  GfaschannelGetSELECT_FUNC(int fd, unsigned short *result);
int  GfaschannelSetSELECT_FUNC(int fd, unsigned short arg);
int  GfaschannelGetSOFT_START(int fd, unsigned short *result);
int  GfaschannelSetSOFT_START(int fd, unsigned short arg);
int  GfaschannelGetSOFT_EV_START(int fd, unsigned short *result);
int  GfaschannelSetSOFT_EV_START(int fd, unsigned short arg);
int  GfaschannelGetWindowACCEPT_STATUS(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetACCEPT_STATUS(int fd, unsigned short result[2]);
int  GfaschannelGetSTATUS(int fd, unsigned short *result);
int  GfaschannelGetWindowDISABLE_STATUS(int fd, unsigned int elOffs, unsigned int depth, unsigned short *result);
int  GfaschannelGetDISABLE_STATUS(int fd, unsigned short result[2]);

#ifdef __cplusplus
}
#endif

#endif /* _GFASCHANNEL_IOCTL_ACCESS_H_INCLUDE_ */
