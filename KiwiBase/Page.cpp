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

#include "Page.h"
#include "Instance.h"
#include "../KiwiDsp/DspContext.h"

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Page::Page(sInstance instance) :
    m_instance(instance),
    m_dsp_context(make_shared<DspContext>()),
    m_boxes_id(1)
    {
        ;
    }
    
    Page::~Page()
    {
        m_links.clear();
        m_boxes.clear();
    }
    
    sBeacon Page::createBeacon(string const& name)
    {
        sInstance instance = m_instance.lock();
        if(instance)
        {
            return instance->createBeacon(name);
        }
        else
        {
            return nullptr;
        }
    }
    
    sPage Page::create(sInstance instance, scDico dico)
    {
        sPage page = make_shared<Page>(instance);
        if(page && dico)
        {
            page->read(dico);
        }
        return page;
    }
    
    sBox Page::createBox(sDico dico)
    {
        if(dico)
        {
            m_boxes_mutex.lock();
            m_boxes_id = m_boxes.size()+1;
            sBox box = Box::create(shared_from_this(), dico);
            if(box)
            {
                m_boxes.push_back(box);
                m_boxes_mutex.unlock();
                
				box->bind(shared_from_this());
                
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
                        Page::sListener listener = (*it).lock();
                        listener->boxHasBeenCreated(shared_from_this(), box);
                        ++it;
                    }
                }
                m_listeners_mutex.unlock();
                return box;
            }
            else
            {
                 m_boxes_mutex.unlock();
            }
        }
        return nullptr;
    }
    
    sBox Page::replaceBox(sBox oldbox, sDico dico)
    {
        m_boxes_mutex.lock();
        vector<sBox>::size_type position = find_position(m_boxes, oldbox);
        if(position != m_boxes.size())
        {
            if(dico)
            {
                m_boxes_id = oldbox->getId();
                sBox newbox = Box::create(shared_from_this(), dico);
                if(newbox)
                {
                    m_boxes[position] = newbox;
                    m_boxes_mutex.unlock();
                    
                    oldbox->unbind(shared_from_this());
                    newbox->bind(shared_from_this());
                
                    m_links_mutex.lock();
                    for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
                    {
                        sLink newconnect = Link::create(m_links[i], oldbox, newbox);
                        if(newconnect)
                        {
                            sLink oldconnect  = m_links[i];
                            m_links[i]        = newconnect;
                            
                            Box::disconnect(oldconnect);
                            
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
                                    Page::sListener listener = (*it).lock();
                                    listener->linkHasBeenReplaced(shared_from_this(), oldconnect, newconnect);
                                    ++it;
                                }
                            }
                            m_listeners_mutex.unlock();
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
                            Page::sListener listener = (*it).lock();
                            listener->boxHasBeenReplaced(shared_from_this(), oldbox, newbox);
                            ++it;
                        }
                    }
                    m_listeners_mutex.unlock();
                    m_links_mutex.unlock();
                }
            }
        }
        m_boxes_mutex.unlock();
        return nullptr;
    }
    
    void Page::removeBox(sBox box)
    {
        m_boxes_mutex.lock();
        vector<sBox>::size_type position = find_position(m_boxes, box);
        if(position != m_boxes.size())
        {
            m_links_mutex.lock();
            for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
            {
                if(m_links[i]->getBoxFrom() == box || m_links[i]->getBoxTo() == box)
                {
                    sLink oldlink = m_links[i];
                    Box::disconnect(oldlink);
                    m_links.erase(m_links.begin()+(long)i);
                    --i;
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
                            Page::sListener listener = (*it).lock();
                            listener->linkHasBeenRemoved(shared_from_this(), oldlink);
                            ++it;
                        }
                    }
                    m_listeners_mutex.unlock();
                }
            }
            m_links_mutex.unlock();
            
            m_boxes.erase(m_boxes.begin()+(long)position);
            m_boxes_mutex.unlock();
            box->unbind(shared_from_this());
            
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
                    Page::sListener listener = (*it).lock();
                    listener->boxHasBeenRemoved(shared_from_this(), box);
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
        else
        {
            m_boxes_mutex.unlock();
        }
    }
    
    sLink Page::addConnection(sLink link)
    {
        if(Box::connect(link))
        {
            m_links_mutex.lock();
            m_links.push_back(link);
            m_links_mutex.unlock();
            
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
                    Page::sListener listener = (*it).lock();
                    listener->linkHasBeenCreated(shared_from_this(), link);
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
            
            return link;
        }
        return nullptr;
    }
    
    sLink Page::createConnection(scDico dico)
    {
        if(dico)
        {
            sLink link = Link::create(shared_from_this(), dico);
            if(link)
            {
                return addConnection(link);
            }
        }
        return nullptr;
    }
    
    void Page::removeConnection(sLink link)
    {
        lock_guard<mutex> guard(m_links_mutex);
        vector<sLink>::size_type position = find_position(m_links, link);
        if(position != m_links.size())
        {
            Box::disconnect(m_links[position]);
            m_links.erase(m_links.begin()+(long)position);
            
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
                    Page::sListener listener = (*it).lock();
                    listener->linkHasBeenRemoved(shared_from_this(), link);
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
        }
    }
    
    void Page::append(scDico dico)
    {
        if(dico)
        {
            ElemVector boxes;
            dico->get(Tag::boxes, boxes);
            sort(m_boxes.begin(), m_boxes.end(), sortBoxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                sDico subdico = boxes[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag::box);
                    if(subdico)
                    {
                        createBox(subdico);
                    }
                }
            }
            
            ElemVector links;
            dico->get(Tag::links, links);
            for(vector<sLink>::size_type i = 0; i < links.size(); i++)
            {
                sDico subdico = links[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag::link);
                    if(subdico)
                    {
                        createConnection(subdico);
                    }
                }
            }
        }
    }
    
    void Page::read(scDico dico)
    {
        m_links.clear();
        m_boxes.clear();
        append(dico);
    }
    
    void Page::write(sDico dico) const
    {
        if(dico)
        {
            ElemVector elements;
            
            m_boxes_mutex.lock();
            for(vector<sBox>::size_type i = 0; i < m_boxes.size(); i++)
            {
                sDico box = Dico::create();
                sDico subbox = Dico::create();
                if(box && subbox)
                {
                    m_boxes[i]->write(subbox);
                    box->set(Tag::box, subbox);
                    elements.push_back(box);
                }
            }
            m_boxes_mutex.unlock();
            dico->set(Tag::boxes, elements);
            
            elements.clear();
      
            m_links_mutex.lock();
            for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
            {
                sDico link = Dico::create();
                sDico subconnect = Dico::create();
                if(link && subconnect && m_links[i]->isValid())
                {
                    m_links[i]->write(subconnect);
                    link->set(Tag::link, subconnect);
                    elements.push_back(link);
                }
            }
            m_links_mutex.unlock();
            dico->set(Tag::links, elements);
        }
    }
    
    bool Page::startDsp(unsigned long samplerate, unsigned long vectorsize)
    {
        m_dsp_context->clear();
        m_dsp_context->setSamplerate(samplerate);
        m_dsp_context->setVectorsize((long)vectorsize);
        
        for(auto it = m_boxes.begin(); it != m_boxes.end(); ++it)
        {
            m_dsp_context->addBox((*it));
        }
        
        for(auto it = m_links.begin(); it != m_links.end(); ++it)
        {
            m_dsp_context->addConnection((*it));
        }
        
        try
        {
            m_dsp_context->compile();
        }
        catch(sBox box)
        {
            Console::error(box, "something appened with me... sniff !");
        }
        return true;
    }
    
    void Page::tickDsp() const noexcept
    {
        m_dsp_context->tick();
    }
    
    void Page::stopDsp()
    {
        m_dsp_context->clear();
    }
    
    bool Page::isDspRunning() const noexcept
    {
        return m_dsp_running;
    }
    
    void Page::inletHasBeenCreated(sBox box, unsigned long index)
    {
        ;
    }
    
    void Page::inletHasBeenRemoved(sBox box, unsigned long index)
    {
        lock_guard<mutex> guard(m_links_mutex);
        vector<Link>::size_type max_size = m_links.size();
        for(vector<Link>::size_type i = 0; i < max_size;)
        {
            if(m_links[i]->getBoxTo() == box && m_links[i]->getInletIndex() == index)
            {
                sLink oldconnect = m_links[i];
                Box::disconnect(oldconnect);
                m_links.erase(m_links.begin()+(long)i);
                max_size--;
                
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
                        Page::sListener listener = (*it).lock();
                        listener->linkHasBeenRemoved(shared_from_this(), oldconnect);
                        ++it;
                    }
                }
                m_listeners_mutex.unlock();
            }
            else
            {
                i++;
            }
        }
    }
    
    void Page::outletHasBeenCreated(sBox box, unsigned long index)
    {
        ;
    }
    
    void Page::outletHasBeenRemoved(sBox box, unsigned long index)
    {
        lock_guard<mutex> guard(m_links_mutex);
        vector<Link>::size_type max_size = m_links.size();
        for(vector<Link>::size_type i = 0; i < max_size;)
        {
            if(m_links[i]->getBoxFrom() == box || m_links[i]->getOutletIndex() == index)
            {
                sLink oldconnect = m_links[i];
                Box::disconnect(oldconnect);
                m_links.erase(m_links.begin()+(long)i);
                max_size--;
                
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
                        Page::sListener listener = (*it).lock();
                        listener->linkHasBeenRemoved(shared_from_this(), oldconnect);
                        ++it;
                    }
                }
                m_listeners_mutex.unlock();
            }
            else
            {
                i++;
            }
        }
    }
    
    void Page::bind(shared_ptr<Page::Listener> listener)
    {
        lock_guard<mutex> guard(m_listeners_mutex);
        m_listeners.insert(listener);
    }
    
    void Page::unbind(shared_ptr<Page::Listener> listener)
    {
        lock_guard<mutex> guard(m_listeners_mutex);
        m_listeners.erase(listener);
    }
    
    bool Page::sortBoxes(Element first, Element second)
    {
        sDico firstdico = first;
        sDico seconddico = second;
        if(firstdico && seconddico)
        {
            firstdico = firstdico->get(Tag::box);
            seconddico = seconddico->get(Tag::box);
            if(firstdico && seconddico)
            {
                return (long)firstdico->get(Tag::id) < (long)seconddico->get(Tag::id);
            }
        }
        return false;
    }
    
    // ================================================================================ //
    //                                  PAGE CONTROLER                                  //
    // ================================================================================ //
    
    Page::Controler::Controler(sPage page) noexcept :
    m_page(page)
    {
        
    }
    
    Page::Controler::~Controler()
    {
        m_boxes.clear();
    }
    
    void Page::Controler::bind()
    {
        m_page->bind(shared_from_this());
    }
    
    void Page::Controler::unbind()
    {
        m_page->unbind(shared_from_this());
    }
}
















