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

#include "Inlet.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     INLET                                        //
    // ================================================================================ //
    
    Box::Inlet::Inlet(const shared_ptr<Object> owner, int index) :
    m_owner(owner), m_index(index), m_description(string("")), m_polarity(1)
    {
        
    }
    
    Box::Inlet::~Inlet()
    {
        ;
    }

    weak_ptr<Object> Box::Inlet::owner() const noexcept
    {
        return m_owner;
    }
    
    int Box::Inlet::index() const noexcept
    {
        return m_index;
    }
    
    string Box::Inlet::description() const noexcept
    {
        return m_description;
    }
    
    bool  Box::Inlet::polarity() const noexcept
    {
        return m_polarity;
    }
    
    void Box::Inlet::receiveBang() const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(owner->m_tag_bang);
        }
    }
    
    void Box::Inlet::receiveLong(long value) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(owner->m_tag_long, value);
        }
    }
    
    void Box::Inlet::receiveDouble(double value) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(owner->m_tag_double, value);
        }
    }
    
    void Box::Inlet::receiveTag(shared_ptr<Tag> tag) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(owner->m_tag_tag, tag);
        }
    }
    
    void Box::Inlet::receiveElements(vector<Element>& elements) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(owner->m_tag_elements, elements);
        }
    }
    
    void Box::Inlet::receiveOther(const shared_ptr<Tag> name, long value) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(name, value);
        }
    }
    
    void Box::Inlet::receiveOther(const shared_ptr<Tag> name, double value) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(name, value);
        }
    }
    
    void Box::Inlet::receiveOther(const shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(name, value);
        }
    }
    
    void Box::Inlet::receiveOther(const shared_ptr<Tag> name, vector<Element>& elements) const noexcept
    {
        shared_ptr<Box> owner = static_pointer_cast<Box>(m_owner.lock());
        if(owner)
        {
            owner->m_last_inlet = m_index;
            owner->callMethod(name, elements);
        }
    }
}

















