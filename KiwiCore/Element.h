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

#include "Defs.h"
#include "Tag.h"

// Later try to figure out to use union instead of struct and to real translator of value and finish elements
namespace Kiwi
{
    class Object;
    class Box;
    class Dico;
    
    typedef enum
    {
        T_NOTHING   = (1<<0),
        T_OPAQUE    = (1<<1),
        T_LONG      = (1<<2),
        T_DOUBLE    = (1<<3),
        T_TAG       = (1<<4),
        T_OBJECT    = (1<<5),
        T_ELEMENT   = (1<<6),
        T_ELEMENTS  = (1<<7),
        T_GARBAGE   = (1<<8),
        T_SIGNAL    = (1<<9)
    }Type;
    
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    
    //! The element holds variables of different kinds.
    /**
     The element operates like the javasacript "var" by changing automatically the kind of value depending of the allocation, the initialization or the cast. The element holds a long, a double, a tag and an object but can receives an int, a long, a float, a double, an object (weak or not), a box (weak or not) or a dico (weak or not).
     */
    class Element
    {
    private:
        struct Ele
        {
            long                m_long;
            double              m_double;
            shared_ptr<Tag>     m_tag;
            shared_ptr<Object>  m_object;
        };
        
        Type    m_type;
        Ele     m_val;
    public:
        
        //! Constructor.
        /** The function allocates the element with a zero long value.
         */
        Element() noexcept;
        
        //! Constructor with an int value.
        /** The function allocates the element with a long value from a casted int value.
         */
        Element(int value) noexcept;
        
        //! Constructor with a long value.
        /** The function allocates the element with a long value.
         */
        Element(long value) noexcept;
        
        //! Constructor with a float value.
        /** The function allocates the element with a double value from a casted float value.
         */
        Element(float value) noexcept;
        
        //! Constructor with a double value.
        /** The function allocates the element with a double value.
         */
        Element(double value) noexcept;
        
        //! Constructor with a tag.
        /** The function allocates the element with a tag.
         */
        Element(shared_ptr<Tag> tag) noexcept;
        
        //! Constructor with a weak object.
        /** The function allocates the element with a locked weak object.
         */
        Element(weak_ptr<Object> object) noexcept;
        
        //! Constructor with an object.
        /** The function allocates the element with an object.
         */
        Element(shared_ptr<Object> object) noexcept;
        
        //! Constructor with a weak box.
        /** The function allocates the element with a locked weak box.
         */
        Element(weak_ptr<Box> object) noexcept;
        
        //! Constructor with a box.
        /** The function allocates the element with a box.
         */
        Element(shared_ptr<Box> object) noexcept;
        
        //! Constructor with a weak dico.
        /** The function allocates the element with a locked weak dico.
         */
        Element(weak_ptr<Dico> object) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(shared_ptr<Dico> object) noexcept;
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        ~Element() noexcept;
        
        //! Retrieve the type of the element.
        /** The function retrieves the type of the element, you should always check the type of the element before to retrieve the value.
         @return    The type of the element as a type.
         */
        inline Type type() const noexcept
        {
            return m_type;
        }
        
        //! Check if the element is of type long.
        /** The function checks if the element is of type long.
         @return    true if the element is a long.
         */
        inline bool isLong() const noexcept
        {
            return m_type == T_LONG;
        }
        
        //! Check if the element is of type double.
        /** The function checks if the element is of type double.
         @return    true if the element is a double.
         */
        inline bool isDouble() const noexcept
        {
            return m_type == T_DOUBLE;
        }
        
        //! Check if the element is of type tag.
        /** The function checks if the element is of type tag.
         @return    true if the element is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_type == T_TAG;
        }
        
        //! Check if the element is of type object.
        /** The function checks if the element is of type object.
         @return    true if the element is a object.
         */
        inline bool isObject() const noexcept
        {
            return m_type == T_OBJECT;
        }
        
        //! Cast the element to an int.
        /** The function casts the element to an int.
         @return An int value if the element is a digit otherwise 0.
         */
        operator int() const noexcept;
        
        //! Cast the element to a long.
        /** The function casts the element to a long.
         @return A long value if the element is a digit otherwise 0.
         */
        operator long() const noexcept;

        //! Cast the element to a float.
        /** The function casts the element to a float.
         @return A float value if the element is a digit otherwise 0.
         */
        operator float() const noexcept;
        
        //! Cast the element to a double.
        /** The function casts the element to a double.
         @return A double value if the element is a digit otherwise 0.
         */
        operator double() const noexcept;
        
        //! Cast the element to a tag.
        /** The function casts the element to a tag.
         @return A tag if the element is a tag otherwise a nullptr.
         */
        operator shared_ptr<Tag>() const noexcept;
        
