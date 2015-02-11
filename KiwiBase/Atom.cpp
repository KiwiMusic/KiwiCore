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
#include "Tag.h"
#include "Dico.h"
#include "Object.h"

namespace Kiwi
{
    const size_t Atom::Quark::NothingCode  = typeid(Kiwi::Atom).hash_code();
    const size_t Atom::Quark::LongCode     = typeid(Kiwi::Atom::Long).hash_code();
    const size_t Atom::Quark::DoubleCode   = typeid(Kiwi::Atom::Double).hash_code();
    const size_t Atom::Quark::TagCode      = typeid(Kiwi::Tag).hash_code();
    const size_t Atom::Quark::ObjectCode   = typeid(Kiwi::Object).hash_code();
    const size_t Atom::Quark::DicoCode     = typeid(Kiwi::Dico).hash_code();
    
    long Atom::Quark::getLong() const noexcept
    {
        const size_t type = getType();
        if(type == Atom::Quark::LongCode)
        {
            return (reinterpret_cast<const Kiwi::Atom::Long*>(this))->val;
        }
        else if(type == Atom::Quark::DoubleCode)
        {
            return (reinterpret_cast<const Kiwi::Atom::Double*>(this))->val;
        }
        else
        {
            return 0;
        }
    }
    
    double Atom::Quark::getDouble() const noexcept
    {
        const size_t type = getType();
        if(type == Atom::Quark::DoubleCode)
        {
            return (reinterpret_cast<const Kiwi::Atom::Double*>(this))->val;
        }
        else if(type == Atom::Quark::LongCode)
        {
            return (reinterpret_cast<const Kiwi::Atom::Long*>(this))->val;
        }
        else
        {
            return 0;
        }
    }
    
    Atom::Atom() noexcept
    {
        m_quark = make_shared<Long>((long)0);
    }
    
    Atom::Atom(const bool value) noexcept
    {
        m_quark = make_shared<Long>((long)value);
    }
    
    Atom::Atom(const int value) noexcept
    {
        m_quark = make_shared<Long>((long)value);
    }
    
    Atom::Atom(const long value) noexcept
    {
        m_quark = make_shared<Long>(value);
    }
    
    Atom::Atom(const ulong value) noexcept
    {
        m_quark = make_shared<Long>((long)value);
    }
    
    Atom::Atom(const float value) noexcept
    {
        m_quark = make_shared<Double>((double)value);
    }
    
    Atom::Atom(const double value) noexcept
    {
        m_quark = make_shared<Double>(value);
    }
    
    Atom::Atom(const char* tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(Tag::create(tag));
    }
    
    Atom::Atom(string const& tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(Tag::create(tag));
    }
    
    Atom::Atom(sTag tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(tag);
    }
    
    Atom::Atom(sObject object) noexcept
    {
        m_quark = object;
    }
    
    Atom::Atom(sDico dico) noexcept
    {
        m_quark = dico;
    }
    
    Atom::Atom(Atom::Atom const& other) noexcept
    {
        m_quark = other.m_quark;
    }
    
    Atom::~Atom() noexcept
    {
        ;
    }
    
    Atom::operator sTag() const noexcept
    {
        return dynamic_pointer_cast<Tag>(m_quark);
    }
    
    //! Cast the atom to a object.
    /** The function casts the atom to a object.
     @return An object if the atom is a object otherwise a nullptr.
     */
    Atom::operator scObject() const noexcept
    {
        return dynamic_pointer_cast<Object>(m_quark);
    }
    
    //! Cast the atom to a object.
    /** The function casts the atom to a object.
     @return An object if the atom is a object otherwise a nullptr.
     */
    Atom::operator sObject() noexcept
    {
        return dynamic_pointer_cast<Object>(m_quark);
    }
    
    //! Cast the atom to a dico.
    /** The function casts the atom to a dico.
     @return An dico if the atom is a dico otherwise a nullptr.
     */
    Atom::operator scDico() const noexcept
    {
        return dynamic_pointer_cast<Dico>(m_quark);
    }
    
    //! Cast the atom to a dico.
    /** The function casts the atom to a dico.
     @return An dico if the atom is a dico otherwise a nullptr.
     */
    Atom::operator sDico() noexcept
    {
        return dynamic_pointer_cast<Dico>(m_quark);
    }
    
    Atom& Atom::operator=(char const* tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(Tag::create(tag));
        return *this;
    }
    
    Atom& Atom::operator=(string const& tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(Tag::create(tag));
        return *this;
    }
    
    Atom& Atom::operator=(sTag tag) noexcept
    {
        m_quark = const_pointer_cast<Tag>(tag);
        return *this;
    }
    
    Atom& Atom::operator=(sObject object) noexcept
    {
        m_quark = object;
        return *this;
    }
    
    Atom& Atom::operator=(sDico dico) noexcept
    {
        m_quark = dico;
        return *this;
    }
    
    bool Atom::operator==(char const* tag) const noexcept
    {
        return m_quark == Tag::create(tag);
    }
    
    bool Atom::operator==(string const& tag) const noexcept
    {
        return m_quark == Tag::create(tag);
    }
    
    bool Atom::operator==(sTag tag) const noexcept
    {
        return m_quark == tag;
    }

    bool Atom::operator==(scObject object) const noexcept
    {
        return m_quark == object;
    }
    
    bool Atom::operator==(scDico dico) const noexcept
    {
        return m_quark == dico;
    }
    
    string toString(Atom const& __val)
    {
        switch(__val.getType())
        {
            case Atom::LONG:
                return toString((long)__val);
                break;
            case Atom::DOUBLE:
                return toString((double)__val);
                break;
            case Atom::TAG:
                return toString((sTag)__val);
                break;
            case Atom::OBJECT:
                return toString((scObject)__val);
                break;
            case Atom::DICO:
                return toString((scDico)__val);
                break;
            default:
                return "";
                break;
        }
    }
    
    string toString(vector<Atom> const& __val)
    {
        if(!__val.empty())
        {
            string desc("[");
            for(size_t i = 0; i < __val.size() - 1; i++)
            {
                desc += toString(__val[i]) + ", ";
            }
            desc += toString(__val[__val.size() - 1]);
            return desc + "]";
        }
        return "";
    }
}
















