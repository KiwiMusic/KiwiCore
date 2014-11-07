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
    
    Instance::Instance() noexcept :
    m_dsp_running(0)
    {
        
    }
    
    Instance::~Instance()
    {
        m_pages.clear();
        m_listeners.clear();
    }
    
    bool libraries_loaded = false;
    
    shared_ptr<Instance> Instance::create()
    {
        if(!libraries_loaded)
        {
            arithmetic();
            libraries_loaded= true;
        }
        return make_shared<Instance>();
    }
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //

    sPage Instance::createPage(sDico dico)
    {
        sPage page = Page::create(shared_from_this(), dico);
        m_pages.insert(page);
        for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
            Instance::sListener listener = (*it).lock();
            if(listener)
            {
                listener->pageHasBeenCreated(shared_from_this(), page);
            }
        }
        return page;
    }
    
    void Instance::removePage(sPage page)
    {
        for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
            Instance::sListener listener = (*it).lock();
            if(listener)
            {
                listener->pageHasBeenRemoved(shared_from_this(), page);
            }
        }
        m_pages.erase(page);
    }
    
    void Instance::startDsp(double samplerate, long vectorsize)
    {
        m_dsp_running   = true;
        for(auto it = m_pages.begin(); it != m_pages.end(); ++it)
        {
            (*it)->startDsp(samplerate, vectorsize);
        }
    }
    
    inline void Instance::tickDsp() const noexcept
    {
        for(auto it = m_pages.begin(); it != m_pages.end(); ++it)
        {
            (*it)->tickDsp();
        }
    }
    
    void Instance::stopDsp()
    {
        for(auto it = m_pages.begin(); it != m_pages.end(); ++it)
        {
            (*it)->stopDsp();
        }
        m_dsp_running = false;
    }
    
    bool Instance::isDspRunning() const noexcept
    {
        return m_dsp_running;
    }
    
    void Instance::bind(weak_ptr<Instance::Listener> listener)
    {
        m_listeners.insert(listener);
    }
    
    void Instance::unbind(weak_ptr<Instance::Listener> listener)
    {
        m_listeners.erase(listener);
    }
}
















