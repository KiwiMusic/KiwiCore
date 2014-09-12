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
#include "Dico.h"

namespace Kiwi
{
    Attribute::Attribute(shared_ptr<Tag> name) :
    m_name(name)
    {
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
    
    void Attribute::behavior(bool opaque, bool visible, bool save) noexcept
    {
        m_opaque = opaque;
        m_visible = visible;
        m_save = save;
    }
    
    void Attribute::get(vector<Element>& elements) const noexcept
    {
        elements = m_values;
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
    
    void Attribute::write(shared_ptr<Dico> dico) const noexcept
    {
        dico->set(m_name, m_values);
    }
    
    void Attribute::read(shared_ptr<const Dico> dico) noexcept
    {
        vector<Element> elements;
        dico->get(m_name, elements);
        set(elements);
    }
}
















