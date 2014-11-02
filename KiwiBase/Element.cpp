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
#include "Box.h"
#include "Dico.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    
    Element::Element() noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = 0;
    }
    
    Element::Element(int value) noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = value;
    }
    
    Element::Element(long value) noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = value;
    }
    
    Element::Element(size_t value) noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = value;
    }
    
    Element::Element(float value) noexcept
    {
        m_type = Element::DOUBLE;
        m_val.m_double = value;
    }
    
    Element::Element(double value) noexcept
    {
        m_type = Element::DOUBLE;
        m_val.m_double = value;
    }
    
    Element::Element(string const& tag) noexcept
    {
        m_type = Element::TAG;
        m_val.m_tag = Tag::create(tag);
    }
    
    Element::Element(sTag tag) noexcept
    {
        m_type = Element::TAG;
        m_val.m_tag = tag;
    }
    
    Element::Element(shared_ptr<Box> box) noexcept
    {
        m_type = Element::BOX;
        m_val.m_box = box;
    }
    
    Element::Element(shared_ptr<Dico> dico) noexcept
    {
        m_type = Element::DICO;
        m_val.m_dico = dico;
    }

    Element::~Element() noexcept
    {
        ;
    }
    
    Element::operator int() const noexcept
    {
        if(m_type == Element::LONG)
            return (int)m_val.m_long;
        else if(m_type == Element::DOUBLE)
            return (int)m_val.m_double;
        else
            return 0;
    }
    
    Element::operator long() const noexcept
    {
        if(m_type == Element::LONG)
            return m_val.m_long;
        else if(m_type == Element::DOUBLE)
            return (long)m_val.m_double;
        else
            return 0;
    }
    
    Element::operator size_t() const noexcept
    {
        if(m_type == Element::LONG)
            return m_val.m_long;
        else if(m_type == Element::DOUBLE)
            return (long)m_val.m_double;
        else
            return 0;
    }
    
    Element::operator float() const noexcept
    {
        if(m_type == Element::DOUBLE)
            return (float)m_val.m_double;
        else if(m_type == Element::LONG)
            return (float)m_val.m_long;
        else
            return 0;
    }
    
    Element::operator double() const noexcept
    {
        if(m_type == Element::DOUBLE)
            return m_val.m_double;
        else if(m_type == Element::LONG)
            return (double)m_val.m_long;
        else
            return 0;
    }
    
    Element::operator sTag() const noexcept
    {
        return m_val.m_tag;
    }
    
    Element::operator shared_ptr<Box>() const noexcept
    {
        return m_val.m_box;
    }
    
    Element::operator shared_ptr<Dico>() const noexcept
    {
        return m_val.m_dico;
    }
    
    Element& Element::operator=(const Element& other) noexcept
    {
        m_type = other.m_type;
        if(m_type == Element::LONG)
            m_val.m_long = other.m_val.m_long;
        else if(m_type == Element::DOUBLE)
            m_val.m_double = other.m_val.m_double;
        else if(m_type == Element::TAG)
            m_val.m_tag = other.m_val.m_tag;
            else
        m_val.m_box = other.m_val.m_box;
            return *this;
    }
    
    Element& Element::operator=(const int value) noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = value;
        return *this;
    }
    
    Element& Element::operator=(const long value) noexcept
    {
        m_type = Element::LONG;
        m_val.m_long = value;
        return *this;
    }
    
    Element& Element::operator=(const float value) noexcept
    {
        m_type  = Element::DOUBLE;
        m_val.m_double = value;
        return *this;
    }
    
    Element& Element::operator=(const double value) noexcept
    {
        m_type  = Element::DOUBLE;
        m_val.m_double = value;
        return *this;
    }
    
    Element& Element::operator=(string const& tag) noexcept
    {
        m_type  = Element::TAG;
        m_val.m_tag = Tag::create(tag);
        return *this;
    }
    
    Element& Element::operator=(sTag tag) noexcept
    {
        m_type  = Element::TAG;
        m_val.m_tag = tag;
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Box> box) noexcept
    {
        m_type  = Element::BOX;
        m_val.m_box = box;
        return *this;
    }
    
    Element& Element::operator=(shared_ptr<Dico> dico) noexcept
    {
        m_type  = Element::DICO;
        m_val.m_dico = dico;
        return *this;
    }
    
    bool Element::operator==(const Element& other) const noexcept
    {
        if(m_type == other.m_type)
        {
            if(m_type == Element::LONG)
            {
                return m_val.m_long == other.m_val.m_long;
            }
            else if(m_type == Element::DOUBLE)
            {
                return  m_val.m_double == other.m_val.m_double;
            }
            else if(m_type == Element::TAG)
            {
                return m_val.m_tag == other.m_val.m_tag;
            }
            else if(m_type == Element::BOX)
            {
                return m_val.m_box == other.m_val.m_box;
            }
            else
            {
                return m_val.m_dico == other.m_val.m_dico;
            }
        }
        return false;
    }
    
    bool Element::operator==(const int value) const noexcept
    {
        return m_type == Element::LONG && m_val.m_long == (long)value;
    }
    
    bool Element::operator==(const long value) const noexcept
    {
        return m_type == Element::LONG && m_val.m_long == value;
    }
    
    bool Element::operator==(const float value) const noexcept
    {
        return m_type == Element::LONG && m_val.m_double == (double)value;
    }
    
    bool Element::operator==(const double value) const noexcept
    {
        return m_type == Element::LONG && m_val.m_double == value;
    }
    
    bool Element::operator==(string const& tag) const noexcept
    {
        return m_type == Element::TAG && m_val.m_tag == Tag::create(tag);
    }
    
    bool Element::operator==(sTag tag) const noexcept
    {
        return m_type == Element::TAG && m_val.m_tag == tag;
    }

    bool Element::operator==(shared_ptr<Box> box) const noexcept
    {
        return m_type == Element::BOX && m_val.m_box == box;
    }
    
    bool Element::operator==(shared_ptr<Dico> dico) const noexcept
    {
        return m_type == Element::DICO && m_val.m_dico == dico;
    }
    
    string toString(Element const& __val)
    {
        switch(__val.type())
        {
            case Element::LONG:
                return to_string((long)__val);
                break;
            case Element::DOUBLE:
                return to_string((double)__val);
                break;
            case Element::TAG:
                return toString((sTag)__val);
                break;
            case Element::BOX:
                return toString(shared_ptr<const Box>((sBox)__val));
                break;
            case Element::DICO:
                return toString((sDico)__val);
                break;
            default:
                return "";
                break;
        }
    }
    
    string toString(ElemVector const& __val)
    {
        if(__val.size())
        {
            string desc;
            for(size_t i = 0; i < __val.size() - 1; i++)
            {
                desc += toString(__val[i]) + " ";
            }
            desc += toString(__val[__val.size() - 1]);
            return desc;
        }
        return "";
    }
    
    string toString(Element::Type __val)
    {
        switch(__val)
        {
            case Element::NOTHING:
                return "nothing";
                break;
            case Element::LONG:
                return "long";
                break;
            case Element::DOUBLE:
                return "double";
                break;
            case Element::TAG:
                return "tag";
                break;
            case Element::BOX:
                return "box";
                break;
            case Element::DICO:
                return "dico";
                break;
            default:
                return "elements";
                break;
        }
    }
}
















