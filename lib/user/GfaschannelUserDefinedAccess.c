#include "GfaschannelUserDefinedAccess.h"

/*
   If you want to implement your own library for module accessing, then you
   should do this in this file. API function prototypes should be declared
   in GfaschannelUserDefinedAccess.h file.
*/

#include <GfaschannelRegId.h>
#include <GfaschannelVersionDrvr.h>


static const unsigned short nBitsInByte = 8;
#define N_SLOTS 32

const int functionRegisterIDs[N_SLOTS]= { // Register ID look up table for registers used for storing functions
  VECTORS1_ID,
  VECTORS2_ID,
  VECTORS3_ID,
  VECTORS4_ID,
  VECTORS5_ID,
  VECTORS6_ID,
  VECTORS7_ID,
  VECTORS8_ID,
  VECTORS9_ID,
  VECTORS10_ID,
  VECTORS11_ID,
  VECTORS12_ID,
  VECTORS13_ID,
  VECTORS14_ID,
  VECTORS15_ID,
  VECTORS16_ID,
  VECTORS17_ID,
  VECTORS18_ID,
  VECTORS19_ID,
  VECTORS20_ID,
  VECTORS21_ID,
  VECTORS22_ID,
  VECTORS23_ID,
  VECTORS24_ID,
  VECTORS25_ID,
  VECTORS26_ID,
  VECTORS27_ID,
  VECTORS28_ID,
  VECTORS29_ID,
  VECTORS30_ID,
  VECTORS31_ID,
  VECTORS32_ID
};

const unsigned short gfasLowChannel = 0; // libgfas counts channels starting from 0
const unsigned short gfasLowSlot = 0; // libgfas counts function slots starting from 0
const unsigned long gfasVectorLength = 3;


static
int
getRegisterSize(GFASHandle handle, int registerId, int* registerDepth, int* registerElementSize)
{
  int registerSize = 0;

  *registerDepth = DaGetRegDepth(handle, registerId);
  if (*registerDepth <= 0) {
    return -GFAS_DAL_ERR;
  }
  *registerElementSize = DaGetRegSize(handle, registerId);
  if (*registerElementSize <= 0) {
    return -GFAS_DAL_ERR;
  }
  registerSize = (*registerDepth) * (*registerElementSize);

  return registerSize;
}


GFASHandle
gfasOpen(int lun)
{
  static char* moduleName = "GFASCHANNEL";
  static const METHOD moduleAccessMethod = IOCTL;
  static const int minorDeviceNo = 0;
  GFASHandle rc = -1;

  rc = DaEnableAccess(moduleName, moduleAccessMethod, lun, minorDeviceNo);
  if (rc < 0) {
    return -GFAS_DAL_ERR;
  }

  return rc;
}


int
gfasClose(GFASHandle handle)
{
  int rc = DaDisableAccess(handle);
  if (rc < 0) {
    return -GFAS_DAL_ERR;
  }

  return 0;
}


