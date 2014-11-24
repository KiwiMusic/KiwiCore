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

#ifndef __DEF_KIWI_RECTANGLE__
#define __DEF_KIWI_RECTANGLE__

#include "Element.h"
#include "Point.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      RECTANGLE                                   //
    // ================================================================================ //
    
    //! The rectangle holds four double values.
    /**
     The rectangle is used to represent a rectangle in a space and allows several modification.
     */
    class Rectangle
    {
    private:
        double m_x;
        double m_y;
        double m_width;
        double m_height;
        
    public:
        
        //! Constructor.
        /** The function initialize a rectangle.
         */
        Rectangle() noexcept :
        m_x(0.), m_y(0.), m_width(0.), m_height(0.)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a rectangle with four double values.
         */
        Rectangle(const double x, const double y, const double width, const double height) noexcept :
        m_x(x), m_y(y), m_width(width), m_height(height)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a rectangle at zero origin with four double values.
         */
        Rectangle(Point const& position, Point const& size) noexcept :
        m_x(position.x()), m_y(position.y()), m_width(size.x()), m_height(size.y())
        {
            ;
        }
        
        ~Rectangle()
        {
            ;
        }
        
        inline double x() const noexcept
        {
            return m_x;
        }
        
        inline double y() const noexcept
        {
            return m_y;
        }
        
        inline double width() const noexcept
        {
            return m_width;
        }
        
        inline double height() const noexcept
        {
            return m_height;
        }
        
        inline Point position() const noexcept
        {
            return Point(m_x, m_y);
        }
        
        inline Point size() const noexcept
        {
            return Point(m_width, m_height);
        }
        
        inline bool contains(Point const& pt)
        {
            return pt.x() >= m_x && pt.y() >= m_y && pt.x() < m_x + m_width && pt.y() < m_y + m_height;
        }
        
        inline Rectangle& operator=(ElemVector const& elements) noexcept
        {
            const ElemVector::size_type size = elements.size();
            if(size && elements[0].isNumber())
            {
                m_x = (double)elements[0];
            }
            if(size > 1 && elements[1].isNumber())
            {
                m_y = (double)elements[1];
            }
            if(size > 2 && elements[2].isNumber())
            {
                m_width = (double)elements[1];
            }
            if(size > 2 && elements[3].isNumber())
            {
                m_height = (double)elements[1];
            }
            return *this;
        }
        
        inline operator ElemVector() const noexcept
        {
            return {m_x, m_y, m_width, m_height};
        }
        
    };
}

#endif