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

#define STACK_LIMIT 256

namespace Kiwi
{
    Object::Object(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name) :
    m_kiwi(kiwi),
    m_name(name),
    m_stack_count(0),
    
    m_method_create({T_NOTHING, nullptr}),
    m_method_bang(nullptr),
    m_method_long(nullptr),
    m_method_double(nullptr),
    m_method_tag(nullptr),
    m_method_object(nullptr),
    m_method_elements(nullptr),
    m_method_garbage(nullptr),
    
    m_tag_bang(kiwi->createTag("bang")),
    m_tag_long(kiwi->createTag("long")),
    m_tag_double(kiwi->createTag("double")),
    m_tag_tag(kiwi->createTag("tag")),
    m_tag_object(kiwi->createTag("object")),
    m_tag_elements(kiwi->createTag("elements")),
    m_tag_garbage(kiwi->createTag("garbage")),
    m_tag_signal(kiwi->createTag("signal")),
    m_tag_empty(kiwi->createTag("")),
    m_tag_create(kiwi->createTag("create"))
    {
        ;
    }
    
    Object::Object(shared_ptr<Instance> kiwi, const string name) :
    m_kiwi(kiwi),
    m_name(kiwi->createTag(name)),
    m_stack_count(0),
    
    m_method_create({T_NOTHING, nullptr}),
    m_method_bang(nullptr),
    m_method_long(nullptr),
    m_method_double(nullptr),
    m_method_tag(nullptr),
    m_method_object(nullptr),
    m_method_elements(nullptr),
    m_method_garbage(nullptr),
    
