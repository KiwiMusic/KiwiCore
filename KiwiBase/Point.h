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
        /** The function initializes a point at zero origin.
         */
        Point() noexcept;
        
        //! Constructor.
        /** The function initializes a point with two double values.
         @param x The abscissa.
         @param y The ordinate.
         */
        Point(const double x, const double y) noexcept;
        
        //! Constructor.
        /** The function initializes a point a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         */
        Point(ElemVector const& elements) noexcept;
        
        //! Constructor.
        /** The function initializes another point.
         */
        Point(Point const& pt) noexcept;
        
        //! Destructor.
        /** The function deletes the point.
         */
        ~Point();
       
        //! Set the abscissa.
        /** The function sets the abscissa.
         @param x The abscissa.
         */
        inline void x(const double x) noexcept
        {
            m_x = x;
        }
        
        //! Set the ordinate.
        /** The function sets the ordinate.
         @param y The ordinate.
         */
        inline void y(const double y) noexcept
        {
            m_y = y;
        }
        
        //! Retrieve the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        inline double x() const noexcept
        {
            return m_x;
        }
        
        //! Retrieve the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        inline double y() const noexcept
        {
            return m_y;
        }
        
        //! Set the abscissa and the ordinate with another point.
        /** The function sets the abscissa and the ordinate with another point.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator=(Point const& pt) noexcept
        {
            m_x = pt.m_x;
            m_y = pt.m_y;
            return *this;
        }
        
        //! Set the abscissa and the ordinate with a vector of elements.
        /** The function sets the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return The point.
         */
        inline Point& operator=(ElemVector const& elements) noexcept
        {
            return *this = Point(elements);
        }
        
        //! Set the abscissa and the ordinate with a value.
        /** The function sets the abscissa and the ordinate with a value.
         @param value The value.
         @return The point.
         */
        inline Point& operator=(double const& value) noexcept
        {
            m_x = value;
            m_y = value;
            return *this;
        }
        
        //! Increment the abscissa and the ordinate with another point.
        /** The function increments the abscissa and the ordinate with another point.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator+=(Point const& pt) noexcept
        {
            m_x += pt.m_x;
            m_y += pt.m_y;
            return *this;
        }
        
        //! Increment the abscissa and the ordinate with a vector of elements.
        /** The function increments the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return The point.
         */
        inline Point& operator+=(ElemVector const& elements) noexcept
        {
            return *this += Point(elements);
        }
        
        //! Increment the abscissa and the ordinate with a value.
        /** The function increments the abscissa and the ordinate with a value.
         @param value The value.
         @return The point.
         */
        inline Point& operator+=(double const value) noexcept
        {
            m_x += value;
            m_y += value;
            return *this;
        }
        
        //! Decrement the abscissa and the ordinate with another point.
        /** The function decrements the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator-=(Point const& pt) noexcept
        {
            m_x -= pt.m_x;
            m_y -= pt.m_y;
            return *this;
        }
        
        //! Decrement the abscissa and the ordinate with a vector of elements.
        /** The function decrements the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return The point.
         */
        inline Point& operator-=(ElemVector const& elements) noexcept
        {
            return *this -= Point(elements);
        }
        
        //! Multiply the abscissa and the ordinate with a vector of elements.
        /** The function multiplies the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return The point.
         */
        inline Point& operator*=(ElemVector const& elements) noexcept
        {
            return *this *= Point(elements);
        }
        
        //! Decrement the abscissa and the ordinate with a value.
        /** The function decrements the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator-=(double const value) noexcept
        {
            m_x -= value;
            m_y -= value;
            return *this;
        }
        
        //! Multiply the abscissa and the ordinate with another point.
        /** The function multiplies the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator*=(Point const& pt) noexcept
        {
            m_x *= pt.m_x;
            m_y *= pt.m_y;
            return *this;
        }
        
        //! Multiply the abscissa and the ordinate with a value.
        /** The function multiplies the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator*=(double const value) noexcept
        {
            m_x *= value;
            m_y *= value;
            return *this;
        }
        
        //! Divide the abscissa and the ordinate with another point.
        /** The function divides the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator/=(Point const& pt) noexcept
        {
            m_x /= pt.m_x;
            m_y /= pt.m_y;
            return *this;
        }
        
        //! Divide the abscissa and the ordinate with a vector of elements.
        /** The function divides the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return The point.
         */
        inline Point& operator/=(ElemVector const& elements) noexcept
        {
            return *this /= Point(elements);
        }
        
        //! Divide the abscissa and the ordinate with a value.
        /** The function divides the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator/=(double const value) noexcept
        {
            m_x /= value;
            m_y /= value;
            return *this;
        }
        
        //! Compare the equality of the abscissa and the ordinate with a vector of elements.
        /** The function compares the equality of the the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return true if the vector of elements is equal to the point, otherwise false.
         */
        inline bool operator==(ElemVector const& elements) const noexcept
        {
            if(elements.size() > 1 && elements[1].isNumber() && elements[0].isNumber())
            {
                return m_x == (double)elements[0] && m_y == (double)elements[1];
            }
            return false;
        }
        
        //! Compare the equality of the abscissa and the ordinate with another point.
        /** The function compares the equality of the the abscissa and the ordinate with another point.
         @param value The value.
         @return true if the two poitn are equal, otherwise false.
         */
        inline bool operator==(Point const& pt) const noexcept
        {
            return m_x == pt.x() && m_y == pt.y();
        }
        
        //! Compare the equality of the abscissa and the ordinate with a value.
        /** The function compares the equality of the abscissa and the ordinate with a value.
         @param value A value.
         @return true if the vector of elements is equal to the point, otherwise false.
         */
        inline bool operator==(double const value) const noexcept
        {
            return m_x == value && m_y == value;
        }
        
        //! Compare the equality of the abscissa and the ordinate with a vector of elements.
        /** The function compares the equality of the the abscissa and the ordinate with a vector of elements.
         @param elements A vector of elements with two numbers for the abscissa and the ordinate.
         @return true if the vector of elements is not equal to the point, otherwise false.
         */
        inline bool operator!=(ElemVector const& elements) const noexcept
        {
            if(elements.size() > 1 && elements[1].isNumber() && elements[0].isNumber())
            {
                return m_x != (double)elements[0] || m_y != (double)elements[1];
            }
            return false;
        }
        
        //! Compare the equality of the abscissa and the ordinate with another point.
        /** The function compares the equality of the the abscissa and the ordinate with another point.
         @param value The value.
         @return true if the two poitn are not equal, otherwise false.
         */
        inline bool operator!=(Point const& pt) const noexcept
        {
            return m_x != pt.x() || m_y != pt.y();
        }
        
        //! Compare the equality of the abscissa and the ordinate with a value.
        /** The function compares the equality of the abscissa and the ordinate with a value.
         @param value A value.
         @return true if the vector of elements is not equal to the point, otherwise false.
         */
        inline bool operator!=(double const value) const noexcept
        {
            return m_x != value && m_y != value;
        }
        
        inline Point& operator+(ElemVector const& elements) const noexcept
        {
            return Point(*this) += elements;
        }
        
        inline Point& operator+(Point const& pt) const noexcept
        {
            return Point(*this) += pt;
        }
        
        inline Point& operator+(double const value) const noexcept
        {
            return Point(*this) += value;
        }
        
        inline Point operator-(ElemVector const& elements) const noexcept
        {
            return Point(*this) -= elements;
        }
        
        inline Point operator-(Point const& pt) const noexcept
        {
            return Point(*this) -= pt;
        }
        
        inline Point operator-(double const value) const noexcept
        {
            return Point(*this) -= value;
        }
        
        inline Point operator*(ElemVector const& elements) const noexcept
        {
            return Point(*this) *= elements;
        }
        
        inline Point operator*(Point const& pt) const noexcept
        {
            return Point(*this) *= pt;
        }
        
        inline Point operator*(double const value) const noexcept
        {
            return Point(*this) *= value;
        }
        
        inline Point operator/(ElemVector const& elements) const noexcept
        {
            return Point(*this) /= elements;
        }
        
        inline Point operator/(Point const& pt) const noexcept
        {
            return Point(*this) /= pt;
        }
        
        inline Point operator/(double const value) const noexcept
        {
            return Point(*this) /= value;
        }
        
        inline Point operator-() const noexcept
        {
            return Point(-m_x, -m_y);
        }
        
        //! Retrieve the distance from the origin.
        /** The function retrieves the distance from the origin.
         @return The distance.
         */
        inline double distance() const noexcept
        {
            return sqrt(m_x * m_x + m_y * m_y);
        }
        
        //! Retrieve the distance from another point.
        /** The function retrieves the distance from another point.
         @param pt The other point.
         @return The distance.
         */
        inline double distance(Point const& pt) const noexcept
        {
            return sqrt((m_x - pt.x()) * (m_x - pt.x()) + (m_y - pt.y()) * (m_y - pt.y()));
        }
        
        //! Retrieve the point over a line.
        /** The function retrieves the point over a line.
         @param begin The first point of the line.
         @param end   The end point of the line.
         @param delta The position of the point over the line (first point is 0 and and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& begin, Point const& end, const double delta) noexcept;
        
        //! Retrieve the point over a quadratic bezier line.
        /** The function retrieves the point over a quadratic bezier line.
         @param begin The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @param delta The position of the point over the line (first point is 0 and and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& begin, Point const& ctrl, Point const& end, const double delta) noexcept;
        
        //! Retrieve the point over a cubic bezier line.
        /** The function retrieves the point over a cubic bezier line.
         @param begin The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end The end point of the line.
         @param delta The position of the point over the line (first point is 0 and and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, const double delta) noexcept;
        
        //! Retrieve the distance from a line.
        /** The function retrieves the distance a line.
         @param begin The first point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& begin, Point const& end) const noexcept;
        
        //! Retrieve the distance from a quadratic bezier line.
        /** The function retrieves the distance a quadratic bezier line.
         @param begin The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& begin, Point const& ctrl, Point const& end) const noexcept;
        
        //! Retrieve the distance from a cubic bezier line.
        /** The function retrieves the distance a cubic bezier line.
         @param begin The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept;
        
        //! Retrieve the angle from the origin.
        /** The function retrieves the angle from origin.
         @return The angle.
         */
        inline double angle() const noexcept
        {
            return atan2(m_y, m_x);
        }
        
        //! Retrieve the angle from another point.
        /** The function retrieves the angle from another point.
         @param pt The other point.
         @return The angle.
         */
        inline double angle(Point const& pt) const noexcept
        {
            return atan2(m_y - pt.y(), m_x - pt.x());
        }
        
        //! Retrieve a copy and apply a rotation from the origin.
        /** The function retrieves a copy and applies a rotation from the origin.
         @param The angle
         @return The copy with the rotation.
         */
        Point rotate(double const angle) const noexcept
        {
            return Point(m_x * cos(angle) - m_y * sin(angle), m_x * sin(angle) + m_y * cos(angle));
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @pram pt The other point.
         @param The angle
         @return The copy with the rotation.
         */
        inline Point rotate(Point const& pt, double const angle) const noexcept
        {
            Point newpt = Point(*this) - pt;
            return Point(newpt.x() * cos (angle) - newpt.y() * sin (angle) + pt.x(), newpt.x() * sin (angle) + newpt.y() * cos (angle) + pt.y());
        }
        
        //! Retrieve the dot product with another point.
        /** The function retrieves the dot product with another point.
         @param pt The other point.
         @return The dot product.
         */
        inline double dot(Point const& pt) const noexcept
        {
            return m_x * pt.x() + m_y * pt.y();
        }
        
        //! Get if the point is near to another point.
        /** The function gets if the point is near to another point.
         @param pt The other point.
         @param distance The distance of neighborhood.
         @return true if the two points are near, otherwise false.
         */
        bool near(Point const& pt, double const distance) const noexcept;
    
        //! Get if the point is near or over a line.
        /** The function gets if the point is near or over a line.
         @param begin The first point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& begin, Point const& end, double const distance) const noexcept;
        
        //! Get if the point is near or over a quadratic bezier line.
        /** The function gets if the point is near or over a quadratic bezier line.
         @param begin The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& begin, Point const& ctrl, Point const& end, double const distance) const noexcept;
        
        //! Get if the point is near or over a cubic bezier line.
        /** The function gets if the point is near or over a cubic bezier line.
         @param begin The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, double const distance) const noexcept;
        
        //! Create a vector of elements with the point.
        /** The function creates a vector of elements with the point.
         @return The vector of elements.
         */
        inline operator ElemVector() const noexcept
        {
            return ElemVector{m_x, m_y};
        }
    };
}

#endif