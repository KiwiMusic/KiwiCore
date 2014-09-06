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

#ifndef __DEF_KIWI_ARITHMETIC__
#define __DEF_KIWI_ARITHMETIC__

#include "../KiwiCore/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ARITHMETIC                                      //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    
    class Plus : public Box
    {
    private:
        friend class Arithmetic;
        bool            m_double;
        double          m_augend;
        double          m_addend;
    public:
        Plus(shared_ptr<Instance> kiwi);
        ~Plus();
        
        static shared_ptr<Object> create(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name, vector<Element>& elements);
    
        static void receiveBang(shared_ptr<Plus> plus);
        static void receiveLong(shared_ptr<Plus> plus, long value);
        static void receiveDouble(shared_ptr<Plus> plus, double value);
    };
    
    // ================================================================================ //
    //                                      MINUS                                       //
    // ================================================================================ //
    /*
    class Minus : public Object
    {
    private:
        friend class    Arithmetic;
        const bool      m_double;
        double          m_minuend;
        double          m_subtrahend;
    protected:
        Minus(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Minus();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Minus);
    };
    
    // ================================================================================ //
    //                                      TIMES                                       //
    // ================================================================================ //
    
    class Times : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_multiplicand;
        double      m_multiplier;
    protected:
        Times(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Times();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Times);
    };
    
    // ================================================================================ //
    //                                     DIVIDE                                       //
    // ================================================================================ //
    
    class Divide : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_dividend;
        double      m_divisor;
    protected:
        Divide(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Divide();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Divide);
    };
    
    // ================================================================================ //
    //                                     MODULO                                       //
    // ================================================================================ //
    
    class Modulo : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_dividend;
        double      m_divisor;
    protected:
        Modulo(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Modulo();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Modulo);
    };
    
    // ================================================================================ //
    //                                     POWER                                        //
    // ================================================================================ //
    
    class Power : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_base;
        double      m_exponent;
    protected:
        Power(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Power();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Power);
    };
    
    // ================================================================================ //
    //                                     SQUARE                                       //
    // ================================================================================ //
    
    class Square : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_radiacand;
    protected:
        Square(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Square();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Square);
    };
    
    // ================================================================================ //
    //                                     ROOT                                         //
    // ================================================================================ //
    
    class Root : public Object
    {
    private:
        friend class Arithmetic;
        const bool  m_double;
        double      m_radiacand;
        double      m_degree;
    protected:
        Root(KiwiInstance* kiwi, const string name, vector<Element>& elements);
    public:
        ~Root();
        void output();
        void receiveBang(const Inlet* inlet) override;
        void receiveLong(const Inlet* inlet, long value) override;
        void receiveDouble(const Inlet* inlet, double value) override;
        
        KIWI_CREATE(Root);
    };
    
    // ================================================================================ //
    //                              ARITHMETIC LOADER                                   //
    // ================================================================================ //
    */
    class Arithmetic
    {
    public:
        static void load(shared_ptr<Instance> kiwi)
        {
            kiwi->addObjectPrototype(unique_ptr<Box>(new Plus(kiwi)));
            /*
            k->addObjectPrototype(unique_ptr<Object>(new Minus(k, string("Minus"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Minus(k, string("-"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Times(k, string("Times"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Times(k, string("*"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Divide(k, string("Divide"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Divide(k, string("/"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Modulo(k, string("Modulo"), elements)));
            k->addObjectPrototype(unique_ptr<Object>(new Modulo(k, string("%"), elements)));*/
        }
    };
    
}

#endif


