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
            return (reinterpret_cast<const Kiwi::Atom::Bool*>(this))->val;
        }
        else if(type == LONG)
        {
            return bool((reinterpret_cast<const Kiwi::Atom::Long*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return bool((reinterpret_cast<const Kiwi::Atom::Double*>(this))->val);
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
            return (reinterpret_cast<const Kiwi::Atom::Long*>(this))->val;
        }
        else if(type == DOUBLE)
        {
            return long((reinterpret_cast<const Kiwi::Atom::Double*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return long((reinterpret_cast<const Kiwi::Atom::Bool*>(this))->val);
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
            return (reinterpret_cast<const Kiwi::Atom::Double*>(this))->val;
        }
        else if(type == LONG)
        {
            return double((reinterpret_cast<const Kiwi::Atom::Long*>(this))->val);
        }
        else if(type == DOUBLE)
        {
            return double((reinterpret_cast<const Kiwi::Atom::Bool*>(this))->val);
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
    
    vector<Atom> Atom::Quark::getVector() const noexcept
    {
        if(getType() == VECTOR)
        {
            return (reinterpret_cast<const Kiwi::Atom::Vector*>(this))->val;
        }
        else
        {
            vector<Atom>* zaza= new vector<Atom>(0);
            return *zaza;
        }
    }
    
    map<sTag, Atom> Atom::Quark::getMap() const noexcept
    {
        if(getType() == MAP)
        {
            return (reinterpret_cast<const Kiwi::Atom::Map*>(this))->val;
        }
        else
        {
            return map<sTag, Atom>();
        }
    }
    
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    Atom::Atom() noexcept
    {
        m_quark = Quark();
    }
    
    Atom::Atom(Atom::Atom const& other) noexcept
    {
        m_quark = other.m_quark;
    }
    
    Atom::Atom(const bool value) noexcept
    {
        m_quark = Bool(value);
    }
    
    Atom::Atom(const int value) noexcept
    {
         m_quark = Long((long)value);
    }
    
    Atom::Atom(const long value) noexcept
    {
        m_quark = Long(value);
    }
    
    Atom::Atom(const float value) noexcept
    {
        m_quark = Double((double)value);
    }
    
    Atom::Atom(const double value) noexcept
    {
        m_quark = Double(value);
    }
    
    Atom::Atom(const char* tag) noexcept
    {
        m_quark = QuarkTag(Tag::create(tag));
    }
    
    Atom::Atom(string const& tag) noexcept
    {
        m_quark = QuarkTag(Tag::create(tag));
    }
    
    Atom::Atom(sTag tag) noexcept
    {
        m_quark = QuarkTag(tag);
    }
    
    Atom::Atom(vector<Atom> const& atoms) noexcept
    {
        m_quark = Vector(atoms);
    }
    
    Atom::Atom(vector<Atom>::iterator first, vector<Atom>::iterator last) noexcept
    {
        m_quark = Vector(first, last);
    }
        
    Atom::Atom(vector<Atom>&& atoms) noexcept
    {
        m_quark = Vector(atoms);
    }
    
    Atom::Atom(initializer_list<Atom> il) noexcept
    {
        m_quark = Vector(il);
    }
    
    Atom::Atom(map<sTag, Atom> const& atoms) noexcept
    {
        m_quark = Map(atoms);
    }
    
    Atom::Atom(map<sTag, Atom>::iterator first, map<sTag, Atom>::iterator last) noexcept
    {
        m_quark = Map(first, last);
    }
    
    Atom::Atom(map<sTag, Atom>&& atoms) noexcept
    {
        m_quark = Map(atoms);
    }
    
    Atom::Atom(initializer_list<pair<const sTag, Atom>> il) noexcept
    {
        m_quark = Map(il);
    }
    
    Atom::~Atom() noexcept
    {
        ;
    }
    
    Atom Atom::evaluate(string const& _text)
    {
        vector<Atom> atoms;
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
















