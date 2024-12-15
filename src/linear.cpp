# include <cmath>
# include <assert.h>
# include "linear.h"
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
 
 
//! Class constructor
/*!
Initializes the class by calculating the slope, intercept and
            regression coefficient based on the given constructor arguments.
\note The slope should not be infinite.
*/
Linear::Linear(){}

bool Linear::set(int n, double *x, double *y) {
 
            // calculate the averages of arrays x and y
            double xa = 0, ya = 0;
            for (int i = 0; i < n; i++) {
                xa += x[i];
                ya += y[i];
            }
            xa /= n;
            ya /= n;
 
            // calculate auxiliary sums
            double xx = 0, yy = 0, xy = 0;
            for (int i = 0; i < n; i++) {
                double tmpx = x[i] - xa, tmpy = y[i] - ya;
                xx += tmpx * tmpx;
                yy += tmpy * tmpy;
                xy += tmpx * tmpy;
            }
 
            // calculate regression line parameters
 
            // make sure slope is not infinite
            if (xx == 0) return false;
 
                m_b = xy / xx;
                m_a = ya - m_b * xa;
            m_coeff = (fabs(yy) == 0) ? 1 : xy / sqrt(xx * yy);
            return true;
        }
 
//! Class destructor (empty)
Linear::~Linear() {
 
        }
 
//! Evaluates the linear regression function at the given abscissa.
/*!
\param x the abscissa used to evaluate the linear regression function
*/


double Linear::getValue(double x) {
 
            return m_a + m_b * x;
 
        }
 
//! Returns the slope of the regression line
double Linear::getSlope() {
 
            return m_b;
 
        }
 
//! Returns the intercept on the Y axis of the regression line
double Linear::getIntercept() {
 
            return m_a;
 
        }
 
//! Returns the linear regression coefficient
/*!
The regression coefficient indicated how well linear regression fits to the
original data. It is an expression of error in the fitting and is defined as:
 
\f[ r = \frac{S_{xy}}{\sqrt{S_{x} \cdot S_{y}}} \f]
 
This varies from 0 (no linear trend) to 1 (perfect linear fit). If \f$ |S_y| =
0\f$ and \f$ |S_x| \neq 0 \f$,   then \e r is considered to be equal to 1.
*/
double Linear::getCoefficient() {
 
            return m_coeff;
 
        }
 
