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

#include "Instance.h"
#include "../KiwiBoxes/Arithmetic.h"
#include "../KiwiBoxes/ArithmeticTilde.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    Instance::Instance() noexcept : m_untitled_pages(0), m_dsp_running(0)
    {
        
    }
    
    Instance::~Instance()
    {
        m_pages.clear();
        m_prototypes.clear();
    }
    
    shared_ptr<Instance> Instance::create()
    {
        shared_ptr<Instance> that = make_shared<Instance>();
        Arithmetic::load(that);
        ArithmeticTilde::load(that);
        return that;
    }
    
    void Instance::addObjectPrototype(unique_ptr<Object> object)
    {
        map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(object->name());
        if(it != m_prototypes.end())
        {
            error("The object prototype " + (string)*object->name() + " already exist !");
        }
        else
        {
            m_prototypes[object->name()] = move(object);
        }
    }
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    shared_ptr<Object> Instance::createObject(shared_ptr<Dico> dico) noexcept
    {
        shared_ptr<Object> object;
        shared_ptr<Tag> name = (shared_ptr<Tag>)dico->get(createTag("name"));
        if(name)
        {
            map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(name);
            if(it != m_prototypes.end())
            {
                ObjectMethod create = (ObjectMethod)it->second->getObjectMethod(createTag("create"));
                if(create.m_method)
                {
                    vector<Element> elements;
                    dico->get(createTag("arguments"), elements);
                    switch (create.m_type)
                    {
                        case T_NOTHING:
        
                            object = MethodCreate(create.m_method)(shared_from_this(), name);
                            if(elements.size())
                            {
                                warningCreation(name);
                            }
                            break;
                            
                        case T_LONG:
                            
                            if(elements.size() && (elements[0].isLong() || elements[0].isDouble()))
                            {
                                object = MethodCreate(create.m_method)(shared_from_this(), name, (long)elements[0]);
                                if(elements.size() > 1)
                                {
                                   warningCreation(name);
                                }
                            }
                            else
                            {
                                errorCreation(name, "long");
                            }
                            break;
                            
                        case T_DOUBLE:
                            
                            if(elements.size() && (elements[0].isLong() || elements[0].isDouble()))
                            {
                                object = MethodCreate(create.m_method)(shared_from_this(), name, (double)elements[0]);
                                if(elements.size() > 1)
                                {
                                    warningCreation(name);
                                }
                            }
                            else
                            {
                                errorCreation(name, "double");
                            }
                            break;
                        
                        case T_TAG:
                            
                            if(elements.size() && elements[0].isTag())
                            {
                                object = MethodCreateTag(create.m_method)(shared_from_this(), name, (shared_ptr<Tag>)elements[0]);
                                if(elements.size() > 1)
                                {
                                    warningCreation(name);
                                }
                            }
                            else
                            {
                                errorCreation(name, "tag");
                            }
                            break;
                            
                        case T_OBJECT:
                            
                            if(elements.size() && elements[0].isObject())
                            {
                                object = MethodCreateObject(create.m_method)(shared_from_this(), name, dico);
                            }
                            else
                            {
                                errorCreation(name, "object");
                            }
                            break;
                            
                        case T_ELEMENT:
                            
                            if(elements.size())
                            {
                                object = MethodCreateElement(create.m_method)(shared_from_this(), name, elements[0]);
                                if(elements.size() > 1)
                                {
                                    warningCreation(name);
                                }
                            }
                            else
                            {
                                errorCreation(name, "element");
                            }
                            break;
                            
                        case T_ELEMENTS:
                            
                            object = MethodCreateElements(create.m_method)(shared_from_this(), name, elements);
                            break;
                            
                        default:
                            error("The object \"" + (string)*name + " doesn't have a creation method properly defined.");
                            break;
                    }
                    if(object && create.m_type != T_OBJECT)
                    {
                        object->read(dico);
                    }
                }
                else
                {
                    error("The object \"" + (string)*name + " can't be allocated dynamically.");
                }
            }
            else
            {
                error("The object \"" + (string)*name + " doesn't exists.");
            }
        }
        else
        {
            error("The dico should have tag in a name key to create an object.");
        }
    
        return object;
    }

    shared_ptr<Dico> Instance::createDico() noexcept
    {
        return make_shared<Dico>(shared_from_this());
    }

    shared_ptr<Page> Instance::createPage(string file, string directory)
    {
        if(file.empty())
		{
			file = string("Untitled") + to_string(++m_untitled_pages);
		}
  
        shared_ptr<Page> newPage = make_shared<Page>(shared_from_this(), file, directory);
        m_pages.push_back(newPage);
        return newPage;
    }
    
    void Instance::closePage(shared_ptr<Page> page)
    {
        for(int i = 0; i < m_pages.size(); i++)
        {
            if(m_pages[i] == page)
            {
                m_pages.erase(m_pages.begin()+i);
                break;
            }
        }
    }
    
    // ================================================================================ //
    //                                      DSP                                         //
    // ================================================================================ //
    
    void Instance::startDsp(double samplerate, long vectorsize)
    {
        m_dsp_running = true;
        for(size_t i = 0; i < m_pages.size(); i++)
            m_pages[i]->startDsp(samplerate, vectorsize);
    }
    
    void Instance::tickDsp() const noexcept
    {
        for(size_t i = 0; i < m_pages.size(); i++)
            m_pages[i]->tickDsp();
    }
    
    void Instance::stopDsp()
    {
        for(size_t i = 0; i < m_pages.size(); i++)
            m_pages[i]->stopDsp();
        m_dsp_running = false;
    }
    
    bool Instance::isDspRunning() const noexcept
    {
        return m_dsp_running;
    }
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    void Instance::bind(weak_ptr<InstanceListener> listener)
    {
        m_listeners.insert(listener);
    }
    
    void Instance::unbind(weak_ptr<InstanceListener> listener)
    {
        m_listeners.erase(listener);
    }
    
    void Instance::post(string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cout << message << "\n";
#endif
        set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<InstanceListener> to = (*it).lock();
            to->post(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
            ++it;
        }
        }
        
        void Instance::post(const shared_ptr<const Object> object, string message) const noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << (string)*object->name() << " : " << message << "\n";
#endif
            set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<InstanceListener> to = (*it).lock();
                to->post(shared_from_this(), object, message);
                ++it;
            }
        }
        
        void Instance::warning(string message) const noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << "warning : " << message << "\n";
#endif
            set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<InstanceListener> to = (*it).lock();
                to->warning(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
                ++it;
            }
        }
        
        void Instance::warning(const shared_ptr<const Object> object, string message) const noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << (string)*object->name() << " : " << message << "\n";
#endif
            set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<InstanceListener> to = (*it).lock();
                to->warning(shared_from_this(), object, message);
                ++it;
            }
        }
        
        void Instance::error(string message) const noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << "error : " << message << "\n";
#endif
            set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<InstanceListener> to = (*it).lock();
                to->error(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
                ++it;
            }
        }
        
        void Instance::error(const shared_ptr<const Object> object, string message) const noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << (string)*object->name() << " : " << message << "\n";
#endif
            set<weak_ptr<InstanceListener>>::iterator it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<InstanceListener> to = (*it).lock();
                to->error(shared_from_this(), object, message);
                ++it;
            }
        }
}
















