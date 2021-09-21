/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * peak.cpp
 *
 * Code generation for function 'peak'
 *
 */

/* Include files */
#include "peak.h"
#include "eml_setop.h"
#include "peak_data.h"
#include "peak_emxutil.h"
#include "peak_initialize.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void peak(const emxArray_real_T *input, emxArray_real_T *output)
{
  emxArray_int32_T *idx;
  emxArray_int32_T *iFinite;
  emxArray_int32_T *iInfinite;
  int i;
  int ny;
  int nPk;
  int nInf;
  char dir;
  int kfirst;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  emxArray_int32_T *iPk;
  double yk;
  boolean_T isinfyk;
  char previousdir;
  emxArray_int32_T *c;
  emxArray_int32_T *y;
  double base;
  if (isInitialized_peak == false) {
    peak_initialize();
  }

  emxInit_int32_T(&idx, 1);
  emxInit_int32_T(&iFinite, 1);
  emxInit_int32_T(&iInfinite, 1);
  i = iFinite->size[0];
  iFinite->size[0] = input->size[1];
  emxEnsureCapacity_int32_T(iFinite, i);
  i = iInfinite->size[0];
  iInfinite->size[0] = input->size[1];
  emxEnsureCapacity_int32_T(iInfinite, i);
  ny = input->size[1];
  nPk = 0;
  nInf = 0;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 1; k <= ny; k++) {
    yk = input->data[k - 1];
    if (rtIsNaN(yk)) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(yk) && (yk > 0.0)) {
      isinfyk = true;
      nInf++;
      iInfinite->data[nInf - 1] = k;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
      } else if (yk < ykfirst) {
        dir = 'd';
        if (('d' != previousdir) && (previousdir == 'i')) {
          nPk++;
          iFinite->data[nPk - 1] = kfirst;
        }
      } else {
        dir = 'i';
      }

      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }

  emxInit_int32_T(&iPk, 1);
  if (1 > nPk) {
    i = 0;
  } else {
    i = nPk;
  }

  ny = iFinite->size[0];
  if (1 > nPk) {
    iFinite->size[0] = 0;
  } else {
    iFinite->size[0] = nPk;
  }

  emxEnsureCapacity_int32_T(iFinite, ny);
  ny = iInfinite->size[0];
  if (1 > nInf) {
    iInfinite->size[0] = 0;
  } else {
    iInfinite->size[0] = nInf;
  }

  emxEnsureCapacity_int32_T(iInfinite, ny);
  ny = iPk->size[0];
  iPk->size[0] = i;
  emxEnsureCapacity_int32_T(iPk, ny);
  nPk = 0;
  for (k = 0; k < i; k++) {
    ykfirst = input->data[iFinite->data[k] - 1];
    if (ykfirst > rtMinusInf) {
      base = input->data[iFinite->data[k]];
      yk = input->data[iFinite->data[k] - 2];
      if ((yk > base) || rtIsNaN(base)) {
        base = yk;
      }

      if (ykfirst - base >= 0.0) {
        nPk++;
        iPk->data[nPk - 1] = iFinite->data[k];
      }
    }
  }

  emxInit_int32_T(&c, 1);
  emxInit_int32_T(&y, 2);
  i = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity_int32_T(iPk, i);
  do_vectors(iPk, iInfinite, c, idx, iFinite);
  if (c->size[0] < 1) {
    kfirst = 0;
  } else {
    kfirst = c->size[0];
  }

  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = kfirst;
  emxEnsureCapacity_int32_T(y, i);
  emxFree_int32_T(&iInfinite);
  emxFree_int32_T(&iFinite);
  if (kfirst > 0) {
    y->data[0] = 1;
    ny = 1;
    for (k = 2; k <= kfirst; k++) {
      ny++;
      y->data[k - 1] = ny;
    }
  }

  i = idx->size[0];
  idx->size[0] = y->size[1];
  emxEnsureCapacity_int32_T(idx, i);
  ny = y->size[1];
  for (i = 0; i < ny; i++) {
    idx->data[i] = y->data[i];
  }

  emxFree_int32_T(&y);
  if (idx->size[0] > input->size[1]) {
    i = idx->size[0];
    if (1 > input->size[1]) {
      idx->size[0] = 0;
    } else {
      idx->size[0] = input->size[1];
    }

    emxEnsureCapacity_int32_T(idx, i);
  }

  i = iPk->size[0];
  iPk->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(iPk, i);
  ny = idx->size[0];
  for (i = 0; i < ny; i++) {
    iPk->data[i] = c->data[idx->data[i] - 1];
  }

  emxFree_int32_T(&c);
  emxFree_int32_T(&idx);
  i = output->size[0] * output->size[1];
  output->size[0] = 1;
  output->size[1] = iPk->size[0];
  emxEnsureCapacity_real_T(output, i);
  ny = iPk->size[0];
  for (i = 0; i < ny; i++) {
    output->data[i] = input->data[iPk->data[i] - 1];
  }

  emxFree_int32_T(&iPk);
}

/* End of code generation (peak.cpp) */
