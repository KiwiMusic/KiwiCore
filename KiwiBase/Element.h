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

#ifndef __DEF_KIWI_ELEMENT__
#define __DEF_KIWI_ELEMENT__

#include "Tag.h"

// TODO
// - See if ElemVector shouldn't be in the Element class.
namespace Kiwi
{
    class Element;

    //! The vector of elements
    /**
     The elements is a standard vector of elements.
     */
    typedef vector<Element> ElemVector;
    
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    
    //! The element holds variables of different kinds.
    /**
     The element operates like the javasacript "var" by changing automatically the kind of value depending of the allocation, the initialization or the cast. The element holds a long, a double, a tag and a object but can receives an int, a long, a float, a double, a tag (weak or shared) or a object (weak or shared).
     */
    class Element
    {
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
        
    private:
        struct Ele
        {
            long            m_long      = 0;
            double          m_double    = 0.;
            sTag            m_tag       = nullptr;
            sObject         m_object       = nullptr;
            sDico           m_dico      = nullptr;
        };
        
        Element::Type   m_type;
        Ele             m_val;
    
        inline void changeType(Element::Type type) noexcept
        {
            if(m_type != type)
            {
                switch(m_type)
                {
                    case TAG:
                        m_val.m_tag.reset();
                        break;
                    case OBJECT:
                        m_val.m_object.reset();
                        break;
                    case DICO:
                        m_val.m_dico.reset();
                        break;
                    default:
                        break;
                }
                m_type = type;
            }
        }
    public:
        
        //! Constructor.
        /** The function allocates the element with a zero long value.
         */
        Element() noexcept
        {
            m_type = LONG;
            m_val.m_long = 0;
        }
        
        //! Constructor with a boolean value.
        /** The function allocates the element with a long value from a casted boolean value.
         */
        Element(const bool value) noexcept
        {
            m_type = LONG;
            m_val.m_long = (long)value;
        }
        
        //! Constructor with an int value.
        /** The function allocates the element with a long value from a casted int value.
         */
        Element(const int value) noexcept
        {
            m_type = LONG;
            m_val.m_long = (long)value;
        }
        
        //! Constructor with a long value.
        /** The function allocates the element with a long value.
         */
        Element(const long value) noexcept
        {
            m_type = LONG;
            m_val.m_long = value;
        }
        
        //! Constructor with a long value.
        /** The function allocates the element with a long value.
         */
        Element(const ulong value) noexcept
        {
            m_type = LONG;
            m_val.m_long = (long)value;
        }
        
        //! Constructor with a float value.
        /** The function allocates the element with a double value from a casted float value.
         */
        Element(const float value) noexcept
        {
            m_type = DOUBLE;
            m_val.m_double = (double)value;
        }
        
        //! Constructor with a double value.
        /** The function allocates the element with a double value.
         */
        Element(const double value) noexcept
        {
            m_type = DOUBLE;
            m_val.m_double = value;
        }
        
        //! Constructor with a string.
        /** The function allocates the element with a string.
         */
        Element(const char* tag) noexcept
        {
            m_type = TAG;
            m_val.m_tag = Tag::create(tag);
        }
        
        //! Constructor with a string.
        /** The function allocates the element with a string.
         */
        Element(string const& tag) noexcept
        {
            m_type = TAG;
            m_val.m_tag = Tag::create(tag);
        }
        
        //! Constructor with a tag.
        /** The function allocates the element with a tag.
         */
        Element(sTag tag) noexcept
        {
            m_type = TAG;
            m_val.m_tag = tag;
        }
        
        //! Constructor with a object.
        /** The function allocates the element with a object.
         */
        Element(sObject object) noexcept
        {
            m_type = OBJECT;
            m_val.m_object = object;
        }
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(sDico dico) noexcept
        {
            m_type = DICO;
            m_val.m_dico = dico;
        }
        
