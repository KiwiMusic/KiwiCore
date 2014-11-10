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

#include "AttributeSet.h"

//==============================================================================
namespace Kiwi
{
	AttributeSet::AttributeSet(const string& name) noexcept : m_name(name)
	{
		;
	}
	
	AttributeSet::~AttributeSet()
	{
		m_attributes.clear();
		m_listeners.clear();
	}
	
	sAttribute AttributeSet::getAttribute(sTag name) const noexcept
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end() && !(it->second->isInvisible()))
			return it->second;
		else
			return nullptr;
	}
	
	void AttributeSet::addAttribute(sAttribute attr)
	{
		if (attr != nullptr)
		{
			sTag attrname = attr->name();
			
			m_attributes[attrname] = attr;
			
			for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
				if (!it->expired())
					it->lock()->attributeAdded(attr);
		}
	}
	
	void AttributeSet::addAttributeSet(shared_ptr<AttributeSet> attrset)
	{
		if (attrset)
		{
			vector<sTag> names = attrset->getAttributeNames();
			
			for (int i=0; i < names.size(); i++)
				addAttribute(attrset->getAttribute(names[i]));
		}
	}
	
	void AttributeSet::removeAttribute(sAttribute attr)
	{
		if (attr != nullptr)
		{
			sTag attrname = attr->name();
			m_attributes.erase(attrname);
		}
	}
	
	void AttributeSet::removeAttribute(sTag name)
	{
		removeAttribute(getAttribute(name));
	}
	
	bool AttributeSet::setValue(sTag name, ElemVector const& elements)
	{
		sAttribute attr = getAttribute(name);
		
		if (attr != nullptr && !attr->isOpaque())
		{
			attr->set(elements);
			
			if(attr->shouldNotifyChanges())
				notify(attr);
			
			return true;
		}
		
		return false;
	}
	
	bool AttributeSet::getValue(sTag name, ElemVector& elements, ElemVector const& defaultValue)
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
	
	void AttributeSet::resetAllToDefault()
	{
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
			if (it->second)
				it->second->reset();
	}
	
	vector<sTag> AttributeSet::getAttributeNames() const noexcept
	{
		sAttribute attr;
		vector<sTag> names;
		
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			attr = it->second;
			if (!attr->isInvisible())
				names.push_back(attr->name());
		}
		
		return names;
	}
	
	bool AttributeSet::hasKey(sTag name) const noexcept
	{
		return (m_attributes.find(name) != m_attributes.end());
	}
	
	long AttributeSet::getNumberOfCategories() const
	{
		return getCategories().size();
	}
	
	vector<string> AttributeSet::getCategories() const
	{
		sAttribute attr;
		vector<string> categories;
		string category;
		
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
		{
			attr = it->second;
			
			if (!attr->isInvisible())
			{
				category = attr->category();
				
				// add category in categories if it's not allready there :
				if (find(categories.begin(), categories.end(), category) == categories.end())
					categories.push_back(category);
			}
		}
		
		return categories;
	}
	
	void AttributeSet::bind(weak_ptr<AttributeSet::Listener> listener)
	{
		m_listeners.insert(listener);
	}
	
	void AttributeSet::unbind(weak_ptr<AttributeSet::Listener> listener)
	{
		m_listeners.erase(listener);
	}
	
	void AttributeSet::write(shared_ptr<Dico> dico) const noexcept
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
					frozen_attributes_names.push_back(attr->name());
			}
		}
		
		if(!frozen_attributes_names.empty())
			dico->set(Tag::frozen_attributes, frozen_attributes_names);
	}
	
	void AttributeSet::read(shared_ptr<const Dico> dico) noexcept
	{
		for(auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
			it->second->read(dico);
		
		if(dico->has(Tag::frozen_attributes));
		{
			ElemVector frozenAttrs;
			dico->get(Tag::frozen_attributes, frozenAttrs);
			for(int i=0; i < frozenAttrs.size(); i++)
				if(frozenAttrs[i].isTag())
					getAttribute(frozenAttrs[i])->freeze(true);
		}
	}
	
	void AttributeSet::removeAttributesWithNonCommonKey(shared_ptr<AttributeSet> attrset)
	{
		if (attrset)
		{
			vector<sTag> names = getAttributeNames();
			
			for (int i=0; i < names.size(); i++)
				if (!attrset->hasKey(names[i]))
					removeAttribute(names[i]);
		}
	}
	
	void AttributeSet::setAttributeAppearance(sTag name, string const& label, Attribute::Style style, string const& category)
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end())
		{
			it->second->setLabel(label);
			it->second->setStyle(style);
			it->second->setCategory(category);
		}
	}
	
	void AttributeSet::setAttributeBehavior(sTag name, Attribute::Behavior behavior)
	{
		auto it = m_attributes.find(name);
		if(it != m_attributes.end())
		{
			it->second->setBehavior(behavior);
		}
	}
}
