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

#include "Link.h"
#include "Object.h"
#include "Patcher.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    Link::Link(const sPatcher patcher, const sObject from, const ulong outlet, const sObject to, const ulong inlet, const Object::Io::Type type) noexcept :
    m_patcher(patcher),
    m_object_from(from),
    m_object_to(to),
    m_index_outlet(outlet),
    m_index_intlet(inlet),
    m_type(type)
    {
        ;
    }
    
    Link::~Link()
    {
        sObject     from    = getObjectFrom();
        sObject     to      = getObjectTo();
        if(from && to)
        {
            Object::sOutlet outlet  = from->getOutlet(m_index_outlet);
            if(outlet)
            {
                outlet->erase(to, m_index_outlet);
            }
            Object::sInlet inlet    = to->getInlet(m_index_intlet);
            if(inlet)
            {
                inlet->erase(from, m_index_intlet);
            }
        }
    }
    
    void Link::write(sDico dico) const noexcept
    {
        sObject     from    = getObjectFrom();
        sObject     to      = getObjectTo();
        if(from && to)
        {
            dico->set(Tag::List::from, {from->getId(), getOutletIndex()});
            dico->set(Tag::List::to, {to->getId(), getInletIndex()});
        }
        else
        {
            dico->clear(Tag::List::from);
            dico->clear(Tag::List::to);
        }
    }
}
















