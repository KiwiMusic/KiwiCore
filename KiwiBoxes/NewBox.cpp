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
	
	/*
    NewBox::NewBox(Initializer const& init) : Object(init, "newbox")
    {
		//m_textfield->setText(L"newbox test");
		//textfield_setText()
    }
	
    NewBox::~NewBox()
    {
        ;
    }
	
	sObject NewBox::create(Initializer const& initiliazer) const
	{
		sObject obj = make_shared<NewBox>(initiliazer);
		
		Gui::sWriter writer = dynamic_pointer_cast<Gui::Writer>(obj);
		if(writer)
		{
			Gui::Writer::sTextField textfield = writer->getTextField();
			if(textfield)
			{
				textfield->setWriter(writer);
			}
		}
		
		return obj;
	}
	*/
	
	/*
    bool NewBox::receive(ulong index, ElemVector const& elements)
    {
        return false;
    }
	
	bool NewBox::receive(Gui::Event::Mouse const& event)
	{
		setText(L"newbox newtext");
        return false;
	}
	
	
	bool NewBox::receive(Gui::Event::Keyboard const& event)
	{
		Console::post("newbox keybord event");
        return false; // return true to block keyboard events
	}
	
	bool NewBox::receive(Gui::Event::Focus event)
	{
        return false;
	}
	
	bool NewBox::textFilter(wstring& newtext)
	{
		string text = string(newtext.begin(), newtext.end());
		Console::post("newbox textenter: " + text + ";");
		return true;
	}
	
	void NewBox::textChanged()
	{
		;
	}
	
	void NewBox::draw(Gui::Doodle& d) const
	{
		const double borderSize = 1;
		d.fillAll(getBackgroundColor());
		d.setColor(getBorderColor());
		d.drawRectangle(d.getBounds().reduced(borderSize*0.5), borderSize);
	}
	*/
}