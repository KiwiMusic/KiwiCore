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
#include "Console.h"

// Stack limit should warning then pass and stop at +1 to let the whole stack overflow chain visisble

namespace Kiwi
{
    class Instance;
    class DspNode;
    class Box;
    class Dico;
    
    typedef void (*Method)(shared_ptr<Object>, ...);
    typedef void (*MethodNothing)(shared_ptr<Object>);
    typedef void (*MethodLong)(shared_ptr<Object>, long value);
    typedef void (*MethodDouble)(shared_ptr<Object>, double value);
    typedef void (*MethodTag)(shared_ptr<Object>, shared_ptr<Tag> tag);
    typedef void (*MethodObject)(shared_ptr<Object>, shared_ptr<Object> tag);
    typedef void (*MethodElements)(shared_ptr<Object>, vector<Element>& elements);
    typedef void (*MethodGarbage)(shared_ptr<Object>, const shared_ptr<Tag> tag, vector<Element>& elements);
    typedef void (*MethodDsp)(shared_ptr<Box>, shared_ptr<DspNode> node);
    typedef void (*MethodProcess)(shared_ptr<Box>, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize);
    typedef shared_ptr<Object> (*MethodCreate)(shared_ptr<Instance>, shared_ptr<Tag> name, ...);
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The object manages dynamic methods.
    /**
     Most of the kiwi boxes inherites from the object to manage the methods. Please look at the implementation of the boxes to understand how to define the prototype of the methods and to see how to add and remove them. You can also have a look on the method prototypes above. Note that we consider three kind of methods the standard and defaults ones that got a type nothing, long, double, tag, object, element and garbage (garbage type is only for the garbage method) and the opaque ones that can have whatever you want as parameters. Standard and default methods are mostly used by inlets and outlets to pass datas to objects (but it's not an strict rule) while opaque methods are used internally for everything else.
     @see Box
     */
    class Object : public enable_shared_from_this<Object>
    {
    private:
        struct ObjectMethod
        {
            Type    m_type;
            Method  m_method;
        };
   
        const weak_ptr<Instance>                    m_kiwi;
        const shared_ptr<Tag>                       m_name;
        int                                         m_stack_count;
    
        // The default methods //
        ObjectMethod                                m_method_create;
        MethodNothing                               m_method_bang;
        MethodLong                                  m_method_long;
        MethodDouble                                m_method_double;
        MethodTag                                   m_method_tag;
        MethodObject                                m_method_object;
        MethodElements                              m_method_elements;
        MethodGarbage                               m_method_garbage;
        
        // The standard methods //
        map<const shared_ptr<Tag>, ObjectMethod>    m_method_standard;
        // The opaque methods //
        map<const shared_ptr<Tag>, Method>          m_method_opaque;
        
    public:
        const shared_ptr<Tag>  m_tag_bang;
        const shared_ptr<Tag>  m_tag_long;
        const shared_ptr<Tag>  m_tag_double;
        const shared_ptr<Tag>  m_tag_tag;
        const shared_ptr<Tag>  m_tag_object;
        const shared_ptr<Tag>  m_tag_elements;
        const shared_ptr<Tag>  m_tag_garbage;
        const shared_ptr<Tag>  m_tag_signal;
        const shared_ptr<Tag>  m_tag_empty;
        const shared_ptr<Tag>  m_tag_create;
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        Object(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name);
        
        //! Descrutor.
        /** Free the methods.
         */
        virtual ~Object();
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline shared_ptr<Tag> getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the type of the object.
        /** The function retrieves the type of the object.
         @return The type of the object.
         */
        virtual bool isBox() const noexcept
        {
            return false;
        }
        
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
        bool hasMethod(const shared_ptr<Tag> name) const;
        
        //! Retrieve if a method exists.
        /** The function check the existence of a method. It could be a default method bang, long, double, tag, object, elements or garbage, a standard method or an opaque method.
         @param name    The name of the method.
         @return        True if the method exist otherwise false.
         */
        bool hasMethod(string name);
        
        //! Retrieve a method.
        /** The function retrieves a method.
         @param name    The name of the method.
         @return        The adress of the method or null if the method doesn't exist.
         */
        Method getMethod(const shared_ptr<Tag> name) const;
        
        //! Retrieve a method.
        /** The function retrieves a method.
         @param name    The name of the method.
         @return        The adress of the method or null if the method doesn't exist.
         */
        Method getMethod(string name);
        
        //! Retrieve if the type of a method.
        /** The function retrieves the type of a method. For the default and standart methods it will be the nothing, long, double, tag, object, elements or garbage (only for the garbage method) type and for the other it will be the opaque type.
         @param name    The name of the method.
         @return        True if the method exist otherwise false.
         */
        Type getMethodType(const shared_ptr<Tag> name) const;
        
