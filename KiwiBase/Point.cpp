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
    
    Point Point::fromLine(Point const& begin, Point const& end, double delta) noexcept
    {
        return Point(begin.x() + (end.x() - begin.x()) * delta, begin.y() + (end.y() - begin.y()) * delta);
    }
    
    Point Point::fromLine(Point const& begin, Point const& ctrl, Point const& end, const double delta) noexcept
    {
        const double mdelta = (1. - delta);
        const double fac1 = mdelta * mdelta;
        const double fac2 = 2. * delta * mdelta;
        const double fac3 = delta * delta;
        return Point(begin.x() * fac1 + ctrl.x() * fac2 + end.x() * fac3, begin.y() * fac1 + ctrl.y() * fac2 + end.y() * fac3);
    }
    
    Point Point::fromLine(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, const double delta) noexcept
    {
        const double mdelta = (1. - delta);
        const double fac1 = mdelta * mdelta * mdelta;
        const double fac2 = 3. * delta * mdelta * mdelta;
        const double fac3 = 3. * delta * delta * mdelta;
        const double fac4 = delta * delta * mdelta;
        return Point(begin.x() * fac1 + ctrl1.x() * fac2 + ctrl2.x() * fac3 + end.x() * fac4, begin.y() * fac1 + ctrl1.y() * fac2 + ctrl2.y() * fac3 + end.y() * fac4);
    }
    
    double Point::distance(Point const& begin, Point const& end) const noexcept
    {
        const Point delta(end - begin);
        const double length = delta.x() * delta.x() + delta.y() * delta.y();
        
        if(length > 0.)
        {
            const double ratio = ((m_x - begin.x()) * delta.x() + (m_x - begin.x()) * delta.x()) / length;
            if(ratio < 0.)
            {
                return this->distance(begin);
            }
            else if(ratio > 1.)
            {
                return this->distance(end);
            }
            else
            {
                return this->distance(begin + delta * ratio);
            }
        }
        else
        {
            const double distBegin  = this->distance(begin);
            const double distEnd    = this->distance(end);
            if(distBegin < distEnd)
            {
                return distBegin;
            }
            else
            {
                return distEnd;
            }
        }
    }
    
    double Point::distance(Point const& begin, Point const& ctrl, Point const& end) const noexcept
    {
		const Point A(ctrl.x() - begin.x(), ctrl.y() - begin.y());
        const Point B(begin.x() - ctrl.x() * 2. + end.x(), begin.y() - ctrl.y() * 2. + end.y());
        const Point C(begin.x() - m_x, begin.y() - m_y);
        
        double sol1, sol2, sol3;
        const ulong nresult = solve(B.x() * B.x() + B.y() * B.y(), 3 * (A.x() * B.x() + A.y() * B.y()), 2 * (A.x() * A.x() + A.y() * A.y()) + C.x() * B.x() + C.y() * B.y(), C.x() * A.x() + C.y() * A.y(), sol1, sol2, sol3);
        if(nresult)
        {
            double dist = this->distance(fromLine(begin, ctrl, end, sol1));
            if(nresult > 1)
            {
                const double dist2 = this->distance(fromLine(begin, ctrl, end, sol2));
                if(dist2 < dist)
                {
                    dist  = dist2;
                }
            }
            if(nresult > 2)
            {
                const double dist2 = this->distance(fromLine(begin, ctrl, end, sol3));
                if(dist2 < dist)
                {
                    dist  = dist2;
                }
            }
            return dist;
        }
        else
        {
            const double dist1 = this->distance(begin);
            const double dist2 = this->distance(end);
            if(dist1 < dist2)
            {
                return dist1;
            }
            else
            {
                return dist2;
            }
        }
    }
    
    bool Point::near(Point const& pt, double const distance) const noexcept
    {
        return this->distance(pt) <= distance;
    }
    
    bool Point::near(Point const& begin, Point const& end, double const distance) const noexcept
    {
        return this->distance(begin, end) <= distance;
    }
    
    bool Point::near(Point const& begin, Point const& ctrl, Point const& end, double const dist) const noexcept
    {
        return this->distance(begin, ctrl, end) <= dist;
    }
    
    bool Point::near(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, double const distance) const noexcept
    {
        int zaza; // For the moment
        for(double i = 0; i < 1.001; i += 0.01)
        {
            if(this->distance(fromLine(begin, ctrl1, ctrl2, end, i)) <= distance)
            {
                return true;
            }
        }
        return false;
    }
    
}