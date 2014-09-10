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

#ifndef __DEF_KIWI_OBJECT_EXTENTED__
#define __DEF_KIWI_OBJECT_EXTENTED__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"
#include "Attribute.h"

// TODO add the methods and figure out how to acess the attributes

namespace Kiwi
{
    class ObjectExtentedListener;
    class Instance;
    
    // ================================================================================ //
    //                                  OBJECT EXTENTED                                 //
    // ================================================================================ //
    
    //! The object extented inherites from object and manages a set of attributes of differents kinds and differents sizes.
    /**
     The object extented can create, delete, save and recall a set of attributes. Most of the classes of Kiwi inherit from this class.
     @see ObjectExtentedListener, Attribute
     */
    class ObjectExtented : public Object
    {
    private:
        map<shared_ptr<Tag>, Attribute>             m_attributes;
        map<shared_ptr<Tag>, Attribute>::iterator   m_current_attribute;
        set<weak_ptr<ObjectExtentedListener>>       m_listeners;
        
        bool getAttribute(const shared_ptr<Tag> name);
        bool currentAttributeValid();
    protected:
        
        //! Create a new attribute.
        /** The function creates a new attribute of a defined type with a defined size and that matchs to a specific name. The default category is the object name except if a sticky category has been set.
         @param name    The name of the attribute.
         @param type    The type of the attribute.
         @param size    The number of elements of the attribute.
         */
        void createAttribute(const shared_ptr<Tag> name, Type type, int size);
        
        //! Set the size of the attribute.
        /** The function sets the size of the attribute.
         @param name    The name of the attribute.
         @param size    The size of the attribute.
         */
        void setAttributeSize(const shared_ptr<Tag> name, int size);
        
        //! Set the size of the attribute.
        /** The function sets the size of the last used or created attribute.
         @param size    The size of the attribute.
         */
        void setAttributeSize(int size);
        
        //! Set the label of the attribute.
        /** The function sets the label of the attribute.
         @param name    The name of the attribute.
         @param label    The label of the attribute.
         */
        void setAttributeLabel(const shared_ptr<Tag> name, const shared_ptr<Tag> label);
        
        //! Set the label of the attribute.
        /** The function sets the label of the last used or created attribute.
         @param label    The label of the attribute.
         */
        void setAttributeLabel(const shared_ptr<Tag> label);
        
        //! Set the style of the attribute.
        /** The function sets the style of the attribute.
         @param name    The name of the attribute.
         @param style    The style of the attribute.
         */
        void setAttributeStyle(const shared_ptr<Tag> name, const shared_ptr<Tag> style);
        
        //! Set the style of the attribute.
        /** The function sets the style of the last used or created attribute.
         @param style    The style of the attribute.
         */
        void setAttributeStyle(const shared_ptr<Tag> style);
        
        //! Set the category of the attribute.
        /** The function sets the category of the attribute.
         @param name        The name of the attribute.
         @param category    The category of the attribute.
         */
        void setAttributeCategory(const shared_ptr<Tag> name, const shared_ptr<Tag> category);
        
        //! Set the category of the attribute.
        /** The function sets the category of the last used or created attribute.
         @param category    The category of the attribute.
         */
        void setAttributeCategory(const shared_ptr<Tag> category);
        
        //! Set the label and the style of the attribute.
        /** The function sets the label and the style of the attribute.
         @param name        The name of the attribute.
         @param label       The label of the attribute.
         @param style       The style of the attribute.
         @param category    The category of the attribute.
         */
        void setAttributeAppearance(const shared_ptr<Tag> name, const shared_ptr<Tag> label, const shared_ptr<Tag> style, const shared_ptr<Tag> category);
        
        //! Set the label and the style of the attribute.
        /** The function sets the label and the style of the last used or created attribute.
         @param label       The label of the attribute.
         @param style       The style of the attribute.
         @param category    The category of the attribute.
         */
        void setAttributeAppearance(const shared_ptr<Tag> label, const shared_ptr<Tag> style, const shared_ptr<Tag> category);

        //! Set the attribute opaque or not to user.
        /** The function sets the attribute opaque or not to user.
         @param name        The name of the attribute.
         @param status      Opaque or not.
         */
        void setAttributeOpaque(const shared_ptr<Tag> name, bool status);
        
        //! Set the attribute opaque or not to user.
        /** The function sets the last used or created attribute opaque or not to user.
         @param status      Opaque or not.
         */
        void setAttributeOpaque(bool status);
        
        //! Set the attribute visible or not by the user.
        /** The function sets the attribute visible or not to user.
         @param name        The name of the attribute.
         @param status      Visible or not.
         */
        void setAttributeVisible(const shared_ptr<Tag> name, bool status);
        
        //! Set the attribute visible or not  to user.
        /** The function sets the last used or created attribute visible or not to user.
         @param status      Visible or not.
         */
        void setAttributeVisible(bool status);
        
        //! Set if the the attribute is saved or not with the object.
        /** The function sets if the attribute is saved or not with the object.
         @param name        The name of the attribute.
         @param status      Saved or not.
         */
        void setAttributeSave(const shared_ptr<Tag> name, bool status);
        
