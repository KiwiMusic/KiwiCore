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

#ifndef __DEF_KIWI_NEWBOX__
#define __DEF_KIWI_NEWBOX__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                    NEWBOX                                        //
    // ================================================================================ //
    
	class NewBox : public Box, public Gui::Mouser, public Gui::Writer
    {
    public:
        
        NewBox(sPage page, ElemVector const& elements = {}, string const& name = "newbox");
        ~NewBox();
        bool receive(ulong index, ElemVector const& elements) override;
        bool receive(Gui::Event::Mouse const& event) override;
		bool receive(Gui::Event::Keyboard const& event) override;
		bool receive(Gui::Event::Focus event) override;
		void draw(Gui::Doodle& doodle) const override;
		
		bool textFilter(wstring& newtext) override;
		void textChanged() override;
		
		sBox allocate(sPage page, sDico dico) const override;
    };
	
	// ================================================================================ //
	//                                 STANDARD BOXES                                   //
	// ================================================================================ //
	
    inline void standardBoxes()
    {
		Box::addPrototype(unique_ptr<Box>(new NewBox(sPage())));
    }
}

#endif


