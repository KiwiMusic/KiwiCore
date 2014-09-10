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
    
    Dico::Dico(shared_ptr<Instance> kiwi) : Object(kiwi, "dico")
    {
        ;
    }
    
    Dico::~Dico()
    {
        clear();
    }
    
    void Dico::clear()
    {
        m_entries.clear();
    }
    
    void Dico::getKeys(vector<Element>& elements)
    {
        elements.clear();
        for(map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.begin(); it != m_entries.end(); ++it)
            elements.push_back(it->first);
    }
    
    void Dico::clear(shared_ptr<Tag> key)
    {
        if(m_entries.find(key) != m_entries.end())
            m_entries.erase(m_entries.find(key));
    }
    
    bool Dico::has(shared_ptr<Tag> key)
    {
        return m_entries.find(key) != m_entries.end();
    }
    
    Type Dico::type(shared_ptr<Tag> key)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
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
    
    bool Dico::isLong(shared_ptr<Tag> key)
    {
        return type(key) == T_LONG;
    }
    
    bool Dico::isDouble(shared_ptr<Tag> key)
    {
        return type(key) == T_DOUBLE;
    }
    
    bool Dico::isTag(shared_ptr<Tag> key)
    {
        return type(key) == T_TAG;
    }
    
    bool Dico::isObject(shared_ptr<Tag> key)
    {
        return type(key) == T_OBJECT;
    }
    
    bool Dico::isElements(shared_ptr<Tag> key)
    {
        return type(key) == T_ELEMENTS;
    }
    
    long Dico::getLong(shared_ptr<Tag> key)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return (long)elements[0];
            }
        }
        return 0;
    }
    
    double Dico::getDouble(shared_ptr<Tag> key)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return (double)elements[0];
            }
        }
        return 0.;
    }
    
    shared_ptr<Tag> Dico::getTag(shared_ptr<Tag> key)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return (shared_ptr<Tag>)elements[0];
            }
        }
        return nullptr;
    }
    
    shared_ptr<Object> Dico::getObject(shared_ptr<Tag> key)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return (shared_ptr<Object>)elements[0];
            }
        }
        return nullptr;
    }
    
    void Dico::getElements(shared_ptr<Tag> key, vector<Element>& elements)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            elements = it->second;
        }
    }

    void Dico::set(shared_ptr<Tag> key, int value)
    {
        set(key, (long)value);
    }
    
    void Dico::set(shared_ptr<Tag> key, long value)
    {
        vector<Element> elements;
        elements.push_back(value);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, float value)
    {
        set(key, (double)value);
    }
    
    void Dico::set(shared_ptr<Tag> key, double value)
    {
        vector<Element> elements;
        elements.push_back(value);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, string value)
    {
        set(key, createTag(value));
    }
    
    void Dico::set(shared_ptr<Tag> key, shared_ptr<Tag> tag)
    {
        vector<Element> elements;
        elements.push_back(tag);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, shared_ptr<Object> object)
    {
        vector<Element> elements;
        elements.push_back(object);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, Element element)
    {
        vector<Element> elements;
        elements.push_back(element);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, vector<Element> const& elements)
    {
        if(elements.size() == 0)
            return;
        m_entries[key] = elements;
    }
    
    void Dico::append(shared_ptr<Tag> key, int value)
    {
        append(key, (long)value);
    }
    
    void Dico::append(shared_ptr<Tag> key, long value)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(value);
        else
            set(key, value);
    }
    
    void Dico::append(shared_ptr<Tag> key, float value)
    {
        append(key, (double)value);
    }
    
    void Dico::append(shared_ptr<Tag> key, double value)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(value);
        else
            set(key, value);
    }
    
    void Dico::append(shared_ptr<Tag> key, string value)
    {
        append(key, createTag(value));
    }
    
    void Dico::append(shared_ptr<Tag> key, shared_ptr<Tag> tag)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(tag);
        else
            set(key, tag);
    }
    
    void Dico::append(shared_ptr<Tag> key, shared_ptr<Object> object)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(object);
        else
            set(key, object);
    }
    
    void Dico::append(shared_ptr<Tag> key, Element element)
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(element);
        else
            set(key, element);
    }
    
    void Dico::append(shared_ptr<Tag> key, vector<Element> const& elements)
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
    
    void Dico::write(string filename, string directoryname)
    {
        createJson()->write(static_pointer_cast<Dico>(shared_from_this()), filename, directoryname);
    }
    
    void Dico::read(string filename, string directoryname)
    {
        createJson()->read(static_pointer_cast<Dico>(shared_from_this()), filename, directoryname);
    }
    
    void Dico::post()
    {
        createJson()->post(static_pointer_cast<Dico>(shared_from_this()));
    }
}
















