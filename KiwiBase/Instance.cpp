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
    
    shared_ptr<Object> Instance::createObject(shared_ptr<Dico> dico)
    {
        shared_ptr<Tag> name = (shared_ptr<Tag>)dico->get(createTag("name"));
        if(name)
        {
            map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(name);
            if(it != m_prototypes.end())
            {
                return it->second->create(dico);
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
    
        return nullptr;
    }

    shared_ptr<Dico> Instance::createDico()
    {
        return make_shared<Dico>(shared_from_this());
    }

    shared_ptr<Page> Instance::createPage(string file, string directory)
    {
        if(file.empty())
		{
			file = string("Untitled") + toString(++m_untitled_pages);
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
    
    void Instance::post(string const& message) const noexcept
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
        
        void Instance::post(const shared_ptr<const Object> object, string const& message) const noexcept
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
        
        void Instance::warning(string const& message) const noexcept
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
        
        void Instance::warning(const shared_ptr<const Object> object, string const& message) const noexcept
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
        
        void Instance::error(string const& message) const noexcept
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
        
        void Instance::error(const shared_ptr<const Object> object, string const& message) const noexcept
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
















