/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Written (W) 2013 Soumyajit De
 */

#include <shogun/lib/common.h>
#include <shogun/lib/SGVector.h>
#include <shogun/mathematics/Math.h>
#include <shogun/mathematics/logdet/IterativeShiftedLinearFamilySolver.h>

namespace shogun
{

template <class T, class ST>
CIterativeShiftedLinearFamilySolver<T, ST>::CIterativeShiftedLinearFamilySolver()
	: CIterativeLinearSolver<T, T>()
	{
		SG_SGCDEBUG("%s created (%p)\n", this->get_name(), this);
	}

template <class T, class ST>
CIterativeShiftedLinearFamilySolver<T, ST>::~CIterativeShiftedLinearFamilySolver()
	{
		SG_SGCDEBUG("%s destroyed (%p)\n", this->get_name(), this);
	}

template <class T, class ST>
void CIterativeShiftedLinearFamilySolver<T, ST>::compute_zeta_sh_new(
		SGVector<ST> zeta_sh_old, SGVector<ST> zeta_sh_cur, SGVector<ST> shifts,
		T beta_old, T beta_cur, T alpha, SGVector<ST>& zeta_sh_new)
	{
		// compute zeta_sh_new according to Jergerlehner, eq. 2.44
		// [see IterativeShiftedLinearFamilySolver.h]
		for (index_t i=0; i<zeta_sh_new.vlen; ++i)
		{
			ST numer=zeta_sh_old[i]*zeta_sh_cur[i]*beta_old;

			ST denom=beta_cur*alpha*(zeta_sh_old[i]-zeta_sh_cur[i])
				+beta_old*zeta_sh_old[i]*(1.0-beta_cur*shifts[i]);

			// handle division by zero
			if (denom==static_cast<ST>(0.0))
				denom=static_cast<ST>(CMath::MACHINE_EPSILON);

			zeta_sh_new[i]=numer/denom;
	  }
	}

template <class T, class ST>
void CIterativeShiftedLinearFamilySolver<T, ST>::compute_beta_sh(
		SGVector<ST> zeta_sh_new, SGVector<ST> zeta_sh_cur, T beta_cur,
		SGVector<ST>& beta_sh_cur)
	{
		// compute beta_sh_cur according to Jergerlehner, eq. 2.42
		// [see IterativeShiftedLinearFamilySolver.h]
		for (index_t i=0; i<beta_sh_cur.vlen; ++i)
		{
			ST numer=beta_cur*zeta_sh_new[i];
			ST denom=zeta_sh_cur[i];

			// handle division by zero
			if (denom==static_cast<ST>(0.0))
				denom=static_cast<ST>(CMath::MACHINE_EPSILON);

			beta_sh_cur[i]=numer/denom;
		}
	}

template <class T, class ST>
void CIterativeShiftedLinearFamilySolver<T, ST>::compute_alpha_sh(
		SGVector<ST> zeta_sh_cur, SGVector<ST> zeta_sh_old,
		SGVector<ST> beta_sh, T beta, T alpha, SGVector<ST>& alpha_sh)
	{
		// compute alpha_sh_cur according to Jergerlehner, eq. 2.43
		// [see IterativeShiftedLinearFamilySolver.h]
  	for (index_t i=0; i<alpha_sh.vlen; ++i)
	  {
			ST numer=alpha*zeta_sh_cur[i]*beta_sh[i];
			ST denom=zeta_sh_old[i]*beta;

			// handle division by zero
			if (denom==static_cast<ST>(0.0))
				denom=static_cast<ST>(CMath::MACHINE_EPSILON);

			alpha_sh[i]=numer/denom;
	  }
	}

template class CIterativeShiftedLinearFamilySolver<float64_t>;
template class CIterativeShiftedLinearFamilySolver<float64_t, complex64_t>;
}