        //! Set if the the attribute is saved or not with the object.
        /** The function sets if the last used or created attribute is saved or not with the object.
         @param status      Saved or not.
         */
        void setAttributeSave(bool status);
        
        //! Set the attribute opaque, visible and save status.
        /** The function sets the attribute's opaque, visible and save status.
         @param name        The name of the attribute.
         @param opaque      Opaque or not.
         @param visible     Visible or not.
         @param save        Saved or not.
         */
        void setAttributeBehavior(const shared_ptr<Tag> name, bool opaque, bool visible, bool save);
        
        //! Set the attribute opaque, visible and save status.
        /** The function sets the last used or created attribute's opaque, visible and save status.
         @param opaque      Opaque or not.
         @param visible     Visible or not.
         @param save        Saved or not.
         */
        void setAttributeBehavior(bool opaque, bool visible, bool save);
        
        //! Set the attribute minimum value.
        /** The function sets the attribute's minimum value. If the minimum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the minimum clipping method.
         @param name        The name of the attribute.
         @param min         The minimum value as a string.
         */
        void setAttributeClipMin(const shared_ptr<Tag> name, string min);
        
        //! Set the attribute minimum value.
        /** The function sets the last used or created attribute's minimum value. If the minimum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the minimum clipping method.
         @param name        The name of the attribute.
         @param min         The minimum value as a string.
         */
        void setAttributeClipMin(string min);
        
        //! Set the attribute maximum value.
        /** The function sets the attribute's maximum value. If the maximum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the maximum clipping method.
         @param name        The name of the attribute.
         @param max         The maximum value as a string.
         */
        void setAttributeClipMax(const shared_ptr<Tag> name, string max);
        
        //! Set the attribute maximum value.
        /** The function sets the last used or created attribute's maximum value. If the maximum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the maximum clipping method.
         @param name        The name of the attribute.
         @param max         The maximum value as a string.
         */
        void setAttributeClipMax(string max);
        
        //! Set the attribute minimum and maximum values.
        /** The function sets the attribute's minimum and maximum values. If the minimum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the minimum clipping method and idem for maximum.
         @param name        The name of the attribute.
         @param min         The minimum value as a string.
         @param max         The maximum value as a string.
         */
        void setAttributeClip(const shared_ptr<Tag> name, string min, string max);
        
        //! Set the attribute minimum and maximum values.
        /** The function sets the last used or created attribute's minimum and maximum values. If the minimum string owns a digit value then the attribute activates the clipping method otherwise the attribute desactivates the minimum clipping method and idem for maximum.
         @param name        The name of the attribute.
         @param min         The minimum value as a string.
         @param max         The maximum value as a string.
         */
        void setAttributeClip(string min, string max);
        
        //! Set the attribute setter and getter.
        /** The function sets the attribute's setter and getter methods instead of the default ones. If you want to use the default ones pass a null poiner.
         @param name        The name of the attribute.
         @param setter      The setter method.
         @param getter      The getter method.
         */
        void setAttributeMethods(const shared_ptr<Tag> name, Method setter, Method getter);
        
        //! Set the attribute setter and getter.
        /** The function sets the last used or created attribute's setter and getter methods instead of the default ones. If you want to use the default ones pass a null poiner.
         @param name        The name of the attribute.
         @param setter      The setter method.
         @param getter      The getter method.
         */
        void setAttributeMethods(Method setter, Method getter);
        
        //! Set the attribute's default values.
        /** The function sets the attribute's default values.
         @param name        The name of the attribute.
         @param elements    The default values.
         */
        void setAttributeDefaultValues(const shared_ptr<Tag> name, const vector<Element>& elements);
        
        //! Set the attribute's default values.
        /** The function sets the last used or created attribute's default values.
         @param name        The name of the attribute.
         @param elements    The default values.
         */
        void setAttributeDefaultValues(const vector<Element>& elements);
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        ObjectExtented(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name);
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        ObjectExtented(shared_ptr<Instance> kiwi, const string name);
        
        //! Descrutor.
        /** Free the methods.
         */
        ~ObjectExtented();
        
        //! Retrieve the type of the object.
        /** The function retrieves the type of the object.
         @return The type of the object.
         */
        virtual bool isBox() const noexcept
        {
            return false;
        }

        //! Set the values of an attribute.
        /** The function set the values of an attribute.
         @param name        The name of the attribute.
         @param elements    A set of values.
         */
        void setAttributeValues(const shared_ptr<Tag> name, const vector<Element>& elements);
        
        //! Get the values of an attribute.
        /** The function retrieves the values of an attribute.
         @param name        The name of the attribute.
         @param elements    A vector of value to write the attribute's values.
         */
        void getAttributeValues(const shared_ptr<Tag> name, vector<Element>& elements);
        
        //! Write the object in a dico.
        /** The function writes the object in a dico.
         @param dico The dico.
         */
        virtual void write(shared_ptr<Dico> dico) override;
    };
    
    class ObjectExtentedListener
    {
    public:
        ObjectExtentedListener(){};
        virtual ~ObjectExtentedListener(){};
        
        //! Receive the notifivation that an attribute has changed.
        /** 
         */
        virtual void attributeChanged() const noexcept{};
    };
}


#endif


