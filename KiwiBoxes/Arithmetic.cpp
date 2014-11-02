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
    
    Plus::Plus(sPage page, Element const& element) : Box(page, "+"),
    m_double(element.isDouble()),
    m_augend(0),
    m_addend(element)
    {
        addInlet(Inlet::DataHot, "Augend (int or double)");
        addInlet(Inlet::DataCold, "Addend (int or double)");
        if(m_double)
            addOutlet(Outlet::Data, "Sum (double)");
        else
            addOutlet(Outlet::Data, "Sum (int)");
    }
    
    Plus::~Plus()
    {
        
    }
    
    sBox Plus::allocate(sPage page, sDico dico) const
    {
        Element element = dico->get(Tag::create("arguments"));
        shared_ptr<Plus> plus = make_shared<Plus>(page, element);
        return plus;
    }
    
    bool Plus::receive(size_t index, ElemVector const& elements)
    {
        Console::post("Receive inlet " + toString(index) + " : " + toString(elements));
        if(!elements.empty() && (elements[0].type() == Element::LONG || elements[0].type() == Element::DOUBLE))
        {
            if(index)
            {
                m_addend = elements[0];
            }
            else if(m_double)
            {
                Console::post("Send : " + toString({(double)elements[0] + m_addend}));
                send(0, {(double)elements[0] + m_addend});
            }
            else
            {
                Console::post("Send : " + toString({(long)elements[0] + (long)m_addend}));
                send(0, {(long)elements[0] + (long)m_addend});
            }
            return true;
        }
        return false;
    }
}

