# include <cmath>
# include <assert.h>
// CodeCogs GNU General Public License Agreement
// Copyright (C) 2004-2005 CodeCogs, Zyba Ltd, Broadwood, Holford, TA5 1DU, England.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by CodeCogs. 
// You must retain a copy of this licence in all copies. 
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the Adapted GNU General Public License for more details.
//
// *** THIS SOFTWARE CAN NOT BE USED FOR COMMERCIAL GAIN. ***
// --------------------------------------------------------------------------------- 
 
class Linear {
public:
 
//! Class constructor
/*!
Initializes the class by calculating the slope, intercept and
            regression coefficient based on the given constructor arguments.
\note The slope should not be infinite.
*/

Linear();
bool set(int n, double *x, double *y);
 
//! Class destructor (empty)
~Linear();
//! Evaluates the linear regression function at the given abscissa.
/*!
\param x the abscissa used to evaluate the linear regression function
*/
double getValue(double x);
 
//! Returns the slope of the regression line
double getSlope();
 
//! Returns the intercept on the Y axis of the regression line
double getIntercept();
//! Returns the linear regression coefficient
/*!
The regression coefficient indicated how well linear regression fits to the
original data. It is an expression of error in the fitting and is defined as:
 
\f[ r = \frac{S_{xy}}{\sqrt{S_{x} \cdot S_{y}}} \f]
 
This varies from 0 (no linear trend) to 1 (perfect linear fit). If \f$ |S_y| =
0\f$ and \f$ |S_x| \neq 0 \f$,   then \e r is considered to be equal to 1.
*/
double getCoefficient();
 
private:
 
double m_a, m_b, m_coeff;
};
