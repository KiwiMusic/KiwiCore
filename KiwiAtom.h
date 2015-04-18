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

#include "KiwiTag.h"

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
        
        enum Type
        {
            UNDEFINED = 0,
            BOOLEAN   = 1,
            LONG      = 2,
            DOUBLE    = 3,
            TAG       = 4,
            VECTOR    = 5,
            DICO      = 6
        };
        
    private:
        // ================================================================================ //
        //                                      QUARK                                       //
        // ================================================================================ //
        
        class Quark
        {
        public:
            inline Quark() noexcept
            {
                ;
            }
            
            virtual inline ~Quark() noexcept
            {
                ;
            }
            
            virtual inline Type getType() const noexcept
            {
                return UNDEFINED;
            }
            
            inline bool isUndefined() const noexcept
            {
                return getType() == UNDEFINED;
            }
            
            inline bool isBool() const noexcept
            {
                return getType() == BOOLEAN;
            }
            
            inline bool isLong() const noexcept
            {
                return getType() == LONG;
            }
            
            inline bool isDouble() const noexcept
            {
                return getType() == DOUBLE;
            }
            
            inline bool isNumber() const noexcept
            {
                return isLong() || isDouble() || isBool();
            }
            
            inline bool isTag() const noexcept
            {
                return getType() == TAG;
            }
            
            inline bool isDico() const noexcept
            {
                return getType() == DICO;
            }
            
            inline bool isVector() const noexcept
            {
                return getType() == VECTOR;
            }
            
            bool getBool() const noexcept;
            
            long getLong() const noexcept;
            
            double getDouble() const noexcept;
            
            sTag getTag() const;
            
            Vector& getVector();
            
            Vector const& getVector() const;
            
            Dico& getDico();
            
            Dico const& getDico() const;
        };
        
        // ================================================================================ //
        //                                     ATOM BOOL                                    //
        // ================================================================================ //
        
        //! The bool
        /**
         ...
         */
        class QuarkBool : public Quark
        {
        public:
            const bool val;
            
            QuarkBool(QuarkBool const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkBool(bool const& _val) noexcept : val(_val)
            {
                ;
            }
            
            inline Type getType() const noexcept override
            {
                return BOOLEAN;
            }
        };
        
        // ================================================================================ //
        //                                     ATOM LONG                                    //
        // ================================================================================ //
        
        //! The long
        /**
         ...
         */
        class QuarkLong : public Quark
        {
        public:
            const long val;
            
            QuarkLong(QuarkLong const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkLong(long const& _val) noexcept : val(_val)
            {
                ;
            }
            
            //! Retrieve the type of the quark.
            /** The function retrieves the type of the quark.
             @return The type of the quark;
             */
            inline Type getType() const noexcept override
            {
                return LONG;
            }
        };
        
        // ================================================================================ //
        //                                     ATOM DOUBLE                                  //
        // ================================================================================ //
        
        //! The double
        /**
         ...
         */
        class QuarkDouble : public Quark
        {
        public:
            const double val;
            
            QuarkDouble(QuarkDouble const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkDouble(double const& _val) noexcept : val(_val)
            {
                ;
            }
            
            //! Retrieve the type of the quark.
            /** The function retrieves the type of the quark.
             @return The type of the quark;
             */
            inline Type getType() const noexcept override
            {
                return DOUBLE;
            }
        };
        
        // ================================================================================ //
        //                                     ATOM TAG                                     //
        // ================================================================================ //
        
        //! The tag
        /**
         ...
         */
        class QuarkTag : public Quark
        {
        public:
            const sTag val;
            
            QuarkTag(QuarkTag const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkTag(sTag _val) noexcept : val(_val)
            {
                ;
            }
            
            //! Retrieve the type of the quark.
            /** The function retrieves the type of the quark.
             @return The type of the quark;
             */
            inline Type getType() const noexcept override
            {
                return TAG;
            }
        };
        
        // ================================================================================ //
        //                                     ATOM VECTOR                                  //
        // ================================================================================ //
        
        //! The vector
        /**
         ...
         */
        class QuarkVector : public Quark
        {
        public:
            Vector val;
            
            QuarkVector(QuarkVector const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkVector(Vector const& _val) noexcept : val(_val)
            {
                ;
            }
            
            QuarkVector(Vector::iterator first, Vector::iterator last) noexcept : val(first, last)
            {
                ;
            }
            
            QuarkVector(Vector&& atoms) noexcept : val(atoms)
            {
                ;
            }
            
            QuarkVector(initializer_list<Atom> il) noexcept : val(il)
            {
                ;
            }
            
            ~QuarkVector() noexcept
            {
                val.clear();
            }
            
            //! Retrieve the type of the quark.
            /** The function retrieves the type of the quark.
             @return The type of the quark;
             */
            inline Type getType() const noexcept override
            {
                return VECTOR;
            }
        };
        
        // ================================================================================ //
        //                                     ATOM DICO                                     //
        // ================================================================================ //
        
        //! The map
        /**
         ...
         */
        class QuarkDico : public Quark
        {
        public:
            Dico val;
            
            QuarkDico(QuarkDico const& _val) noexcept : val(_val.val)
            {
                ;
            }
            
            QuarkDico(Dico const& _val) noexcept : val(_val)
            {
                ;
            }
            
            QuarkDico(Dico::iterator first, Dico::iterator last) noexcept : val(first, last)
            {
                ;
            }
            
            QuarkDico(Dico&& atoms) noexcept : val(atoms)
            {
                ;
            }
            
            QuarkDico(initializer_list<pair<const sTag, Atom>> il) noexcept : val(il)
            {
                ;
            }
            
            ~QuarkDico() noexcept
            {
                val.clear();
            }
            
            //! Retrieve the type of the quark.
            /** The function retrieves the type of the quark.
             @return The type of the quark;
             */
            inline Type getType() const noexcept override
            {
                return DICO;
            }
        };
        
        Quark* m_quark;
        
    public:
        
        // ================================================================================ //
        //                                      ATOM                                        //
        // ================================================================================ //
        
        //! Constructor.
        /** The function allocates an undefined atom.
         */
        Atom() noexcept;
        
        //! Constructor with another atom.
        /** The function allocates the atom with an atom.
         */
        Atom(Atom const& other) noexcept;
        
        //! Constructor with a boolean value.
        /** The function allocates the atom with a long value created with a boolean value.
         @param value The value.
         */
        Atom(const bool value) noexcept;
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        Atom(const int value) noexcept;
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        Atom(const long value) noexcept;
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
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
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Vector const& atoms) noexcept;
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Vector::iterator first, Vector::iterator last) noexcept;
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Vector&& atoms) noexcept;
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(initializer_list<Atom> il) noexcept;
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Dico const& atoms) noexcept;
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Dico::iterator first, Dico::iterator last) noexcept;
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(Dico&& atoms) noexcept;
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        Atom(initializer_list<pair<const sTag, Atom>> il) noexcept;
        
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
            return m_quark->getType();
        }
        
        //! Check if the atom is undefined.
        /** The function checks if the atom is undefined.
         @return    true if the atom is undefined.
         */
        inline bool isUndefined() const noexcept
        {
            return m_quark->isLong();
        }
        
        //! Check if the atom is of type bool.
        /** The function checks if the atom is of type bool.
         @return    true if the atom is a bool.
         */
        inline bool isBool() const noexcept
        {
            return m_quark->isBool();
        }
        
        //! Check if the atom is of type long.
        /** The function checks if the atom is of type long.
         @return    true if the atom is a long.
         */
        inline bool isLong() const noexcept
        {
            return m_quark->isLong();
        }
        
        //! Check if the atom is of type double.
        /** The function checks if the atom is of type double.
         @return    true if the atom is a double.
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
        
        //! Check if the atom is of type vector.
        /** The function checks if the atom is of type vector.
         @return    true if the atom is a vector.
         */
        inline bool isVector() const noexcept
        {
            return m_quark->isVector();
        }
        
        //! Check if the atom is of type map.
        /** The function checks if the atom is of type map.
         @return    true if the atom is a map.
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
            return m_quark->getBool();
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
        inline operator sTag() const
        {
            return m_quark->getTag();
        }
        
        //! Cast the atom to a vector of atoms.
        /** The function casts the atom to a vector of atoms.
         @return A vector of atoms.
         */
        inline operator Vector&()
        {
            return m_quark->getVector();
        }
        
        //! Cast the atom to a vector of atoms.
        /** The function casts the atom to a vector of atoms.
         @return A vector of atoms.
         */
        inline operator Vector const&() const
        {
            return m_quark->getVector();
        }
        
        //! Cast the atom to a map of atoms.
        /** The function casts the atom to a map of atoms.
         @return A map of atoms.
         */
        inline operator Dico&()
        {
            return m_quark->getDico();
        }
        
        //! Cast the atom to a map of atoms.
        /** The function casts the atom to a map of atoms.
         @return A map of atoms.
         */
        inline operator Dico const&() const
        {
            return m_quark->getDico();
        }
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        Atom& operator=(Atom const& other) noexcept;
        
        //! Set up the atom with a boolean value.
        /** The function sets up the atom with a long value created with aboolean value.
         @param value   The boolean value.
         @return An atom.
         */
        inline Atom& operator=(const bool value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkBool(value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const int value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkLong((long)value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const long value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkLong(value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline Atom& operator=(const float value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDouble((float)value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline Atom& operator=(const double value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDouble(value);
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline Atom& operator=(char const* tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkTag(Tag::create(tag));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline Atom& operator=(string const& tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkTag(Tag::create(tag));
            return *this;
        }
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        inline Atom& operator=(sTag tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkTag(tag);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(Vector const& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(Vector&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(initializer_list<Atom> il) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(il);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(Dico const& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(Dico&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(initializer_list<pair<const sTag, Atom>> il) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(il);
            return *this;
        }
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms hold the same value otherwise false.
         */
        inline bool operator==(Atom const& other) const noexcept
        {
            if(other.isUndefined() && isUndefined())
            {
                return true;
            }
            else if(other.isBool() && isNumber())
            {
                return m_quark->getBool() == other.m_quark->getBool();
            }
            else if(other.isLong() && isNumber())
            {
                return m_quark->getLong() == other.m_quark->getLong();
            }
            else if(other.isDouble() && isNumber())
            {
                return m_quark->getDouble() == other.m_quark->getDouble();
            }
            else if(other.isTag() && isTag())
            {
                return m_quark->getTag() == other.m_quark->getTag();
            }
            else if(other.isVector() && isVector())
            {
                return m_quark->getVector() == other.m_quark->getVector();
            }
            else if(other.isDico() && isDico())
            {
                return m_quark->getDico() == other.m_quark->getDico();
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a boolean value.
        /** The function compares the atom with a boolean value.
         @param value   The boolean value.
         @return true if the atom hold the same boolean value otherwise false.
         */
        inline bool operator==(const bool value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getBool() == value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a integer value.
        /** The function compares the atom with a integer value.
         @param value   The integer value.
         @return true if the atom hold the same integer value otherwise false.
         */
        inline bool operator==(const int value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() == (long)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom hold the same long value otherwise false.
         */
        inline bool operator==(const long value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() == value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a float value.
        /** The function compares the atom with a float value.
         @param value   The float value.
         @return true if the atom hold the same float value otherwise false.
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
         @return true if the atom hold the same double value otherwise false.
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
         @return true if the atom hold the same tag create with the string otherwise false.
         */
        bool operator==(char const* tag) const noexcept;
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the same tag create with the string otherwise false.
         */
        bool operator==(string const& tag) const noexcept;
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param tag   The tag.
         @return true if the atom hold the same tag otherwise false.
         */
        bool operator==(sTag tag) const noexcept;
        
        //! Compare the atom with a vector.
        /** The function compares the atom with a vector.
         @param vector   The vector.
         @return true if the atom hold the same vector otherwise false.
         */
        inline bool operator==(Vector& vector) const noexcept
        {
            if(isVector())
            {
                return m_quark->getVector() == vector;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom hold the same dico otherwise false.
         */
        inline bool operator==(Dico& dico) const noexcept
        {
            if(isDico())
            {
                return m_quark->getDico() == dico;
            }
            else
            {
                return false;
            }
        }
        
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
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom differ from the long value otherwise false.
         */
        inline bool operator!=(const long value) const noexcept
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
        
        //! Compare the atom with a vector.
        /** The function compares the atom with a vector.
         @param vector   The vector.
         @return true if the atom differ from the vector, otherwise false.
         */
        inline bool operator!=(Vector const& vector) const noexcept
        {
            return !(*this == vector);
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom differ from the dico otherwise false.
         */
        inline bool operator!=(Dico dico) const noexcept
        {
            return !(*this == dico);
        }
        
        static ostream& toJson(ostream &output, const Atom &atom, ulong& indent);
        
        //! Parse a string into a vector of atoms.
        /** Parse a string into a vector of atoms.
         @param     text	The string to parse.
         @return    The vector of atoms.
         @remark    For example, the string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
         The atom types will be determined automatically as 2 #Atom::Type::TAG atoms, 2 #Atom::Type::LONG atoms, and 1 #Atom::Type::DOUBLE atom.
         */
        static Vector parse(string const& text);
    };
    
    ostream& operator<<(ostream &output, const Atom &atom);
}



#endif


