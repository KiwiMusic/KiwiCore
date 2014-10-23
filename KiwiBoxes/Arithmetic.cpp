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


#include "Arithmetic.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    
    Plus::Plus(sInstance kiwi, ElemVector const& elements) : Box(kiwi, "+"),
    m_double(elements.size() && elements[0].isDouble()),
    m_augend(0),
    m_addend(0)
    {
        if(elements.size() && m_double)
            m_addend = (double)elements[0];
        else if(elements.size())
            m_addend = (long)elements[0];
        
        addInlet(Inlet::Type::DataHot, "Augend");
        addInlet(Inlet::Type::DataCold, "Addend");
        addOutlet(Outlet::Type::Data, "Sum");
    }
    
    Plus::~Plus()
    {
        
    }
    
    sObject Plus::create(sDico dico) const
    {
        ElemVector elements;
        dico->get(createTag("arguments"), elements);
        return makeObject<Plus>(elements);
    }
    
    void Plus::receive(ElemVector const& elements)
    {
        if(elements.size())
        {
            switch (elements[1].type())
            {
                case Element::LONG:
                    ;
                    break;
                    
                default:
                    break;
            }
        }
        Box::receive(elements);
    }
}

