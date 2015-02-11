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
#include "Atom.h"
#include "Console.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" string in the scope of all the kiwi applications.
    /**
     The tag are uniques and matchs to a string. If you create a tag with a string that already matchs to a tag, the creation function will return this tag, otherwise it will create a new tag.
     @see TagFactory
     */
    class Tag : public Atom::Quark
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
        ~Tag() noexcept
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
        
        static map<string, sTag> m_tags;
        static mutex             m_mutex;

    public:
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it, otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return    The tag that match with the name.
         */
        static inline sTag create(string const& name) noexcept
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
        
        class List;
    };
    
    //! Tag comparaison by alphabetic order.
    /** This function compares the strings of the tags.
     @param  tag1  The first tag.
     @param  tag2  The second tag.
     @return true if the the first tag before the sedond one in the alphabetic order.
     */
    static inline bool operator<(const sTag tag1, const sTag tag2)
    {
        return tag1->getName() < tag2->getName();
    }
	
    inline string toString(sTag __val)
    {
        return __val->getName();
    }
    
    class Tag::List
    {
    public:
        static const sTag arguments;
        static const sTag Arial;
        
        static const sTag bang;
        static const sTag bold;
        static const sTag bold_italic;
        
        static const sTag center;
        static const sTag color;
        static const sTag Color;
        
        static const sTag focus;
        static const sTag Font;
        static const sTag Font_Face;
        static const sTag Font_Justification;
        static const sTag Font_Name;
        static const sTag Font_Size;
        static const sTag fontface;
        static const sTag fontjustification;
        static const sTag fontname;
        static const sTag fontsize;
        static const sTag from;
        
        static const sTag id;
        static const sTag italic;
        
        static const sTag left;
        static const sTag link;
        static const sTag links;
        
        static const sTag Menelo;
        static const sTag mescolor;
        static const sTag Message_Color;
        
        static const sTag name;
        static const sTag ninlets;
        static const sTag normal;
        static const sTag noutlets;
        
        static const sTag object;
        static const sTag objects;
        
        static const sTag patcher;
        
        static const sTag right;
        
        static const sTag set;
        static const sTag sigcolor;
        static const sTag Signal_Color;
        
        static const sTag text;
        static const sTag to;
        
    };
};


#endif


