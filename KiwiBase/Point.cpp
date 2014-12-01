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

#include "Point.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    Point::Point() noexcept :
    m_x(0.), m_y(0.)
    {
        ;
    }
    
    Point::Point(const double x, const double y) noexcept :
    m_x(x), m_y(y)
    {
        ;
    }
    
    Point::Point(ElemVector const& elements) noexcept :
    m_x(!elements.empty() ? (double)elements[0] : 0.), m_y(elements.size() > 1 ? (double)elements[1] : 0.)
    {
        ;
    }
    
    Point::Point(Point const& pt) noexcept :
    m_x(pt.m_x), m_y(pt.m_y)
    {
        ;
    }
    
    Point::~Point()
    {
        ;
    }
    
}