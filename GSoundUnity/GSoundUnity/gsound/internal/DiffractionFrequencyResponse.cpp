/*
 * Project:     GSound
 * 
 * File:        gsound/internal/DiffractionFrequencyResponse.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::DiffractionFrequencyResponse class implementation
 * 
 * License:
 * 
 *     Copyright (C) 2010-12 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 * 
 * 
 * Contact Information:
 *     
 *     Please send all bug reports and other contact to:
 *     Carl Schissler
 *     carl.schissler@gmail.com
 * 
 *     Updates and downloads are available at the main GSound web page:
 *     http://gamma.cs.unc.edu/GSOUND/
 * 
 */


#include "DiffractionFrequencyResponse.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




DiffractionFrequencyResponse:: DiffractionFrequencyResponse(
	const Vector3& sourcePosition,
	const Vector3& listenerPosition,
	const Vector3& diffractionPoint,
	const Vector3& sourceFaceNormal,
	const Vector3& listenerFaceNormal,
	const Vector3& edgeAxis,
	Real speedOfSound )
{
	Vector3 sourceFaceVector = math::cross( edgeAxis, sourceFaceNormal );
	
	Real n = Real(2) - angleBetween(-sourceFaceNormal, listenerFaceNormal) / math::pi<Real>();
	
	Vector3 sourceDirection = sourcePosition - diffractionPoint;  //from apex point to source
	Vector3 listenerDirection = listenerPosition - diffractionPoint; // from apex to receiver
	
	Real p = sourceDirection.getMagnitude();
	Real r = listenerDirection.getMagnitude();
	
	sourceDirection /= p;
	listenerDirection /= r;
	
	Real thetaI = angleBetween( sourceDirection, edgeAxis );
	
	if ( thetaI > math::pi<Real>()*Real(0.5) )
		thetaI = math::pi<Real>() - thetaI;
	
	// Project source and receiver position to plane perpindicular to edge vector.
	Vector3 sDir = projectToPlane(sourceDirection, edgeAxis);
	Vector3 rDir = projectToPlane(listenerDirection, edgeAxis);

	// Get the angles around the wedge for source and receiver, measured from source face.
	Real alphaI = angleBetween(-sDir, sourceFaceVector);
	Real alphaD = angleBetween(rDir, sourceFaceVector) + math::pi<Real>();
	
	FrequencyResponse& output = *this;
	
	for ( Index i = 0; i < output.getNumberOfBands(); i++ )
	{
		Real lambda = speedOfSound / output.getBandCenterFrequency(i);
		Real k = Real(2)*math::pi<Real>() / lambda;
		
		Real utdCoeff = UTD_coefficient( n, k, p, r, thetaI, alphaI, alphaD );
		
		// shadow boundary value
		Real sbCoeff = UTD_coefficient( n, k, p, r, thetaI, alphaI, alphaI + math::pi<Real>() + Real(0.001) );
		
		output[i] = math::clamp( (utdCoeff / sbCoeff)*(utdCoeff / sbCoeff), Real(0), Real(1) );
	}
}




Real DiffractionFrequencyResponse:: UTD_coefficient( Real n, Real k, Real p, Real r, Real thetaI, Real alphaI, Real alphaD )
{
	math::Complex<Real> c1 = UTD_freqTerm( n, k, thetaI );
	math::Complex<Real> F1 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD-alphaI, n, 1) );
	math::Complex<Real> F2 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD-alphaI, n, -1) );
	math::Complex<Real> F3 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD+alphaI, n, 1) );
	math::Complex<Real> F4 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD+alphaI, n, -1) );
	
	Real cot1 = UTD_cotan( math::pi<Real>() + (alphaD - alphaI), Real(2)*n );
	Real cot2 = UTD_cotan( math::pi<Real>() - (alphaD - alphaI), Real(2)*n );
	Real cot3 = UTD_cotan( math::pi<Real>() + (alphaD + alphaI), Real(2)*n );
	Real cot4 = UTD_cotan( math::pi<Real>() - (alphaD + alphaI), Real(2)*n );

	math::Complex<Real> coeff = F1*cot1 + F2*cot2 + F3*cot3 + F4*cot4;
	coeff = coeff * c1;

	coeff = UTD_euler(-k*r) * coeff;
	
	// Kouyoumjian calls for a different distance term here
	// I have used the one Tsingos used
	coeff = math::sqrt( UTD_sphereDisKouyoumjian( r, p ) ) * coeff;
	
	return coeff.getMagnitude();
}




