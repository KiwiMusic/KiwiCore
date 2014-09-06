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

#include "Element.h"
#include "Object.h"

namespace Kiwi
{
    Element::Element() noexcept
    {
        m_type = T_LONG;
        m_val.m_long = 0;
    }
    
    Element::Element(int value) noexcept
    {
        m_type = T_LONG;
        m_val.m_long = value;
    }
    
    Element::Element(long value) noexcept
    {
        m_type = T_LONG;
        m_val.m_long = value;
    }
    
    Element::Element(float value) noexcept
    {
        m_type = T_DOUBLE;
        m_val.m_double = value;
    }
    
    Element::Element(double value) noexcept
    {
        m_type = T_DOUBLE;
        m_val.m_double = value;
    }
    
    Element::Element(shared_ptr<Tag> tag) noexcept
    {
        m_type = T_TAG;
        m_val.m_tag = tag;
    }
    
    Element::Element(weak_ptr<Object> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = object.lock();
    }
    
    Element::Element(shared_ptr<Object> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = object;
    }
    
    Element::~Element() noexcept
    {
        ;
    }
    
    void Element::write(ofstream* file, int indent)
    {
        if(m_type == T_LONG)
            (*file) << m_val.m_long;
        else if(m_type == T_DOUBLE)
            (*file) << m_val.m_double;
        else if(m_type == T_TAG)
            (*file) << "\"" << m_val.m_tag->name() << "\"";
        else if(m_val.m_object)
        {
            if(m_val.m_object->hasMethod("write"))
            {
                m_val.m_object->callMethodOpaque("write", 2, file, indent);
            }
        }
    }
    
    string Element::getString() const noexcept
    {
        if(isLong())
            return to_string(m_val.m_long);
        else if(isDouble())
            return to_string(m_val.m_double);
        else if(isTag())
            return m_val.m_tag->name();
        else
            return m_val.m_object->getName()->name();
    }
}
















