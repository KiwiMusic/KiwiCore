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
    class ObjectExtented;
    class Box;
    class Dictionary;
    
    class Instance;
    
    typedef enum
    {
        T_NOTHING   = (1<<0),
        T_OPAQUE    = (1<<1),
        T_LONG      = (1<<2),
        T_DOUBLE    = (1<<3),
        T_TAG       = (1<<4),
        T_OBJECT    = (1<<5),
        T_ELEMENTS  = (1<<6),
        T_GARBAGE   = (1<<7),
        T_SIGNAL    = (1<<8)
    }Type;
    
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    
    //! The element holds variables of different kinds.
    /**
     The element operates like the javasacript "var" by changing automatically the kind of value depending of the initialization. The values allowed are adapted to Kiwi : long, double, tag and object.
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
        
        //! Constructor with a weak extented object.
        /** The function allocates the element with a locked weak extented object.
         */
        Element(weak_ptr<ObjectExtented> object) noexcept;
        
        //! Constructor with an extented object.
        /** The function allocates the element with an extented object.
         */
        Element(shared_ptr<ObjectExtented> object) noexcept;
        
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
        Element(weak_ptr<Dictionary> object) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(shared_ptr<Dictionary> object) noexcept;
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        ~Element() noexcept;
        
        //! Retrieve the type of an element.
        /** The function retrieves the type of an element, you should always check the type of the element before to retrieve the value.
         @return    The type of the element as a type.
         */
        inline Type type() const noexcept
        {
            return m_type;
        }
        
        //! Check if an element is of type long.
        /** The function checks if an element is of type long.
         @return    true if the element is a long.
         */
        inline bool isLong() const noexcept
        {
            return m_type == T_LONG;
        }
        
        //! Check if an element is of type double.
        /** The function checks if an element is of type double.
         @return    true if the element is a double.
         */
        inline bool isDouble() const noexcept
        {
            return m_type == T_DOUBLE;
        }
        
        //! Check if an element is of type tag.
        /** The function checks if an element is of type tag.
         @return    true if the element is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_type == T_TAG;
        }
        
        //! Check if an element is of type object.
        /** The function checks if an element is of type object.
         @return    true if the element is a object.
         */
        inline bool isObject() const noexcept
        {
            return m_type == T_OBJECT;
        }
        
        //! Cast an element to an int.
        /** The function casts an element to an int.
         @return An int value if the element is a digit otherwise 0.
         */
        operator int() const noexcept;
        
        //! Cast an element to a long.
        /** The function casts an element to a long.
         @return A long value if the element is a digit otherwise 0.
         */
        operator long() const noexcept;

        //! Cast an element to a float.
        /** The function casts an element to a float.
         @return A float value if the element is a digit otherwise 0.
         */
        operator float() const noexcept;
        
        //! Cast an element to a double.
        /** The function casts an element to a double.
         @return A double value if the element is a digit otherwise 0.
         */
        operator double() const noexcept;
        
        //! Cast an element to a tag.
        /** The function casts an element to a tag.
         @return A tag if the element is a tag otherwise a nullptr.
         */
        operator shared_ptr<Tag>() const noexcept;
        
        //! Cast an element to a weak object.
        /** The function casts an element to a weak object.
         @return A weak object if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Object>() const noexcept;
        
        //! Cast an element to an object.
        /** The function casts an element to a weak object.
         @return An object if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Object>() const noexcept;
        
        //! Cast an element to a weak extented object.
        /** The function casts an element to a weak extented object.
         @return A weak extented object if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<ObjectExtented>() const noexcept;
        
        //! Cast an element to an extented object.
        /** The function casts an element to an extented object.
         @return An extented object if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<ObjectExtented>() const noexcept;
        
        //! Cast an element to a weak box.
        /** The function casts an element to a weak box.
         @return A weak box if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Box>() const noexcept;
        
        //! Cast an element to a box.
        /** The function casts an element to a box.
         @return A box if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Box>() const noexcept;
        
        //! Cast an element to a weak dico.
        /** The function casts an element to a weak dico.
         @return A weak dico if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Dictionary>() const noexcept;
        
        //! Cast an element to a dico.
        /** The function casts an element to a dico.
         @return A dico if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Dictionary>() const noexcept;
        
        //! Set up the element with another element.
        /** The function sets up the element with another element.
         */
        Element& operator=(const Element& other) noexcept;
        
        //! Set up the element with a int value.
        /** The function sets up the element with a long value from a casted int value.
         */
        Element& operator=(int value) noexcept;
        
        //! Set up the element with a long value.
        /** The function sets up the element with a long value.
         */
        Element& operator=(long value) noexcept;
        
        //! Set up the element with a float value.
        /** The function sets up the element with a double value from a casted float value.
         */
        Element& operator=(float value) noexcept;
        
        //! Set up the element with a double value.
        /** The function sets up the element with a double value.
         */
        Element& operator=(double value) noexcept;
        
        //! Set up the element with a tag.
        /** The function sets up the element with a tag.
         */
        Element& operator=(shared_ptr<Tag> tag) noexcept;
        
        //! Set up the element with a weak object.
        /** The function sets up the element with a weak object.
         */
        Element& operator=(weak_ptr<Object> object) noexcept;
        
        //! Set up the element with an object.
        /** The function sets up the element with an object.
         */
        Element& operator=(shared_ptr<Object> object) noexcept;
        
        //! Set up the element with a weak extented object.
        /** The function sets up the element with a weak extented object.
         */
        Element& operator=(weak_ptr<ObjectExtented> object) noexcept;
        
        //! Set up the element with an extented object.
        /** The function sets up the element with an extented object.
         */
        Element& operator=(shared_ptr<ObjectExtented> object) noexcept;
        
        //! Set up the element with a weak box.
        /** The function sets up the element with a weak box.
         */
        Element& operator=(weak_ptr<Box> object) noexcept;
        
        //! Set up the element with a box.
        /** The function sets up the element with a box.
         */
        Element& operator=(shared_ptr<Box> object) noexcept;
        
        //! Set up the element with a weak dico.
        /** The function sets up the element with a weak dico.
         */
        Element& operator=(weak_ptr<Dictionary> object) noexcept;
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         */
        Element& operator=(shared_ptr<Dictionary> object) noexcept;
        
        bool operator==(const Element& other) const noexcept;
        
        bool operator==(const int value) const noexcept;
        
        bool operator==(const long value) const noexcept;
        
        bool operator==(const float value) const noexcept;
        
        bool operator==(const double value) const noexcept;
        
        bool operator==(shared_ptr<Tag> tag) const noexcept;
        
        bool operator==(weak_ptr<Object> object) const noexcept;
        
        bool operator==(shared_ptr<Object> object) const noexcept;
        
        bool operator==(weak_ptr<ObjectExtented> object) const noexcept;
        
        bool operator==(shared_ptr<ObjectExtented> object) const noexcept;
        
        bool operator==(weak_ptr<Box> object) const noexcept;
        
        bool operator==(shared_ptr<Box> object) const noexcept;
        
        bool operator==(weak_ptr<Dictionary> object) const noexcept;
        
        bool operator==(shared_ptr<Dictionary> object) const noexcept;
    };
}



#endif


