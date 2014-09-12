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
#include "Dico.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    Connection::Connection(const shared_ptr<Box> from, int outlet, const shared_ptr<Box> to, int inlet) noexcept :
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
    
    bool Connection::isDsp() const noexcept
    {
        shared_ptr<Box> from    = m_from.lock();
        shared_ptr<Box> to      = m_to.lock();
        if(from && to)
        {
            return from->isOutletSignal(m_outlet) && to->isInletSignal(m_inlet);
        }
        else
        {
            return false;
        }
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
    
    void Connection::write(shared_ptr<Dico> dico)
    {
        shared_ptr<Box> from = m_from.lock();
        shared_ptr<Box> to      = m_to.lock();
        if(from && to)
        {
            vector<Element> elements;
            from->callMethod("getid", elements);
            shared_ptr<Tag> idfrom = (shared_ptr<Tag>)elements[0];
            to->callMethod("getid", elements);
            shared_ptr<Tag> idto = (shared_ptr<Tag>)elements[0];
            if(idfrom && idto)
            {
                dico->set("from", idfrom);
                dico->set("to", idto);
                dico->set("outlet", m_outlet);
                dico->set("inlet", m_inlet);
            }
        }
    }
    
    bool Connection::operator==(const Connection& other) noexcept
    {
        if(!(other.m_to.lock()))        // Compare only the input connections
        {
            if(other.m_inlet < 0)       // Compare only the input objects
            {
                return this->m_from.lock() == other.m_from.lock();
            }
            else                        // Compare the input objects and their outlets
            {
                return this->m_from.lock() == other.m_from.lock() && this->m_outlet == other.m_outlet;
            }
        }
        else if(!(other.m_from.lock())) // Compare only the output connections
        {
            if(other.m_outlet < 0)      // Compare only the output objects
            {
                return this->m_to.lock() == other.m_to.lock();
            }
            else                        // Compare the output objects and their inlets
            {
                return this->m_to.lock() == other.m_to.lock() && this->m_inlet == other.m_inlet;
            }
        }
        else                            // Compare everything
        {
            return this->m_from.lock() == other.m_from.lock() && this->m_outlet == other.m_outlet && this->m_to.lock() == other.m_to.lock() && this->m_inlet == other.m_inlet;
        }
    }
}

















