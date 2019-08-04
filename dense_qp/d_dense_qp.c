/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High-Performance Interior Point Method.                                                *
* Copyright (C) 2019 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* The 2-Clause BSD License                                                                        *
*                                                                                                 *
* Redistribution and use in source and binary forms, with or without                              *
* modification, are permitted provided that the following conditions are met:                     *
*                                                                                                 *
* 1. Redistributions of source code must retain the above copyright notice, this                  *
*    list of conditions and the following disclaimer.                                             *
* 2. Redistributions in binary form must reproduce the above copyright notice,                    *
*    this list of conditions and the following disclaimer in the documentation                    *
*    and/or other materials provided with the distribution.                                       *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR                 *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                  *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/



#include <stdlib.h>
#include <stdio.h>

#include <blasfeo_target.h>
#include <blasfeo_common.h>
#include <blasfeo_d_aux.h>

#include <hpipm_d_dense_qp_dim.h>
#include <hpipm_d_dense_qp.h>


#define CREATE_STRMAT blasfeo_create_dmat
#define CREATE_STRVEC blasfeo_create_dvec
#define CVT_MAT2STRMAT blasfeo_pack_dmat
#define CVT_TRAN_MAT2STRMAT blasfeo_pack_tran_dmat
#define CVT_TRAN_STRMAT2MAT blasfeo_unpack_tran_dmat
#define CVT_VEC2STRVEC blasfeo_pack_dvec
#define CVT_STRMAT2MAT blasfeo_unpack_dmat
#define CVT_STRVEC2VEC blasfeo_unpack_dvec
#define DENSE_QP d_dense_qp
#define DENSE_QP_DIM d_dense_qp_dim
#define GECP_LIBSTR blasfeo_dgecp
#define GETR_LIBSTR blasfeo_dgetr
#define REAL double
#define ROWIN_LIBSTR blasfeo_drowin
#define SIZE_STRMAT blasfeo_memsize_dmat
#define SIZE_STRVEC blasfeo_memsize_dvec
#define STRMAT blasfeo_dmat
#define STRVEC blasfeo_dvec
#define VECCP_LIBSTR blasfeo_dveccp
#define VECSC_LIBSTR blasfeo_dvecsc
#define VECSE_LIBSTR blasfeo_dvecse

#define MEMSIZE_DENSE_QP d_memsize_dense_qp
#define CREATE_DENSE_QP d_create_dense_qp
#define CVT_COLMAJ_TO_DENSE_QP d_cvt_colmaj_to_dense_qp
#define CVT_DENSE_QP_TO_COLMAJ d_cvt_dense_qp_to_colmaj
#define CVT_ROWMAJ_TO_DENSE_QP d_cvt_rowmaj_to_dense_qp
#define CVT_DENSE_QP_TO_ROWMAJ d_cvt_dense_qp_to_rowmaj
#define CVT_LIBSTR_TO_DENSE_QP d_cvt_libstr_to_dense_qp
#define CVT_DENSE_QP_TO_LIBSTR d_cvt_dense_qp_to_libstr
#define CAST_DENSE_QP_DIM d_cast_dense_qp_dim

#define DENSE_QP_SET_H d_dense_qp_set_H
#define DENSE_QP_SET_G d_dense_qp_set_g
#define DENSE_QP_SET_A d_dense_qp_set_A
#define DENSE_QP_SET_B d_dense_qp_set_b
#define DENSE_QP_SET_IDXB d_dense_qp_set_idxb
#define DENSE_QP_SET_LB d_dense_qp_set_lb
#define DENSE_QP_SET_UB d_dense_qp_set_ub
#define DENSE_QP_SET_C d_dense_qp_set_C
#define DENSE_QP_SET_LG d_dense_qp_set_lg
#define DENSE_QP_SET_UG d_dense_qp_set_ug
#define DENSE_QP_SET_IDXS d_dense_qp_set_idxs
#define DENSE_QP_SET_ZZL d_dense_qp_set_Zl
#define DENSE_QP_SET_ZZU d_dense_qp_set_Zu
#define DENSE_QP_SET_ZL d_dense_qp_set_zl
#define DENSE_QP_SET_ZU d_dense_qp_set_zu
#define DENSE_QP_SET_LS d_dense_qp_set_ls
#define DENSE_QP_SET_US d_dense_qp_set_us

#define DENSE_QP_GET_H d_dense_qp_get_H
#define DENSE_QP_GET_G d_dense_qp_get_g
#define DENSE_QP_GET_A d_dense_qp_get_A
#define DENSE_QP_GET_B d_dense_qp_get_b
#define DENSE_QP_GET_IDXB d_dense_qp_get_idxb
#define DENSE_QP_GET_LB d_dense_qp_get_lb
#define DENSE_QP_GET_UB d_dense_qp_get_ub
#define DENSE_QP_GET_C d_dense_qp_get_C
#define DENSE_QP_GET_LG d_dense_qp_get_lg
#define DENSE_QP_GET_UG d_dense_qp_get_ug
#define DENSE_QP_GET_IDXS d_dense_qp_get_idxs
#define DENSE_QP_GET_ZZL d_dense_qp_get_Zl
#define DENSE_QP_GET_ZZU d_dense_qp_get_Zu
#define DENSE_QP_GET_ZL d_dense_qp_get_zl
#define DENSE_QP_GET_ZU d_dense_qp_get_zu
#define DENSE_QP_GET_LS d_dense_qp_get_ls
#define DENSE_QP_GET_US d_dense_qp_get_us


#include "x_dense_qp.c"
