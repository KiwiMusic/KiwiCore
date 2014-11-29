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
    Bang::Bang(sPage page) : Box(page, "bang", Graphic | Mouse)
    {
        addInlet(Inlet::DataHot, "Flash (anything)");
        addOutlet(Outlet::Data, "Output (bang)");
    }
    
    Bang::~Bang()
    {
        ;
    }
    
    bool Bang::receive(unsigned long index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            send(0, {Tag_bang});
        }
        return false;
    }
    
    bool Bang::receive(Event::Mouse const& event)
    {
        if(event.type == Event::Mouse::Down)
        {
            send(0, {Tag_bang});
            return true;
        }
        return false;
    }
    
    bool Bang::draw(Doodle& doodle) const
    {
        doodle.setColor(Color(1., 1., 0., 0.3));
        doodle.fillAll();
        return true;
    }
    
    bool Bang::attributeChanged(sAttr attr)
    {
        cout << "aki" << endl;
        if(attr == appearance_size)
        {
            Point size = appearance_size->get();
            if(size.x() != size.y())
            {
                appearance_size->set({size.x(), size.x()});
            }
        }
        return true;
    }
    
    Toggle::Toggle(sPage page) : Box(page, "toggle", Graphic | Mouse),
    m_value(false),
    m_cross_color(Attr::create<AttrColor>(Tag::create("crosscolor"), Tag::create("Cross Color"), Tag_Color, (ElemVector){0.4, 0.4, 0.4, 1.}))
    {
        addInlet(Inlet::DataHot, "Active/Desactive (bang, int or float)");
        addOutlet(Outlet::Data, "Boolean 0 or 1 (float)");
        addAttribute(m_cross_color);
        setAttributeDefaultValues(Tag_size, {20., 20.});
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
                send(0, {m_value});
                redraw();
                return true;
            }
            else if(elements[0] == Tag_bang)
            {
                m_value = !m_value;
                send(0, {m_value});
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
            send(0, {m_value});
            redraw();
            return true;
        }
        return false;
    }
    
    bool Toggle::draw(Doodle& doodle) const
    {
        const double size = getSize().x();
        if(m_value)
        {
            doodle.setColor(m_cross_color->get());
        }
        else
        {
            Color color = m_cross_color->get();
            color.alpha(0.1);
            doodle.setColor(color);
        }
        
        doodle.drawLine(size * 0.25, size * 0.25, size * 0.75, size * 0.75, 1.5);
        doodle.drawLine(size * 0.75, size * 0.25, size * 0.25, size * 0.75, 1.5);
        return true;
    }
    
    bool Toggle::attributeChanged(sAttr attr)
    {
        if(attr == appearance_size)
        {
            Point size = appearance_size->get();
            if(size.x() != size.y())
            {
                appearance_size->set({size.x(), size.x()});
            }
        }
        else if(attr == m_cross_color)
        {
            redraw();
        }
        return true;
    }
}

