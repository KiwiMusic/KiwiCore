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

#include "Path.h"

namespace Kiwi
{
    Path::Path() noexcept
    {
        ;
    }
    
    Path::Path(Path const& path) noexcept
    {
        m_points = path.m_points;
    }

    Path::Path(Point const& pt) noexcept
    {
        m_points.push_back({pt, Move});
    }
    
    Path::Path(ElemVector const& elements) noexcept
    {
        if(elements.size() > 1)
        {
            m_points.push_back({Point((double)elements[0], (double)elements[1]), Move});
        }
        for(ElemVector::size_type i = 2; i < elements.size(); i += 2)
        {
            if(i+1 < elements.size())
            {
                m_points.push_back({Point((double)elements[i], (double)elements[i+1]), Linear});
            }
        }
    }
    
    Path::~Path()
    {
        m_points.clear();
    }
    
    void Path::moveTo(const Kiwi::Point &pt) noexcept
    {
        m_points.push_back({pt, Move});
    }
    
    void Path::lineTo(const Kiwi::Point &pt) noexcept
    {
        m_points.push_back({pt, Linear});
    }
    
    void Path::quadraticTo(Point const& control, Point const& end)
    {
        m_points.push_back({control, Quadratic});
        m_points.push_back({end, Linear});
    }

    void Path::cubicTo(Point const& control1, Point const& control2, Point const& end)
    {
        m_points.push_back({control1, Cubic});
        m_points.push_back({control2, Cubic});
        m_points.push_back({end, Linear});
    }
	
	void Path::setPoint(ulong index, Point const& pt) noexcept
	{
		if(index < m_points.size())
		{
			m_points[(vector<Node>::size_type)index].point = pt;
		}
	}
    
    void Path::clear() noexcept
    {
        m_points.clear();
    }
    
    Point Path::getPosition() const noexcept
    {
        Point position(0., 0.);
        if(!m_points.empty())
        {
            position = m_points[0].point;
            
            for(vector<Point>::size_type i = 1; i < m_points.size(); i++)
            {
                if(m_points[i].point.x() < position.x())
                {
                    position.x(m_points[i].point.x());
                }
                if(m_points[i].point.y() < position.y())
                {
                    position.y(m_points[i].point.y());
                }
            }
        }
        return position;
    }
    
    Point Path::getSize() const noexcept
    {
        return getBounds().size();
    }
    
    Rectangle Path::getBounds() const noexcept
    {
        Point position(0., 0.);
        Point size(0., 0.);
        if(!m_points.empty())
        {
            position = m_points[0].point;
            size = m_points[0].point;
            
            for(vector<Point>::size_type i = 1; i < m_points.size(); i++)
            {
                if(m_points[i].point.x() < position.x())
                {
                    position.x(m_points[i].point.x());
                }
                else if(m_points[i].point.x() > size.x())
                {
                    size.x(m_points[i].point.x());
                }
                
                if(m_points[i].point.y() < position.y())
                {
                    position.y(m_points[i].point.y());
                }
                else if(m_points[i].point.y() > size.y())
                {
                    size.y(m_points[i].point.y());
                }
            }
        }
        return Rectangle(position, size - position);
        
    }
    
    bool Path::contains(Point const& point, double const tolerance) const noexcept
    {
        if(m_points.size() == 1)
        {
            
        }
        else if(m_points.size() > 1)
        {
            for(vector<Node>::size_type i = 0; i < m_points.size(); i++)
            {
                
            }
        }
        return false;
    }
    
    bool Path::near(Point const& pt, double const distance) const noexcept
    {
        if(m_points.size() == 1)
        {
            return pt.near(m_points[0].point, distance);
        }
        else if(m_points.size() > 1)
        {
            Point previous;
            for(vector<Node>::size_type i = 0; i < m_points.size(); i++)
            {
                Point current = m_points[i].point;
                switch(m_points[i].mode)
                {
                    case Move:
                        if(pt.near(current, distance))
                        {
                            return true;
                        }
                        break;
                    case Linear:
                        if(pt.near(previous, current, distance))
                        {
                            return true;
                        }
                        break;
                    case Quadratic:
                        i++;
                        if(i < m_points.size())
                        {
                            const Point ctrl = current;
                            current = m_points[i].point;
                            if(pt.near(previous, ctrl, current, distance))
                            {
                                return true;
                            }

                        }
                        break;
                    case Cubic:
                        i += 2;
                        if(i < m_points.size())
                        {
                            const Point ctrl1 = current;
                            const Point ctrl2 = m_points[i-1].point;
                            current = m_points[i].point;
                            if(pt.near(previous, ctrl1, ctrl2, current, distance))
                            {
                                return true;
                            }
                            
                        }
                        break;
                        
                    default:
                        break;
                }
                previous = current;
            }
        }
        return false;
    }
    
    
    bool Path::overlaps(Rectangle const& rect) const noexcept
    {
        if(m_points.size() == 1)
        {
            return rect.contains(m_points[0].point);
        }
        else if(m_points.size() > 1)
        {
            Point previous;
            for(vector<Node>::size_type i = 0; i < m_points.size(); i++)
            {
                Point current = m_points[i].point;
                switch(m_points[i].mode)
                {
                    case Move:
                        if(rect.contains(current))
                        {
                            return true;
                        }
                        break;
                    case Linear:
                        if(rect.overlaps(previous, current))
                        {
                            return true;
                        }
                        break;
                    case Quadratic:
                        i++;
                        if(i < m_points.size())
                        {
                            const Point ctrl = current;
                            current = m_points[i].point;
                            if(rect.overlaps(previous, ctrl, current))
                            {
                                return true;
                            }
                            
                        }
                        break;
                    case Cubic:
                        i += 2;
                        if(i < m_points.size())
                        {
                            const Point ctrl1 = current;
                            const Point ctrl2 = m_points[i-1].point;
                            current = m_points[i].point;
                            if(rect.overlaps(previous, ctrl1, ctrl2, current))
                            {
                                return true;
                            }
                            
                        }
                        break;
                        
                    default:
                        break;
                }
                previous = current;
            }
        }
        return false;
    }
    
}