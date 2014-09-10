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
    class Dico;
    
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
        /** Set up the element with a zero long value or with a defined value : int, long, float, double, tag or object.
         */
        Element() noexcept;
        Element(int value) noexcept;
        Element(long value) noexcept;
        Element(float value) noexcept;
        Element(double value) noexcept;
        Element(shared_ptr<Tag> tag) noexcept;
        Element(weak_ptr<Object> object) noexcept;
        Element(shared_ptr<Object> object) noexcept;
        Element(weak_ptr<ObjectExtented> object) noexcept;
        Element(shared_ptr<ObjectExtented> object) noexcept;
        Element(weak_ptr<Box> object) noexcept;
        Element(shared_ptr<Box> object) noexcept;
        Element(weak_ptr<Dico> object) noexcept;
        Element(shared_ptr<Dico> object) noexcept;
        
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
        
        inline operator int() const noexcept
        {
            if(isLong())
                return (int)m_val.m_long;
            else if(isDouble())
                return (int)m_val.m_double;
            else
                return 0;
        }
        
        inline operator long() const noexcept
        {
            if(isLong())
                return m_val.m_long;
            else if(isDouble())
                return (long)m_val.m_double;
            else
                return 0;
        }

        inline operator float() const noexcept
        {
            if(isDouble())
                return (float)m_val.m_double;
            else if(isLong())
                return (float)m_val.m_long;
            else
                return 0;
        }
        
        inline operator double() const noexcept
        {
            if(isDouble())
                return m_val.m_double;
            else if(isLong())
                return (double)m_val.m_long;
            else
                return 0;
        }
        
        inline operator shared_ptr<Tag>() const noexcept
        {
            return m_val.m_tag;
        }
        
        inline operator weak_ptr<Object>() const noexcept
        {
            return m_val.m_object;
        }
        
        inline operator shared_ptr<Object>() const noexcept
        {
            return m_val.m_object;
        }
        
        inline Element& operator=(const Element& other) noexcept
        {
            m_type = other.m_type;
            if(m_type == T_LONG)
                m_val.m_long = other.m_val.m_long;
            else if(m_type == T_DOUBLE)
                m_val.m_double = other.m_val.m_double;
            else if(m_type == T_TAG)
                m_val.m_tag = other.m_val.m_tag;
            else
                m_val.m_object = other.m_val.m_object;
            return *this;
        }
        
        inline Element& operator=(int value) noexcept
        {
            m_type = T_LONG;
            m_val.m_long = value;
            return *this;
        }
        
        inline Element& operator=(long value) noexcept
        {
            m_type = T_LONG;
            m_val.m_long = value;
            return *this;
        }
        
        inline Element& operator=(float value) noexcept
        {
            m_type  = T_DOUBLE;
            m_val.m_double= value;
            return *this;
        }
        
        inline Element& operator=(double value) noexcept
        {
            m_type  = T_DOUBLE;
            m_val.m_double= value;
            return *this;
        }
        
        inline Element& operator=(shared_ptr<Tag> tag) noexcept
        {
            m_type  = T_TAG;
            m_val.m_tag= tag;
            return *this;
        }
        
        inline Element& operator=(weak_ptr<Object> object) noexcept
        {
            m_type  = T_TAG;
            m_val.m_object= object.lock();
            return *this;
        }
        
        inline Element& operator=(shared_ptr<Object> object) noexcept
        {
            m_type  = T_TAG;
            m_val.m_object= object;
            return *this;
        }
    };
}



#endif


