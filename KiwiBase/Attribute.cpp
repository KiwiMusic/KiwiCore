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
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
	Attribute::Attribute(sTag name,
						 ElemVector defaultValue,
						 string const& label,
						 Style style,
						 string const& category,
						 long behavior) :
    m_name(name),
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
	
	void Attribute::setBehavior(long behavior)
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
		if (!b)
			m_behavior &= ~Disabled;
		else
			m_behavior |= Disabled;
	}

	void Attribute::setSaveable(const bool b) noexcept
	{
		if (!b)
			m_behavior &= ~NotSaveable;
		else
			m_behavior |= NotSaveable;
	}
	
	void Attribute::setNotifyChanges(const bool b) noexcept
	{
		if (!b)
			m_behavior &= ~NotNotifyChanges;
		else
			m_behavior |= NotNotifyChanges;
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
	
    void Attribute::write(sDico dico) const noexcept
    {
		if(!(m_behavior & Behavior::NotSaveable) || m_frozen)
		{
			ElemVector elements;
			
			if(!m_frozen)
				get(elements);
			else
				elements = m_frozen_value;
				
			dico->set(m_name, elements);
		}
    }
    
    void Attribute::read(scDico dico)
    {
        ElemVector elements;
        dico->get(m_name, elements);
        set(elements);
    }
	
	// ================================================================================ //
	//                                 ATTRIBUTES MANAGER								//
	// ================================================================================ //
	
	
	Attribute::Manager::Manager() noexcept
	{
		;
	}
	
	Attribute::Manager::~Manager()
	{
		m_attributes.clear();
		m_listeners.clear();
	}
	
	sAttribute Attribute::Manager::getAttribute(sTag name) const noexcept
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end() && !(it->second->isInvisible()))
			return it->second;
		else
			return nullptr;
	}
	
	void Attribute::Manager::addAttribute(sAttribute attr)
	{
		if (attr != nullptr)
		{
			sTag attrname = attr->getName();
			
			m_attributes[attrname] = attr;
			
			triggerNotification(attr, NotificationType::AttrAdded);
		}
	}
	
	/*
	void Attribute::Manager::addAttributeSet(shared_ptr<AttributeSet> attrset)
	{
		if (attrset)
		{
			vector<sTag> names = attrset->getAttributeNames();
			
			for (int i=0; i < names.size(); i++)
				addAttribute(attrset->getAttribute(names[i]));
		}
	}
	*/
	
	void Attribute::Manager::removeAttribute(sAttribute attr)
	{
		if (attr != nullptr)
		{
			sTag attrname = attr->getName();
			m_attributes.erase(attrname);
		}
	}
	
	void Attribute::Manager::removeAttribute(sTag name)
	{
		removeAttribute(getAttribute(name));
	}
	
	bool Attribute::Manager::setValue(sTag name, ElemVector const& elements)
	{
		sAttribute attr = getAttribute(name);
		
		if (attr != nullptr && !attr->isOpaque())
		{
			attr->set(elements);
			if(attr->shouldNotifyChanges())
				triggerNotification(attr, NotificationType::ValueChanged);
			
			return true;
		}
		
		return false;
	}
	
	bool Attribute::Manager::getValue(sTag name, ElemVector& elements, ElemVector const& defaultValue)
	{
		sAttribute attr = getAttribute(name);
		
		if (attr != nullptr && !attr->isOpaque())
		{
			attr->get(elements);
			return true;
		}
		else
		{
			elements = defaultValue;
		}
		
		return false;
	}
	
	void Attribute::Manager::receive(ElemVector const& elements)
	{
		if(elements.size() && elements[0].isTag())
		{
			auto it = m_attributes.find((sTag)elements[0]);
			if(it != m_attributes.end())
			{
				ElemVector nelements;
				nelements.assign(elements.begin()+1, elements.end());
				
				sAttribute attr = it->second;
				
				if(!attr->isOpaque())
				{
					attr->set(nelements);
					
					if(attr->shouldNotifyChanges())
						triggerNotification(attr, NotificationType::ValueChanged);
				}
			}
		}
	}
	
	void Attribute::Manager::triggerNotification(sAttribute attr, NotificationType type)
	{
		for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
		{
			if(!it->expired())
				it->lock()->attributeChanged(this, attr, type);
		}
	}
	
	void Attribute::Manager::resetAllToDefault()
	{
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
			if (it->second)
				it->second->reset();
	}
	
	vector<sTag> Attribute::Manager::getAttributeNames() const noexcept
	{
		sAttribute attr;
		vector<sTag> names;
		
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			attr = it->second;
			if (!attr->isInvisible())
				names.push_back(attr->getName());
		}
		
		return names;
	}
	
	bool Attribute::Manager::hasKey(sTag name) const noexcept
	{
		return (m_attributes.find(name) != m_attributes.end());
	}
	
	long Attribute::Manager::getNumberOfCategories() const
	{
		return getCategories().size();
	}
	
	vector<string> Attribute::Manager::getCategories() const
	{
		sAttribute attr;
		vector<string> categories;
		string category;
		
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			attr = it->second;
			
			if (!attr->isInvisible())
			{
				category = attr->getCategory();
				
				// add category in categories if it's not allready there :
				if (find(categories.begin(), categories.end(), category) == categories.end())
					categories.push_back(category);
			}
		}
		
		return categories;
	}
	
	void Attribute::Manager::bind(weak_ptr<Attribute::Manager::Listener> listener)
	{
		m_listeners.insert(listener);
	}
	
	void Attribute::Manager::unbind(weak_ptr<Attribute::Manager::Listener> listener)
	{
		m_listeners.erase(listener);
	}
	
	void Attribute::Manager::write(sDico dico) const noexcept
	{
		sAttribute attr;
		ElemVector frozen_attributes_names = {};
		
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			attr = it->second;
			
			if(attr && attr->isSaveable())
			{
				attr->write(dico);
				
				if(attr->isFrozen())
					frozen_attributes_names.push_back(attr->getName());
			}
		}
		
		if(!frozen_attributes_names.empty())
			dico->set(Tag::frozen_attributes, frozen_attributes_names);
	}
	
	void Attribute::Manager::read(scDico dico) noexcept
	{
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
			it->second->read(dico);
			
		if(dico->has(Tag::frozen_attributes))
		{
			ElemVector frozenAttrs;
			dico->get(Tag::frozen_attributes, frozenAttrs);
			
			for(int i=0; i < frozenAttrs.size(); i++)
			{
				if(frozenAttrs[i].isTag())
					getAttribute(frozenAttrs[i])->freeze(true);
			}
		}
	}
	
	/*
	void Attribute::Manager::removeAttributesWithNonCommonKey(sAttributeSet attrset)
	{
		if (attrset)
		{
			vector<sTag> names = getAttributeNames();
			
			for (int i=0; i < names.size(); i++)
				if (!attrset->hasKey(names[i]))
					removeAttribute(names[i]);
		}
	}
	*/
	
	void Attribute::Manager::setAttributeAppearance(sTag name, string const& label, Attribute::Style style, string const& category)
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end())
		{
			it->second->setLabel(label);
			it->second->setStyle(style);
			it->second->setCategory(category);
		}
	}
	
	void Attribute::Manager::setAttributeBehavior(sTag name, Attribute::Behavior behavior)
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end())
		{
			it->second->setBehavior(behavior);
		}
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE BOOL									//
	// ================================================================================ //
	
	AttributeBool::AttributeBool(sTag name,
								 int defaultValue,
								 string const& label,
								 string const& category,
								 long behavior) :
	Attribute(name, {defaultValue}, label, Attribute::Style::Toggle, category, behavior)
	{
		set({defaultValue});
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

    // ================================================================================ //
    //                                 ATTRIBUTE LONG									//
    // ================================================================================ //
    
	AttributeLong::AttributeLong(sTag name,
								 long defaultValue,
								 string const& label,
								 string const& category,
								 long behavior)
	: Attribute(name, {defaultValue}, label, Attribute::Style::Number, category, behavior)
    {
    }
    
    void AttributeLong::set(ElemVector const& elements)
    {
        if(elements.size() && (elements[0].isNumber()))
            m_value = elements[0];
    }
    
    void AttributeLong::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
	
	// ================================================================================ //
	//                                 ATTRIBUTE DOUBLE									//
	// ================================================================================ //
	
	AttributeDouble::AttributeDouble(sTag name,
									 double defaultValue,
									 string const& label,
									 string const& category,
									 Attribute::Style style,
									 long behavior) :
		Attribute(name, {defaultValue}, label, style, category, behavior)
    {
		set({defaultValue});
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

	// ================================================================================ //
	//                                 ATTRIBUTE TAG									//
	// ================================================================================ //
	
	AttributeTag::AttributeTag(sTag name,
							   string defaultValue,
							   string const& label,
							   string const& category,
							   long behavior) :
		Attribute(name, {defaultValue}, label, Attribute::Style::Text, category, behavior)
    {
		set({defaultValue});
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
	
	// ================================================================================ //
	//                                 ATTRIBUTE ENUM									//
	// ================================================================================ //
	
	AttributeEnum::AttributeEnum(sTag name,
								 ElemVector const& enumValues,
								 long defaultValue,
								 string const& label,
								 string const& category,
								 long behavior) :
	Attribute(name, {defaultValue}, label, Attribute::Style::Enum, category, behavior),
	m_enum_values(enumValues)
	{
		set({defaultValue});
	}
	
	void AttributeEnum::set(ElemVector const& elements)
	{
		if(elements.size() && elements[0].isTag())
		{
			if (elements[0].isNumber())
			{
				m_value = clip((long)elements[0], (long)0, (long)m_enum_values.size());
			}
			else if(elements[0].isTag())
			{
				sTag tag = elements[0];
				for (int i=0; i < m_enum_values.size(); i++)
				{
					if (m_enum_values[i] == tag)
					{
						m_value = i;
						return;
					}
				}
			}
		}
	}
	
	void AttributeEnum::get(ElemVector& elements) const noexcept
	{
		elements = {m_value};
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE COLOR									//
	// ================================================================================ //
	
	AttributeColor::AttributeColor(sTag name,
								   ElemVector const& defaultValue,
								   string const& label,
								   string const& category,
								   long behavior)
	: Attribute(name, defaultValue, label, Attribute::Style::Color, category, behavior)
	{
		set(defaultValue);
	}
	
	void AttributeColor::set(ElemVector const& elements)
	{
		const size_t nelems = elements.size();
		for(size_t i = 0; i < 4; i++)
		{
			if (nelems > i && elements[i].isNumber())
				m_value[i] = clip((double)elements[i], 0., 1.);
			else
				m_value[i] = (i < 3) ? 0.f : 1.f; // default is black with full alpha color
		}
	}
	
	void AttributeColor::get(ElemVector& elements) const noexcept
	{
		elements = {m_value[0], m_value[1], m_value[2], m_value[3]};
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE RECT									//
	// ================================================================================ //
	
	AttributeRect::AttributeRect(sTag name,
								 ElemVector const& defaultValue,
								 string const& label,
								 string const& category,
								 long behavior)
	: Attribute(name, defaultValue, label, Attribute::Style::List, category, behavior)
	{
		set(defaultValue);
	}
	
	void AttributeRect::set(ElemVector const& elements)
	{
		const size_t nelems = elements.size();
		
		for(size_t i = 0; i < 4; i++)
		{
			if (nelems > i && elements[i].isNumber())
				m_value[i] = (double)elements[i];
			else
				m_value[i] = 0.f;
		}
	}
	
	void AttributeRect::get(ElemVector& elements) const noexcept
	{
		elements = {m_value[0], m_value[1], m_value[2], m_value[3]};
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE POINT									//
	// ================================================================================ //
	
	AttributePoint::AttributePoint(sTag name,
								 ElemVector const& defaultValue,
								 string const& label,
								 string const& category,
								 long behavior)
	: Attribute(name, defaultValue, label, Attribute::Style::List, category, behavior)
	{
		set(defaultValue);
	}
	
	void AttributePoint::set(ElemVector const& elements)
	{
		const size_t nelems = elements.size();
		
		for(size_t i = 0; i < 2; i++)
		{
			if (nelems > i && elements[i].isNumber())
				m_value[i] = (double)elements[i];
		}
	}
	
	void AttributePoint::get(ElemVector& elements) const noexcept
	{
		elements = {m_value[0], m_value[1]};
	}
}
















