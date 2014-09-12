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

#ifndef __DEF_KIWI_OBJECT__
#define __DEF_KIWI_OBJECT__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Attribute.h"

// Have to clean the callMethod and check the Element and Elements method

namespace Kiwi
{
    class Instance;
    class DspNode;
    class Box;
    class Dico;
    class ObjectListener;
    
    // ================================================================================ //
    //                                      DEFINTIONS                                  //
    // ================================================================================ //
    
    typedef void (*Method)(shared_ptr<Object>, ...);
    typedef void (*MethodNothing)(shared_ptr<Object>);
    typedef void (*MethodLong)(shared_ptr<Object>, long value);
    typedef void (*MethodDouble)(shared_ptr<Object>, double value);
    typedef void (*MethodTag)(shared_ptr<Object>, shared_ptr<Tag> tag);
    typedef void (*MethodObject)(shared_ptr<Object>, shared_ptr<Object> object);
    typedef void (*MethodElement)(shared_ptr<Object>, Element& elements);
    typedef void (*MethodElements)(shared_ptr<Object>, vector<Element>& elements);
    typedef void (*MethodGarbage)(shared_ptr<Object>, shared_ptr<Tag> tag, vector<Element>& elements);
    
    
    typedef void (*MethodDsp)(shared_ptr<Box>, shared_ptr<DspNode> node);
    typedef void (*MethodProcess)(shared_ptr<Box>, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize);
    
    typedef shared_ptr<Object> (*MethodCreate)(shared_ptr<Instance>, shared_ptr<Tag> name, ...);
    
    struct ObjectMethod
    {
        Type    m_type;
        Method  m_method;
    };
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The object manages dynamic methods.
    /**
     Most of the kiwi classes inherites from the object to manage the methods. Please look at the implementation of the boxes to understand how to define the prototype of the methods and to see how to add and remove them. You can also have a look on the method prototypes above. Note that we consider three kind of methods the standard and defaults ones that got a type nothing, long, double, tag, object, element and garbage (garbage type is only for the garbage method) and the opaque ones that can have whatever you want as parameters. Standard and default methods are mostly used by inlets and outlets to pass datas to objects (but it's not an strict rule) while opaque methods are used internally for everything else.
     */
    class Object : public enable_shared_from_this<Object>
    {
        // ================================================================================ //
        //                                      TAGS                                        //
        // ================================================================================ //
        
    public:
        const shared_ptr<Tag>  m_tag_bang;
        const shared_ptr<Tag>  m_tag_long;
        const shared_ptr<Tag>  m_tag_double;
        const shared_ptr<Tag>  m_tag_tag;
        const shared_ptr<Tag>  m_tag_object;
        const shared_ptr<Tag>  m_tag_element;
        const shared_ptr<Tag>  m_tag_elements;
        const shared_ptr<Tag>  m_tag_garbage;
        const shared_ptr<Tag>  m_tag_signal;
        const shared_ptr<Tag>  m_tag_empty;
        const shared_ptr<Tag>  m_tag_create;
        
        // ================================================================================ //
        //                                      STRUCTURE                                   //
        // ================================================================================ //
        
    private:
   
        const weak_ptr<Instance>                    m_kiwi;
        const shared_ptr<Tag>                       m_name;
        int                                         m_stack_count;
    
        ObjectMethod                                m_method_create;
        MethodNothing                               m_method_bang;
        MethodLong                                  m_method_long;
        MethodDouble                                m_method_double;
        MethodTag                                   m_method_tag;
        MethodObject                                m_method_object;
        MethodElement                               m_method_element;
        MethodElements                              m_method_elements;
        MethodGarbage                               m_method_garbage;
        map<shared_ptr<Tag>, ObjectMethod>          m_method_standard;
        map<shared_ptr<Tag>, Method>                m_method_opaque;
        
        map<shared_ptr<Tag>, shared_ptr<Attribute>> m_attributes;
        set<weak_ptr<ObjectListener>>               m_listeners;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        Object(shared_ptr<Instance> kiwi, string const& name);
        
        //! Descrutor.
        /** Free the methods and the attributes.
         */
        virtual ~Object();
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline shared_ptr<Tag> name() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the type of the object.
        /** The function retrieves the type of the object.
         @return The type of the object.
         */
        virtual inline bool isBox() const noexcept
        {
            return false;
        }
        
        //! Write the object in a dico.
        /** The function writes the object in a dico.
         @param dico The dico.
         */
        virtual void write(shared_ptr<Dico> dico) noexcept;
        
