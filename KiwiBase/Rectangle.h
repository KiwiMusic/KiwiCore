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
        /** The function initializes a rectangle.
         */
        Rectangle() noexcept;
        
        //! Constructor.
        /** The function initializes a rectangle with four double values.
         */
        Rectangle(const double x, const double y, const double width, const double height) noexcept;
        
        //! Constructor.
        /** The function initializes a rectangle with two points.
         */
        Rectangle(Point const& position, Point const& size) noexcept;
        
        //! Constructor.
        /** The function initializes a rectangle with a vector of elements.
         */
        Rectangle(ElemVector const& elements) noexcept;
        
        //! Destructor.
        /** The function deletes the rectangle.
         */
        ~Rectangle();
        
        //! Retrieve the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        inline double x() const noexcept
        {
            return m_position.x();
        }
        
        //! Retrieve the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        inline double y() const noexcept
        {
            return m_position.y();
        }
        
        //! Retrieve the width.
        /** The function retrieves the width.
         @return The width.
         */
        inline double width() const noexcept
        {
            return m_size.x();
        }
        
        //! Retrieve the height.
        /** The function retrieves the height.
         @return The height.
         */
        inline double height() const noexcept
        {
            return m_size.y();
        }
        
        //! Retrieve the position.
        /** The function retrieves the position.
         @return The position.
         */
        inline Point position() const noexcept
        {
            return m_position;
        }
        
        //! Retrieve the size.
        /** The function retrieves the size.
         @return The size.
         */
        inline Point size() const noexcept
        {
            return m_size;
        }
        
        //! Set the abscissa.
        /** The function sets the abscissa.
         @param x The abscissa.
         */
        inline void x(double x) noexcept
        {
            m_position.x(x);
        }
        
        //! Set the ordinate.
        /** The function sets the ordinate.
         @param y The ordinate.
         */
        inline void y(double y) noexcept
        {
            m_position.y(y);
        }
        
        //! Set the width.
        /** The function sets the width.
         @param width The width.
         */
        inline void width(double width) noexcept
        {
            m_size.x(max(width, 0.));
        }
        
        //! Set the height.
        /** The function sets the height.
         @param height The height.
         */
        inline void height(double height) noexcept
        {
            m_size.y(max(height, 0.));
        }
        
        //! Set the position.
        /** The function sets the position.
         @param position The position.
         */
        inline void position(Point const& position) noexcept
        {
            m_position = position;
        }
        
        //! Set the size.
        /** The function sets the size.
         @param size The size.
         */
        inline void size(Point const& size) noexcept
        {
            m_size.x(max(size.x(), 0.));
            m_size.y(max(size.x(), 0.));
        }
        
        //! Set the position and the size with another rectangle.
        /** The function sets the position and the size with another rectangle.
         @param rect The other rectangle.
         @return The rectangle.
         */
        inline Rectangle& operator=(Rectangle const& rect) noexcept
        {
            m_position = rect.position();
            m_size = rect.size();
            return *this;
        }
        
        //! Set the position and the size with a vector of elements.
        /** The function sets the position and the size with a vector of elements.
         @param elements A vector of elements with four numbers for the position and the size.
         @return The rectangle.
         */
        inline Rectangle& operator=(ElemVector const& elements) noexcept
        {
            Rectangle other(elements);
            return *this = other;
        }
        
        //! Shift the rectangle with a point.
        /** The function shifts the rectangle with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle& operator+=(Point const& pt) noexcept
        {
            m_position += pt;
            return *this;
        }
        
        //! Shift the rectangle with a vector of elements.
        /** The function shifts the rectangle with a vector of elements.
         @param elements The vector of elements with two values.
         @return The rectangle.
         */
        inline Rectangle& operator+=(ElemVector const& elements) noexcept
        {
            m_position += Point(elements);
            return *this;
        }
        
        //! Shift the rectangle with a value.
        /** The function shifts the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator+=(double const value) noexcept
        {
            m_position += value;
            return *this;
        }
        
        //! Shift inversed the rectangle with a point.
        /** The function shifts inversed the rectangle with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle& operator-=(Point const& pt) noexcept
        {
            m_position -= pt;
            return *this;
        }
        
        //! Shift inversed the rectangle with a vector of elements.
        /** The function shifts inversed the rectangle with a vector of elements.
         @param elements The vector of elements with two values.
         @return The rectangle.
         */
        inline Rectangle& operator-=(ElemVector const& elements) noexcept
        {
            m_position -= Point(elements);
            return *this;
        }
        
        //! Shift inversed the rectangle with a value.
        /** The function shifts inversed the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator-=(double const value) noexcept
        {
            m_position -= value;
            return *this;
        }
        
        //! Expand the rectangle with a point.
        /** The function expands the rectangle with a point.
         @param point The point.
         @return The rectangle.
         */
        inline Rectangle& operator*=(Point const& pt) noexcept
        {
            m_size *= pt;
            return *this;
        }
        
        //! Expand the rectangle with a vector of elements.
        /** The function expands the rectangle with a vector of elements.
         @param elements The vector of elements with two values.
         @return The rectangle.
         */
        inline Rectangle& operator*=(ElemVector const& elements) noexcept
        {
            m_size *= Point(elements);
            return *this;
        }
        
        //! Expand the rectangle with a value.
        /** The function expands the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator*=(double const value) noexcept
        {
            m_size      *= value;
            return *this;
        }
        
        //! Retract the rectangle with a point.
        /** The function retracts the rectangle with a point.
         @param point The point.
         @return The rectangle.
         */
        inline Rectangle& operator/=(Point const& pt) noexcept
        {
            m_size /= pt;
            return *this;
        }
        
        //! Retract the rectangle with a vector of elements.
        /** The function retracts the rectangle with a vector of elements.
         @param elements The vector of elements with two values.
         @return The rectangle.
         */
        inline Rectangle& operator/=(ElemVector const& elements) noexcept
        {
            m_size /=  Point(elements);
            return *this;
        }
        
        //! Retract the rectangle with a value.
        /** The function retracts the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator/=(double const value) noexcept
        {
            m_size /= value;
            return *this;
        }
        
        inline Rectangle operator+(ElemVector const& elements) const noexcept
        {
            return Rectangle(*this) += elements;
        }
        
        inline Rectangle operator+(Point const& pt) const noexcept
        {
            return Rectangle(*this) += pt;
        }
        
        inline Rectangle operator+(double const value) const noexcept
        {
            return Rectangle(*this) += value;
        }

        inline Rectangle operator-(ElemVector const& elements) const noexcept
        {
            return Rectangle(*this) -= elements;
        }

        inline Rectangle operator-(Point const& pt) const noexcept
        {
            return Rectangle(*this) -= pt;
        }

        inline Rectangle operator-(double const value) const noexcept
        {
            return Rectangle(*this) -= value;
        }
        
        inline Rectangle operator*(ElemVector const& elements) const noexcept
        {
            return Rectangle(*this) *= elements;
        }
        
        inline Rectangle operator*(Point const& pt) const noexcept
        {
            return Rectangle(*this) *= pt;
        }
        
        inline Rectangle operator*(double const value) const noexcept
        {
            return Rectangle(*this) *= value;
        }
        
        inline Rectangle operator/(ElemVector const& elements) const noexcept
        {
            return Rectangle(*this) /= elements;
        }
        
        inline Rectangle operator/(Point const& pt) const noexcept
        {
            return Rectangle(*this) /= pt;
        }
        
        inline Rectangle operator/(double const value) const noexcept
        {
            return Rectangle(*this) /= value;
        }
        
        inline bool operator==(Rectangle const rect) const noexcept
        {
            return m_position == rect.position() && m_size == rect.size();
        }
        
        inline bool operator==(ElemVector const& elements) const noexcept
        {
            return *this == Rectangle(elements);
        }
        
        inline bool operator==(Point const& pt) const noexcept
        {
            return m_position == pt && m_size == pt;
        }
        
        inline bool operator==(double const value) const noexcept
        {
            return m_position == value && m_size == value;
        }
        
        inline bool operator!=(Rectangle const rect) const noexcept
        {
            return m_position != rect.position() || m_size != rect.size();
        }
        
        inline bool operator!=(ElemVector const& elements) const noexcept
        {
            return *this != Rectangle(elements);
        }
        
        inline bool operator!=(Point const& pt) const noexcept
        {
            return m_position != pt || m_size != pt;
        }
        
        inline bool operator!=(double const value) const noexcept
        {
            return m_position != value && m_size != value;
        }
        
        inline bool contains(Point const& pt) const noexcept
        {
            return pt.x() >= m_position.x() && pt.y() >= m_position.y() && pt.x() < m_position.x() + m_size.x() && pt.y() < m_position.y() + m_size.y();
        }
        
        inline void expand(double const value) noexcept
        {
            m_position -= value * 0.5;
            m_size     += value;
        }
        
        inline void reduce(double const value) noexcept
        {
            m_position += value * 0.5;
            m_size     -= value;
        }
        
        inline void expand(Point const& pt) noexcept
        {
            m_position -= pt * 0.5;
            m_size     += pt;
        }
        
        inline void reduce(Point const& pt) noexcept
        {
            m_position += pt * 0.5;
            m_size     -= pt;
        }
        
        inline operator ElemVector() const noexcept
        {
            return {m_position.x(), m_position.y(), m_size.x(), m_size.y()};
        }
        
    };
}

#endif