        //! Constructor with another element.
        /** The function allocates the element with an element.
         */
        Element(Element const& other) noexcept
        {
            m_type = other.m_type;
            switch(m_type)
            {
                case LONG:
                {
                    m_val.m_long = other.m_val.m_long;
                }
                    break;
                case DOUBLE:
                {
                    m_val.m_double = other.m_val.m_double;
                }
                    break;
                case TAG:
                {
                    m_val.m_tag = other.m_val.m_tag;
                }
                    break;
                case OBJECT:
                {
                    m_val.m_object = other.m_val.m_object;
                }
                    break;
                case DICO:
                {
                    m_val.m_dico = other.m_val.m_dico;
                }
                    break;
                default:
                    break;
            }
        }
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        ~Element() noexcept
        {
            ;
        }
        
        //! Retrieve the type of the element.
        /** The function retrieves the type of the element, you should always check the type of the element before to retrieve the value.
         @return    The type of the element as a type.
         */
        inline Element::Type type() const noexcept
        {
            return m_type;
        }
        
        //! Check if the element is of type long.
        /** The function checks if the element is of type long.
         @return    true if the element is a long.
		 @see isDouble, isNumber
         */
        inline bool isLong() const noexcept
        {
            return m_type == LONG;
        }
        
        //! Check if the element is of type double.
        /** The function checks if the element is of type double.
         @return    true if the element is a double.
		 @see isLong, isNumber
         */
        inline bool isDouble() const noexcept
        {
            return m_type == DOUBLE;
        }
		
		//! Checks if the element is of type long or double.
		/** The function checks if the element is of type long or double.
		 @return    true if the element is a long or a double.
		 */
		inline bool isNumber() const noexcept
		{
			return m_type == DOUBLE || m_type == LONG;
		}
		
        //! Check if the element is of type tag.
        /** The function checks if the element is of type tag.
         @return    true if the element is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_type == TAG;
        }
        
        //! Check if the element is of type object.
        /** The function checks if the element is of type object.
         @return    true if the element is a object.
         */
        inline bool isObject() const noexcept
        {
            return m_type == OBJECT;
        }
        
        //! Check if the element is of type dico.
        /** The function checks if the element is of type dico.
         @return    true if the element is a dico.
         */
        inline bool isDico() const noexcept
        {
            return m_type == DICO;
        }
        
        //! Cast the element to a boolean.
        /** The function casts the element to a boolean.
         @return An boolean value if the element is a digit otherwise 0.
         */
        inline operator bool() const noexcept
        {
            if(m_type == LONG)
            {
                return (bool)m_val.m_long;
            }
            else if(m_type == DOUBLE)
            {
                return (bool)m_val.m_double;
            }
            else
            {
                return 0;
            }
        }
        
        //! Cast the element to an int.
        /** The function casts the element to an int.
         @return An int value if the element is a digit otherwise 0.
         */
        inline operator int() const noexcept
        {
            if(m_type == LONG)
            {
                return (int)m_val.m_long;
            }
            else if(m_type == DOUBLE)
            {
                return (int)m_val.m_double;
            }
            else
            {
                return 0;
            }
        }
        
        //! Cast the element to a long.
        /** The function casts the element to a long.
         @return A long value if the element is a digit otherwise 0.
         */
        inline  operator long() const noexcept
        {
            if(m_type == LONG)
            {
                return m_val.m_long;
            }
            else if(m_type == DOUBLE)
            {
                return (long)m_val.m_double;
            }
            else
            {
                return 0;
            }
        }
        
        //! Cast the element to a long.
        /** The function casts the element to a long.
         @return A long value if the element is a digit otherwise 0.
         */
        inline operator ulong() const noexcept
        {
            if(m_type == LONG)
            {
                return (ulong)m_val.m_long;
            }
            else if(m_type == DOUBLE)
            {
                return (ulong)m_val.m_double;
            }
            else
            {
                return 0;
            }
        }

