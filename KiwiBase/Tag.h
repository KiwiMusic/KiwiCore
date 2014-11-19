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
    
    //! The tag is an unique object that matchs to a "unique" string (in the scope of all the kiwi application).
    /**
     The tag are uniques and matchs to a string. If you create a tag with a string that already matchs to a tag, the creation function will return this tag, otherwise it will create a new tag.
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
        
        static unordered_map<string, sTag> m_tags;
        static mutex                       m_mutex;
    public:
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it, otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        static inline sTag create(string const& name)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                sTag tag = make_shared<Tag>(name);
                m_tags[name] = tag;
                return tag;
            }
        }
        
        static sTag empty;
        static sTag text;
        static sTag box;
        static sTag boxes;
        static sTag link;
        static sTag links;
        static sTag id;
        static sTag from;
        static sTag to;
        static sTag name;
        static sTag ninlets;
        static sTag noutlets;
        static sTag set;
        static sTag null;
        static sTag arguments;
		static sTag frozen_attributes;
    };
    
    inline string toString(sTag __val)
    {
        return __val->getName();
    }
};


#endif


