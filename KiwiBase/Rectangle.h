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
        Point m_position;
        Point m_size;
        
    public:
        
        //! Constructor.
        /** The function initialize a rectangle.
         */
        Rectangle() noexcept :
        m_position(0., 0.), m_size(0., 0.)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a rectangle with four double values.
         */
        Rectangle(const double x, const double y, const double width, const double height) noexcept :
        m_position(x, y), m_size(width, height)
        {
            ;
        }
        
        //! Constructor.
        /** The function initialize a rectangle at zero origin with four double values.
         */
        Rectangle(Point const& position, Point const& size) noexcept :
        m_position(position), m_size(size)
        {
            ;
        }
        
        ~Rectangle()
        {
            ;
        }
        
        inline double x() const noexcept
        {
            return m_position.x();
        }
        
        inline double y() const noexcept
        {
            return m_position.y();
        }
        
        inline double width() const noexcept
        {
            return m_size.x();
        }
        
        inline double height() const noexcept
        {
            return m_size.y();
        }
        
        inline Point position() const noexcept
        {
            return m_position;
        }
        
        inline Point size() const noexcept
        {
            return m_size;
        }
        
        inline void x(double x) noexcept
        {
            m_position.x(x);
        }
        
        inline void y(double y) noexcept
        {
            m_position.y(y);
        }
        
        inline void width(double width) noexcept
        {
            m_size.x(width);
        }
        
        inline void height(double height) noexcept
        {
            m_size.y(height);
        }
        
        inline void position(Point const& position) noexcept
        {
            m_position = position;
        }
        
        inline void size(Point const& size) noexcept
        {
            m_size = size;
        }
        
        inline bool contains(Point const& pt) const noexcept
        {
            return pt.x() >= m_position.x() && pt.y() >= m_position.y() && pt.x() < m_position.x() + m_size.x() && pt.y() < m_position.y() + m_size.y();
        }
        
        inline Rectangle& operator=(ElemVector const& elements) noexcept
        {
            const ElemVector::size_type size = elements.size();
            if(size && elements[0].isNumber())
            {
                m_position.x((double)elements[0]);
            }
            if(size > 1 && elements[1].isNumber())
            {
                m_position.y((double)elements[1]);
            }
            if(size > 2 && elements[2].isNumber())
            {
                m_size.x((double)elements[1]);
            }
            if(size > 2 && elements[3].isNumber())
            {
                m_size.y((double)elements[1]);
            }
            return *this;
        }
        
        inline operator ElemVector() const noexcept
        {
            return {m_position.x(), m_position.y(), m_size.x(), m_size.y()};
        }
        
    };
}

#endif