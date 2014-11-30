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
                                            (ElemVector){0.4, 0.4, 0.4, 1.})),
    m_color_led(Attr::create<AttrColor>(Tag::create("ledcolor"),
                                            Tag::create("Led Color"),
                                            Tag::create("Color"),
                                            (ElemVector){0.6, 0.6, 0.6, 1.})),
    m_tag_receive(Attr::create<AttrTag>(Tag::create("receivename"),
                                            Tag::create("Receive Name"),
                                            Tag::create("Behavior"))),
    m_tag_send(Attr::create<AttrTag>(Tag::create("sendname"),
                                            Tag::create("Send Name"),
                                            Tag::create("Behavior"))),
    m_led(false)
    {
        addInlet(Inlet::DataHot, "Flash (anything)");
        addOutlet(Outlet::Data, "Output (bang)");
        addAttribute(m_color_circle);
        addAttribute(m_color_led);
        addAttribute(m_tag_receive);
        addAttribute(m_tag_send);
        setAttributeDefaultValues(Tag_size, {20., 20.});
    }
    
    Bang::~Bang()
    {
        ;
    }
    
    void Bang::send() const
    {
        ElemVector elements = {Tag_bang};
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
    
    void Bang::tick()
    {
        m_led = false;
        redraw();
    }
    
    bool Bang::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            send();
            m_led = true;
            redraw();
            Clock clock(getShared(), 150.);
            return true;
        }
        return false;
    }
    
    bool Bang::receive(Event::Mouse const& event)
    {
        if(event.type == Event::Mouse::Down)
        {
            send();
            m_led = true;
            redraw();
            return true;
        }
        else if(event.type == Event::Mouse::Up)
        {
            m_led = false;
            redraw();
            return true;
        }
        return false;
    }
    
    bool Bang::draw(Doodle& doodle) const
    {
        const double size1 = getSize().x() * 0.25;
        const double size2 = getSize().x() * 0.5;
        
        doodle.setColor(m_color_circle->get());
        doodle.drawEllipse(size1, size1, size2, size2, 1.5);
        if(m_led)
        {
            doodle.setColor(m_color_led->get());
            doodle.fillEllipse(size1 + 1.5, size1 + 1.5, size2 - 3., size2 - 3.);
        }
        
        return true;
    }
    
    bool Bang::attributeChanged(sAttr attr)
    {
        if(attr == m_color_circle)
        {
            redraw();
        }
        else if(attr == m_tag_receive)
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
    m_tag_receive(Attr::create<AttrTag>(Tag::create("receivename"),
                                            Tag::create("Receive Name"),
                                            Tag::create("Behavior"))),
    m_tag_send(Attr::create<AttrTag>(Tag::create("sendname"),
                                            Tag::create("Send Name"),
                                            Tag::create("Behavior"))),
    m_value(false)
    {
        addInlet(Inlet::DataHot, "Active/Desactive (bang, int or float)");
        addOutlet(Outlet::Data, "Boolean 0 or 1 (float)");
        addAttribute(m_color_cross_on);
        addAttribute(m_color_cross_off);
        addAttribute(m_tag_receive);
        addAttribute(m_tag_send);
        setAttributeDefaultValues(Tag_size, {20., 20.});
    }
    
    Toggle::~Toggle()
    {
        ;
    }
    
    void Toggle::send() const
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
    
    bool Toggle::receive(unsigned long index, ElemVector const& elements)
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
    
    bool Toggle::receive(Event::Mouse const& event)
    {
        if(event.type == Event::Mouse::Down)
        {
            m_value = !m_value;
            send();
            redraw();
            return true;
        }
        return false;
    }
    
    bool Toggle::draw(Doodle& doodle) const
    {
        if(m_value)
        {
            doodle.setColor(m_color_cross_on->get());
        }
        else
        {
            doodle.setColor(m_color_cross_off->get());
        }
        const double size1 = getSize().x() * 0.25;
        const double size2 = getSize().x() * 0.75;
        doodle.drawLine(size1, size1, size2, size2, 1.5);
        doodle.drawLine(size2, size1, size1, size2, 1.5);
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
        else if(attr == m_tag_receive)
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
        addInlet(Inlet::DataHot, "New value (int or float)");
        addOutlet(Outlet::Data, "Mapped value (float)");
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
        if(event.type == Event::Mouse::Down)
        {
            m_value = !m_value;
            send();
            redraw();
            return true;
        }
        return false;
    }
    
    bool Slider::draw(Doodle& doodle) const
    {
        const Point size = getSize();
        if(size.x() > size.y())
        {
            doodle.setColor(m_color_on->get());
            doodle.fillAll();
            doodle.setColor(m_color_off->get());
            doodle.fillRectangle(0., 0., size.x() * 0.5 - 3., size.y(), 3.);
            doodle.setColor(m_color_knob->get());
            doodle.drawLine(size.x() * 0.5, 0., size.x() * 0.5, size.y(), 3.);
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
}

