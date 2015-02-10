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

#include "Object.h"
#include "Page.h"
#include "Instance.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      IOLET                                       //
    // ================================================================================ //
    
    bool Object::Iolet::has(sObject object, ulong index) const noexcept
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sObject cbox = m_connections[i].object.lock();
                if(cbox && object == cbox && m_connections[i].index == index)
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Object::Iolet::append(sObject object, ulong index) noexcept
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sObject cbox = m_connections[i].object.lock();
                if(cbox && object == cbox && m_connections[i].index == index)
                {
                    return false;
                }
            }
            m_connections.push_back({object, index});
            return true;
        }
        return false;
    }
    
    bool Object::Iolet::erase(sObject object, ulong index) noexcept
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sObject cbox = m_connections[i].object.lock();
                if(cbox && object == cbox && m_connections[i].index == index)
                {
                    m_connections.erase(m_connections.begin() + i);
                    return false;
                }
            }
        }
        return false;
    }
    
    Object::Outlet::Outlet(Io::Type type, string const& description) noexcept :
    Iolet(type, Io::Polarity::Hot, description)
    {
        ;
    }
    
    Object::Outlet::~Outlet()
    {
        ;
    }
    
    void Object::Outlet::send(ElemVector const& elements) const noexcept
    {
        lock_guard<mutex> guard(m_mutex);
        for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
        {
            sObject receiver       = m_connections[i].object.lock();
            ulong inlet  = m_connections[i].index;
            if(receiver)
            {
                if(++receiver->m_stack_count < 256)
                {
                    receiver->receive(inlet, elements);
                }
                else if(receiver->m_stack_count  == 256)
                {
                    receiver->receive(inlet, elements);
                }
                else
                {
                    Console::error(receiver, "Stack overflow");
                }
                receiver->m_stack_count--;
            }
        }
    }
    
    Object::Inlet::Inlet(Io::Type type, Io::Polarity polarity, string const& description) noexcept :
    Iolet(type, polarity, description)
    {
        ;
    }
    
    Object::Inlet::~Inlet()
    {
        ;
    }
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    Object::Object(Initializer const& initiliazer, string const& name) :
    m_instance(initiliazer.instance),
    m_page(initiliazer.page),
    m_name(Tag::create(name)),
    m_text(initiliazer.text),
    m_id(initiliazer.lid),
    m_stack_count(0),
    m_presentation_position(Attr::create("presentation_position","Presentation Position",    "Appearance", Point(0., 0.))),
    m_presentation_size(Attr::create("presentation_size",        "Presentation Size",        "Appearance", Size(10., 10.))),
    m_hidden(Attr::create("hidden",                              "Hide on Lock",             "Appearance",   false)),
    m_presentation(Attr::create("presentation",                  "Include in presentation",  "Appearance",   false)),
    m_ignoreclick(Attr::create("ignoreclick",                    "Ignore Click",             "Behavior",     false))
    {
        addAttr(m_presentation_position);
        addAttr(m_presentation_size);
        addAttr(m_hidden);
        addAttr(m_presentation);
        addAttr(m_ignoreclick);
    }
    
    Object::~Object()
    {
        m_outlets.clear();
        m_inlets.clear();
    }
    
    void Object::write(sDico dico) const
    {
        this->save(dico);
        dico->set(Tag::List::name, getName());
        dico->set(Tag::List::text, getText());
        dico->set(Tag::List::id, getId());
        dico->set(Tag::List::ninlets, getNumberOfInlets());
        dico->set(Tag::List::ninlets, getNumberOfOutlets());
        vector<sAttr> attrs;
        getAttrs(attrs);
        for(vector<sAttr>::size_type i = 0; i < attrs.size(); i++)
        {
            dico->set(Tag::create(attrs[i]->getName()), Tag::create(attrs[i]->getValueAsString()));
        }
    }
    
    void Object::send(ulong index, ElemVector const& elements) const noexcept
    {
        m_mutex.lock();
        if(index < m_outlets.size())
        {
            m_outlets[(vector<sOutlet>::size_type)index]->send(elements);
        }
        m_mutex.unlock();
    }

    void Object::addInlet(Io::Type type, Io::Polarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
        sInlet inlet = make_shared<Inlet>(type, polarity, description);
        if(inlet)
        {
            m_inlets.push_back(inlet);
        }
    }
    
    void Object::addOutlet(Io::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
        sOutlet outlet = make_shared<Outlet>(type, description);
        if(outlet)
        {
            m_outlets.push_back(outlet);
        }
    }
}















