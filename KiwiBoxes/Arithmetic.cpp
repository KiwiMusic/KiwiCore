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


#include "Arithmetic.h"

namespace Kiwi
{
    Arithmetic::Arithmetic(sPage page, string const& name, ElemVector const& elements, string const& input1, string const& input2,string const& output) :
    Object(page, name)
    {
        m_first     = 0.;
        m_second    = 0.;
        addInlet(Io::Message, Io::Hot, input1 + " (int or float)");
        if(!elements.empty() && (elements[0].isDouble() || elements[0].isLong()))
        {
            m_second = elements[0];
        }
        else
        {
            addInlet(Io::Message, Io::Cold, input2 + " (int or float)");
        }
        addOutlet(Io::Message, output + " (float)");
    }
    
    Arithmetic::~Arithmetic()
    {
        ;
    }
    
    void Arithmetic::receive(ulong index, ElemVector const& elements)
    {
        if(!elements.empty())
        {
            if(elements.size() == 1)
            {
                if(elements[0].isLong() || elements[0].isDouble())
                {
                    if(!index)
                    {
                        m_first = elements[0];
                        //Console::post(getShared(), "Send [" + toString({compute()}) + "]");
                        Console::post(toString(m_first) + " " + toString(getName()) + " " + toString(m_second) + " = " + toString(compute()));
                        send(0, {compute()});
                    }
                    else
                    {
                        m_second = elements[0];
                    }
                }
                else if(elements[0] == Tag::List::set)
                {
                    if(!index)
                    {
                        m_first = elements[0];
                        //Console::post(getShared(), "Send : " + toString({compute()}));
                        send(0, {compute()});
                    }
                    else
                    {
                        m_second = elements[0];
                    }
                }
            }
        }
        else
        {
            int zaza;
        }
    }
    
    string Arithmetic::getExpression() const noexcept
    {
        if(getNumberOfInlets() == 1)
        {
            return "i1" + toString(getName()) + toString(m_second);
        }
        else
        {
            return "i1" + toString(getName()) + "i2";
        }
    }
    
    /*
    Expression::Expression(sPage page, ElemVector const& elements) : Object(page, "expr")
    {
        ulong   max = 0;
        string word;
        string expr = toString(elements);
        istringstream iss(expr);
        while(iss >> word)
        {
            if(word[0] == 'i')
            {
                if(isdigit(word[1]))
                {
                    ulong num = (ulong)atol(word.c_str()+1);
                    if(max < num)
                    {
                        max = num;
                    }
                }
            }
        }
        m_values.resize(max, 0.);
        for(size_t i = 0; i < m_values.size(); i++)
        {
            m_parser.DefineVar("i" + toString(i+1), &m_values[i]);
            addInlet(Io::Message, Io::Hot, "i" + toString(i+1) + " (int or float)");
        }
        addOutlet(Io::Message, "result (float)");
        m_parser.SetExpr(expr);
    }
    
    Expression::~Expression()
    {
        
    }
    
    bool Expression::receive(size_t index, ElemVector const& elements)
    {
        if(index < m_values.size() && !elements.empty())
        {
            if(elements[0].isLong() || elements[0].isDouble())
            {
                double result;
                m_values[index] = elements[0];
                try
                {
                    result = m_parser.Eval();
                }
                catch (mu::Parser::exception_type &e)
                {
                    cout << e.GetMsg() << endl;
                }
                
                send(0, {result});
                Console::post(getShared(), " = " + toString(result));
                return true;
            }
        }
        return false;
    }*/
    
}

