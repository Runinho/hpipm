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



hpipm_size_t DENSE_QP_SOL_MEMSIZE(struct DENSE_QP_DIM *dim)
	{

	int nv = dim->nv;
	int ne = dim->ne;
	int nb = dim->nb;
	int ng = dim->ng;
	int ns = dim->ns;

	hpipm_size_t size = 0;

	size += 4*sizeof(struct STRVEC); // v pi lam t

	size += 1*SIZE_STRVEC(nv+2*ns); // ux
	size += 1*SIZE_STRVEC(ne); // pi
	size += 2*SIZE_STRVEC(2*nb+2*ng+2*ns); // lam t

	size = (size+63)/64*64; // make multiple of typical cache line size
	size += 64; // align to typical cache line size

	return size;

	}



void DENSE_QP_SOL_CREATE(struct DENSE_QP_DIM *dim, struct DENSE_QP_SOL *qp_sol, void *mem)
	{

	// loop index
	int ii;

	// zero memory (to avoid corrupted memory like e.g. NaN)
	hpipm_size_t memsize = DENSE_QP_SOL_MEMSIZE(dim);
	hpipm_zero_memset(memsize, mem);

	// extract dim
	int nv = dim->nv;
	int ne = dim->ne;
	int nb = dim->nb;
	int ng = dim->ng;
	int ns = dim->ns;


	// vector struct stuff
	struct STRVEC *sv_ptr = (struct STRVEC *) mem;

	qp_sol->v = sv_ptr;
	sv_ptr += 1;
	qp_sol->pi = sv_ptr;
	sv_ptr += 1;
	qp_sol->lam = sv_ptr;
	sv_ptr += 1;
	qp_sol->t = sv_ptr;
	sv_ptr += 1;


	// align to typical cache line size
	hpipm_size_t l_ptr = (hpipm_size_t) sv_ptr;
	l_ptr = (l_ptr+63)/64*64;


	// double stuff
	char *c_ptr;
	c_ptr = (char *) l_ptr;

	char *tmp_ptr;

	// v
	CREATE_STRVEC(nv+2*ns, qp_sol->v, c_ptr);
	c_ptr += qp_sol->v->memsize;
	// pi
	CREATE_STRVEC(ne, qp_sol->pi, c_ptr);
	c_ptr += qp_sol->pi->memsize;
	// lam
	CREATE_STRVEC(2*nb+2*ng+2*ns, qp_sol->lam, c_ptr);
	c_ptr += qp_sol->lam->memsize;
	// t
	CREATE_STRVEC(2*nb+2*ng+2*ns, qp_sol->t, c_ptr);
	c_ptr += qp_sol->t->memsize;

	qp_sol->valid_obj = 0;

	qp_sol->dim = dim;

	qp_sol->memsize = DENSE_QP_SOL_MEMSIZE(dim);


#if defined(RUNTIME_CHECKS)
	if(c_ptr > ((char *) mem) + qp_sol->memsize)
		{
		printf("\nDENSE_QP_SOL_CREATE: outsize memory bounds!\n\n");
		exit(1);
		}
#endif


	return;

	}



void DENSE_QP_SOL_GET_ALL(struct DENSE_QP_SOL *qp_sol, REAL *v, REAL *ls, REAL *us, REAL *pi, REAL *lam_lb, REAL *lam_ub, REAL *lam_lg, REAL *lam_ug, REAL *lam_ls, REAL *lam_us)
	{

	int nv = qp_sol->dim->nv;
	int ne = qp_sol->dim->ne;
	int nb = qp_sol->dim->nb;
	int ng = qp_sol->dim->ng;
	int ns = qp_sol->dim->ns;

	UNPACK_VEC(nv, qp_sol->v, 0, v, 1);
	if(ne>0)
		{
		UNPACK_VEC(ne, qp_sol->pi, 0, pi, 1);
		}
	if(nb>0)
		{
		UNPACK_VEC(nb, qp_sol->lam, 0, lam_lb, 1);
		UNPACK_VEC(nb, qp_sol->lam, nb+ng, lam_ub, 1);
		}
	if(ng>0)
		{
		UNPACK_VEC(ng, qp_sol->lam, nb, lam_lg, 1);
		UNPACK_VEC(ng, qp_sol->lam, 2*nb+ng, lam_ug, 1);
		}
	if(ns>0)
		{
		UNPACK_VEC(ns, qp_sol->v, nv, ls, 1);
		UNPACK_VEC(ns, qp_sol->v, nv+ns, us, 1);
		UNPACK_VEC(ns, qp_sol->lam, 2*nb+2*ng, lam_ls, 1);
		UNPACK_VEC(ns, qp_sol->lam, 2*nb+2*ng+ns, lam_us, 1);
		}

	return;

	}



void DENSE_QP_SOL_GET(char *field, struct DENSE_QP_SOL *qp_sol, void *value)
	{
	if(hpipm_strcmp(field, "v")) 
		{
		DENSE_QP_SOL_GET_V(qp_sol, value);
		}
	else
		{
		printf("error: DENSE_QP_SOL_GET: wrong field name '%s'. Exiting.\n", field);
		exit(1);	
		}
	return;
	}



void DENSE_QP_SOL_GET_V(struct DENSE_QP_SOL *qp_sol, REAL *v)
	{
	int nv = qp_sol->dim->nv;
	UNPACK_VEC(nv, qp_sol->v, 0, v, 1);
	}



void DENSE_QP_SOL_GET_VALID_OBJ(struct DENSE_QP_SOL *qp_sol, int *valid_obj)
	{
	*valid_obj = qp_sol->valid_obj;
	}



void DENSE_QP_SOL_GET_OBJ(struct DENSE_QP_SOL *qp_sol, REAL *obj)
	{
	*obj = qp_sol->obj;
	}
