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

#ifndef __DEF_KIWI_POINT__
#define __DEF_KIWI_POINT__

#include "Element.h"

namespace Kiwi
{
    class Doodle;
    
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    //! The point holds two double value.
    /**
     The point is used to represent a point in a space and allows several modification.
     */
    class Point
    {
    public:
        double x;
        double y;
        
        //! Constructor.
        /** The function initialize a point at zero origin.
         */
        Point() noexcept :
        x(0.), y(0.)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a point with two double values.
         */
        Point(const double _x, const double _y) noexcept :
        x(_x), y(_y)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a point a vector of elements.
         */
        Point(ElemVector const& elements) noexcept :
        x(0.), y(0.)
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                y = (double)elements[1];
            }
        }
        
        //! Constructor.
        /** The function initialize another point.
         */
        Point(Point const& pt) noexcept :
        x(pt.x), y(pt.y)
        {
            ;
        }
        
        //! Destructor.
        /** The function initialize another point.
         */
        ~Point()
        {
            ;
        }
        
        inline Point& operator=(ElemVector const& elements) noexcept
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                y = (double)elements[1];
            }
            return *this;
        }
        
        inline Point& operator=(Point const& pt) noexcept
        {
            x = pt.x;
            y = pt.y;
            return *this;
        }
    };
}

#endif