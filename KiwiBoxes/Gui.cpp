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


#include "Gui.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      BANG                                        //
    // ================================================================================ //
    
    Bang::Bang(sPage page) : Box(page, "bang"),
    m_color_circle(Attr::create<AttrColor>(Tag::create("circlecolor"),
										   Tag::create("Circle Color"),
                                           Tag::create("Color"),
										   (ElemVector){0.52, 0.52, 0.52, 1.})),
    m_color_led(Attr::create<AttrColor>(Tag::create("ledcolor"),
                                            Tag::create("Led Color"),
                                            Tag::create("Color"),
                                            (ElemVector){0.6, 0.6, 0.6, 1.})),
    m_led(false),
    m_clock(Clock::create())
    {
        addInlet(Io::Message, Io::Hot, "Flash (anything)");
        addOutlet(Io::Message, "Output (bang)");
        addAttribute(m_color_circle);
        addAttribute(m_color_led);
        setAttributeDefaultValues(Tag_size, {20., 20.});
        setSizeLimits({10, 10});
		setSizeRatio(1.);
    }
    
    Bang::~Bang()
    {
        ;
    }
    
    void Bang::tick()
    {
        m_led = false;
        redraw();
    }
    
    bool Bang::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            Box::send(0, {Tag::List::bang});
            m_led = true;
            redraw();
            m_clock->delay(getShared(), 150);
            return true;
        }
        return false;
    }
    
    bool Bang::receive(Gui::Event::Mouse const& event)
    {
        if(event.isDown())
        {
            Box::send(0, {Tag::List::bang});
            m_led = true;
            redraw();
            return true;
        }
        else if(event.isUp())
        {
            m_led = false;
            redraw();
            return true;
        }
        return false;
    }
    
    void Bang::draw(Gui::Doodle& d) const
    {
		const double borderSize = 1;
		const Gui::Rectangle ledRect = d.getBounds().reduced(d.getWidth() * 0.2);

		d.fillAll(getBackgroundColor());
		
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize*0.5), borderSize, 0);

		d.setColor(m_color_circle->get());
		d.drawEllipse(ledRect, d.getWidth() * 0.1);
		
		if(m_led)
		{
			d.setColor(m_color_led->get());
			d.fillEllipse(ledRect);
		}
    }
    
    bool Bang::notify(sAttr attr)
    {
        if(attr == m_color_circle)
        {
            redraw();
        }
        return true;
    }
}

