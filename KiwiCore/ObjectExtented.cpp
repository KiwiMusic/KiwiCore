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

#include "ObjectExtented.h"

namespace Kiwi
{
    ObjectExtented::ObjectExtented(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name) : Object(kiwi, name)
    {
        m_current_attribute = m_attributes.end();
    }
    
    ObjectExtented::ObjectExtented(shared_ptr<Instance> kiwi, const string name) : Object(kiwi, name)
    {
        m_current_attribute = m_attributes.end();
    }
    
    ObjectExtented::~ObjectExtented()
    {
        m_attributes.clear();
        m_listeners.clear();
    };
    
    void ObjectExtented::write(shared_ptr<Dico> dico)
    {
        for(map<shared_ptr<Tag>, Attribute>::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            it->second.write(dico);
        }
        Object::write(dico);
    }
    
    bool ObjectExtented::getAttribute(const shared_ptr<Tag> name)
    {
        m_current_attribute = m_attributes.find(name);
        if(m_current_attribute != m_attributes.end())
            return true;
        else
        {
            string message = (string)*getName() + string(" : The attribute ") + (string)*name + string(" doesn't exists !");
            error(message);
            return false;
        }
    }
    
    bool ObjectExtented::currentAttributeValid()
    {
        if(m_current_attribute != m_attributes.end())
            return true;
        else
        {
            string message = (string)*getName() + string(" : The attribute used isn't valid !");
            error(message);
            return false;
        }
    }
    
    void ObjectExtented::createAttribute(const shared_ptr<Tag> name, Type type, int size)
    {
        if(m_attributes.find(name) != m_attributes.end())
        {
            string message = (string)*getName() + string(" : The attribute ") + (string)*name + string(" already exists !");
            error(message);
            return;
        }
        
        Attribute newAttr = Attribute(name, type, size);
        newAttr.m_category = Object::getName();
        m_attributes.insert(make_pair(name, newAttr));
        m_current_attribute = m_attributes.find(name);
        addMethod(name, T_ELEMENTS, nullptr); // TODO
    }
    
    void ObjectExtented::setAttributeSize(const shared_ptr<Tag> name, int size)
    {
        if(getAttribute(name))
            m_current_attribute->second.setSize(size);
    }
    
