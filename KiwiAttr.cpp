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

#include "KiwiAttr.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    Attr::Attr(string const& name, string const& label, string const& category, const ulong behavior, const ulong order) noexcept : m_name(name), m_label(label), m_category(category), m_order(order), m_behavior(0 | behavior), m_frozen(false)
    {
        ;
    }
    
    Attr::~Attr() noexcept
    {
        ;
    }
    
    void Attr::setBehavior(const ulong behavior) noexcept
    {
        if(m_behavior != behavior)
        {
            m_behavior = behavior;
        }
        notify();
    }
    
    void Attr::setInvisible(const bool b) noexcept
    {
        if(b)
        {
            m_behavior &= ~Invisible;
        }
        else
        {
            m_behavior |= Invisible;
        }
        notify();
    }
    
    void Attr::setDisabled(const bool b) noexcept
    {
        if(!b)
        {
            m_behavior &= ~Disabled;
        }
        else
        {
            m_behavior |= Disabled;
        }
        notify();
    }
    
    void Attr::setSaved(const bool b) noexcept
    {
        if(!b)
        {
            m_behavior &= ~Unsaved;
        }
        else
        {
            m_behavior |= Unsaved;
        }
        notify();
    }
    
    void Attr::setNotifier(const bool b) noexcept
    {
        if(!b)
        {
            m_behavior &= ~Notifier;
        }
        else
        {
            m_behavior |= Notifier;
        }
        notify();
    }
    
    void Attr::addListener(sListener listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            m_lists.insert(listener);
        }
    }
    
    void Attr::removeListener(sListener listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            m_lists.erase(listener);
        }
    }
    
    void Attr::notify() noexcept
    {
        sManager manager = getManager();
        if(manager)
        {
            manager->notify(shared_from_this());
        }
        lock_guard<mutex> guard(m_lists_mutex);
        auto it = m_lists.begin();
        while(it != m_lists.end())
        {
            if((*it).expired())
            {
                it = m_lists.erase(it);
            }
            else
            {
                sListener listener = (*it).lock();
                listener->notify(shared_from_this());
                ++it;
            }
        }
    }
    
    // ================================================================================ //
    //                                 ATTRIBUTE MANAGER								//
    // ================================================================================ //
    
    
    Attr::Manager::Manager() noexcept
    {
        ;
    }
    
    Attr::Manager::~Manager()
    {
        m_attrs.clear();
    }
    
    void Attr::Manager::initialize() noexcept
    {
        for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
        {
            it->second->m_manager = shared_from_this();
        }
    }
    
    void Attr::Manager::write(Dico& dico) const
    {
        for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
        {
            dico[Tag::create(it->first)] = it->second->get();
        }
    }
    
    void Attr::Manager::addListener(sListener listener, vector<sTag> const& names)
    {
        
    }
    
    void Attr::Manager::removeListener(sListener listener, vector<sTag> const& names)
    {
        
    }
    
    // ================================================================================ //
    //                                      DEFAULTS                                    //
    // ================================================================================ //    
    
    void LongValue::set(Vector const& vector)
    {
        if(!vector.empty() && vector[0].isNumber())
        {
            m_value = (long)vector[0];
        }
    }
    
    Vector LongValue::get() const noexcept
    {
        return {m_value};
    }
    
    void DoubleValue::set(Vector const& vector)
    {
        if(!vector.empty() && vector[0].isNumber())
        {
            m_value = (double)vector[0];
        }
    }
    
    Vector DoubleValue::get() const noexcept
    {
        return {m_value};
    }
    
    void StringValue::set(Vector const& vector)
    {
        if(!vector.empty() && vector[0].isTag())
        {
            m_value = sTag(vector[0])->getName();
        }
    }
    
    Vector StringValue::get() const noexcept
    {
        return {m_value};
    }
}
















