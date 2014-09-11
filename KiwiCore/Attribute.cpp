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

#include "Attribute.h"
#include "Object.h"
#include "Dico.h"
#include "Instance.h"

namespace Kiwi
{
    Attribute::Attribute(shared_ptr<Instance> kiwi, shared_ptr<Tag> name) :
    m_kiwi(kiwi),
    m_name(name)
    {
        appearance("unknown", "unknown", "unknown");
        behavior(true, false, true);
    }
    
    Attribute::~Attribute()
    {
        m_values.clear();
    }
    
    void Attribute::appearance(shared_ptr<Tag> label, shared_ptr<Tag> style, shared_ptr<Tag> category) noexcept
    {
        m_label = label;
        m_style = style;
        m_category = category;
    }
    
    void Attribute::appearance(string const& label, string const& style, string const& category) noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            m_label = kiwi->createTag(label);
            m_style = kiwi->createTag(style);
            m_category = kiwi->createTag(category);
        }
    }
    
    void Attribute::behavior(bool opaque, bool visible, bool save) noexcept
    {
        m_opaque = opaque;
        m_visible = visible;
        m_save = save;
    }
    
    void Attribute::get(vector<Element>& elements) const noexcept
    {
        for(int i = 0; i < m_values.size(); i++)
            elements[i] = m_values[i];
    }
    
    Element Attribute::get() const noexcept
    {
        if(m_values.size())
            return m_values[0];
        else
            return 0;
    }
    
    void Attribute::set(Element const& element) noexcept
    {
        m_values = {element};
    }
    
    void Attribute::set(vector<Element> const& elements) noexcept
    {
        m_values = elements;
    }
    
    void Attribute::write(shared_ptr<Dico> dico)
    {
        dico->set(m_name, m_values);
    }
    
    void Attribute::read(shared_ptr<Dico> dico)
    {
        vector<Element> elements;
        dico->get(m_name, elements);
        set(elements);
    }
}
