int
gfasConfigure(GFASHandle handle, unsigned short channel, GFASConfig config)
{
  static const int registerId = CONFIG_ID;
  GFASMode mode = config.mode;
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
/*   printf("Handle: %d; registerId: %d; registerDepth: %d; registerElementSize: %d\n", (int)handle, registerId, registerDepth, registerElementSize); */
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  switch (mode) {
    case GFAS_NORMAL:
      registerValue = 0;
      break;
    case GFAS_DAC:
      registerValue = 1;
      break;
    case GFAS_LOW_JITTER:
      registerValue = 2;
      break;
    case GFAS_NORMAL_RECURRENT:
      registerValue = 3;
      break;
    case GFAS_LOW_JITTER_RECURRENT:
      registerValue = 4;
      break;
    case GFAS_TEST1:
      registerValue = 5;
      break;
    case GFAS_TEST2:
      registerValue = 6;
      break;
    case GFAS_NO_FUNCTION_GENERATION:
      registerValue = 7;
      break;
    default:
      return -GFAS_OUT_OF_RANGE;
  }
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasReadDriverVersion(GFASHandle handle)
{
/*   int rc = 0; */
/*   rc = DaDrvrVers(handle, NULL, NULL); */
  return GFASCHANNEL_DRVR_CURRENT_VERSION;
}


int
gfasReadLibVersion(GFASHandle handle, char* version)
{
  sprintf(version, "%s", "\0");

  return 0;
}


int
gfasLoadFunction(GFASHandle handle, unsigned short channel, unsigned short slot, const GFASFunction* function)
{
  int registerId = functionRegisterIDs[slot];
/*   void* registerValue = (void*)function; */
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int registerElementSize_bits = 0;
  long registerElementNo = 0;
  long nOffsetElements = 0;
  unsigned int nRegisterElements = 1;
  unsigned short bitNo = 0;
  uint16_t mask = 0;
  int rc = 0;

  // Write function to VME memory
  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  rc = DaSetRegister(handle, registerId, (void*)function, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }
  // Set the conversion bit corresponding to the function
  registerId = FUNC_CONVERT_ID;
  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if (rc <= 0) {
    return rc;
  }
  registerSize = rc;
  registerElementSize_bits = registerElementSize * nBitsInByte;
  registerElementNo = slot / registerElementSize_bits; // Register element to which the bit corresponding to the 'slot' belongs
  nOffsetElements = registerDepth - registerElementNo - 1;
  rc = DaGetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }
  bitNo = slot % registerElementSize_bits;
  mask = 0x0001 << bitNo;
  registerValue = registerValue | mask;
  rc = DaSetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }
  // Set conversion flag (function will be transfered from VME memory to GFAS memory at next start/stop (?) pulse)
  registerId = CONVERT_FUNC_ID;
  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  // TODO: First check that the flag is 0
  registerValue = 1;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasReadFunction(GFASHandle handle, unsigned short channel, unsigned short slot, GFASFunction* function)
{
  const int registerId = functionRegisterIDs[slot];
  void* registerValue = (void*)function;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  rc = DaGetRegister(handle, registerId, registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasSelectFunction(GFASHandle handle, unsigned short channel, unsigned short slot)
{
  int registerId = FUNC_SELECT_ID;
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  registerValue = slot;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }
  registerId = SELECT_FUNC_ID;
  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  registerValue = 1;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasWriteRecurrentCycles(GFASHandle handle, unsigned short channel, long nRecurrences)
{
  static const int registerId = RECURR_CYCLES_ID;
  int16_t registerValue = nRecurrences;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasEnableFunction(GFASHandle handle, unsigned short channel, unsigned short slot)
{
  static const int registerId = FUNC_DISABLE_ID;
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int registerElementSize_bits = 0;
  long registerElementNo = 0;
  long nOffsetElements = 0;
  unsigned int nRegisterElements = 1;
  unsigned short bitNo = 0;
  uint16_t mask = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if (rc <= 0) {
    return rc;
  }
  registerSize = rc;
  registerElementSize_bits = registerElementSize * nBitsInByte;
  registerElementNo = slot / registerElementSize_bits; // Register element to which the bit corresponding to the 'slot' belongs
  nOffsetElements = registerDepth - registerElementNo - 1;
  rc = DaGetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }
  bitNo = slot % registerElementSize_bits;
  mask = ~(0x0001 << bitNo);
  registerValue = registerValue & mask;
  rc = DaSetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasDisableFunction(GFASHandle handle, unsigned short channel, unsigned short slot)
{
  static const int registerId = FUNC_DISABLE_ID;
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int registerElementSize_bits = 0;
  long registerElementNo = 0;
  long nOffsetElements = 0;
  unsigned int nRegisterElements = 1;
  unsigned short bitNo = 0;
  uint16_t mask = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if (rc <= 0) {
    return rc;
  }
  registerSize = rc;
  registerElementSize_bits = registerElementSize * nBitsInByte;
  registerElementNo = slot / registerElementSize_bits; // Register element to which the bit corresponding to the 'slot' belongs
  nOffsetElements = registerDepth - registerElementNo - 1;
  rc = DaGetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }
  bitNo = slot % registerElementSize_bits;
  mask = 0x0001 << bitNo;
  registerValue = registerValue | mask;
  rc = DaSetRegChunk(handle, registerId, nOffsetElements, nRegisterElements, &registerValue, registerElementSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != nRegisterElements) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasDisableAllFunctions(GFASHandle handle, unsigned short channel)
{
  static const int registerId = FUNC_DISABLE_ID;
  uint32_t registerValue = ~0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
/*   printf("Handle: %d; registerId: %d; registerDepth: %d; registerElementSize: %d\n", (int)handle, registerId, registerDepth, registerElementSize); */
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;

}


int
gfasSendSoftwarePulse(GFASHandle handle, unsigned short channel, GFASInputPulse pulse)
{
  int registerId = SOFT_START_ID;
  uint16_t registerValue = 0;
  int registerDepth = 0;
  int registerElementSize = 0;
  int registerSize = 0;
  int rc = 0;

  switch (pulse) {
    case GFAS_START:
      registerId = SOFT_START_ID;
      registerValue = 1;
      break;
    case GFAS_STOP:
      registerId = SOFT_START_ID;
      registerValue = 0;
      break;
    case GFAS_EVENT_START:
      registerId = SOFT_EV_START_ID;
      registerValue = 1;
      break;
    case GFAS_EVENT_STOP:
      registerId = SOFT_EV_START_ID;
      registerValue = 0;
      break;
    default:
      return -GFAS_ERR;
  }
  rc = getRegisterSize(handle, registerId, &registerDepth, &registerElementSize);
  if(rc <= 0) {
    return rc;
  }
  registerSize = rc;
  rc = DaSetRegister(handle, registerId, &registerValue, registerSize);
  if (rc <= 0) {
    return -GFAS_DAL_ERR;
  }
  if (rc != registerDepth) {
    return -GFAS_ERR;
  }

  return 0;
}


int
gfasReset(GFASHandle handle, unsigned short channel)
{
  int rc = 0;
  rc = gfasSendSoftwarePulse(handle, channel, GFAS_STOP);
  if (rc < 0) {
    return rc;
  }
  rc = gfasDisableAllFunctions(handle, channel);
  if (rc < 0) {
    return rc;
  }

  return 0;
}


char*
gfasStrError(int errNo)
{
  switch (errNo) {
    case -GFAS_DAL_ERR:
      return "DAL error";
    case -GFAS_ERR:
      return "Internal library error";
    case -GFAS_OUT_OF_RANGE:
      return "Value out-of-range";
    default:
      return "Unknown error";
  }
}
