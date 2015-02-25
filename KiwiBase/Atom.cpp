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

#include "Atom.h"
#include "Console.h"

namespace Kiwi
{
    Atom::Quark::Quark() noexcept
    {
        ;
    }
    
    Atom::Quark::~Quark() noexcept
    {
        ;
    }
    
    bool Atom::Quark::getBool() const noexcept
    {
        const size_t type = getType();
        if(type == BOOLEAN)
        {
            return (reinterpret_cast<const QuarkBool*>(this))->val;
        }
        else if(type == LONG)
        {
            return bool((reinterpret_cast<const QuarkLong*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return bool((reinterpret_cast<const QuarkDouble*>(this))->val);
        }
        else
        {
            return false;
        }
    }
    
    long Atom::Quark::getLong() const noexcept
    {
        const size_t type = getType();
        if(type == LONG)
        {
            return (reinterpret_cast<const QuarkLong*>(this))->val;
        }
        else if(type == DOUBLE)
        {
            return long((reinterpret_cast<const QuarkDouble*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return long((reinterpret_cast<const QuarkBool*>(this))->val);
        }
        else
        {
            return 0;
        }
    }
    
    double Atom::Quark::getDouble() const noexcept
    {
        const size_t type = getType();
        if(type == DOUBLE)
        {
            return (reinterpret_cast<const QuarkDouble*>(this))->val;
        }
        else if(type == LONG)
        {
            return double((reinterpret_cast<const QuarkLong*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return double((reinterpret_cast<const QuarkBool*>(this))->val);
        }
        else
        {
            return 0;
        }
    }
    
    sTag Atom::Quark::getTag() const noexcept
    {
        if(isTag())
        {
            return (reinterpret_cast<const Kiwi::Atom::QuarkTag*>(this))->val;
        }
        else
        {
            return nullptr;
        }
    }
    
    Vector Atom::Quark::getVector() const noexcept
    {
        if(getType() == VECTOR)
        {
            return (reinterpret_cast<const QuarkVector*>(this))->val;
        }
        else
        {
            Vector* zaza= new Vector(0);
            return *zaza;
        }
    }
    
    Dico Atom::Quark::getDico() const noexcept
    {
        if(getType() == DICO)
        {
            return (reinterpret_cast<const QuarkDico*>(this))->val;
        }
        else
        {
            return Dico();
        }
    }
    
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    Atom::Atom() noexcept
    {
        m_quark = new Quark();
    }
    
    Atom::Atom(Atom::Atom const& other) noexcept
    {
        m_quark = other.m_quark;
    }
    
    Atom::Atom(const bool value) noexcept
    {
        m_quark = new QuarkBool(value);
    }
    
    Atom::Atom(const int value) noexcept
    {
         m_quark = new QuarkLong((long)value);
    }
    
    Atom::Atom(const long value) noexcept
    {
        m_quark = new QuarkLong(value);
    }
    
    Atom::Atom(const float value) noexcept
    {
        m_quark = new QuarkDouble((double)value);
    }
    
    Atom::Atom(const double value) noexcept
    {
        m_quark = new QuarkDouble(value);
    }
    
    Atom::Atom(const char* tag) noexcept
    {
        m_quark = new QuarkTag(Tag::create(tag));
    }
    
    Atom::Atom(string const& tag) noexcept
    {
        m_quark = new QuarkTag(Tag::create(tag));
    }
    
    Atom::Atom(sTag tag) noexcept
    {
        m_quark = new QuarkTag(tag);
    }
    
    Atom::Atom(Vector const& atoms) noexcept
    {
        m_quark = new QuarkVector(atoms);
    }
    
    Atom::Atom(Vector::iterator first, Vector::iterator last) noexcept
    {
        m_quark = new QuarkVector(first, last);
    }
        
    Atom::Atom(Vector&& atoms) noexcept
    {
        m_quark = new QuarkVector(atoms);
    }
    
    Atom::Atom(initializer_list<Atom> il) noexcept
    {
        m_quark = new QuarkVector(il);
    }
    
    Atom::Atom(Dico const& atoms) noexcept
    {
        m_quark = new QuarkDico(atoms);
    }
    
    Atom::Atom(Dico::iterator first, Dico::iterator last) noexcept
    {
        m_quark = new QuarkDico(first, last);
    }
    
    Atom::Atom(Dico&& atoms) noexcept
    {
        m_quark = new QuarkDico(atoms);
    }
    
    Atom::Atom(initializer_list<pair<const sTag, Atom>> il) noexcept
    {
        m_quark = new QuarkDico(il);
    }
    
    Atom::~Atom() noexcept
    {
        delete m_quark;
    }
    
    Atom Atom::evaluate(string const& _text)
    {
        Vector atoms;
        string word;
        istringstream iss(_text);
        while(iss >> word)
        {
            if(isdigit(_text[0]))
            {
                string::size_type pos = _text.find_first_not_of("-0123456789.");
                if(pos != string::npos)
                {
                    if(_text.find('.') == string::npos)
                    {
                        atoms.push_back(Atom(stol(_text.c_str())));
                    }
                    else
                    {
                        atoms.push_back(Atom(stod(_text.c_str())));
                    }
                }
            }
            else
            {
                atoms.push_back(Atom(Tag::create(jsonUnescape(_text))));
            }
        }
        return Atom();
    }
}
















