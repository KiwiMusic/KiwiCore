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
	//										PRINT                                       //
	// ================================================================================ //
	
	Print::Print(sPage page, ElemVector const& value) : Object(page, "print"), m_name("print")
	{
		addInlet(Io::Message, Io::Hot, "Anything to be printed in the kiwi console");
		
		if(value.size() >= 1)
		{
			m_name = toString(value[0]);
		}
	}
	
	Print::~Print()
	{
		;
	}
	
	bool Print::receive(ulong index, ElemVector const& elements)
	{
		if(!elements.empty())
		{
			Console::error(toString(elements));
		}
		return false;
	}
	
	bool Print::receive(Gui::Event::Mouse const& event)
	{
		if(event.isDoubleClick())
		{
			// brings kiwi console to front
			return true;
		}
        return false;
	}
	
    // ================================================================================ //
    //                                      INT                                         //
    // ================================================================================ //
	
    Int::Int(sPage page, ElemVector const& value) : Object(page, "int"),
    m_init(!value.empty()),
    m_value(0)
    {
        if(value.empty())
        {
            addInlet(Io::Message, Io::Hot, "Value or Bang to output (int, float or bang)");
            addInlet(Io::Message, Io::Cold, "Value to set (int or float)");
        }
        else if(value[0].isNumber())
        {
            addInlet(Io::Message, Io::Hot, "Bang to output (bang)");
            m_value = value[0];
        }
        addOutlet(Io::Message, "The integer (int)");
    }
    
    Int::~Int()
    {
        ;
    }
    
    bool Int::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index)
            {
                if(elements[0] == Tag::List::bang)
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
    
    bool Int::receive(Gui::Event::Mouse const& event)
    {
        if(event.isDoubleClick())
        {
            send(0, {m_value});
            return true;
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      SEND                                        //
    // ================================================================================ //
    
    Send::Send(sPage page, sTag name) : Object(page, "send"),
    m_name(name)
    {
        addInlet(Io::Message, Io::Hot, "Messages to send (anything)");
        if(!name)
        {
            addInlet(Io::Message, Io::Cold, "Link name (tag)");
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
    
    bool Send::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index && m_beacon)
            {
                for(ulong i = 0; i < m_beacon->size(); i++)
                {
                    sObject object = m_beacon->getBox(i);
                    if(object)
                    {
                        object->receive(1, elements);
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
    
    Receive::Receive(sPage page, sTag name) : Object(page, "receive"),
    m_name(name)
    {
        addOutlet(Io::Message, "Messages received (anything)");
        if(!name)
        {
            addInlet(Io::Message, Io::Cold, "Link name (tag)");
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
    
    bool Receive::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(index)
            {
                Object::send(0, elements);
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
    
    Value::Value(sPage page, sTag name) : Object(page, "value"),
    m_name(name)
    {
        addOutlet(Io::Message, "Messages received (anything)");
        addInlet(Io::Message, Io::Cold, "Messages to send or bang to send (anything)");
        if(!name)
        {
            addInlet(Io::Message, Io::Cold, "Link name (tag)");
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
    
    bool Value::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(!index)
            {
                /*
                if(elements[0].isTag() && elements[0] == Tag::List::bang)
                {
                    Object::send(0, m_values);
                }
                else
                {
                    m_values = elements;
                    if(m_beacon)
                    {
                        for(ulong i = 0; i < m_beacon->size(); i++)
                        {
                            sObject object = m_beacon->getBox(i);
                            if(object && object != getShared())
                            {
                                object->receive(0, m_values);
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