    m_tag_bang(kiwi->createTag("bang")),
    m_tag_long(kiwi->createTag("long")),
    m_tag_double(kiwi->createTag("double")),
    m_tag_tag(kiwi->createTag("tag")),
    m_tag_object(kiwi->createTag("object")),
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
    }
    
    void Object::addMethod(const shared_ptr<Tag> name, Type type, Method method)
    {
        if(name == m_tag_create)
        {
            if(type == T_NOTHING || type == T_LONG || type == T_DOUBLE || type == T_ELEMENTS || type == T_TAG || type == T_OBJECT)
            {
                m_method_create = {type, method};
            }
            else
            {
                string message = string("You defined a wrong type for the ") + (string)*name + string( "method ! \n This type has been replaced with elements.");
                warningObject(message);
                m_method_create = {T_ELEMENTS, method};
            }
        }
        else if(name == m_tag_bang)
        {
            m_method_bang = (MethodNothing)method;
            if(type != T_NOTHING)
                warningObject(string("You defined a wrong type for the bang method !"));
        }
        else if(name == m_tag_long)
        {
            m_method_long = (MethodLong)method;
            if(type != T_LONG)
                warningObject(string("You defined a wrong type for the long method !"));
        }
        else if(name == m_tag_double)
        {
            m_method_double = (MethodDouble)method;
            if(type != T_DOUBLE)
                warningObject(string("You defined a wrong type for the double method !"));
        }
        else if(name == m_tag_tag)
        {
            m_method_tag = (MethodTag)method;
            if(type != T_TAG)
                warningObject(string("You defined a wrong type for the tag method !"));
        }
        else if(name == m_tag_elements)
        {
            m_method_elements = (MethodElements)method;
            if(type != T_ELEMENTS)
                warningObject(string("You defined a wrong type for the elements method !"));
        }
        else if(name == m_tag_garbage)
        {
            m_method_garbage = (MethodGarbage)method;
            if(type != T_GARBAGE)
                warningObject(string("You defined a wrong type for the garbage method  !"));
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
                string message = string("You defined a wrong type for the ") + (string)*name + string( "method ! \n This type has been replaced with elements.");
                warningObject(message);
                m_method_standard[name] = {T_ELEMENTS, method};
            }
        }
       
    }
    
    void Object::addMethod(string name, Type type, Method method)
    {
        addMethod(createTag(name), type, method);
    }
    
    void Object::removeMethod(const shared_ptr<Tag> name) noexcept
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
        else if(name == m_tag_elements)
            m_method_elements = nullptr;
        else if(name == m_tag_garbage)
            m_method_garbage = nullptr;
        else if(m_method_standard.find(name) != m_method_standard.end())
            m_method_standard.erase(name);
        else if(m_method_opaque.find(name) != m_method_opaque.end())
            m_method_opaque.erase(name);
    }
    
    void Object::removeMethod(string name) noexcept
    {
        removeMethod(createTag(name));
    }
    
    bool Object::hasMethod(const shared_ptr<Tag> name) const
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
    
    bool Object::hasMethod(string name)
    {
        return hasMethod(createTag(name));
    }
    
    Method Object::getMethod(const shared_ptr<Tag> name) const
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
    
    Method Object::getMethod(string name)
    {
        return getMethod(createTag(name));
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
        if(m_method_elements != nullptr)
            ++size;
        if(m_method_garbage != nullptr)
            ++size;
        return size + (int)m_method_opaque.size() + (int)m_method_standard.size();
    }
    
    Type Object::getMethodType(const shared_ptr<Tag> name) const
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
    
    Type Object::getMethodType(string name)
    {
        return getMethodType(createTag(name));
    }

    void Object::callMethod(const shared_ptr<Tag> name)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        if(name == m_tag_bang)
        {
            if(m_method_bang != nullptr)
            {
                m_method_bang(shared_from_this());
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_NOTHING)
                {
                    it->second.m_method(shared_from_this());
                }
                else
                {
                    string message = string("Wrong arguments for the method ") + (string)*name + string(" received nothing !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name)
    {
        callMethod(createTag(name));
    }

    void Object::callMethod(const shared_ptr<Tag> name, long value)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        if(name == m_tag_long)
        {
            if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else if(name == m_tag_double)
        {
            if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_LONG || it->second.m_type == T_DOUBLE)
                {
                    it->second.m_method(shared_from_this(), value);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> temp;
                    temp.push_back(value);
                    it->second.m_method(shared_from_this(), temp);
                    temp.clear();
                }
                else
                {
                    string message = string("Wrong arguments for the method ")+ (string)*name + string(" received long !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name, long value)
    {
        callMethod(createTag(name), value);
    }
    
    void Object::callMethod(const shared_ptr<Tag> name, double value)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        if(name == m_tag_double)
        {
            if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else if(name == m_tag_long)
        {
            if(m_method_long != nullptr)
                m_method_long(shared_from_this(), value);
            else if(m_method_double != nullptr)
                m_method_double(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_LONG || it->second.m_type == T_DOUBLE)
                {
                    it->second.m_method(shared_from_this(), value);
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> temp;
                    temp.push_back(value);
                    it->second.m_method(shared_from_this(), temp);
                    temp.clear();
                }
                else
                {
                    string message = string("Wrong arguments for the method ")+ (string)*name + string(" received double !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name, double value)
    {
        callMethod(createTag(name), value);
    }
    
    void Object::callMethod(const shared_ptr<Tag> name, shared_ptr<Tag> value)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        if(name == m_tag_tag)
        {
            if(m_method_tag != nullptr)
                m_method_tag(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_TAG)
                {
                    it->second.m_method(shared_from_this());
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> temp;
                    temp.push_back(value);
                    it->second.m_method(shared_from_this(), temp);
                    temp.clear();
                }
                else
                {
                    string message = string("Wrong arguments for the method ")+ (string)*name + string(" received tag !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name, shared_ptr<Tag> value)
    {
        callMethod(createTag(name), value);
    }
    
    void Object::callMethod(const shared_ptr<Tag> name, shared_ptr<Object> value)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        if(name == m_tag_object)
        {
            if(m_method_object != nullptr)
                m_method_object(shared_from_this(), value);
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_OBJECT)
                {
                    it->second.m_method(shared_from_this());
                }
                else if(it->second.m_type == T_ELEMENTS)
                {
                    vector<Element> temp;
                    temp.push_back(value);
                    it->second.m_method(shared_from_this(), temp);
                    temp.clear();
                }
                else
                {
                    string message = string("Wrong arguments for the method ")+ (string)*name + string(" received tag !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                vector<Element> temp;
                temp.push_back(value);
                m_method_garbage(shared_from_this(), name, temp);
                temp.clear();
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name, shared_ptr<Object> object)
    {
        callMethod(createTag(name), object);
    }
    
    void Object::callMethod(const shared_ptr<Tag> name, vector<Element>& elements)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return error(string("Stack overflow"));
        
        if(name == m_tag_elements)
        {
            if(m_method_elements != nullptr)
                m_method_elements(shared_from_this(), elements);
            else if(m_method_garbage != nullptr)
            {
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        else if(name == m_tag_bang)
        {
            --m_stack_count;
            callMethod(name);
        }
        else if(name == m_tag_long)
        {
            --m_stack_count;
            callMethod(name, (long)elements[0]);
        }
        else if(name == m_tag_double)
        {
            --m_stack_count;
            callMethod(name, (double)elements[0]);
        }
        else if(name == m_tag_tag)
        {
            --m_stack_count;
            callMethod(name, (shared_ptr<Tag>)elements[0]);
        }
        else if(name == m_tag_object)
        {
            --m_stack_count;
            callMethod(name, (shared_ptr<Object>)elements[0]);
        }
        else
        {
            map<const shared_ptr<Tag>, ObjectMethod>::const_iterator it = m_method_standard.find(name);
            if(it != m_method_standard.end())
            {
                if(it->second.m_type == T_ELEMENTS)
                {
                    it->second.m_method(shared_from_this(), elements);
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
                        it->second.m_method(shared_from_this(), (shared_ptr<Tag>)(elements[0]));
                    }
                }
                else
                {
                    string message = string("Wrong arguments for the method ")+ (string)*name + string(" received elements !");
                    warningObject(message);
                }
            }
            else if(m_method_garbage != nullptr)
            {
                m_method_garbage(shared_from_this(), name, elements);
            }
            else
            {
                string message = string("Don't have such method : ") + (string)*name;
                warningObject(message);
            }
        }
        
        --m_stack_count;
    }
    
    void Object::callMethod(string name, vector<Element>& elements)
    {
        callMethod(createTag(name), elements);
    }
    
    void Object::callMethodOpaque(const shared_ptr<Tag> name, int size, ...)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        map<const shared_ptr<Tag>, Method>::iterator it = m_method_opaque.find(name);
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
            string message = string("Don't have such method : ") + (string)*name;
            warningObject(message);
        }
        
        --m_stack_count;
    }
    
    void Object::callMethodOpaque(string name, int size, ...)
    {
        if(++m_stack_count >= STACK_LIMIT)
            return errorObject(string("Stack overflow"));
        
        map<const shared_ptr<Tag>, Method>::iterator it = m_method_opaque.find(createTag(name));
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
            string message = string("Don't have such method : ") + name;
            warningObject(message);
        }
        
        --m_stack_count;
    }
    
    shared_ptr<Tag> Object::createTag(string name) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createTag(name);
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(shared_ptr<Tag> name, vector<Element>& elements) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createObject(name, elements);
        else
            return nullptr;
    }
    
    shared_ptr<Object> Object::createObject(string name, vector<Element>& elements) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createObject(name, elements);
        else
            return nullptr;
    }
    
    shared_ptr<Connection> Object::createConnection(shared_ptr<Box> from, int oulet, shared_ptr<Box> to, int inlet) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createConnection(from, oulet, to, inlet);
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
    
    shared_ptr<Json> Object::createJson() const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createJson();
        else
            return nullptr;
    }
    
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
    
    void Object::write(shared_ptr<Dico> dico)
    {
        /*
        if(hasMethod(createTag("write")))
            callMethodOpaque(createTag("write"), 1, dico);
         */
        dico->set(createTag("name"), getName());
    }
}
