Real DiffractionFrequencyResponse:: UTD_alpha( Real beta, Real n, int nSign )
{
	int N = UTD_N(beta, n, nSign);
	Real numer = Real(2)*math::pi<Real>()*n*Real(N) - beta;
	Real denom = 2;
	Real cosine = math::cos(numer/denom);
	Real alpha = Real(2) * cosine * cosine;
	
	return alpha;
}




Real DiffractionFrequencyResponse:: UTD_L( Real p, Real r, Real thetaI )
{
	Real sine = math::sin(thetaI);
	Real L = UTD_sphereDis(r,p) * sine * sine;
	
	return L;
}




int DiffractionFrequencyResponse:: UTD_N( Real beta, Real n, int nSign )
{
	if ( nSign > 0 )
	{
		if ( beta <= math::pi<Real>()*(n-1) )
			return 0;
		else  // beta > Pi*(n-1)
			return 1;
	}
	else
	{
		if ( beta < math::pi<Real>()*(1-n) )
			return -1;
		else if ( beta >= math::pi<Real>()*(1-n) && beta <= math::pi<Real>()*(1+n) ) // pi(1-n) <= beta <= pi(1+n)
			return 0;
		else // beta > pi*(1+n)
			return 1;
	}
}




Real DiffractionFrequencyResponse:: UTD_cotan( Real numer, Real denom )
{
	return cotangent( numer / denom );
}




math::Complex<Real> DiffractionFrequencyResponse:: UTD_euler( Real x )
{
	return math::Complex<Real>( math::cos(x), math::sin(x) );
}




math::Complex<Real> DiffractionFrequencyResponse:: UTD_estimateF( Real X )
{
	Real numerPhase = X;
	Real denomPhase = X + Real(1.4);
	math::Complex<Real> phaseTerm = UTD_euler(math::pi<Real>()*Real(0.25) * math::sqrt(numerPhase/denomPhase));
	math::Complex<Real> F;

	if ( X < Real(0.8) )
	{
		Real numer = math::sqrt(X);
		Real denom = Real(0.7)*math::sqrt(X) + Real(1.2);
		F = phaseTerm * math::sqrt(math::pi<Real>()*X) * (1 - numer/denom);
	}
	else
	{
		Real numer = Real(0.8);
		Real denom = math::square(X + Real(1.25));
		F = (Real(1) - numer/denom) * phaseTerm;
	}
	
	return F;
}




math::Complex<Real> DiffractionFrequencyResponse:: UTD_freqTerm( Real n, Real k, Real thetaI )
{
	math::Complex<Real> numer = UTD_euler( -math::pi<Real>()*Real(0.25) );
	Real denom = Real(2) * n * math::sqrt(Real(2)*math::pi<Real>()*k) * math::sin( thetaI );
	
	return -numer / denom;
}




Real DiffractionFrequencyResponse:: UTD_sphereDisKouyoumjian( Real r, Real p )
{
	Real numer = p;
	Real denom = r*(p + r);
	
	return numer / denom;
}




Real DiffractionFrequencyResponse:: UTD_sphereDis( Real r, Real p )
{
	Real numer = p * r;
	Real denom = p + r;
	
	return numer / denom;
}




Real DiffractionFrequencyResponse:: cotangent( Real x )
{
	if ( math::abs(x) < math::epsilon<Real>() )
		return math::max<Real>();
	
	return Real(1) / math::tan(x);
}





Real DiffractionFrequencyResponse:: angleBetween( const Vector3 &v1, const Vector3 &v2 )
{
	Real angle = math::dot( v1.normalize(), v2.normalize() );
	
	if ( angle > Real(1) )
		return Real(0);
	else if ( angle < -Real(1) )
		return math::pi<Real>();
	
	return math::acos(angle);
}




Vector3 DiffractionFrequencyResponse:: projectToPlane( const Vector3& v, const Vector3& n )
{
	Real projAmount = math::dot(v, n);
	
	return v - projAmount * n;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
