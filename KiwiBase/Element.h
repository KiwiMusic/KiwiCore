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

namespace Kiwi
{
    class Object;
    class Box;
    class Dico;
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
     The element operates like the javasacript "var" by changing automatically the kind of value depending of the allocation, the initialization or the cast. The element holds a long, a double, a tag and an object but can receives an int, a long, a float, a double, a tag (weak or shared) or an object (weak or shared).
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
            long                m_long      = 0;
            double              m_double    = 0;
            shared_ptr<Tag>     m_tag       = nullptr;
            shared_ptr<Object>  m_object    = nullptr;
            shared_ptr<Dico>    m_dico      = nullptr;
        };
        
        Element::Type   m_type;
        Ele             m_val;
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
        
        //! Constructor with a long value.
        /** The function allocates the element with a long value.
         */
        Element(size_t value) noexcept;
        
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
        Element(weak_ptr<Tag> tag) noexcept;
        
        //! Constructor with a tag.
        /** The function allocates the element with a tag.
         */
        Element(shared_ptr<Tag> tag) noexcept;
        
        //! Constructor with an object.
        /** The function allocates the element with an object.
         */
        Element(weak_ptr<Object> object) noexcept;
        
        //! Constructor with an object.
        /** The function allocates the element with an object.
         */
        Element(shared_ptr<Object> object) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(weak_ptr<Dico> dico) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(shared_ptr<Dico> dico) noexcept;
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        ~Element() noexcept;
        
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
         */
        inline bool isLong() const noexcept
        {
            return m_type == Element::LONG;
        }
        
        //! Check if the element is of type double.
        /** The function checks if the element is of type double.
         @return    true if the element is a double.
         */
        inline bool isDouble() const noexcept
        {
            return m_type == Element::DOUBLE;
        }
        
        //! Check if the element is of type tag.
        /** The function checks if the element is of type tag.
         @return    true if the element is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_type == Element::TAG;
        }
        
        //! Check if the element is of type object.
        /** The function checks if the element is of type object.
         @return    true if the element is a object.
         */
        inline bool isObject() const noexcept
        {
            return m_type == Element::OBJECT;
        }
        
        //! Check if the element is of type dico.
        /** The function checks if the element is of type dico.
         @return    true if the element is a dico.
         */
        inline bool isDico() const noexcept
        {
            return m_type == Element::DICO;
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
        operator weak_ptr<Tag>() const noexcept;
        
        //! Cast the element to a tag.
        /** The function casts the element to a tag.
         @return A tag if the element is a tag otherwise a nullptr.
         */
        operator shared_ptr<Tag>() const noexcept;
        
        //! Cast the element to an object.
        /** The function casts the element to an object.
         @return An object if the element is an object otherwise a nullptr.
         */
        operator weak_ptr<Object>() const noexcept;
        
        //! Cast the element to an object.
        /** The function casts the element to an object.
         @return An object if the element is an object otherwise a nullptr.
         */
        operator shared_ptr<Object>() const noexcept;
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return An dico if the element is a dico otherwise a nullptr.
         */
        operator weak_ptr<Dico>() const noexcept;
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return An dico if the element is a dico otherwise a nullptr.
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
        Element& operator=(weak_ptr<Tag> tag) noexcept;
        
        //! Set up the element with a tag.
        /** The function sets up the element with a tag.
         @param tag   The tag.
         @return An element.
         */
        Element& operator=(shared_ptr<Tag> tag) noexcept;
        
        //! Set up the element with an object.
        /** The function sets up the element with an object.
         @param object   The object.
         @return An element.
         */
        Element& operator=(weak_ptr<Object> object) noexcept;
        
        //! Set up the element with an object.
        /** The function sets up the element with an object.
         @param object   The object.
         @return An element.
         */
        Element& operator=(shared_ptr<Object> object) noexcept;
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         @param dico   The dico.
         @return An element.
         */
        Element& operator=(weak_ptr<Dico> dico) noexcept;
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         @param dico   The dico.
         @return An element.
         */
        Element& operator=(shared_ptr<Dico> dico) noexcept;
        
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
        bool operator==(weak_ptr<Tag> tag) const noexcept;
        
        //! Compare the element with a tag.
        /** The function compares the element with a tag.
         @param value   The tag.
         @return true if the element hold the tag otherwise false.
         */
        bool operator==(shared_ptr<Tag> tag) const noexcept;
        
        //! Compare the element with an object.
        /** The function compares the element with an object.
         @param object   The object.
         @return true if the element hold the object otherwise false.
         */
        bool operator==(weak_ptr<Object> object) const noexcept;
        
        //! Compare the element with an object.
        /** The function compares the element with an object.
         @param object   The object.
         @return true if the element hold the object otherwise false.
         */
        bool operator==(shared_ptr<Object> object) const noexcept;
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param dico   The dico.
         @return true if the element hold the dico otherwise false.
         */
        bool operator==(weak_ptr<Dico> dico) const noexcept;
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param dico   The dico.
         @return true if the element hold the dico otherwise false.
         */
        bool operator==(shared_ptr<Dico> dico) const noexcept;
    };
    
    string toString(Element const& __val);
    string toString(ElemVector const& __val);
    string toString(Element::Type __val);
}



#endif


