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
    
    // ================================================================================ //
    //                                  ATTRIBUTE FACTORY                               //
    // ================================================================================ //
    
    AttributeFactory::AttributeFactory() noexcept
    {
        
    }
    
    AttributeFactory::~AttributeFactory()
    {
        m_attributes.clear();
    }
    
    
    void AttributeFactory::receive(ElemVector const& elements)
    {
        if(elements.size() && elements[0].isTag())
        {
            map<sTag, sAttribute>::const_iterator it = m_attributes.find((sTag)elements[0]);
            if(it != m_attributes.end())
            {
                ElemVector nelements;
                nelements.assign(elements.begin()+1, elements.end());
                if(!(it->second->m_behavior & Attribute::Opaque))
                {
                    it->second->set(nelements);
                    if(it->second->m_behavior & Attribute::Notify)
                    {
                        notify(it->second);
                    }
                }
            }
        }
    }
    
    void AttributeFactory::write(shared_ptr<Dico> dico) const noexcept
    {
        for(map<sTag, sAttribute>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            if(it->second->m_behavior & Attribute::Saved)
            {
                it->second->write(dico);
            }
        }
    }
    
    void AttributeFactory::read(shared_ptr<const Dico> dico) noexcept
    {
        for(map<sTag, sAttribute>::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            it->second->read(dico);
        }
    }
    
    sAttribute AttributeFactory::getAttribute(sTag name) const noexcept
    {
        map<sTag, sAttribute>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end() && it->second->m_behavior & Attribute::Visible)
            return it->second;
        else
            return nullptr;
    }
    
    void AttributeFactory::notify(sAttribute attr)
    {
        ;
    }
    
    void AttributeFactory::addAttribute(shared_ptr<Attribute> attr)
    {
        m_attributes[attr->name()] = attr;
    }
    
    void AttributeFactory::setAttributeAppearance(sTag name, string const& label, string const& style, string const& category)
    {
        map<sTag, sAttribute>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->m_label = label;
            it->second->m_style = style;
            it->second->m_category = category;
        }
    }
    
    void AttributeFactory::setAttributeBehavior(sTag name, long behavior)
    {
        map<sTag, sAttribute>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->m_behavior = 0 | behavior;
        }
    }
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    Attribute::Attribute(sTag name,
                         string const& label,
                         string const& style,
                         string const& category,
                         long behavior) :
    m_name(name),
    m_label(label),
    m_style(style),
    m_category(category),
    m_behavior(0 | behavior)
    {
        
        ;
    }
    
    Attribute::~Attribute()
    {
        ;
    }
    
    void Attribute::write(shared_ptr<Dico> dico) const noexcept
    {
        ElemVector elements;
        get(elements);
        dico->set(m_name, elements);
    }
    
    void Attribute::read(shared_ptr<const Dico> dico)
    {
        ElemVector elements;
        dico->get(m_name, elements);
        set(elements);
    }
    
    // ================================================================================ //
    //                                      ATTRIBUTE TYPED                             //
    // ================================================================================ //
    
    AttributeDouble::AttributeDouble(sTag name, string const& label, string const& style, string const& category, long behavior) : Attribute(name, label, style, category, behavior)
    {
    }
    
    AttributeDouble::~AttributeDouble()
    {
    }
    
    void AttributeDouble::set(ElemVector const& elements)
    {
        if(elements.size() && (elements[0].isLong() || elements[0].isDouble()))
        {
            m_value = elements[0];
        }
    }
    
    void AttributeDouble::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }

    AttributeTag::AttributeTag(sTag name, string const& label, string const& style, string const& category, long behavior) : Attribute(name, label, style, category, behavior)
    {
    }
    
    AttributeTag::~AttributeTag()
    {
    }
    
    void AttributeTag::set(ElemVector const& elements)
    {
        if(elements.size() && elements[0].isTag())
        {
            m_value = elements[0];
        }
    }
    
    void AttributeTag::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
    
    AttributeObject::AttributeObject(sTag name, string const& label, string const& style, string const& category, long behavior) : Attribute(name, label, style, category, behavior)
    {
    }
    
    AttributeObject::~AttributeObject()
    {
    }
    
    void AttributeObject::set(ElemVector const& elements)
    {
        if(elements.size() && elements[0].isObject())
        {
            m_value = elements[0];
        }
    }
    
    void AttributeObject::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
}
















