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
#include "Instance.h"

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
}
















