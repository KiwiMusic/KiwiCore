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
    const sTag Page::Tag_page       = Tag::create("page");
    const sTag Page::Tag_box        = Tag::create("box");
    const sTag Page::Tag_boxes      = Tag::create("boxes");
    const sTag Page::Tag_link       = Tag::create("link");
    const sTag Page::Tag_links      = Tag::create("links");
    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Page::Page(sInstance instance) :
    m_instance(instance)
    {
        ;
    }
    
    Page::~Page()
    {
        m_links.clear();
        m_boxes.clear();
        m_ctrls.clear();
    }
    
    sPage Page::create(sInstance instance, sDico dico)
    {
        sPage page = make_shared<Page>(instance);
        if(page && dico)
        {
            sDico pageDico = Dico::create();
            if(pageDico)
            {
                if(dico->has(Tag_page))
                {
                    pageDico = dico->get(Tag_page);
                    page->add(pageDico);
                    page->Attr::Manager::read(pageDico);
                }
            }
        }
        return page;
    }
    
    void Page::add(sDico dico)
    {
        if(dico)
        {
            map<ulong, ulong> m_ids_mapper;
            ElemVector boxes;
            dico->get(Tag_boxes, boxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                sDico subdico = boxes[i];
                if(subdico)
                {
                    sBox box;
                    subdico = subdico->get(Tag_box);
                    if(subdico)
                    {
                        lock_guard<mutex> guard(m_mutex);
                        
                        ulong _id;
                        for(_id = 1; _id <= m_boxes.size() + 1; _id++)
                        {
                            for(ulong j = 0; j < m_boxes.size(); j++)
                            {
                                if(m_boxes[j]->getId() == _id)
                                {
                                    break;
                                }
                            }
                        }
                        
                        subdico->set(Box::Tag_id, _id);
                        box = Box::create(getShared(), subdico);
                        if(box)
                        {
                            m_boxes.push_back(box);
                        }
                        
                        if(dico->has(Tag_links) && box && subdico->has(Box::Tag_id))
                        {
                            ulong _id = subdico->get(Box::Tag_id);
                            if(box->getId() != _id)
                            {
                                m_ids_mapper[_id] = box->getId();
                            }
                        }
                    }
                    send(box, Notification::Added);
                }
            }
            
            ElemVector links;
            dico->get(Tag_links, links);
            for(vector<sLink>::size_type i = 0; i < links.size(); i++)
            {
                sDico subdico = links[i];
                if(subdico)
                {
                    sLink link;
                    subdico = subdico->get(Tag_link);
                    if(subdico)
                    {
                        ElemVector elem;
                        subdico->get(Link::Tag_from, elem);
                        if(elem.size() == 2 && elem[0].isNumber() && elem[1].isNumber())
                        {
                            if(m_ids_mapper.find((ulong)elem[0]) != m_ids_mapper.end())
                            {
                                subdico->set(Link::Tag_from, {m_ids_mapper[(ulong)elem[0]], elem[1]});
                            }
                        }
                        subdico->get(Link::Tag_to, elem);
                        if(elem.size() == 2 && elem[0].isNumber() && elem[1].isNumber())
                        {
                            if(m_ids_mapper.find((ulong)elem[0]) != m_ids_mapper.end())
                            {
                                subdico->set(Link::Tag_to, {m_ids_mapper[(ulong)elem[0]], elem[1]});
                            }
                        }
                        
                        lock_guard<mutex> guard(m_mutex);
                        link = Link::create(getShared(), subdico);
                        if(link)
                        {
                            m_links.push_back(link);
                        }
                    }
                    send(link, Notification::Added);
                }
            }
        }
    }
    
    void Page::remove(sBox box)
    {
        vector<sLink> links;
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_boxes.begin(), m_boxes.end(), box);
            if(it != m_boxes.end())
            {
                auto li = find(m_links.begin(), m_links.end(), box);
                while(li != m_links.end())
                {
                    links.push_back((*li));
                    m_links.erase(li);
                    li = find(m_links.begin(), m_links.end(), box);
                }
                
                m_boxes.erase(it);
            }
        }
        for(vector<sLink>::size_type i = 0; i < links.size(); i++)
        {
            send(links[i], Notification::Removed);
        }
        send(box, Notification::Removed);
    }
    
    void Page::remove(sLink link)
    {
        if(link)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_links.begin(), m_links.end(), link);
            if(it != m_links.end())
            {
                m_links.erase(it);
            }
        }
        send(link, Notification::Removed);
    }
    
    sBox Page::replace(sBox oldbox, sDico dico)
    {
        sBox newbox;
        vector<sLink> oldlinks;
        vector<sLink> newlinks;
        if(oldbox && dico)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_boxes.begin(), m_boxes.end(), oldbox);
            if(it != m_boxes.end())
            {
                dico->set(Box::Tag_id, oldbox->getId());
                newbox = Box::create(getShared(), dico);
                if(newbox)
                {
                    (*it) = newbox;
                    auto li = find(m_links.begin(), m_links.end(), oldbox);
                    while(li != m_links.end())
                    {
                        oldlinks.push_back((*li));
                        (*li) = Link::create((*li), oldbox, newbox);
                        newlinks.push_back((*li));
                        li = find(m_links.begin(), m_links.end(), oldbox);
                    }
                }
            }
        }
        for(vector<sLink>::size_type i = 0; i < oldlinks.size(); i++)
        {
            send(oldlinks[i], Notification::Removed);
        }
        send(oldbox, Notification::Removed);
        send(newbox, Notification::Added);
        for(vector<sLink>::size_type i = 0; i < newlinks.size(); i++)
        {
            send(newlinks[i], Notification::Added);
        }
        return newbox;
    }
    
    void Page::toFront(sBox box)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_boxes.begin(), m_boxes.end(), box);
            if(it != m_boxes.end())
            {
                m_boxes.erase(it);
                m_boxes.push_back(box);
            }
        }
    }
    
    void Page::toBack(sBox box)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_boxes.begin(), m_boxes.end(), box);
            if(it != m_boxes.end())
            {
                m_boxes.erase(it);
                m_boxes.insert(m_boxes.begin(), box);
            }
        }
    }
	
    void Page::write(sDico dico) const
    {
        if(dico)
        {
			sDico subpage = Dico::create();
			if(subpage)
			{
				ElemVector elements;
				Attr::Manager::write(subpage);
				lock_guard<mutex> guard(m_mutex);
                
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
				subpage->set(Tag_boxes, elements);
				
				elements.clear();
				
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
				subpage->set(Tag_links, elements);
				dico->set(Tag_page, subpage);
			}
        }
    }
	
    void Page::dspStart(const ulong samplerate, const ulong vectorsize)
    {
        dspStop();
        m_dsp_context = Dsp::Context::create(samplerate, vectorsize);

        for(auto it = m_boxes.begin(); it != m_boxes.end(); ++it)
        {
            Dsp::sProcess process = dynamic_pointer_cast<Dsp::Process>((*it));
            if(process)
            {
                m_dsp_context->add(process);
            }
        }

        for(auto it = m_links.begin(); it != m_links.end(); ++it)
        {
            Dsp::sProcess from = dynamic_pointer_cast<Dsp::Process>((*it)->getBoxFrom());
            Dsp::sProcess to = dynamic_pointer_cast<Dsp::Process>((*it)->getBoxTo());
            if(from && to)
            {
                int zaza;
                Dsp::sConnection con = Dsp::Connection::create(from, (*it)->getOutletIndex(), to, (*it)->getInletIndex());
                m_dsp_context->add(con);
            } 
        }
        
        try
        {
            m_dsp_context->compile();
        }
        catch(Dsp::sProcess box)
        {
            Console::error(dynamic_pointer_cast<Box>(box), "something appened with me... sniff !");
            throw shared_from_this();
        }
    }
    
    void Page::dspStop()
    {
        if(m_dsp_context)
        {
            m_dsp_context->stop();
            m_dsp_context.reset();
        }
    }
	
	void Page::addController(sController ctrl)
	{
        lock_guard<mutex> guard(m_ctrls_mutex);
        m_ctrls.insert(ctrl);
	}
    
    void Page::removeController(sController ctrl)
    {
        lock_guard<mutex> guard(m_ctrls_mutex);
        m_ctrls.erase(ctrl);
    }
    
    void Page::send(sBox box, Page::Notification type)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_ctrls_mutex);
            for(auto it = m_ctrls.begin(); it != m_ctrls.end(); )
            {
                sController ctrl = (*it).lock();
                if(ctrl)
                {
                    if(type == Notification::Added)
                    {
                        ctrl->boxCreated(box);
                    }
                    else
                    {
                        ctrl->boxRemoved(box);
                    }
                    
                    ++it;
                }
                else
                {
                    it = m_ctrls.erase(it);
                }
            }
        }
    }
    
    void Page::send(sLink link, Page::Notification type)
    {
        if(link)
        {
            lock_guard<mutex> guard(m_ctrls_mutex);
            for(auto it = m_ctrls.begin(); it != m_ctrls.end(); )
            {
                sController ctrl = (*it).lock();
                if(ctrl)
                {
                    if(type == Notification::Added)
                    {
                        ctrl->linkCreated(link);
                    }
                    else
                    {
                        ctrl->linkRemoved(link);
                    }
                    
                    ++it;
                }
                else
                {
                    it = m_ctrls.erase(it);
                }
            }
        }
    }
    
    bool Page::attributeChanged(sAttr attr)
    {
        if(attr)
        {
            lock_guard<mutex> guard(m_ctrls_mutex);
            for(auto it = m_ctrls.begin(); it != m_ctrls.end(); )
            {
                sController ctrl = (*it).lock();
                if(ctrl)
                {
                    ctrl->attributeChanged(attr);
                    ++it;
                }
                else
                {
                    it = m_ctrls.erase(it);
                }
            }
            return true;
        }
        return false;
    }
}




