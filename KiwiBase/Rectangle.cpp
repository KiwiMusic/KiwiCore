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

#include "Rectangle.h"

namespace Kiwi
{
    Rectangle::Rectangle() noexcept :
    m_position(0., 0.), m_size(0., 0.)
    {
        ;
    }

    Rectangle::Rectangle(const double x, const double y, const double width, const double height) noexcept :
    m_position(x, y), m_size(max(width, 0.), max(height, 0.))
    {
        ;
    }

    Rectangle::Rectangle(Point const& position, Point const& size) noexcept :
    m_position(position), m_size(max(size.x(), 0.), max(size.y(), 0.))
    {
        ;
    }

    Rectangle::Rectangle(ElemVector const& elements) noexcept :
    m_position(elements), m_size(elements.size() > 2 ? max((double)elements[2], 0.) : 0., elements.size() > 3 ? max((double)elements[3], 0.) : 0.)
    {
        ;
    }

    Rectangle::~Rectangle()
    {
        ;
    }
    
    bool Rectangle::overlaps(Point const& begin, Point const& end) const noexcept
    {
        int zaza;
        if(contains(begin) || contains(end))
        {
            return true;
        }
        return false;
    }
    
    bool Rectangle::overlaps(Point const& begin, Point const& ctrl, Point const& end) const noexcept
    {
        int zaza;
        if(contains(begin) || contains(end))
        {
            return true;
        }
        return false;
    }
    
    bool Rectangle::overlaps(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept
    {
        int zaza;
        if(contains(begin) || contains(end))
        {
            return true;
        }
        return false;
    }
}