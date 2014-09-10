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
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    
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
    
    Element::Element(weak_ptr<ObjectExtented> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object.lock());
    }
    
    Element::Element(shared_ptr<ObjectExtented> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object);
    }
    
    Element::Element(weak_ptr<Box> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object.lock());
    }
    
    Element::Element(shared_ptr<Box> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object);
    }
    
    Element::Element(weak_ptr<Dictionary> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object.lock());
    }
    
    Element::Element(shared_ptr<Dictionary> object) noexcept
    {
        m_type = T_OBJECT;
        m_val.m_object = static_pointer_cast<Object>(object);
    }
    
    Element::~Element() noexcept
    {
        ;
    }
    
    Element& Element::operator=(const Element& other) noexcept
    {
        m_type = other.m_type;
        if(m_type == T_LONG)
            m_val.m_long = other.m_val.m_long;
        else if(m_type == T_DOUBLE)
            m_val.m_double = other.m_val.m_double;
        else if(m_type == T_TAG)
            m_val.m_tag = other.m_val.m_tag;
            else
        m_val.m_object = other.m_val.m_object;
            return *this;
    }
    
    Element& Element::operator=(int value) noexcept
    {
        m_type = T_LONG;
        m_val.m_long = value;
        return *this;
    }
    
    Element& Element::operator=(long value) noexcept
    {
        m_type = T_LONG;
        m_val.m_long = value;
        return *this;
    }
    
    Element& Element::operator=(float value) noexcept
    {
        m_type  = T_DOUBLE;
        m_val.m_double= value;
        return *this;
    }
    
    Element& Element::operator=(double value) noexcept
    {
        m_type  = T_DOUBLE;
        m_val.m_double= value;
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Tag> tag) noexcept
    {
        m_type  = T_TAG;
        m_val.m_tag= tag;
        return *this;
    }
    
    Element& Element::operator=(weak_ptr<Object> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= object.lock();
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Object> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= object;
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<ObjectExtented> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= static_pointer_cast<Object>(object);
        return *this;
    }
    
    Element& Element::operator=(weak_ptr<Box> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= static_pointer_cast<Object>(object.lock());
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Box> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= static_pointer_cast<Object>(object);
        return *this;
    }
    
    Element& Element::operator=(weak_ptr<Dictionary> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= static_pointer_cast<Object>(object.lock());
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Dictionary> object) noexcept
    {
        m_type  = T_TAG;
        m_val.m_object= static_pointer_cast<Object>(object);
        return *this;
    }
    
    bool Element::operator==(const Element& other) const noexcept
    {
        if(m_type == other.m_type)
        {
            if(m_type == T_LONG)
                return m_val.m_long == other.m_val.m_long;
            else if(m_type == T_DOUBLE)
                return  m_val.m_double == other.m_val.m_double;
            else if(m_type == T_TAG)
                return m_val.m_tag == other.m_val.m_tag;
            else
                return m_val.m_object == other.m_val.m_object;
        }
        return false;
    }
    
    Element::operator int() const noexcept
    {
        if(m_type == T_LONG)
            return (int)m_val.m_long;
        else if(m_type == T_DOUBLE)
            return (int)m_val.m_double;
        else
            return 0;
    }
    
    Element::operator long() const noexcept
    {
        if(m_type == T_LONG)
            return m_val.m_long;
        else if(m_type == T_DOUBLE)
            return (long)m_val.m_double;
        else
            return 0;
    }
    
    Element::operator float() const noexcept
    {
        if(m_type == T_DOUBLE)
            return (float)m_val.m_double;
        else if(m_type == T_LONG)
            return (float)m_val.m_long;
        else
            return 0;
    }
    
    Element::operator double() const noexcept
    {
        if(m_type == T_DOUBLE)
            return m_val.m_double;
        else if(m_type == T_LONG)
            return (double)m_val.m_long;
        else
            return 0;
    }
    
    Element::operator shared_ptr<Tag>() const noexcept
    {
        return m_val.m_tag;
    }
    
    Element::operator weak_ptr<Object>() const noexcept
    {
        return m_val.m_object;
    }
    
    Element::operator shared_ptr<Object>() const noexcept
    {
        return m_val.m_object;
    }
    
    Element::operator weak_ptr<ObjectExtented>() const noexcept
    {
        return static_pointer_cast<ObjectExtented>(m_val.m_object);
    }
    
    Element::operator shared_ptr<ObjectExtented>() const noexcept
    {
        return static_pointer_cast<ObjectExtented>(m_val.m_object);
    }
    
    Element::operator weak_ptr<Box>() const noexcept
    {
        return static_pointer_cast<Box>(m_val.m_object);
    }
    
    Element::operator shared_ptr<Box>() const noexcept
    {
        return static_pointer_cast<Box>(m_val.m_object);
    }
    
    Element::operator weak_ptr<Dictionary>() const noexcept
    {
        return static_pointer_cast<Dictionary>(m_val.m_object);
    }
    
    Element::operator shared_ptr<Dictionary>() const noexcept
    {
        return static_pointer_cast<Dictionary>(m_val.m_object);
    }
    
    bool Element::operator==(const int value) const noexcept
    {
        return m_type == T_LONG && m_val.m_long == (long)value;
    }
    
    bool Element::operator==(const long value) const noexcept
    {
        return m_type == T_LONG && m_val.m_long == value;
    }
    
    bool Element::operator==(const float value) const noexcept
    {
        return m_type == T_LONG && m_val.m_double == (double)value;
    }
    
    bool Element::operator==(const double value) const noexcept
    {
        return m_type == T_LONG && m_val.m_double == value;
    }
    
    bool Element::operator==(shared_ptr<Tag> tag) const noexcept
    {
        return m_type == T_TAG && m_val.m_tag == tag;
    }
    
    bool Element::operator==(weak_ptr<Object> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == object.lock();
    }
    
    bool Element::operator==(shared_ptr<Object> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == object;
    }
    
    bool Element::operator==(weak_ptr<ObjectExtented> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object.lock());
    }
    
    bool Element::operator==(shared_ptr<ObjectExtented> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object);
    }
    
    bool Element::operator==(weak_ptr<Box> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object.lock());
    }
    
    bool Element::operator==(shared_ptr<Box> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object);
    }
    
    bool Element::operator==(weak_ptr<Dictionary> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object.lock());
    }
    
    bool Element::operator==(shared_ptr<Dictionary> object) const noexcept
    {
        return m_type == T_OBJECT && m_val.m_object == static_pointer_cast<Object>(object);
    }
}
















