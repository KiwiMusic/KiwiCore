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


#include "Object.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    Tag::Tag(string const& name) noexcept : m_name(name)
    {
        
    }
    
    Tag::~Tag()
    {
        m_objects.clear();
    }
    
    void Tag::bind(weak_ptr<Object> object)
    {
        m_objects.insert(object);
    }
    
    void Tag::unbind(weak_ptr<Object> object)
    {
        m_objects.erase(object);
    }
    
    // ================================================================================ //
    //                                      TAG FACTORY                                 //
    // ================================================================================ //
    
    TagFactory::TagFactory()
    {
        
    }
    
    TagFactory::~TagFactory()
    {
        m_tags.clear();
    }
    
    shared_ptr<Tag> TagFactory::createTag(string const& name) noexcept
    {
        map<string, shared_ptr<Tag>>::const_iterator it = m_tags.find(name);
        if(it != m_tags.end())
            return it->second;
        else
        {
            shared_ptr<Tag> newTag = make_shared<Tag>(name);
            m_tags[name] = newTag;
            return newTag;
        }
    }
}









