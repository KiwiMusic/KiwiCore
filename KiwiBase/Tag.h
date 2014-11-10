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

#include "Tools.h"
#include "Console.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" string (in the scope of a tag factory).
    /**
     The tag can be created only by the tag factory class and are never destroyed until the deletion of the it. If you create a tag with a string already used that already matchs to a tag the tag factory will return this tag, otherwise it will create a new tag.
     @see TagFactory
     */
    class Tag
    {
    private:
        
        const string m_name;
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Tag(string const& name) noexcept : m_name(name)
        {
            ;
        }
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Tag()
        {
            ;
        }
        
        //! Retrieve the string of the tag.
        /** The function retrieves the unique string of the tag.
         @return The string of the tag.
         */
        inline string getName() const noexcept
        {
            return m_name;
        }
        
    private:
        
        static map<string, shared_ptr<const Tag>> m_tags;
    public:
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it, otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        static inline shared_ptr<const Tag> create(string const& name)
        {
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                shared_ptr<const Tag> tag = make_shared<Tag>(name);
                m_tags[name] = tag;
                return tag;
            }
        }
        
        static shared_ptr<const Tag> text;
        static shared_ptr<const Tag> box;
        static shared_ptr<const Tag> boxes;
        static shared_ptr<const Tag> connection;
        static shared_ptr<const Tag> connections;
        static shared_ptr<const Tag> id;
        static shared_ptr<const Tag> from;
        static shared_ptr<const Tag> to;
        static shared_ptr<const Tag> name;
        static shared_ptr<const Tag> ninlets;
        static shared_ptr<const Tag> noutlets;
        static shared_ptr<const Tag> set;
        static shared_ptr<const Tag> null;
        static shared_ptr<const Tag> arguments;
		static shared_ptr<const Tag> frozen_attributes;
    };
    
    typedef shared_ptr<const Tag> sTag;
    
    typedef weak_ptr<const Tag> wTag;
    
    inline string toString(sTag __val)
    {
        return __val->getName();
    }
};


#endif


