/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * eml_setop.cpp
 *
 * Code generation for function 'eml_setop'
 *
 */

/* Include files */
#include "eml_setop.h"
#include "peak.h"
#include "peak_emxutil.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
                emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib)
{
  int na;
  int nb;
  int ncmax;
  int iafirst;
  int nc;
  int nia;
  int nib;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  int ak;
  int b_iblast;
  int bk;
  na = a->size[0];
  nb = b->size[0];
  ncmax = a->size[0] + b->size[0];
  iafirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity_int32_T(c, iafirst);
  iafirst = ia->size[0];
  ia->size[0] = a->size[0];
  emxEnsureCapacity_int32_T(ia, iafirst);
  iafirst = ib->size[0];
  ib->size[0] = b->size[0];
  emxEnsureCapacity_int32_T(ib, iafirst);
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= na) && (iblast <= nb)) {
    b_ialast = ialast;
    ak = a->data[ialast - 1];
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast;
    b_iblast = iblast;
    bk = b->data[iblast - 1];
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c->data[nc] = ak;
      nia++;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      nc++;
      nia++;
      c->data[nc] = ak;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
    } else {
      nc++;
      nib++;
      c->data[nc] = bk;
      ib->data[nib - 1] = ibfirst + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }

  while (ialast <= na) {
    b_ialast = ialast;
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == a->data[ialast - 1]))
    {
      b_ialast++;
    }

    nc++;
    nia++;
    c->data[nc] = a->data[ialast - 1];
    ia->data[nia] = ialast;
    ialast = b_ialast + 1;
  }

  while (iblast <= nb) {
    b_iblast = iblast;
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == b->data[iblast - 1]))
    {
      b_iblast++;
    }

    nc++;
    nib++;
    c->data[nc] = b->data[iblast - 1];
    ib->data[nib - 1] = iblast;
    iblast = b_iblast + 1;
  }

  if (a->size[0] > 0) {
    iafirst = ia->size[0];
    if (1 > nia + 1) {
      nia = -1;
    }

    ia->size[0] = nia + 1;
    emxEnsureCapacity_int32_T(ia, iafirst);
  }

  if (b->size[0] > 0) {
    iafirst = ib->size[0];
    if (1 > nib) {
      ib->size[0] = 0;
    } else {
      ib->size[0] = nib;
    }

    emxEnsureCapacity_int32_T(ib, iafirst);
  }

  if (ncmax > 0) {
    iafirst = c->size[0];
    if (1 > nc + 1) {
      nc = -1;
    }

    c->size[0] = nc + 1;
    emxEnsureCapacity_int32_T(c, iafirst);
  }
}

/* End of code generation (eml_setop.cpp) */
