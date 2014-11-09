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
            auto it = m_attributes.find((sTag)elements[0]);
            if(it != m_attributes.end())
            {
                ElemVector nelements;
                nelements.assign(elements.begin()+1, elements.end());
                if(!(it->second->m_behavior & Attribute::Opaque))
                {
                    it->second->set(nelements);
                    if(!(it->second->m_behavior & Attribute::NotNotify))
                    {
                        notify(it->second);
                    }
                }
            }
        }
    }
    
    void AttributeFactory::write(shared_ptr<Dico> dico) const noexcept
    {
        for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            if(!(it->second->m_behavior & Attribute::NotSaved))
            {
                it->second->write(dico);
            }
        }
    }
    
    void AttributeFactory::read(shared_ptr<const Dico> dico) noexcept
    {
        for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            it->second->read(dico);
        }
    }
    
    sAttribute AttributeFactory::getAttribute(sTag name) const noexcept
    {
        auto it = m_attributes.find(name);
        if(it != m_attributes.end() && !(it->second->m_behavior & Attribute::Invisible))
            return it->second;
        else
            return nullptr;
    }
    
	void AttributeFactory::setAttributeAppearance(sTag name, string const& label, Attribute::Style style, string const& category)
    {
        auto it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->m_label = label;
            it->second->m_style = style;
            it->second->m_category = category;
        }
    }
    
    void AttributeFactory::setAttributeBehavior(sTag name, long behavior)
    {
        auto it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->m_behavior = 0 | behavior;
        }
    }
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
	Attribute::Attribute(sTag name,
						 Type type,
						 ElemVector defaultValue,
						 string const& label,
						 Style style,
						 string const& category,
						 long behavior) :
    m_name(name),
	m_type(type),
	m_default_value(defaultValue),
    m_label(label),
    m_style(style),
    m_category(category),
    m_behavior(0 | behavior),
	m_frozen(false)
    {
		;
    }
    
    Attribute::~Attribute()
    {
        m_frozen_value.clear();
    }
	
	void Attribute::setBehavior(Behavior behavior)
	{
		if(m_behavior != behavior)
		{
			m_behavior = behavior;
		}
	}
	
	void Attribute::setInvisible(const bool b) noexcept
	{
		if (b)
			m_behavior &= ~Invisible;
		else
			m_behavior |= Invisible;
	}
	
	void Attribute::setOpaque(const bool b) noexcept
	{
		if (b)
			m_behavior &= ~Opaque;
		else
			m_behavior |= Opaque;
	}
	
	void Attribute::setDisabled(const bool b) noexcept
	{
		if (b)
			m_behavior &= ~Disabled;
		else
			m_behavior |= Disabled;
	}

	void Attribute::setSaved(const bool b) noexcept
	{
		if (b)
			m_behavior &= ~NotSaved;
		else
			m_behavior |= NotSaved;
	}
	
	void Attribute::setNotify(const bool b) noexcept
	{
		if (b)
			m_behavior &= ~NotNotify;
		else
			m_behavior |= NotNotify;
	}
	
	void Attribute::freeze(const bool frozen)
	{
		m_frozen = frozen;
		
		if (m_frozen)
			get(m_frozen_value);
		else
			m_frozen_value.clear();
	}
	
	void Attribute::reset()
	{
		set(m_default_value);
	}
	
    void Attribute::write(shared_ptr<Dico> dico) const noexcept
    {
		if(!(m_behavior & Behavior::NotSaved) || m_frozen)
		{
			ElemVector elements;
			
			if(!m_frozen)
				get(elements);
			else
				elements = m_frozen_value;
				
			dico->set(m_name, elements);
		}
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
    
	AttributeLong::AttributeLong(sTag name,
								 long defaultValue,
								 string const& label,
								 string const& category,
								 Attribute::Style style,
								 long behavior)
		: Attribute(name, Attribute::Type::Long, {defaultValue}, label, style, category, behavior)
    {
    }
    
    void AttributeLong::set(ElemVector const& elements)
    {
        if(elements.size() && (elements[0].isLong() || elements[0].isDouble()))
        {
            m_value = elements[0];
        }
    }
    
    void AttributeLong::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
    
	AttributeDouble::AttributeDouble(sTag name,
									 double defaultValue,
									 string const& label,
									 string const& category,
									 Attribute::Style style,
									 long behavior) :
		Attribute(name, Attribute::Type::Double, {defaultValue}, label, style, category, behavior)
    {
    }
    
    void AttributeDouble::set(ElemVector const& elements)
    {
        if(elements.size() && elements[0].isNumber())
        {
            m_value = elements[0];
        }
    }
    
    void AttributeDouble::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
	
	
	AttributeBool::AttributeBool(sTag name,
								 int defaultValue,
								 string const& label,
								 string const& category,
								 Attribute::Style style,
								 long behavior) :
	Attribute(name, Attribute::Type::Long, {defaultValue}, label, style, category, behavior)
	{
		
	}
	
	void AttributeBool::set(ElemVector const& elements)
	{
		if(elements.size() && elements[0].isNumber())
		{
			m_value = ((int)elements[0] != 0);
		}
	}
	
	void AttributeBool::get(ElemVector& elements) const noexcept
	{
		elements = {m_value};
	}

	AttributeTag::AttributeTag(sTag name,
							   string defaultValue,
							   string const& label,
							   string const& category,
							   Attribute::Style style,
							   long behavior) :
		Attribute(name, Attribute::Type::Tag, {defaultValue}, label, style, category, behavior)
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
	
	
	AttributeBox::AttributeBox(sTag name,
							   shared_ptr<Box> defaultValue,
							   string const& label,
							   string const& category,
							   Attribute::Style style,
							   long behavior) :
		Attribute(name, Attribute::Type::BoxPointer, {defaultValue}, label, style, category, behavior)
    {
    }
    
    void AttributeBox::set(ElemVector const& elements)
    {
        if(elements.size() && elements[0].isObject())
            m_value = elements[0];
    }
    
    void AttributeBox::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
	
	
	
	AttributeColor::AttributeColor(sTag name,
								   ElemVector defaultValue,
								   string const& label,
								   string const& category,
								   Attribute::Style style,
								   long behavior)
	: Attribute(name, Attribute::Type::DoubleArray, defaultValue, label, style, category, behavior)
	{
	}
	
	AttributeColor::~AttributeColor()
	{
		m_value.clear();
	}
	
	void AttributeColor::set(ElemVector const& elements)
	{
		const size_t nelems = elements.size();
		
		for (unsigned int i = 0; i < 4; i++)
		{
			if (nelems > i && elements[i].isNumber())
				m_value[i] = clip((double)elements[i], (double)0.f, (double)1.f);
			else
				m_value[i] = (i < 3) ? 0.f : 1.f; // default is black with full alpha color
		}
	}
	
	void AttributeColor::get(ElemVector& elements) const noexcept
	{
		elements = {m_value};
	}
}
