        //! Read a dico.
        /** The function reads a dico an applyes the changes.
         @param dico The dico.
         */
        virtual void read(shared_ptr<const Dico> dico) noexcept;
        
        // ================================================================================ //
        //                                      METHODS                                     //
        // ================================================================================ //
    
        //! Retrieve the number of methods.
        /** The function retrieves the number of methods. It takes into account the default methods bang, long, double, tag, object, elements and garbage, the standard methods and the opaque methods.
         @return The number of methods.
         */
        int getNumberOfMethods() const noexcept;
        
        //! Retrieve if a method exists.
        /** The function check the existence of a method. It could be a default method bang, long, double, tag, object, elements or garbage, a standard method or an opaque method.
         @param name    The name of the method.
         @return        True if the method exist otherwise false.
         */
        bool hasMethod(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve if a method exists.
        /** The function check the existence of a method. It could be a default method bang, long, double, tag, object, elements or garbage, a standard method or an opaque method.
         @param name    The name of the method.
         @return        True if the method exist otherwise false.
         */
        inline bool hasMethod(string const& name) const noexcept
        {
            return hasMethod(createTag(name));
        }
        
        //! Retrieve a method.
        /** The function retrieves a method.
         @param name    The name of the method.
         @return        The adress of the method or null if the method doesn't exist.
         */
        Method getMethod(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve a method.
        /** The function retrieves a method.
         @param name    The name of the method.
         @return        The adress of the method or null if the method doesn't exist.
         */
        inline Method getMethod(string const& name) const noexcept
        {
            return getMethod(createTag(name));
        }
        
        //! Retrieve if the type of a method.
        /** The function retrieves the type of a method. For the default and standart methods it will be the nothing, long, double, tag, object, elements or garbage (only for the garbage method) type and for the other it will be the opaque type.
         @param name    The name of the method.
         @return        True if the method exist otherwise false.
         */
        Type getMethodType(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve if the type of a method.
        /** The function retrieves the type of a method. For the default and standart methods it will be the nothing, long, double, tag, object, elements or garbage (only for the garbage method) type and for the other it will be the opaque type.
         @param name    The of the method.
         @return        True if the method exist otherwise false.
         */
        inline Type getMethodType(string const& name) const noexcept
        {
            return getMethodType(createTag(name));
        }
        
        //! Retrieve an object method.
        /** The function retrieves an object method.
         @param name    The name of the method.
         @return        The object method.
         */
        ObjectMethod getObjectMethod(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve an object method.
        /** The function retrieves an object method.
         @param name    The name of the method.
         @return        The object method.
         */
        inline ObjectMethod getObjectMethod(string const& name) const noexcept
        {
            return getObjectMethod(createTag(name));
        }
        
        //! Call a standard method without parameters.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         */
        void callMethod(shared_ptr<Tag> name);
        
        //! Call a standard method without parameters.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         */
        inline void callMethod(string const& name)
        {
            callMethod(createTag(name));
        }
        
        //! Call a standard method with a long parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A long value to pass.
         */
        void callMethod(shared_ptr<Tag> name, long value);
        
        //! Call a standard method with a long parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A long value to pass.
         */
        inline void callMethod(string const& name, long value)
        {
            callMethod(createTag(name), value);
        }
        
        //! Call a standard method with a double parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A double value to pass.
         */
        void callMethod(shared_ptr<Tag> name, double value);
        
        //! Call a standard method with a double parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A double value to pass.
         */
        inline void callMethod(string const& name, double value)
        {
            callMethod(createTag(name), value);
        }
        
        //! Call a standard method with a tag parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param tag     A tag to pass.
         */
        void callMethod(shared_ptr<Tag> name, shared_ptr<Tag> tag);
        
        //! Call a standard method with a tag parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param tag     A tag to pass.
         */
        inline void callMethod(string const& name, shared_ptr<Tag> tag)
        {
            callMethod(createTag(name), tag);
        }
        
        //! Call a standard method with an object parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param object  An object to pass.
         */
        void callMethod(shared_ptr<Tag> name, shared_ptr<Object> object);
        
        //! Call a standard method with an object parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param object  An object to pass.
         */
        inline void callMethod(string const& name, shared_ptr<Object> object)
        {
            callMethod(createTag(name), object);
        }
        
        //! Call a standard method with a list of elements parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name        A name that matchs to a method.
         @param elements    A list of elements to pass.
         */
        void callMethod(shared_ptr<Tag> name, vector<Element>& elements);
        
        //! Call a standard method with a list of elements parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name        A name that matchs to a method.
         @param elements    A list of elements to pass.
         */
        inline void callMethod(string const& name, vector<Element>& elements)
        {
            callMethod(createTag(name), elements);
        }
        
        //! Call an opaque method with unformatted parameters.
        /** The function looks for the opaque method that matchs with the name and pass the arguments. Note that you should know what can of parameters the method is expected because we can't know it for you.
         @param name    A name that matchs to a method.
         @param size    The number of parameters (limited to 10).
         @param ...     Some parameters.
         */
        void callMethodOpaque(shared_ptr<Tag> name, int size, ...);
        
        //! Call an opaque method with unformatted parameters.
        /** The function looks for the opaque method that matchs with the name and pass the arguments. Note that you should know what can of parameters the method is expected because we can't know it for you.
         @param name    A name that matchs to a method.
         @param size    The number of parameters (limited to 10).
         @param ...     Some parameters.
         */
        void callMethodOpaque(string name, int size, ...);
        
    protected:
        
        //! Add a new method method.
        /** The function add a method to the list of methods. Note that the bang, long, double, tag, object, elements and garbage methods should be defined with the good types (you can't have a bang method that receive a double value as parameter for exemple). The standard methods can only have the same types as the default methods except the garbage one (nothing, long, double, tag, object and element types). Look at the class definition to see the prototypes of the default methods. If you want to use other parameters, you should use an opaque type. An opaque type means that the methods won't be used by the inlets the and outlets with a connection in a graphical context but internally (directly from an object to another, for example). You should use the opaque call to use it.
         @param name    A tag that matchs to a method.
         @param type    A type that defines the method parameters.
         @param method  A function that matchs the method prototypes depending of the type.
         */
        void addMethod(shared_ptr<Tag> name, Type type, Method method) noexcept;
        
        //! Add a new method method.
        /** The function add a method to the list of methods. Note that the bang, long, double, tag, object, elements and garbage methods should be defined with the good types (you can't have a bang method that receive a double value as parameter for exemple). The standard methods can only have the same types as the default methods except the garbage one (nothing, long, double, tag, object and element types). Look at the class definition to see the prototypes of the default methods. If you want to use other parameters, you should use an opaque type. An opaque type means that the methods won't be used by the inlets the and outlets with a connection in a graphical context but internally (directly from an object to another, for example). You should use the opaque call to use it.
         @param name    A tag that matchs to a method.
         @param type    A type that defines the method parameters.
         @param method  A function that matchs the method prototypes depending of the type.
         */
        inline void addMethod(string name, Type type, Method method) noexcept
        {
            addMethod(createTag(name), type, method);
        }
        
        //! Remove a method.
        /** The function removes a method from the method list.
         @param name    A tag that matchs to a method.
         */
        void removeMethod(shared_ptr<Tag> name) noexcept;
        
        //! Remove a method.
        /** The function removes a method from the method list.
         @param name    A tag that matchs to a method.
         */
        inline void removeMethod(string name) noexcept
        {
            removeMethod(createTag(name));
        }
        
    private:
        
        //! Increment the stack of methods called.
        /** The function increments the stack of methods called.
         @return true if the stack overflow is reached otherwise false.
         */
        inline bool stackIncrement() noexcept
        {
            ++m_stack_count;
            if(m_stack_count < 256)
            {
                return false;
            }
            else if(m_stack_count == 256)
            {
                errorObject(string("Stack overflow"));
                return false;
            }
            else
                return true;
        }
        
        //! Decrement the stack of methods called.
        /** The function decrements the stack of methods called.
         */
        inline void stackDecrement() noexcept
        {
            --m_stack_count;
        }
        
        //! Send an object warning about a wrong method called.
        /** The function sends an object warning about a wrong method called.
         @param name    The name of the method called.
         */
        inline void warningWrongMethod(shared_ptr<Tag> name) const noexcept
        {
            warningObject("don't have such method \"" + (string)*name + "\"");
        }
        
        //! Send an object warning about a wrong method called.
        /** The function sends an object warning about a wrong method called.
         @param name    The name of the method called.
         */
        inline void warningWrongMethodArguments(shared_ptr<Tag> name, string const& argument) const noexcept
        {
            warningObject("Wrong arguments for the method " + (string)*name + " received " + argument + " !");
        }
        
        //! Send an object warning about a wrong method called.
        /** The function sends an object warning about a wrong method called.
         @param name    The name of the method called.
         */
        inline void warningWrongMethodDefinition(shared_ptr<Tag> name) const noexcept
        {
            warning((string)*m_name + " has a wrong defintion of the type for the "+ (string)*name +" method !");
        }
        
        // ================================================================================ //
        //                                  ATTRIBUTES                                      //
        // ================================================================================ //
        
    protected:
        //! Create an attribute.
        /** The function creates an attribute and intializes the corresponding setter and getter methods.
         @param name    The name of the attribute.
         @param getter  The getter method.
         @param setter  The setter method.
         @return The attribute.
         */
        void createAttribute(shared_ptr<Tag> name, Method getter, Method setter) noexcept;
        
        //! Create an attribute.
        /** The function creates an attribute and intializes the corresponding setter and getter methods.
         @param name    The name of the attribute.
         @param getter  The getter method.
         @param setter  The setter method.
         @return The attribute.
         */
        inline void createAttribute(string const& name, Method getter, Method setter) noexcept
        {
            createAttribute(createTag(name), getter, setter);
        }
        
        //! Check if an attribute exists.
        /** The function checks if an attribute exists.
         @param name    The name of the attribute.
         @return true if the attribute exists otherwise false.
         */
        bool hasAttribute(shared_ptr<Tag> name) const noexcept;
        
        //! Check if an attribute exists.
        /** The function checks if an attribute exists.
         @param name    The name of the attribute.
         @return true if the attribute exists otherwise false.
         */
        inline bool hasAttribute(string const& name) const noexcept
        {
            return hasAttribute(createTag(name));
        }
        
        //! Retrieve an attribute.
        /** The function retrieves an attribute
         @param name    The name of the attribute.
         @return The attribute if the attribute exists otherwise nullptr.
         */
        shared_ptr<Attribute> getAttribute(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve an attribute.
        /** The function retrieves an attribute
         @param name    The name of the attribute.
         @return The attribute if the attribute exists otherwise nullptr.
         */
        inline shared_ptr<Attribute> getAttribute(string const& name) const noexcept
        {
            return getAttribute(createTag(name));
        }
        
        //! Set the label, the style and the category of an attribute.
        /** The function sets the label, the style and the category of an attribute.
         @param name        The name of the attribute.
         @param label       The label of the attribute.
         @param style       The style of the attribute.
         @param category    The category of the attribute.
         */
        void setAttributeAppearance(shared_ptr<Tag> name, shared_ptr<Tag> label, shared_ptr<Tag> style, shared_ptr<Tag> category) noexcept;
        
        //! Set the label, the style and the category of an attribute.
        /** The function sets the label, the style and the category of an attribute.
         @param name        The name of the attribute.
         @param label       The label of the attribute.
         @param style       The style of the attribute.
         @param category    The category of the attribute.
         */
        inline void setAttributeAppearance(string const& name, string const& label, string const& style, string const& category) noexcept
        {
            setAttributeAppearance(createTag(name), createTag(label), createTag(style), createTag(category));
        }
        
        //! Set the opaque, visible and save states of an attribute.
        /** The function sets the opaque, visible and save states of an attribute.
         @param name        The name of the attribute
         @param opaque Opaque or not.
         @param visible Visible or not.
         @param save Saved or not.
         */
        void setAttributeBehavior(shared_ptr<Tag> name, bool opaque, bool visible, bool save) noexcept;
        
        //! Set the opaque, visible and save states of an attribute.
        /** The function sets the opaque, visible and save states of an attribute.
         @param name        The name of the attribute
         @param opaque Opaque or not.
         @param visible Visible or not.
         @param save Saved or not.
         */
        inline void setAttributeBehavior(string const& name, bool opaque, bool visible, bool save) noexcept
        {
            setAttributeBehavior(createTag(name), opaque, visible, save);
        }
        
        //! Set the value of an attribute with an element.
        /** The function sets the value  of an attribute with an element and resize the values to one if necessary.
         @param name        The name of the attribute
         @param element The element.
         */
        void setAttributeValue(shared_ptr<Tag> name, Element const& element) noexcept;
        
        //! Set the value of an attribute with an element.
        /** The function sets the value  of an attribute with an element and resize the values to one if necessary.
         @param name        The name of the attribute
         @param element The element.
         */
        inline void setAttributeValue(string const& name, Element const& element) noexcept
        {
            setAttributeValue(createTag(name), element);
        }
        
        //! Set the values of an attribute with a vector of elements.
        /** The function sets the values of an attribute with a vector of elements and resize the values if necessary.
         @param name        The name of the attribute
         @param elements The vector of elements.
         */
        void setAttributeValues(shared_ptr<Tag> name, vector<Element> const& elements) noexcept;
        
        //! Set the values of an attribute with a vector of elements.
        /** The function sets the values of an attribute with a vector of elements and resize the values if necessary.
         @param name        The name of the attribute
         @param elements The vector of elements.
         */
        inline void setAttributeValues(string const& name, vector<Element> const& elements) noexcept
        {
            setAttributeValues(createTag(name), elements);
        }
        
        //! Retrieve the value of an attribute.
        /** The function retrieves the unique or first value of an attribute.
         @param name        The name of the attribute
         @return The element.
         */
        Element getAttributeValue(shared_ptr<Tag> name) const noexcept;
        
        //! Retrieve the value of an attribute.
        /** The function retrieves the unique or first value of an attribute.
         @param name        The name of the attribute
         @return The element.
         */
        inline Element getAttributeValue(string const& name) const noexcept
        {
            return getAttributeValue(createTag(name));
        }
        
        //! Retrieve the value sof an attribute.
        /** The function retrieves the values of an attribute.
         @param name        The name of the attribute
         @param elements The vector of elements to fill.
         */
        void getAttributeValues(shared_ptr<Tag> name, vector<Element>& elements) const noexcept;
        
        //! Retrieve the values of an attribute.
        /** The function retrieves the values of an attribute.
         @param name        The name of the attribute
         @param elements The vector of elements to fill.
         */
        inline  void getAttributeValues(string const& name, vector<Element>& elements) const noexcept
        {
            getAttributeValues(createTag(name), elements);
        }
        
    private:
        
        //! Send an object warning about a wrong method called.
        /** The function sends an object warning about a wrong method called.
         @param name    The name of the method called.
         */
        inline void warningWrongAttribute(shared_ptr<Tag> name) const noexcept
        {
            warning((string)*m_name + "don't have such attribute \"" + (string)*name + "\"");
        }
        
    protected:
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        //! Tag factory.
        /** This function uses the object's instance to create a tag.
         @param name The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        shared_ptr<Tag> createTag(string name) const noexcept;
        
        //! Object factory.
        /** This function uses the object's instance to create an object.
         @param name The name of the tag to retrieve.
         @param element The elements for the object argument.
         */
        shared_ptr<Object> createObject(shared_ptr<Tag> name, Element const& element) const;
        
        //! Object factory.
        /** This function uses the object's instance to create an object.
         @param name The name of the tag to retrieve.
         @param element The elements for the object argument.
         */
        inline shared_ptr<Object> createObject(string name, Element const& element) const
        {
            return createObject(createTag(name), element);
        }
        
        //! Object factory.
        /** This function uses the object's instance to create an object.
         @param name The name of the tag to retrieve.
         @param elements The vector of elements for the object argument.
         */
        shared_ptr<Object> createObject(shared_ptr<Tag> name, vector<Element> const& elements) const;
        
        //! Object factory.
        /** This function uses the object's instance to create an object.
         @param name The name of the tag to retrieve.
         @param elements The vector of elements for the object argument.
         */
        inline shared_ptr<Object> createObject(string name, vector<Element> const& elements) const
        {
            return createObject(createTag(name), elements);
        }
        
        //! Dico factory.
        /** This function uses the object's instance to create a dico.
         */
        shared_ptr<Dico> createDico() const;
        
        // ================================================================================ //
        //                                      MESSAGES                                    //
        // ================================================================================ //
        
        //! Post a standard message.
        /** The function post a message via the instance specific console.
         @param message  The message in the string format.
         */
        void    post(string message) const noexcept;
        
        //! Post a standard message that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The message in the string format.
         */
        void    postObject(string message) const noexcept;
        
        //! Post a warning.
        /** The function post a message via the instance specific console.
         @param message  The warning in the string format.
         */
        void    warning(string message) const noexcept;
        
        //! Post a warning that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The warning in the string format.
         */
        void    warningObject(string message) const noexcept;
        
        //! Post an error.
        /** The function post a message via the instance specific console.
         @param message  The error in the string format.
         */
        void    error(string message) const noexcept;
        
        //! Post an error that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The error in the string format.
         */
        void    errorObject(string message) const noexcept;
        
    };
    
    class ObjectListener
    {
    public:
        ObjectListener(){};
        virtual ~ObjectListener(){};
        
        //! Receive the notifivation that an attribute has changed.
        /**
         */
        virtual void attributeChanged(shared_ptr<Object> object) const noexcept{};
    };
}


#endif


