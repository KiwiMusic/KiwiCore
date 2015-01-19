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
    
    // ================================================================================ //
    //                                      BANG                                        //
    // ================================================================================ //
    
    class Bang : public Box, public Clock::Maker
    {
    private:
        const sAttrColor    m_color_circle;
        const sAttrColor    m_color_led;
        atomic_bool         m_led;
        sClock              m_clock;
    public:
        
        Bang(sPage page);
        ~Bang();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        void tick() override;
        Allocate(Bang);
    };
    
    // ================================================================================ //
    //                                      TOGGLE                                      //
    // ================================================================================ //
    
    class Toggle : public Box
    {
    private:
        const sAttrColor    m_color_cross_on;
        const sAttrColor    m_color_cross_off;
        bool  m_value;
    public:
 
        Toggle(sPage page);
        ~Toggle();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        Allocate(Toggle);
    };
    
    // ================================================================================ //
    //                                      MESSAGE                                     //
    // ================================================================================ //
    
    class Message : public Box, public TextEditor, public Clock::Maker
    {
    private:
        ElemVector      m_elements;
        vector<string>  m_text;
		bool			m_mouse_down;
    public:
        
        Message(sPage page);
        ~Message();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool receive(Event::Keyboard const& event) override;
        bool receive(Event::Focus::Type event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        void tick() override;
        Allocate(Message);
    };
    
    // ================================================================================ //
    //                                      NUMBER                                      //
    // ================================================================================ //
    
    class Number : public Box, public TextEditor, public Clock::Maker
    {
    private:
        double m_value;
        double m_increment;
        double m_last_y;
        bool   m_edition;
        string m_text;
        bool   m_maker;
        Point  m_selection;
        sClock m_clock;
    public:
        
        Number(sPage page);
        ~Number();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool receive(Event::Keyboard const& event) override;
        bool receive(Event::Focus::Type event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        void tick() override;
        Allocate(Number);
    };
    
    // ================================================================================ //
    //                                      SLIDER                                      //
    // ================================================================================ //
    
    class Slider : public Box
    {
    private:
        const sAttrColor    m_color_on;
        const sAttrColor    m_color_off;
        const sAttrColor    m_color_knob;
        const sAttrTag      m_tag_receive;
        const sAttrTag      m_tag_send;
        sBeacon             m_beacon_receive;
        sBeacon             m_beacon_send;
        double              m_value;
    public:
        
        Slider(sPage page);
        ~Slider();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        bool draw(Doodle& doodle) const override;
        bool attributeChanged(sAttr attr) override;
        void send() const;
        Allocate(Slider);
    };
	
	// ================================================================================ //
	//                                      PANEL                                       //
	// ================================================================================ //
	
	class Panel : public Box
	{
	private:
		const sAttrLong m_border_size;
		const sAttrLong m_border_radius;
	public:
		
		Panel(sPage page);
		~Panel() {};
		bool receive(unsigned long index, ElemVector const& elements) override;
		bool draw(Doodle& doodle) const override;
		bool attributeChanged(sAttr attr) override;
		Allocate(Panel);
	};
	
	// ================================================================================ //
	//									PROTOTYPES                                      //
	// ================================================================================ //
	
    inline void gui()
    {
        Box::addPrototype(unique_ptr<Box>(new Bang(nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Toggle(nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Message(nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Number(nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Slider(nullptr)));
		Box::addPrototype(unique_ptr<Box>(new Panel(nullptr)));
    }
}

#endif


