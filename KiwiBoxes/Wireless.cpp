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


#include "Wireless.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      INT                                         //
    // ================================================================================ //
    
    Int::Int(sPage page, ElemVector const& value) : Box(page, "int", Mouse),
    m_init(!value.empty()),
    m_value(0)
    {
        if(value.empty())
        {
            addInlet(IoType::Data, IoPolarity::Hot, "Value or Bang to output (int, float or bang)");
            addInlet(IoType::Data, IoPolarity::Hot, "Value to set (int or float)");
        }
        else if(value[0].isNumber())
        {
            addInlet(IoType::Data, IoPolarity::Hot, "Bang to output (bang)");
            m_value = value[0];
        }
        addOutlet(IoType::Data, "The integer (int)");
    }
    
    Int::~Int()
    {
        ;
    }
    
    bool Int::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index)
            {
                if(elements[0] == Tag_bang)
                {
                    send(0, {m_value});
                    return true;
                }
                else if(!m_init && elements[0].isNumber())
                {
                    m_value = elements[0];
                    send(0, {m_value});
                    return true;
                }
            }
            else
            {
                if(elements[0].isNumber())
                {
                    m_value = elements[0];
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Int::receive(Event::Mouse const& event)
    {
        if(event.type == Event::Mouse::DoubleClick)
        {
            send(0, {m_value});
            return true;
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      SEND                                        //
    // ================================================================================ //
    
    Send::Send(sPage page, sTag name) : Box(page, "send"),
    m_name(name)
    {
        addInlet(IoType::Data, IoPolarity::Hot, "Messages to send (anything)");
        if(!name)
        {
            addInlet(IoType::Data, IoPolarity::Cold, "Link name (tag)");
        }
    }
    
    Send::~Send()
    {
        ;
    }
    
    void Send::load(scDico dico)
    {
        if(m_name)
        {
            m_beacon = Beacon::create(getShared(), toString(m_name));
        }
    }
    
    bool Send::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index && m_beacon)
            {
                for(unsigned long i = 0; i < m_beacon->size(); i++)
                {
                    sBox box = m_beacon->getBox(i);
                    if(box)
                    {
                        box->receive(1, elements);
                    }
                }
                return true;
            }
            else if(index && elements[0].isTag())
            {
                m_name = elements[0];
                m_beacon = Beacon::create(getShared(), toString(m_name));
                return true;
            }
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      RECEIVE                                     //
    // ================================================================================ //
    
    Receive::Receive(sPage page, sTag name) : Box(page, "receive"),
    m_name(name)
    {
        addOutlet(IoType::Data, "Messages received (anything)");
        if(!name)
        {
            addInlet(IoType::Data, IoPolarity::Cold, "Link name (tag)");
        }
    }
    
    Receive::~Receive()
    {
        ;// Unbind
    }
    
    void Receive::load(scDico dico)
    {
        if(m_name)
        {
            sBeacon beacon = Beacon::create(getShared(), toString(m_name));
            if(beacon)
            {
                beacon->bind(getShared());
            }
        }
    }
    
    bool Receive::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(index)
            {
                Box::send(0, elements);
            }
            else if(!index && elements[0].isTag())
            {
                if(m_name)
                {
                    sBeacon beacon = Beacon::create(getShared(), toString(m_name));
                    if(beacon)
                    {
                        beacon->unbind(getShared());
                    }
                }
                
                m_name = elements[0];
                sBeacon beacon = Beacon::create(getShared(), toString(m_name));
                if(beacon)
                {
                    beacon->bind(getShared());
                }
            }
            return true;
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      VALUE                                       //
    // ================================================================================ //
    
    Value::Value(sPage page, sTag name) : Box(page, "value"),
    m_name(name)
    {
        addOutlet(IoType::Data, "Messages received (anything)");
        addInlet(IoType::Data, IoPolarity::Cold, "Messages to send or bang to send (anything)");
        if(!name)
        {
            addInlet(IoType::Data, IoPolarity::Cold, "Link name (tag)");
        }
        else
        {
            m_beacon = Beacon::create(getShared(), toString(m_name));
            if(m_beacon)
            {
                ;
            }
        }
    }
    
    Value::~Value()
    {
        
    }
    
    void Value::load(scDico dico)
    {
        if(m_name)
        {
            m_beacon = Beacon::create(getShared(), toString(m_name));
            if(m_beacon)
            {
                m_beacon->bind(getShared());
            }
        }
    }
    
    bool Value::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index)
            {
                /*
                if(elements[0].isTag() && elements[0] == Tag_bang)
                {
                    Box::send(0, m_values);
                }
                else
                {
                    m_values = elements;
                    if(m_beacon)
                    {
                        for(unsigned long i = 0; i < m_beacon->size(); i++)
                        {
                            sBox box = m_beacon->getBox(i);
                            if(box && box != getShared())
                            {
                                box->receive(0, m_values);
                            }
                        }
                    }
                }*/
            }
            else if(!index && elements[0].isTag())
            {
                if(m_name)
                {
                    sBeacon beacon = Beacon::create(getShared(), toString(m_name));
                    if(beacon)
                    {
                        beacon->unbind(getShared());
                    }
                }
                
                m_name = elements[0];
                sBeacon beacon = Beacon::create(getShared(), toString(m_name));
                if(beacon)
                {
                    beacon->bind(getShared());
                }
            }
            return true;
        }
        return false;
    }
}

