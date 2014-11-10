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
#include "../ThirdParty/muParser/include/muParser.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ARITHMETIC                                      //
    // ================================================================================ //
    
    class Arithmetic : public Box
    {
    protected:
        double          m_first;
        double          m_second;
    public:
        Arithmetic(sPage page, string const& name, ElemVector const& elements, string const& input1, string const& input2,string const& output);
        virtual ~Arithmetic();
        virtual string getExpression() const noexcept override;
    private:
        bool receive(size_t index, ElemVector const& elements) override;
        virtual inline double compute() = 0;
        virtual sBox allocate(sPage page, sDico dico) const = 0;
    };
    
    class Plus : public Arithmetic
    {
    public:
        Plus(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "+", elements, "Augend", "Addend", "Sum"){}
        ~Plus(){}
    private:
        inline double compute() override{return m_first + m_second;}
        AllocateElemVector(Plus);
    };
    
    class Minus : public Arithmetic
    {
    public:
        Minus(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "-", elements, "Minuend", "Subtrahend", "Difference"){}
        ~Minus(){}
    private:
        inline double compute() override{return m_first - m_second;}
        AllocateElemVector(Minus);
    };
    
    class Times : public Arithmetic
    {
    public:
        Times(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "*", elements, "Multiplicand", "Multiplier", "Product"){}
        ~Times(){}
    private:
        inline double compute() override{return m_first * m_second;}
        AllocateElemVector(Times);
    };
    
    class Divide : public Arithmetic
    {
    public:
        Divide(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "/", elements, "Dividend", "Divisor", "Quotient"){}
        ~Divide(){}
    private:
        inline double compute() override{return m_first / m_second;}
        AllocateElemVector(Divide);
    };
    
    class Modulo : public Arithmetic
    {
    public:
        Modulo(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "%", elements, "Dividend", "Divisor", "Remainder"){}
        ~Modulo(){}
    private:
        inline double compute() override{return fmod(m_first, m_second);}
        AllocateElemVector(Modulo);
    };
    
    class Power : public Arithmetic
    {
    public:
        Power(sPage page = nullptr, ElemVector const& elements = {}) :
        Arithmetic(page, "^", elements, "Base", "Exponent", "Power"){}
        ~Power(){}
    private:
        inline double compute() override{return pow(m_first, m_second);}
        AllocateElemVector(Power);
    };
    
    class Expression : public Box
    {
    private:
        mu::Parser      m_parser;
        vector<double>  m_values;
    public:
        Expression(sPage page, ElemVector const& elements);
        ~Expression();
    private:
        bool receive(size_t index, ElemVector const& elements) override;
        sBox allocate(sPage page, sDico dico) const override
        {
            ElemVector elements;
            dico->get(Tag::create("arguments"), elements);
            return make_shared<Expression>(page, elements);
        }
    };
    
    inline void arithmetic()
    {
        Box::addPrototype(unique_ptr<Box>(new Plus()));
        Box::addPrototype(unique_ptr<Box>(new Minus()));
        Box::addPrototype(unique_ptr<Box>(new Times()));
        Box::addPrototype(unique_ptr<Box>(new Divide()));
        Box::addPrototype(unique_ptr<Box>(new Modulo()));
        Box::addPrototype(unique_ptr<Box>(new Power()));
        Box::addPrototype(unique_ptr<Box>(new Expression(nullptr, {})));
    }
}

#endif


