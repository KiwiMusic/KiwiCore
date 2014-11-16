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
// - Make it threadsafe
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
     The element operates like the javasacript "var" by changing automatically the kind of value depending of the allocation, the initialization or the cast. The element holds a long, a double, a tag and a box but can receives an int, a long, a float, a double, a tag (weak or shared) or a box (weak or shared).
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
            BOX       = 4,
            DICO      = 5,
            VECTOR    = 6
        };
        
    private:
        struct Ele
        {
            long    m_long      = 0;
            double  m_double    = 0;
            sTag    m_tag       = nullptr;
            sBox    m_box       = nullptr;
            sDico   m_dico      = nullptr;
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
        Element(unsigned long value) noexcept;
        
        //! Constructor with a float value.
        /** The function allocates the element with a double value from a casted float value.
         */
        Element(float value) noexcept;
        
        //! Constructor with a double value.
        /** The function allocates the element with a double value.
         */
        Element(double value) noexcept;
        
        //! Constructor with a string.
        /** The function allocates the element with a string.
         */
        Element(string const& tag) noexcept;
        
        //! Constructor with a tag.
        /** The function allocates the element with a tag.
         */
        Element(sTag tag) noexcept;
        
        //! Constructor with a box.
        /** The function allocates the element with a box.
         */
        Element(sBox box) noexcept;
        
        //! Constructor with a dico.
        /** The function allocates the element with a dico.
         */
        Element(sDico dico) noexcept;
        
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
		 @see isDouble, isNumber
         */
        inline bool isLong() const noexcept
        {
            return m_type == Element::LONG;
        }
        
        //! Check if the element is of type double.
        /** The function checks if the element is of type double.
         @return    true if the element is a double.
		 @see isLong, isNumber
         */
        inline bool isDouble() const noexcept
        {
            return m_type == Element::DOUBLE;
        }
		
		//! Checks if the element is of type long or double.
		/** Handy method for checking if an element is of type long or double.
		 
		 This is just so you can write things like:
		 @code
		 myElement.isNumber();
		 @endcode
		 instead of
		 @code
		 ((myElement.isLong() || (myElement.isDouble());
		 @endcode
		 
		 @return    true if the element is a long or a double, false otherwise.
		 @see isLong, isDouble
		 */
		inline bool isNumber() const noexcept
		{
			return (m_type == Element::DOUBLE) || (m_type == Element::LONG);
		}
		
        //! Check if the element is of type tag.
        /** The function checks if the element is of type tag.
         @return    true if the element is a tag.
         */
        inline bool isTag() const noexcept
        {
            return m_type == Element::TAG;
        }
        
        //! Check if the element is of type box.
        /** The function checks if the element is of type box.
         @return    true if the element is a box.
         */
        inline bool isObject() const noexcept
        {
            return m_type == Element::BOX;
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
        
        //! Cast the element to a long.
        /** The function casts the element to a long.
         @return A long value if the element is a digit otherwise 0.
         */
        operator unsigned long() const noexcept;

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
        operator sTag() const noexcept;
        
        //! Cast the element to a box.
        /** The function casts the element to a box.
         @return An box if the element is a box otherwise a nullptr.
         */
        operator sBox() const noexcept;
        
        //! Cast the element to a dico.
        /** The function casts the element to a dico.
         @return An dico if the element is a dico otherwise a nullptr.
         */
        operator sDico() const noexcept;
        
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
        
        //! Set up the element with a string.
        /** The function sets up the element with string.
         @param tag   The string.
         @return An element.
         */
        Element& operator=(string const& tag) noexcept;
        
        //! Set up the element with a tag.
        /** The function sets up the element with a tag.
         @param tag   The tag.
         @return An element.
         */
        Element& operator=(sTag tag) noexcept;
        
        //! Set up the element with a box.
        /** The function sets up the element with a box.
         @param box   The box.
         @return An element.
         */
        Element& operator=(sBox box) noexcept;
        
        //! Set up the element with a dico.
        /** The function sets up the element with a dico.
         @param dico   The dico.
         @return An element.
         */
        Element& operator=(sDico dico) noexcept;
        
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
        
        //! Compare the element with a string.
        /** The function compares the element with a string.
         @param tag   The string.
         @return true if the element hold the tag create with the string otherwise false.
         */
        bool operator==(string const& tag) const noexcept;
        
        //! Compare the element with a tag.
        /** The function compares the element with a tag.
         @param value   The tag.
         @return true if the element hold the tag otherwise false.
         */
        bool operator==(sTag tag) const noexcept;
        
        //! Compare the element with a box.
        /** The function compares the element with a box.
         @param box   The box.
         @return true if the element hold the box otherwise false.
         */
        bool operator==(sBox box) const noexcept;
        
        //! Compare the element with a dico.
        /** The function compares the element with a dico.
         @param dico   The dico.
         @return true if the element hold the dico otherwise false.
         */
        bool operator==(sDico dico) const noexcept;
    };
    
    string toString(Element const& __val);
    string toString(ElemVector const& __val);
    string toString(Element::Type __val);
}



#endif


