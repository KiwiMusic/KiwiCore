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

#include "Connection.h"
#include "Box.h"
#include "Dico.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     INLET                                        //
    // ================================================================================ //
    
    Inlet::Inlet(Type type) :
    m_flag(type),
    m_index(0)
    {
        
    }
    
    Inlet::~Inlet()
    {
        ;
    }
    
    void Inlet::setIndex(size_t index)
    {
        m_index = index;
    }
    
    size_t Inlet::index() const noexcept
    {
        return m_index;
    }
    
    Inlet::Type Inlet::type() const noexcept
    {
        return m_flag;
    }
    
    // ================================================================================= //
    //                                     OUTLET                                        //
    // ================================================================================= //
    
    Outlet::Outlet(Type type) :
    m_type(type),
    m_index(0)
    {
        
    }
    
    Outlet::~Outlet()
    {
        m_inlets.clear();
    }
    
    void Outlet::setIndex(size_t index)
    {
        m_index = index;
    }
    
    size_t Outlet::index() const noexcept
    {
        return m_index;
    }
    
    Outlet::Type Outlet::type() const noexcept
    {
        return m_type;
    }
    
    bool Outlet::has(sInlet inlet) const noexcept
    {
        return m_inlets.find(inlet) != m_inlets.end();
    }
    
    void Outlet::connect(const sInlet inlet, shared_ptr<Box> box)
    {
        m_inlets[inlet] = box;
    }
    
    void Outlet::disconnect(const sInlet inlet)
    {
        m_inlets.erase(inlet);
    }
    
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    sTag tagFrom = make_shared<Tag>("from");
    sTag tagTo = make_shared<Tag>("to");
    
    Connection::Connection(const shared_ptr<Box> from, const shared_ptr<Outlet> outlet, const shared_ptr<Box> to, const shared_ptr<Inlet> inlet) noexcept :
    m_from(from),
    m_to(to),
    m_outlet(outlet),
    m_inlet(inlet)
    {
        
    }
    
    Connection::~Connection()
    {
        ;
    }
    
    shared_ptr<Outlet> Connection::outlet()
    {
        return m_outlet.lock();
    }
    
    shared_ptr<Inlet> Connection::inlet()
    {
        return m_inlet.lock();
    }
    
    void Connection::write(shared_ptr<Dico> dico)
    {
        shared_ptr<Box>     from    = m_from.lock();
        shared_ptr<Box>     to      = m_to.lock();
        shared_ptr<Outlet>  outlet  = m_outlet.lock();
        shared_ptr<Inlet>   inlet   = m_inlet.lock();
        
        if(from && to && outlet && inlet)
        {
            dico->set(tagFrom, {from->getId(), outlet->index()});
            dico->set(tagTo, {to->getId(), inlet->index()});
        }
    }
}

















