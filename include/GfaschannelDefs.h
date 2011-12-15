#ifndef _GFASCHANNEL_DEFS_H_INCLUDE_
#define _GFASCHANNEL_DEFS_H_INCLUDE_

#include <dg/dal.h>

#include <stdint.h>

extern const unsigned short gfasLowChannel; // number from which libgfas counts channels
extern const unsigned short gfasLowSlot; // number from which libgfas counts function slots

/* typedef int Handle; */
typedef HANDLE GFASHandle;

typedef enum {
  GFAS_NORMAL,
  GFAS_DAC,
  GFAS_LOW_JITTER,
  GFAS_NORMAL_RECURRENT,
  GFAS_LOW_JITTER_RECURRENT,
  GFAS_TEST1,
  GFAS_TEST2,
  GFAS_NO_FUNCTION_GENERATION
} GFASMode;

typedef struct {
  GFASMode mode;
} GFASConfig;

typedef enum {
  GFAS_START,
  GFAS_STOP,
  GFAS_EVENT_START,
  GFAS_EVENT_STOP
} GFASInputPulse;

typedef struct {
  uint16_t nSteps;
  int16_t stepSizeMultiple;
  uint16_t voltage;
} GFASVector;

extern const unsigned long gfasVectorLength;

typedef enum {
  GFAS_N_STEPS,
  GFAS_STEP_SIZE_MULTIPLE,
  GFAS_VOLTAGE
} GFASVectorOffset;

#define GFAS_MAX_N_VECTORS 510

typedef struct {
  uint16_t nVectors;
  uint16_t nVectorsCounted;
  uint16_t originVoltage;
  GFASVector vectors[GFAS_MAX_N_VECTORS+1]; /* + tail 0-vector */
} GFASFunction;

enum gfasError {
  GFAS_OK,
  GFAS_ERR,
  GFAS_BAD_HANDLE,
  GFAS_DAL_ERR,
  GFAS_OUT_OF_RANGE,
  GFAS_IOCTL_FAILURE
};

#endif /* _GFASCHANNEL_DEFS_H_INCLUDE_ */
