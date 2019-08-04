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



#ifndef HPIPM_D_DENSE_QP_SOL_H_
#define HPIPM_D_DENSE_QP_SOL_H_



#include <blasfeo_target.h>
#include <blasfeo_common.h>

#include "hpipm_d_dense_qp_dim.h"



#ifdef __cplusplus
extern "C" {
#endif



struct d_dense_qp_sol
	{
	struct d_dense_qp_dim *dim;
	struct blasfeo_dvec *v;
	struct blasfeo_dvec *pi;
	struct blasfeo_dvec *lam;
	struct blasfeo_dvec *t;
	void *misc;
	int memsize;
	};



//
int d_memsize_dense_qp_sol(struct d_dense_qp_dim *dim);
//
void d_create_dense_qp_sol(struct d_dense_qp_dim *dim, struct d_dense_qp_sol *qp_sol, void *memory);
//
void d_cvt_dense_qp_sol_to_colmaj(struct d_dense_qp_sol *qp_sol, double *v, double *ls, double *us, double *pi, double *lam_lb, double *lam_ub, double *lam_lg, double *lam_ug, double *lam_ls, double *lam_us);
//
void d_cvt_dense_qp_sol_to_rowmaj(struct d_dense_qp_sol *qp_sol, double *v, double *ls, double *us, double *pi, double *lam_lb, double *lam_ub, double *lam_lg, double *lam_ug, double *lam_ls, double *lam_us);
//
void d_cvt_dense_qp_sol_to_libstr(struct d_dense_qp_sol *qp_sol, struct blasfeo_dvec *v, struct blasfeo_dvec *ls, struct blasfeo_dvec *us, struct blasfeo_dvec *pi, struct blasfeo_dvec *lam_lb, struct blasfeo_dvec *lam_ub, struct blasfeo_dvec *lam_lg, struct blasfeo_dvec *lam_ug, struct blasfeo_dvec *lam_ls, struct blasfeo_dvec *lam_us);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif // HPIPM_D_DENSE_QP_SOL_H_