    void ObjectExtented::setAttributeSize(int size)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setSize(size);
    }
    
    void ObjectExtented::setAttributeLabel(const shared_ptr<Tag> name, const shared_ptr<Tag> label)
    {
        if(getAttribute(name))
            m_current_attribute->second.setLabel(label);
    }
    
    void ObjectExtented::setAttributeLabel(const shared_ptr<Tag> label)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setLabel(label);
    }
    
    void ObjectExtented::setAttributeStyle(const shared_ptr<Tag> name, const shared_ptr<Tag> style)
    {
        if(getAttribute(name))
            m_current_attribute->second.setStyle(style);
    }
    
    void ObjectExtented::setAttributeStyle(const shared_ptr<Tag> style)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setStyle(style);
    }
    
    void ObjectExtented::setAttributeCategory(const shared_ptr<Tag> name, const shared_ptr<Tag> category)
    {
        if(getAttribute(name))
            m_current_attribute->second.setCategory(category);
    }
    
    void ObjectExtented::setAttributeCategory(const shared_ptr<Tag> category)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setCategory(category);
    }
    
    
    void ObjectExtented::setAttributeAppearance(const shared_ptr<Tag> name, const shared_ptr<Tag> label, const shared_ptr<Tag> style, const shared_ptr<Tag> category)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setLabel(label);
            m_current_attribute->second.setStyle(style);
            m_current_attribute->second.setCategory(category);
        }
    }
    
    void ObjectExtented::setAttributeAppearance(const shared_ptr<Tag> label, const shared_ptr<Tag> style, const shared_ptr<Tag> category)
    {
        if(currentAttributeValid())
        {
            m_current_attribute->second.setLabel(label);
            m_current_attribute->second.setStyle(style);
            m_current_attribute->second.setCategory(category);
        }
    }
    
    void ObjectExtented::setAttributeOpaque(const shared_ptr<Tag> name, bool status)
    {
        if(getAttribute(name))
            m_current_attribute->second.setOpaque(status);
    }
    
    void ObjectExtented::setAttributeOpaque(bool status)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setOpaque(status);
    }
    
    void ObjectExtented::setAttributeVisible(const shared_ptr<Tag> name, bool status)
    {
        if(getAttribute(name))
            m_current_attribute->second.setVisible(status);
    }
    
    void ObjectExtented::setAttributeVisible(bool status)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setVisible(status);
    }
    
    void ObjectExtented::setAttributeSave(const shared_ptr<Tag> name, bool status)
    {
        if(getAttribute(name))
            m_current_attribute->second.setSave(status);
    }
    
    void ObjectExtented::setAttributeSave(bool status)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setSave(status);
    }
    
    void ObjectExtented::setAttributeBehavior(const shared_ptr<Tag> name, bool opaque, bool visible, bool save)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setOpaque(opaque);
            m_current_attribute->second.setVisible(visible);
            m_current_attribute->second.setSave(save);
        }
    }
    
    void ObjectExtented::setAttributeBehavior(bool opaque, bool visible, bool save)
    {
        if(currentAttributeValid())
        {
            m_current_attribute->second.setOpaque(opaque);
            m_current_attribute->second.setVisible(visible);
            m_current_attribute->second.setSave(save);
        }
    }
    
    void ObjectExtented::setAttributeClipMin(const shared_ptr<Tag> name, string min)
    {
        if(getAttribute(name))
            m_current_attribute->second.setMinimum(min);
    }
    
    void ObjectExtented::setAttributeClipMin(string min)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setMinimum(min);
    }
    
    void ObjectExtented::setAttributeClipMax(const shared_ptr<Tag> name, string max)
    {
        if(getAttribute(name))
            m_current_attribute->second.setMaximum(max);
    }
    
    void ObjectExtented::setAttributeClipMax(string max)
    {
        if(currentAttributeValid())
            m_current_attribute->second.setMaximum(max);
    }
    
    void ObjectExtented::setAttributeClip(const shared_ptr<Tag> name, string min, string max)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setMinimum(min);
            m_current_attribute->second.setMaximum(max);
        }
    }
    
    void ObjectExtented::setAttributeClip(string min, string max)
    {
        if(currentAttributeValid())
        {
            m_current_attribute->second.setMinimum(min);
            m_current_attribute->second.setMaximum(max);
        }
    }
    
    void ObjectExtented::setAttributeMethods(const shared_ptr<Tag> name, Method setter, Method getter)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setSetter((MethodElements)setter);
            m_current_attribute->second.setGetter((MethodElements)getter);
        }
    }
    void ObjectExtented::setAttributeMethods(Method setter, Method getter)
    {
        if(currentAttributeValid())
        {
            m_current_attribute->second.setSetter((MethodElements)setter);
            m_current_attribute->second.setGetter((MethodElements)getter);
        }
    }
    
    void ObjectExtented::setAttributeDefaultValues(const shared_ptr<Tag> name, const vector<Element>& elements)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setDefaultValues(elements);
        }
    }
    
    void ObjectExtented::setAttributeDefaultValues(const vector<Element>& elements)
    {
        if(currentAttributeValid())
        {
            m_current_attribute->second.setDefaultValues(elements);
        }
    }
    
    void ObjectExtented::setAttributeDefaultValue(const shared_ptr<Tag> name, Element value)
    {
        if(getAttribute(name))
        {
            vector<Element> elements;
            elements.push_back(value);
            m_current_attribute->second.setDefaultValues(elements);
        }
    }
    
    void ObjectExtented::setAttributeDefaultValue(Element value)
    {
        if(currentAttributeValid())
        {
            vector<Element> elements;
            elements.push_back(value);
            m_current_attribute->second.setDefaultValues(elements);
        }
    }

    void ObjectExtented::setAttributeValues(const shared_ptr<Tag> name, const vector<Element>& elements)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.setValues(elements);
        }
    }
    
    Element ObjectExtented::getAttributeValue(const shared_ptr<Tag> name)
    {
        if(getAttribute(name))
        {
            return m_current_attribute->second.getValue();
        }
        return 0;
    }
    
    void ObjectExtented::getAttributeValues(const shared_ptr<Tag> name, vector<Element>& elements)
    {
        if(getAttribute(name))
        {
            m_current_attribute->second.getValues(elements);
        }
    }
}
















