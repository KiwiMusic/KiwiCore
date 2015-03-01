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

#include "KiwiAtom.h"

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
        else if(type == BOOLEAN)
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
        else if(type == BOOLEAN)
        {
            return double((reinterpret_cast<const QuarkBool*>(this))->val);
        }
        else
        {
            return 0;
        }
    }
    
    sTag Atom::Quark::getTag() const
    {
        return (reinterpret_cast<const Kiwi::Atom::QuarkTag*>(this))->val;
    }
    
    Vector& Atom::Quark::getVector()
    {
        return (reinterpret_cast<QuarkVector*>(this))->val;
    }
    
    Vector const& Atom::Quark::getVector() const
    {
        return (reinterpret_cast<const QuarkVector*>(this))->val;
    }
    
    Dico& Atom::Quark::getDico()
    {
        return (reinterpret_cast<QuarkDico*>(this))->val;
    }
    
    Dico const& Atom::Quark::getDico() const
    {
        return (reinterpret_cast<const QuarkDico*>(this))->val;
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
        if(other.isBool())
        {
            m_quark = new QuarkBool((bool)other);
        }
        else if(other.isLong())
        {
            m_quark = new QuarkLong((long)other);
        }
        else if(other.isDouble())
        {
            m_quark = new QuarkDouble((double)other);
        }
        else if(other.isTag())
        {
            m_quark = new QuarkTag((sTag)other);
        }
        else if(other.isVector())
        {
            m_quark = new QuarkVector((Vector const&)other);
        }
        else if(other.isDico())
        {
            m_quark = new QuarkDico((Dico const&)other);
        }
        else
        {
            m_quark = new Quark();
        }
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
    
    Atom& Atom::operator=(Atom const& other) noexcept
    {
        delete m_quark;
        if(other.isBool())
        {
            m_quark = new QuarkBool((bool)other);
        }
        else if(other.isLong())
        {
            m_quark = new QuarkLong((long)other);
        }
        else if(other.isDouble())
        {
            m_quark = new QuarkDouble((double)other);
        }
        else if(other.isTag())
        {
            m_quark = new QuarkTag((sTag)other);
        }
        else if(other.isVector())
        {
            m_quark = new QuarkVector((Vector const&)other);
        }
        else if(other.isDico())
        {
            m_quark = new QuarkDico((Dico const&)other);
        }
        else
        {
            m_quark = new Quark();
        }
        return *this;
    }
    
    ostream& Atom::toJson(ostream &output, const Atom &atom, ulong& indent)
    {
        if(atom.isBool())
        {
            output << (bool)atom;
        }
        else if(atom.isLong())
        {
            output << (long)atom;
        }
        else if(atom.isDouble())
        {
            output << (double)atom;
        }
        else if(atom.isTag())
        {
            output << jsonEscape(((sTag)atom)->getName());
        }
        else if(atom.isVector())
        {
            Vector const& vec = atom;
            output << '[';
            for(Vector::size_type i = 0; i < vec.size();)
            {
                toJson(output, vec[i], indent);
                if(++i != vec.size())
                {
                    output << ", ";
                }
            }
            output << ']';
        }
        else if(atom.isDico())
        {
            Dico const& dico = atom;
            output << '{' << endl;
            ++indent;
            for(auto it = dico.begin(); it != dico.end();)
            {
                for(ulong i = 0; i < indent; i++)
                {
                    output << '\t';
                }
                output << jsonEscape(it->first->getName()) << " : ";
                toJson(output, it->second, indent);
                if(++it != dico.end())
                {
                    output << ',' << endl;
                }
                else
                {
                    output << endl;
                }
            }
            --indent;
            for(ulong i = 0; i < indent; i++)
            {
                output << '\t';
            }
            output << '}';
        }
        return output;
    }
    
    ostream& operator<<(ostream &output, const Atom &atom)
    {
        const bool boolalpha = output.flags() & ios::boolalpha;
        if(!boolalpha)
        {
            output << std::boolalpha;
        }
        ulong indent = 0;
        Atom::toJson(output, atom, indent);
        if(!boolalpha)
        {
            output << std::noboolalpha;
        }
        return output;
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
















