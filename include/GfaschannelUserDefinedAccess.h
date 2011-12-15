#ifndef _GFASCHANNEL_USER_DEFINED_ACCESS_H_INCLUDE_
#define _GFASCHANNEL_USER_DEFINED_ACCESS_H_INCLUDE_

#include <GfaschannelDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GFASHandle gfasOpen(int lun);

int gfasClose(GFASHandle handle);

int gfasConfigure(GFASHandle handle, unsigned short channel, GFASConfig config);

int gfasReadDriverVersion(GFASHandle handle);

int gfasReadLibVersion(GFASHandle handle, char* version);

int gfasLoadFunction(GFASHandle handle, unsigned short channel, unsigned short slot, const GFASFunction* function);

int gfasReadFunction(GFASHandle handle, unsigned short channel, unsigned short slot, GFASFunction* function);

int gfasSelectFunction(GFASHandle handle, unsigned short channel, unsigned short slot);

int gfasWriteRecurrentCycles(GFASHandle handle, unsigned short channel, long nRecurrences);

int gfasEnableFunction(GFASHandle handle, unsigned short channel, unsigned short slot);

int gfasDisableFunction(GFASHandle handle, unsigned short channel, unsigned short slot);

int gfasDisableAllFunctions(GFASHandle handle, unsigned short channel);

int gfasSendSoftwarePulse(GFASHandle handle, unsigned short channel, GFASInputPulse pulse);

int gfasReset(GFASHandle handle, unsigned short channel);

char* gfasStrError(int errNo);

#ifdef __cplusplus
}
#endif

#endif /* _GFASCHANNEL_USER_DEFINED_ACCESS_H_INCLUDE_ */
