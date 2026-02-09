/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: rt_nonfinite.c
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

#include <stdbool.h>
#include "rt_nonfinite.h"
#include "math.h"

double rtNaN = -(double)NAN;
double rtInf = (double)INFINITY;
double rtMinusInf = -(double)INFINITY;
float rtNaNF = -(float)NAN;
float rtInfF = (float)INFINITY;
float rtMinusInfF = -(float)INFINITY;

/* Test if value is infinite */
bool rtIsInf(double value)
{
  return (bool)isinf(value);
}

/* Test if single-precision value is infinite */
bool rtIsInfF(float value)
{
  return (bool)isinf(value);
}

/* Test if value is not a number */
bool rtIsNaN(double value)
{
  return (bool)(isnan(value) != 0);
}

/* Test if single-precision value is not a number */
bool rtIsNaNF(float value)
{
  return (bool)(isnan(value) != 0);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
