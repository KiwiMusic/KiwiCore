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
    
	Attr::Attr(sTag name, sTag label, sTag category, Style style, ElemVector defaultValues, long behavior) :
    m_name(name),
    m_label(label),
    m_category(category),
    m_style(style),
	m_default_values(defaultValues),
    m_behavior(0 | behavior)
    {
		;
    }
    
    Attr::~Attr()
    {
        m_frozen_values.clear();
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
	}
	
	void Attr::Manager::addAttribute(sAttr attr)
	{
		if(attr)
		{
            lock_guard<mutex> guard(m_attrs_mutex);
			m_attrs[attr->getName()] = attr;
		}
	}
	
	void Attr::Manager::removeAttribute(sAttr attr)
	{
		if(attr)
		{
            lock_guard<mutex> guard(m_attrs_mutex);
            auto it = m_attrs.find(attr->getName());
            if(it != m_attrs.end())
            {
                m_attrs.erase(it);
            }
		}
	}
	
	void Attr::Manager::removeAttribute(sTag name)
	{
        if(name)
        {
            lock_guard<mutex> guard(m_attrs_mutex);
            auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                m_attrs.erase(it);
            }
        }
	}
	
	bool Attr::Manager::setAttributeValue(sTag name, ElemVector const& elements)
	{
        lock_guard<mutex> guard(m_attrs_mutex);
        auto it = m_attrs.find(name);
		if(it != m_attrs.end())
        {
            sAttr attr = it->second;
            if(attr && !attr->isDisabled())
            {
                attr->set(elements);
            }
        }
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
            }
        }
	}
	
	void Attr::Manager::setAttributeBehavior(sTag name, Attr::Behavior behavior)
	{
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setBehavior(behavior);
            }
		}
	}
    
    void Attr::Manager::setAttributeInvisible(sTag name, bool invisible) noexcept
    {
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setInvisible(invisible);
            }
		}
    }
    
    void Attr::Manager::setAttributeDisabled(sTag name, bool disable) noexcept
    {
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setDisabled(disable);
            }
		}
    }
    
    void Attr::Manager::setAttributeSaved(sTag name, bool saved) noexcept
    {
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setSaved(saved);
            }
		}
    }
    
    void Attr::Manager::setAttributeNotifier(sTag name, bool notifier) noexcept
    {
        lock_guard<mutex> guard(m_attrs_mutex);
		auto it = m_attrs.find(name);
		if(it != m_attrs.end())
		{
            sAttr attr = it->second;
            if(attr)
            {
                attr->setNotifier(notifier);
            }
		}
    }
    
    unsigned long Attr::Manager::getNumberOfCategories() const noexcept
    {
        vector<sTag> names;
        getCategoriesNames(names);
        return (unsigned long)names.size();
    }
    
    void Attr::Manager::getCategoriesNames(vector<sTag>& names) const noexcept
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
	
	void Attr::Manager::getCategory(sTag name, vector<sAttr>& attrs) const
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
    
    // ================================================================================ //
	//                                 ATTRIBUTE OTHER									//
	// ================================================================================ //
	
    const sTag AttrFont::Tag_Font               = Tag::create("Font");
    const sTag AttrFont::Tag_fontname                           = Tag::create("fontname");
    const sTag AttrFont::Tag_Font_Name                          = Tag::create("Font Name");
    const sTag AttrFont::Tag_Arial                              = Tag::create("Arial");
    const sTag AttrFont::Tag_fontsize                           = Tag::create("fontsize");
    const sTag AttrFont::Tag_Font_Size                          = Tag::create("Font Size");
    const sTag AttrFont::Tag_fontface                           = Tag::create("fontface");
    const sTag AttrFont::Tag_Font_Face                          = Tag::create("Font Face");
    const sTag AttrFont::Tag_normal                             = Tag::create("normal");
    const sTag AttrFont::Tag_bold                               = Tag::create("bold");
    const sTag AttrFont::Tag_italic                             = Tag::create("italic");
    const sTag AttrFont::Tag_bold_italic                        = Tag::create("bold_italic");
    const sTag AttrFont::Tag_fontjustification                  = Tag::create("fontjustification");
    const sTag AttrFont::Tag_Font_Justification                 = Tag::create("Font Justification");
    const sTag AttrFont::Tag_left                               = Tag::create("left");
    const sTag AttrFont::Tag_center                             = Tag::create("center");
    const sTag AttrFont::Tag_right                              = Tag::create("right");
    
    const sTag AttrAppearance::Tag_Appearance   = Tag::create("Appearance");
    const sTag AttrAppearance::Tag_position                     = Tag::create("position");
    const sTag AttrAppearance::Tag_Position                     = Tag::create("Position");
    const sTag AttrAppearance::Tag_size                         = Tag::create("size");
    const sTag AttrAppearance::Tag_Size                         = Tag::create("Size");
    const sTag AttrAppearance::Tag_presentation_position        = Tag::create("presentation position");
    const sTag AttrAppearance::Tag_Presentation_Position        = Tag::create("Presentation Position");
    const sTag AttrAppearance::Tag_presentation_size            = Tag::create("presentation size");
    const sTag AttrAppearance::Tag_Presentation_Size            = Tag::create("Presentation Size");
    const sTag AttrAppearance::Tag_hidden                       = Tag::create("hidden");
    const sTag AttrAppearance::Tag_Hide_on_Lock                 = Tag::create("Hide on Lock");
    const sTag AttrAppearance::Tag_presentation                 = Tag::create("presentation");
    const sTag AttrAppearance::Tag_Include_in_Presentation      = Tag::create("Include in Presentation");
}
















