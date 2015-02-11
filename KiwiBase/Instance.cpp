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
#include "../KiwiObjectes/Wireless.h"
#include "../KiwiObjectes/Time.h"
#include "../KiwiObjectes/DspGenerator.h"
#include "../KiwiObjectes/NewObject.h"
#include "../KiwiObjectes/Arithmetic.h"
#include "../KiwiObjectes/ArithmeticTilde.h"
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
        m_patchers.clear();
        m_lists.clear();
        m_dsp_patchers.clear();
    }
    
    sInstance Instance::create()
    {
        if(!libraries_loaded)
        {
            guiInit();
            /*
			standardObjectes();
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
        sPatcher patcher = Patcher::create(shared_from_this(), dico);
        if(patcher)
        {
            m_patchers_mutex.lock();
            m_patchers.insert(patcher);
            m_patchers_mutex.unlock();
            
            if(m_dsp_running)
            {
                try
                {
                    int todo;
                    patcher->dspStart(m_sample_rate, m_vector_size);
                    m_dsp_mutex.lock();
                    m_dsp_patchers.push_back(patcher);
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
                    listener->patcherCreated(shared_from_this(), patcher);
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
            if(m_dsp_running && patcher->isDspRunning())
            {
                m_dsp_mutex.lock();
                patcher->dspStop();
                auto it  = find(m_dsp_patchers.begin(), m_dsp_patchers.end(), patcher);
                if(it != m_dsp_patchers.end())
                {
                    m_dsp_patchers.erase(it);
                }
                m_dsp_mutex.unlock();
            }
            
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
                    listener->patcherRemoved(shared_from_this(), patcher);
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
    
    void Instance::dspStart(ulong samplerate, ulong vectorsize)
    {
        if(m_dsp_running)
        {
            dspStop();
        }
        m_sample_rate   = samplerate;
        m_vector_size   = vectorsize;
        
        m_dsp_mutex.lock();
        m_patchers_mutex.lock();
        for(auto it = m_patchers.begin(); it != m_patchers.end(); ++it)
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
            m_dsp_patchers.push_back((*it));
        }
        m_patchers_mutex.unlock();
        
        if(!m_dsp_patchers.empty())
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
            for(vector<sPatcher>::size_type i = 0; i < m_dsp_patchers.size(); i++)
            {
                m_dsp_patchers[i]->dspStop();
            }
            m_dsp_patchers.clear();
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
            }
            return obj;
        }
        else
        {
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
















