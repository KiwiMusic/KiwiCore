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
    bool libraries_loaded = false;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    Instance::Instance() noexcept :
    m_dsp_running(false),
    m_sample_rate(0),
    m_vector_size(0)
    {
        
    }
    
    Instance::~Instance()
    {
        m_pages.clear();
        m_listeners.clear();
        m_dsp_pages.clear();
    }
    
    sInstance Instance::create()
    {
        if(!libraries_loaded)
        {
            arithmetic();
            libraries_loaded = true;
        }
        return make_shared<Instance>();
    }
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //

    sPage Instance::createPage(sDico dico)
    {
        sPage page = Page::create(shared_from_this(), dico);
        if(page)
        {
            m_pages_mutex.lock();
            m_pages.insert(page);
            m_pages_mutex.unlock();
            
            if(m_dsp_running)
            {
                if(page->startDsp(m_sample_rate, m_vector_size))
                {
                    m_dsp_mutex.lock();
                    m_dsp_pages.push_back(page);
                    m_dsp_mutex.unlock();
                }
            }
            
            m_listeners_mutex.lock();
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                if((*it).expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->pageHasBeenCreated(shared_from_this(), page);
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
        return page;
    }
    
    void Instance::removePage(sPage page)
    {
        m_pages_mutex.lock();
        if(m_pages.find(page) != m_pages.end())
        {
            if(m_dsp_running && page->isDspRunning())
            {
                m_dsp_mutex.lock();
                page->stopDsp();
                auto it  = find(m_dsp_pages.begin(), m_dsp_pages.end(), page);
                if(it != m_dsp_pages.end())
                {
                    m_dsp_pages.erase(it);
                }
                m_dsp_mutex.unlock();
            }
            
            m_pages.erase(page);
            m_pages_mutex.unlock();
            
            m_listeners_mutex.lock();
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                if((*it).expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->pageHasBeenRemoved(shared_from_this(), page);
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
        else
        {
            m_pages_mutex.unlock();
        }
    }
    
    void Instance::getPages(vector<sPage>& pages)
    {
        lock_guard<mutex> guard(m_pages_mutex);
        pages.assign(m_pages.begin(), m_pages.end());
    }
    
    void Instance::startDsp(unsigned long samplerate, unsigned long vectorsize)
    {
        if(m_dsp_running)
        {
            stopDsp();
        }
        m_sample_rate   = samplerate;
        m_vector_size   = vectorsize;
        
        m_dsp_mutex.lock();
        m_pages_mutex.lock();
        for(auto it = m_pages.begin(); it != m_pages.end(); ++it)
        {
            if((*it)->startDsp(m_sample_rate, m_vector_size))
            {
                m_dsp_pages.push_back((*it));
            }
        }
        m_pages_mutex.unlock();
        
        if(!m_dsp_pages.empty())
        {
            m_dsp_running   = true;
            m_dsp_mutex.unlock();
            
            m_listeners_mutex.lock();
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                if((*it).expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->dspHasBeenStarted(shared_from_this());
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
        else
        {
            m_dsp_mutex.unlock();
        }
    }
    
    void Instance::stopDsp()
    {
        if(m_dsp_running)
        {
            m_dsp_mutex.lock();
            for(vector<sPage>::size_type i = 0; i < m_dsp_pages.size(); i++)
            {
                m_dsp_pages[i]->stopDsp();
            }
            m_dsp_pages.clear();
            m_dsp_mutex.unlock();
            
            m_dsp_running = false;
            
            m_listeners_mutex.lock();
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                if((*it).expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    Instance::sListener listener = (*it).lock();
                    listener->dspHasBeenStopped(shared_from_this());
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
    }    
    
    void Instance::bind(shared_ptr<Instance::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            m_listeners.insert(listener);
        }
    }
    
    void Instance::unbind(shared_ptr<Instance::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            m_listeners.erase(listener);
        }
    }
}
















