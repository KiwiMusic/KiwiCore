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

#ifndef __DEF_KIWI_TAG__
#define __DEF_KIWI_TAG__

#include "Defs.h"
#include "Tools.h"

namespace Kiwi
{
    class Object;
    class Tag;
    
    // ================================================================================ //
    //                                      TAG FACTORY                                 //
    // ================================================================================ //
    
    //! The tag factory creates tag. More often, you shouldn't need to use it.
    /**
     The tag factory is the only class that can create and delete tags. Important, a tag factory is always created with a kiwi instance to manage global instance's tag ans most often this tag factory will be sufficient for your needs. If you want another tag factory, don't forget that the tags created with it won't be retrievable by the natives classes or other extern classes. With other words, if you just want to bind your object to a global tag use the method of your object.
     @see Tag
     */
    
    class TagFactory
    {
    private:
        map<string, shared_ptr<Tag>> m_tags;
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        TagFactory();
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~TagFactory();
        
        //! Tag factory.
        /** This function checks if a tag with this name has already been created and returns it, otherwise it creates a new tag with this name.
         @param     The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        shared_ptr<Tag> createTag(string const& name) noexcept;
    };
    
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" string (in the scope of a tag factory) and can be used to bind objects and retrieve them. See the tag factory for futher informations.
    /**
    The tag can be created only by the tag factory class and are never destroyed until the deletion of the it. If you create a tag with a string already used that already matchs to a tag the tag factory will return this tag, otherwise it will create a new tag. Then you can use the tag to bind objects and to send messages and call methods.
     @see TagFactory
     */
    class Tag
    {
    private:
        const string  m_name;
        set<weak_ptr<Object>, owner_less<weak_ptr<Object>>> m_objects;

    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Tag(string const& name) noexcept;
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Tag();
        
        //! Retrieve the string of the tag.
        /** The function retrieves the unique string of the tag.
         @return The string of the tag.
         */
        inline operator string() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the number of objects in the binding list of the tag.
        /** The function retrieves the number of objects in the binding list of the tag.
         @return The number of objects binded to the tag.
         */
        inline int size() const noexcept
        {
            return (int)m_objects.size();
        }
        
        //! Retrieve an object from the binding list of the tag.
        /** The function retrieves an object from the binding list of the tag at a defined position.
         @param index   The position of the object in the binding list from 0 to the number of objects in the binding list -1.
         @return        The pointer of the binded objects or NULL is the index is less than 0 or greater or equal to the number of objects in the binding list.
         */
        inline weak_ptr<Object> operator[](int index) const noexcept
        {
            set<weak_ptr<Object>>::iterator it = m_objects.begin();
            while(--index && it != m_objects.end())
                ++it;
            return *(it);
        }
        
        //! Add an objects in the binding list of the tag.
        /** The function adds an object in the binding list of the tag. If the object is already in the binding list, the function doesn't do anything.
         @param object  The pointer of the object.
         @see           unbind()
         */
        void bind(weak_ptr<Object> object);
        
        //! Remove an objects from the binding list of the tag.
        /** The function removes an object from the binding list of the tag. If the object isn't in the binding list, the function doesn't do anything.
         @param object  The pointer of the object.
         @see           bind()
         */
        void unbind(weak_ptr<Object> object);
    };
};


#endif


