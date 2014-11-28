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
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    //! The point holds two double values.
    /**
     The point is used to represent a point in a space and allows several modification.
     */
    class Point
    {
    private:
        double m_x;
        double m_y;
        
    public:
        //! Constructor.
        /** The function initialize a point at zero origin.
         */
        Point() noexcept :
        m_x(0.), m_y(0.)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a point with two double values.
         */
        Point(const double x, const double y) noexcept :
        m_x(x), m_y(y)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a point a vector of elements.
         */
        Point(ElemVector const& elements) noexcept :
        m_x(0.), m_y(0.)
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                m_x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                m_y = (double)elements[1];
            }
        }
        
        //! Constructor.
        /** The function initialize another point.
         */
        Point(Point const& pt) noexcept :
        m_x(pt.m_x), m_y(pt.m_y)
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
       
        inline void x(const double _x) noexcept
        {
            m_x = _x;
        }
        
        inline void y(const double _y) noexcept
        {
            m_y = _y;
        }
        
        inline double x() const noexcept
        {
            return m_x;
        }
        
        inline double y() const noexcept
        {
            return m_y;
        }
        
        inline Point& operator=(ElemVector const& elements) noexcept
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                m_x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                m_y = (double)elements[1];
            }
            return *this;
        }
        
        inline Point& operator=(Point const& pt) noexcept
        {
            m_x = pt.m_x;
            m_y = pt.m_y;
            return *this;
        }
        
        inline Point& operator+=(Point const& pt) noexcept
        {
            m_x += pt.m_x;
            m_y += pt.m_y;
            return *this;
        }
        
        inline Point& operator+(Point const& pt) noexcept
        {
            return Point(*this) += pt;
        }
        
        inline Point& operator+=(double const value) noexcept
        {
            m_x += value;
            m_y += value;
            return *this;
        }
        
        inline Point& operator+(double const value) noexcept
        {
            return Point(*this) += value;
        }
        
        inline Point& operator-=(Point const& pt) noexcept
        {
            m_x -= pt.m_x;
            m_y -= pt.m_y;
            return *this;
        }
        
        inline Point operator-(Point const& pt) noexcept
        {
            return Point(*this) -= pt;
        }
        
        inline Point& operator-=(double const value) noexcept
        {
            m_x -= value;
            m_y -= value;
            return *this;
        }
        
        inline Point operator-(double const value) noexcept
        {
            return Point(*this) -= value;
        }
        
        inline operator ElemVector() const noexcept
        {
            return {m_x, m_y};
        }
    };
}

#endif