        //! Cast the element to a float.
        /** The function casts the element to a float.
         @return A float value if the element is a digit otherwise 0.
         */
        inline operator float() const noexcept
        {
            if(m_type == DOUBLE)
            {
                return (float)m_val.m_double;
            }
            else if(m_type == LONG)
            {
                return (float)m_val.m_long;
            }
            else
            {
                return 0.f;
            }
        }
        
        //! Cast the element to a double.
        /** The function casts the element to a double.
         @return A double value if the element is a digit otherwise 0.
         */
        inline operator double() const noexcept
        {
            if(m_type == DOUBLE)
            {
                return m_val.m_double;
            }
            else if(m_type == LONG)
            {
                return (double)m_val.m_long;
            }
            else
            {
                return 0.;
            }
        }
        
        //! Cast the element to a tag.
        /** The function casts the element to a tag.
         @return A tag if the element is a tag otherwise a nullptr.
         */
        inline operator sTag() const noexcept
        {
            return m_val.m_tag;
        }
        
        //! Cast the element to a object.
        /** The function casts the element to a object.
         @return An object if the element is a object otherwise a nullptr.
         */
        inline operator scObject() const noexcept
        {
            return m_val.m_object;
        }
        
        //! Cast the element to a object.
        /** The function casts the element to a object.
         @return An object if the element is a object otherwise a nullptr.
         */
        inline operator sObject() noexcept
        {
            return m_val.m_object;
        }
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return An dico if the element is a dico otherwise a nullptr.
         */
        inline operator scDico() const noexcept
        {
            return m_val.m_dico;
        }
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return An dico if the element is a dico otherwise a nullptr.
         */
        inline operator sDico() noexcept
        {
            return m_val.m_dico;
        }
        
        //! Set up the element with another element.
        /** The function sets up the element with another element.
         @param other   The other element.
         @return An element.
         */
        inline Element& operator=(Element const& other) noexcept
        {
            changeType(other.m_type);
            switch(m_type)
            {
                case LONG:
                {
                    m_val.m_long = other.m_val.m_long;
                }
                    break;
                case DOUBLE:
                {
                    m_val.m_double = other.m_val.m_double;
                }
                    break;
                case TAG:
                {
                    m_val.m_tag = other.m_val.m_tag;
                }
                    break;
                case OBJECT:
                {
                    m_val.m_object = other.m_val.m_object;
                }
                    break;
                case DICO:
                {
                    m_val.m_dico = other.m_val.m_dico;
                }
                    break;
                default:
                    break;
            }
            return *this;
        }
        
        //! Set up the element with a boolean value.
        /** The function sets up the element with a long value from a casted boolean value.
         @param value   The boolean value.
         @return An element.
         */
        inline Element& operator=(const bool value) noexcept
        {
            changeType(LONG);
            m_val.m_long = (long)value;
            return *this;
        }
        
        //! Set up the element with a int value.
        /** The function sets up the element with a long value from a casted int value.
         @param value   The int value.
         @return An element.
         */
        inline Element& operator=(const int value) noexcept
        {
            changeType(LONG);
            m_val.m_long = (long)value;
            return *this;
        }
        
        //! Set up the element with a long value.
        /** The function sets up the element with a long value.
         @param value   The long value.
         @return An element.
         */
        inline Element& operator=(const long value) noexcept
        {
            changeType(LONG);
            m_val.m_long = value;
            return *this;
        }
        
        //! Set up the element with a ulong value.
        /** The function sets up the element with a ulong value.
         @param value   The ulong value.
         @return An element.
         */
        inline Element& operator=(const ulong value) noexcept
        {
            changeType(LONG);
            m_val.m_long = (long)value;
            return *this;
        }
        
        //! Set up the element with a float value.
        /** The function sets up the element with a double value from a casted float value.
         @param value   The float value.
         @return An element.
         */
        inline Element& operator=(const float value) noexcept
        {
            changeType(DOUBLE);
            m_val.m_double = (double)value;
            return *this;
        }
        
