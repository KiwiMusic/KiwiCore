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
        if(!elements.empty())
        {
            int zaza; 
            set(elements);
        }
        dico->get(Attr::frozen_attributes, elements);
        if(find(elements.begin(), elements.end(), m_name) != elements.end())
        {
            freeze(true);
        }
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
			send(attr, Notification::Added);
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
				send(attr, Notification::Removed);
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
				send(it->second, Notification::Removed);
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
				
				if(attributeChanged(attr))
				{
					send(attr, Notification::ValueChanged);
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
				
				if(attributeChanged(attr))
				{
					send(attr, Notification::ValueChanged);
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
    
    ulong Attr::Manager::getNumberOfAttributes() const noexcept
    {
        ulong size = 0;
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
            if(sAttr attr = it->second)
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
            if(sAttr attr = it->second)
            {
                attr->read(dico);
                if(attributeChanged(attr))
                {
                    send(attr, Notification::ValueChanged);
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
				send(attr, Notification::BehaviorChanged);
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
				send(attr, Notification::BehaviorChanged);
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
				send(attr, Notification::BehaviorChanged);
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
				send(attr, Notification::BehaviorChanged);
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
				send(attr, Notification::BehaviorChanged);
            }
		}
    }
    
    ulong Attr::Manager::getNumberOfCategories() const noexcept
    {
        vector<sTag> names;
        getCategoriesNames(names);
        return (ulong)names.size();
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
        {
            sort(names.begin(), names.end());
        }
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
        {
            sort(attrs.begin(), attrs.end(), compareOrderOrLabel);
        }
	}
	
	void Attr::Manager::bind(sListener listener, sTag name, Notification type)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
            auto it = m_listeners.find(listener);
            if(it != m_listeners.end())
            {
                auto it2 = it->second.attrs.find(name);
                if(it2 != it->second.attrs.end())
                {
                    it2->second |= type;
                }
                else
                {
                    it->second.attrs[name] = 0 | type;
                }
                if(name == nullptr)
                {
                    for(auto it2 = it->second.attrs.begin(); it2 != it->second.attrs.end(); ++it)
                    {
                        it2->second &= ~type;
                        if(!it2->second)
                        {
                            it->second.attrs.erase(it2);
                        }
                    }
                }
            }
            else
            {
                m_listeners.insert(pair<wListener, ListenerAttrList>(listener, ListenerAttrList(name, 0 | type)));
            }
        }
	}
	
	void Attr::Manager::unbind(sListener listener, sTag name, Notification type)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
			auto it = m_listeners.find(listener);
            if(it != m_listeners.end())
            {
                if(name == nullptr && type == Anything)
                {
                     m_listeners.erase(listener);
                }
                else if(name == nullptr && type != Anything)
                {
                    for(auto it2 = it->second.attrs.begin(); it2 != it->second.attrs.end(); ++it)
                    {
                        it2->second &= ~type;
                        if(!it2->second)
                        {
                            it->second.attrs.erase(it2);
                        }
                    }
                    if(it->second.attrs.empty())
                    {
                        m_listeners.erase(listener);
                    }
                }
                else
                {
                    auto it2 = it->second.attrs.find(name);
                    if(it2 != it->second.attrs.end())
                    {
                        it2->second &= ~type;
                        if(!it2->second)
                        {
                            it->second.attrs.erase(it2);
                        }
                    }
                    if(it->second.attrs.empty())
                    {
                        m_listeners.erase(listener);
                    }
                }
            }
		}
	}
	
	void Attr::Manager::send(sAttr attr, Notification type)
	{
		lock_guard<mutex> guard(m_listeners_mutex);
     
		auto it = m_listeners.begin();
		while(it != m_listeners.end())
		{
			if(it->first.expired())
			{
				it = m_listeners.erase(it);
			}
			else
			{
                sListener listener = it->first.lock();
                if(listener)
                {
                    auto it2 = it->second.attrs.find(attr->getName());
                    if(it2 != it->second.attrs.end())
                    {
                        if(it2->second & type)
                        {
                            listener->notify(shared_from_this(), attr, type);
                        }
                    }
                    else if((it2 = it->second.attrs.find(nullptr)) != it->second.attrs.end())
                    {
                        if(it2->second & type)
                        {
                            listener->notify(shared_from_this(), attr, type);
                        }
                    }
                }
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
        else
        {
            m_value = Tag::create("");
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
                for(ElemVector::size_type i = 0; i < m_enum_values.size(); i++)
                {
                    if(m_enum_values[i] == tag)
                    {
                        m_value = i;
                    }
                }
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
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value.red(elements[0]);
        }
        if(elements.size() > 1 && elements[1].isNumber())
        {
            m_value.green(elements[1]);
        }
        if(elements.size() > 2 && elements[2].isNumber())
        {
            m_value.blue(elements[2]);
        }
        if(elements.size() > 3 && elements[3].isNumber())
        {
            m_value.alpha(elements[3]);
        }
	}
	
	void AttrColor::get(ElemVector& elements) const noexcept
	{
		elements = {m_value.red(), m_value.green(), m_value.blue(), m_value.alpha()};
	}
	
    // ================================================================================ //
	//                                 ATTRIBUTE POINT									//
	// ================================================================================ //
	
	void AttrPoint::set(ElemVector const& elements)
	{
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value.x(elements[0]);
        }
        if(elements.size() > 1 && elements[1].isNumber())
        {
            m_value.y(elements[1]);
        }
	}
	
	void AttrPoint::get(ElemVector& elements) const noexcept
	{
        elements = {m_value.x(), m_value.y()};
	}
	
	// ================================================================================ //
	//                                 ATTRIBUTE SIZE									//
	// ================================================================================ //
	
	void AttrSize::set(ElemVector const& elements)
	{
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value.x(elements[0]);
        }
        if(elements.size() > 1 && elements[1].isNumber())
        {
            m_value.y(elements[1]);
        }
		clipValue();
	}
	
	void AttrSize::get(ElemVector& elements) const noexcept
	{
        elements = {m_value.x(), m_value.y()};
	}

	
	// ================================================================================ //
	//                                 ATTRIBUTE RECT									//
	// ================================================================================ //
	
	void AttrRect::set(ElemVector const& elements)
	{
        if(!elements.empty() && elements[0].isNumber())
        {
            m_value.x(elements[0]);
        }
        if(elements.size() > 1 && elements[1].isNumber())
        {
            m_value.y(elements[1]);
        }
        if(elements.size() > 2 && elements[2].isNumber())
        {
            m_value.width(elements[2]);
        }
        if(elements.size() > 3 && elements[3].isNumber())
        {
            m_value.height(elements[3]);
        }
	}
	
	void AttrRect::get(ElemVector& elements) const noexcept
	{
        elements = {m_value.x(), m_value.y(), m_value.width(), m_value.height()};
	}
    
    // ================================================================================ //
    //                                  ATTRIBUTE PAGE                                  //
    // ================================================================================ //
    
    const sTag AttrPage::Tag_editing_bgcolor                    = Tag::create("editing_bgcolor");
    const sTag AttrPage::Tag_Unlocked_Background_Color          = Tag::create("Unlocked Background Color");
    const sTag AttrPage::Tag_locked_bgcolor						= Tag::create("locked_bgcolor");
    const sTag AttrPage::Tag_Locked_Background_Color			= Tag::create("Locked Background Color");
    
    const sTag AttrPage::Tag_Editing							= Tag::create("Editing");
    const sTag AttrPage::Tag_gridsize							= Tag::create("gridsize");
    const sTag AttrPage::Tag_Grid_Size							= Tag::create("Grid Size");
    
    // ================================================================================ //
    //                                  ATTRIBUTE BOX                                   //
    // ================================================================================ //
    
    const sTag AttrBox::Tag_ninlets							= Tag::create("ninlets");
    const sTag AttrBox::Tag_noutlets						= Tag::create("noutlets");
    const sTag AttrBox::Tag_id                              = Tag::create("id");
    const sTag AttrBox::Tag_text                            = Tag::create("text");
    
    const sTag AttrBox::Tag_Appearance						= Tag::create("Appearance");
    const sTag AttrBox::Tag_position						= Tag::create("position");
    const sTag AttrBox::Tag_Position						= Tag::create("Position");
    const sTag AttrBox::Tag_size							= Tag::create("size");
    const sTag AttrBox::Tag_Size							= Tag::create("Size");
    const sTag AttrBox::Tag_presentation_position			= Tag::create("presentation_position");
    const sTag AttrBox::Tag_Presentation_Position			= Tag::create("Presentation Position");
    const sTag AttrBox::Tag_presentation_size				= Tag::create("presentation_size");
    const sTag AttrBox::Tag_Presentation_Size				= Tag::create("Presentation Size");
    const sTag AttrBox::Tag_hidden							= Tag::create("hidden");
    const sTag AttrBox::Tag_Hide_on_Lock					= Tag::create("Hide on Lock");
    const sTag AttrBox::Tag_presentation					= Tag::create("presentation");
    const sTag AttrBox::Tag_Include_in_Presentation			= Tag::create("Include in Presentation");
    
    const sTag AttrBox::Tag_Behavior						= Tag::create("Behavior");
    const sTag AttrBox::Tag_ignoreclick						= Tag::create("ignoreclick");
    const sTag AttrBox::Tag_Ignore_Click					= Tag::create("Ignore Click");
    
    const sTag AttrBox::Tag_bgcolor							= Tag::create("bgcolor");
    const sTag AttrBox::Tag_Background_Color				= Tag::create("Background Color");
    const sTag AttrBox::Tag_bdcolor							= Tag::create("bdcolor");
    const sTag AttrBox::Tag_Border_Color					= Tag::create("Border Color");
    const sTag AttrBox::Tag_textcolor						= Tag::create("textcolor");
    const sTag AttrBox::Tag_Text_Color						= Tag::create("Text Color");
}
















