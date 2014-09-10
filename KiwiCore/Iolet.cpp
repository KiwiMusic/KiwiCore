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

#include "Iolet.h"
#include "Box.h"
#include "Instance.h"

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
    
    // ================================================================================= //
    //                                     OUTLET                                        //
    // ================================================================================= //
    
    Box::Outlet::Outlet(const weak_ptr<Object> owner, int index) :
    m_owner(owner), m_index(index), m_description(string(""))
    {
        
    }
    
    Box::Outlet::~Outlet()
    {
        m_inlets.clear();
    }
    
    weak_ptr<Object> Box::Outlet::owner() const noexcept
    {
        return m_owner;
    }
    
    int Box::Outlet::index() const noexcept
    {
        return m_index;
    }
    
    std::string Box::Outlet::description() const noexcept
    {
        return m_description;
    }
    
    void Box::Outlet::sendBang() const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveBang();
    }
    
    void Box::Outlet::sendLong(long value) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveLong(value);
    }
    
    void Box::Outlet::sendDouble(double value) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveDouble(value);
    }
    
    void Box::Outlet::sendTag(shared_ptr<Tag> tag) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveTag(tag);
    }
    
    void Box::Outlet::sendElements(vector<Element>& elements) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveElements(elements);
    }
    
    void Box::Outlet::sendOther(const shared_ptr<Tag> name, long value) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveOther(name, value);
    }
    
    void Box::Outlet::sendOther(const shared_ptr<Tag> name, double value) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveOther(name, value);
    }
    
    void Box::Outlet::sendOther(const shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveOther(name, value);
    }
    
    void Box::Outlet::sendOther(const shared_ptr<Tag> name, vector<Element>& elements) const noexcept
    {
        for(set<shared_ptr<Box::Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
            (*it)->receiveOther(name, elements);
    }
    
    bool Box::Outlet::compatible(const shared_ptr<Box::Inlet> inlet)
    {
        if(inlet->owner().lock() == m_owner.lock())
        {
            return false;
        }
        else
        {
            shared_ptr<Tag> garbage_tag = (m_owner.lock())->m_tag_garbage;
            shared_ptr<Tag> signal_tag = (m_owner.lock())->m_tag_garbage;
            for(set<shared_ptr<Tag>>::iterator it = m_names.begin(); it != m_names.end(); ++it)
            {
                bool garbage = ((*it) == garbage_tag);
                for(set<shared_ptr<Tag>>::iterator it2 = inlet->m_names.begin(); it2 != inlet->m_names.end(); ++it2)
                {
                    if((*it) == (*it2) || (garbage && (*it2) != signal_tag))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    }
    
    bool Box::Outlet::connect(const shared_ptr<Box::Inlet> inlet)
    {
        if(compatible(inlet))
        {
            m_inlets.insert(inlet);
            return true;
        }
        else
            return false;
    }
    
    void Box::Outlet::disconnect(const shared_ptr<Box::Inlet> inlet)
    {
        m_inlets.erase(inlet);
    }
    
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    Connection::Connection(const shared_ptr<Instance> kiwi, const shared_ptr<Box> from, int outlet, const shared_ptr<Box> to, int inlet) :
    m_kiwi(kiwi),
    m_from(from),
    m_outlet(outlet),
    m_to(to),
    m_inlet(inlet)
    {
        
    }
    
    Connection::~Connection()
    {
        ;
    }
    
    bool Connection::isDsp()
    {
        return m_from.lock()->isOutletSignal(m_outlet) && m_to.lock()->isInletSignal(m_inlet);
    }
    
    shared_ptr<Box> Connection::getFrom()
    {
        return m_from.lock();
    }
    
    int Connection::getOutletIndex()
    {
        return m_outlet;
    }
    
    shared_ptr<Box> Connection::getTo()
    {
        return m_to.lock();
    }
    
    int Connection::getInletIndex()
    {
        return m_inlet;
    }
    
    void Connection::write(shared_ptr<Dictionary> dico)
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            shared_ptr<Box> box;
            if((box = m_from.lock()))
            {
                shared_ptr<Tag> tag = (shared_ptr<Tag>)box->getAttributeValue(kiwi->createTag("id"));
                if(tag)
                    dico->set(kiwi->createTag("from"), tag);
                else
                    dico->set(kiwi->createTag("from"), kiwi->createTag("id-0"));
            }
            else
                dico->set(kiwi->createTag("from"), kiwi->createTag("id-0"));
            
            if((box = m_to.lock()))
            {
                shared_ptr<Tag> tag = (shared_ptr<Tag>)box->getAttributeValue(kiwi->createTag("id"));
                if(tag)
                    dico->set(kiwi->createTag("to"), tag);
                else
                    dico->set(kiwi->createTag("to"), kiwi->createTag("id-0"));
            }
            else
                dico->set(kiwi->createTag("to"), kiwi->createTag("id-0"));
            
            dico->set(kiwi->createTag("outlet"), m_outlet);
            dico->set(kiwi->createTag("inlet"), m_inlet);
        }
        
    }
}

















