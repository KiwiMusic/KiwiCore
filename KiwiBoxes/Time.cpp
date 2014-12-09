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
    
    Metro::Metro(sPage page, ElemVector const& elements) : Box(page, "metro"),
    m_active(false),
    m_clock(Clock::create())
    {
        addInlet(IoType::Data, IoPolarity::Hot, "Start/Stop Metronome");
        if(elements.empty())
        {
            addInlet(IoType::Data, IoPolarity::Cold, "Set Metronome Time Interval");
            m_interval = 200.;
        }
        else
        {
            m_interval = max((double)elements[0], 1.);
        }
        addOutlet(IoType::Data, "Output (bang) on Metronome Ticks");
    }
    
    Metro::~Metro()
    {
        ;
    }
    
    void Metro::tick()
    {
		Box::send(0, {Tag_bang});
		if(m_active)
        {
			m_clock->delay(getShared(), m_interval);
        }
    }
    
    bool Metro::receive(unsigned long index, ElemVector const& elements)
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
                            m_clock->delay(getShared(), m_interval);
                            Box::send(0, {Tag_bang});
                        }
                    }
				}
				else
				{
					m_interval = max((double)elements[0], 1.);
				}
                return true;
			}
        }
        return false;
    }}

