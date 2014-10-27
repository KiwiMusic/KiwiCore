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

#include "Box.h"
#include "Connection.h"
#include "Page.h"
#include "Dico.h"

namespace Kiwi
{
    sTag tagNinlets     = make_shared<Tag>("ninlets");
    sTag tagNoutlets    = make_shared<Tag>("noutlets");
    sTag tagId          = make_shared<Tag>("id");
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    Box::Box(shared_ptr<Instance> kiwi, string const& name) : Object(kiwi, name), AttributeFactory(),
    m_last_inlet(0)
    {
        addAttribute(make_shared<AttributeTag>(createTag("fontname"), "Font Name", "Tag", "Appearance", Attribute::Visible | Attribute::Saved | Attribute::Notify));
    }
    
    Box::~Box()
    {
        m_outlets.clear();
        m_inlets.clear();
    }
    
    Object::Type Box::type() const noexcept
    {
        return Object::BOX;
    }
    
    void Box::write(shared_ptr<Dico> dico) const noexcept
    {
        Object::write(dico);
        AttributeFactory::write(dico);
        dico->set(tagNinlets, (long)m_inlets.size());
        dico->set(tagNoutlets, (long)m_outlets.size());
        dico->set(tagId, (long)m_id);
    }
    
    void Box::read(shared_ptr<const Dico> dico) noexcept
    {
        Object::read(dico);
        AttributeFactory::read(dico);
    }
    
    void Box::receive(ElemVector const& elements)
    {
        AttributeFactory::receive(elements);
    }
    
    // ================================================================================ //
    //                                      INLETS                                      //
    // ================================================================================ //
    
    void Box::addInlet(Inlet::Type type, string const& description)
    {
        shared_ptr<Inlet> inlet = make_shared<Inlet>(type);
        inlet->setIndex(m_inlets.size());
        m_inlets.push_back(inlet);
    }
    
    shared_ptr<Inlet> Box::getInlet(size_t index) const noexcept
    {
        if(index < m_inlets.size())
        {
            return m_inlets[index];
        }
        else
        {
            return nullptr;
        }
    }
    
    size_t Box::getInletIndex()
    {
        return m_last_inlet;
    }
    
    void Box::removeInlet(size_t index)
    {
        if(index < m_inlets.size())
        {
            shared_ptr<Page> page = m_page.lock();
            if(page)
                page->disconnect(nullptr, 0, static_pointer_cast<Box>(shared_from_this()), (int)index);
            
            for(size_t i = index+1; i < m_inlets.size(); i++)
            {
                shared_ptr<Inlet> inlet = m_inlets[i];
                if(inlet)
                    inlet->setIndex(i-1);
            }
            m_inlets.erase(m_inlets.begin()+index);
        }
    }
    
    size_t Box::getNumberOfInlets() const noexcept
    {
        return m_inlets.size();
    }

    void Box::receive(size_t index, ElemVector const& values)
    {
        ++m_stack_count;
        if(m_stack_count < 256)
        {
            m_last_inlet = index;
            Box::receive(values);
        }
        else if(m_stack_count == 256)
        {
            error(shared_from_this(), string("Stack overflow"));
            m_last_inlet = index;
            Box::receive(values);
        }
        --m_stack_count;
    }

    
    // ================================================================================ //
    //                                      OUTLETS                                     //
    // ================================================================================ //
    
    void Box::addOutlet(Outlet::Type type, string const& description)
    {
        sOutlet outlet = make_shared<Outlet>(type);
        outlet->setIndex(m_outlets.size());
        m_outlets.push_back(outlet);
    }
    
    sOutlet Box::getOutlet(size_t index) const noexcept
    {
        if(index < m_outlets.size())
        {
            return m_outlets[index];
        }
        else
        {
            return nullptr;
        }
    }
    
    void Box::removeOutlet(size_t index)
    {
        if(index < m_outlets.size())
        {
            shared_ptr<Page> page = m_page.lock();
            if(page)
                page->disconnect(static_pointer_cast<Box>(shared_from_this()), (int)index, nullptr, 0);
            
            for(size_t i = index+1; i < m_outlets.size(); i++)
                m_outlets[i]->setIndex(i-1);
            m_outlets.erase(m_outlets.begin()+index);
        }
    }

    void Box::send(size_t index, ElemVector const& value) const noexcept
    {
        if(index < m_outlets.size())
        {
            sOutlet outlet = m_outlets[index];
            for(map<sInlet, sBox>::iterator it = outlet->m_inlets.begin(); it != outlet->m_inlets.end(); ++it)
            {
                it->second->receive(it->first->index(), value);
            }
        }
    }
    
    size_t Box::getNumberOfOutlets() const noexcept
    {
        return m_outlets.size();
    }
}
















