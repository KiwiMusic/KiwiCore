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

#ifndef __DEF_KIWI_ATTRIBUTE__
#define __DEF_KIWI_ATTRIBUTE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"
#include "Dico.h"


namespace Kiwi
{
    class ObjectExtented;
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute holds a set of values of differents kinds and differents sizes....
    /**
     The attribute...
     @see ObjectExtented
     */
    class Attribute
    {
        friend class ObjectExtented;
    private:
        const shared_ptr<Tag>   m_name;     ///< The name of the attribute
        const Type              m_type;     ///< The type of the attribute
        int                     m_size;     ///< The size of the attribute
        
        shared_ptr<Tag>         m_label;    ///< The label of the attribute
        shared_ptr<Tag>         m_style;    ///< The style of the attribute
        shared_ptr<Tag>         m_category; ///< The category of the attribute
        
        bool                    m_visible;  ///< If the attribute is visible by the user (default true)
        bool                    m_opaque;   ///< If the attribute can be changed by the user (default false)
        bool                    m_save;     ///< If the attribute should be saved (default true)
        
        vector<Element>         m_elements; ///< The list of elements
        vector<Element>         m_default;  ///< The default list of elements
 
        bool                    m_minlimits;///< The minimum clipping state if the attribute owns double or long values (default false)
        bool                    m_maxlimits;///< The maximum clipping state if the attribute owns double or long values (default false)
        double                  m_minimum;  ///< The minimum value if the attribute owns double or long values
        double                  m_maximum;  ///< The maximum value if the attribute owns double or long values
        
        MethodElements          m_setter;   ///< The setter method of the attribute
        MethodElements          m_getter;   ///< The getter method of the attribute
        
        
        //! Set the size of the attribute.
        /** The function sets the number elements of the attribute.
         @param size    The size of the attribute.
         */
        void setSize(int size);
        
        //! Set the label of the attribute.
        /** The function sets the label of the attribute.
         @param label    The label of the attribute.
         */
        void setLabel(shared_ptr<Tag> label) noexcept;
        
        //! Set the style of the attribute.
        /** The function sets the style of the attribute.
         @param style    The style of the attribute.
         */
        void setStyle(shared_ptr<Tag> style) noexcept;
        
        //! Set the category of the attribute.
        /** The function sets the category of the attribute.
         @param category    The category of the attribute.
         */
        void setCategory(shared_ptr<Tag> category) noexcept;
        
        //! Set the attribute opaque or not to user.
        /** The function sets the attribute opaque or not to user.
         @param status      Opaque or not.
         */
        void setOpaque(bool opaque) noexcept;
        
        //! Set the attribute visible or not by the user.
        /** The function sets the attribute visible or not to user.
         @param status      Visible or not.
         */
        void setVisible(bool visible) noexcept;
        
        //! Set if the the attribute is saved or not with the object.
        /** The function sets if the attribute is saved or not with the object.
         @param status      Saved or not.
         */
        void setSave(bool save) noexcept;
        
        //! Set the attribute minimum value.
        /** The function sets the attribute's minimum value. If the minimum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the minimum clipping method.
         @param min         The minimum value as a string.
         */
        void setMinimum(const string value);
        
        //! Set the attribute maximum value.
        /** The function sets the attribute's maximum value. If the maximum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the maximum clipping method.
         @param max         The maximum value as a string.
         */
        void setMaximum(const string  value);
        
        //! Set the attribute setter method.
        /** The function sets the attribute's setter method instead of the default ones. If you want to use the default ones pass a null poiner.
         @param setter      The setter method.
         */
        void setSetter(MethodElements method);
        
        //! Set the attribute getter method.
        /** The function sets the attribute's getter method instead of the default ones. If you want to use the default ones pass a null poiner.
         @param setter      The getter method.
         */
        void setGetter(MethodElements method);
        
        //! The default setter.
        /** The function sets the attribute values.
         @param elements   The values.
         */
        void setValuesDefault(const vector<Element>& elements) noexcept
        {
            for(int i = 0; i < m_elements.size(); i++)
            {
                if(i < elements.size() && elements[i].type() == m_type)
                {
                    m_elements[i] = elements[i];
                }
            }
            doClip();
        }
        
        //! The default getter.
        /** The function retrieves the attribute values.
         @param elements   The values.
         */
        void getValuesDefault(vector<Element>& elements) noexcept
        {
            for(int i = 0; i < m_elements.size(); i++)
                elements[i] = m_elements[i];
        }
                
        //! Set the attribute's default values.
        /** The function sets the attribute's default values.
         @param elements    The default values.
         */
        void setDefaultValues(const vector<Element>& elements);
        
        inline void doClip()
        {
            if(m_minlimits && m_maxlimits)
            {
                if(m_type == T_LONG)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = clip((long)m_elements[i], (long)m_minimum, (long)m_maximum);
                else if (m_type == T_DOUBLE)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = clip((double)m_elements[i], m_minimum, m_maximum);
            }
            else if(m_minlimits)
            {
                if(m_type == T_LONG)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = max((long)m_elements[i], (long)m_minimum);
                else if (m_type == T_DOUBLE)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = max((double)m_elements[i], m_minimum);
            }
            else if(m_maxlimits)
            {
                if(m_type == T_LONG)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = min((long)m_elements[i], (long)m_maximum);
                else if (m_type == T_DOUBLE)
                    for(int i = 0; i < m_elements.size(); i++)
                        m_elements[i] = min((double)m_elements[i], m_maximum);
            }
        }
        
        Attribute(const shared_ptr<Tag> name, const Type type, int size) : m_name(name), m_type(type), m_size(size)
        {
            
        }
        
    public:
        
        ~Attribute()
        {
            m_elements.clear();
            m_default.clear();
        }
        
        inline shared_ptr<Tag> name() const noexcept
        {
            return m_name;
        }
        
        inline Type type() const noexcept
        {
            return m_type;
        }
        
        inline int size() const noexcept
        {
            return m_size;
        }
        
        inline shared_ptr<Tag> label() const noexcept
        {
            return m_label;
        }
        
        inline shared_ptr<Tag> style() const noexcept
        {
            return m_style;
        }
        
        inline shared_ptr<Tag> category() const noexcept
        {
            return m_category;
        }
        
        inline bool visible() const noexcept
        {
            return m_visible;
        }
        
        inline bool opaque() const noexcept
        {
            return m_opaque;
        }
        
        inline bool save() const noexcept
        {
            return m_save;
        }
        
        void getValues(vector<Element>& elements) noexcept
        {
            for(int i = 0; i < m_elements.size(); i++)
                elements[i] = m_elements[i];
        }
        
        Element getValue() noexcept
        {
            if(m_elements.size())
                return m_elements[0];
            else
                return 0;
        }
        
        void setValues(const vector<Element>& elements) // Perhaps we could just copy and avoid checking ?
        {
            for(int i = 0; i < m_elements.size(); i++)
            {
                if(i < elements.size() && elements[i].type() == m_type)
                {
                    m_elements[i] = elements[i];
                }
            }
            doClip();
        }
        
        void write(shared_ptr<Dictionary> dico)
        {
            dico->set(m_name, 0.01);
        }
        
        void read(shared_ptr<Dictionary> dico)
        {
            
        }
    };
}


#endif


