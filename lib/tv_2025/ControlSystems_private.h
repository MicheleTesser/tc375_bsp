/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ControlSystems_ONLYFRONT_private.h
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

#ifndef ControlSystems_ONLYFRONT_private_h_
#define ControlSystems_ONLYFRONT_private_h_
#include <stdbool.h>
#include <stdint.h>
#include "complex_types.h"
#include "ControlSystems_types.h"
#include "ControlSystems.h"

extern double rt_powd_snf(double u0, double u1);
extern double rt_hypotd_snf(double u0, double u1);
extern double rt_atan2d_snf(double u0, double u1);
extern double look1_binlxpw(double u0, const double bp0[], const double table[],
  uint32_t maxIndex);
extern void ControlSystems_ONLYFRONT_Discretelowpassfilter(double rtu_x, double
  rtu_x_prec, double rtu_omega_0_inv, double rtu_y_prec, double *rty_y);

#endif                                 /* ControlSystems_ONLYFRONT_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
