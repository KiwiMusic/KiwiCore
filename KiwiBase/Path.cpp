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
        m_points.push_back(pt);
    }
    
    Path::Path(ElemVector const& elements) noexcept
    {
        for(ElemVector::size_type i = 0; i < elements.size(); i += 2)
        {
            if(i+1 < elements.size())
            {
                m_points.push_back(Point((double)elements[i], (double)elements[i+1]));
            }
        }
    }
    
    Path::~Path()
    {
        m_points.clear();
    }
    
    void Path::add(const Kiwi::Point &pt) noexcept
    {
        m_points.push_back(pt);
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
            position = m_points[0];
            
            for(vector<Point>::size_type i = 1; i < m_points.size(); i++)
            {
                if(m_points[i].x() < position.x())
                {
                    position.x(m_points[i].x());
                }
                if(m_points[i].y() < position.y())
                {
                    position.y(m_points[i].y());
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
            position = m_points[0];
            size = m_points[0];
            
            for(vector<Point>::size_type i = 1; i < m_points.size(); i++)
            {
                if(m_points[i].x() < position.x())
                {
                    position.x(m_points[i].x());
                }
                else if(m_points[i].x() > size.x())
                {
                    size.x(m_points[i].x());
                }
                
                if(m_points[i].y() < position.y())
                {
                    position.y(m_points[i].y());
                }
                else if(m_points[i].y() > size.y())
                {
                    size.y(m_points[i].y());
                }
            }
        }
        return Rectangle(position, size - position);
        
    }
    
    bool Path::contains(Point const& pt) const noexcept
    {
        
    }
}