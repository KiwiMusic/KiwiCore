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

#ifndef __DEF_KIWI_COLOR__
#define __DEF_KIWI_COLOR__

#include "Element.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      COLOR                                       //
    // ================================================================================ //
    
    class Color
    {
    private:
        double m_red;
        double m_green;
        double m_blue;
        double m_alpha;
        
    public:
        
        Color() noexcept :
        m_red(0.), m_green(0.), m_blue(0.), m_alpha(1.)
        {
            ;
        }
        
        Color(const double red, const double green, const double blue, const double alpha = 1.) noexcept :
        m_red(clip(red, 0., 1.)), m_green(clip(green, 0., 1.)), m_blue(clip(blue, 0., 1.)), m_alpha(clip(alpha, 0., 1.))
        {
            ;
        }
        
        Color(ElemVector const& elements) noexcept :
        m_red(0.), m_green(0.), m_blue(0.), m_alpha(1.)
        {
			const ElemVector::size_type size = elements.size();
			if(size && elements[0].isNumber())
			{
				m_red = clip((double)elements[0], 0., 1.);
			}
			if(size > 1 && elements[1].isNumber())
			{
				m_green = clip((double)elements[1], 0., 1.);
			}
			if(size > 2 && elements[2].isNumber())
			{
				m_blue = clip((double)elements[2], 0., 1.);
			}
			if(size > 2 && elements[3].isNumber())
			{
				m_alpha = clip((double)elements[3], 0., 1.);
			}
        }
		
        Color(Color const& color) noexcept :
        m_red(color.m_red), m_green(color.m_green), m_blue(color.m_blue), m_alpha(color.m_alpha)
        {
            ;
        }
        
        ~Color()
        {
            ;
        }
        
        double red() const noexcept
        {
            return m_red;
        }
        
        double green() const noexcept
        {
            return m_green;
        }
        
        double blue() const noexcept
        {
            return m_blue;
        }
        
        double alpha() const noexcept
        {
            return m_alpha;
        }
        
        void red(const double value) noexcept
        {
            m_red = clip(value, 0., 1.);
        }
        
        void green(const double value) noexcept
        {
            m_green = clip(value, 0., 1.);
        }
        
        void blue(const double value) noexcept
        {
            m_blue = clip(value, 0., 1.);
        }
        
        void alpha(const double value) noexcept
        {
            m_alpha = clip(value, 0., 1.);
        }
        
        inline Color& operator=(ElemVector const& elements) noexcept
        {
            const ElemVector::size_type size = elements.size();
            if(size && elements[0].isNumber())
            {
                m_red = clip((double)elements[0], 0., 1.);
            }
            if(size > 1 && elements[1].isNumber())
            {
                m_green = clip((double)elements[1], 0., 1.);
            }
            if(size > 2 && elements[2].isNumber())
            {
                m_blue = clip((double)elements[2], 0., 1.);
            }
            if(size > 2 && elements[3].isNumber())
            {
                m_alpha = clip((double)elements[3], 0., 1.);
            }
            return *this;
        }
        
        inline Color& operator=(Color const& color) noexcept
        {
            m_red = color.m_red;
            m_green = color.m_green;
            m_blue = color.m_blue;
            m_alpha = color.m_alpha;
            return *this;
        }
        
        inline operator ElemVector() const noexcept
        {
            return {m_red, m_green, m_blue, m_alpha};
        }
        
        inline Color brighter(const double value) const noexcept
        {
            return Color(red() + value, green() + value, blue() + value, alpha());
        }
        
        inline Color darker(const double value) const noexcept
        {
            return Color(red() - value, green() - value, blue() - value, alpha());
        }
    };
}

#endif