        //! Set up the element with a double value.
        /** The function sets up the element with a double value.
         @param value   The double value.
         @return An element.
         */
        inline Element& operator=(const double value) noexcept
        {
            changeType(DOUBLE);
            m_val.m_double = value;
            return *this;
        }
        
        //! Set up the element with a string.
        /** The function sets up the element with string.
         @param tag   The string.
         @return An element.
         */
        inline Element& operator=(char const* tag) noexcept
        {
            changeType(TAG);
            m_val.m_tag = Tag::create(tag);
            return *this;
        }
        
        //! Set up the element with a string.
        /** The function sets up the element with string.
         @param tag   The string.
         @return An element.
         */
        inline Element& operator=(string const& tag) noexcept
        {
            changeType(TAG);
            m_val.m_tag = Tag::create(tag);
            return *this;
        }
        
        //! Set up the element with a tag.
        /** The function sets up the element with a tag.
         @param tag   The tag.
         @return An element.
         */
        inline Element& operator=(sTag tag) noexcept
        {
            changeType(TAG);
            m_val.m_tag = tag;
            return *this;
        }
        
        //! Set up the element with a object.
        /** The function sets up the element with a object.
         @param object   The object.
         @return An element.
         */
        inline Element& operator=(sObject object) noexcept
        {
            changeType(OBJECT);
            m_val.m_object = object;
            return *this;
        }
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         @param dico   The dico.
         @return An element.
         */
        inline Element& operator=(sDico dico) noexcept
        {
            changeType(DICO);
            m_val.m_dico = dico;
            return *this;
        }
        
        //! Compare the element with another.
        /** The function compares the element with another.
         @param other The other element.
         @return true if the elements hold the same value otherwise false.
         */
        inline bool operator==(Element const& other) const noexcept
        {
            if(m_type == other.m_type)
            {
                switch(m_type)
                {
                    case LONG:
                    {
                        return m_val.m_long == other.m_val.m_long;
                    }
                        break;
                    case DOUBLE:
                    {
                        return m_val.m_double == other.m_val.m_double;
                    }
                        break;
                    case TAG:
                    {
                        return m_val.m_tag == other.m_val.m_tag;
                    }
                        break;
                    case OBJECT:
                    {
                        return m_val.m_object == other.m_val.m_object;
                    }
                        break;
                    default:
                    {
                        return m_val.m_dico == other.m_val.m_dico;
                    }
                        break;
                }
            }
            return false;
        }
        
        //! Compare the element with a boolean value.
        /** The function compares the element with a boolean value.
         @param value   The boolean value.
         @return true if the element hold the boolean value otherwise false.
         */
        inline bool operator==(const bool value) const noexcept
        {
            return m_type == LONG && m_val.m_long == (long)value;
        }
        
        //! Compare the element with an int value.
        /** The function compares the element with an int value.
         @param value   The int value.
         @return true if the element hold the int value otherwise false.
         */
        inline bool operator==(const int value) const noexcept
        {
            return m_type == LONG && m_val.m_long == (long)value;
        }
        
        //! Compare the element with a long value.
        /** The function compares the element with a long.
         @param value   The long value.
         @return true if the element hold the long value otherwise false.
         */
        inline bool operator==(const long value) const noexcept
        {
            return m_type == LONG && m_val.m_long == value;
        }
        
        //! Compare the element with an ulong value.
        /** The function compares the element with an unsigned  long.
         @param value   The ulong value.
         @return true if the element hold the ulong value otherwise false.
         */
        inline bool operator==(const ulong value) const noexcept
        {
            return m_type == LONG && m_val.m_long == (long)value;
        }
        
        //! Compare the element with a float value.
        /** The function compares the element with a float value.
         @param value   The float value.
         @return true if the element hold the float value otherwise false.
         */
        inline bool operator==(const float value) const noexcept
        {
            return m_type == DOUBLE && m_val.m_double == (double)value;
        }
        
