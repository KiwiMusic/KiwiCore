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
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //    

    shared_ptr<Object> Plus::create(shared_ptr<Tag> name, vector<Element>& elements)
    {
        shared_ptr<Plus> plus =  static_pointer_cast<Plus>(allocObject("+"));
        
        plus->addMethod("bang",     T_NOTHING,  (Method)receiveBang);
        plus->addMethod("long",     T_LONG,     (Method)receiveLong);
        plus->addMethod("double",   T_DOUBLE,   (Method)receiveDouble);
        
        plus->m_augend = 0;
        plus->m_addend = 0;
        plus->m_double = false;
        if(elements.size())
        {
            plus->m_double = elements[0].isDouble();
            plus->m_addend = (double)elements[0];
        }
        
        plus->addInlet("bang", "long", "double");
        plus->addInlet("long", "double");
        if(plus->m_double)
            plus->addOutlet("double");
        else
            plus->addOutlet("long");
        
        plus->setInletAttributes(0, "Augend", InletPolarity::Hot);
        plus->setInletAttributes(1, "Addend", InletPolarity::Cold);
        plus->setOutletDescription(0, "Sum");
        
        return shared_ptr<Object>(static_pointer_cast<Box>(plus));
    }
    
    shared_ptr<Object> Plus::create(shared_ptr<Instance> kiwi, shared_ptr<Tag> name, vector<Element>& elements) const
    {
        shared_ptr<Plus> plus =  static_pointer_cast<Plus>(allocObject("+"));
        
        plus->addMethod("bang",     T_NOTHING,  (Method)receiveBang);
        plus->addMethod("long",     T_LONG,     (Method)receiveLong);
        plus->addMethod("double",   T_DOUBLE,   (Method)receiveDouble);
        
        plus->m_augend = 0;
        plus->m_addend = 0;
        plus->m_double = false;
        if(elements.size())
        {
            plus->m_double = elements[0].isDouble();
            plus->m_addend = (double)elements[0];
        }
        
        plus->addInlet("bang", "long", "double");
        plus->addInlet("long", "double");
        if(plus->m_double)
            plus->addOutlet("double");
        else
            plus->addOutlet("long");
        
        plus->setInletAttributes(0, "Augend", InletPolarity::Hot);
        plus->setInletAttributes(1, "Addend", InletPolarity::Cold);
        plus->setOutletDescription(0, "Sum");
        
        return shared_ptr<Object>(static_pointer_cast<Box>(plus));
    }
    
    void Plus::receiveBang(shared_ptr<Plus> plus)
    {
#ifdef DEBUG
        string message = to_string(plus->m_augend) + string(" ") + to_string(plus->m_addend) + string(" = ") + to_string(plus->m_augend + plus->m_addend);
        
        plus->postObject(message);
#endif
        if(plus->m_double)
            plus->sendDouble(0, plus->m_augend + plus->m_addend);
        else
            plus->sendLong(0, plus->m_augend +plus-> m_addend);
    }
    
    void Plus::receiveLong(shared_ptr<Plus> plus, long value)
    {
        if(plus->getProxy() == 0)
        {
            plus->m_augend = value;
            plus->receiveBang(plus);
        }
        else
            plus->m_addend = value;
    }
    
    void Plus::receiveDouble(shared_ptr<Plus> plus, double value)
    {
        if(plus->getProxy() == 0)
        {
            plus->m_augend = value;
            plus->receiveBang(plus);
        }
        else
            plus->m_addend = value;
    }
    
    // ================================================================================ //
    //                                      MINUS                                       //
    // ================================================================================ //
    /*
    Minus::Minus(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_minuend(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Minuend"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Subtrahend"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Difference"));
        if(m_double)
            m_subtrahend = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_subtrahend = (long)elements[0];
        else
            m_subtrahend = 0.;
    }
    
    Minus::~Minus()
    {
        ;
    }
    
    void Minus::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(m_minuend - m_subtrahend);
        else
            getOutlet(0)->sendLong(m_minuend - m_subtrahend);
    }
    
    void Minus::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Minus::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_minuend = value;
            output();
        }
        else
            m_subtrahend = value;
    }
    
    void Minus::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_minuend = value;
            output();
        }
        else
            m_subtrahend = value;
    }
    
    // ================================================================================ //
    //                                      TIMES                                       //
    // ================================================================================ //
    
    Times::Times(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_multiplicand(1.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Multiplicand"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Multiplier"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Product"));
        if(m_double)
            m_multiplier = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_multiplier = (long)elements[0];
        else
            m_multiplier = 0.;
    }
    
    Times::~Times()
    {
        ;
    }
    
    void Times::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(m_multiplicand * m_multiplier);
        else
            getOutlet(0)->sendLong(m_multiplicand * m_multiplier);
    }
    
    void Times::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Times::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_multiplicand = value;
            output();
        }
        else
            m_multiplier = value;
    }
    
    void Times::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_multiplicand = value;
            output();
        }
        else
            m_multiplier = value;
    }
    
    // ================================================================================ //
    //                                     DIVIDE                                       //
    // ================================================================================ //
    
    Divide::Divide(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_dividend(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Dividend"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Divisor"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Quotient"));
        if(m_double)
            m_divisor = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_divisor = (long)elements[0];
        else
            m_divisor = 1.;
    }
    
    Divide::~Divide()
    {
        ;
    }
    
    void Divide::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(m_dividend / m_divisor);
        else
            getOutlet(0)->sendLong(m_dividend / m_divisor);
    }
    
    void Divide::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Divide::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_dividend = value;
            output();
        }
        else
            m_divisor = value;
    }
    
    void Divide::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_dividend = value;
            output();
        }
        else
            m_divisor = value;
    }
    
    // ================================================================================ //
    //                                     MODULO                                       //
    // ================================================================================ //
    
    Modulo::Modulo(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_dividend(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Dividend"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Divisor"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Remainder"));
        if(m_double)
            m_divisor = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_divisor = (long)elements[0];
        else
            m_divisor = 1.;
    }
    
    Modulo::~Modulo()
    {
        ;
    }
    
    void Modulo::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(fmod(m_dividend, m_divisor));
        else
            getOutlet(0)->sendLong((long)m_dividend % (long)m_divisor);
    }
    
    void Modulo::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Modulo::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_dividend = value;
            output();
        }
        else
            m_divisor = value;
    }
    
    void Modulo::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_dividend = value;
            output();
        }
        else
            m_divisor = value;
    }
    
    // ================================================================================ //
    //                                     POWER                                        //
    // ================================================================================ //
    
    Power::Power(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_base(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Base"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Exponent"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Power"));
        if(m_double)
            m_exponent = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_exponent = (long)elements[0];
        else
            m_exponent = 0.;
    }
    
    Power::~Power()
    {
        ;
    }
    
    void Power::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(pow(m_base, m_exponent));
        else
            getOutlet(0)->sendLong(pow(m_base, m_exponent));
    }
    
    void Power::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Power::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_base = value;
            output();
        }
        else
            m_exponent = value;
    }
    
    void Power::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_base = value;
            output();
        }
        else
            m_exponent = value;
    }
    
    // ================================================================================ //
    //                                     SQUARE                                       //
    // ================================================================================ //
    
    Square::Square(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_radiacand(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Radicand"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Square Root"));
        if(elements.size() && (elements[0].type() == S_LONG || elements[0].type() == S_DOUBLE))
            m_radiacand = (double)elements[0];
    }
    
    Square::~Square()
    {
        ;
    }
    
    void Square::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(sqrt(m_radiacand));
        else
            getOutlet(0)->sendLong(sqrt(m_radiacand));
    }
    
    void Square::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Square::receiveLong(const Inlet* inlet, long value)
    {
        m_radiacand = value;
        output();
    }
    
    void Square::receiveDouble(const Inlet* inlet, double value)
    {
        m_radiacand = value;
        output();
    }
    
    // ================================================================================ //
    //                                     ROOT                                         //
    // ================================================================================ //
    
    Root::Root(KiwiInstance* kiwi, const string name, vector<Element>& elements) : Object(kiwi, name, elements), m_double(elements.size() && elements[0].type() == S_DOUBLE), m_radiacand(0.)
    {
        addInlet((0 | S_BANG | S_DOUBLE | S_LONG), std::string("Radicand"));
        addInlet((0 | S_DOUBLE | S_LONG), std::string("Degree"));
        addOutlet((0 | S_DOUBLE | S_LONG), std::string("Nth Root"));
        if(m_double)
            m_degree = (double)elements[0];
        else if(elements.size() && elements[0].type() == S_LONG)
            m_degree = (long)elements[0];
        else
            m_degree = 1.;
    }
    
    Root::~Root()
    {
        ;
    }
    
    void Root::output()
    {
        if(m_double)
            getOutlet(0)->sendDouble(pow(m_radiacand, 1. / m_degree));
        else
            getOutlet(0)->sendLong(pow(m_radiacand, 1. / m_degree));
    }
    
    void Root::receiveBang(const Inlet* inlet)
    {
        if(inlet->getIndex() == 0)
            output();
        else
            warning(this, std::string("shouldn't receive a bang from the right inlet !"));
    }
    
    void Root::receiveLong(const Inlet* inlet, long value)
    {
        if(inlet->getIndex() == 0)
        {
            m_radiacand = value;
            output();
        }
        else
            m_degree = value;
    }
    
    void Root::receiveDouble(const Inlet* inlet, double value)
    {
        if(inlet->getIndex() == 0)
        {
            m_radiacand = value;
            output();
        }
        else
            m_degree = value;
    }*/
}

