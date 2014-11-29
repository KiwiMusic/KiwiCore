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
    const sTag Attr::frozen_attributes  = Tag::create("frozen_attributes");
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
	Attr::Attr(sTag name, sTag label, sTag category, Style style, ElemVector defaultValues, long behavior, long order) :
    m_name(name),
    m_label(label),
    m_category(category),
    m_style(style),
	m_order(order),
	m_default_values(defaultValues),
    m_behavior(0 | behavior)
    {
		;
    }
    
    Attr::~Attr()
    {
        m_frozen_values.clear();
    }
    
    void Attr::setDefaultValues(ElemVector const& elements)
    {
        m_default_values = elements;
        set(m_default_values);
    }
	
	void Attr::setBehavior(long behavior) noexcept
	{
		if(m_behavior != behavior)
		{
			m_behavior = behavior;
		}
	}
	
	void Attr::setInvisible(bool b) noexcept
	{
		if (b)
			m_behavior &= ~Invisible;
		else
			m_behavior |= Invisible;
	}
	
	void Attr::setDisabled(bool b) noexcept
	{
		if (!b)
			m_behavior &= ~Disabled;
		else
			m_behavior |= Disabled;
	}

	void Attr::setSaved(bool b) noexcept
	{
		if (!b)
			m_behavior &= ~Unsaved;
		else
			m_behavior |= Unsaved;
	}
	
	void Attr::setNotifier(bool b) noexcept
	{
		if (!b)
			m_behavior &= ~Notifier;
		else
			m_behavior |= Notifier;
	}
	
	void Attr::freeze(bool frozen)
	{
		if(frozen)
        {
			get(m_frozen_values);
        }
		else
        {
			m_frozen_values.clear();
        }
	}
	
	void Attr::setDefaultValues()
	{
        if(!m_default_values.empty())
        {
            set(m_default_values);
        }
	}
	
    void Attr::setFrozenValues()
    {
        if(!m_frozen_values.empty())
        {
            set(m_frozen_values);
        }
    }
    
    void Attr::write(sDico dico) const noexcept
    {
		if(!(m_behavior & Behavior::Unsaved) || !m_frozen_values.empty())
		{
			if(m_frozen_values.empty() && !(m_behavior & Behavior::Unsaved))
            {
                ElemVector elements;
				get(elements);
                if(elements.size() == m_default_values.size())
                {
                    for(ElemVector::size_type i = 0; i < elements.size(); i++)
                    {
                        if(elements[i] != m_default_values[i])
                        {
                            dico->set(m_name, elements);
                            return;
                        }
                    }
                }
                else
                {
                    dico->set(m_name, elements);
                }
            }
			else
            {
                dico->set(m_name, m_frozen_values);
                dico->append(Attr::frozen_attributes, m_name);
            }
		}
    }
    
    void Attr::read(scDico dico)
    {
        ElemVector elements;
        dico->get(m_name, elements);
        set(elements);
        dico->get(Attr::frozen_attributes, elements);
        if(find(elements.begin(), elements.end(), m_name) != elements.end())
        {
            freeze(true);
        }
    }
	
	void Attr::sort(vector<sAttr>& attrs)
	{
		struct less_than_compare
		{
			inline bool operator() (const sAttr& attr1, const sAttr& attr2)
			{
				if(attr1->getCategory() == attr2->getCategory())
				{
					// defined order in a same category sorts attrs by order
					if(attr1->getOrder() > 0 && attr2->getOrder() > 0)
					{
						return (attr1->getOrder() < attr2->getOrder());
					}
				}
				
				// sort attribute by label
				return (attr1->getLabel()->getName() < attr2->getLabel()->getName());
			}
		};
		
		std::sort(attrs.begin(), attrs.end(), less_than_compare());
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTES MANAGER								//
	// ================================================================================ //
	
	
	Attr::Manager::Manager() noexcept
	{
		;
	}
	
	Attr::Manager::~Manager()
	{
		m_attrs.clear();
		m_listeners.clear();
	}
	
	void Attr::Manager::addAttribute(sAttr attr)
	{
		if(attr)
		{
			m_attrs_mutex.lock();
			m_attrs[attr->getName()] = attr;
			m_attrs_mutex.unlock();
			
			sendNotification(attr, Notification::AttrAdded);
		}
	}
	
	void Attr::Manager::removeAttribute(sAttr attr)
	{
		if(attr)
		{
            m_attrs_mutex.lock();
            auto it = m_attrs.find(attr->getName());
            if(it != m_attrs.end())
            {
                m_attrs.erase(it);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::AttrRemoved);
            }
		}
	}
	
	void Attr::Manager::removeAttribute(sTag name)
	{
        if(name)
        {
            m_attrs_mutex.lock();
            auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                m_attrs.erase(it);
				m_attrs_mutex.unlock();
				sendNotification(it->second, Notification::AttrRemoved);
            }
        }
	}
	
    void Attr::Manager::setAttributeDefaultValues(sTag name, ElemVector const& elements)
    {
        m_attrs_mutex.lock();
        auto it = m_attrs.find(name);
		if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr && !attr->isDisabled())
            {
                attr->setDefaultValues(elements);
				m_attrs_mutex.unlock();
				
				if(attributeValueChanged(attr))
				{
					sendNotification(attr, Notification::ValueChanged);
				}
            }
        }
		m_attrs_mutex.unlock();
    }
    
	bool Attr::Manager::setAttributeValue(sTag name, ElemVector const& elements)
	{
		m_attrs_mutex.lock();
        auto it = m_attrs.find(name);
		if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr && !attr->isDisabled())
            {
                attr->set(elements);
				m_attrs_mutex.unlock();
				
				if(attributeValueChanged(attr))
				{
					sendNotification(attr, Notification::ValueChanged);
				}
				return true;
            }
        }
		m_attrs_mutex.unlock();
		return false;
	}
	
	bool Attr::Manager::getAttributeValue(sTag name, ElemVector& elements)
	{
        elements.clear();
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr)
            {
                attr->get(elements);
                return true;
            }
        }
		return false;
	}
    
    unsigned long Attr::Manager::getNumberOfAttributes() const noexcept
    {
        unsigned long size = 0;
        lock_guard<mutex> guard(m_attrs_mutex);
        for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
        {
            sAttr attr = it->second;
            if(attr && !attr->isInvisible())
            {
                size++;
            }
        }
        return size;
    }
    
    void Attr::Manager::getAttributeNames(vector<sTag>& names) const noexcept
	{
        names.clear();
        lock_guard<mutex> guard(m_attrs_mutex);
		for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
		{
            sAttr attr = it->second;
            if(attr && !attr->isInvisible())
            {
                names.push_back(attr->getName());
            }
		}
	}
	
	bool Attr::Manager::hasAttribute(sTag name) const noexcept
	{
        lock_guard<mutex> guard(m_attrs_mutex);
        auto it = m_attrs.find(name);
        if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr && !attr->isInvisible())
            {
                return true;
            }
        }
		return false;
	}
	
	sAttr Attr::Manager::getAttribute(sTag name) const noexcept
	{
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr && !attr->isInvisible())
            {
                return attr;
            }
			return nullptr;
        }
		else
        {
			return nullptr;
        }
	}
	
	void Attr::Manager::write(sDico dico) const noexcept
	{
        lock_guard<mutex> guard(m_attrs_mutex);
		for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->write(dico);
            }
		}
	}
	
	void Attr::Manager::read(scDico dico) noexcept
	{
        lock_guard<mutex> guard(m_attrs_mutex);
		for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
        {
            sAttr attr = it->second;
            if(attr)
            {
                attr->read(dico);
                if(attributeValueChanged(attr))
                {
                    sendNotification(attr, Notification::ValueChanged);
                }
            }
        }
	}
	
	void Attr::Manager::setAttributeBehavior(sTag name, Attr::Behavior behavior)
	{
        m_attrs_mutex.lock();
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setBehavior(behavior);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::BehaviorChanged);
            }
		}
	}
    
    void Attr::Manager::setAttributeInvisible(sTag name, bool invisible) noexcept
    {
        m_attrs_mutex.lock();
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setInvisible(invisible);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::BehaviorChanged);
            }
		}
    }
    
    void Attr::Manager::setAttributeDisabled(sTag name, bool disable) noexcept
    {
        m_attrs_mutex.lock();
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setDisabled(disable);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::BehaviorChanged);
            }
		}
    }
    
    void Attr::Manager::setAttributeSaved(sTag name, bool saved) noexcept
    {
        m_attrs_mutex.lock();
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setSaved(saved);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::BehaviorChanged);
            }
		}
    }
    
    void Attr::Manager::setAttributeNotifier(sTag name, bool notifier) noexcept
    {
        m_attrs_mutex.lock();
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setNotifier(notifier);
				m_attrs_mutex.unlock();
				sendNotification(attr, Notification::BehaviorChanged);
            }
		}
    }
    
    unsigned long Attr::Manager::getNumberOfCategories() const noexcept
    {
        vector<sTag> names;
        getCategoriesNames(names);
        return (unsigned long)names.size();
    }
	
    void Attr::Manager::getCategoriesNames(vector<sTag>& names, bool sortAlphabetically) const noexcept
    {
        names.clear();
        lock_guard<mutex> guard(m_attrs_mutex);
		for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
		{
            sAttr attr = it->second;
            if(attr && !attr->isInvisible())
            {
				if(find(names.begin(), names.end(), attr->getCategory()) == names.end())
                {
					names.push_back(attr->getCategory());
                }
            }
		}
		
		if(sortAlphabetically)
			Tag::sort(names);
    }
    
    bool Attr::Manager::hasCategory(sTag name) const noexcept
    {
        lock_guard<mutex> guard(m_attrs_mutex);
        for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
		{
            sAttr attr = it->second;
            if(attr && !attr->isInvisible() && attr->getCategory() == name)
            {
                return true;
            }
        }
        return false;
    }
	
	void Attr::Manager::getAttributesInCategory(sTag name, vector<sAttr>& attrs, bool sorted) const
	{
        attrs.clear();
        lock_guard<mutex> guard(m_attrs_mutex);
        for(auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
		{
            sAttr attr = it->second;
            if(attr && !attr->isInvisible() && attr->getCategory() == name)
            {
                attrs.push_back(attr);
            }
		}
		
		if (sorted)
			Attr::sort(attrs);
	}
	
	void Attr::Manager::bind(shared_ptr<Attr::Manager::Listener> listener)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
			m_listeners.insert(listener);
		}
	}
	
	void Attr::Manager::unbind(shared_ptr<Attr::Manager::Listener> listener)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
			m_listeners.erase(listener);
		}
	}
	
	//! @internal Trigger notification to subclasses and listeners.
	void Attr::Manager::sendNotification(sAttr attr, Notification type)
	{
		lock_guard<mutex> guard(m_listeners_mutex);
		auto it = m_listeners.begin();
		while(it != m_listeners.end())
		{
			if((*it).expired())
			{
				it = m_listeners.erase(it);
			}
			else
			{
				Manager::sListener listener = (*it).lock();
				listener->attributeNotify(shared_from_this(), attr, type);
				++it;
			}
		}
	}
	
    // ================================================================================ //
    //                                 ATTRIBUTE BOOL									//
    // ================================================================================ //
    
    void AttrBool::get(ElemVector& elements) const noexcept
    {
        elements = {(long)m_value};
    }
    
    void AttrBool::set(ElemVector const& elements)
    {
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value = (bool)((long)elements[0]);
        }
    }

    // ================================================================================ //
    //                                 ATTRIBUTE LONG									//
    // ================================================================================ //
    
    void AttrLong::set(ElemVector const& elements)
    {
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value = elements[0];
        }
    }
    
    void AttrLong::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
	
	// ================================================================================ //
	//                                 ATTRIBUTE DOUBLE									//
	// ================================================================================ //
	
    void AttrDouble::set(ElemVector const& elements)
    {
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value = elements[0];
        }
    }
    
    void AttrDouble::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }

	// ================================================================================ //
	//                                 ATTRIBUTE TAG									//
	// ================================================================================ //
    
    void AttrTag::set(ElemVector const& elements)
    {
        if(!elements.empty() && elements[0].isTag())
        {
            m_value = elements[0];
        }
    }
    
    void AttrTag::get(ElemVector& elements) const noexcept
    {
        elements = {m_value};
    }
	
	// ================================================================================ //
	//                                 ATTRIBUTE ENUM									//
	// ================================================================================ //
	
	void AttrEnum::set(ElemVector const& elements)
	{
		if(!elements.empty())
		{
			if(elements[0].isNumber())
			{
				m_value = clip((ElemVector::size_type)elements[0], (ElemVector::size_type)0, m_enum_values.size()-1);
			}
			else if(elements[0].isTag())
			{
				sTag tag = elements[0];
                m_value = clip(find_position(m_enum_values, tag), (ElemVector::size_type)0, m_enum_values.size()-1);
			}
		}
	}
	
	void AttrEnum::get(ElemVector& elements) const noexcept
	{
		elements = {(long)m_value};
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE COLOR									//
	// ================================================================================ //
	
	void AttrColor::set(ElemVector const& elements)
	{
        m_value = elements;
	}
	
	void AttrColor::get(ElemVector& elements) const noexcept
	{
		elements = m_value;
	}
	
    // ================================================================================ //
	//                                 ATTRIBUTE POINT									//
	// ================================================================================ //
	
	void AttrPoint::set(ElemVector const& elements)
	{
        m_value = elements;
	}
	
	void AttrPoint::get(ElemVector& elements) const noexcept
	{
		elements = m_value;
	}

    
	// ================================================================================ //
	//                                 ATTRIBUTE RECT									//
	// ================================================================================ //
	
	void AttrRect::set(ElemVector const& elements)
	{
        m_value = elements;
	}
	
	void AttrRect::get(ElemVector& elements) const noexcept
	{
		elements = m_value;
	}
}
















