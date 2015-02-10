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
    
	class NewBox : public Object, public Gui::Sketcher, public Gui::Mouser, public Gui::Keyboarder, public Gui::TextField::Owner
    {
    private:
		
    public:
        
        NewBox(sPage page, ElemVector const& elements = {}, string const& name = "newbox");
        ~NewBox();
        bool receive(ulong index, ElemVector const& elements) override;
        bool receive(Gui::Event::Mouse const& event) override;
		bool receive(Gui::Event::Keyboard const& event) override;
		bool receive(Gui::Event::Focus event) override;
		void draw(Gui::Doodle& doodle) const override;
		
		//! This function is called whenever text is entered into the editor.
		/** An implementation of this class should check the input string,
		 and return an edited version of it that should be used.
		 @param newtext The new entered text.
		 @return True if you want to pass the key to the textfield, false otherwise.
		 */
		virtual bool textEntered(Gui::sTextField textfield, wstring& newtext) { return true; };
		
		//! This function is called when the text of the textfield changed.
		/** An implementation of this class should check the new text and do something with it.
		 @param textfield The textfield that changed.
		 */
		virtual void textChanged(Gui::sTextField textfield) {};
		
		//! This function is called when the user pressed the return key.
		/** This function is called when the user pressed the return key.
		 @param textfield The textfield in which the user pressed the return key.
		 */
		virtual void returnPressed() {};
		
		//! This function is called when the user pressed the escape key.
		/** This function is called when the user pressed the escape key.
		 @param textfield The textfield in which the user pressed the escape key.
		 */
		virtual void escapePressed() {};
		
        AllocateElemVector(NewBox);
    };
	
	// ================================================================================ //
	//                                 STANDARD BOXES                                   //
	// ================================================================================ //
	
    inline void standardBoxes()
    {
		Object::addPrototype(unique_ptr<Object>(new NewBox(sPage())));
    }
}

#endif


