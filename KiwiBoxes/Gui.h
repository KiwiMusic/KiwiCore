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

#ifndef __DEF_KIWI_GUI__
#define __DEF_KIWI_GUI__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      GUI                                         //
    // ================================================================================ //
    
    class Bang : public Box
    {
    public:
        
        Bang(sPage page);
        ~Bang();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        Allocate(Bang);
    };
    
    class Toggle : public Box
    {
    private:
        bool       m_value;
        sAttrColor m_cross_color;
    public:
 
        Toggle(sPage page);
        ~Toggle();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        Allocate(Toggle);
    };
    
    inline void gui()
    {
        Box::addPrototype(unique_ptr<Box>(new Bang(nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Toggle(nullptr)));
    }
}

#endif


