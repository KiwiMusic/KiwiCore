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

namespace Kiwi
{
    class Instance;
    class Object;
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute holds a set of values of differents kinds and differents sizes....
    /**
     The attribute...
     */
    class Attribute
    {
        friend class Object;
    private:
        shared_ptr<Tag>             m_name;     ///< The name of the attribute
        
        shared_ptr<Tag>             m_label;    ///< The label of the attribute
        shared_ptr<Tag>             m_style;    ///< The style of the attribute
        shared_ptr<Tag>             m_category; ///< The category of the attribute
        
        bool                        m_visible;  ///< If the attribute is visible by the user (default true)
        bool                        m_opaque;   ///< If the attribute cant be changed by the user (default false)
        bool                        m_save;     ///< If the attribute should be saved (default true)
        
        vector<Element>             m_values;   ///< The list of elements
        
        //! Set the label, the style and the category of the attribute.
        /** The function sets the label, the style and the category of the attribute.
         @param label    The label of the attribute.
         @param style    The style of the attribute.
         @param category    The category of the attribute.
         */
        void appearance(shared_ptr<Tag> label, shared_ptr<Tag> style, shared_ptr<Tag> category) noexcept;
        
        //! Set the attribute opaque, visible and save states.
        /** The function sets the attribute opaque, visible and save states
         @param opaque Opaque or not.
         @param visible Visible or not.
         @param save Saved or not.
         */
        void behavior(bool opaque, bool visible, bool save) noexcept;
        
        //! Set the value with an element.
        /** The function sets the value with an element and resize the values to one if necessary.
         @param element The element.
         */
        void set(Element const& element) noexcept;
        
        //! Set the values with a vector of elements.
        /** The function sets the values with a vector of elements and resize the values if necessary.
         @param elements The vector of elements.
         */
        void set(vector<Element> const& elements) noexcept;
        
        //! Read the attribute in a dico.
        /** The function reads the attribute in a dico.
         @param dico The dico.
         */
        void read(shared_ptr<const Dico> dico) noexcept;
        
    public:
        
        //! Constructor.
        /** Allocate and initialize the member values.
         */
        Attribute(shared_ptr<Tag> name);
        
        //! Destructor.
        /** Clear the attribute.
         */
        ~Attribute();
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline shared_ptr<Tag> name()
        {
            return m_name;
        }
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
         */
        inline shared_ptr<Tag> label() const noexcept
        {
            return m_label;
        }
        
        //! Retrieve the attribute style.
        /** The function retrieves the attribute style.
         @return The attribute style.
         */
        inline shared_ptr<Tag> style() const noexcept
        {
            return m_style;
        }
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
         */
        inline shared_ptr<Tag> category() const noexcept
        {
            return m_category;
        }
        
        //! Retrieve the attribute visible status.
        /** The function retrieves the attribute visible status.
         @return true if the attribute is visible otherwise false.
         */
        inline bool visible() const noexcept
        {
            return m_visible;
        }
        
        //! Retrieve the attribute opaque status.
        /** The function retrieves the attribute opaque status.
         @return true if the attribute is opaque otherwise false.
         */
        inline bool opaque() const noexcept
        {
            return m_opaque;
        }
        
        //! Retrieve the attribute save status.
        /** The function retrieves the attribute save status
         @return true if the attribute is saved otherwise false.
         */
        inline bool save() const noexcept
        {
            return m_save;
        }
        
        //! Retrieve the values.
        /** The function retrieves the values.
         @param elements The vector of elements to fill.
         */
        void get(vector<Element>& elements) const noexcept;
        
        //! Retrieve the value.
        /** The function retrieves the unique or first value.
         @return The element.
         */
        Element get() const noexcept;
        
        //! Write the attribute in a dico.
        /** The function writes the attribute in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico) const noexcept;
    };
}


#endif