        //! Compare the element with a double value.
        /** The function compares the element with a double value.
         @param value   The double value.
         @return true if the element hold the double value otherwise false.
         */
        inline bool operator==(const double value) const noexcept
        {
            return m_type == DOUBLE && m_val.m_double == value;
        }
        
        //! Compare the element with a string.
        /** The function compares the element with a string.
         @param tag   The string.
         @return true if the element hold the tag create with the string otherwise false.
         */
        inline bool operator==(char const* tag) const noexcept
        {
            return m_type == TAG && m_val.m_tag == Tag::create(tag);
        }
        
        //! Compare the element with a string.
        /** The function compares the element with a string.
         @param tag   The string.
         @return true if the element hold the tag create with the string otherwise false.
         */
        inline bool operator==(string const& tag) const noexcept
        {
            return m_type == TAG && m_val.m_tag == Tag::create(tag);
        }
        
        //! Compare the element with a tag.
        /** The function compares the element with a tag.
         @param value   The tag.
         @return true if the element hold the tag otherwise false.
         */
        inline bool operator==(sTag tag) const noexcept
        {
            return m_type == TAG && m_val.m_tag == tag;
        }
        
        //! Compare the element with a object.
        /** The function compares the element with a object.
         @param object   The object.
         @return true if the element hold the object otherwise false.
         */
        inline bool operator==(sObject object) const noexcept
        {
            return m_type == OBJECT && m_val.m_object == object;
        }
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param dico   The dico.
         @return true if the element hold the dico otherwise false.
         */
        inline bool operator==(sDico dico) const noexcept
        {
            return m_type == DICO && m_val.m_dico == dico;
        }
        
        //! Compare the element with another.
        /** The function compares the element with another.
         @param other The other element.
         @return true if the elements differ otherwise false.
         */
        inline bool operator!=(const Element& other) const noexcept
        {
            return !(*this == other);
        }
        
        //! Compare the element with a boolean value.
        /** The function compares the element with a boolean value.
         @param value   The boolean value.
         @return true if the element differ from the boolean value otherwise false.
         */
        inline bool operator!=(const bool value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the element with an int value.
        /** The function compares the element with an int value.
         @param value   The int value.
         @return true if the element differ from the int value otherwise false.
         */
        inline bool operator!=(const int value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the element with a long value.
        /** The function compares the element with a long.
         @param value   The long value.
         @return true if the element differ from the long value otherwise false.
         */
        inline bool operator!=(const long value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the element with a float value.
        /** The function compares the element with a float value.
         @param value   The float value.
         @return true if the element differ from the float value otherwise false.
         */
        inline bool operator!=(const float value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the element with a double value.
        /** The function compares the element with a double value.
         @param value   The double value.
         @return true if the element differ from the double value otherwise false.
         */
        inline bool operator!=(const double value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the element with a string.
        /** The function compares the element with a string.
         @param tag   The string.
         @return true if the element differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(char const* tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the element with a string.
        /** The function compares the element with a string.
         @param tag   The string.
         @return true if the element differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(string const& tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the element with a tag.
        /** The function compares the element with a tag.
         @param value   The tag.
         @return true if the element differ from the tag otherwise false.
         */
        inline bool operator!=(sTag tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the element with a object.
        /** The function compares the element with a object.
         @param object   The object.
         @return true if the element differ from the object otherwise false.
         */
        inline bool operator!=(sObject object) const noexcept
        {
            return !(*this == object);
        }
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param dico   The dico.
         @return true if the element differ from the dico otherwise false.
         */
        bool operator!=(sDico dico) const noexcept
        {
            return !(*this == dico);
        }
    };
    
    string toString(Element const& __val);
    string toString(ElemVector const& __val);
    string toString(Element::Type __val);
}



#endif


