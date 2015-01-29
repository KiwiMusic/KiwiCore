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


#include "NewBox.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     METRO                                        //
    // ================================================================================ //
    
    NewBox::NewBox(sPage page, ElemVector const& elements, string const& name) : Box(page, name)
    {
        //addInlet(Io::Message, Io::Hot, "inlet 1");
    }
	
    NewBox::~NewBox()
    {
        ;
    }
	
    bool NewBox::receive(ulong index, ElemVector const& elements)
    {
        return false;
    }
	
	bool NewBox::receive(Gui::Event::Mouse const& event)
	{
		setText(L"testtttt");
        return false;
	}
	
	bool NewBox::receive(Gui::Event::Keyboard const& event)
	{
        return false;
	}
	
	bool NewBox::receive(Gui::Event::Focus event)
	{
		//Text::Editor::receive(event);
        return false;
	}
	
	void NewBox::draw(Gui::Doodle& d) const
	{
		const double borderSize = 1;
		
		d.fillAll(getBackgroundColor());
		
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize*0.5), borderSize);
		
		if (toString(getName()) != "newbox")
		{
			d.setColor(getTextColor());
			d.drawText(toString(getText()), 3, 0, d.getWidth(), d.getHeight(), getFontJustification());
		}
		//Text::Editor::draw(d);
	}
}