        //! Retrieve if the type of a method.
        /** The function retrieves the type of a method. For the default and standart methods it will be the nothing, long, double, tag, object, elements or garbage (only for the garbage method) type and for the other it will be the opaque type.
         @param name    The of the method.
         @return        True if the method exist otherwise false.
         */
        Type getMethodType(string name);
        
        //! Retrieve an object method.
        /** The function retrieves an object method.
         @param name    The name of the method.
         @return        The object method.
         */
        ObjectMethod getObjectMethod(const shared_ptr<Tag> name) const;
        
        //! Retrieve an object method.
        /** The function retrieves an object method.
         @param name    The name of the method.
         @return        The object method.
         */
        ObjectMethod getObjectMethod(string name);
        
        //! Call a standard method without parameters.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         */
        void callMethod(const shared_ptr<Tag> name);
        
        //! Call a standard method without parameters.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         */
        void callMethod(string name);
       
        //! Call a standard method with a long parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A long value to pass.
         */
        void callMethod(const shared_ptr<Tag> name, long value);
        
        //! Call a standard method with a long parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A long value to pass.
         */
        void callMethod(string name, long value);
        
        //! Call a standard method with a double parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A double value to pass.
         */
        void callMethod(const shared_ptr<Tag> name, double value);
        
        //! Call a standard method with a double parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param value   A double value to pass.
         */
        void callMethod(string name, double value);
        
        //! Call a standard method with a tag parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param tag     A tag to pass.
         */
        void callMethod(const shared_ptr<Tag> name, shared_ptr<Tag> tag);
        
        //! Call a standard method with a tag parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param tag     A tag to pass.
         */
        void callMethod(string name, shared_ptr<Tag> tag);
        
        //! Call a standard method with an object parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param object  An object to pass.
         */
        void callMethod(const shared_ptr<Tag> name, shared_ptr<Object> object);
        
        //! Call a standard method with an object parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name    A name that matchs to a method.
         @param object  An object to pass.
         */
        void callMethod(string name, shared_ptr<Object> object);
        
        //! Call a standard method with a list of elements parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name        A name that matchs to a method.
         @param elements    A list of elements to pass.
         */
        void callMethod(const shared_ptr<Tag> name, vector<Element>& elements);
        
        //! Call a standard method with a list of elements parameter.
        /** The function looks for the standard method that matchs with the name and the parameter and send call it or doesn't do anything if the method doesn't exist or if the parameters can't be formatted for the method.
         @param name        A name that matchs to a method.
         @param elements    A list of elements to pass.
         */
        void callMethod(string name, vector<Element>& elements);
        
        //! Call an opaque method with unformatted parameters.
        /** The function looks for the opaque method that matchs with the name and pass the arguments. Note that you should know what can of parameters the method is expected because we can't know it for you.
         @param name    A name that matchs to a method.
         @param size    The number of parameters (limited to 10).
         @param ...     Some parameters.
         */
        void callMethodOpaque(const shared_ptr<Tag> name, int size, ...);
        
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
        void addMethod(const shared_ptr<Tag> name, Type type, Method method);
        
        //! Add a new method method.
        /** The function add a method to the list of methods. Note that the bang, long, double, tag, object, elements and garbage methods should be defined with the good types (you can't have a bang method that receive a double value as parameter for exemple). The standard methods can only have the same types as the default methods except the garbage one (nothing, long, double, tag, object and element types). Look at the class definition to see the prototypes of the default methods. If you want to use other parameters, you should use an opaque type. An opaque type means that the methods won't be used by the inlets the and outlets with a connection in a graphical context but internally (directly from an object to another, for example). You should use the opaque call to use it.
         @param name    A tag that matchs to a method.
         @param type    A type that defines the method parameters.
         @param method  A function that matchs the method prototypes depending of the type.
         */
        void addMethod(string name, Type type, Method method);
        
        //! Remove a method.
        /** The function removes a method from the method list.
         @param name    A tag that matchs to a method.
         */
        void removeMethod(const shared_ptr<Tag> name) noexcept;
        
        //! Remove a method.
        /** The function removes a method from the method list.
         @param name    A tag that matchs to a method.
         */
        void removeMethod(string name) noexcept;
        
        //! Tag factory.
        /** This function uses the instance specific tag factory to checks if a tag with this name has already been created and returns it, otherwise the tag factory creates a new tag with this name.
         @param     The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        shared_ptr<Tag> createTag(string name) const;
        
        //! Object factory.
        /** This function uses the instance specific object factory to create an object.
         @param     The name of the tag to retrieve.
         */
        shared_ptr<Object> createObject(shared_ptr<Tag> name, vector<Element>& elements) const;
        
        //! Object factory.
        /** This function uses the instance specific object factory to create an object.
         @param     The name of the tag to retrieve.
         */
        shared_ptr<Object> createObject(string name, vector<Element>& elements) const;
        
        //! Dico factory.
        /** This function uses the instance specific object factory to create a dico.
         */
        shared_ptr<Dico> createDico() const;
        
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
}


#endif


