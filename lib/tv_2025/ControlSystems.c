/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ControlSystems_ONLYFRONT.c
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

#include "ControlSystems.h"
#include "ControlSystems_types.h"
#include "ControlSystems_private.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <stdint.h>
#include <stdbool.h>
#include "rt_defines.h"
#define ControlSystems_ONLYFRONT_period (0.5)

/* Block signals (default storage) */
B_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_B;

/* Block states (default storage) */
DW_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_DW;

/* External inputs (root inport signals with default storage) */
ExtU_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_Y;

/* Real-time model */
static RT_MODEL_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_M_;
RT_MODEL_ControlSystems_ONLYFRONT_T *const ControlSystems_ONLYFRONT_M =
  &ControlSystems_ONLYFRONT_M_;

/* Forward declaration for local functions */
static void ControlSystems_ONLYFRONT_SystemCore_setup
  (dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T *obj);
static void rate_scheduler(void);
double look1_binlxpw(double u0, const double bp0[], const double table[],
                     uint32_t maxIndex)
{
  double frac;
  double yL_0d0;
  uint32_t iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_t bpIdx;
    uint32_t iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (ControlSystems_ONLYFRONT_M->Timing.TaskCounters.TID[1])++;
  if ((ControlSystems_ONLYFRONT_M->Timing.TaskCounters.TID[1]) > 499) {/* Sample time: [0.5s, 0.0s] */
    ControlSystems_ONLYFRONT_M->Timing.TaskCounters.TID[1] = 0;
  }
}

/*
 * Output and update for atomic system:
 *    '<Root>/Discrete lowpass filter'
 *    '<Root>/Discrete lowpass filter1'
 */
void ControlSystems_ONLYFRONT_Discretelowpassfilter(double rtu_x, double
  rtu_x_prec, double rtu_omega_0_inv, double rtu_y_prec, double *rty_y)
{
  double B;
  double den_tmp;
  den_tmp = 2.0 * rtu_omega_0_inv / 0.001;
  B = 1.0 / (den_tmp + 1.0);
  *rty_y = (-((1.0 - den_tmp) / (den_tmp + 1.0)) * rtu_y_prec + B * rtu_x) + B *
    rtu_x_prec;
}

