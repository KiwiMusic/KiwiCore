/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef __DEF_KIWI_TOOLS_MATH__
#define __DEF_KIWI_TOOLS_MATH__

#include "Defs.h"

namespace Kiwi
{
	template <typename Type> Type clip(const Type& n, const Type& lower, const Type& upper)
    {
        return max(lower, min(n, upper));
    }
    
    // Solve a 3rd degree equation, return the number of solution and the solution are in solution1, solution2 and solution3. 
    template <typename Type> unsigned long solve(Type a, Type b, Type c, Type const d, Type &solution1, Type &solution2,  Type &solution3)
	{
		if(abs(a) > 0.)
		{
			Type z = a;
			a = b / z;
			b = c / z;
			c = d / z;
			
			Type p = b - a * a / 3.;
			Type q = a * (2. * a * a - 9. * b) / 27. + c;
			Type p3 = p * p * p;
			Type D = q * q + 4. * p3 / 27.;
			Type offset = -a / 3.;
            if(D == 0.)
            {
                Type u;
				if(q < 0.)
                {
                    u = pow( -q / 2., 1. / 3.);
                }
                else
                {
                    u = -pow( q / 2., 1. / 3.);
                }
                solution1 = 2. * u + offset;
                solution2 = -u + offset;
                return 2;
            }
			else if(D > 0.)
			{
				z = sqrt(D);
				Type u = ( -q + z) / 2.;
				Type v = ( -q - z) / 2.;
				u = (u >= 0.) ? pow(u, 1. / 3.) : - pow( -u, 1. / 3.);
				v = (v >= 0.) ? pow(v, 1. / 3.) : - pow( -v, 1. / 3.);
				solution1 = u + v + offset;
				return 1;
			}
            else
			{
				Type u = 2. * sqrt( -p / 3.);
				Type v = acos(-sqrt( -27. / p3) * q / 2.) / 3.;
				solution1 = u * cos(v) + offset;
				solution2 = u * cos(v + 2. * M_PI / 3.) + offset;
				solution3 = u * cos(v + 4. * M_PI / 3.) + offset;
				return 3;
			}
		}
        else
		{
			a = b;
			b = c;
			c = d;
			if(abs(a) <= 0.)
			{
				if(abs(b) <= 0.)
                {
                    return 0;
                }
				else
				{
					solution1 = -c / b;
					return 1;
				}
			}
            
			Type D = b*b - 4.*a*c;
			if(D == 0)
            {
				solution1 = -b / (2. * a);
				return 1;
            }
			if(D > 0.)
			{
				D = sqrt(D);
				solution1 = ( -b - D) / (2. * a);
				solution2 = ( -b + D) / (2. * a);
				return 2;
			}
            else
			{
				return 0;
			}
		}
	}
    
    inline long roundupPowerOfTwo(long x)
    {
        if(x < 1)
            return 1;
        
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x+1;
    }
};

#endif


