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
    
    NewBox::NewBox(sPage page, ElemVector const& elements, string const& name) : Box(page, name, Graphic | Mouse | Keyboard)
    {
        //addInlet(Iolet::Message, Iolet::Hot, "inlet 1");
		TextEditor::setFont(getFont());
		TextEditor::setJustification(Font::Justification::VerticallyCentred);
		TextEditor::setColor(attr_color_text->get());
		TextEditor::setSize(getSize());
		TextEditor::setPadding(3., 3., 3., 3.);
    }
	
    NewBox::~NewBox()
    {
        ;
    }
	
    bool NewBox::receive(unsigned long index, ElemVector const& elements)
    {
        return false;
    }
	
	bool NewBox::receive(Event::Mouse const& event)
	{
		return false;
	}
	
	bool NewBox::receive(Event::Keyboard const& event)
	{
		/*
		if(Text::Editor::receive(event))
		{
			redraw();
			return true;
		}
		*/
		return false;
	}
	
	bool NewBox::receive(Event::Focus::Type event)
	{
		return false; //Text::Editor::receive(event);
	}
	
	bool NewBox::draw(Doodle& d) const
	{
		const double borderSize = 1;
		
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize), borderSize);
		d.setColor(getBackgroundColor());
		d.fillRectangle(d.getBounds().reduced(borderSize*2));
		
		if (toString(getName()) != "newbox")
		{
			d.setColor(getTextColor());
			d.drawText(toString(getText()), 3, 0, d.getWidth(), d.getHeight(), getFontJustification());
		}
		//Text::Editor::draw(d);
		return true;
	}
}