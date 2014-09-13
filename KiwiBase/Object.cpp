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

#include "Object.h"
#include "Instance.h"

namespace Kiwi
{
    Object::Object(shared_ptr<Instance> kiwi, string const& name) :
    m_kiwi(kiwi),
    m_name(kiwi->createTag(name)),
    m_stack_count(0),
    
    m_method_create({T_NOTHING, nullptr}),
    m_method_bang(nullptr),
    m_method_long(nullptr),
    m_method_double(nullptr),
    m_method_tag(nullptr),
    m_method_object(nullptr),
    m_method_element(nullptr),
    m_method_elements(nullptr),
    m_method_garbage(nullptr),
    
    m_tag_bang(kiwi->createTag("bang")),
    m_tag_long(kiwi->createTag("long")),
    m_tag_double(kiwi->createTag("double")),
    m_tag_tag(kiwi->createTag("tag")),
    m_tag_object(kiwi->createTag("object")),
    m_tag_element(kiwi->createTag("element")),
    m_tag_elements(kiwi->createTag("elements")),
    m_tag_garbage(kiwi->createTag("garbage")),
    m_tag_signal(kiwi->createTag("signal")),
    m_tag_empty(kiwi->createTag("")),
    m_tag_create(kiwi->createTag("create"))
    {
        
    }
    
    Object::~Object()
    {
        m_method_standard.clear();
        m_method_opaque.clear();
        m_attributes.clear();
        m_listeners.clear();
    }
    
    void Object::write(shared_ptr<Dico> dico) noexcept
    {
        MethodObject mwrite = (MethodObject)getMethod(createTag("write"));
        if(mwrite)
            mwrite(shared_from_this(), dico);
        
        vector<Element> elements;
        for(map<shared_ptr<Tag>, shared_ptr<Attribute>>::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            if(it->second->save())
            {
                elements.clear();
                if(hasMethod("get" + (string)*it->second->name()))
                {
                    callMethod("get" + (string)*it->second->name(), elements);
                }
                dico->set(it->second->name(), elements);
            }
        }
        dico->set(createTag("name"), name());
    }
    
