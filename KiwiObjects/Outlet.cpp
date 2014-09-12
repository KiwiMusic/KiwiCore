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

#include "Outlet.h"
#include "Inlet.h"

namespace Kiwi
{
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
}

















