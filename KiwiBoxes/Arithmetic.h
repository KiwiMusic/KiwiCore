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

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ARITHMETIC                                      //
    // ================================================================================ //
    
    class ArithmeticBox : public Object
    {
    protected:
        double          m_first;
        double          m_second;
    public:
        ArithmeticBox(sPage page, string const& name, ElemVector const& elements, string const& input1, string const& input2,string const& output);
        virtual ~ArithmeticBox();
        virtual string getExpression() const noexcept override;
    private:
        void receive(ulong index, ElemVector const& elements) override;
        virtual inline double compute() = 0;
        virtual sObject allocate(sPage page, sDico dico) const = 0;
    };
    
    class PlusBox : public ArithmeticBox
    {
    public:
        PlusBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "+", elements, "Augend", "Addend", "Sum"){}
        ~PlusBox(){}
    private:
        inline double compute() override{return m_first + m_second;}
        AllocateElemVector(PlusBox);
    };
    
    class MinusBox : public ArithmeticBox
    {
    public:
        MinusBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "-", elements, "Minuend", "Subtrahend", "Difference"){}
        ~MinusBox(){}
    private:
        inline double compute() override{return m_first - m_second;}
        AllocateElemVector(MinusBox);
    };
    
    class TimesBox : public ArithmeticBox
    {
    public:
        TimesBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "*", elements, "Multiplicand", "Multiplier", "Product"){}
        ~TimesBox(){}
    private:
        inline double compute() override{return m_first * m_second;}
        AllocateElemVector(TimesBox);
    };
    
    class DivideBox : public ArithmeticBox
    {
    public:
        DivideBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "/", elements, "Dividend", "Divisor", "Quotient"){}
        ~DivideBox(){}
    private:
        inline double compute() override{return m_first / m_second;}
        AllocateElemVector(DivideBox);
    };
    
    class ModuloBox : public ArithmeticBox
    {
    public:
        ModuloBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "%", elements, "Dividend", "Divisor", "Remainder"){}
        ~ModuloBox(){}
    private:
        inline double compute() override{return fmod(m_first, m_second);}
        AllocateElemVector(ModuloBox);
    };
    
    class PowerBox : public ArithmeticBox
    {
    public:
        PowerBox(sPage page = nullptr, ElemVector const& elements = {}) :
        ArithmeticBox(page, "^", elements, "Base", "Exponent", "PowerBox"){}
        ~PowerBox(){}
    private:
        inline double compute() override{return pow(m_first, m_second);}
        AllocateElemVector(PowerBox);
    };
    
    inline void arithmetic()
    {
        Object::addPrototype(unique_ptr<Object>(new PlusBox()));
        Object::addPrototype(unique_ptr<Object>(new MinusBox()));
        Object::addPrototype(unique_ptr<Object>(new TimesBox()));
        Object::addPrototype(unique_ptr<Object>(new DivideBox()));
        Object::addPrototype(unique_ptr<Object>(new ModuloBox()));
        Object::addPrototype(unique_ptr<Object>(new PowerBox()));
        //Object::addPrototype(unique_ptr<Object>(new Expression(nullptr, {})));
    }
}

#endif