double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    double tmp;
    double tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = sqrt(a * a + 1.0) * b;
  } else if (a > b) {
    b /= a;
    y = sqrt(b * b + 1.0) * a;
  } else if (rtIsNaN(b)) {
    y = (rtNaN);
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_t tmp;
    int32_t tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp, tmp_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static void ControlSystems_ONLYFRONT_SystemCore_setup
  (dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T *obj)
{
  obj->isInitialized = 1;

  /* Start for MATLABSystem: '<Root>/Moving Average' */
  obj->NumChannels = 1;
  obj->FrameLength = 1;
  obj->pwN = 0.0;
  obj->pmN = 0.0;
  obj->plambda = obj->ForgettingFactor;
  obj->isSetupComplete = true;
  obj->TunablePropsChanged = false;
}

/* Model step function */
void ControlSystems_ONLYFRONT_step(void)
{
  double tmpForInput[4];
  double x[3];
  double OCV_filt;
  double P_idx_1;
  double T_max_idx_0;
  double T_max_idx_1;
  double T_max_idx_2;
  double T_max_idx_3;
  double deltaK;
  double frontMotorTorque;
  double ripartizione_idx_0;
  double ripartizione_idx_2;
  double rtb_DeadZone_b;
  double rtb_DeadZone_c;
  double rtb_DeadZone_do;
  double rtb_DeadZone_f;
  double rtb_DeadZone_i;
  double rtb_DeadZone_p;
  double rtb_Gain1;
  double rtb_Gain19;
  double rtb_Gain_g_idx_1;
  double rtb_Gain_g_idx_2;
  double rtb_Integrator_gf;
  double rtb_Integrator_jj;
  double rtb_Min2;
  double rtb_Min_g_idx_0;
  double rtb_Min_g_idx_1;
  double rtb_Min_g_idx_2;
  double rtb_RR;
  double rtb_Regen_active;
  double rtb_Saturation22;
  double rtb_Saturation28;
  double rtb_Sliperror_idx_0;
  double rtb_Sliperror_idx_1;
  double rtb_YMCTorque_idx_1;
  double rtb_YMCTorque_idx_3;
  double rtb_YMC_lim;
  double rtb_acc_x;
  double rtb_acc_y;
  double rtb_battery_voltage;
  double rtb_estimatedspeedmotors;
  double rtb_yaw_rate;
  double s_im;
  double t_p_lim;
  double u0;
  float Imax;
  float b;
  float rade;
  float rads;
  float rpm;
  int32_t idx;
  int32_t k;
  int8_t tmp;
  int8_t tmp_0;
  bool exitg1;
  bool rtb_RelationalOperator1_0;
  bool rtb_RelationalOperator1_idx_0;
  bool rtb_RelationalOperator1_idx_1;
  bool rtb_RelationalOperator1_idx_2;

  /* Saturate: '<Root>/Saturation28' */
  if (ControlSystems_ONLYFRONT_P.MAX_RPM < 20000.0) {
    rtb_Saturation28 = ControlSystems_ONLYFRONT_P.MAX_RPM;
  } else {
    rtb_Saturation28 = 20000.0;
  }

  /* End of Saturate: '<Root>/Saturation28' */

  /* Saturate: '<Root>/Saturation12' incorporates:
   *  Inport: '<Root>/battery_voltage'
   */
  if (ControlSystems_ONLYFRONT_U.battery_voltage > 600.0) {
    rtb_battery_voltage = 600.0;
  } else if (ControlSystems_ONLYFRONT_U.battery_voltage < 300.0) {
    rtb_battery_voltage = 300.0;
  } else {
    rtb_battery_voltage = ControlSystems_ONLYFRONT_U.battery_voltage;
  }

  /* End of Saturate: '<Root>/Saturation12' */

  /* MATLAB Function: '<S21>/MATLAB Function1' */
  ControlSystems_ONLYFRONT_Y.Power_max_motor = (sqrt(1.2350321424000001 -
    (357.21000000000009 - rtb_battery_voltage * rtb_battery_voltage / 3.0) *
    0.00797328) - 1.11132) * 0.9 / 0.00398664 / 5.0 * 21.0;

  /* Saturate: '<Root>/Saturation10' incorporates:
   *  Inport: '<Root>/acc_y'
   */
  if (ControlSystems_ONLYFRONT_U.acc_y > 24.525000000000002) {
    rtb_acc_y = 24.525000000000002;
  } else if (ControlSystems_ONLYFRONT_U.acc_y < -24.525000000000002) {
    rtb_acc_y = -24.525000000000002;
  } else {
    rtb_acc_y = ControlSystems_ONLYFRONT_U.acc_y;
  }

  /* End of Saturate: '<Root>/Saturation10' */

  /* MATLAB Function: '<Root>/Discrete lowpass filter1' incorporates:
   *  Constant: '<Root>/Constant17'
   *  Constant: '<Root>/Constant2'
   *  Delay: '<Root>/Delay4'
   *  Delay: '<Root>/Delay5'
   *  Gain: '<Root>/Gain8'
   *  Product: '<Root>/Divide2'
   */
  ControlSystems_ONLYFRONT_Discretelowpassfilter(rtb_acc_y,
    ControlSystems_ONLYFRONT_DW.Delay4_DSTATE, 1.0 / (6.2831853071795862 *
    ControlSystems_ONLYFRONT_P.IN_ACC_Y_LP_HZ),
    ControlSystems_ONLYFRONT_DW.Delay5_DSTATE,
    &ControlSystems_ONLYFRONT_DW.Delay5_DSTATE);

  /* Gain: '<S16>/Gain3' incorporates:
   *  Delay: '<Root>/Delay5'
   */
  rtb_DeadZone_do = -426.56002682926851 *
    ControlSystems_ONLYFRONT_DW.Delay5_DSTATE;

  /* Saturate: '<Root>/Saturation9' incorporates:
   *  Inport: '<Root>/acc_x'
   */
  if (ControlSystems_ONLYFRONT_U.acc_x > 19.62) {
    rtb_acc_x = 19.62;
  } else if (ControlSystems_ONLYFRONT_U.acc_x < -19.62) {
    rtb_acc_x = -19.62;
  } else {
    rtb_acc_x = ControlSystems_ONLYFRONT_U.acc_x;
  }

  /* End of Saturate: '<Root>/Saturation9' */

  /* MATLAB Function: '<Root>/Discrete lowpass filter' incorporates:
   *  Constant: '<Root>/Constant12'
   *  Constant: '<Root>/Constant16'
   *  Delay: '<Root>/Delay'
   *  Delay: '<Root>/Delay3'
   *  Gain: '<Root>/Gain7'
   *  Product: '<Root>/Divide1'
   */
  ControlSystems_ONLYFRONT_Discretelowpassfilter(rtb_acc_x,
    ControlSystems_ONLYFRONT_DW.Delay_DSTATE, 1.0 / (6.2831853071795862 *
    ControlSystems_ONLYFRONT_P.IN_ACC_X_LP_HZ),
    ControlSystems_ONLYFRONT_DW.Delay3_DSTATE,
    &ControlSystems_ONLYFRONT_DW.Delay3_DSTATE);

  /* Gain: '<S16>/Gain2' incorporates:
   *  Delay: '<Root>/Delay3'
   */
  rtb_DeadZone_f = 23.4 * ControlSystems_ONLYFRONT_DW.Delay3_DSTATE;

  /* Saturate: '<Root>/Saturation4' incorporates:
   *  Inport: '<Root>/rpm_fl'
   */
  if (ControlSystems_ONLYFRONT_U.rpm_fl > 20000.0) {
    /* Saturate: '<Root>/Saturation4' */
    ControlSystems_ONLYFRONT_B.rpm_fl = 20000.0;
  } else if (ControlSystems_ONLYFRONT_U.rpm_fl < 0.0) {
    /* Saturate: '<Root>/Saturation4' */
    ControlSystems_ONLYFRONT_B.rpm_fl = 0.0;
  } else {
    /* Saturate: '<Root>/Saturation4' */
    ControlSystems_ONLYFRONT_B.rpm_fl = ControlSystems_ONLYFRONT_U.rpm_fl;
  }

  /* End of Saturate: '<Root>/Saturation4' */

  /* Saturate: '<Root>/Saturation5' incorporates:
   *  Inport: '<Root>/rpm_fr'
   */
  if (ControlSystems_ONLYFRONT_U.rpm_fr > 20000.0) {
    /* Saturate: '<Root>/Saturation5' */
    ControlSystems_ONLYFRONT_B.rpm_fr = 20000.0;
  } else if (ControlSystems_ONLYFRONT_U.rpm_fr < 0.0) {
    /* Saturate: '<Root>/Saturation5' */
    ControlSystems_ONLYFRONT_B.rpm_fr = 0.0;
  } else {
    /* Saturate: '<Root>/Saturation5' */
    ControlSystems_ONLYFRONT_B.rpm_fr = ControlSystems_ONLYFRONT_U.rpm_fr;
  }

  /* End of Saturate: '<Root>/Saturation5' */

  /* Saturate: '<Root>/Saturation6' incorporates:
   *  Inport: '<Root>/rpm_rl'
   */
  if (ControlSystems_ONLYFRONT_U.rpm_rl > 20000.0) {
    /* Saturate: '<Root>/Saturation6' */
    ControlSystems_ONLYFRONT_B.rpm_rl = 20000.0;
  } else if (ControlSystems_ONLYFRONT_U.rpm_rl < 0.0) {
    /* Saturate: '<Root>/Saturation6' */
    ControlSystems_ONLYFRONT_B.rpm_rl = 0.0;
  } else {
    /* Saturate: '<Root>/Saturation6' */
    ControlSystems_ONLYFRONT_B.rpm_rl = ControlSystems_ONLYFRONT_U.rpm_rl;
  }

  /* End of Saturate: '<Root>/Saturation6' */

  /* Saturate: '<Root>/Saturation7' incorporates:
   *  Inport: '<Root>/rpm_rr'
   */
  if (ControlSystems_ONLYFRONT_U.rpm_rr > 20000.0) {
    /* Saturate: '<Root>/Saturation7' */
    ControlSystems_ONLYFRONT_B.rpm_rr = 20000.0;
  } else if (ControlSystems_ONLYFRONT_U.rpm_rr < 0.0) {
    /* Saturate: '<Root>/Saturation7' */
    ControlSystems_ONLYFRONT_B.rpm_rr = 0.0;
  } else {
    /* Saturate: '<Root>/Saturation7' */
    ControlSystems_ONLYFRONT_B.rpm_rr = ControlSystems_ONLYFRONT_U.rpm_rr;
  }

  /* End of Saturate: '<Root>/Saturation7' */

  /* Gain: '<Root>/Gain' incorporates:
   *  Sum: '<Root>/Sum of Elements'
   */
  rtb_estimatedspeedmotors = (((ControlSystems_ONLYFRONT_B.rpm_fl +
    ControlSystems_ONLYFRONT_B.rpm_fr) + ControlSystems_ONLYFRONT_B.rpm_rl) +
    ControlSystems_ONLYFRONT_B.rpm_rr) * 0.00041997028184090281;

  /* Math: '<S16>/Square' incorporates:
   *  Math: '<S532>/Square'
   */
  rtb_yaw_rate = rtb_estimatedspeedmotors * rtb_estimatedspeedmotors;

  /* Gain: '<S16>/Fl' incorporates:
   *  Math: '<S16>/Square'
   */
  rtb_Integrator_jj = rtb_yaw_rate * 0.60205;

  /* Gain: '<S16>/Gain5' */
  rtb_Integrator_gf = 5.29 * rtb_Integrator_jj;

  /* Gain: '<S16>/Gain1' */
  rtb_DeadZone_p = 0.22250814332247557 * rtb_Integrator_gf;

  /* Gain: '<S16>/Gain8' incorporates:
   *  Gain: '<S16>/Gain7'
   */
  rtb_Integrator_jj = 1.6 * rtb_Integrator_jj * 0.057328990228013028;

  /* Sum: '<S16>/Add' incorporates:
   *  Constant: '<S16>/Constant'
   */
  rtb_Regen_active = (((600.372 - rtb_DeadZone_do) - rtb_DeadZone_f) +
                      rtb_DeadZone_p) - rtb_Integrator_jj;

  /* Sum: '<S16>/Add1' incorporates:
   *  Constant: '<S16>/Constant'
   */
  rtb_DeadZone_do = (((rtb_DeadZone_do + 600.372) - rtb_DeadZone_f) +
                     rtb_DeadZone_p) - rtb_Integrator_jj;

  /* Gain: '<S16>/Gain4' incorporates:
   *  Delay: '<Root>/Delay5'
   */
  rtb_DeadZone_p = 497.0890275 * ControlSystems_ONLYFRONT_DW.Delay5_DSTATE;

  /* Gain: '<S16>/Gain' */
  rtb_Integrator_gf *= 0.27749185667752446;

  /* Sum: '<S16>/Add2' incorporates:
   *  Constant: '<S16>/Constant1'
   */
  rtb_YMC_lim = (((733.78800000000012 - rtb_DeadZone_p) + rtb_DeadZone_f) +
                 rtb_Integrator_gf) + rtb_Integrator_jj;

  /* Saturate: '<S16>/Saturation' */
  if (rtb_Regen_active > 2500.0) {
    ControlSystems_ONLYFRONT_Y.Fz[0] = 2500.0;
  } else if (rtb_Regen_active < 50.0) {
    ControlSystems_ONLYFRONT_Y.Fz[0] = 50.0;
  } else {
    ControlSystems_ONLYFRONT_Y.Fz[0] = rtb_Regen_active;
  }

  if (rtb_DeadZone_do > 2500.0) {
    ControlSystems_ONLYFRONT_Y.Fz[1] = 2500.0;
  } else if (rtb_DeadZone_do < 50.0) {
    ControlSystems_ONLYFRONT_Y.Fz[1] = 50.0;
  } else {
    ControlSystems_ONLYFRONT_Y.Fz[1] = rtb_DeadZone_do;
  }

  if (rtb_YMC_lim > 2500.0) {
    ControlSystems_ONLYFRONT_Y.Fz[2] = 2500.0;
  } else if (rtb_YMC_lim < 50.0) {
    ControlSystems_ONLYFRONT_Y.Fz[2] = 50.0;
  } else {
    ControlSystems_ONLYFRONT_Y.Fz[2] = rtb_YMC_lim;
  }

  /* Sum: '<S16>/Add3' incorporates:
   *  Constant: '<S16>/Constant1'
   */
  u0 = (((rtb_DeadZone_p + 733.78800000000012) + rtb_DeadZone_f) +
        rtb_Integrator_gf) + rtb_Integrator_jj;

  /* Saturate: '<S16>/Saturation' */
  if (u0 > 2500.0) {
    ControlSystems_ONLYFRONT_Y.Fz[3] = 2500.0;
  } else if (u0 < 50.0) {
    ControlSystems_ONLYFRONT_Y.Fz[3] = 50.0;
  } else {
    ControlSystems_ONLYFRONT_Y.Fz[3] = u0;
  }

  /* Product: '<S1>/Product1' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Constant1'
   *  Constant: '<S1>/Constant2'
   *  Product: '<S1>/Divide'
   *  Product: '<S1>/Product'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Subtract'
   */
  rtb_Min_g_idx_2 = ((ControlSystems_ONLYFRONT_Y.Fz[2] - 667.461609) /
                     667.461609 * -0.32974244 + 1.579596792) *
    ControlSystems_ONLYFRONT_Y.Fz[2];
  rtb_Integrator_jj = ((ControlSystems_ONLYFRONT_Y.Fz[3] - 667.461609) /
                       667.461609 * -0.32974244 + 1.579596792) *
    ControlSystems_ONLYFRONT_Y.Fz[3];

  /* MATLABSystem: '<Root>/Moving Average' */
  if (ControlSystems_ONLYFRONT_DW.obj.ForgettingFactor != 0.98) {
    if (ControlSystems_ONLYFRONT_DW.obj.isInitialized == 1) {
      ControlSystems_ONLYFRONT_DW.obj.TunablePropsChanged = true;
    }

    ControlSystems_ONLYFRONT_DW.obj.ForgettingFactor = 0.98;
  }

  if (ControlSystems_ONLYFRONT_DW.obj.TunablePropsChanged) {
    ControlSystems_ONLYFRONT_DW.obj.TunablePropsChanged = false;
    ControlSystems_ONLYFRONT_DW.obj.plambda = 0.98;
  }

  rtb_YMC_lim = ControlSystems_ONLYFRONT_DW.obj.plambda *
    ControlSystems_ONLYFRONT_DW.obj.pwN + 1.0;

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Constant1'
   *  Constant: '<S1>/Constant2'
   *  MATLABSystem: '<Root>/Moving Average'
   *  Product: '<S1>/Divide'
   *  Product: '<S1>/Divide1'
   *  Product: '<S1>/Product'
   *  Product: '<S1>/Product1'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Subtract'
   *  Sum: '<S1>/Sum of Elements'
   *  Sum: '<S1>/Sum of Elements1'
   * */
  ControlSystems_ONLYFRONT_Y.csi_opt = (rtb_Min_g_idx_2 + rtb_Integrator_jj) /
    (((((ControlSystems_ONLYFRONT_Y.Fz[0] - 667.461609) / 667.461609 *
        -0.32974244 + 1.579596792) * ControlSystems_ONLYFRONT_Y.Fz[0] +
       ((ControlSystems_ONLYFRONT_Y.Fz[1] - 667.461609) / 667.461609 *
        -0.32974244 + 1.579596792) * ControlSystems_ONLYFRONT_Y.Fz[1]) +
      rtb_Min_g_idx_2) + rtb_Integrator_jj) * (1.0 / rtb_YMC_lim) + (1.0 - 1.0 /
    rtb_YMC_lim) * ControlSystems_ONLYFRONT_DW.obj.pmN;

  /* MATLABSystem: '<Root>/Moving Average' */
  ControlSystems_ONLYFRONT_DW.obj.pwN = rtb_YMC_lim;
  ControlSystems_ONLYFRONT_DW.obj.pmN = ControlSystems_ONLYFRONT_Y.csi_opt;

  /* Saturate: '<Root>/Saturation14' incorporates:
   *  Inport: '<Root>/TV_active'
   */
  if (ControlSystems_ONLYFRONT_U.TV_active > 1.0) {
    rtb_YMC_lim = 1.0;
  } else if (ControlSystems_ONLYFRONT_U.TV_active < 0.0) {
    rtb_YMC_lim = 0.0;
  } else {
    rtb_YMC_lim = ControlSystems_ONLYFRONT_U.TV_active;
  }

  /* End of Saturate: '<Root>/Saturation14' */

  /* Saturate: '<Root>/Saturation19' incorporates:
   *  Inport: '<Root>/brake'
   */
  if (ControlSystems_ONLYFRONT_U.brake > 100.0) {
    rtb_YMCTorque_idx_1 = 100.0;
  } else if (ControlSystems_ONLYFRONT_U.brake < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_U.brake;
  }

  /* Math: '<Root>/Power1' incorporates:
   *  Constant: '<Root>/Constant22'
   *  Gain: '<Root>/Gain4'
   *  Saturate: '<Root>/Saturation19'
   */
  ControlSystems_ONLYFRONT_B.regen_input = rt_powd_snf(0.01 *
    rtb_YMCTorque_idx_1, ControlSystems_ONLYFRONT_P.IN_BRAKE_MAP_EXP);

  /* Saturate: '<Root>/Saturation21' */
  if (ControlSystems_ONLYFRONT_B.regen_input > 1.0) {
    /* Math: '<Root>/Power1' incorporates:
     *  Saturate: '<Root>/Saturation21'
     */
    ControlSystems_ONLYFRONT_B.regen_input = 1.0;
  } else if (ControlSystems_ONLYFRONT_B.regen_input < 0.0) {
    /* Math: '<Root>/Power1' incorporates:
     *  Saturate: '<Root>/Saturation21'
     */
    ControlSystems_ONLYFRONT_B.regen_input = 0.0;
  }

  /* End of Saturate: '<Root>/Saturation21' */

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<Root>/Constant15'
   *  Product: '<Root>/Product4'
   */
  if (!(ControlSystems_ONLYFRONT_P.variable_csi * rtb_YMC_lim != 0.0)) {
    /* Switch: '<Root>/Switch1' incorporates:
     *  Saturate: '<Root>/Saturation3'
     */
    if (ControlSystems_ONLYFRONT_B.regen_input > 0.0) {
      /* Switch: '<Root>/Switch' incorporates:
       *  Constant: '<Root>/Constant14'
       *  Constant: '<Root>/Constant8'
       *  Sum: '<Root>/Subtract2'
       */
      ControlSystems_ONLYFRONT_Y.csi_opt = 1.0 - ControlSystems_ONLYFRONT_P.csi;
    } else {
      /* Switch: '<Root>/Switch' incorporates:
       *  Constant: '<Root>/Constant14'
       */
      ControlSystems_ONLYFRONT_Y.csi_opt = ControlSystems_ONLYFRONT_P.csi;
    }

    /* End of Switch: '<Root>/Switch1' */
  }

  /* End of Switch: '<Root>/Switch' */

  /* Saturate: '<S6>/Saturation' */
  if (ControlSystems_ONLYFRONT_B.rpm_fl > 20000.0) {
    rtb_Min_g_idx_0 = 20000.0;
  } else if (ControlSystems_ONLYFRONT_B.rpm_fl < 0.0) {
    rtb_Min_g_idx_0 = 0.0;
  } else {
    rtb_Min_g_idx_0 = ControlSystems_ONLYFRONT_B.rpm_fl;
  }

  if (ControlSystems_ONLYFRONT_B.rpm_fr > 20000.0) {
    rtb_Min_g_idx_1 = 20000.0;
  } else if (ControlSystems_ONLYFRONT_B.rpm_fr < 0.0) {
    rtb_Min_g_idx_1 = 0.0;
  } else {
    rtb_Min_g_idx_1 = ControlSystems_ONLYFRONT_B.rpm_fr;
  }

  if (ControlSystems_ONLYFRONT_B.rpm_rl > 20000.0) {
    rtb_Min_g_idx_2 = 20000.0;
  } else if (ControlSystems_ONLYFRONT_B.rpm_rl < 0.0) {
    rtb_Min_g_idx_2 = 0.0;
  } else {
    rtb_Min_g_idx_2 = ControlSystems_ONLYFRONT_B.rpm_rl;
  }

  if (ControlSystems_ONLYFRONT_B.rpm_rr > 20000.0) {
    rtb_Integrator_jj = 20000.0;
  } else if (ControlSystems_ONLYFRONT_B.rpm_rr < 0.0) {
    rtb_Integrator_jj = 0.0;
  } else {
    rtb_Integrator_jj = ControlSystems_ONLYFRONT_B.rpm_rr;
  }

  /* End of Saturate: '<S6>/Saturation' */

  /* Gain: '<S21>/Gain1' incorporates:
   *  Sum: '<S21>/Sum of Elements'
   */
  rtb_Gain1 = (((rtb_Min_g_idx_0 + rtb_Min_g_idx_1) + rtb_Min_g_idx_2) +
               rtb_Integrator_jj) * 0.25;

  /* MATLAB Function: '<S21>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant24'
   *  Constant: '<S6>/Constant3'
   */
  T_max_idx_0 = (1.0 - ControlSystems_ONLYFRONT_Y.csi_opt) *
    ControlSystems_ONLYFRONT_P.Tmax / ControlSystems_ONLYFRONT_Y.csi_opt;
  t_p_lim = fmin(ControlSystems_ONLYFRONT_Y.Power_max_motor, 35000.0) * 30.0 /
    (3.1415926535897931 * rtb_Gain1);
  x[0] = ControlSystems_ONLYFRONT_P.Tmax;
  x[1] = t_p_lim;
  x[2] = ControlSystems_ONLYFRONT_P.Pmax * ControlSystems_ONLYFRONT_Y.csi_opt /
    2.0 * 30.0 / (3.1415926535897931 * rtb_Gain1);
  if (!rtIsNaN(ControlSystems_ONLYFRONT_P.Tmax)) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 4)) {
      if (!rtIsNaN(x[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    rtb_Regen_active = ControlSystems_ONLYFRONT_P.Tmax;
  } else {
    rtb_Regen_active = x[idx - 1];
    for (k = idx + 1; k < 4; k++) {
      rtb_DeadZone_c = x[k - 1];
      if (rtb_Regen_active > rtb_DeadZone_c) {
        rtb_Regen_active = rtb_DeadZone_c;
      }
    }
  }

  tmpForInput[0] = T_max_idx_0;
  tmpForInput[1] = (1.0 - ControlSystems_ONLYFRONT_Y.csi_opt) /
    ControlSystems_ONLYFRONT_Y.csi_opt * rtb_Regen_active;
  tmpForInput[2] = t_p_lim;
  tmpForInput[3] = (1.0 - ControlSystems_ONLYFRONT_Y.csi_opt) *
    ControlSystems_ONLYFRONT_P.Pmax / 2.0 * 30.0 / (3.1415926535897931 *
    rtb_Gain1);
  if (!rtIsNaN(T_max_idx_0)) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 5)) {
      if (!rtIsNaN(tmpForInput[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx != 0) {
    T_max_idx_0 = tmpForInput[idx - 1];
    for (k = idx + 1; k < 5; k++) {
      ripartizione_idx_0 = tmpForInput[k - 1];
      if (T_max_idx_0 > ripartizione_idx_0) {
        T_max_idx_0 = ripartizione_idx_0;
      }
    }
  }

  /* Saturate: '<S21>/Saturation' */
  if (rtb_Min_g_idx_0 <= 0.1) {
    rtb_YMCTorque_idx_1 = 0.1;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Min_g_idx_0;
  }

  /* Product: '<S21>/Divide' incorporates:
   *  Gain: '<S21>/Gain'
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain19 = ControlSystems_ONLYFRONT_Y.Power_max_motor / (0.10471975511965977
    * rtb_YMCTorque_idx_1);
  ripartizione_idx_0 = rtb_Gain19;

  /* Lookup_n-D: '<S21>/1-D Lookup Table' incorporates:
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain1 = look1_binlxpw(rtb_Min_g_idx_0,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_bp01Data,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_tableData, 21U);
  t_p_lim = rtb_Gain1;

  /* Saturate: '<S6>/Saturation1' */
  if (T_max_idx_0 > ControlSystems_ONLYFRONT_P.Tmax) {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_P.Tmax;
  } else if (T_max_idx_0 < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = T_max_idx_0;
  }

  /* MinMax: '<S6>/Min' incorporates:
   *  MinMax: '<S21>/Min'
   *  MinMax: '<S21>/Min1'
   *  Saturate: '<S6>/Saturation1'
   */
  ControlSystems_ONLYFRONT_Y.limitTorque[0] = fmin(rtb_YMCTorque_idx_1, fmin
    (fmin(rtb_Gain19, T_max_idx_0), rtb_Gain1));

  /* Saturate: '<S21>/Saturation' */
  if (rtb_Min_g_idx_1 <= 0.1) {
    rtb_YMCTorque_idx_1 = 0.1;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Min_g_idx_1;
  }

  /* Product: '<S21>/Divide' incorporates:
   *  Gain: '<S21>/Gain'
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain19 = ControlSystems_ONLYFRONT_Y.Power_max_motor / (0.10471975511965977
    * rtb_YMCTorque_idx_1);
  rtb_DeadZone_f = rtb_Gain19;

  /* Lookup_n-D: '<S21>/1-D Lookup Table' incorporates:
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain1 = look1_binlxpw(rtb_Min_g_idx_1,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_bp01Data,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_tableData, 21U);
  rtb_Gain_g_idx_1 = rtb_Gain1;

  /* Saturate: '<S6>/Saturation1' */
  if (T_max_idx_0 > ControlSystems_ONLYFRONT_P.Tmax) {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_P.Tmax;
  } else if (T_max_idx_0 < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = T_max_idx_0;
  }

  /* MinMax: '<S6>/Min' incorporates:
   *  MinMax: '<S21>/Min'
   *  MinMax: '<S21>/Min1'
   *  Saturate: '<S6>/Saturation1'
   */
  ControlSystems_ONLYFRONT_Y.limitTorque[1] = fmin(rtb_YMCTorque_idx_1, fmin
    (fmin(rtb_Gain19, T_max_idx_0), rtb_Gain1));

  /* Saturate: '<S21>/Saturation' */
  if (rtb_Min_g_idx_2 <= 0.1) {
    rtb_YMCTorque_idx_1 = 0.1;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Min_g_idx_2;
  }

  /* Product: '<S21>/Divide' incorporates:
   *  Gain: '<S21>/Gain'
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain19 = ControlSystems_ONLYFRONT_Y.Power_max_motor / (0.10471975511965977
    * rtb_YMCTorque_idx_1);
  ripartizione_idx_2 = rtb_Gain19;

  /* Lookup_n-D: '<S21>/1-D Lookup Table' incorporates:
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain1 = look1_binlxpw(rtb_Min_g_idx_2,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_bp01Data,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_tableData, 21U);
  rtb_Gain_g_idx_2 = rtb_Gain1;

  /* Saturate: '<S6>/Saturation1' incorporates:
   *  MATLAB Function: '<S21>/MATLAB Function'
   */
  if (rtb_Regen_active > ControlSystems_ONLYFRONT_P.Tmax) {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_P.Tmax;
  } else if (rtb_Regen_active < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Regen_active;
  }

  /* MinMax: '<S6>/Min' incorporates:
   *  MATLAB Function: '<S21>/MATLAB Function'
   *  MinMax: '<S21>/Min'
   *  MinMax: '<S21>/Min1'
   *  Saturate: '<S6>/Saturation1'
   */
  ControlSystems_ONLYFRONT_Y.limitTorque[2] = fmin(rtb_YMCTorque_idx_1, fmin
    (fmin(rtb_Gain19, rtb_Regen_active), rtb_Gain1));

  /* Saturate: '<S21>/Saturation' */
  if (rtb_Integrator_jj <= 0.1) {
    rtb_YMCTorque_idx_1 = 0.1;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Integrator_jj;
  }

  /* Product: '<S21>/Divide' incorporates:
   *  Gain: '<S21>/Gain'
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain19 = ControlSystems_ONLYFRONT_Y.Power_max_motor / (0.10471975511965977
    * rtb_YMCTorque_idx_1);

  /* Lookup_n-D: '<S21>/1-D Lookup Table' incorporates:
   *  Saturate: '<S21>/Saturation'
   */
  rtb_Gain1 = look1_binlxpw(rtb_Integrator_jj,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_bp01Data,
    ControlSystems_ONLYFRONT_ConstP.uDLookupTable_tableData, 21U);

  /* Saturate: '<S6>/Saturation1' incorporates:
   *  MATLAB Function: '<S21>/MATLAB Function'
   */
  if (rtb_Regen_active > ControlSystems_ONLYFRONT_P.Tmax) {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_P.Tmax;
  } else if (rtb_Regen_active < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = rtb_Regen_active;
  }

  /* MinMax: '<S6>/Min' incorporates:
   *  MATLAB Function: '<S21>/MATLAB Function'
   *  MinMax: '<S21>/Min'
   *  MinMax: '<S21>/Min1'
   *  Saturate: '<S6>/Saturation1'
   */
  ControlSystems_ONLYFRONT_Y.limitTorque[3] = fmin(rtb_YMCTorque_idx_1, fmin
    (fmin(rtb_Gain19, rtb_Regen_active), rtb_Gain1));

  /* Saturate: '<Root>/Saturation18' incorporates:
   *  Inport: '<Root>/throttle'
   */
  if (ControlSystems_ONLYFRONT_U.throttle > 100.0) {
    rtb_YMCTorque_idx_1 = 100.0;
  } else if (ControlSystems_ONLYFRONT_U.throttle < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_U.throttle;
  }

  /* Math: '<Root>/Power' incorporates:
   *  Constant: '<Root>/Constant21'
   *  Gain: '<Root>/Gain3'
   *  Saturate: '<Root>/Saturation18'
   */
  ControlSystems_ONLYFRONT_B.throttle = rt_powd_snf(0.01 * rtb_YMCTorque_idx_1,
    ControlSystems_ONLYFRONT_P.IN_THROTTLE_MAP_EXP);

  /* Saturate: '<Root>/Saturation20' */
  if (ControlSystems_ONLYFRONT_B.throttle > 1.0) {
    /* Math: '<Root>/Power' incorporates:
     *  Saturate: '<Root>/Saturation20'
     */
    ControlSystems_ONLYFRONT_B.throttle = 1.0;
  } else if (ControlSystems_ONLYFRONT_B.throttle < 0.0) {
    /* Math: '<Root>/Power' incorporates:
     *  Saturate: '<Root>/Saturation20'
     */
    ControlSystems_ONLYFRONT_B.throttle = 0.0;
  }

  /* End of Saturate: '<Root>/Saturation20' */

  /* Saturate: '<Root>/Saturation15' incorporates:
   *  Inport: '<Root>/Regen_active'
   */
  if (ControlSystems_ONLYFRONT_U.Regen_active > 1.0) {
    rtb_Regen_active = 1.0;
  } else if (ControlSystems_ONLYFRONT_U.Regen_active < 0.0) {
    rtb_Regen_active = 0.0;
  } else {
    rtb_Regen_active = ControlSystems_ONLYFRONT_U.Regen_active;
  }

  /* End of Saturate: '<Root>/Saturation15' */

  /* Switch: '<S20>/Switch' */
  if (rtb_Regen_active > 0.0) {
    /* Switch: '<S20>/Switch2' incorporates:
     *  Constant: '<S20>/Constant5'
     *  Saturate: '<Root>/Saturation3'
     */
    if (ControlSystems_ONLYFRONT_B.regen_input > 0.2) {
      rtb_DeadZone_p = 0.0;
    } else {
      /* Saturate: '<Root>/Saturation1' */
      rtb_DeadZone_p = ControlSystems_ONLYFRONT_B.throttle;
    }

    /* End of Switch: '<S20>/Switch2' */
  } else {
    /* Saturate: '<Root>/Saturation1' */
    rtb_DeadZone_p = ControlSystems_ONLYFRONT_B.throttle;
  }

  /* End of Switch: '<S20>/Switch' */

  /* Product: '<S6>/Product' */
  tmpForInput[0] = ControlSystems_ONLYFRONT_Y.limitTorque[0] * rtb_DeadZone_p;
  tmpForInput[1] = ControlSystems_ONLYFRONT_Y.limitTorque[1] * rtb_DeadZone_p;
  tmpForInput[2] = ControlSystems_ONLYFRONT_Y.limitTorque[2] * rtb_DeadZone_p;
  tmpForInput[3] = ControlSystems_ONLYFRONT_Y.limitTorque[3] * rtb_DeadZone_p;

  /* Saturate: '<S21>/Saturation1' */
  if (ripartizione_idx_0 > 21.0) {
    ripartizione_idx_0 = 21.0;
  } else if (ripartizione_idx_0 < 0.0) {
    ripartizione_idx_0 = 0.0;
  }

  if (rtb_DeadZone_f > 21.0) {
    rtb_DeadZone_f = 21.0;
  } else if (rtb_DeadZone_f < 0.0) {
    rtb_DeadZone_f = 0.0;
  }

  if (ripartizione_idx_2 > 21.0) {
    ripartizione_idx_2 = 21.0;
  } else if (ripartizione_idx_2 < 0.0) {
    ripartizione_idx_2 = 0.0;
  }

  if (rtb_Gain19 > 21.0) {
    rtb_Gain19 = 21.0;
  } else if (rtb_Gain19 < 0.0) {
    rtb_Gain19 = 0.0;
  }

  /* MinMax: '<S6>/Min2' incorporates:
   *  Constant: '<Root>/Constant4'
   *  Lookup_n-D: '<S21>/1-D Lookup Table'
   *  MinMax: '<S21>/Min2'
   *  MinMax: '<S6>/Min4'
   *  Saturate: '<S21>/Saturation1'
   */
  rtb_Min2 = fmin(fmin(fmin(fmin(fmin(ripartizione_idx_0, t_p_lim), fmin
    (rtb_DeadZone_f, rtb_Gain_g_idx_1)), fmin(ripartizione_idx_2,
    rtb_Gain_g_idx_2)), fmin(rtb_Gain19, rtb_Gain1)),
                  ControlSystems_ONLYFRONT_P.Tmax_regen);

  /* MinMax: '<S19>/Max' incorporates:
   *  Constant: '<S19>/Constant1'
   *  Constant: '<S19>/Constant2'
   *  Constant: '<S6>/Constant1'
   *  MinMax: '<S19>/Min1'
   *  Sum: '<S6>/Subtract'
   */
  ripartizione_idx_2 = fmax(fmin(1.0 - ControlSystems_ONLYFRONT_Y.csi_opt, 0.8),
    0.5);

  /* MATLAB Function: '<S19>/MATLAB Function1' incorporates:
   *  Constant: '<Root>/Constant'
   */
  rtb_Gain1 = fmin(ControlSystems_ONLYFRONT_P.Imax_regen, (564.94 -
    rtb_battery_voltage) * 3.0211);
  ControlSystems_ONLYFRONT_Y.Pemax = (0.231 * rtb_Gain1 + rtb_battery_voltage) *
    rtb_Gain1 * 1.0204;

  /* MinMax: '<S19>/Max1' incorporates:
   *  Constant: '<S19>/Constant5'
   *  Constant: '<S19>/Constant9'
   *  Gain: '<S6>/Gain4'
   *  MinMax: '<S19>/Min2'
   */
  rtb_DeadZone_f = fmax(fmin(0.10471975511965977 * rtb_Min_g_idx_0,
    2094.3951023931954), 0.001);
  rtb_Min_g_idx_0 = rtb_DeadZone_f;

  /* Abs: '<S19>/Abs' incorporates:
   *  Gain: '<S19>/Gain'
   */
  rtb_Gain1 = 9.5492965855137211 * rtb_DeadZone_f;

  /* MinMax: '<S19>/Max1' incorporates:
   *  Constant: '<S19>/Constant5'
   *  Constant: '<S19>/Constant9'
   *  Gain: '<S6>/Gain4'
   *  MinMax: '<S19>/Min2'
   */
  rtb_DeadZone_f = fmax(fmin(0.10471975511965977 * rtb_Min_g_idx_1,
    2094.3951023931954), 0.001);
  rtb_Min_g_idx_1 = rtb_DeadZone_f;

  /* Abs: '<S19>/Abs' incorporates:
   *  Gain: '<S19>/Gain'
   */
  t_p_lim = 9.5492965855137211 * rtb_DeadZone_f;

  /* MinMax: '<S19>/Max1' incorporates:
   *  Constant: '<S19>/Constant5'
   *  Constant: '<S19>/Constant9'
   *  Gain: '<S6>/Gain4'
   *  MinMax: '<S19>/Min2'
   */
  rtb_DeadZone_f = fmax(fmin(0.10471975511965977 * rtb_Min_g_idx_2,
    2094.3951023931954), 0.001);
  rtb_Min_g_idx_2 = rtb_DeadZone_f;

  /* Abs: '<S19>/Abs' incorporates:
   *  Gain: '<S19>/Gain'
   */
  rtb_Gain_g_idx_1 = 9.5492965855137211 * rtb_DeadZone_f;

  /* MinMax: '<S19>/Max1' incorporates:
   *  Constant: '<S19>/Constant5'
   *  Constant: '<S19>/Constant9'
   *  Gain: '<S6>/Gain4'
   *  MinMax: '<S19>/Min2'
   */
  rtb_DeadZone_f = fmax(fmin(0.10471975511965977 * rtb_Integrator_jj,
    2094.3951023931954), 0.001);

  /* MATLAB Function: '<S19>/MATLAB Function1' incorporates:
   *  Gain: '<S19>/Gain'
   *  MATLAB Function: '<S19>/MATLAB Function'
   */
  ripartizione_idx_0 = ((rtb_Gain1 + t_p_lim) + rtb_Gain_g_idx_1) +
    9.5492965855137211 * rtb_DeadZone_f;
  T_max_idx_0 = ripartizione_idx_0 * 0.25 * 3.1415926535897931 / 30.0;
  rtb_Integrator_jj = ripartizione_idx_2 * 0.5 *
    ControlSystems_ONLYFRONT_Y.Pemax;
  rtb_DeadZone_do = rtb_Integrator_jj / rtb_Min2;
  rtb_DeadZone_p = 1.4142135623730951 * rtb_Min2 / 0.238;
  if (T_max_idx_0 >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = rtb_DeadZone_do / T_max_idx_0 * rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 * rtb_Min2 /
      21.0));
  } else {
    rtb_Gain_g_idx_1 = T_max_idx_0 * 5.0 * 1.5 * 0.0296;
    rtb_Gain_g_idx_1 = (sqrt(rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1 - 1.215 *
      -rtb_Integrator_jj) - rtb_Gain_g_idx_1) / 0.6075;
    rtb_Gain_g_idx_2 = 0.0;
  }

  rtb_Gain1 = (rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 *
               rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj;
  if (T_max_idx_0 >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = rtb_DeadZone_do / T_max_idx_0 * rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 * rtb_Min2 /
      21.0));
  } else {
    rtb_Gain_g_idx_1 = T_max_idx_0 * 5.0 * 1.5 * 0.0296;
    rtb_Gain_g_idx_1 = (sqrt(rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1 - 1.215 *
      -rtb_Integrator_jj) - rtb_Gain_g_idx_1) / 0.6075;
    rtb_Gain_g_idx_2 = 0.0;
  }

  P_idx_1 = (rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 *
             rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj;
  rtb_Integrator_jj = (1.0 - ripartizione_idx_2) * 0.5 *
    ControlSystems_ONLYFRONT_Y.Pemax;
  rtb_Saturation22 = (1.0 - (1.0 - ripartizione_idx_2)) * rtb_Min2 / 2.0;
  rtb_DeadZone_do = rtb_Integrator_jj / rtb_Saturation22;
  rtb_Integrator_gf = 1.4142135623730951 * rtb_Saturation22 / 0.238;
  if (T_max_idx_0 >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = rtb_DeadZone_do / T_max_idx_0 * rtb_Integrator_gf;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_Integrator_gf) /
      ((rtb_Integrator_gf - rtb_Integrator_jj / 2094.4 / 0.238) /
       -(-49.497474683058329 * rtb_Saturation22 / 21.0));
  } else {
    rtb_Gain_g_idx_1 = T_max_idx_0 * 5.0 * 1.5 * 0.0296;
    rtb_Gain_g_idx_1 = (sqrt(rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1 - 1.215 *
      -rtb_Integrator_jj) - rtb_Gain_g_idx_1) / 0.6075;
    rtb_Gain_g_idx_2 = 0.0;
  }

  t_p_lim = (rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 *
             rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj;
  if (T_max_idx_0 >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = rtb_DeadZone_do / T_max_idx_0 * rtb_Integrator_gf;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_Integrator_gf) /
      ((rtb_Integrator_gf - rtb_Integrator_jj / 2094.4 / 0.238) /
       -(-49.497474683058329 * rtb_Saturation22 / 21.0));
  } else {
    rtb_Gain_g_idx_1 = T_max_idx_0 * 5.0 * 1.5 * 0.0296;
    rtb_Gain_g_idx_1 = (sqrt(rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1 - 1.215 *
      -rtb_Integrator_jj) - rtb_Gain_g_idx_1) / 0.6075;
    rtb_Gain_g_idx_2 = 0.0;
  }

  /* Gain: '<S19>/Gain3' incorporates:
   *  Abs: '<S19>/Abs1'
   *  MATLAB Function: '<S19>/MATLAB Function1'
   */
  ControlSystems_ONLYFRONT_Y.Regenpower = -fabs(((rtb_Gain_g_idx_2 *
    rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1) * 0.30375 +
    rtb_Integrator_jj) + ((rtb_Gain1 + P_idx_1) + t_p_lim));

  /* Gain: '<S19>/Gain2' incorporates:
   *  Product: '<S19>/Product5'
   *  Sum: '<S19>/Sum of Elements'
   */
  rtb_Integrator_gf = ripartizione_idx_2 * ControlSystems_ONLYFRONT_Y.Regenpower
    * 0.5;

  /* Saturate: '<S19>/Saturation1' */
  if (rtb_Integrator_gf < -35000.0) {
    rtb_Integrator_gf = -35000.0;
  }

  /* End of Saturate: '<S19>/Saturation1' */

  /* Product: '<S19>/Divide1' incorporates:
   *  Constant: '<S19>/Constant'
   *  Sum: '<S19>/Sum'
   */
  rtb_Integrator_jj = (1.0 - ripartizione_idx_2) / ripartizione_idx_2;

  /* Product: '<S19>/Divide4' incorporates:
   *  Product: '<S19>/Divide'
   */
  rtb_Gain1 = rtb_Integrator_gf / rtb_Min_g_idx_0;

  /* Product: '<S19>/Product4' incorporates:
   *  Gain: '<S13>/Gain'
   *  Gain: '<S19>/Gain7'
   *  MinMax: '<S19>/Max6'
   *  Product: '<S19>/Divide4'
   */
  ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0] = fmax(rtb_Gain1, -rtb_Min2)
    * rtb_Integrator_jj;

  /* Product: '<S19>/Divide4' incorporates:
   *  Product: '<S19>/Divide'
   */
  t_p_lim = rtb_Integrator_gf / rtb_Min_g_idx_1;

  /* Product: '<S19>/Product4' incorporates:
   *  Gain: '<S13>/Gain'
   *  Gain: '<S19>/Gain7'
   *  MinMax: '<S19>/Max6'
   *  Product: '<S19>/Divide4'
   */
  ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1] = fmax(t_p_lim, -rtb_Min2) *
    rtb_Integrator_jj;

  /* MinMax: '<S19>/Max2' incorporates:
   *  Constant: '<S19>/Constant4'
   *  Gain: '<S19>/Gain1'
   *  MinMax: '<S19>/Min3'
   */
  rtb_Min_g_idx_1 = fmax(-rtb_Min2, fmin(0.0, rtb_Gain1));

  /* MinMax: '<S19>/Max3' incorporates:
   *  Constant: '<S19>/Constant3'
   *  MinMax: '<S19>/Min4'
   *  Product: '<S19>/Divide'
   *  Product: '<S19>/Product2'
   */
  T_max_idx_0 = fmax(fmin(rtb_Min_g_idx_2 *
    ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0] / rtb_Min_g_idx_2, 0.0),
                     ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0]);

  /* MinMax: '<S19>/Max2' incorporates:
   *  Constant: '<S19>/Constant4'
   *  Gain: '<S19>/Gain1'
   *  MinMax: '<S19>/Min3'
   */
  rtb_Min_g_idx_0 = fmax(-rtb_Min2, fmin(0.0, t_p_lim));

  /* MinMax: '<S19>/Max3' incorporates:
   *  Constant: '<S19>/Constant3'
   *  MinMax: '<S19>/Max1'
   *  MinMax: '<S19>/Min4'
   *  Product: '<S19>/Divide'
   *  Product: '<S19>/Product2'
   */
  P_idx_1 = fmax(fmin(rtb_DeadZone_f *
                      ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1] /
                      rtb_DeadZone_f, 0.0),
                 ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1]);

  /* MATLAB Function: '<S19>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant'
   */
  rtb_DeadZone_f = (1.0 - ripartizione_idx_2) / ripartizione_idx_2;
  rpm = (float)ripartizione_idx_0 * 0.25F;
  rads = rpm * 3.14159274F / 30.0F;
  rade = rads * 5.0F;
  Imax = fminf((float)ControlSystems_ONLYFRONT_P.Imax_regen, (564.94F - (float)
    rtb_battery_voltage) * 3.0211F);
  Imax = (0.231F * Imax + (float)rtb_battery_voltage) * Imax * 1.0204F;
  rtb_Integrator_jj = (float)(ripartizione_idx_2 * 0.5) * Imax;
  rtb_DeadZone_do = 5.0 * rtb_Integrator_jj / rtb_Min2;
  if (rade >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = (float)rtb_DeadZone_do / rade * (float)rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 * rtb_Min2 /
      21.0));
  } else {
    b = 1.5F * rade * 0.0296F;
    rtb_Gain_g_idx_1 = (sqrtf(b * b - (float)(1.215 * -rtb_Integrator_jj)) - b) /
      0.6075F;
    rtb_Gain_g_idx_2 = 0.0;
  }

  rtb_Min_g_idx_2 = (float)((rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 +
    rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj) +
    0.0083F * rpm;
  rtb_Gain1 = rtb_Min_g_idx_2;
  rtb_Integrator_gf = (float)rtb_Min_g_idx_2 / rads;
  if (rade >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = (float)rtb_DeadZone_do / rade * (float)rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 * rtb_Min2 /
      21.0));
  } else {
    b = 1.5F * rade * 0.0296F;
    rtb_Gain_g_idx_1 = (sqrtf(b * b - (float)(1.215 * -rtb_Integrator_jj)) - b) /
      0.6075F;
    rtb_Gain_g_idx_2 = 0.0;
  }

  rtb_Min_g_idx_2 = (float)((rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 +
    rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj) +
    0.0083F * rpm;
  T_max_idx_1 = (float)rtb_Min_g_idx_2 / rads;
  rtb_Integrator_jj = (float)(rtb_DeadZone_f * 0.5) * Imax;
  rtb_Saturation22 = (1.0 - rtb_DeadZone_f) * rtb_Min2 / 2.0;
  rtb_DeadZone_do = 5.0 * rtb_Integrator_jj / rtb_Saturation22;
  rtb_DeadZone_p = 1.4142135623730951 * rtb_Saturation22 / 0.238;
  if (rade >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = (float)rtb_DeadZone_do / rade * (float)rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 *
      rtb_Saturation22 / 21.0));
  } else {
    b = 1.5F * rade * 0.0296F;
    rtb_Gain_g_idx_1 = (sqrtf(b * b - (float)(1.215 * -rtb_Integrator_jj)) - b) /
      0.6075F;
    rtb_Gain_g_idx_2 = 0.0;
  }

  t_p_lim = (float)((rtb_Gain_g_idx_2 * rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 *
                     rtb_Gain_g_idx_1) * 0.30375 + rtb_Integrator_jj) + 0.0083F *
    rpm;
  if (rade >= rtb_DeadZone_do) {
    rtb_Gain_g_idx_1 = (float)rtb_DeadZone_do / rade * (float)rtb_DeadZone_p;
    rtb_Gain_g_idx_2 = (rtb_Gain_g_idx_1 - rtb_DeadZone_p) / ((rtb_DeadZone_p -
      rtb_Integrator_jj / 2094.4 / 0.238) / -(-49.497474683058329 *
      rtb_Saturation22 / 21.0));
  } else {
    b = 1.5F * rade * 0.0296F;
    rtb_Gain_g_idx_1 = (sqrtf(b * b - (float)(1.215 * -rtb_Integrator_jj)) - b) /
      0.6075F;
    rtb_Gain_g_idx_2 = 0.0;
  }

  T_max_idx_3 = rtb_Integrator_gf * rtb_DeadZone_f;
  T_max_idx_2 = T_max_idx_3;
  rads = 21.0F - (rpm - 13000.0F) * 0.000857F;
  if (rads > 21.0F) {
    rads = 21.0F;
  }

  if (rtb_Min2 > rads) {
    rtb_Min2 = rads;
  }

  ripartizione_idx_2 = rtb_DeadZone_f * rtb_Min2;
  if (rtb_Integrator_gf > rtb_Min2) {
    rtb_Integrator_gf = rtb_Min2;
  }

  if (T_max_idx_1 > rtb_Min2) {
    T_max_idx_1 = rtb_Min2;
  }

  if (T_max_idx_3 > ripartizione_idx_2) {
    T_max_idx_2 = ripartizione_idx_2;
    T_max_idx_3 = ripartizione_idx_2;
  }

  /* Switch: '<S6>/Switch1' incorporates:
   *  Abs: '<S19>/Abs2'
   *  Constant: '<S6>/Constant2'
   *  Gain: '<S19>/Gain5'
   */
  if (rtb_Regen_active > 0.0) {
    /* Switch: '<S19>/Switch' incorporates:
     *  Abs: '<S19>/Abs2'
     *  Constant: '<S19>/Constant6'
     *  Math: '<S19>/Transpose'
     *  Product: '<S19>/Product'
     *  Product: '<S19>/Product1'
     *  Saturate: '<Root>/Saturation3'
     */
    if (ControlSystems_ONLYFRONT_P.REGEN_TYPE > 0.0) {
      rtb_DeadZone_f = rtb_Min_g_idx_1 * ControlSystems_ONLYFRONT_B.regen_input;
      ripartizione_idx_2 = T_max_idx_0 * ControlSystems_ONLYFRONT_B.regen_input;
      rtb_DeadZone_p = rtb_Min_g_idx_0 * ControlSystems_ONLYFRONT_B.regen_input;
      rtb_Min2 = P_idx_1 * ControlSystems_ONLYFRONT_B.regen_input;
    } else {
      rtb_DeadZone_f = ControlSystems_ONLYFRONT_B.regen_input *
        rtb_Integrator_gf;
      rtb_DeadZone_p = ControlSystems_ONLYFRONT_B.regen_input * T_max_idx_1;
      ripartizione_idx_2 = ControlSystems_ONLYFRONT_B.regen_input * T_max_idx_2;
      rtb_Min2 = ControlSystems_ONLYFRONT_B.regen_input * T_max_idx_3;
    }

    /* End of Switch: '<S19>/Switch' */
    ripartizione_idx_0 = -fabs(rtb_DeadZone_f);
    rtb_DeadZone_f = -fabs(rtb_DeadZone_p);
    ripartizione_idx_2 = -fabs(ripartizione_idx_2);
    rtb_DeadZone_do = -fabs(rtb_Min2);
  } else {
    ripartizione_idx_0 = 0.0;
    rtb_DeadZone_f = 0.0;
    ripartizione_idx_2 = 0.0;
    rtb_DeadZone_do = 0.0;
  }

  /* End of Switch: '<S6>/Switch1' */

  /* Sum: '<Root>/Minus' incorporates:
   *  Constant: '<Root>/Constant27'
   *  Gain: '<Root>/Gain11'
   *  Inport: '<Root>/steering_g'
   *  Math: '<Root>/Exp'
   *  Math: '<Root>/Square1'
   *
   * About '<Root>/Exp':
   *  Operator: exp
   */
  rtb_Saturation22 = 1.0 - exp(ControlSystems_ONLYFRONT_U.steering_g *
    ControlSystems_ONLYFRONT_U.steering_g * -0.01);

  /* Saturate: '<Root>/Saturation22' */
  if (rtb_Saturation22 < 0.0) {
    rtb_Saturation22 = 0.0;
  }

  /* End of Saturate: '<Root>/Saturation22' */

  /* Switch: '<S6>/Switch3' incorporates:
   *  Abs: '<S19>/Abs4'
   *  Constant: '<S6>/Constant4'
   *  Gain: '<S19>/Gain8'
   *  Switch: '<S19>/Switch2'
   */
  if (rtb_Regen_active > 0.0) {
    /* Abs: '<S19>/Abs4' incorporates:
     *  Constant: '<S19>/Constant8'
     *  Switch: '<S19>/Switch2'
     */
    if (ControlSystems_ONLYFRONT_P.REGEN_TYPE > 0.0) {
      rtb_Integrator_gf = rtb_Min_g_idx_1;
      T_max_idx_1 = rtb_Min_g_idx_0;
      T_max_idx_2 = T_max_idx_0;
      T_max_idx_3 = P_idx_1;
    }

    ControlSystems_ONLYFRONT_Y.negLimitTorque[0] = -fabs(rtb_Integrator_gf);
    ControlSystems_ONLYFRONT_Y.negLimitTorque[1] = -fabs(T_max_idx_1);
    ControlSystems_ONLYFRONT_Y.negLimitTorque[2] = -fabs(T_max_idx_2);
    ControlSystems_ONLYFRONT_Y.negLimitTorque[3] = -fabs(T_max_idx_3);
  } else {
    ControlSystems_ONLYFRONT_Y.negLimitTorque[0] = 0.0;
    ControlSystems_ONLYFRONT_Y.negLimitTorque[1] = 0.0;
    ControlSystems_ONLYFRONT_Y.negLimitTorque[2] = 0.0;
    ControlSystems_ONLYFRONT_Y.negLimitTorque[3] = 0.0;
  }

  /* End of Switch: '<S6>/Switch3' */
  if (ControlSystems_ONLYFRONT_M->Timing.TaskCounters.TID[1] == 0) {
    /* Switch: '<Root>/Switch2' */
    if (rtb_estimatedspeedmotors > 2.7777777777777777) {
      /* ZeroOrderHold: '<Root>/Zero-Order Hold' */
      ControlSystems_ONLYFRONT_B.ZeroOrderHold = rtb_YMC_lim;
    } else {
      /* ZeroOrderHold: '<Root>/Zero-Order Hold' incorporates:
       *  Constant: '<Root>/Constant1'
       */
      ControlSystems_ONLYFRONT_B.ZeroOrderHold = 0.0;
    }

    /* End of Switch: '<Root>/Switch2' */

    /* RateLimiter: '<S17>/Rate Limiter1' */
    if (ControlSystems_ONLYFRONT_B.ZeroOrderHold -
        ControlSystems_ONLYFRONT_DW.PrevY > ControlSystems_ONLYFRONT_period) {
      /* RateLimiter: '<S17>/Rate Limiter1' */
      ControlSystems_ONLYFRONT_B.RateLimiter1 =
        ControlSystems_ONLYFRONT_DW.PrevY + ControlSystems_ONLYFRONT_period;
    } else {
      /* RateLimiter: '<S17>/Rate Limiter1' */
      ControlSystems_ONLYFRONT_B.RateLimiter1 =
        ControlSystems_ONLYFRONT_B.ZeroOrderHold;
    }

    ControlSystems_ONLYFRONT_DW.PrevY = ControlSystems_ONLYFRONT_B.RateLimiter1;

    /* End of RateLimiter: '<S17>/Rate Limiter1' */
  }

  /* Gain: '<Root>/Gain2' incorporates:
   *  Inport: '<Root>/steering_g'
   */
  rtb_Min_g_idx_1 = 0.017453292519943295 * ControlSystems_ONLYFRONT_U.steering_g;

  /* MATLAB Function: '<Root>/Discrete lowpass filter3' incorporates:
   *  Constant: '<Root>/Constant13'
   *  Constant: '<Root>/Constant6'
   *  Delay: '<Root>/Delay8'
   *  Delay: '<Root>/Delay9'
   *  Gain: '<Root>/Gain5'
   *  Product: '<Root>/Divide'
   */
  ControlSystems_ONLYFRONT_Discretelowpassfilter(rtb_Min_g_idx_1,
    ControlSystems_ONLYFRONT_DW.Delay8_DSTATE, 1.0 / (6.2831853071795862 *
    ControlSystems_ONLYFRONT_P.IN_STEERING_LP_HZ),
    ControlSystems_ONLYFRONT_DW.Delay9_DSTATE,
    &ControlSystems_ONLYFRONT_B.steering_rad);

  /* Gain: '<S13>/Gain1' */
  rtb_YMC_lim = 57.295779513082323 * ControlSystems_ONLYFRONT_B.steering_rad;

  /* Saturate: '<S13>/Saturation9' */
  if (rtb_YMC_lim > 95.0) {
    rtb_YMC_lim = 95.0;
  } else if (rtb_YMC_lim < -95.0) {
    rtb_YMC_lim = -95.0;
  }

  /* End of Saturate: '<S13>/Saturation9' */

  /* Gain: '<S13>/Gain' incorporates:
   *  Lookup_n-D: '<S13>/FL wheel angle'
   *  Lookup_n-D: '<S13>/FR wheel angle'
   *  Sum: '<S563>/Diff'
   */
  ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0] = 0.017453292519943295 *
    look1_binlxpw(rtb_YMC_lim, ControlSystems_ONLYFRONT_ConstP.pooled23,
                  ControlSystems_ONLYFRONT_ConstP.FLwheelangle_tableData, 70U);
  ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1] = 0.017453292519943295 *
    look1_binlxpw(rtb_YMC_lim, ControlSystems_ONLYFRONT_ConstP.pooled23,
                  ControlSystems_ONLYFRONT_ConstP.FRwheelangle_tableData, 70U);

  /* Gain: '<S17>/Gain19' incorporates:
   *  Sum: '<S17>/Sum of Elements6'
   */
  rtb_Gain19 = (ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0] +
                ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1]) * 0.5;

  /* MATLAB Function: '<Root>/MATLAB Function3' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  T_max_idx_2 = tmpForInput[0] + ripartizione_idx_0;
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  if (T_max_idx_2 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_2;
  } else if (T_max_idx_2 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_2;
  }

  u0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[0] - T_max_idx_2;
  rtb_Min_g_idx_0 = fmax(fmin(fmax(T_max_idx_3, u0), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) * rtb_Saturation22;
  rtb_Min2 = rtb_Min_g_idx_0;
  T_max_idx_1 = tmpForInput[1] + rtb_DeadZone_f;
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (T_max_idx_1 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_1;
    rtb_DeadZone_c = fmax(fmin(T_max_idx_1,
      ControlSystems_ONLYFRONT_Y.limitTorque[1] - T_max_idx_1), 0.0);
  } else if (T_max_idx_1 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_1;
    rtb_DeadZone_c = -T_max_idx_1;
  }

  rtb_DeadZone_c = fmin(rtb_DeadZone_c,
                        ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) *
    rtb_Saturation22;
  rtb_DeadZone_p = rtb_DeadZone_c;
  rtb_Sliperror_idx_0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[1] -
    T_max_idx_1;
  rtb_YMCTorque_idx_1 = fmax(fmin(fmax(T_max_idx_3, rtb_Sliperror_idx_0), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) * rtb_Saturation22;

  /* MATLAB Function: '<S17>/MATLAB Function' */
  rtb_YMC_lim = fabs(rtb_Min_g_idx_0) * 12.667 * 0.615 / 0.2032 + fabs
    (rtb_DeadZone_c) * 12.667 * 0.615 / 0.2032;

  /* MATLAB Function: '<Root>/MATLAB Function3' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  P_idx_1 = tmpForInput[2] + ripartizione_idx_2;
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (P_idx_1 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      P_idx_1;
    rtb_DeadZone_c = fmax(fmin(P_idx_1, ControlSystems_ONLYFRONT_Y.limitTorque[2]
      - P_idx_1), 0.0);
  } else if (P_idx_1 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      P_idx_1;
    rtb_DeadZone_c = -P_idx_1;
  }

  rtb_YMCTorque_idx_3 = fmin(rtb_DeadZone_c,
    ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) * rtb_Saturation22;
  rtb_Sliperror_idx_1 = ControlSystems_ONLYFRONT_Y.negLimitTorque[2] - P_idx_1;
  rtb_Min_g_idx_0 = fmax(fmin(fmax(T_max_idx_3, rtb_Sliperror_idx_1), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) * rtb_Saturation22;

  /* MATLAB Function: '<S17>/MATLAB Function' */
  rtb_YMC_lim += fabs(rtb_Min_g_idx_0) * 12.667 * 0.6 / 0.2032;

  /* MATLAB Function: '<Root>/MATLAB Function3' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  T_max_idx_0 = tmpForInput[3] + rtb_DeadZone_do;
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (T_max_idx_0 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_0;
    rtb_DeadZone_c = fmax(fmin(T_max_idx_0,
      ControlSystems_ONLYFRONT_Y.limitTorque[3] - T_max_idx_0), 0.0);
  } else if (T_max_idx_0 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_0;
    rtb_DeadZone_c = -T_max_idx_0;
  }

  rtb_DeadZone_c = fmin(rtb_DeadZone_c,
                        ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) *
    rtb_Saturation22;

  /* MATLAB Function: '<S17>/MATLAB Function' */
  rtb_YMC_lim += fabs(rtb_DeadZone_c) * 12.667 * 0.6 / 0.2032;

  /* Saturate: '<S17>/Saturation' */
  if (rtb_YMC_lim > 3181.0876476377953) {
    rtb_YMC_lim = 3181.0876476377953;
  }

  /* End of Saturate: '<S17>/Saturation' */

  /* Saturate: '<Root>/Saturation2' */
  if (rtb_estimatedspeedmotors <= 1.0) {
    rtb_Integrator_gf = 1.0;
  } else {
    rtb_Integrator_gf = rtb_estimatedspeedmotors;
  }

  /* End of Saturate: '<Root>/Saturation2' */

  /* MATLAB Function: '<Root>/MATLAB Function' */
  deltaK = rtb_Integrator_gf * rtb_Integrator_gf;
  s_im = 2.9514153148042E+7 / (30572.800000000003 * rtb_Integrator_gf);
  deltaK = (deltaK * 272.0 * 870.80549999998766 + 6.917581902226799E+9) /
    (deltaK * 30572.800000000003) + (s_im * 0.0 - 157.91367041742973);
  s_im *= 12.566370614359172;
  if (s_im == 0.0) {
    deltaK = 1.0 / deltaK;
    s_im = 0.0;
  } else if (deltaK == 0.0) {
    deltaK = 0.0;
    s_im = -(1.0 / s_im);
  } else {
    frontMotorTorque = fabs(deltaK);
    if (frontMotorTorque > s_im) {
      frontMotorTorque = s_im / deltaK;
      s_im = frontMotorTorque * s_im + deltaK;
      deltaK = (frontMotorTorque * 0.0 + 1.0) / s_im;
      s_im = (0.0 - frontMotorTorque) / s_im;
    } else if (s_im == frontMotorTorque) {
      deltaK = (deltaK > 0.0 ? 0.5 : -0.5) / frontMotorTorque;
      s_im = -0.5 / frontMotorTorque;
    } else {
      frontMotorTorque = deltaK / s_im;
      s_im += frontMotorTorque * deltaK;
      deltaK = frontMotorTorque / s_im;
      s_im = (frontMotorTorque * 0.0 - 1.0) / s_im;
    }
  }

  frontMotorTorque = 109034.0 / (272.0 * rtb_Integrator_gf * 112.4);
  rtb_Integrator_gf = frontMotorTorque * deltaK - 0.11180045030568658 * s_im;
  s_im = frontMotorTorque * s_im + 0.11180045030568658 * deltaK;
  deltaK = 1.0 / rt_hypotd_snf(rtb_Integrator_gf, s_im);
  rtb_Integrator_gf = -2.4434609527920612 - rt_atan2d_snf(s_im,
    rtb_Integrator_gf);
  ControlSystems_ONLYFRONT_B.Saturation26 = -deltaK * cos(rtb_Integrator_gf);

  /* Saturate: '<Root>/Saturation26' */
  if (ControlSystems_ONLYFRONT_B.Saturation26 > 10000.0) {
    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Saturate: '<Root>/Saturation26'
     */
    ControlSystems_ONLYFRONT_B.Saturation26 = 10000.0;
  } else if (ControlSystems_ONLYFRONT_B.Saturation26 < 0.0) {
    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Saturate: '<Root>/Saturation26'
     */
    ControlSystems_ONLYFRONT_B.Saturation26 = 0.0;
  }

  /* End of Saturate: '<Root>/Saturation26' */

  /* MATLAB Function: '<Root>/MATLAB Function' */
  ControlSystems_ONLYFRONT_B.Saturation27 = -12.566370614359172 * deltaK * sin
    (rtb_Integrator_gf) * 0.001;

  /* Saturate: '<Root>/Saturation27' */
  if (ControlSystems_ONLYFRONT_B.Saturation27 > 10000.0) {
    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Saturate: '<Root>/Saturation27'
     */
    ControlSystems_ONLYFRONT_B.Saturation27 = 10000.0;
  } else if (ControlSystems_ONLYFRONT_B.Saturation27 < 0.0) {
    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Saturate: '<Root>/Saturation27'
     */
    ControlSystems_ONLYFRONT_B.Saturation27 = 0.0;
  }

  /* End of Saturate: '<Root>/Saturation27' */

  /* MATLAB Function: '<S532>/MATLAB Function' */
  rtb_Integrator_gf = -2.0 / (exp(-rtb_estimatedspeedmotors +
    ControlSystems_ONLYFRONT_P.TV_HIGH_SPEED_THRESHOLD) + 1.0) + 1.0;
  deltaK = 1.0;
  if (rtb_Integrator_gf > 0.0) {
    deltaK = rtb_Integrator_gf * ControlSystems_ONLYFRONT_P.TV_YR_LOW_SPEED_MULT
      + 1.0;
  } else if (rtb_Integrator_gf < 0.0) {
    deltaK = 1.0 - rtb_Integrator_gf *
      ControlSystems_ONLYFRONT_P.TV_YR_HIGH_SPEED_MULT;
  }

  /* End of MATLAB Function: '<S532>/MATLAB Function' */

  /* Sum: '<S532>/Sum' incorporates:
   *  Constant: '<S532>/Constant'
   *  Constant: '<S532>/wheelbase'
   *  Gain: '<S532>/Gain1'
   *  Product: '<S532>/Product1'
   */
  rtb_Integrator_gf = rtb_yaw_rate * 0.07 * 0.017453292519943295 + 1.535;

  /* Switch: '<S532>/Switch' incorporates:
   *  Constant: '<S532>/Constant1'
   *  Constant: '<S532>/Constant2'
   */
  if (!(ControlSystems_ONLYFRONT_P.TV_ENABLE_YR_MULT > 0.0)) {
    deltaK = 1.0;
  }

  /* Saturate: '<S532>/Saturation' */
  if (rtb_Integrator_gf <= 1.0E-12) {
    rtb_Integrator_gf = 1.0E-12;
  }

  /* Product: '<S532>/Product2' incorporates:
   *  Product: '<S532>/Divide'
   *  Product: '<S532>/Product'
   *  Saturate: '<S532>/Saturation'
   *  Switch: '<S532>/Switch'
   */
  ControlSystems_ONLYFRONT_B.Product2 = rtb_Gain19 * rtb_estimatedspeedmotors /
    rtb_Integrator_gf * deltaK;

  /* Saturate: '<Root>/Saturation11' incorporates:
   *  Inport: '<Root>/yaw_rate'
   */
  if (ControlSystems_ONLYFRONT_U.yaw_rate > 6.2831853071795862) {
    rtb_yaw_rate = 6.2831853071795862;
  } else if (ControlSystems_ONLYFRONT_U.yaw_rate < -6.2831853071795862) {
    rtb_yaw_rate = -6.2831853071795862;
  } else {
    rtb_yaw_rate = ControlSystems_ONLYFRONT_U.yaw_rate;
  }

  /* End of Saturate: '<Root>/Saturation11' */

  /* MATLAB Function: '<Root>/Discrete lowpass filter2' incorporates:
   *  Constant: '<Root>/Constant18'
   *  Constant: '<Root>/Constant3'
   *  Delay: '<Root>/Delay6'
   *  Delay: '<Root>/Delay7'
   *  Gain: '<Root>/Gain9'
   *  Product: '<Root>/Divide3'
   */
  ControlSystems_ONLYFRONT_Discretelowpassfilter(rtb_yaw_rate,
    ControlSystems_ONLYFRONT_DW.Delay6_DSTATE, 1.0 / (6.2831853071795862 *
    ControlSystems_ONLYFRONT_P.IN_YAW_RATE_LP_HZ),
    ControlSystems_ONLYFRONT_DW.Delay7_DSTATE, &ControlSystems_ONLYFRONT_B.y);

  /* Sum: '<S17>/Sum' */
  ControlSystems_ONLYFRONT_B.yr_e = ControlSystems_ONLYFRONT_B.Product2 -
    ControlSystems_ONLYFRONT_B.y;

  /* DiscreteIntegrator: '<S571>/Integrator' */
  if (((ControlSystems_ONLYFRONT_B.ZeroOrderHold > 0.0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState <= 0)) ||
      ((ControlSystems_ONLYFRONT_B.ZeroOrderHold <= 0.0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState == 1))) {
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE = 0.0;
  }

  /* Sum: '<S581>/Sum' incorporates:
   *  DiscreteIntegrator: '<S571>/Integrator'
   *  Product: '<S17>/Product'
   *  Product: '<S576>/PProd Out'
   */
  rtb_Integrator_gf = ControlSystems_ONLYFRONT_B.Saturation26 *
    ControlSystems_ONLYFRONT_B.RateLimiter1 * ControlSystems_ONLYFRONT_B.yr_e +
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE;

  /* Switch: '<S579>/Switch2' incorporates:
   *  Gain: '<S17>/Gain'
   *  RelationalOperator: '<S579>/LowerRelop1'
   *  RelationalOperator: '<S579>/UpperRelop'
   *  Switch: '<S579>/Switch'
   */
  if (rtb_Integrator_gf > rtb_YMC_lim) {
    /* Switch: '<S579>/Switch2' */
    ControlSystems_ONLYFRONT_B.YMC = rtb_YMC_lim;
  } else if (rtb_Integrator_gf < -rtb_YMC_lim) {
    /* Switch: '<S579>/Switch' incorporates:
     *  Gain: '<S17>/Gain'
     *  Switch: '<S579>/Switch2'
     */
    ControlSystems_ONLYFRONT_B.YMC = -rtb_YMC_lim;
  } else {
    /* Switch: '<S579>/Switch2' incorporates:
     *  Switch: '<S579>/Switch'
     */
    ControlSystems_ONLYFRONT_B.YMC = rtb_Integrator_gf;
  }

  /* End of Switch: '<S579>/Switch2' */

  /* MATLAB Function: '<S533>/MATLAB Function' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function3'
   */
  frontMotorTorque = ControlSystems_ONLYFRONT_P.TV_YMC_REPARTITION *
    ControlSystems_ONLYFRONT_B.YMC / 0.615 / cos(rtb_Gain19) * 0.2032 / 12.667;
  deltaK = (1.0 - ControlSystems_ONLYFRONT_P.TV_YMC_REPARTITION) *
    ControlSystems_ONLYFRONT_B.YMC / 0.6 * 0.2032 / 12.667;
  if (frontMotorTorque > 0.0) {
    s_im = frontMotorTorque / 2.0;
    rtb_YMCTorque_idx_1 = fmin(s_im, rtb_DeadZone_p);
    rtb_Gain19 = fmax(-frontMotorTorque / 2.0 - (s_im - rtb_YMCTorque_idx_1),
                      rtb_Min2);
  } else {
    s_im = -frontMotorTorque / 2.0;
    rtb_Gain19 = fmin(s_im, rtb_DeadZone_p);
    rtb_YMCTorque_idx_1 = fmax(frontMotorTorque / 2.0 - (s_im - rtb_Gain19),
      rtb_YMCTorque_idx_1);
  }

  if (deltaK > 0.0) {
    s_im = deltaK / 2.0;
    rtb_YMCTorque_idx_3 = fmin(s_im, rtb_DeadZone_c);
    frontMotorTorque = fmax(-deltaK / 2.0 - (s_im - rtb_YMCTorque_idx_3),
      rtb_Min_g_idx_0);
  } else {
    s_im = -deltaK / 2.0;
    frontMotorTorque = fmin(s_im, rtb_YMCTorque_idx_3);
    rtb_YMCTorque_idx_3 = fmax(deltaK / 2.0 - (s_im - frontMotorTorque), fmax
      (fmin(fmax(T_max_idx_3, ControlSystems_ONLYFRONT_Y.negLimitTorque[3] -
                 T_max_idx_0), 0.0),
       -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT) * rtb_Saturation22);
  }

  /* Switch: '<S15>/Switch' */
  rtb_DeadZone_p = (rtb_estimatedspeedmotors > 0.5);

  /* Product: '<S17>/Product1' incorporates:
   *  MATLAB Function: '<S533>/MATLAB Function'
   */
  ControlSystems_ONLYFRONT_B.Product1[0] =
    ControlSystems_ONLYFRONT_B.RateLimiter1 * rtb_Gain19;

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function3'
   *  Product: '<S17>/Product1'
   */
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (T_max_idx_2 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_2;
    rtb_DeadZone_c = fmax(fmin(T_max_idx_2,
      ControlSystems_ONLYFRONT_Y.limitTorque[0] - T_max_idx_2), 0.0);
  } else if (T_max_idx_2 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_2;
    rtb_DeadZone_c = -T_max_idx_2;
  }

  rtb_Min_g_idx_0 = fmin(fmax(ControlSystems_ONLYFRONT_B.Product1[0], fmax(fmin
    (fmax(T_max_idx_3, u0), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)), fmin(rtb_DeadZone_c,
    ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)) * rtb_Saturation22;
  rtb_Min2 = rtb_Min_g_idx_0;

  /* Sum: '<Root>/Add' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  rtb_Gain19 = (rtb_Min_g_idx_0 + ripartizione_idx_0) + tmpForInput[0];

  /* Product: '<S15>/Product' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  ripartizione_idx_0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[0] *
    rtb_DeadZone_p;

  /* Switch: '<S529>/Switch2' incorporates:
   *  RelationalOperator: '<S529>/LowerRelop1'
   *  RelationalOperator: '<S529>/UpperRelop'
   *  Switch: '<S529>/Switch'
   */
  if (rtb_Gain19 > ControlSystems_ONLYFRONT_Y.limitTorque[0]) {
    /* Switch: '<S529>/Switch2' */
    ControlSystems_ONLYFRONT_B.Switch2[0] =
      ControlSystems_ONLYFRONT_Y.limitTorque[0];
  } else if (rtb_Gain19 < ripartizione_idx_0) {
    /* Switch: '<S529>/Switch' incorporates:
     *  Switch: '<S529>/Switch2'
     */
    ControlSystems_ONLYFRONT_B.Switch2[0] = ripartizione_idx_0;
  } else {
    /* Switch: '<S529>/Switch2' incorporates:
     *  Switch: '<S529>/Switch'
     */
    ControlSystems_ONLYFRONT_B.Switch2[0] = rtb_Gain19;
  }

  /* Product: '<S17>/Product1' incorporates:
   *  MATLAB Function: '<S533>/MATLAB Function'
   */
  ControlSystems_ONLYFRONT_B.Product1[1] =
    ControlSystems_ONLYFRONT_B.RateLimiter1 * rtb_YMCTorque_idx_1;

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function3'
   *  Product: '<S17>/Product1'
   */
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (T_max_idx_1 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_1;
    rtb_DeadZone_c = fmax(fmin(T_max_idx_1,
      ControlSystems_ONLYFRONT_Y.limitTorque[1] - T_max_idx_1), 0.0);
  } else if (T_max_idx_1 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_1;
    rtb_DeadZone_c = -T_max_idx_1;
  }

  rtb_Min_g_idx_0 = fmin(fmax(ControlSystems_ONLYFRONT_B.Product1[1], fmax(fmin
    (fmax(T_max_idx_3, rtb_Sliperror_idx_0), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)), fmin(rtb_DeadZone_c,
    ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)) * rtb_Saturation22;
  T_max_idx_1 = rtb_Min_g_idx_0;

  /* Sum: '<Root>/Add' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  rtb_Gain19 = (rtb_Min_g_idx_0 + rtb_DeadZone_f) + tmpForInput[1];

  /* Product: '<S15>/Product' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  ripartizione_idx_0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[1] *
    rtb_DeadZone_p;

  /* Switch: '<S529>/Switch2' incorporates:
   *  RelationalOperator: '<S529>/LowerRelop1'
   *  RelationalOperator: '<S529>/UpperRelop'
   *  Switch: '<S529>/Switch'
   */
  if (rtb_Gain19 > ControlSystems_ONLYFRONT_Y.limitTorque[1]) {
    /* Switch: '<S529>/Switch2' */
    ControlSystems_ONLYFRONT_B.Switch2[1] =
      ControlSystems_ONLYFRONT_Y.limitTorque[1];
  } else if (rtb_Gain19 < ripartizione_idx_0) {
    /* Switch: '<S529>/Switch' incorporates:
     *  Switch: '<S529>/Switch2'
     */
    ControlSystems_ONLYFRONT_B.Switch2[1] = ripartizione_idx_0;
  } else {
    /* Switch: '<S529>/Switch2' incorporates:
     *  Switch: '<S529>/Switch'
     */
    ControlSystems_ONLYFRONT_B.Switch2[1] = rtb_Gain19;
  }

  /* Product: '<S17>/Product1' incorporates:
   *  MATLAB Function: '<S533>/MATLAB Function'
   */
  ControlSystems_ONLYFRONT_B.Product1[2] =
    ControlSystems_ONLYFRONT_B.RateLimiter1 * frontMotorTorque;

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function3'
   *  Product: '<S17>/Product1'
   */
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (P_idx_1 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      P_idx_1;
    rtb_DeadZone_c = fmax(fmin(P_idx_1, ControlSystems_ONLYFRONT_Y.limitTorque[2]
      - P_idx_1), 0.0);
  } else if (P_idx_1 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      P_idx_1;
    rtb_DeadZone_c = -P_idx_1;
  }

  rtb_Min_g_idx_0 = fmin(fmax(ControlSystems_ONLYFRONT_B.Product1[2], fmax(fmin
    (fmax(T_max_idx_3, rtb_Sliperror_idx_1), 0.0),
    -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)), fmin(rtb_DeadZone_c,
    ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)) * rtb_Saturation22;
  T_max_idx_2 = rtb_Min_g_idx_0;

  /* Sum: '<Root>/Add' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  rtb_Gain19 = (rtb_Min_g_idx_0 + ripartizione_idx_2) + tmpForInput[2];

  /* Product: '<S15>/Product' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  ripartizione_idx_0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[2] *
    rtb_DeadZone_p;

  /* Switch: '<S529>/Switch2' incorporates:
   *  RelationalOperator: '<S529>/LowerRelop1'
   *  RelationalOperator: '<S529>/UpperRelop'
   *  Switch: '<S529>/Switch'
   */
  if (rtb_Gain19 > ControlSystems_ONLYFRONT_Y.limitTorque[2]) {
    /* Switch: '<S529>/Switch2' */
    ControlSystems_ONLYFRONT_B.Switch2[2] =
      ControlSystems_ONLYFRONT_Y.limitTorque[2];
  } else if (rtb_Gain19 < ripartizione_idx_0) {
    /* Switch: '<S529>/Switch' incorporates:
     *  Switch: '<S529>/Switch2'
     */
    ControlSystems_ONLYFRONT_B.Switch2[2] = ripartizione_idx_0;
  } else {
    /* Switch: '<S529>/Switch2' incorporates:
     *  Switch: '<S529>/Switch'
     */
    ControlSystems_ONLYFRONT_B.Switch2[2] = rtb_Gain19;
  }

  /* Product: '<S17>/Product1' incorporates:
   *  MATLAB Function: '<S533>/MATLAB Function'
   */
  ControlSystems_ONLYFRONT_B.Product1[3] =
    ControlSystems_ONLYFRONT_B.RateLimiter1 * rtb_YMCTorque_idx_3;

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function3'
   *  Product: '<S17>/Product1'
   */
  T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING;
  rtb_DeadZone_c = 0.0;
  if (T_max_idx_0 > 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING -
      T_max_idx_0;
    rtb_DeadZone_c = fmax(fmin(T_max_idx_0,
      ControlSystems_ONLYFRONT_Y.limitTorque[3] - T_max_idx_0), 0.0);
  } else if (T_max_idx_0 < 0.0) {
    T_max_idx_3 = -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT_COASTING +
      T_max_idx_0;
    rtb_DeadZone_c = -T_max_idx_0;
  }

  rtb_Min_g_idx_0 = fmin(fmax(ControlSystems_ONLYFRONT_B.Product1[3], fmax(fmin
    (fmax(T_max_idx_3, ControlSystems_ONLYFRONT_Y.negLimitTorque[3] -
          T_max_idx_0), 0.0), -ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)),
    fmin(rtb_DeadZone_c, ControlSystems_ONLYFRONT_P.YMC_TRQ_DELTA_LIMIT)) *
    rtb_Saturation22;

  /* Sum: '<Root>/Add' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  rtb_Gain19 = (rtb_Min_g_idx_0 + rtb_DeadZone_do) + tmpForInput[3];

  /* Product: '<S15>/Product' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  ripartizione_idx_0 = ControlSystems_ONLYFRONT_Y.negLimitTorque[3] *
    rtb_DeadZone_p;

  /* Switch: '<S529>/Switch2' incorporates:
   *  RelationalOperator: '<S529>/LowerRelop1'
   *  RelationalOperator: '<S529>/UpperRelop'
   *  Switch: '<S529>/Switch'
   */
  if (rtb_Gain19 > ControlSystems_ONLYFRONT_Y.limitTorque[3]) {
    /* Switch: '<S529>/Switch2' */
    ControlSystems_ONLYFRONT_B.Switch2[3] =
      ControlSystems_ONLYFRONT_Y.limitTorque[3];
  } else if (rtb_Gain19 < ripartizione_idx_0) {
    /* Switch: '<S529>/Switch' incorporates:
     *  Switch: '<S529>/Switch2'
     */
    ControlSystems_ONLYFRONT_B.Switch2[3] = ripartizione_idx_0;
  } else {
    /* Switch: '<S529>/Switch2' incorporates:
     *  Switch: '<S529>/Switch'
     */
    ControlSystems_ONLYFRONT_B.Switch2[3] = rtb_Gain19;
  }

  /* MinMax: '<S88>/Max1' incorporates:
   *  Constant: '<S88>/Constant3'
   */
  T_max_idx_0 = fmax(1.5, rtb_estimatedspeedmotors);

  /* MinMax: '<S88>/Max9' incorporates:
   *  Constant: '<S14>/Constant6'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S88>/Constant15'
   *  Constant: '<S88>/Constant16'
   *  Constant: '<S88>/Constant6'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain6'
   *  Gain: '<S7>/Gain'
   *  MinMax: '<S14>/Max'
   *  MinMax: '<S88>/Max1'
   *  MinMax: '<S88>/Min3'
   *  Product: '<Root>/Product'
   *  Product: '<S14>/Product2'
   *  Product: '<S88>/Divide1'
   *  Sum: '<S7>/Sum'
   *  Sum: '<S88>/Sum3'
   *  Trigonometry: '<S14>/Cos'
   */
  rtb_Gain19 = fmax(fmin(fmax((-3.533E-6 * ControlSystems_ONLYFRONT_Y.Fz[0] +
    0.2032) * (0.10471975511965977 * ControlSystems_ONLYFRONT_B.rpm_fl) *
    0.078945290913397012, 1.0E-5) * cos
    (ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[0]) / T_max_idx_0 - 1.0, 1.0),
                    -1.0);
  ripartizione_idx_0 = rtb_Gain19;

  /* RelationalOperator: '<S14>/Relational Operator1' incorporates:
   *  Constant: '<S14>/Constant13'
   */
  rtb_RelationalOperator1_0 = (ControlSystems_ONLYFRONT_B.Switch2[0] > 0.0);
  rtb_RelationalOperator1_idx_0 = rtb_RelationalOperator1_0;

  /* Product: '<S14>/Product10' */
  tmpForInput[0] = rtb_Gain19 * (double)rtb_RelationalOperator1_0;

  /* MinMax: '<S88>/Max9' incorporates:
   *  Constant: '<S14>/Constant6'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S88>/Constant15'
   *  Constant: '<S88>/Constant16'
   *  Constant: '<S88>/Constant6'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain6'
   *  Gain: '<S7>/Gain'
   *  MinMax: '<S14>/Max'
   *  MinMax: '<S88>/Min3'
   *  Product: '<Root>/Product'
   *  Product: '<S14>/Product2'
   *  Product: '<S88>/Divide1'
   *  Sum: '<S7>/Sum'
   *  Sum: '<S88>/Sum3'
   *  Trigonometry: '<S14>/Cos'
   */
  rtb_Gain19 = fmax(fmin(fmax((-3.533E-6 * ControlSystems_ONLYFRONT_Y.Fz[1] +
    0.2032) * (0.10471975511965977 * ControlSystems_ONLYFRONT_B.rpm_fr) *
    0.078945290913397012, 1.0E-5) * cos
    (ControlSystems_ONLYFRONT_Y.Frontwheelsanglerad[1]) / T_max_idx_0 - 1.0, 1.0),
                    -1.0);
  rtb_DeadZone_f = rtb_Gain19;

  /* RelationalOperator: '<S14>/Relational Operator1' incorporates:
   *  Constant: '<S14>/Constant13'
   */
  rtb_RelationalOperator1_0 = (ControlSystems_ONLYFRONT_B.Switch2[1] > 0.0);
  rtb_RelationalOperator1_idx_1 = rtb_RelationalOperator1_0;

  /* Product: '<S14>/Product10' */
  tmpForInput[1] = rtb_Gain19 * (double)rtb_RelationalOperator1_0;

  /* MinMax: '<S88>/Max2' incorporates:
   *  Constant: '<S88>/Constant2'
   */
  T_max_idx_0 = fmax(rtb_estimatedspeedmotors, 1.5);

  /* MinMax: '<S88>/Max9' incorporates:
   *  Constant: '<S14>/Constant6'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S88>/Constant16'
   *  Constant: '<S88>/Constant6'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain6'
   *  Gain: '<S7>/Gain'
   *  MinMax: '<S14>/Max'
   *  MinMax: '<S88>/Max2'
   *  MinMax: '<S88>/Min3'
   *  Product: '<Root>/Product'
   *  Product: '<S88>/Divide1'
   *  Sum: '<S7>/Sum'
   *  Sum: '<S88>/Sum3'
   */
  rtb_Gain19 = fmin(fmax((-3.533E-6 * ControlSystems_ONLYFRONT_Y.Fz[2] + 0.2032)
    * (0.10471975511965977 * ControlSystems_ONLYFRONT_B.rpm_rl) *
    0.078945290913397012, 1.0E-5) / T_max_idx_0 - 1.0, 1.0);
  ripartizione_idx_2 = rtb_Gain19;

  /* RelationalOperator: '<S14>/Relational Operator1' incorporates:
   *  Constant: '<S14>/Constant13'
   */
  rtb_RelationalOperator1_0 = (ControlSystems_ONLYFRONT_B.Switch2[2] > 0.0);
  rtb_RelationalOperator1_idx_2 = rtb_RelationalOperator1_0;

  /* Product: '<S14>/Product10' */
  tmpForInput[2] = rtb_Gain19 * (double)rtb_RelationalOperator1_0;

  /* MinMax: '<S88>/Max9' incorporates:
   *  Constant: '<S14>/Constant6'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S88>/Constant16'
   *  Constant: '<S88>/Constant6'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain6'
   *  Gain: '<S7>/Gain'
   *  MinMax: '<S14>/Max'
   *  MinMax: '<S88>/Min3'
   *  Product: '<Root>/Product'
   *  Product: '<S88>/Divide1'
   *  Sum: '<S7>/Sum'
   *  Sum: '<S88>/Sum3'
   */
  rtb_Gain19 = fmin(fmax((-3.533E-6 * ControlSystems_ONLYFRONT_Y.Fz[3] + 0.2032)
    * (0.10471975511965977 * ControlSystems_ONLYFRONT_B.rpm_rr) *
    0.078945290913397012, 1.0E-5) / T_max_idx_0 - 1.0, 1.0);

  /* RelationalOperator: '<S14>/Relational Operator1' incorporates:
   *  Constant: '<S14>/Constant13'
   */
  rtb_RelationalOperator1_0 = (ControlSystems_ONLYFRONT_B.Switch2[3] > 0.0);

  /* Sum: '<S86>/Sum4' incorporates:
   *  Constant: '<S86>/Desired slip front2'
   */
  T_max_idx_0 = tmpForInput[0] - ControlSystems_ONLYFRONT_P.TC_CL_TARGET_SLIP_F;
  P_idx_1 = tmpForInput[1] - ControlSystems_ONLYFRONT_P.TC_CL_TARGET_SLIP_F;

  /* DiscreteIntegrator: '<S183>/Integrator' */
  if ((rtb_RelationalOperator1_idx_0 &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_p <= 0)) ||
      ((!rtb_RelationalOperator1_idx_0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_p == 1))) {
    ControlSystems_ONLYFRONT_DW.u = 0.0;
  }

  /* Sum: '<S192>/Sum' incorporates:
   *  DiscreteIntegrator: '<S183>/Integrator'
   *  Gain: '<S188>/Proportional Gain'
   */
  rtb_DeadZone_p = ControlSystems_ONLYFRONT_P.TC_CL_P_F * T_max_idx_0 +
    ControlSystems_ONLYFRONT_DW.u;

  /* DiscreteIntegrator: '<S291>/Integrator' */
  if ((rtb_RelationalOperator1_idx_1 &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_ph <= 0)) ||
      ((!rtb_RelationalOperator1_idx_1) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_ph == 1))) {
    ControlSystems_ONLYFRONT_DW.u_o = 0.0;
  }

  /* Sum: '<S300>/Sum' incorporates:
   *  DiscreteIntegrator: '<S291>/Integrator'
   *  Gain: '<S296>/Proportional Gain'
   */
  rtb_DeadZone_do = ControlSystems_ONLYFRONT_P.TC_CL_P_F * P_idx_1 +
    ControlSystems_ONLYFRONT_DW.u_o;

  /* Sum: '<S86>/Sum3' incorporates:
   *  Constant: '<S86>/Desired slip rear2'
   *  Product: '<S14>/Product10'
   */
  rtb_Saturation22 = tmpForInput[2] -
    ControlSystems_ONLYFRONT_P.TC_CL_TARGET_SLIP_R;
  rtb_YMCTorque_idx_3 = rtb_Gain19 * (double)rtb_RelationalOperator1_0 -
    ControlSystems_ONLYFRONT_P.TC_CL_TARGET_SLIP_R;

  /* DiscreteIntegrator: '<S129>/Integrator' */
  if ((rtb_RelationalOperator1_idx_2 &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_e <= 0)) ||
      ((!rtb_RelationalOperator1_idx_2) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_e == 1))) {
    ControlSystems_ONLYFRONT_DW.u_l = 0.0;
  }

  /* Sum: '<S138>/Sum' incorporates:
   *  DiscreteIntegrator: '<S129>/Integrator'
   *  Gain: '<S134>/Proportional Gain'
   */
  T_max_idx_3 = ControlSystems_ONLYFRONT_P.TC_CL_P_R * rtb_Saturation22 +
    ControlSystems_ONLYFRONT_DW.u_l;

  /* DiscreteIntegrator: '<S237>/Integrator' incorporates:
   *  RelationalOperator: '<S14>/Relational Operator1'
   */
  if ((rtb_RelationalOperator1_0 &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_n <= 0)) ||
      ((!rtb_RelationalOperator1_0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_n == 1))) {
    ControlSystems_ONLYFRONT_DW.u_c = 0.0;
  }

  /* Sum: '<S246>/Sum' incorporates:
   *  DiscreteIntegrator: '<S237>/Integrator'
   *  Gain: '<S242>/Proportional Gain'
   */
  rtb_DeadZone_c = ControlSystems_ONLYFRONT_P.TC_CL_P_R * rtb_YMCTorque_idx_3 +
    ControlSystems_ONLYFRONT_DW.u_c;

  /* Product: '<S14>/Product9' incorporates:
   *  Abs: '<S14>/Abs'
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  ripartizione_idx_2 = (1.0 - (double)rtb_RelationalOperator1_idx_2) * fabs
    (ripartizione_idx_2);

  /* Sum: '<S87>/Sum4' incorporates:
   *  Abs: '<S14>/Abs'
   *  Constant: '<S14>/Constant5'
   *  Constant: '<S87>/Desired slip front2'
   *  Product: '<S14>/Product9'
   *  Sum: '<S14>/Subtract1'
   */
  ripartizione_idx_0 = (1.0 - (double)rtb_RelationalOperator1_idx_0) * fabs
    (ripartizione_idx_0) - ControlSystems_ONLYFRONT_P.ABS_TARGET_SLIP_F;
  s_im = (1.0 - (double)rtb_RelationalOperator1_idx_1) * fabs(rtb_DeadZone_f) -
    ControlSystems_ONLYFRONT_P.ABS_TARGET_SLIP_F;

  /* DiscreteIntegrator: '<S403>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  if (((1 - rtb_RelationalOperator1_idx_0 > 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k <= 0)) || ((1 -
        rtb_RelationalOperator1_idx_0 <= 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k == 1))) {
    ControlSystems_ONLYFRONT_DW.u_h = 0.0;
  }

  /* Sum: '<S412>/Sum' incorporates:
   *  DiscreteIntegrator: '<S403>/Integrator'
   *  Gain: '<S408>/Proportional Gain'
   */
  deltaK = ControlSystems_ONLYFRONT_P.ABS_P_F * ripartizione_idx_0 +
    ControlSystems_ONLYFRONT_DW.u_h;

  /* DiscreteIntegrator: '<S511>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  if (((1 - rtb_RelationalOperator1_idx_1 > 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_m <= 0)) || ((1 -
        rtb_RelationalOperator1_idx_1 <= 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_m == 1))) {
    ControlSystems_ONLYFRONT_DW.u_k = 0.0;
  }

  /* Sum: '<S520>/Sum' incorporates:
   *  DiscreteIntegrator: '<S511>/Integrator'
   *  Gain: '<S516>/Proportional Gain'
   */
  rtb_DeadZone_f = ControlSystems_ONLYFRONT_P.ABS_P_F * s_im +
    ControlSystems_ONLYFRONT_DW.u_k;

  /* Sum: '<S87>/Sum3' incorporates:
   *  Abs: '<S14>/Abs'
   *  Constant: '<S14>/Constant5'
   *  Constant: '<S87>/Desired slip rear2'
   *  Product: '<S14>/Product9'
   *  RelationalOperator: '<S14>/Relational Operator1'
   *  Sum: '<S14>/Subtract1'
   */
  rtb_Sliperror_idx_0 = ripartizione_idx_2 -
    ControlSystems_ONLYFRONT_P.ABS_TARGET_SLIP_R;
  rtb_Sliperror_idx_1 = (1.0 - (double)rtb_RelationalOperator1_0) * fabs
    (rtb_Gain19) - ControlSystems_ONLYFRONT_P.ABS_TARGET_SLIP_R;

  /* DiscreteIntegrator: '<S349>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  if (((1 - rtb_RelationalOperator1_idx_2 > 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_eh <= 0)) || ((1 -
        rtb_RelationalOperator1_idx_2 <= 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_eh == 1))) {
    ControlSystems_ONLYFRONT_DW.u_a = 0.0;
  }

  /* Sum: '<S358>/Sum' incorporates:
   *  DiscreteIntegrator: '<S349>/Integrator'
   *  Gain: '<S354>/Proportional Gain'
   */
  rtb_DeadZone_i = ControlSystems_ONLYFRONT_P.ABS_P_R * rtb_Sliperror_idx_0 +
    ControlSystems_ONLYFRONT_DW.u_a;

  /* DiscreteIntegrator: '<S457>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  RelationalOperator: '<S14>/Relational Operator1'
   *  Sum: '<S14>/Subtract1'
   */
  if (((1 - rtb_RelationalOperator1_0 > 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_en <= 0)) || ((1 -
        rtb_RelationalOperator1_0 <= 0) &&
       (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_en == 1))) {
    ControlSystems_ONLYFRONT_DW.u_e = 0.0;
  }

  /* Sum: '<S466>/Sum' incorporates:
   *  DiscreteIntegrator: '<S457>/Integrator'
   *  Gain: '<S462>/Proportional Gain'
   */
  rtb_DeadZone_b = ControlSystems_ONLYFRONT_P.ABS_P_R * rtb_Sliperror_idx_1 +
    ControlSystems_ONLYFRONT_DW.u_e;

  /* Saturate: '<S190>/Saturation' */
  if (rtb_DeadZone_p > 1.0) {
    rtb_YMCTorque_idx_1 = 1.0;
  } else if (rtb_DeadZone_p < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = rtb_DeadZone_p;
  }

  /* Saturate: '<S410>/Saturation' */
  if (deltaK > 1.0) {
    ripartizione_idx_2 = 1.0;
  } else if (deltaK < 0.0) {
    ripartizione_idx_2 = 0.0;
  } else {
    ripartizione_idx_2 = deltaK;
  }

  /* MinMax: '<S14>/Max1' incorporates:
   *  Constant: '<S14>/Constant10'
   *  Constant: '<S86>/Constant'
   *  Constant: '<S86>/Constant5'
   *  Constant: '<S87>/Constant'
   *  Constant: '<S87>/Constant5'
   *  Gain: '<S86>/Gain'
   *  Gain: '<S87>/Gain'
   *  MinMax: '<S14>/Min1'
   *  MinMax: '<S86>/Min'
   *  MinMax: '<S86>/Min1'
   *  MinMax: '<S87>/Min'
   *  MinMax: '<S87>/Min1'
   *  Saturate: '<S14>/Physical motors limits'
   *  Saturate: '<S190>/Saturation'
   *  Saturate: '<S410>/Saturation'
   *  Sum: '<S14>/Add'
   */
  ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] = fmin(fmax(fmin
    (ControlSystems_ONLYFRONT_P.TC_SCALE * rtb_YMCTorque_idx_1, 0.5), 0.0) +
    fmax(fmin(ControlSystems_ONLYFRONT_P.ABS_SCALE * ripartizione_idx_2, 0.8),
         0.0), 1.0);

  /* Saturate: '<S298>/Saturation' */
  if (rtb_DeadZone_do > 1.0) {
    rtb_YMCTorque_idx_1 = 1.0;
  } else if (rtb_DeadZone_do < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = rtb_DeadZone_do;
  }

  /* Saturate: '<S518>/Saturation' */
  if (rtb_DeadZone_f > 1.0) {
    ripartizione_idx_2 = 1.0;
  } else if (rtb_DeadZone_f < 0.0) {
    ripartizione_idx_2 = 0.0;
  } else {
    ripartizione_idx_2 = rtb_DeadZone_f;
  }

  /* MinMax: '<S14>/Max1' incorporates:
   *  Constant: '<S14>/Constant10'
   *  Constant: '<S86>/Constant'
   *  Constant: '<S86>/Constant5'
   *  Constant: '<S87>/Constant'
   *  Constant: '<S87>/Constant5'
   *  Gain: '<S86>/Gain'
   *  Gain: '<S87>/Gain'
   *  MinMax: '<S14>/Min1'
   *  MinMax: '<S86>/Min'
   *  MinMax: '<S86>/Min1'
   *  MinMax: '<S87>/Min'
   *  MinMax: '<S87>/Min1'
   *  Saturate: '<S14>/Physical motors limits'
   *  Saturate: '<S298>/Saturation'
   *  Saturate: '<S518>/Saturation'
   *  Sum: '<S14>/Add'
   */
  ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] = fmin(fmax(fmin
    (ControlSystems_ONLYFRONT_P.TC_SCALE * rtb_YMCTorque_idx_1, 0.5), 0.0) +
    fmax(fmin(ControlSystems_ONLYFRONT_P.ABS_SCALE * ripartizione_idx_2, 0.8),
         0.0), 1.0);

  /* Saturate: '<S136>/Saturation' */
  if (T_max_idx_3 > 1.0) {
    rtb_YMCTorque_idx_1 = 1.0;
  } else if (T_max_idx_3 < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = T_max_idx_3;
  }

  /* Saturate: '<S356>/Saturation' */
  if (rtb_DeadZone_i > 1.0) {
    ripartizione_idx_2 = 1.0;
  } else if (rtb_DeadZone_i < 0.0) {
    ripartizione_idx_2 = 0.0;
  } else {
    ripartizione_idx_2 = rtb_DeadZone_i;
  }

  /* MinMax: '<S14>/Max1' incorporates:
   *  Constant: '<S14>/Constant10'
   *  Constant: '<S86>/Constant'
   *  Constant: '<S86>/Constant5'
   *  Constant: '<S87>/Constant'
   *  Constant: '<S87>/Constant5'
   *  Gain: '<S86>/Gain'
   *  Gain: '<S87>/Gain'
   *  MinMax: '<S14>/Min1'
   *  MinMax: '<S86>/Min'
   *  MinMax: '<S86>/Min1'
   *  MinMax: '<S87>/Min'
   *  MinMax: '<S87>/Min1'
   *  Saturate: '<S136>/Saturation'
   *  Saturate: '<S14>/Physical motors limits'
   *  Saturate: '<S356>/Saturation'
   *  Sum: '<S14>/Add'
   */
  ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] = fmin(fmax(fmin
    (ControlSystems_ONLYFRONT_P.TC_SCALE * rtb_YMCTorque_idx_1, 0.5), 0.0) +
    fmax(fmin(ControlSystems_ONLYFRONT_P.ABS_SCALE * ripartizione_idx_2, 0.8),
         0.0), 1.0);

  /* Saturate: '<S244>/Saturation' */
  if (rtb_DeadZone_c > 1.0) {
    rtb_YMCTorque_idx_1 = 1.0;
  } else if (rtb_DeadZone_c < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = rtb_DeadZone_c;
  }

  /* Saturate: '<S464>/Saturation' */
  if (rtb_DeadZone_b > 1.0) {
    ripartizione_idx_2 = 1.0;
  } else if (rtb_DeadZone_b < 0.0) {
    ripartizione_idx_2 = 0.0;
  } else {
    ripartizione_idx_2 = rtb_DeadZone_b;
  }

  /* MinMax: '<S14>/Max1' incorporates:
   *  Constant: '<S14>/Constant10'
   *  Constant: '<S86>/Constant'
   *  Constant: '<S86>/Constant5'
   *  Constant: '<S87>/Constant'
   *  Constant: '<S87>/Constant5'
   *  Gain: '<S86>/Gain'
   *  Gain: '<S87>/Gain'
   *  MinMax: '<S14>/Min1'
   *  MinMax: '<S86>/Min'
   *  MinMax: '<S86>/Min1'
   *  MinMax: '<S87>/Min'
   *  MinMax: '<S87>/Min1'
   *  Saturate: '<S14>/Physical motors limits'
   *  Saturate: '<S244>/Saturation'
   *  Saturate: '<S464>/Saturation'
   *  Sum: '<S14>/Add'
   */
  ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] = fmin(fmax(fmin
    (ControlSystems_ONLYFRONT_P.TC_SCALE * rtb_YMCTorque_idx_1, 0.5), 0.0) +
    fmax(fmin(ControlSystems_ONLYFRONT_P.ABS_SCALE * ripartizione_idx_2, 0.8),
         0.0), 1.0);

  /* Product: '<S14>/Product5' incorporates:
   *  Sum: '<S14>/Sum5'
   */
  ControlSystems_ONLYFRONT_B.Torquetocut[0] =
    ControlSystems_ONLYFRONT_B.Switch2[0] *
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0];
  ControlSystems_ONLYFRONT_B.Torquetocut[1] =
    ControlSystems_ONLYFRONT_B.Switch2[1] *
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1];
  ControlSystems_ONLYFRONT_B.Torquetocut[2] =
    ControlSystems_ONLYFRONT_B.Switch2[2] *
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2];
  ControlSystems_ONLYFRONT_B.Torquetocut[3] =
    ControlSystems_ONLYFRONT_B.Switch2[3] *
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3];

  /* Product: '<S14>/Product6' incorporates:
   *  Constant: '<S14>/Constant'
   *  Sum: '<S14>/Add4'
   *  Sum: '<S14>/Sum'
   */
  ControlSystems_ONLYFRONT_B.Product6 = (ControlSystems_ONLYFRONT_B.Torquetocut
    [0] + ControlSystems_ONLYFRONT_B.Switch2[2]) * (1.0 -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2]);

  /* Product: '<S14>/Product7' incorporates:
   *  Constant: '<S14>/Constant1'
   *  Sum: '<S14>/Add3'
   *  Sum: '<S14>/Sum1'
   */
  ControlSystems_ONLYFRONT_B.Product7 = (ControlSystems_ONLYFRONT_B.Torquetocut
    [1] + ControlSystems_ONLYFRONT_B.Switch2[3]) * (1.0 -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3]);

  /* MultiPortSwitch: '<S14>/Multiport Switch' incorporates:
   *  Constant: '<S14>/Constant7'
   *  Constant: '<S14>/Constant8'
   *  Product: '<S14>/Product'
   *  Product: '<S14>/Product8'
   *  Saturate: '<S14>/Physical motors limits'
   *  Sum: '<S14>/Sum5'
   */
  switch ((int32_t)ControlSystems_ONLYFRONT_P.TC_STRAT) {
   case 0:
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] =
      ControlSystems_ONLYFRONT_B.Switch2[0];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] =
      ControlSystems_ONLYFRONT_B.Switch2[1];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] =
      ControlSystems_ONLYFRONT_B.Switch2[2];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] =
      ControlSystems_ONLYFRONT_B.Switch2[3];
    break;

   case 1:
    /* Switch: '<S14>/Switch' incorporates:
     *  Constant: '<S14>/Constant7'
     *  Product: '<S14>/Product'
     *  RelationalOperator: '<S14>/Relational Operator1'
     *  Saturate: '<S14>/Physical motors limits'
     *  Sum: '<S14>/Subtract'
     *  Sum: '<S14>/Sum5'
     */
    if (rtb_RelationalOperator1_idx_0) {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] =
        ControlSystems_ONLYFRONT_B.Switch2[0] -
        ControlSystems_ONLYFRONT_B.Torquetocut[0];
    } else {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] = (1.0 -
        ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0]) *
        ControlSystems_ONLYFRONT_B.Switch2[0];
    }

    if (rtb_RelationalOperator1_idx_1) {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] =
        ControlSystems_ONLYFRONT_B.Switch2[1] -
        ControlSystems_ONLYFRONT_B.Torquetocut[1];
    } else {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] = (1.0 -
        ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1]) *
        ControlSystems_ONLYFRONT_B.Switch2[1];
    }

    if (rtb_RelationalOperator1_idx_2) {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] =
        ControlSystems_ONLYFRONT_B.Product6 -
        ControlSystems_ONLYFRONT_B.Constant2;
    } else {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] = (1.0 -
        ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2]) *
        ControlSystems_ONLYFRONT_B.Switch2[2];
    }

    if (rtb_RelationalOperator1_0) {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] =
        ControlSystems_ONLYFRONT_B.Product7 -
        ControlSystems_ONLYFRONT_B.Constant2;
    } else {
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] = (1.0 -
        ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3]) *
        ControlSystems_ONLYFRONT_B.Switch2[3];
    }

    /* End of Switch: '<S14>/Switch' */
    break;

   case 2:
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] = (1.0 -
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0]) *
      ControlSystems_ONLYFRONT_B.Switch2[0];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] = (1.0 -
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1]) *
      ControlSystems_ONLYFRONT_B.Switch2[1];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] = (1.0 -
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2]) *
      ControlSystems_ONLYFRONT_B.Switch2[2];
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] = (1.0 -
      ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3]) *
      ControlSystems_ONLYFRONT_B.Switch2[3];
    break;

   default:
    /* MinMax: '<S14>/Min3' incorporates:
     *  Constant: '<S14>/Constant7'
     *  Sum: '<S14>/Sum5'
     */
    ripartizione_idx_2 = fmin(1.0 - ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2],
      1.0 - ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3]);

    /* MinMax: '<S14>/Min' incorporates:
     *  Constant: '<S14>/Constant7'
     *  Sum: '<S14>/Sum5'
     */
    rtb_YMCTorque_idx_1 = fmin(1.0 - ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0],
      1.0 - ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1]);
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] =
      ControlSystems_ONLYFRONT_B.Switch2[0] * rtb_YMCTorque_idx_1;
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] =
      ControlSystems_ONLYFRONT_B.Switch2[1] * rtb_YMCTorque_idx_1;
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] =
      ControlSystems_ONLYFRONT_B.Switch2[2] * ripartizione_idx_2;
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] =
      ControlSystems_ONLYFRONT_B.Switch2[3] * ripartizione_idx_2;
    break;
  }

  /* End of MultiPortSwitch: '<S14>/Multiport Switch' */

  /* Saturate: '<S14>/Physical motors limits' */
  if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] > 21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] = 21.0;
  } else if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] < -21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] = -21.0;
  }

  if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] > 21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] = 21.0;
  } else if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] < -21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] = -21.0;
  }

  if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] > 21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] = 21.0;
  } else if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] < -21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] = -21.0;
  }

  if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] > 21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] = 21.0;
  } else if (ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] < -21.0) {
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] = -21.0;
  }

  /* End of Saturate: '<S14>/Physical motors limits' */

  /* Sum: '<S12>/Somma delle 4 coppie richieste ai motori' */
  rtb_Gain19 = ((ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] +
                 ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1]) +
                ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2]) +
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3];

  /* Gain: '<S12>/Gain' incorporates:
   *  Abs: '<S12>/Abs'
   */
  rtb_RR = 0.99 * fabs(rtb_Gain19);

  /* Saturate: '<Root>/Saturation13' incorporates:
   *  Inport: '<Root>/battery_current'
   */
  if (ControlSystems_ONLYFRONT_U.battery_current > 300.0) {
    rtb_YMCTorque_idx_1 = 300.0;
  } else if (ControlSystems_ONLYFRONT_U.battery_current < -300.0) {
    rtb_YMCTorque_idx_1 = -300.0;
  } else {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_U.battery_current;
  }

  /* Abs: '<Root>/Abs2' incorporates:
   *  Saturate: '<Root>/Saturation13'
   */
  ripartizione_idx_2 = fabs(rtb_YMCTorque_idx_1);

  /* Saturate: '<Root>/Saturation17' incorporates:
   *  Inport: '<Root>/Vcell_min'
   */
  if (ControlSystems_ONLYFRONT_U.Vcell_min > 5.0) {
    frontMotorTorque = 5.0;
  } else if (ControlSystems_ONLYFRONT_U.Vcell_min < 2.0) {
    frontMotorTorque = 2.0;
  } else {
    frontMotorTorque = ControlSystems_ONLYFRONT_U.Vcell_min;
  }

  /* End of Saturate: '<Root>/Saturation17' */

  /* Saturate: '<Root>/Saturation25' */
  if (ripartizione_idx_2 <= 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = ripartizione_idx_2;
  }

  /* MATLAB Function: '<Root>/discharge limit' incorporates:
   *  Constant: '<Root>/Constant23'
   *  Delay: '<Root>/Delay10'
   *  Delay: '<Root>/Delay11'
   *  Saturate: '<Root>/Saturation25'
   */
  rtb_YMCTorque_idx_1 = 0.00175 * rtb_YMCTorque_idx_1 + frontMotorTorque;
  OCV_filt = (0.521897 * ControlSystems_ONLYFRONT_DW.Delay10_DSTATE + 0.23905 *
              rtb_YMCTorque_idx_1) + 0.23905 *
    ControlSystems_ONLYFRONT_DW.Delay11_DSTATE;
  u0 = fmax((fmax((OCV_filt - ControlSystems_ONLYFRONT_DW.Delay10_DSTATE) * 0.5
                  + OCV_filt, frontMotorTorque) -
             ControlSystems_ONLYFRONT_P.Vcell_lim) * 571.4286, 0.0);
  ControlSystems_ONLYFRONT_DW.Delay10_DSTATE = OCV_filt;
  ControlSystems_ONLYFRONT_DW.Delay11_DSTATE = rtb_YMCTorque_idx_1;
  rtb_YMCTorque_idx_1 = 132.0 * frontMotorTorque * u0;

  /* Saturate: '<Root>/Saturation24' */
  if (rtb_YMCTorque_idx_1 > ControlSystems_ONLYFRONT_P.Pe_lim) {
    rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_P.Pe_lim;
  } else if (rtb_YMCTorque_idx_1 < 1000.0) {
    rtb_YMCTorque_idx_1 = 1000.0;
  }

  /* End of Saturate: '<Root>/Saturation24' */

  /* Product: '<S12>/Product4' */
  rtb_battery_voltage *= ripartizione_idx_2;

  /* Gain: '<S85>/Gain1' incorporates:
   *  Sum: '<S85>/Subtract1'
   *  UnitDelay: '<S85>/Unit Delay'
   */
  ripartizione_idx_2 = (rtb_battery_voltage -
                        ControlSystems_ONLYFRONT_DW.UnitDelay_DSTATE) * 1000.0;

  /* MATLAB Function: '<S12>/MATLAB Function' */
  if ((ripartizione_idx_2 > 0.0) && (!(rtb_battery_voltage -
        (rtb_YMCTorque_idx_1 - 2000.0) < 0.0))) {
    /* Switch: '<S12>/Switch3' incorporates:
     *  Gain: '<S28>/Gain'
     *  Sum: '<S28>/Sum'
     */
    ripartizione_idx_2 = 0.001 * ripartizione_idx_2 + rtb_battery_voltage;
  } else {
    /* Switch: '<S12>/Switch3' */
    ripartizione_idx_2 = rtb_battery_voltage;
  }

  /* End of MATLAB Function: '<S12>/MATLAB Function' */

  /* Sum: '<S12>/Sum3' incorporates:
   *  Constant: '<S12>/Constant'
   *  MinMax: '<S12>/Min'
   */
  ripartizione_idx_2 -= fmin(ControlSystems_ONLYFRONT_P.Pe_lim - 1000.0,
    rtb_YMCTorque_idx_1);

  /* Switch: '<S26>/Switch' */
  idx = !(rtb_YMCTorque_idx_1 > ControlSystems_ONLYFRONT_P.Pe_lim - 2000.0);

  /* DiscreteIntegrator: '<S67>/Integrator' */
  if ((idx > 0) && (ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k1 <=
                    0)) {
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE_o = 0.0;
  }

  /* Sum: '<S76>/Sum' incorporates:
   *  DiscreteIntegrator: '<S67>/Integrator'
   *  Gain: '<S72>/Proportional Gain'
   */
  frontMotorTorque = 0.0018 * ripartizione_idx_2 +
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE_o;

  /* Saturate: '<S74>/Saturation' */
  if (frontMotorTorque > 30000.0) {
    rtb_YMCTorque_idx_1 = 30000.0;
  } else if (frontMotorTorque < 0.0) {
    rtb_YMCTorque_idx_1 = 0.0;
  } else {
    rtb_YMCTorque_idx_1 = frontMotorTorque;
  }

  /* Product: '<S12>/Divide3' incorporates:
   *  Constant: '<S12>/ constant'
   *  Saturate: '<S74>/Saturation'
   */
  rtb_YMCTorque_idx_1 /= 1000.0;

  /* Switch: '<S29>/Switch2' incorporates:
   *  RelationalOperator: '<S29>/LowerRelop1'
   *  Switch: '<S29>/Switch'
   */
  if (!(rtb_YMCTorque_idx_1 > rtb_RR)) {
    rtb_RR = rtb_YMCTorque_idx_1;
  }

  /* End of Switch: '<S29>/Switch2' */

  /* Switch: '<S12>/switch logico  ' incorporates:
   *  Constant: '<S12>/err'
   */
  if (!(rtb_Gain19 > 0.0)) {
    rtb_Gain19 = -1.0;
  }

  /* Product: '<S12>/Divide1' incorporates:
   *  Switch: '<S12>/switch logico  '
   */
  rtb_RR /= rtb_Gain19;

  /* Switch: '<S12>/Switch' incorporates:
   *  Constant: '<S12>/fattore di riduzione nullo '
   */
  if (!(rtb_RR > 0.0)) {
    rtb_RR = 0.0;
  }

  /* End of Switch: '<S12>/Switch' */

  /* Sum: '<S12>/Sum1' incorporates:
   *  Product: '<S12>/Product'
   */
  rtb_Gain19 = ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[0] * rtb_RR;

  /* Saturate: '<Root>/Saturation23' */
  if (rtb_Gain19 > 21.0) {
    rtb_Gain19 = 21.0;
  } else if (rtb_Gain19 < -21.0) {
    rtb_Gain19 = -21.0;
  }

  /* Sum: '<S12>/Sum2' incorporates:
   *  Product: '<S12>/Product1'
   */
  rtb_YMCTorque_idx_1 = ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[1] * rtb_RR;

  /* Saturate: '<Root>/Saturation23' */
  if (rtb_YMCTorque_idx_1 > 21.0) {
    rtb_YMCTorque_idx_1 = 21.0;
  } else if (rtb_YMCTorque_idx_1 < -21.0) {
    rtb_YMCTorque_idx_1 = -21.0;
  }

  /* Sum: '<S12>/Sum4' incorporates:
   *  Product: '<S12>/Product2'
   */
  u0 = ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[2] * rtb_RR;

  /* Sum: '<S12>/Sum5' incorporates:
   *  Product: '<S12>/Product3'
   */
  rtb_RR = ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] -
    ControlSystems_ONLYFRONT_Y.NO_ePL_torques[3] * rtb_RR;

  /* Switch: '<Root>/Switch4' */
  if (rtb_Gain19 > 0.0) {
    /* Outport: '<Root>/speed_setpoints' */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[0] = rtb_Saturation28;
  } else {
    /* Outport: '<Root>/speed_setpoints' incorporates:
     *  Constant: '<Root>/Constant26'
     */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[0] = 0.0;
  }

  if (rtb_YMCTorque_idx_1 > 0.0) {
    /* Outport: '<Root>/speed_setpoints' */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[1] = rtb_Saturation28;
  } else {
    /* Outport: '<Root>/speed_setpoints' incorporates:
     *  Constant: '<Root>/Constant26'
     */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[1] = 0.0;
  }

  /* Saturate: '<Root>/Saturation23' */
  if (u0 > 21.0) {
    u0 = 21.0;
  } else if (u0 < -21.0) {
    u0 = -21.0;
  }

  /* Switch: '<Root>/Switch4' incorporates:
   *  Saturate: '<Root>/Saturation23'
   */
  if (u0 > 0.0) {
    /* Outport: '<Root>/speed_setpoints' */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[2] = rtb_Saturation28;
  } else {
    /* Outport: '<Root>/speed_setpoints' incorporates:
     *  Constant: '<Root>/Constant26'
     */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[2] = 0.0;
  }

  /* Saturate: '<Root>/Saturation23' */
  if (rtb_RR > 21.0) {
    rtb_RR = 21.0;
  } else if (rtb_RR < -21.0) {
    rtb_RR = -21.0;
  }

  /* Switch: '<Root>/Switch4' incorporates:
   *  Saturate: '<Root>/Saturation23'
   */
  if (rtb_RR > 0.0) {
    /* Outport: '<Root>/speed_setpoints' */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[3] = rtb_Saturation28;
  } else {
    /* Outport: '<Root>/speed_setpoints' incorporates:
     *  Constant: '<Root>/Constant26'
     */
    ControlSystems_ONLYFRONT_Y.speed_setpoints[3] = 0.0;
  }

  /* Outport: '<Root>/torques' incorporates:
   *  Constant: '<Root>/Constant28'
   *  Constant: '<Root>/Constant29'
   */
  ControlSystems_ONLYFRONT_Y.torques[0] = rtb_Gain19;
  ControlSystems_ONLYFRONT_Y.torques[1] = rtb_YMCTorque_idx_1;
  ControlSystems_ONLYFRONT_Y.torques[2] = 0.0;
  ControlSystems_ONLYFRONT_Y.torques[3] = 0.0;

  /* Outport: '<Root>/delta_torque' incorporates:
   *  DataTypeConversion: '<Root>/Cast To Single1'
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  ControlSystems_ONLYFRONT_Y.delta_torque[0] = (float)rtb_Min2;
  ControlSystems_ONLYFRONT_Y.delta_torque[1] = (float)T_max_idx_1;
  ControlSystems_ONLYFRONT_Y.delta_torque[2] = (float)T_max_idx_2;
  ControlSystems_ONLYFRONT_Y.delta_torque[3] = (float)rtb_Min_g_idx_0;

  /* DeadZone: '<S59>/DeadZone' */
  if (frontMotorTorque > 30000.0) {
    frontMotorTorque -= 30000.0;
  } else if (frontMotorTorque >= 0.0) {
    frontMotorTorque = 0.0;
  }

  /* End of DeadZone: '<S59>/DeadZone' */

  /* Gain: '<S64>/Integral Gain' */
  ripartizione_idx_2 *= ControlSystems_ONLYFRONT_P.ki_pl * 0.001;

  /* DeadZone: '<S449>/DeadZone' */
  if (rtb_DeadZone_b > 1.0) {
    rtb_DeadZone_b--;
  } else if (rtb_DeadZone_b >= 0.0) {
    rtb_DeadZone_b = 0.0;
  }

  /* End of DeadZone: '<S449>/DeadZone' */

  /* Gain: '<S454>/Integral Gain' */
  rtb_Saturation28 = 0.0015 * rtb_Sliperror_idx_1;

  /* Switch: '<S447>/Switch1' incorporates:
   *  Constant: '<S447>/Clamping_zero'
   *  Constant: '<S447>/Constant'
   *  Constant: '<S447>/Constant2'
   *  RelationalOperator: '<S447>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_b > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S447>/Switch2' incorporates:
   *  Constant: '<S447>/Clamping_zero'
   *  Constant: '<S447>/Constant3'
   *  Constant: '<S447>/Constant4'
   *  RelationalOperator: '<S447>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S447>/Switch' incorporates:
   *  Constant: '<S447>/Clamping_zero'
   *  Constant: '<S447>/Constant1'
   *  Logic: '<S447>/AND3'
   *  RelationalOperator: '<S447>/Equal1'
   *  RelationalOperator: '<S447>/Relational Operator'
   *  Switch: '<S447>/Switch1'
   *  Switch: '<S447>/Switch2'
   */
  if ((rtb_DeadZone_b != 0.0) && (tmp == tmp_0)) {
    rtb_Min_g_idx_0 = 0.0;
  } else {
    rtb_Min_g_idx_0 = rtb_Saturation28;
  }

  /* End of Switch: '<S447>/Switch' */

  /* DeadZone: '<S341>/DeadZone' */
  if (rtb_DeadZone_i > 1.0) {
    rtb_DeadZone_i--;
  } else if (rtb_DeadZone_i >= 0.0) {
    rtb_DeadZone_i = 0.0;
  }

  /* End of DeadZone: '<S341>/DeadZone' */

  /* Gain: '<S346>/Integral Gain' */
  rtb_Saturation28 = 0.0015 * rtb_Sliperror_idx_0;

  /* Switch: '<S339>/Switch1' incorporates:
   *  Constant: '<S339>/Clamping_zero'
   *  Constant: '<S339>/Constant'
   *  Constant: '<S339>/Constant2'
   *  RelationalOperator: '<S339>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_i > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S339>/Switch2' incorporates:
   *  Constant: '<S339>/Clamping_zero'
   *  Constant: '<S339>/Constant3'
   *  Constant: '<S339>/Constant4'
   *  RelationalOperator: '<S339>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S339>/Switch' incorporates:
   *  Constant: '<S339>/Clamping_zero'
   *  Constant: '<S339>/Constant1'
   *  Logic: '<S339>/AND3'
   *  RelationalOperator: '<S339>/Equal1'
   *  RelationalOperator: '<S339>/Relational Operator'
   *  Switch: '<S339>/Switch1'
   *  Switch: '<S339>/Switch2'
   */
  if ((rtb_DeadZone_i != 0.0) && (tmp == tmp_0)) {
    rtb_Min2 = 0.0;
  } else {
    rtb_Min2 = rtb_Saturation28;
  }

  /* End of Switch: '<S339>/Switch' */

  /* DeadZone: '<S503>/DeadZone' */
  if (rtb_DeadZone_f > 1.0) {
    rtb_DeadZone_f--;
  } else if (rtb_DeadZone_f >= 0.0) {
    rtb_DeadZone_f = 0.0;
  }

  /* End of DeadZone: '<S503>/DeadZone' */

  /* Gain: '<S508>/Integral Gain' */
  rtb_Saturation28 = 0.0015 * s_im;

  /* Switch: '<S501>/Switch1' incorporates:
   *  Constant: '<S501>/Clamping_zero'
   *  Constant: '<S501>/Constant'
   *  Constant: '<S501>/Constant2'
   *  RelationalOperator: '<S501>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_f > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S501>/Switch2' incorporates:
   *  Constant: '<S501>/Clamping_zero'
   *  Constant: '<S501>/Constant3'
   *  Constant: '<S501>/Constant4'
   *  RelationalOperator: '<S501>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S501>/Switch' incorporates:
   *  Constant: '<S501>/Clamping_zero'
   *  Constant: '<S501>/Constant1'
   *  Logic: '<S501>/AND3'
   *  RelationalOperator: '<S501>/Equal1'
   *  RelationalOperator: '<S501>/Relational Operator'
   *  Switch: '<S501>/Switch1'
   *  Switch: '<S501>/Switch2'
   */
  if ((rtb_DeadZone_f != 0.0) && (tmp == tmp_0)) {
    rtb_DeadZone_f = 0.0;
  } else {
    rtb_DeadZone_f = rtb_Saturation28;
  }

  /* End of Switch: '<S501>/Switch' */

  /* DeadZone: '<S395>/DeadZone' */
  if (deltaK > 1.0) {
    deltaK--;
  } else if (deltaK >= 0.0) {
    deltaK = 0.0;
  }

  /* End of DeadZone: '<S395>/DeadZone' */

  /* Gain: '<S400>/Integral Gain' */
  rtb_Saturation28 = 0.0015 * ripartizione_idx_0;

  /* Switch: '<S393>/Switch1' incorporates:
   *  Constant: '<S393>/Clamping_zero'
   *  Constant: '<S393>/Constant'
   *  Constant: '<S393>/Constant2'
   *  RelationalOperator: '<S393>/fix for DT propagation issue'
   */
  if (deltaK > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S393>/Switch2' incorporates:
   *  Constant: '<S393>/Clamping_zero'
   *  Constant: '<S393>/Constant3'
   *  Constant: '<S393>/Constant4'
   *  RelationalOperator: '<S393>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S393>/Switch' incorporates:
   *  Constant: '<S393>/Clamping_zero'
   *  Constant: '<S393>/Constant1'
   *  Logic: '<S393>/AND3'
   *  RelationalOperator: '<S393>/Equal1'
   *  RelationalOperator: '<S393>/Relational Operator'
   *  Switch: '<S393>/Switch1'
   *  Switch: '<S393>/Switch2'
   */
  if ((deltaK != 0.0) && (tmp == tmp_0)) {
    ripartizione_idx_0 = 0.0;
  } else {
    ripartizione_idx_0 = rtb_Saturation28;
  }

  /* End of Switch: '<S393>/Switch' */

  /* DeadZone: '<S229>/DeadZone' */
  if (rtb_DeadZone_c > 1.0) {
    rtb_DeadZone_c--;
  } else if (rtb_DeadZone_c >= 0.0) {
    rtb_DeadZone_c = 0.0;
  }

  /* End of DeadZone: '<S229>/DeadZone' */

  /* Gain: '<S234>/Integral Gain' */
  rtb_Saturation28 = ControlSystems_ONLYFRONT_P.TC_CL_I_R * rtb_YMCTorque_idx_3;

  /* Switch: '<S227>/Switch1' incorporates:
   *  Constant: '<S227>/Clamping_zero'
   *  Constant: '<S227>/Constant'
   *  Constant: '<S227>/Constant2'
   *  RelationalOperator: '<S227>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_c > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S227>/Switch2' incorporates:
   *  Constant: '<S227>/Clamping_zero'
   *  Constant: '<S227>/Constant3'
   *  Constant: '<S227>/Constant4'
   *  RelationalOperator: '<S227>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S227>/Switch' incorporates:
   *  Constant: '<S227>/Clamping_zero'
   *  Constant: '<S227>/Constant1'
   *  Logic: '<S227>/AND3'
   *  RelationalOperator: '<S227>/Equal1'
   *  RelationalOperator: '<S227>/Relational Operator'
   *  Switch: '<S227>/Switch1'
   *  Switch: '<S227>/Switch2'
   */
  if ((rtb_DeadZone_c != 0.0) && (tmp == tmp_0)) {
    T_max_idx_1 = 0.0;
  } else {
    T_max_idx_1 = rtb_Saturation28;
  }

  /* End of Switch: '<S227>/Switch' */

  /* DeadZone: '<S121>/DeadZone' */
  if (T_max_idx_3 > 1.0) {
    T_max_idx_3--;
  } else if (T_max_idx_3 >= 0.0) {
    T_max_idx_3 = 0.0;
  }

  /* End of DeadZone: '<S121>/DeadZone' */

  /* Gain: '<S126>/Integral Gain' */
  rtb_Saturation28 = ControlSystems_ONLYFRONT_P.TC_CL_I_R * rtb_Saturation22;

  /* Switch: '<S119>/Switch1' incorporates:
   *  Constant: '<S119>/Clamping_zero'
   *  Constant: '<S119>/Constant'
   *  Constant: '<S119>/Constant2'
   *  RelationalOperator: '<S119>/fix for DT propagation issue'
   */
  if (T_max_idx_3 > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S119>/Switch2' incorporates:
   *  Constant: '<S119>/Clamping_zero'
   *  Constant: '<S119>/Constant3'
   *  Constant: '<S119>/Constant4'
   *  RelationalOperator: '<S119>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S119>/Switch' incorporates:
   *  Constant: '<S119>/Clamping_zero'
   *  Constant: '<S119>/Constant1'
   *  Logic: '<S119>/AND3'
   *  RelationalOperator: '<S119>/Equal1'
   *  RelationalOperator: '<S119>/Relational Operator'
   *  Switch: '<S119>/Switch1'
   *  Switch: '<S119>/Switch2'
   */
  if ((T_max_idx_3 != 0.0) && (tmp == tmp_0)) {
    rtb_Saturation22 = 0.0;
  } else {
    rtb_Saturation22 = rtb_Saturation28;
  }

  /* End of Switch: '<S119>/Switch' */

  /* DeadZone: '<S283>/DeadZone' */
  if (rtb_DeadZone_do > 1.0) {
    rtb_DeadZone_do--;
  } else if (rtb_DeadZone_do >= 0.0) {
    rtb_DeadZone_do = 0.0;
  }

  /* End of DeadZone: '<S283>/DeadZone' */

  /* Gain: '<S288>/Integral Gain' */
  rtb_Saturation28 = ControlSystems_ONLYFRONT_P.TC_CL_I_F * P_idx_1;

  /* Switch: '<S281>/Switch1' incorporates:
   *  Constant: '<S281>/Clamping_zero'
   *  Constant: '<S281>/Constant'
   *  Constant: '<S281>/Constant2'
   *  RelationalOperator: '<S281>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_do > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S281>/Switch2' incorporates:
   *  Constant: '<S281>/Clamping_zero'
   *  Constant: '<S281>/Constant3'
   *  Constant: '<S281>/Constant4'
   *  RelationalOperator: '<S281>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S281>/Switch' incorporates:
   *  Constant: '<S281>/Clamping_zero'
   *  Constant: '<S281>/Constant1'
   *  Logic: '<S281>/AND3'
   *  RelationalOperator: '<S281>/Equal1'
   *  RelationalOperator: '<S281>/Relational Operator'
   *  Switch: '<S281>/Switch1'
   *  Switch: '<S281>/Switch2'
   */
  if ((rtb_DeadZone_do != 0.0) && (tmp == tmp_0)) {
    P_idx_1 = 0.0;
  } else {
    P_idx_1 = rtb_Saturation28;
  }

  /* End of Switch: '<S281>/Switch' */

  /* DeadZone: '<S175>/DeadZone' */
  if (rtb_DeadZone_p > 1.0) {
    rtb_DeadZone_p--;
  } else if (rtb_DeadZone_p >= 0.0) {
    rtb_DeadZone_p = 0.0;
  }

  /* End of DeadZone: '<S175>/DeadZone' */

  /* Gain: '<S180>/Integral Gain' */
  rtb_Saturation28 = ControlSystems_ONLYFRONT_P.TC_CL_I_F * T_max_idx_0;

  /* Switch: '<S173>/Switch1' incorporates:
   *  Constant: '<S173>/Clamping_zero'
   *  Constant: '<S173>/Constant'
   *  Constant: '<S173>/Constant2'
   *  RelationalOperator: '<S173>/fix for DT propagation issue'
   */
  if (rtb_DeadZone_p > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S173>/Switch2' incorporates:
   *  Constant: '<S173>/Clamping_zero'
   *  Constant: '<S173>/Constant3'
   *  Constant: '<S173>/Constant4'
   *  RelationalOperator: '<S173>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S173>/Switch' incorporates:
   *  Constant: '<S173>/Clamping_zero'
   *  Constant: '<S173>/Constant1'
   *  Logic: '<S173>/AND3'
   *  RelationalOperator: '<S173>/Equal1'
   *  RelationalOperator: '<S173>/Relational Operator'
   *  Switch: '<S173>/Switch1'
   *  Switch: '<S173>/Switch2'
   */
  if ((rtb_DeadZone_p != 0.0) && (tmp == tmp_0)) {
    T_max_idx_0 = 0.0;
  } else {
    T_max_idx_0 = rtb_Saturation28;
  }

  /* End of Switch: '<S173>/Switch' */

  /* Outport: '<Root>/yaw_moment' incorporates:
   *  DataTypeConversion: '<Root>/Cast To Single2'
   */
  ControlSystems_ONLYFRONT_Y.yaw_moment = (float)ControlSystems_ONLYFRONT_B.YMC;

  /* Switch: '<S563>/Switch' incorporates:
   *  RelationalOperator: '<S563>/u_GTE_up'
   */
  if (!(rtb_Integrator_gf >= rtb_YMC_lim)) {
    /* Switch: '<S563>/Switch1' incorporates:
     *  Gain: '<S17>/Gain'
     *  RelationalOperator: '<S563>/u_GT_lo'
     */
    if (rtb_Integrator_gf > -rtb_YMC_lim) {
      rtb_YMC_lim = rtb_Integrator_gf;
    } else {
      rtb_YMC_lim = -rtb_YMC_lim;
    }

    /* End of Switch: '<S563>/Switch1' */
  }

  /* End of Switch: '<S563>/Switch' */

  /* Sum: '<S563>/Diff' */
  rtb_YMC_lim = rtb_Integrator_gf - rtb_YMC_lim;

  /* Product: '<S568>/IProd Out' incorporates:
   *  Product: '<S17>/Product'
   */
  rtb_Saturation28 = ControlSystems_ONLYFRONT_B.Saturation27 *
    ControlSystems_ONLYFRONT_B.RateLimiter1 * ControlSystems_ONLYFRONT_B.yr_e;

  /* Outport: '<Root>/yr_ref' incorporates:
   *  DataTypeConversion: '<Root>/Cast To Single3'
   */
  ControlSystems_ONLYFRONT_Y.yr_ref = (float)ControlSystems_ONLYFRONT_B.Product2;

  /* Switch: '<S6>/Switch2' incorporates:
   *  Switch: '<S6>/Switch4'
   */
  if (rtb_Regen_active > 0.0) {
    /* Switch: '<S19>/Switch1' incorporates:
     *  Constant: '<S19>/Constant7'
     */
    if (!(ControlSystems_ONLYFRONT_P.REGEN_TYPE > 0.0)) {
      /* Gain: '<S19>/Gain3' incorporates:
       *  Abs: '<S19>/Abs3'
       *  Gain: '<S19>/Gain6'
       *  MATLAB Function: '<S19>/MATLAB Function'
       *  Outport: '<Root>/Regen power'
       */
      ControlSystems_ONLYFRONT_Y.Regenpower = -fabs(((float)((rtb_Gain_g_idx_2 *
        rtb_Gain_g_idx_2 + rtb_Gain_g_idx_1 * rtb_Gain_g_idx_1) * 0.30375 +
        rtb_Integrator_jj) + 0.0083F * rpm) + ((rtb_Gain1 + rtb_Min_g_idx_2) +
        t_p_lim));
    }

    /* End of Switch: '<S19>/Switch1' */
  } else {
    /* Gain: '<S19>/Gain3' incorporates:
     *  Constant: '<S6>/Constant6'
     *  Outport: '<Root>/Regen power'
     */
    ControlSystems_ONLYFRONT_Y.Regenpower = 0.0;

    /* Outport: '<Root>/Pemax' incorporates:
     *  Constant: '<S6>/Constant5'
     */
    ControlSystems_ONLYFRONT_Y.Pemax = 0.0;
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Outport: '<Root>/vx' incorporates:
   *  DataTypeConversion: '<Root>/Cast To Single'
   */
  ControlSystems_ONLYFRONT_Y.vx = (float)rtb_estimatedspeedmotors;

  /* Update for Delay: '<Root>/Delay4' */
  ControlSystems_ONLYFRONT_DW.Delay4_DSTATE = rtb_acc_y;

  /* Update for Delay: '<Root>/Delay' */
  ControlSystems_ONLYFRONT_DW.Delay_DSTATE = rtb_acc_x;

  /* Update for Delay: '<Root>/Delay8' */
  ControlSystems_ONLYFRONT_DW.Delay8_DSTATE = rtb_Min_g_idx_1;

  /* Update for Delay: '<Root>/Delay9' */
  ControlSystems_ONLYFRONT_DW.Delay9_DSTATE =
    ControlSystems_ONLYFRONT_B.steering_rad;

  /* Update for Delay: '<Root>/Delay6' */
  ControlSystems_ONLYFRONT_DW.Delay6_DSTATE = rtb_yaw_rate;

  /* Update for Delay: '<Root>/Delay7' */
  ControlSystems_ONLYFRONT_DW.Delay7_DSTATE = ControlSystems_ONLYFRONT_B.y;

  /* Switch: '<S560>/Switch1' incorporates:
   *  Constant: '<S560>/Clamping_zero'
   *  Constant: '<S560>/Constant'
   *  Constant: '<S560>/Constant2'
   *  RelationalOperator: '<S560>/fix for DT propagation issue'
   */
  if (rtb_YMC_lim > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S560>/Switch2' incorporates:
   *  Constant: '<S560>/Clamping_zero'
   *  Constant: '<S560>/Constant3'
   *  Constant: '<S560>/Constant4'
   *  RelationalOperator: '<S560>/fix for DT propagation issue1'
   */
  if (rtb_Saturation28 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S560>/Switch' incorporates:
   *  Constant: '<S560>/Clamping_zero'
   *  Constant: '<S560>/Constant1'
   *  Logic: '<S560>/AND3'
   *  RelationalOperator: '<S560>/Equal1'
   *  RelationalOperator: '<S560>/Relational Operator'
   *  Switch: '<S560>/Switch1'
   *  Switch: '<S560>/Switch2'
   */
  if ((rtb_YMC_lim != 0.0) && (tmp == tmp_0)) {
    rtb_Saturation28 = 0.0;
  }

  /* Update for DiscreteIntegrator: '<S571>/Integrator' incorporates:
   *  Switch: '<S560>/Switch'
   */
  ControlSystems_ONLYFRONT_DW.Integrator_DSTATE += rtb_Saturation28;
  if (ControlSystems_ONLYFRONT_DW.Integrator_DSTATE > 3181.0876476377953) {
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE = 3181.0876476377953;
  } else if (ControlSystems_ONLYFRONT_DW.Integrator_DSTATE < -3181.0876476377953)
  {
    ControlSystems_ONLYFRONT_DW.Integrator_DSTATE = -3181.0876476377953;
  }

  if (ControlSystems_ONLYFRONT_B.ZeroOrderHold > 0.0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState = 1;
  } else if (ControlSystems_ONLYFRONT_B.ZeroOrderHold < 0.0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState = -1;
  } else if (ControlSystems_ONLYFRONT_B.ZeroOrderHold == 0.0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState = 0;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState = 2;
  }

  /* End of Update for DiscreteIntegrator: '<S571>/Integrator' */

  /* Update for DiscreteIntegrator: '<S183>/Integrator' */
  ControlSystems_ONLYFRONT_DW.u += T_max_idx_0;
  if (ControlSystems_ONLYFRONT_DW.u > 1.0) {
    ControlSystems_ONLYFRONT_DW.u = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u < 0.0) {
    ControlSystems_ONLYFRONT_DW.u = 0.0;
  }

  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_p = (int8_t)
    rtb_RelationalOperator1_idx_0;

  /* End of Update for DiscreteIntegrator: '<S183>/Integrator' */

  /* Update for DiscreteIntegrator: '<S291>/Integrator' */
  ControlSystems_ONLYFRONT_DW.u_o += P_idx_1;
  if (ControlSystems_ONLYFRONT_DW.u_o > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_o = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_o < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_o = 0.0;
  }

  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_ph = (int8_t)
    rtb_RelationalOperator1_idx_1;

  /* End of Update for DiscreteIntegrator: '<S291>/Integrator' */

  /* Update for DiscreteIntegrator: '<S129>/Integrator' */
  ControlSystems_ONLYFRONT_DW.u_l += rtb_Saturation22;
  if (ControlSystems_ONLYFRONT_DW.u_l > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_l = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_l < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_l = 0.0;
  }

  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_e = (int8_t)
    rtb_RelationalOperator1_idx_2;

  /* End of Update for DiscreteIntegrator: '<S129>/Integrator' */

  /* Update for DiscreteIntegrator: '<S237>/Integrator' incorporates:
   *  RelationalOperator: '<S14>/Relational Operator1'
   */
  ControlSystems_ONLYFRONT_DW.u_c += T_max_idx_1;
  if (ControlSystems_ONLYFRONT_DW.u_c > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_c = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_c < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_c = 0.0;
  }

  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_n = (int8_t)
    rtb_RelationalOperator1_0;

  /* End of Update for DiscreteIntegrator: '<S237>/Integrator' */

  /* Update for DiscreteIntegrator: '<S403>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  ControlSystems_ONLYFRONT_DW.u_h += ripartizione_idx_0;
  if (ControlSystems_ONLYFRONT_DW.u_h > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_h = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_h < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_h = 0.0;
  }

  if (1 - rtb_RelationalOperator1_idx_0 > 0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k = 1;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k = 0;
  }

  /* End of Update for DiscreteIntegrator: '<S403>/Integrator' */

  /* Update for DiscreteIntegrator: '<S511>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  ControlSystems_ONLYFRONT_DW.u_k += rtb_DeadZone_f;
  if (ControlSystems_ONLYFRONT_DW.u_k > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_k = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_k < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_k = 0.0;
  }

  if (1 - rtb_RelationalOperator1_idx_1 > 0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_m = 1;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_m = 0;
  }

  /* End of Update for DiscreteIntegrator: '<S511>/Integrator' */

  /* Update for DiscreteIntegrator: '<S349>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Sum: '<S14>/Subtract1'
   */
  ControlSystems_ONLYFRONT_DW.u_a += 0.001 * rtb_Min2;
  if (ControlSystems_ONLYFRONT_DW.u_a > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_a = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_a < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_a = 0.0;
  }

  if (1 - rtb_RelationalOperator1_idx_2 > 0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_eh = 1;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_eh = 0;
  }

  /* End of Update for DiscreteIntegrator: '<S349>/Integrator' */

  /* Update for DiscreteIntegrator: '<S457>/Integrator' incorporates:
   *  Constant: '<S14>/Constant5'
   *  RelationalOperator: '<S14>/Relational Operator1'
   *  Sum: '<S14>/Subtract1'
   */
  ControlSystems_ONLYFRONT_DW.u_e += rtb_Min_g_idx_0;
  if (ControlSystems_ONLYFRONT_DW.u_e > 1.0) {
    ControlSystems_ONLYFRONT_DW.u_e = 1.0;
  } else if (ControlSystems_ONLYFRONT_DW.u_e < 0.0) {
    ControlSystems_ONLYFRONT_DW.u_e = 0.0;
  }

  if (1 - rtb_RelationalOperator1_0 > 0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_en = 1;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_en = 0;
  }

  /* End of Update for DiscreteIntegrator: '<S457>/Integrator' */

  /* Update for UnitDelay: '<S85>/Unit Delay' */
  ControlSystems_ONLYFRONT_DW.UnitDelay_DSTATE = rtb_battery_voltage;

  /* Switch: '<S57>/Switch1' incorporates:
   *  Constant: '<S57>/Clamping_zero'
   *  Constant: '<S57>/Constant'
   *  Constant: '<S57>/Constant2'
   *  RelationalOperator: '<S57>/fix for DT propagation issue'
   */
  if (frontMotorTorque > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S57>/Switch2' incorporates:
   *  Constant: '<S57>/Clamping_zero'
   *  Constant: '<S57>/Constant3'
   *  Constant: '<S57>/Constant4'
   *  RelationalOperator: '<S57>/fix for DT propagation issue1'
   */
  if (ripartizione_idx_2 > 0.0) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S57>/Switch' incorporates:
   *  Constant: '<S57>/Clamping_zero'
   *  Constant: '<S57>/Constant1'
   *  Logic: '<S57>/AND3'
   *  RelationalOperator: '<S57>/Equal1'
   *  RelationalOperator: '<S57>/Relational Operator'
   *  Switch: '<S57>/Switch1'
   *  Switch: '<S57>/Switch2'
   */
  if ((frontMotorTorque != 0.0) && (tmp == tmp_0)) {
    ripartizione_idx_2 = 0.0;
  }

  /* Update for DiscreteIntegrator: '<S67>/Integrator' incorporates:
   *  Switch: '<S57>/Switch'
   */
  ControlSystems_ONLYFRONT_DW.Integrator_DSTATE_o += ripartizione_idx_2;
  if (idx > 0) {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k1 = 1;
  } else {
    ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k1 = 0;
  }

  /* End of Update for DiscreteIntegrator: '<S67>/Integrator' */
  rate_scheduler();
}

/* Model initialize function */
void ControlSystems_ONLYFRONT_initialize(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S571>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S183>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_p = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S291>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_ph = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S129>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_e = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S237>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_n = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S403>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S511>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_m = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S349>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_eh = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S457>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_en = 2;

  /* InitializeConditions for DiscreteIntegrator: '<S67>/Integrator' */
  ControlSystems_ONLYFRONT_DW.Integrator_PrevResetState_k1 = 2;

  /* Start for MATLABSystem: '<Root>/Moving Average' */
  ControlSystems_ONLYFRONT_DW.obj.isInitialized = 0;
  ControlSystems_ONLYFRONT_DW.obj.NumChannels = -1;
  ControlSystems_ONLYFRONT_DW.obj.FrameLength = -1;
  ControlSystems_ONLYFRONT_DW.obj.matlabCodegenIsDeleted = false;
  ControlSystems_ONLYFRONT_DW.obj.ForgettingFactor = 0.98;
  ControlSystems_ONLYFRONT_SystemCore_setup(&ControlSystems_ONLYFRONT_DW.obj);

  /* InitializeConditions for MATLABSystem: '<Root>/Moving Average' */
  ControlSystems_ONLYFRONT_DW.obj.pwN = 0.0;
  ControlSystems_ONLYFRONT_DW.obj.pmN = 0.0;
}

/* Model terminate function */
void ControlSystems_ONLYFRONT_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/Moving Average' */
  if (!ControlSystems_ONLYFRONT_DW.obj.matlabCodegenIsDeleted) {
    ControlSystems_ONLYFRONT_DW.obj.matlabCodegenIsDeleted = true;
    if ((ControlSystems_ONLYFRONT_DW.obj.isInitialized == 1) &&
        ControlSystems_ONLYFRONT_DW.obj.isSetupComplete) {
      ControlSystems_ONLYFRONT_DW.obj.NumChannels = -1;
      ControlSystems_ONLYFRONT_DW.obj.FrameLength = -1;
    }
  }

  /* End of Terminate for MATLABSystem: '<Root>/Moving Average' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
