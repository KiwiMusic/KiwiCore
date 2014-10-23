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
#include "Dico.h"

namespace Kiwi
{
    class Instance;
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The basic object.
    /**
     Most of the kiwi classes inherites from the object. Please look at the several implementations to understand how to define the inheritance.
     */
    class Object : public enable_shared_from_this<Object>
    {
    public:
        typedef enum
        {
            OBJECT  = 0,
            BOX     = 1,
            DSPBOX  = 2
        }Type;
        // ================================================================================ //
        //                                      STRUCTURE                                   //
        // ================================================================================ //
        
    private:
        const weak_ptr<Instance>    m_kiwi;
        const sTag                  m_name;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        Object(shared_ptr<Instance> kiwi, string const& name) noexcept;
        
        //! Descrutor.
        /** Doesn't perform anything.
         */
        virtual ~Object();
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        sTag name() const noexcept;
        
        //! Retrieve the type of the object.
        /** The function retrieves the type of the object.
         @return The type of the object.
         */
        virtual Type type() const noexcept;
        
        //! The virtual constructor called by the kiwi instance to create an instance of the object class.
        /** The function retrieves an instance of the object class.
         @param dico        The dico that defines the object.
         */
        virtual shared_ptr<Object> create(shared_ptr<Dico> dico) const;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual void receive(ElemVector const& elements);
        
        //! Write the object in a dico.
        /** The function writes the name of the object in a dico.
         @param dico The dico.
         */
        virtual void write(shared_ptr<Dico> dico) const noexcept;
        
        //! Read a dico.
        /** The function reads a dico but doesn't do anything.
         @param dico The dico.
         */
        virtual void read(shared_ptr<const Dico> dico) noexcept;
        
    protected:
        
        //! Send an object error about a wrong method called.
        /** The function sends an object error about a wrong method called. You should call this method if some kind of "selector" received doesn't match at all with what the object is expecting. For example, the first element of the vector is a Tag with the name "zaza" but the object is only expecting "zozo" as tag then can use this method.
         @param name    The name of the "selector" received.
         */
        void wrong(sTag const& name) const noexcept;
        
        //! Send an object error about a wrong method called.
        /** The function sends an object error about a wrong method called. You should call this method if the "selector" that the object received is good but the arguments hasn't been well defined.
         @param name    The name of the "selector" received.
         @param type    The type of arguments expected.
         @param elements The vector of elements received.
         */
        void wrong(sTag const& name, Element::Type type, ElemVector const& elements) const noexcept;
        
    protected:
        
        //! Object maker.
        /** This function makes an object from the class with the same kiwi instance.
         */
        template<class ObjectClass, class Args> shared_ptr<ObjectClass> makeObject(Args&& arguments) const
        {
            return make_shared<ObjectClass>(m_kiwi.lock(), forward<Args>(arguments));
        }
        
        //! Tag creator.
        /** This function uses the kiwi instance to create a tag.
         @param name The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        sTag createTag(string const& name) const;
        
        //! Object creator.
        /** This function uses the kiwi instance to create an object with a dico.
         @param dico The dico that defines the object.
         @return    The object.
         */
        shared_ptr<Object> createObject(sDico dico) const;
        
        //! Dico creator.
        /** This function uses the kiwi instance to create a dico.
         @return    The dico.
         */
        sDico createDico() const;
        
        // ================================================================================ //
        //                                      MESSAGES                                    //
        // ================================================================================ //
        
        //! Post a standard message.
        /** The function post a message via the instance specific console.
         @param message  The message in the string format.
         */
        void    post(string message) const noexcept;
        
        //! Post a warning.
        /** The function post a message via the instance specific console.
         @param message  The warning in the string format.
         */
        void    warning(string message) const noexcept;
        
        //! Post an error.
        /** The function post a message via the instance specific console.
         @param message  The error in the string format.
         */
        void    error(string message) const noexcept;
        
        //! Post a standard message that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The message in the string format.
         */
        void    post(shared_ptr<const Object> object, string message) const noexcept;
        
        //! Post a warning that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The warning in the string format.
         */
        void    warning(shared_ptr<const Object> object, string message) const noexcept;
        
        //! Post an error that refers to a specific object.
        /** The function post a message from this object via the instance specific console.
         @param object  The object sender.
         @param message The error in the string format.
         */
        void    error(shared_ptr<const Object> object, string message) const noexcept;
        
    };
    
    typedef shared_ptr<Object>      sObject;
    
    inline string toString(const shared_ptr<Object> __val)
    {
        return toString(__val->name());
    }
}


#endif