    void Object::read(shared_ptr<Dico> dico) noexcept
    {
        vector<Element> elements;
        for(map<shared_ptr<Tag>, shared_ptr<Attribute>>::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
        {
            elements.clear();
            dico->get(it->second->name(), elements);
            if(hasMethod(it->second->name()) && elements.size())
            {
                callMethod(it->second->name(), elements);
            }
        }
    }
    
    // ================================================================================ //
    //                                      METHODS                                     //
    // ================================================================================ //
    
    void Object::addMethod(shared_ptr<Tag> name, Type type, Method method) noexcept
    {
        if(name == m_tag_create)
        {
            if(type == T_NOTHING || type == T_LONG || type == T_DOUBLE || type == T_ELEMENTS || type == T_TAG || type == T_OBJECT || type == T_ELEMENT)
            {
                m_method_create = {type, method};
            }
            else
            {
                warningWrongMethodDefinition(name);
                warning("This type has been replaced with elements.");
                m_method_create = {T_ELEMENTS, method};
            }
        }
        else if(name == m_tag_bang)
        {
            m_method_bang = (MethodNothing)method;
            if(type != T_NOTHING)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_long)
        {
            m_method_long = (MethodLong)method;
            if(type != T_LONG)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_double)
        {
            m_method_double = (MethodDouble)method;
            if(type != T_DOUBLE)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_tag)
        {
            m_method_tag = (MethodTag)method;
            if(type != T_TAG)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_element)
        {
            m_method_element = (MethodElement)method;
            if(type != T_ELEMENT)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_elements)
        {
            m_method_elements = (MethodElements)method;
            if(type != T_ELEMENTS)
                warningWrongMethodDefinition(name);
        }
        else if(name == m_tag_garbage)
        {
            m_method_garbage = (MethodGarbage)method;
            if(type != T_GARBAGE)
                warningWrongMethodDefinition(name);
        }
        else if(type == T_OPAQUE)
        {
            m_method_opaque[name] = method;
        }
        else
        {
            if(type == T_NOTHING || type == T_LONG || type == T_DOUBLE || type == T_ELEMENTS || type == T_TAG || type == T_OBJECT)
            {
                m_method_standard[name] = {type, method};
            }
            else
            {
                warningWrongMethodDefinition(name);
                warning("This type has been replaced with elements.");
                m_method_standard[name] = {T_ELEMENTS, method};
            }
        }
       
    }
    
    void Object::removeMethod(shared_ptr<Tag> name) noexcept
    {
        if(name == m_tag_create)
            m_method_create = {T_NOTHING, nullptr};
        else if(name == m_tag_bang)
            m_method_bang = nullptr;
        else if(name == m_tag_long)
            m_method_long = nullptr;
        else if(name == m_tag_double)
            m_method_double = nullptr;
        else if(name == m_tag_tag)
            m_method_tag = nullptr;
        else if(name == m_tag_element)
            m_method_element = nullptr;
        else if(name == m_tag_elements)
            m_method_elements = nullptr;
        else if(name == m_tag_garbage)
            m_method_garbage = nullptr;
        else if(m_method_standard.find(name) != m_method_standard.end())
            m_method_standard.erase(name);
        else if(m_method_opaque.find(name) != m_method_opaque.end())
            m_method_opaque.erase(name);
    }
    
    bool Object::hasMethod(shared_ptr<Tag> name) const noexcept
    {
        if(name == m_tag_create)
            return m_method_create.m_method != nullptr;
        if(name == m_tag_bang)
            return m_method_bang != nullptr;
        else if(name == m_tag_long)
            return m_method_long != nullptr;
        else if(name == m_tag_double)
            return m_method_double != nullptr;
        else if(name == m_tag_tag)
            return m_method_tag != nullptr;
        else if(name == m_tag_element)
            return m_method_element != nullptr;
        else if(name == m_tag_elements)
            return m_method_elements != nullptr;
        else if(name == m_tag_garbage)
            return m_method_garbage != nullptr;
        else
        {
            if(m_method_standard.find(name) != m_method_standard.end())
               return true;
            else
                return (m_method_opaque.find(name) != m_method_opaque.end());
        }
    }
    
    Method Object::getMethod(shared_ptr<Tag> name) const noexcept
    {
        if(name == m_tag_create)
            return (Method)m_method_create.m_method;
        else if(name == m_tag_bang)
            return (Method)m_method_bang;
        else if(name == m_tag_long)
            return (Method)m_method_long;
        else if(name == m_tag_double)
            return (Method)m_method_double;
        else if(name == m_tag_tag)
            return (Method)m_method_tag;
        else if(name == m_tag_element)
            return (Method)m_method_element;
        else if(name == m_tag_elements)
            return (Method)m_method_elements;
        else if(name == m_tag_garbage)
            return (Method)m_method_garbage;
        else
        {
            if(m_method_standard.find(name) != m_method_standard.end())
                return m_method_standard.find(name)->second.m_method;
            else if(m_method_opaque.find(name) != m_method_opaque.end())
                return m_method_opaque.find(name)->second;
            else
                return nullptr;
        }
    }
    
    int Object::getNumberOfMethods() const noexcept
    {
        int size = 0;
        if(m_method_create.m_method != nullptr)
            ++size;
        if(m_method_bang != nullptr)
            ++size;
        if(m_method_long != nullptr)
            ++size;
        if(m_method_double != nullptr)
            ++size;
        if(m_method_tag != nullptr)
            ++size;
        if(m_method_element != nullptr)
            ++size;
        if(m_method_elements != nullptr)
            ++size;
        if(m_method_garbage != nullptr)
            ++size;
        return size + (int)m_method_opaque.size() + (int)m_method_standard.size();
    }
    
    Type Object::getMethodType(shared_ptr<Tag> name) const noexcept
    {
        if(name == m_tag_create)
            return m_method_create.m_type;
        else if(name == m_tag_bang)
            return T_NOTHING;
        else if(name == m_tag_long)
            return T_LONG;
        else if(name == m_tag_double)
            return T_DOUBLE;
        else if(name == m_tag_tag)
            return T_TAG;
        else if(name == m_tag_element)
            return T_ELEMENT;
        else if(name == m_tag_elements)
            return T_ELEMENTS;
        else if(name == m_tag_garbage)
            return T_GARBAGE;
        else
        {
            if(m_method_standard.find(name) != m_method_standard.end())
            {
                return m_method_standard.find(name)->second.m_type;
            }
            else if(m_method_opaque.find(name) != m_method_opaque.end())
            {
                return T_OPAQUE;
            }
            else
            {
                return T_NOTHING;
            }
        }
    }
    
    ObjectMethod Object::getObjectMethod(shared_ptr<Tag> name) const noexcept
    {
        if(name == m_tag_create)
            return m_method_create;
        else if(name == m_tag_bang)
            return {T_NOTHING, (Method)m_method_bang};
        else if(name == m_tag_long)
            return {T_LONG, (Method)m_method_long};
        else if(name == m_tag_double)
            return {T_DOUBLE, (Method)m_method_double};
        else if(name == m_tag_tag)
            return {T_TAG, (Method)m_method_tag};
        else if(name == m_tag_element)
            return {T_ELEMENT, (Method)m_method_element};
        else if(name == m_tag_elements)
            return {T_ELEMENTS, (Method)m_method_elements};
        else if(name == m_tag_garbage)
            return {T_ELEMENTS, (Method)m_method_garbage};
        else
        {
            if(m_method_standard.find(name) != m_method_standard.end())
                return m_method_standard.find(name)->second;
            else if(m_method_opaque.find(name) != m_method_opaque.end())
                return {T_OPAQUE, m_method_opaque.find(name)->second};
            else
                return {T_NOTHING, (Method)nullptr};
        }
    }
    
    void Object::callMethod(shared_ptr<Tag> name)
    {
        if(stackIncrement())
            return;

        if(name == m_tag_bang)
        {
            if(m_method_bang != nullptr)
            {
                m_method_bang(shared_from_this());
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements;
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_NOTHING)
                {
                    it->second.m_method(shared_from_this());
                }
                else
                {
                    warningWrongMethodArguments(name, "nothing");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements;
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }

    void Object::callMethod(shared_ptr<Tag> name, long value)
    {
        if(stackIncrement())
            return;
        
        if(name == m_tag_long)
        {
            if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else if(name == m_tag_double)
        {
            if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_LONG || it->second.m_type == T_DOUBLE)
                {
                    it->second.m_method(shared_from_this(), value);
                }
                else if(it->second.m_type == T_ELEMENT)
                {
                    Element element = value;
                    MethodElement method = (MethodElement)it->second.m_method;
                    method(shared_from_this(), element);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> elements = {value};
                    MethodElements method = (MethodElements)it->second.m_method;
                    method(shared_from_this(), elements);
                }
                else
                {
                    warningWrongMethodArguments(name, "long");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }
    
    void Object::callMethod(shared_ptr<Tag> name, double value)
    {
        if(stackIncrement())
            return;
        
        if(name == m_tag_double)
        {
            if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else if(name == m_tag_long)
        {
            if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_LONG || it->second.m_type == T_DOUBLE)
                {
                    it->second.m_method(shared_from_this(), value);
                }
                else if(it->second.m_type == T_ELEMENT)
                {
                    Element element = value;
                    MethodElement method = (MethodElement)it->second.m_method;
                    method(shared_from_this(), element);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> elements = {value};
                    MethodElements method = (MethodElements)it->second.m_method;
                    method(shared_from_this(), elements);
                }
                else
                {
                    warningWrongMethodArguments(name, "double");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }
    
    void Object::callMethod(shared_ptr<Tag> name, shared_ptr<Tag> value)
    {
        if(stackIncrement())
            return;
        
        if(name == m_tag_tag)
        {
            if(m_method_tag != nullptr)
                m_method_tag(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
               warningWrongMethod(name);
            }
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_TAG)
                {
                    MethodTag method = (MethodTag) it->second.m_method;
                    method(shared_from_this(), value);
                }
                else if(it->second.m_type == T_ELEMENT)
                {
                    Element element = value;
                    MethodElement method = (MethodElement)it->second.m_method;
                    method(shared_from_this(), element);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> elements = {value};
                    MethodElements method = (MethodElements)it->second.m_method;
                    method(shared_from_this(), elements);
                }
                else
                {
                    warningWrongMethodArguments(name, "tag");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }
    
    void Object::callMethod(shared_ptr<Tag> name, shared_ptr<Object> value)
    {
        if(stackIncrement())
            return;
        
        if(name == m_tag_object)
        {
            if(m_method_object != nullptr)
                m_method_object(shared_from_this(), value);
            else if(m_method_element != nullptr)
            {
                Element element = value;
                m_method_element(shared_from_this(), element);
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_OBJECT)
                {
                    it->second.m_method(shared_from_this());
                }
                else if(it->second.m_type == T_ELEMENT)
                {
                    Element element = value;
                    MethodElement method = (MethodElement)it->second.m_method;
                    method(shared_from_this(), element);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> elements = {value};
                    MethodElements method = (MethodElements)it->second.m_method;
                    method(shared_from_this(), elements);
                }
                else
                {
                    warningWrongMethodArguments(name, "object");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements = {value};
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }
    
    void Object::callMethod(shared_ptr<Tag> name, vector<Element>& elements)
    {
        if(stackIncrement())
            return;
        
        if(name == m_tag_elements)
        {
            if(m_method_elements != nullptr)
                m_method_elements(shared_from_this(), elements);
            else if(m_method_element != nullptr)
            {
                if(elements.size() == 1)
                    m_method_element(shared_from_this(), elements[0]);
                else if(elements.size() > 1)
                {
                    m_method_element(shared_from_this(), elements[0]);
                    warningWrongMethodArguments(name, "element");
                }
                else
                {
                     warningWrongMethodArguments(name, "element");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else if(name == m_tag_bang)
        {
            if(m_method_bang != nullptr)
            {
                m_method_bang(shared_from_this());
                if(elements.size())
                {
                    warningWrongMethodArguments(name, "nothing");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> elements;
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        else if(name == m_tag_long)
        {
            stackDecrement();
            if(elements.size() && (elements[0].isLong() || elements[0].isDouble()))
            {
                if(m_method_long != nullptr)
                {
                    m_method_long(shared_from_this(), elements[0]);
                    if(elements.size() > 1)
                    {
                        warningWrongMethodArguments(name, "long");
                    }
                }
                else if(m_method_double != nullptr)
                {
                    m_method_double(shared_from_this(), elements[0]);
                    if(elements.size() > 1)
                    {
                        warningWrongMethodArguments(name, "double");
                    }
                }
                else if(m_method_element != nullptr)
                {
                    m_method_element(shared_from_this(), elements[0]);
                    if(elements.size() > 1)
                    {
                        warningWrongMethodArguments(name, "element");
                    }
                }
                else if(m_method_garbage != nullptr)
                {
                    m_method_garbage(shared_from_this(), name, elements);
                }
                else
                {
                    warningWrongMethod(name);
                }
            }
            else
            {
                warningWrongMethodArguments(name, "long");
            }
        }
        else if(name == m_tag_double)
        {
            stackDecrement();
            callMethod(name, (double)elements[0]);
        }
        else if(name == m_tag_tag)
        {
            stackDecrement();
            callMethod(name, (shared_ptr<Tag>)elements[0]);
        }
        else if(name == m_tag_object)
        {
            stackDecrement();
            callMethod(name, (shared_ptr<Object>)elements[0]);
        }
        else
        {
            map<shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_ELEMENTS)
                {
                    MethodElements method = (MethodElements)it->second.m_method;
                    method(shared_from_this(), elements);
                }
                else if(elements.size())
                {
                    if(it->second.m_type == T_LONG && (elements[0].isLong() || elements[0].isDouble()))
                    {
                        it->second.m_method(shared_from_this(), (long)elements[0]);
                    }
                    else if(it->second.m_type == T_DOUBLE && (elements[0].isLong() || elements[0].isDouble()))
                    {
                         it->second.m_method(shared_from_this(), (double)elements[0]);
                    }
                    else if(it->second.m_type == T_TAG && elements[0].isTag())
                    {
                        MethodTag method = (MethodTag)it->second.m_method;
                        method(shared_from_this(), (shared_ptr<Tag>)(elements[0]));
                    }
                }
                else
                {
                    warningWrongMethodArguments(name, "elements");
                }
            }
            else if(m_method_garbage != nullptr)
            {
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                warningWrongMethod(name);
            }
        }
        
        stackDecrement();
    }
    
    void Object::callMethodOpaque(shared_ptr<Tag> name, int size, ...)
    {
        if(stackIncrement())
            return;
        
        map<shared_ptr<Tag>, Method>::iterator it = m_method_opaque.find(name);
        if(it != m_method_opaque.end())
        {
            if(size > 10)
            {
                string message = string("Hey ?!? You're so evil to pass ") + to_string(size) + string("  parameters that we take only 10 of them !");
                warningObject(message);
                size= 10;
            }
            void* args[11];
            va_list argp;
            va_start(argp, size);
            for(int i = 0; i < size; i++)
            {
                args[i] = (void *)va_arg(argp, long long);
            }
            switch (size)
            {
                case 0:it->second(shared_from_this());break;
                case 1:it->second(shared_from_this(), args[0]);break;
                case 2:it->second(shared_from_this(), args[0], args[1]);break;
                case 3:it->second(shared_from_this(), args[0], args[1], args[2]);break;
                case 4:it->second(shared_from_this(), args[0], args[1], args[2], args[3]);break;
                case 5:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4]);break;
                case 6:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5]);break;
                case 7:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6]);break;
                case 8:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);break;
                case 9:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);break;
                case 10:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);break;
                default: break;
            }
            va_end(argp);
        }
        else
        {
            warningWrongMethod(name);
        }
        
        stackDecrement();
    }
    
    void Object::callMethodOpaque(string name, int size, ...)
    {
        if(stackIncrement())
            return;
        
        map<shared_ptr<Tag>, Method>::iterator it = m_method_opaque.find(createTag(name));
        if(it != m_method_opaque.end())
        {
            if(size > 10)
            {
                string message = string("Hey ?!? You're so evil to pass ") + to_string(size) + string("  parameters that we take only 10 of them !");
                warningObject(message);
                size= 10;
            }
            void* args[11];
            va_list argp;
            va_start(argp, size);
            for(int i = 0; i < size; i++)
            {
                args[i] = (void *)va_arg(argp, long long);
            }
            switch (size)
            {
                case 0:it->second(shared_from_this());break;
                case 1:it->second(shared_from_this(), args[0]);break;
                case 2:it->second(shared_from_this(), args[0], args[1]);break;
                case 3:it->second(shared_from_this(), args[0], args[1], args[2]);break;
                case 4:it->second(shared_from_this(), args[0], args[1], args[2], args[3]);break;
                case 5:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4]);break;
                case 6:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5]);break;
                case 7:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6]);break;
                case 8:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);break;
                case 9:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);break;
                case 10:it->second(shared_from_this(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);break;
                default: break;
            }
            va_end(argp);
        }
        else
        {
            warningWrongMethod(createTag(name));
        }
        
        stackDecrement();
    }
    
    // ================================================================================ //
    //                                  ATTRIBUTES                                      //
    // ================================================================================ //
    
    void Object::createAttribute(shared_ptr<Tag> name, Method getter, Method setter) noexcept
    {
        if(m_attributes.find(name) != m_attributes.end())
        {
            error((string)*m_name + string(" : The attribute ") + (string)*name + string(" already exists !"));
        }
        else
        {
            if(getter)
            {
                addMethod("get" + (string)*name, T_ELEMENTS, getter);
            }
            if(setter)
            {
                addMethod(name, T_ELEMENTS, setter);
            }
            m_attributes[name] = make_shared<Attribute>(name);
        }
    }
    
    bool Object::hasAttribute(shared_ptr<Tag> name) const noexcept
    {
        return m_attributes.find(name) != m_attributes.end();
    }
    
    shared_ptr<Attribute> Object::getAttribute(shared_ptr<Tag> name) const noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
            return it->second;
        else
            return nullptr;
    }
    
    void Object::setAttributeAppearance(shared_ptr<Tag> name, shared_ptr<Tag> label, shared_ptr<Tag> style, shared_ptr<Tag> category) noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->appearance(label, style, category);
        }
        else
        {
            warningWrongAttribute(name);
        }
    }
    
    void Object::setAttributeBehavior(shared_ptr<Tag> name, bool opaque, bool visible, bool save) noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->behavior(opaque, visible, save);
        }
        else
        {
            warningWrongAttribute(name);
        }
    }
    
    void Object::setAttributeValue(shared_ptr<Tag> name, Element const& element) noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->set(element);
        }
        else
        {
            warningWrongAttribute(name);
        }
    }
    
    void Object::setAttributeValues(shared_ptr<Tag> name, vector<Element> const& elements) noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->set(elements);
        }
        else
        {
            warningWrongAttribute(name);
        }
    }
    
    Element Object::getAttributeValue(shared_ptr<Tag> name) const noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            return it->second->get();
        }
        else
        {
            warningWrongAttribute(name);
            return 0;
        }
    }

    void Object::getAttributeValues(shared_ptr<Tag> name, vector<Element>& elements) const noexcept
    {
        map<shared_ptr<Tag>, shared_ptr<Attribute>>::const_iterator it = m_attributes.find(name);
        if(it != m_attributes.end())
        {
            it->second->get(elements);
        }
        else
        {
            warningWrongAttribute(name);
        }
    }
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    shared_ptr<Tag> Object::createTag(string name) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createTag(name);
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(shared_ptr<Tag> name, Element const& element) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            shared_ptr<Dico> dico = kiwi->createDico();
            dico->set(createTag("name"), name);
            dico->set(createTag("arguments"), element);
            return kiwi->createObject(dico);
        }
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(shared_ptr<Tag> name, vector<Element> const& elements) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            shared_ptr<Dico> dico = kiwi->createDico();
            dico->set(createTag("name"), name);
            dico->set(createTag("arguments"), elements);
            return kiwi->createObject(dico);
        }
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(shared_ptr<Dico> dico) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            return kiwi->createObject(dico);
        }
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(string const& text) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            shared_ptr<Dico> dico = kiwi->createDico();
            dico->readFormatted(text);
            return kiwi->createObject(dico);
        }
        else
            return nullptr;
    }
    
    shared_ptr<Dico> Object::createDico() const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createDico();
        else
            return nullptr;
    }
    
    // ================================================================================ //
    //                                      MESSAGES                                    //
    // ================================================================================ //
    
    void Object::post(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->post(message);
    }
    
    void Object::postObject(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->post(shared_from_this(), message);
    }
    
    void Object::warning(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->warning(message);
    }
    
    void Object::warningObject(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->warning(shared_from_this(), message);
    }
    
    void Object::error(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->error(message);
    }
    
    void Object::errorObject(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->error(shared_from_this(), message);
    }
}


















