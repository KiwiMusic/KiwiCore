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
    Plus::Plus(sPage page, Element const& element) : Box(page, "+"),
    m_double(element.isDouble()),
    m_augend(0),
    m_addend(element)
    {
        addInlet(Inlet::DataHot, "Augend (int or float)");
        addInlet(Inlet::DataCold, "Addend (int or float)");
        if(m_double)
            addOutlet(Outlet::Data, "Sum (float)");
        else
            addOutlet(Outlet::Data, "Sum (int)");
    }
    
    Plus::~Plus()
    {
        
    }
    
    bool Plus::receive(size_t index, ElemVector const& elements)
    {
        Console::post(shared_from_this(), "Receive inlet " + toString(index) + " : " + toString(elements));
        if(!elements.empty() && elements[0].isNumber())
        {
            if(index)
            {
                m_addend = elements[0];
            }
            else if(m_double)
            {
                m_augend = elements[0];
                Console::post(shared_from_this(), "Send : " + toString({m_augend + m_addend}));
                send(0, {m_augend + m_addend});
            }
            else
            {
                m_augend = (long)elements[0];
                Console::post(shared_from_this(), "Send : " + toString({(long)m_augend + (long)m_addend}));
                send(0, {(long)m_augend + (long)m_addend});
            }
            return true;
        }
        return false;
    }
    
    Minus::Minus(sPage page, Element const& element) : Box(page, "-"),
    m_double(element.isDouble()),
    m_minuend(0),
    m_subtrahend(element)
    {
        addInlet(Inlet::DataHot, "Minuend (int or float)");
        addInlet(Inlet::DataCold, "Subtrahend (int or float)");
        if(m_double)
            addOutlet(Outlet::Data, "Difference (float)");
        else
            addOutlet(Outlet::Data, "Difference (int)");
    }
    
    Minus::~Minus()
    {
        
    }
    
    bool Minus::receive(size_t index, ElemVector const& elements)
    {
        Console::post(shared_from_this(), "Receive inlet " + toString(index) + " : " + toString(elements));
        if(!elements.empty() && elements[0].isNumber())
        {
            if(index)
            {
                m_subtrahend = elements[0];
            }
            else if(m_double)
            {
                m_minuend = elements[0];
                Console::post(shared_from_this(), "Send : " + toString({m_minuend - m_subtrahend}));
                send(0, {m_minuend + m_subtrahend});
            }
            else
            {
                m_minuend = (long)elements[0];
                Console::post(shared_from_this(), "Send : " + toString({(long)m_minuend - (long)m_subtrahend}));
                send(0, {(long)m_minuend + (long)m_subtrahend});
            }
            return true;
        }
        return false;
    }
}

