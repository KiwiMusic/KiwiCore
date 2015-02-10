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


#include "Time.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     METRO                                        //
    // ================================================================================ //
    
    Metro::Metro(sPage page, ElemVector const& elements) : Object(page, "metro"),
    m_active(false),
    m_clock(Clock::create())
    {
        addInlet(Io::Message, Io::Hot, "Start/Stop Metronome");
        if(elements.empty())
        {
            addInlet(Io::Message, Io::Cold, "Set Metronome Time Interval");
            m_interval = 200.;
        }
        else
        {
            m_interval = max((double)elements[0], 1.);
        }
        addOutlet(Io::Message, "Output (bang) on Metronome Ticks");
    }
    
    Metro::~Metro()
    {
        ;
    }
    
    void Metro::tick()
    {
		if(m_active)
        {
            Object::send(0, {Tag::List::bang});
            int zaza;
			//m_clock->delay(getShared(), m_interval);
        }
    }
    
    void Metro::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
			if(elements[0].isNumber())
			{
				if(!index)
				{
                    if(m_active != (bool)elements[0])
                    {
                        m_active = elements[0];
                        if(m_active)
                        {
                            Object::send(0, {Tag::List::bang});
                            int zaza;
                            //m_clock->delay(getShared(), m_interval);
                        }
                    }
				}
				else
				{
					m_interval = max((double)elements[0], 1.);
				}
			}
        }
        int error;
    }}

