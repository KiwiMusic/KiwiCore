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
    
    Metro::Metro(sPage page, ElemVector const& args) : Box(page, "metro")
    {
        addInlet(IoType::Data, IoPolarity::Hot, "Start/Stop Metronome");
		addInlet(IoType::Data, IoPolarity::Cold, "Set Metronome Time Interval");
        addOutlet(IoType::Data, "Output (bang) on Metronome Ticks");
		
		if (args.size() >= 1 && args[0].isNumber())
		{
			setInterval(args[0]);
		}
		else
		{
			setInterval(200);
		}
    }
    
    Metro::~Metro()
    {
        ;
    }
    
    void Metro::tick()
    {
		Box::send(0, {Tag_bang});
		
		if (m_active)
			Clock::create(getShared(), m_interval);
    }
    
    bool Metro::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
			if (elements[0].isNumber())
			{
				if (index == 0) // start/stop metro
				{
					const bool wasActive = m_active;
					m_active = elements[0];
					
					if (!wasActive && m_active)
						Clock::create(getShared(), m_interval);
				}
				else if (index == 1) // set interval time
				{
					setInterval(elements[0]);
				}
			}
            return true;
        }
        return false;
    }
	
    bool Metro::attributeChanged(sAttr attr)
    {
        return true;
    }
	
	void Metro::setInterval(double interval)
	{
		m_interval = interval < 1. ? 1 : interval;
	}
}

