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

#ifndef __DEF_KIWI_CORE_ATOM__
#define __DEF_KIWI_CORE_ATOM__

#include "Defs.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! The atom class
    /**
     The atom is a base class that you should inherite from if you want to able to pass you class in an atom vector or in a dico. The default atoms are the long, the double, the tag, the dico and the object.
     */
    class Atom
    {
    public:
        class Quark;
        typedef shared_ptr<Quark>       sQuark;
        typedef shared_ptr<const Quark> scQuark;
        // ================================================================================ //
        //                                      QUARK                                       //
        // ================================================================================ //
        
        class Quark
        {
            friend Atom;
        public:
            
            virtual ~Quark() noexcept
            {
                ;
            }
            
        private:
            
            static const size_t NothingCode;
            static const size_t LongCode;
            static const size_t DoubleCode;
            static const size_t TagCode;
            static const size_t ObjectCode;
            static const size_t DicoCode;
            
            inline size_t getType() const noexcept
            {
                return typeid(*this).hash_code();
            }
            
            inline bool isNothing() const noexcept
            {
                return getType() == NothingCode;
            }
            
            inline bool isLong() const noexcept
            {
                return getType() == LongCode;
            }
            
            inline bool isDouble() const noexcept
            {
                return getType() == DoubleCode;
            }
            
            inline bool isNumber() const noexcept
            {
                return isLong() || isDouble();
            }
            
            inline bool isTag() const noexcept
            {
                return getType() == TagCode;
            }
            
            inline bool isDico() const noexcept
            {
                return getType() == DicoCode;
            }
            
            inline bool isObject() const noexcept
            {
                return getType() == ObjectCode;
            }
            
            long getLong() const noexcept;
            
            double getDouble() const noexcept;
        };
        
    private:
        // ================================================================================ //
        //                                     ATOM LONG                                    //
        // ================================================================================ //
        
        //! The long
        /**
         ...
         */
        class Long : public Quark
        {
        public:
            const long val;
            Long(long const& _val) noexcept : val(_val){;}
        };
        
        // ================================================================================ //
        //                                     ATOM DOUBLE                                  //
        // ================================================================================ //
        
        //! The double
        /**
         ...
         */
        class Double : public Quark
        {
        public:
            const double val;
            Double(double const& _val) noexcept : val(_val){;}
        };
        
        shared_ptr<Quark> m_quark;
        
    public:
        
        enum Type
        {
            NOTHING   = 0,
            LONG      = 1,
            DOUBLE    = 2,
            TAG       = 3,
            OBJECT    = 4,
            DICO      = 5,
            VECTOR    = 6
        };
        // ================================================================================ //
        //                                      ATOM                                        //
        // ================================================================================ //
        
        //! Constructor.
        /** The function allocates the atom with a zero long value.
         */
        Atom() noexcept;
        
        //! Constructor with a boolean value.
        /** The function allocates the atom with a long value created with a boolean value.
         @param value The value.
         */
        Atom(const bool value) noexcept;
        
        //! Constructor with an int value.
        /** The function allocates the atom with a long value created with an int value.
         @param value The value.
         */
        Atom(const int value) noexcept;
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        Atom(const long value) noexcept;
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        Atom(const ulong value) noexcept;
        
        //! Constructor with a float value.
        /** The function allocates the atom with a double value created with a float value.
         @param value The value.
         */
        Atom(const float value) noexcept;
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
         @param value The value.
         */
        Atom(const double value) noexcept;
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        Atom(const char* tag) noexcept;
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        Atom(string const& tag) noexcept;
        
        //! Constructor with a tag.
        /** The function allocates the atom with a tag.
         */
        Atom(sTag tag) noexcept;
        
        //! Constructor with an object.
        /** The function allocates the atom with an object.
         */
        Atom(sObject object) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the atom with a dico.
         */
        Atom(sDico dico) noexcept;
        
        //! Constructor with another atom.
        /** The function allocates the atom with an atom.
         */
        Atom(Atom const& other) noexcept;
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        ~Atom() noexcept;
        
        //! Retrieve the type of the atom.
        /** The function retrieves the type of the atom.
         @return The type of the atom as a type.
         */
        inline Type getType() const noexcept
        {
            const size_t type = m_quark->getType();
            if(type == Quark::LongCode)
            {
                return LONG;
            }
            else if(type == Quark::DoubleCode)
            {
                return DOUBLE;
            }
            else if(type == Quark::TagCode)
            {
                return TAG;
            }
            else if(type == Quark::DicoCode)
            {
                return DICO;
            }
            else
            {
                return OBJECT;
            }
        }
        
        //! Check if the atom is of type long.
        /** The function checks if the atom is of type long.
         @return    true if the atom is a long.
         @see isDouble, isNumber
         */
        inline bool isLong() const noexcept
        {
            return m_quark->isLong();
        }
        
        //! Check if the atom is of type double.
        /** The function checks if the atom is of type double.
         @return    true if the atom is a double.
         @see isLong, isNumber
         */
        inline bool isDouble() const noexcept
        {
            return m_quark->isDouble();
        }
        
        //! Checks if the atom is of type long or double.
        /** The function checks if the atom is of type long or double.
         @return    true if the atom is a long or a double.
         */
        inline bool isNumber() const noexcept
        {
            return m_quark->isNumber();
        }
        
        //! Check if the atom is of type tag.
        /** The function checks if the atom is of type tag.
         @return    true if the atom is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_quark->isTag();
        }
        
        //! Check if the atom is of type object.
        /** The function checks if the atom is of type object.
         @return    true if the atom is a object.
         */
        inline bool isObject() const noexcept
        {
            return m_quark->isObject();
        }
        
        //! Check if the atom is of type dico.
        /** The function checks if the atom is of type dico.
         @return    true if the atom is a dico.
         */
        inline bool isDico() const noexcept
        {
            return m_quark->isDico();
        }
        
        //! Cast the atom to a boolean.
        /** The function casts the atom to a boolean.
         @return An boolean value if the atom is a digit otherwise 0.
         */
        inline operator bool() const noexcept
        {
            return (bool)m_quark->getLong();
        }
        
        //! Cast the atom to an int.
        /** The function casts the atom to an int.
         @return An int value if the atom is a digit otherwise 0.
         */
        inline operator int() const noexcept
        {
            return (int)m_quark->getLong();
        }
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator long() const noexcept
        {
            return m_quark->getLong();
        }
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator ulong() const noexcept
        {
            return (ulong)m_quark->getLong();
        }
        
        //! Cast the atom to a float.
        /** The function casts the atom to a float.
         @return A float value if the atom is a digit otherwise 0.
         */
        inline operator float() const noexcept
        {
            return (float)m_quark->getDouble();
        }
        
        //! Cast the atom to a double.
        /** The function casts the atom to a double.
         @return A double value if the atom is a digit otherwise 0.
         */
        inline operator double() const noexcept
        {
            return m_quark->getDouble();
        }
        
        //! Cast the atom to a tag.
        /** The function casts the atom to a tag.
         @return A tag if the atom is a tag otherwise a nullptr.
         */
        operator sTag() const noexcept;
        
        //! Cast the atom to a object.
        /** The function casts the atom to a object.
         @return An object if the atom is a object otherwise a nullptr.
         */
        operator scObject() const noexcept;
        
        //! Cast the atom to a object.
        /** The function casts the atom to a object.
         @return An object if the atom is a object otherwise a nullptr.
         */
        operator sObject() noexcept;
        
        //! Cast the atom to a dico.
        /** The function casts the atom to a dico.
         @return An dico if the atom is a dico otherwise a nullptr.
         */
        operator scDico() const noexcept;
        
        //! Cast the atom to a dico.
        /** The function casts the atom to a dico.
         @return An dico if the atom is a dico otherwise a nullptr.
         */
        operator sDico() noexcept;
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        inline Atom& operator=(Atom const& other) noexcept
        {
            m_quark = other.m_quark;
            return *this;
        }
        
        //! Set up the atom with a boolean value.
        /** The function sets up the atom with a long value created with aboolean value.
         @param value   The boolean value.
         @return An atom.
         */
        inline Atom& operator=(const bool value) noexcept
        {
            m_quark = make_shared<Long>((long)value);
            return *this;
        }
        
        //! Set up the atom with a int value.
        /** The function sets up the atom with a long value created with aint value.
         @param value   The int value.
         @return An atom.
         */
        inline Atom& operator=(const int value) noexcept
        {
            m_quark = make_shared<Long>((long)value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const long value) noexcept
        {
            m_quark = make_shared<Long>(value);
            return *this;
        }
        
        //! Set up the atom with a ulong value.
        /** The function sets up the atom with a ulong value.
         @param value   The ulong value.
         @return An atom.
         */
        inline Atom& operator=(const ulong value) noexcept
        {
            m_quark = make_shared<Long>((long)value);
            return *this;
        }
        
        //! Set up the atom with a float value.
        /** The function sets up the atom with a double value created with afloat value.
         @param value   The float value.
         @return An atom.
         */
        inline Atom& operator=(const float value) noexcept
        {
            m_quark = make_shared<Double>((double)value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline Atom& operator=(const double value) noexcept
        {
            m_quark = make_shared<Double>(value);
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        Atom& operator=(char const* tag) noexcept;
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        Atom& operator=(string const& tag) noexcept;
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        Atom& operator=(sTag tag) noexcept;
        
        //! Set up the atom with a object.
        /** The function sets up the atom with a object.
         @param object   The object.
         @return An atom.
         */
        Atom& operator=(sObject object) noexcept;
        
        //! Set up the atom with a dico.
        /** The function sets up the atom with a dico.
         @param dico   The dico.
         @return An atom.
         */
        Atom& operator=(sDico dico) noexcept;
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms hold the same value otherwise false.
         */
        inline bool operator==(Atom const& other) const noexcept
        {
            size_t type = m_quark->getType();
            if(type == Atom::Quark::LongCode || type == Atom::Quark::DoubleCode)
            {
                return other == m_quark->getDouble();
            }
            else
            {
                return other.m_quark == m_quark;
            }
        }
        
        //! Compare the atom with a boolean value.
        /** The function compares the atom with a boolean value.
         @param value   The boolean value.
         @return true if the atom hold the boolean value otherwise false.
         */
        inline bool operator==(const bool value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with an int value.
        /** The function compares the atom with an int value.
         @param value   The int value.
         @return true if the atom hold the int value otherwise false.
         */
        inline bool operator==(const int value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom hold the long value otherwise false.
         */
        inline bool operator==(const long value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with an ulong value.
        /** The function compares the atom with an unsigned  long.
         @param value   The ulong value.
         @return true if the atom hold the ulong value otherwise false.
         */
        inline bool operator==(const ulong value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a float value.
        /** The function compares the atom with a float value.
         @param value   The float value.
         @return true if the atom hold the float value otherwise false.
         */
        inline bool operator==(const float value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a double value.
        /** The function compares the atom with a double value.
         @param value   The double value.
         @return true if the atom hold the double value otherwise false.
         */
        inline bool operator==(const double value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the tag create with the string otherwise false.
         */
        bool operator==(char const* tag) const noexcept;
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the tag create with the string otherwise false.
         */
        bool operator==(string const& tag) const noexcept;
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param value   The tag.
         @return true if the atom hold the tag otherwise false.
         */
        bool operator==(sTag tag) const noexcept;
        
        //! Compare the atom with a object.
        /** The function compares the atom with a object.
         @param object   The object.
         @return true if the atom hold the object otherwise false.
         */
        bool operator==(scObject object) const noexcept;
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom hold the dico otherwise false.
         */
        bool operator==(scDico dico) const noexcept;
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms differ otherwise false.
         */
        inline bool operator!=(const Atom& other) const noexcept
        {
            return !(*this == other);
        }
        
        //! Compare the atom with a boolean value.
        /** The function compares the atom with a boolean value.
         @param value   The boolean value.
         @return true if the atom differ from the boolean value otherwise false.
         */
        inline bool operator!=(const bool value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with an int value.
        /** The function compares the atom with an int value.
         @param value   The int value.
         @return true if the atom differ from the int value otherwise false.
         */
        inline bool operator!=(const int value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom differ from the long value otherwise false.
         */
        inline bool operator!=(const long value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a float value.
        /** The function compares the atom with a float value.
         @param value   The float value.
         @return true if the atom differ from the float value otherwise false.
         */
        inline bool operator!=(const float value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a double value.
        /** The function compares the atom with a double value.
         @param value   The double value.
         @return true if the atom differ from the double value otherwise false.
         */
        inline bool operator!=(const double value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(char const* tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(string const& tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param value   The tag.
         @return true if the atom differ from the tag otherwise false.
         */
        inline bool operator!=(sTag tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a object.
        /** The function compares the atom with a object.
         @param object   The object.
         @return true if the atom differ from the object otherwise false.
         */
        inline bool operator!=(scObject object) const noexcept
        {
            return !(*this == object);
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom differ from the dico otherwise false.
         */
        bool operator!=(scDico dico) const noexcept
        {
            return !(*this == dico);
        }
        
        static vector<Atom>& evaluate(string const& _text);
    };
    
    string toString(Atom const& __val);
    string toString(vector<Atom> const& __val);
}



#endif


