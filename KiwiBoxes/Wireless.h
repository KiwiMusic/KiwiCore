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

#ifndef __DEF_KIWI_SEND__
#define __DEF_KIWI_SEND__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
	// ================================================================================ //
	//                                    APPLICATION									//
	// ================================================================================ //
	
	// ================================================================================ //
	//										PRINT                                       //
	// ================================================================================ //
	
    class Print : public Object, public Gui::Mouser
	{
	private:
		string m_name;
	public:
		
		Print(sPage page, ElemVector const& value);
		~Print();
		bool receive(ulong index, ElemVector const& elements) override;
        bool receive(Gui::Event::Mouse const& event) override;
		AllocateElemVector(Print);
	};
	
    // ================================================================================ //
    //                                      STORAGE                                     //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                      INT                                         //
    // ================================================================================ //
    
    class Int : public Object, public Gui::Mouser
    {
    private:
        bool m_init;
        long m_value;
    public:
        
        Int(sPage page, ElemVector const& value);
        ~Int();
        bool receive(ulong index, ElemVector const& elements) override;
        bool receive(Gui::Event::Mouse const& event) override;
        AllocateElemVector(Int);
    };
    
    // ================================================================================ //
    //                                      SEND                                        //
    // ================================================================================ //
    
    class Send : public Object
    {
    private:
        sTag    m_name;
        sBeacon m_beacon;
    public:
        
        Send(sPage page, sTag name);
        ~Send();
        bool receive(ulong index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Send);
    };
    
    // ================================================================================ //
    //                                      RECEIVE                                     //
    // ================================================================================ //
    
    class Receive : public Object
    {
    private:
        sTag m_name;
    public:
        
        Receive(sPage page, sTag name);
        ~Receive();
        bool receive(ulong index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Receive);
    };
    
    // ================================================================================ //
    //                                      VALUE                                       //
    // ================================================================================ //
    
    class Value : public Object
    {
        class ValueHolder : public Object
        {
            public:
            ElemVector  m_values;
            ValueHolder(sPage page) : Object(nullptr, ""){};
            ~ValueHolder() {};
            Allocate(ValueHolder);
        };
    private:
        sTag                    m_name;
        sBeacon                 m_beacon;
        shared_ptr<ValueHolder> m_value_holder;
    public:
        
        Value(sPage page, sTag name);
        ~Value();
        bool receive(ulong index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Value);
    };
    
    inline void wireless()
    {
		Object::addPrototype(unique_ptr<Object>(new Print(sPage(), {})));
        Object::addPrototype(unique_ptr<Object>(new Int(sPage(), {})));
        Object::addPrototype(unique_ptr<Object>(new Send(sPage(), nullptr)));
        Object::addPrototype(unique_ptr<Object>(new Send(sPage(), nullptr)), "s");
        Object::addPrototype(unique_ptr<Object>(new Receive(sPage(), nullptr)));
        Object::addPrototype(unique_ptr<Object>(new Receive(sPage(), nullptr)), "r");
        Object::addPrototype(unique_ptr<Object>(new Value(sPage(), nullptr)));
    }
}

#endif


