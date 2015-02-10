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
    
    sObject BangBox::create(Initializer const& initiliazer) const
    {
        return make_shared<BangBox>(initiliazer);
    }
    
    BangBox::BangBox(Initializer const& init) : Object(init, "bang"),
    m_clock(Clock::create())
    {
        addInlet(Io::Message, Io::Hot, "Flash (anything)");
        addOutlet(Io::Message, "Output (bang)");
    }
    
    BangBox::~BangBox()
    {
        ;
    }
    
    void BangBox::tick()
    {
        Console::post("bang tick");
        m_led = false;
        redraw();
    }
    
    void BangBox::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            Console::post("bang receive");
            bang();
        }
    }
    
    void BangBox::bang()
    {
        Console::post("bang dobang");
        Object::send(0, {Tag::List::bang});
        m_led = true;
        redraw();
        m_clock->delay(getShared(), 150);
    }
}

