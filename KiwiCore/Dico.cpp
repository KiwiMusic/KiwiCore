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

#include "Dico.h"
#include "Instance.h"
#include "Json.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    Dictionary::Dictionary(shared_ptr<Instance> kiwi) : Object(kiwi, "dico")
    {
        ;
    }
    
    Dictionary::~Dictionary()
    {
        clear();
    }
    
    void Dictionary::clear() noexcept
    {
        m_entries.clear();
    }
    
    void Dictionary::clear(shared_ptr<Tag> key) noexcept
    {
        m_entries.erase(key);
    }
    
    void Dictionary::keys(vector<Element>& elements) const noexcept
    {
        elements.clear();
        for(map<const shared_ptr<Tag>, vector<Element>>::const_iterator it = m_entries.begin(); it != m_entries.end(); ++it)
            elements.push_back(it->first);
    }
    
    bool Dictionary::has(shared_ptr<Tag> key) const noexcept
    {
        return m_entries.find(key) != m_entries.end();
    }
    
    Type Dictionary::type(shared_ptr<Tag> key) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size() == 1)
            {
                return elements[0].type();
            }
            else
            {
                return T_ELEMENTS;
            }
        }
        else
            return T_NOTHING;
    }
    
    Element Dictionary::get(shared_ptr<Tag> key) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return elements[0];
            }
        }
        return 0;
    }
    
    void Dictionary::get(shared_ptr<Tag> key, vector<Element>& elements) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            elements = it->second;
        }
    }
    
    void Dictionary::set(shared_ptr<Tag> key, Element const& element) noexcept
    {
        vector<Element> elements;
        elements.push_back(element);
        m_entries[key] = elements;
    }
    
    void Dictionary::set(shared_ptr<Tag> key, vector<Element> const& elements) noexcept
    {
        if(elements.size() == 0)
            return;
        m_entries[key] = elements;
    }

    void Dictionary::append(shared_ptr<Tag> key, Element const& element) noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(element);
        else
            set(key, element);
    }
    
    void Dictionary::append(shared_ptr<Tag> key, vector<Element> const& elements) noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            for(int i = 0; i < elements.size(); i++)
            {
                it->second.push_back(elements[i]);
            }
        }
        else
            set(key, elements);
    }
    
    void Dictionary::write(string filename, string directoryname)
    {
        createJson()->write(static_pointer_cast<Dictionary>(shared_from_this()), filename, directoryname);
    }
    
    void Dictionary::read(string filename, string directoryname)
    {
        createJson()->read(static_pointer_cast<Dictionary>(shared_from_this()), filename, directoryname);
    }
    
    void Dictionary::post()
    {
        createJson()->post(static_pointer_cast<Dictionary>(shared_from_this()));
    }
}
















