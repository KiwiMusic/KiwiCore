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
    
    Bang::Bang(sPage page) : Box(page, "bang", Graphic | Mouse),
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
        addInlet(Iolet::Message, Iolet::Hot, "Flash (anything)");
        addOutlet(Iolet::Message, "Output (bang)");
        addAttribute(m_color_circle);
        addAttribute(m_color_led);
        setAttributeDefaultValues(Tag_size, {20., 20.});
		setSizeLimits(Point(10, 10));
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
    
    bool Bang::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            Box::send(0, {Tag_bang});
            m_led = true;
            redraw();
            m_clock->delay(getShared(), 150);
            return true;
        }
        return false;
    }
    
    bool Bang::receive(Event::Mouse const& event)
    {
        if(event.isDown())
        {
            Box::send(0, {Tag_bang});
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
    
    bool Bang::draw(Doodle& d) const
    {
		double borderSize = 1;
		const Rectangle bounds = d.getBounds().reduced(borderSize);
		const Rectangle ledRect = bounds.reduced(d.getWidth() * 0.4);
		
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize), borderSize, 0);
		
		d.setColor(getBackgroundColor());
		d.fillRectangle(d.getBounds().reduced(borderSize*2));

		d.setColor(m_color_circle->get());
		d.drawEllipse(ledRect, d.getWidth() * 0.1);
		
		if(m_led)
		{
			d.setColor(m_color_led->get());
			d.fillEllipse(ledRect);
		}
			
        return true;
    }
    
    bool Bang::attributeChanged(sAttr attr)
    {
        if(attr == m_color_circle)
        {
            redraw();
        }
        return true;
    }
    
    // ================================================================================ //
    //                                      TOGGLE                                      //
    // ================================================================================ //
    
    Toggle::Toggle(sPage page) : Box(page, "toggle", Graphic | Mouse),
    m_color_cross_on(Attr::create<AttrColor>(Tag::create("crosscoloron"),
                                            Tag::create("Cross Color On"),
                                            Tag::create("Color"),
                                            (ElemVector){0.4, 0.4, 0.4, 1.})),
    m_color_cross_off(Attr::create<AttrColor>(Tag::create("crosscoloroff"),
                                            Tag::create("Cross Color Off"),
                                            Tag::create("Color"),
                                            (ElemVector){0.6, 0.6, 0.6, 1.})),
    m_value(false)
    {
        addInlet(Iolet::Message, Iolet::Hot, "Active/Desactive (bang, int or float)");
        addOutlet(Iolet::Message, "Boolean 0 or 1 (float)");
        addAttribute(m_color_cross_on);
        addAttribute(m_color_cross_off);
        setAttributeDefaultValues(Tag_size, {20., 20.});
		setSizeLimits(Point(10, 10));
		setSizeRatio(1.);
    }
    
    Toggle::~Toggle()
    {
        ;
    }
    
    bool Toggle::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(elements[0].isNumber())
            {
                m_value = elements[0];
                Box::send(0, {m_value});
                redraw();
                return true;
            }
            else if(elements[0] == Tag_bang)
            {
                m_value = !m_value;
                Box::send(0, {m_value});
                redraw();
                return true;
            }
            else if(elements[0] == Tag_set)
            {
                if(elements.size() > 1 && elements[1].isNumber())
                {
                    m_value = elements[0];
                    redraw();
                    return true;
                }
                else
                {
                    Console::error(getShared(), "The message \"set\" implies a number after it.");
                }
            }
        }
        return false;
    }
    
    bool Toggle::receive(Event::Mouse const& event)
    {
        if(event.isDown())
        {
            m_value = !m_value;
            Box::send(0, {m_value});
            redraw();
            return true;
        }
        return false;
    }
    
    bool Toggle::draw(Doodle& d) const
    {
		double borderSize = 2;
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize), borderSize, 0);
		d.setColor(getBackgroundColor());
		d.fillRectangle(d.getBounds().reduced(borderSize));
		
        if(m_value)
        {
            d.setColor(m_color_cross_on->get());
        }
        else
        {
            d.setColor(m_color_cross_off->get());
        }
		
		const double size1 = d.getWidth() * 0.25;
		const double size2 = size1 * 3;
        d.drawLine(size1, size1, size2, size2, 1.5);
        d.drawLine(size2, size1, size1, size2, 1.5);
        return true;
    }
    
    bool Toggle::attributeChanged(sAttr attr)
    {
        if(attr == m_color_cross_on && m_value)
        {
            redraw();
        }
        else if(attr == m_color_cross_off && !m_value)
        {
            redraw();
        }
        return true;
    }
    
    // ================================================================================ //
    //                                      MESSAGE                                     //
    // ================================================================================ //
    
    Message::Message(sPage page) : Box(page, "message", Graphic | Mouse | Keyboard)
    {
        addInlet(Iolet::Message, Iolet::Hot, "Messages and Ouput (anything)");
        addInlet(Iolet::Message, Iolet::Hot, "Messages without Ouput (anything)");
        addOutlet(Iolet::Message, "Messages (anything)");
        setAttributeDefaultValues(Tag_size, {80., 20.});
		setAttributeValue(Tag_bgcolor, {0.66, 0.66, 0.66, 1.});
        
        Text::Editor::setFont(getFont());
        Text::Editor::setJustification(Font::Justification::VerticallyCentred);
        Text::Editor::setColor(attr_color_text->get());
        Text::Editor::setSize(getSize());
        Text::Editor::setMargins(3., 3., 3., 3.);
        Text::Editor::setBehavior(Text::Editor::Wrapped);
    }
    
    Message::~Message()
    {
        m_elements.clear();
    }
    
    bool Message::receive(unsigned long index, ElemVector const& elements)
    {
        return false;
    }
    
    bool Message::receive(Event::Mouse const& e)
    {
		if (e.isDown())
		{
			m_mouse_down = true;
			redraw();
		}
		else if (e.isUp())
		{
			m_mouse_down = false;
			redraw();
		}
        return false;
    }
    
    bool Message::receive(Event::Keyboard const& event)
    {
        if(Text::Editor::receive(event))
        {
            redraw();
            return true;
        }
        return false;
    }
    
    bool Message::receive(Event::Focus::Type event)
    {
        return Text::Editor::receive(event);
    }
    
    bool Message::draw(Doodle& d) const
    {
		const double borderSize = m_mouse_down ? 3 : 1;
		const double borderRadius = 4;
		
		d.setColor(getBackgroundColor());
		d.fillRectangle(d.getBounds().reduced(borderSize), borderRadius);
		
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize), borderSize, borderRadius);
		
        Text::Editor::draw(d);
        return true;
    }
    
    bool Message::attributeChanged(sAttr attr)
    {
        if(attr == attr_size)
        {
            Text::Editor::setSize(attr_size->get());
        }
        else if(attr == attr_color_text)
        {
            Text::Editor::setColor(attr_color_text->get());
        }
        else if(attr == attr_font_face || attr == attr_font_name || attr == attr_font_size)
        {
            Text::Editor::setFont(getFont());
        }
        return true;
    }
    
    void Message::tick()
    {
        
    }
    
    // ================================================================================ //
    //                                      NUMBER                                      //
    // ================================================================================ //
    
    Number::Number(sPage page) : Box(page, "number", Graphic | Mouse | Keyboard),
    m_value(0.),
    m_increment(0.),
    m_last_y(0.),
    m_edition(false),
    m_clock(Clock::create())
    {
        addInlet(Iolet::Message, Iolet::Hot, "New value and Ouput (int, float or bang)");
        addOutlet(Iolet::Message, "Value (float)");
        addOutlet(Iolet::Message, "Tab key pressed (bang)");
        setAttributeDefaultValues(Tag_size, {50., 20.});

        Text::Editor::setFont(getFont());
        Text::Editor::setJustification(Font::Justification::VerticallyCentred);
        Text::Editor::setColor(attr_color_text->get());
        Text::Editor::setSize(getSize());
        Text::Editor::setMargins(3., 3., 3., 3.);
    }
    
    Number::~Number()
    {
        ;
    }
    
    bool Number::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(elements[0].isNumber())
            {
                Text::Editor::setText(toString((double)elements[0]));
				redraw();
                Box::send(0, {m_value});
                return true;
            }
            else if(elements[0] == Tag_bang)
            {
                Box::send(0, {m_value});
                return true;
            }
            else if(elements[0] == Tag_set)
            {
                if(elements.size() > 1 && elements[1].isNumber())
                {
                    Text::Editor::setText(toString((double)elements[1]));
					redraw();
                    return true;
                }
                else
                {
                    Console::error(getShared(), "The message \"set\" implies a number after it.");
                }
            }
            else if(elements[0] == Tag_focus)
            {
                grabKeyboardFocus();
            }
            m_edition = false;
        }
        return false;
    }
    
    bool Number::receive(Event::Mouse const& event)
    {
        if(!m_edition)
        {
            if(event.isDown())
            {
                event.setMouseUnlimited(false);
                if(event.getX() < 14)
                {
                    send(0, {m_value});
                    m_edition = false;
                }
                else if(!m_edition)
                {
                    string text = Text::Editor::getStringSelection(getFont(), to_string(m_value), 0., event.getX() - 14.);
                    for(unsigned long i = 0; i < text.size(); i++)
                    {
                        if(text[i] != '.')
                            text[i] = '0';
                    }
                    if(text.find('.') != string::npos)
                    {
                        text.push_back('1');
                        m_increment = stod(text);
                    }
                    else
                    {
                        m_increment = 1.;
                    }
                    m_last_y = event.getY();
                    m_edition = false;
                }
                return true;
            }
            else if(event.isDrag())
            {
                if(!m_edition)
                {
                    event.setMouseUnlimited(true);
                    m_value += m_increment * (m_last_y - event.getY());
                    m_last_y = event.getY();
                    send(0, {m_value});
                    redraw();
                }
                
                return true;
            }
			else if(event.isDoubleClick())
			{
				if(!m_edition)
				{
					m_edition = true;
					m_maker   = true;
					m_clock->delay(getShared(), 500.);
				}
			}
            else
            {
                event.setMouseUnlimited(false);
                return false;
            }

        }
        else
        {
            if(Text::Editor::receive(event))
            redraw();
        }
        
        return false;
    }
    
    bool Number::receive(Event::Keyboard const& event)
    {
        if(m_edition && event.isEscape())
        {
            m_value = stod(m_text);
            m_edition = false;
            m_text.clear();
            redraw();
        }
        else if(m_edition && event.isReturn())
        {
            m_value = stod(m_text);
            send(0, {m_value});
            m_edition = false;
            m_text.clear();
            redraw();
        }
        else if(m_edition && event.isTab())
        {
            m_value = stod(m_text);
            send(1, {Tag_bang});
            send(0, {m_value});
            m_text.clear();
            redraw();
        }
        else if(m_edition && event.isBackspace())
        {
            m_text.pop_back();
            redraw();
        }
        else
        {
            m_text.push_back(event.getCharacter());
            redraw();
            if(!m_edition)
            {
                m_edition = true;
                m_maker   = true;
                m_clock->delay(getShared(), 500.);
            }
        }
        return true;
    }
    
    bool Number::receive(Event::Focus::Type event)
    {
        if(event == Event::Focus::Out && m_edition && !m_text.empty())
        {
			try
			{
				m_value = stod(m_text);
			}
			catch (std::invalid_argument)
			{
				Console::error(getShared(), "invalid argument");
				m_value = 0.;
			}
			
			send(0, {m_value});
			m_edition = false;
			m_text.clear();
			redraw();
        }
        return true;
    }
    
    void Number::tick()
    {
        if(m_edition)
        {
            m_maker = !m_maker;
            redraw();
            m_clock->delay(getShared(), 500.);
        }
    }
    
    bool Number::draw(Doodle& d) const
    {
		double borderSize = 2;
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize), borderSize, 0);
		d.setColor(getBackgroundColor());
		d.fillRectangle(d.getBounds().reduced(borderSize));
		
        const Point size = getSize();
        d.setColor(getBorderColor());
        d.drawLine(12., 0., 12., size.y(), 1.);
        d.setFont(getFont());
        if(m_edition)
        {
            d.setColor(getTextColor());
            d.drawText(m_text, 14., 0., size.x() - 16., size.y(), Font::VerticallyCentred);
            if(m_selection.y() != 0.)
            {
                d.setColor(Color(0.1, 0.2, 0.3, 0.25));
                d.fillRectangle(m_selection.x() + 14., 3., m_selection.y() + 14., size.y() - 3.);
            }
            else if(m_maker)
            {
                Point pt = Text::getStringSize(getFont(), m_text);
                d.drawLine(pt.x()+15., 3., pt.x()+15., size.y() - 3., 1.);
            }
        }
        else
        {
            d.setColor(getTextColor());
            d.drawText(toString(m_value), 14., 0., size.x() - 16., size.y(), Font::VerticallyCentred);
        }
        
        return true;
    }
    
    bool Number::attributeChanged(sAttr attr)
    {
        if(attr == attr_size)
        {
            Text::Editor::setSize(attr_size->get());
        }
        else if(attr == attr_color_text)
        {
            Text::Editor::setColor(getTextColor());
        }
        else if(attr == attr_font_face || attr == attr_font_name || attr == attr_font_size)
        {
            Text::Editor::setFont(getFont());
        }
        return true;
    }
    
    // ================================================================================ //
    //                                      SLIDER                                      //
    // ================================================================================ //
    
    Slider::Slider(sPage page) : Box(page, "slider", Graphic | Mouse),
    m_color_on(Attr::create<AttrColor>(Tag::create("coloron"),
                                       Tag::create("Color On"),
                                       Tag::create("Color"),
                                       (ElemVector){0.4, 0.4, 0.4, 1.})),
    m_color_off(Attr::create<AttrColor>(Tag::create("coloroff"),
                                        Tag::create("Color Off"),
                                        Tag::create("Color"),
                                        (ElemVector){0., 0.6, 0.6, 1.})),
    m_color_knob(Attr::create<AttrColor>(Tag::create("colorknob"),
                                        Tag::create("Knob Color"),
                                        Tag::create("Color"),
                                        (ElemVector){0.4, 0.4, 0.4, 1.})),
    m_tag_receive(Attr::create<AttrTag>(Tag::create("receivename"),
                                        Tag::create("Receive Name"),
                                        Tag::create("Behavior"))),
    m_tag_send(Attr::create<AttrTag>(Tag::create("sendname"),
                                        Tag::create("Send Name"),
                                        Tag::create("Behavior"))),
    m_value(false)
    {
        addInlet(Iolet::Message, Iolet::Hot, "New value (int or float)");
        addOutlet(Iolet::Message, "Mapped value (float)");
        addAttribute(m_color_on);
        addAttribute(m_color_off);
        addAttribute(m_color_knob);
        addAttribute(m_tag_receive);
        addAttribute(m_tag_send);
        setAttributeDefaultValues(Tag_size, {120., 20.});
    }
    
    Slider::~Slider()
    {
        ;
    }
    
    void Slider::send() const
    {
        ElemVector elements = {m_value};
        Box::send(0, elements);
        if(m_beacon_send)
        {
            for(unsigned long i = 0; i < m_beacon_send->size(); i++)
            {
                sBox box = m_beacon_send->getBox(i);
                if(box)
                {
                    box->receive(0, elements);
                }
            }
        }
    }
    
    bool Slider::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(elements[0].isNumber())
            {
                m_value = elements[0];
                send();
                redraw();
                return true;
            }
            else if(elements[0] == Tag_bang)
            {
                m_value = !m_value;
                send();
                redraw();
                return true;
            }
            else if(elements[0] == Tag_set)
            {
                if(elements.size() > 1 && elements[1].isNumber())
                {
                    m_value = elements[0];
                    redraw();
                    return true;
                }
                else
                {
                    Console::error(getShared(), "The message \"set\" implies a number after it.");
                }
            }
        }
        return false;
    }
    
    bool Slider::receive(Event::Mouse const& event)
    {
        if(event.isDown())
        {
            m_value = !m_value;
            send();
            redraw();
            return true;
        }
        return false;
    }
    
    bool Slider::draw(Doodle& d) const
    {
        const Point size = getSize();
        if(size.x() > size.y())
        {
            d.setColor(m_color_on->get());
            d.fillAll();
            d.setColor(m_color_off->get());
            d.fillRectangle(0., 0., size.x() * 0.5 - 3., size.y(), 3.);
            d.setColor(m_color_knob->get());
            d.drawLine(size.x() * 0.5, 0., size.x() * 0.5, size.y(), 3.);
        }
        return true;
    }
    
    bool Slider::attributeChanged(sAttr attr)
    {
        if(attr == m_tag_receive)
        {
            if(m_tag_receive->get())
            {
                sBeacon newbeacon = Beacon::create(getShared(), toString(m_tag_receive->get()));
                if(m_beacon_receive != newbeacon)
                {
                    if(m_beacon_receive)
                    {
                        m_beacon_receive->unbind(getShared());
                        m_beacon_receive = newbeacon;
                        m_beacon_receive->bind(getShared());
                    }
                }
            }
            else if(m_beacon_receive)
            {
                m_beacon_receive->unbind(getShared());
                m_beacon_receive = nullptr;
            }
        }
        else if(attr == m_tag_send)
        {
            if(m_tag_send->get())
            {
                m_beacon_send = Beacon::create(getShared(), toString(m_tag_send->get()));
            }
            else
            {
                m_beacon_send = nullptr;
            }
        }
        return true;
    }
	
	// ================================================================================ //
	//                                      PANEL                                       //
	// ================================================================================ //
	
	Panel::Panel(sPage page) : Box(page, "panel", Graphic),
	m_border_size(Attr::create<AttrLong>(Tag::create("border"),
										  Tag::create("Border Size"),
										  AttrBox::Tag_Appearance,
										  0)),
	m_border_radius(Attr::create<AttrLong>(Tag::create("rounded"),
											Tag::create("Border Radius"),
											AttrBox::Tag_Appearance,
											0))
	{
		addInlet(Iolet::Message, Iolet::Hot, "Messages");
		addAttribute(m_border_radius);
		addAttribute(m_border_size);
		setAttributeDefaultValues(Tag_size, {120., 120.});
		setAttributeDefaultValues(Tag_bgcolor, {0.44, 0.44, 0.44, 1.});
		setAttributeDefaultValues(Tag_bdcolor, {0.3, 0.3, 0.3, 1.});
	}
	
	bool Panel::receive(unsigned long index, ElemVector const& elements)
	{
		return false;
	}

	bool Panel::draw(Doodle& d) const
	{
		const long borderSize = m_border_size->get();
		const long radius = m_border_radius->get();
		const Rectangle rect = d.getBounds().reduced(borderSize);
		
		d.setColor(getBackgroundColor());
		d.fillRectangle(rect, radius);
		
		d.setColor(getBorderColor());
		d.drawRectangle(rect, borderSize, radius);
		
		return true;
	}
	
	bool Panel::attributeChanged(sAttr attr)
	{
		if(attr == m_border_radius || attr == m_border_size)
		{
			redraw();
		}
		return true;
	}

}

