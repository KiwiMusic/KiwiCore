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

#ifndef __DEF_KIWI_GUI_BOXES__
#define __DEF_KIWI_GUI_BOXES__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      GUI                                         //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                      BANG                                        //
    // ================================================================================ //
    
    class Bang : public Box, public Gui::Sketcher, public Gui::Mouser, public Clock::Maker
    {
    private:
        const sAttrColor    m_color_circle;
        const sAttrColor    m_color_led;
        atomic_bool         m_led;
        sClock              m_clock;
    public:
        
        Bang(sPage page);
        ~Bang();
        bool receive(ulong index, ElemVector const& elements) override;
        bool receive(Gui::Event::Mouse const& event) override;
        void draw(Gui::Doodle& doodle) const override;
        bool notify(sAttr attr) override;
        void tick() override;
        Allocate(Bang);
    };
	
	// ================================================================================ //
	//									PROTOTYPES                                      //
	// ================================================================================ //
	
    inline void guiInit()
    {
        Box::addPrototype(unique_ptr<Box>(new Bang(nullptr)));
    }
}

#endif


