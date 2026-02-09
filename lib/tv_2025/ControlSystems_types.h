/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ControlSystems_ONLYFRONT_types.h
 *
 * Code generated for Simulink model 'ControlSystems_ONLYFRONT'.
 *
 * Model version                  : 3.1213
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sun Sep 14 13:11:02 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef ControlSystems_ONLYFRONT_types_h_
#define ControlSystems_ONLYFRONT_types_h_
#include <stdint.h>
#include <stdbool.h>
#ifndef struct_tag_iK5Gh2hERNaoMM6WGurZaB
#define struct_tag_iK5Gh2hERNaoMM6WGurZaB

struct tag_iK5Gh2hERNaoMM6WGurZaB
{
  uint32_t f1[8];
};

#endif                                 /* struct_tag_iK5Gh2hERNaoMM6WGurZaB */

#ifndef typedef_cell_wrap_ControlSystems_ONLYFRONT_T
#define typedef_cell_wrap_ControlSystems_ONLYFRONT_T

typedef struct tag_iK5Gh2hERNaoMM6WGurZaB cell_wrap_ControlSystems_ONLYFRONT_T;

#endif                        /* typedef_cell_wrap_ControlSystems_ONLYFRONT_T */

#ifndef struct_tag_SvZkGj2RxPUWmxQFvuw7rC
#define struct_tag_SvZkGj2RxPUWmxQFvuw7rC

struct tag_SvZkGj2RxPUWmxQFvuw7rC
{
  bool matlabCodegenIsDeleted;
  int32_t isInitialized;
  bool isSetupComplete;
  bool TunablePropsChanged;
  cell_wrap_ControlSystems_ONLYFRONT_T inputVarSize;
  double ForgettingFactor;
  int32_t NumChannels;
  int32_t FrameLength;
  double pwN;
  double pmN;
  double plambda;
};

#endif                                 /* struct_tag_SvZkGj2RxPUWmxQFvuw7rC */

#ifndef typedef_dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T
#define typedef_dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T

typedef struct tag_SvZkGj2RxPUWmxQFvuw7rC
  dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T;

#endif       /* typedef_dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T */

/* Parameters (default storage) */
typedef struct P_ControlSystems_ONLYFRONT_T_ P_ControlSystems_ONLYFRONT_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_ControlSystems_ONLYFRONT_T
  RT_MODEL_ControlSystems_ONLYFRONT_T;

#endif                                 /* ControlSystems_ONLYFRONT_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
