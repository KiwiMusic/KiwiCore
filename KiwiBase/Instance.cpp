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

#include "../KiwiBoxes/Gui.h"
/*
#include "../KiwiBoxes/Wireless.h"
#include "../KiwiBoxes/Time.h"
#include "../KiwiBoxes/DspGenerator.h"
#include "../KiwiBoxes/NewBox.h"
#include "../KiwiBoxes/Arithmetic.h"
#include "../KiwiBoxes/ArithmeticTilde.h"
*/
namespace Kiwi
{
    bool libraries_loaded = false;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    Instance::Instance() noexcept :
    m_dsp_running(false),
    m_sample_rate(0),
    m_vector_size(0)
    {
		;
    }
    
    Instance::~Instance()
    {
        m_pages.clear();
        m_lists.clear();
        m_dsp_pages.clear();
    }
    
    sInstance Instance::create()
    {
        if(!libraries_loaded)
        {
            guiInit();
            /*
			standardBoxes();
            arithmetic();
            wireless();
			timing();
            GeneratorTildeInit();
             */
            libraries_loaded = true;
        }
        return make_shared<Instance>();
    }

    sPatcher Instance::createPatcher(sDico dico)
    {
        sPatcher page = Patcher::create(shared_from_this(), dico);
        if(page)
        {
            m_pages_mutex.lock();
            m_pages.insert(page);
            m_pages_mutex.unlock();
            
            if(m_dsp_running)
            {
                try
                {
                    int todo;
                    page->dspStart(m_sample_rate, m_vector_size);
                    m_dsp_mutex.lock();
                    m_dsp_pages.push_back(page);
                    m_dsp_mutex.unlock();
                }
                catch(sPatcher)
                {
                    ;
                }
            }
            
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
                    listener->pageCreated(shared_from_this(), page);
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
        return page;
    }
    
    void Instance::removePatcher(sPatcher page)
    {
        m_pages_mutex.lock();
        if(m_pages.find(page) != m_pages.end())
        {
            if(m_dsp_running && page->isDspRunning())
            {
                m_dsp_mutex.lock();
                page->dspStop();
                auto it  = find(m_dsp_pages.begin(), m_dsp_pages.end(), page);
                if(it != m_dsp_pages.end())
                {
                    m_dsp_pages.erase(it);
                }
                m_dsp_mutex.unlock();
            }
            
            m_pages.erase(page);
            m_pages_mutex.unlock();
            
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
                    listener->pageRemoved(shared_from_this(), page);
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
        else
        {
            m_pages_mutex.unlock();
        }
    }
    
    void Instance::getPatchers(vector<sPatcher>& pages)
    {
        lock_guard<mutex> guard(m_pages_mutex);
        pages.assign(m_pages.begin(), m_pages.end());
    }
    
    void Instance::dspStart(ulong samplerate, ulong vectorsize)
    {
        if(m_dsp_running)
        {
            dspStop();
        }
        m_sample_rate   = samplerate;
        m_vector_size   = vectorsize;
        
        m_dsp_mutex.lock();
        m_pages_mutex.lock();
        for(auto it = m_pages.begin(); it != m_pages.end(); ++it)
        {
            int todo;
            try
            {
                (*it)->dspStart(m_sample_rate, m_vector_size);
            }
            catch (sPatcher)
            {
                ;
            }
            m_dsp_pages.push_back((*it));
        }
        m_pages_mutex.unlock();
        
        if(!m_dsp_pages.empty())
        {
            m_dsp_running   = true;
            m_dsp_mutex.unlock();
            
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
                    listener->dspStarted(shared_from_this());
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
        else
        {
            m_dsp_mutex.unlock();
        }
    }
    
    void Instance::dspStop()
    {
        if(m_dsp_running)
        {
            m_dsp_mutex.lock();
            for(vector<sPatcher>::size_type i = 0; i < m_dsp_pages.size(); i++)
            {
                m_dsp_pages[i]->dspStop();
            }
            m_dsp_pages.clear();
            m_dsp_mutex.unlock();
            
            m_dsp_running = false;
            
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
                    listener->dspStopped(shared_from_this());
                    ++it;
                }
            }
            m_lists_mutex.unlock();
        }
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
    
    // ================================================================================ //
    //                                      OBJECT FACTORY                              //
    // ================================================================================ //
    
    map<sTag, shared_ptr<Prototypes::ObjectCreator>> Prototypes::m_creators;
    mutex Prototypes::m_mutex;
    
    sObject Prototypes::create(sTag name, Initializer const& init)
    {
        auto it = m_creators.find(name);
        if(it != m_creators.end())
        {
            return it->second->create(init);
        }
        else
        {
            return nullptr;
        }
    }
    
    bool Prototypes::has(sTag name)
    {
        lock_guard<mutex> guard(m_mutex);
        return m_creators.find(name) != m_creators.end();
    }
    
    void Prototypes::getNames(vector<sTag>& names)
    {
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_creators.begin(); it !=  m_creators.end(); ++it)
        {
            names.push_back(it->first);
        }
    }
}
















