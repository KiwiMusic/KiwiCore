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
#include "Console.h"
#include "../KiwiObjects/Objects.h"

namespace Kiwi
{
    bool libraries_loaded = false;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    Instance::Instance(sDspDeviceManager device, string const& name) noexcept :
    DspContext(device),
    m_name(name)
    {
		;
    }
    
    Instance::~Instance()
    {
        m_patchers.clear();
        m_lists.clear();
#ifdef __KIWI_VERBOSE__
        Console::post("The instance \"" + m_name + "\" has been deleted.");
#endif
    }
    
    sInstance Instance::create(sDspDeviceManager device, string const& name)
    {
        sInstance instance;
        if(!name.empty())
        {
            sTag tname = Tag::create(name);
            if(!libraries_loaded)
            {
                libraries_loaded = ObjectsInitialize();
            }
            if(Console::m_instances.find(tname) == Console::m_instances.end())
            {
                instance = make_shared<Instance>(device, name);
                if(instance)
                {
                    Console::m_instances[tname] = instance;
#ifdef __KIWI_VERBOSE__
                    Console::post("The instance \"" + name + "\" has been created.");
#endif
                }
            }
            else
            {
                Console::error("The instance's name \"" + instance->getName() + "\" already exists.");
            }
        }
        else
        {
            Console::error("The instance needs a name to be created.");
        }
        return instance;
    }
    
    sPatcher Instance::createPatcher()
    {
        Dico dico;
        return createPatcher(dico);
    }

    sPatcher Instance::createPatcher(Dico& dico)
    {
        sPatcher patcher = Patcher::create(getShared(), dico);
        if(patcher)
        {
            m_patchers_mutex.lock();
            m_patchers.insert(patcher);
            m_patchers_mutex.unlock();
            DspContext::add(patcher);
            
            m_lists_mutex.lock();
            auto it = m_lists.begin();
            while(it != m_lists.end())
            {
                if((*it).expired())
                {
                    it = m_lists.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->patcherCreated(getShared(), patcher);
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
        return patcher;
    }
    
    void Instance::removePatcher(sPatcher patcher)
    {
        m_patchers_mutex.lock();
        if(m_patchers.find(patcher) != m_patchers.end())
        {
            DspContext::remove(patcher);
            
            m_patchers.erase(patcher);
            m_patchers_mutex.unlock();
            
            m_lists_mutex.lock();
            auto it = m_lists.begin();
            while(it != m_lists.end())
            {
                if((*it).expired())
                {
                    it = m_lists.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->patcherRemoved(getShared(), patcher);
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
        else
        {
            m_patchers_mutex.unlock();
        }
    }
    
    void Instance::getPatchers(vector<sPatcher>& patchers)
    {
        lock_guard<mutex> guard(m_patchers_mutex);
        patchers.assign(m_patchers.begin(), m_patchers.end());
    }
    
    void Instance::addListener(sListener listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            m_lists.insert(listener);
        }
    }
    
    void Instance::removeListener(sListener listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            m_lists.erase(listener);
        }
    }
    
    void Instance::create(Vector const& inputs, Vector outputs)
    {
        
    }
    
    void Instance::remove(Vector const& inputs)
    {
        
    }
    
    void Instance::get(Vector const& inputs, Vector outputs) const
    {
        if(!inputs.empty())
        {
            if(inputs[0].isTag() && sTag(inputs[0]) == Tag::List::dsp)
            {
                if(inputs.size() > 1)
                {
                    if(inputs[0].isTag())
                    {
                        if(sTag(inputs[0]) == Tag::List::dsp)
                        {
                            
                        }
                    }
                }
                else
                {
                    Console::error("The dsp gettings method needs arguments.");
                }
            }
        }
        else
        {
            Console::error("The gettings method needs arguments.");
        }
    }
    
    void Instance::set(Vector const& inputs)
    {
        if(!inputs.empty())
        {
            if(inputs[0].isTag() && sTag(inputs[0]) == Tag::List::dsp)
            {
                
            }
        }
        else
        {
            Console::error("The setting method needs arguments.");
        }
    }
    
    // ================================================================================ //
    //                                      OBJECT FACTORY                              //
    // ================================================================================ //
    
    map<sTag, shared_ptr<Factory::Creator>> Factory::m_creators;
    mutex Factory::m_mutex;
    
    sObject Factory::create(sTag name, Detail const& detail)
    {
        auto it = m_creators.find(name);
        if(it != m_creators.end())
        {
            sObject obj = it->second->create(detail);
            if(obj)
            {
                obj->initialize();
                //scDico dico = detail.dico;
                vector<sAttr> attrs;
                obj->getAttrs(attrs);
                for(vector<sAttr>::size_type i = 0; i < attrs.size(); i++)
                {
                    sTag name = Tag::create(attrs[i]->getName());
                    int todo;
                    /*
                    if(dico->has(name))
                    {
                         attrs[i]->setValueString(dico->getAsString(name));
                    }
                     */
                }
            }
            return obj;
        }
        else
        {
            Console::error("The factory doesn't know the object " + name->getName());
            return nullptr;
        }
    }
    
    bool Factory::has(sTag name)
    {
        lock_guard<mutex> guard(m_mutex);
        return m_creators.find(name) != m_creators.end();
    }
    
    vector<sTag> Factory::names()
    {
        lock_guard<mutex> guard(m_mutex);
        vector<sTag> names;
        for(auto it = m_creators.begin(); it !=  m_creators.end(); ++it)
        {
            names.push_back(it->first);
        }
        return names;
    }
}
