        //! Cast the element to a weak object.
        /** The function casts the element to a weak object.
         @return A weak object if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Object>() const noexcept;
        
        //! Cast the element to an object.
        /** The function casts the element to a weak object.
         @return An object if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Object>() const noexcept;
        
        //! Cast the element to a weak box.
        /** The function casts the element to a weak box.
         @return A weak box if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Box>() const noexcept;
        
        //! Cast the element to a box.
        /** The function casts the element to a box.
         @return A box if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Box>() const noexcept;
        
        //! Cast the element to a weak dico.
        /** The function casts the element to a weak dico.
         @return A weak dico if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Dico>() const noexcept;
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return A dico if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Dico>() const noexcept;
        
        //! Set up the element with another element.
        /** The function sets up the element with another element.
         @param other   The other element.
         @return An element.
         */
        Element& operator=(const Element& other) noexcept;
        
        //! Set up the element with a int value.
        /** The function sets up the element with a long value from a casted int value.
         @param value   The int value.
         @return An element.
         */
        Element& operator=(const int value) noexcept;
        
        //! Set up the element with a long value.
        /** The function sets up the element with a long value.
         @param value   The long value.
         @return An element.
         */
        Element& operator=(const long value) noexcept;
        
        //! Set up the element with a float value.
        /** The function sets up the element with a double value from a casted float value.
         @param value   The float value.
         @return An element.
         */
        Element& operator=(const float value) noexcept;
        
        //! Set up the element with a double value.
        /** The function sets up the element with a double value.
         @param value   The double value.
         @return An element.
         */
        Element& operator=(const double value) noexcept;
        
        //! Set up the element with a tag.
        /** The function sets up the element with a tag.
         @param tag   The tag.
         @return An element.
         */
        Element& operator=(shared_ptr<Tag> tag) noexcept;
        
        //! Set up the element with a weak object.
        /** The function sets up the element with a weak object.
         @param object   The weak object.
         @return An element.
         */
        Element& operator=(weak_ptr<Object> object) noexcept;
        
        //! Set up the element with an object.
        /** The function sets up the element with an object.
         @param object   The object.
         @return An element.
         */
        Element& operator=(shared_ptr<Object> object) noexcept;
        
        //! Set up the element with a weak box.
        /** The function sets up the element with a weak box.
         @param object   The weak box.
         @return An element.
         */
        Element& operator=(weak_ptr<Box> object) noexcept;
        
        //! Set up the element with a box.
        /** The function sets up the element with a box.
         @param object   The box.
         @return An element.
         */
        Element& operator=(shared_ptr<Box> object) noexcept;
        
        //! Set up the element with a weak dico.
        /** The function sets up the element with a weak dico.
         @param object   The weak dico.
         @return An element.
         */
        Element& operator=(weak_ptr<Dico> object) noexcept;
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         @param object   The dico.
         @return An element.
         */
        Element& operator=(shared_ptr<Dico> object) noexcept;
        
        //! Compare the element with another.
        /** The function compares the element with another.
         @param other The other element.
         @return true if the elements hold the same value otherwise false.
         */
        bool operator==(const Element& other) const noexcept;
        
        //! Compare the element with an int value.
        /** The function compares the element with an int value.
         @param value   The int value.
         @return true if the element hold the int value otherwise false.
         */
        bool operator==(const int value) const noexcept;
        
        //! Compare the element with a long value.
        /** The function compares the element with a long.
         @param value   The long value.
         @return true if the element hold the long value otherwise false.
         */
        bool operator==(const long value) const noexcept;
        
        //! Compare the element with a float value.
        /** The function compares the element with a float value.
         @param value   The float value.
         @return true if the element hold the float value otherwise false.
         */
        bool operator==(const float value) const noexcept;
        
        //! Compare the element with a double value.
        /** The function compares the element with a double value.
         @param value   The double value.
         @return true if the element hold the double value otherwise false.
         */
        bool operator==(const double value) const noexcept;
        
        //! Compare the element with a tag.
        /** The function compares the element with a tag.
         @param value   The tag.
         @return true if the element hold the tag otherwise false.
         */
        bool operator==(shared_ptr<Tag> tag) const noexcept;
        
        //! Compare the element with a weak object.
        /** The function compares the element with a locked weak object.
         @param object   The weak object.
         @return true if the element hold the locked weak object otherwise false.
         */
        bool operator==(weak_ptr<Object> object) const noexcept;
        
        //! Compare the element with an object.
        /** The function compares the element with an object.
         @param object   The object.
         @return true if the element hold the objectotherwise false.
         */
        bool operator==(shared_ptr<Object> object) const noexcept;
        
        //! Compare the element with a weak box.
        /** The function compares the element with a locked weak box.
         @param object   The weak box.
         @return true if the element hold the locked weak box otherwise false.
         */
        bool operator==(weak_ptr<Box> object) const noexcept;
        
        //! Compare the element with a box.
        /** The function compares the element with a box.
         @param object   The box.
         @return true if the element hold the box otherwise false.
         */
        bool operator==(shared_ptr<Box> object) const noexcept;
        
        //! Compare the element with a weak dico.
        /** The function compares the element with a locked weak dico.
         @param object   The weak box.
         @return true if the element hold the locked weak dico otherwise false.
         */
        bool operator==(weak_ptr<Dico> object) const noexcept;
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param object   The dico.
         @return true if the element hold the dico otherwise false.
         */
        bool operator==(shared_ptr<Dico> object) const noexcept;
    };
}



#endif


