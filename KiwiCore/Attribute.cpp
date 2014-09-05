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

#include "Attribute.h"

namespace Kiwi
{
    void Attribute::setSize(int size)
    {
        if(size <= 0)
            m_elements.clear();
        else
            m_elements.resize(size);
        m_size = (int)m_elements.size();
    }
    
    void Attribute::setLabel(shared_ptr<Tag> label) noexcept
    {
        m_label = label;
    }
    
    void Attribute::setStyle(shared_ptr<Tag> style) noexcept
    {
        m_style = style;
    }
    
    void Attribute::setCategory(shared_ptr<Tag> category) noexcept
    {
        m_category = category;
    }
    
    void Attribute::setOpaque(bool opaque) noexcept
    {
        m_opaque = opaque;
    }
    
    void Attribute::setVisible(bool visible) noexcept
    {
        m_visible = visible;
    }
    
    void Attribute::setSave(bool save) noexcept
    {
        m_save = save;
    }
    
    void Attribute::setMinimum(string value)
    {
        if(value.length() && isdigit(value[0]))
        {
            m_minimum = atof(value.c_str());
            m_minlimits = true;
            if(m_maxlimits && m_minimum > m_maximum)
            {
                double temp = m_minimum;
                m_minimum = m_maximum;
                m_maximum = temp;
            }
            doClip();
        }
        else
        {
            m_minlimits = false;
        }
    }
    
    void Attribute::setMaximum(string  value)
    {
        if(value.length() && isdigit(value[0]))
        {
            m_maximum = atof(value.c_str());
            m_maxlimits = true;
            if(m_minlimits && m_minimum > m_maximum)
            {
                double temp = m_minimum;
                m_minimum = m_maximum;
                m_maximum = temp;
            }
            doClip();
        }
        else
        {
            m_maxlimits = false;
        }
    }
    
    void Attribute::setSetter(MethodElements method)
    {
        m_setter = method;
    }
    
    void Attribute::setGetter(MethodElements method)
    {
        m_getter = method;
    }
    
    void Attribute::setDefaultValues(const vector<Element>& elements)
    {
        m_default = elements;
    }
    
}
















