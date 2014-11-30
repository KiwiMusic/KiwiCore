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
    //                                      STORAGE                                     //
    // ================================================================================ //
    
    class Int : public Box
    {
    private:
        bool m_init;
        long m_value;
    public:
        
        Int(sPage page, ElemVector const& value);
        ~Int();
        bool receive(unsigned long index, ElemVector const& elements) override;
        bool receive(Event::Mouse const& event) override;
        AllocateElemVector(Int);
    };
    
    // ================================================================================ //
    //                                      SEND                                        //
    // ================================================================================ //
    
    class Send : public Box
    {
    private:
        sTag    m_name;
        sBeacon m_beacon;
    public:
        
        Send(sPage page, sTag name);
        ~Send();
        bool receive(unsigned long index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Send);
    };
    
    // ================================================================================ //
    //                                      RECEIVE                                     //
    // ================================================================================ //
    
    class Receive : public Box
    {
    private:
        sTag m_name;
    public:
        
        Receive(sPage page, sTag name);
        ~Receive();
        bool receive(unsigned long index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Receive);
    };
    
    // ================================================================================ //
    //                                      VALUE                                       //
    // ================================================================================ //
    
    class Value : public Box
    {
        class ValueHolder : public Box
        {
            public:
            ElemVector  m_values;
            ValueHolder(sPage page) : Box(nullptr, ""){};
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
        bool receive(unsigned long index, ElemVector const& elements) override;
        void load(scDico dico) override;
        AllocateTag(Value);
    };
    
    inline void wireless()
    {
        Box::addPrototype(unique_ptr<Box>(new Int(nullptr, {})));
        Box::addPrototype(unique_ptr<Box>(new Send(nullptr, nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Receive(nullptr, nullptr)));
        Box::addPrototype(unique_ptr<Box>(new Value(nullptr, nullptr)));
    }
}

#endif


