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

namespace Kiwi
{
    
    const sTag Page::Tag_box        = Tag::create("box");
    const sTag Page::Tag_boxes      = Tag::create("boxes");
    const sTag Page::Tag_link       = Tag::create("link");
    const sTag Page::Tag_links      = Tag::create("links");
    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Page::Page(sInstance instance) :
    m_instance(instance),
    m_dsp_context(nullptr),
    m_boxe_id(1)
    {
        ;
    }
    
    Page::~Page()
    {
        m_links.clear();
        m_boxes.clear();
    }
    
    sPage Page::create(sInstance instance, sDico dico)
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
            for(vector<sBox>::size_type i = 0; i < m_boxes.size(); i++)
            {
                if(m_boxe_id == m_boxes[i]->getId())
                {
                    m_boxe_id = m_boxes.size() + 1;
                    break;
                }
            }
            sBox box = Box::create(shared_from_this(), dico);
            m_boxe_id++;
            if(box)
            {
                m_boxes.push_back(box);
                m_boxes_mutex.unlock();
                
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
                unsigned long current_box_id = m_boxe_id;
                m_boxe_id = oldbox->getId();
                sBox newbox = Box::create(shared_from_this(), dico);
                m_boxe_id = current_box_id;
                if(newbox)
                {
                    m_boxes[position] = newbox;
                    m_boxes_mutex.unlock();
                
                    m_links_mutex.lock();
                    for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
                    {
                        sLink newconnect = Link::create(m_links[i], oldbox, newbox);
                        if(newconnect)
                        {
                            sLink oldlink  = m_links[i];
                            m_links[i]        = newconnect;
                            oldlink->disconnect();
                            
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
                                    listener->linkHasBeenReplaced(shared_from_this(), oldlink, newconnect);
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
                    oldlink->disconnect();
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
            m_boxe_id = box->getId();
            m_boxes_mutex.unlock();
            
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
    
    void Page::bringToFront(sBox box)
    {
        lock_guard<mutex> guard(m_boxes_mutex);
        vector<sBox>::size_type pos = find_position(m_boxes, box);
        if(pos != m_boxes.size())
        {
            m_boxes.erase(m_boxes.begin()+(long)pos);
            m_boxes.push_back(box);
        }
    }
    
    void Page::bringToBack(sBox box)
    {
        lock_guard<mutex> guard(m_boxes_mutex);
        vector<sBox>::size_type pos = find_position(m_boxes, box);
        if(pos != m_boxes.size())
        {
            m_boxes.erase(m_boxes.begin()+(long)pos);
            m_boxes.insert(m_boxes.begin(), box);
        }
    }
    
    sLink Page::addLink(sLink link)
    {
        if(link && link->connect())
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
    
    sLink Page::createLink(scDico dico)
    {
        if(dico)
        {
            sLink link = Link::create(shared_from_this(), dico);
            if(link)
            {
                return addLink(link);
            }
        }
        return nullptr;
    }
    
    void Page::removeLink(sLink link)
    {
        lock_guard<mutex> guard(m_links_mutex);
        vector<sLink>::size_type position = find_position(m_links, link);
        if(position != m_links.size())
        {
            m_links[position]->disconnect();
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
    
    void Page::append(sDico dico)
    {
        if(dico)
        {
            map<unsigned long, unsigned long> m_ids_mapper;
            ElemVector boxes;
            dico->get(Tag_boxes, boxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                sDico subdico = boxes[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag_box);
                    if(subdico)
                    {
                        sBox box = createBox(subdico);
                    
                        if(dico->has(Tag_links) && box && subdico->has(Box::Tag_id))
                        {
                            unsigned long _id = subdico->get(Box::Tag_id);
                            if(box->getId() != _id)
                            {
                                m_ids_mapper[_id] = box->getId();
                            }
                        }
                            
                    }
                }
            }
            
            ElemVector links;
            dico->get(Tag_links, links);
            for(vector<sLink>::size_type i = 0; i < links.size(); i++)
            {
                sDico subdico = links[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag_link);
                    if(subdico)
                    {
                        ElemVector elem;
                        subdico->get(Link::Tag_from, elem);
                        if(elem.size() == 2 && elem[0].isNumber() && elem[1].isNumber())
                        {
                            if(m_ids_mapper.find((unsigned long)elem[0]) != m_ids_mapper.end())
                            {
                                subdico->set(Link::Tag_from, {m_ids_mapper[(unsigned long)elem[0]], elem[1]});
                            }
                        }
                        subdico->get(Link::Tag_to, elem);
                        if(elem.size() == 2 && elem[0].isNumber() && elem[1].isNumber())
                        {
                            if(m_ids_mapper.find((unsigned long)elem[0]) != m_ids_mapper.end())
                            {
                                subdico->set(Link::Tag_to, {m_ids_mapper[(unsigned long)elem[0]], elem[1]});
                            }
                        }
                        createLink(subdico);
                    }
                }
            }
        }
    }
    
    void Page::read(sDico dico)
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
                    box->set(Tag_box, subbox);
                    elements.push_back(box);
                }
            }
            m_boxes_mutex.unlock();
            dico->set(Tag_boxes, elements);
            
            elements.clear();
      
            m_links_mutex.lock();
            for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
            {
                sDico link = Dico::create();
                sDico sublink = Dico::create();
                if(link && sublink)
                {
                    m_links[i]->write(sublink);
                    link->set(Tag_link, sublink);
                    elements.push_back(link);
                }
            }
            m_links_mutex.unlock();
            dico->set(Tag_links, elements);
        }
    }
    
    bool Page::startDsp(unsigned long samplerate, unsigned long vectorsize)
    {
        /*
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
        }*/
        return true;
    }
    
    void Page::tickDsp() const noexcept
    {
        //m_dsp_context->tick();
    }
    
    void Page::stopDsp()
    {
        //m_dsp_context->clear();
    }
    
    bool Page::isDspRunning() const noexcept
    {
        return m_dsp_running;
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
















