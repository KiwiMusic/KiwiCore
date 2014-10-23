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

#ifndef __DEF_KIWI_OBJECT_ATTRIBUTE__
#define __DEF_KIWI_OBJECT_ATTRIBUTE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Dico.h"

namespace Kiwi
{
    class Instance;
    class Attribute;
    
    //! The shared pointer of a attribute.
    /**
     The sAttribute is shared pointer of a attribute.
     */
    typedef shared_ptr<Attribute>         sAttribute;
    
    // ================================================================================ //
    //                                  ATTRIBUTE FACTORY                               //
    // ================================================================================ //
    
    //! The attribute factory creates attributes.
    /**
     The attribute factory manages attributes.
     @see Tag
     */
    class AttributeFactory
    {
    private:
        map<sTag, sAttribute>       m_attributes;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        AttributeFactory() noexcept;
        
        //! Descrutor.
        /** Free the attributes.
         */
        ~AttributeFactory();
        
        //! Write the object in a dico.
        /** The function writes the name of the object in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico) const noexcept;
        
        //! Retrieve an attribute.
        /** The function retrieves an attribute.
         @param name The name of the attribute.
         @return The attribute.
         */
        sAttribute getAttribute(sTag name) const noexcept;
        
        //! The receive method that set the values of the attributes.
        /** The function looks for the names of that match with the attributes and call the set methods if necessary. 
         @param elements    A list of elements to pass.
         */
        void receive(ElemVector const& elements);
        
    protected:
        
        //! Read a dico.
        /** The function reads a dico but doesn't do anything.
         @param dico The dico.
         */
        void read(shared_ptr<const Dico> dico) noexcept;
        
        //! Receive a notification from an attribute.
        /** The function receives the notifications from the attribute.
         @param attr The attribute that notify.
         */
        virtual void notify(sAttribute attr);
        
        //! Add a new attribute.
        /** The function adds a new attribute to the object.
         @param name    A name for the attribute.
         */
        void addAttribute(sAttribute attr);
        
        //! Set the label, the style and the category of an attribute.
        /** The function set the label, the style and the category of an attribute.
         @param name The name of the attribute.
         @param label The label of the attribute.
         @param style The style of the attribute.
         @param category The category of the attribute.
         */
        void setAttributeAppearance(sTag name, string const& label, string const& style, string const& category);
        
        //! Set the attribute opaque, visible and save states of an attribute.
        /** The function sets the attribute opaque, visible and save states of an attribute.
         @param name The name of the attribute.
         @param opaque Opaque or not.
         @param visible Visible or not.
         @param save Saved or not.
         */
        void setAttributeBehavior(sTag name, long behavior);
    };
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute holds a set of values of differents kinds and differents sizes.
    /**
     The attribute manages a set of values.
     */
    class Attribute
    {
    public:
        
        enum
        {
            Visible = (1<<0),
            Opaque  = (1<<1),
            Saved   = (1<<2),
            Notify  = (1<<3)
        };
        
        friend class AttributeFactory;
        
    private:
        sTag            m_name;     ///< The name of the attribute
        string          m_label;    ///< The label of the attribute
        string          m_style;    ///< The style of the attribute
        string          m_category; ///< The category of the attribute
        long            m_behavior; ///< The behavior of the attribute
        
        //! Read the attribute in a dico.
        /** The function reads the attribute in a dico.
         @param dico The dico.
         */
        virtual void read(shared_ptr<const Dico> dico);
        
        //! Set the values with a vector of elements.
        /** The function sets the values with a vector of elements and resize the values if necessary.
         @param elements The vector of elements.
         */
        virtual void set(ElemVector const& elements) = 0;
        
    public:
        
        //! Constructor.
        /** Allocate and initialize the member values.
         */
        Attribute(sTag name,
                  string const& label,
                  string const& style,
                  string const& category,
                  long behavior);
        
        //! Destructor.
        /** Clear the attribute.
         */
        ~Attribute();
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline sTag name()
        {
            return m_name;
        }
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
         */
        inline string label() const noexcept
        {
            return m_label;
        }
        
        //! Retrieve the attribute style.
        /** The function retrieves the attribute style.
         @return The attribute style.
         */
        inline string style() const noexcept
        {
            return m_style;
        }
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
         */
        inline string category() const noexcept
        {
            return m_category;
        }
        
        //! Retrieve the values.
        /** The function retrieves the values.
         @param elements The vector of elements to fill.
         */
        virtual void get(ElemVector& elements) const noexcept = 0;
        
        //! Write the attribute in a dico.
        /** The function writes the attribute in a dico.
         @param dico The dico.
         */
        virtual void write(shared_ptr<Dico> dico) const noexcept;
    };
    
    // ================================================================================ //
    //                                      ATTRIBUTE TYPED                             //
    // ================================================================================ //
    
    class AttributeDouble : public Attribute
    {
    private:
        double m_value;
        void set(ElemVector const& elements) override;
    public:
        AttributeDouble(sTag name, string const& label, string const& style, string const& category, long behavior);
        ~AttributeDouble();
        void get(ElemVector& elements) const noexcept;
    };
    
    class AttributeTag : public Attribute
    {
    private:
        shared_ptr<Tag> m_value;
        void set(ElemVector const& elements) override;
    public:
        AttributeTag(sTag name, string const& label, string const& style, string const& category, long behavior);
        ~AttributeTag();
        void get(ElemVector& elements) const noexcept;
    };
    
    class AttributeObject : public Attribute
    {
    private:
        shared_ptr<Object> m_value;
        void set(ElemVector const& elements) override;
    public:
        AttributeObject(sTag name, string const& label, string const& style, string const& category, long behavior);
        ~AttributeObject();
        void get(ElemVector& elements) const noexcept;
    };
}


#endif


