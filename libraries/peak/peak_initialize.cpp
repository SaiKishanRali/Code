/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * peak_initialize.cpp
 *
 * Code generation for function 'peak_initialize'
 *
 */

/* Include files */
#include "peak_initialize.h"
#include "peak.h"
#include "peak_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void peak_initialize()
{
  rt_InitInfAndNaN();
  isInitialized_peak = true;
}

/* End of code generation (peak_initialize.cpp) */
