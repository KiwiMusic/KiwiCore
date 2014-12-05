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
            sBox box = Box::create(getShared(), dico);
            m_boxe_id++;
            if(box)
            {
                m_boxes.push_back(box);
				m_boxes_mutex.unlock();
				
				sController ctrl = getController();
				if (ctrl)
					ctrl->boxHasBeenCreated(box);
				
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
                sBox newbox = Box::create(getShared(), dico);
                m_boxe_id = current_box_id;
                if(newbox)
                {
                    m_boxes[position] = newbox;
                    m_boxes_mutex.unlock();
                
                    m_links_mutex.lock();
                    for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
                    {
                        sLink newlink = Link::create(m_links[i], oldbox, newbox);
                        if(newlink)
                        {
                            sLink oldlink  = m_links[i];
                            m_links[i]     = newlink;
                            oldlink->disconnect();
							
							sController ctrl = getController();
							if (ctrl)
								ctrl->linkHasBeenReplaced(oldlink, newlink);
                        }
                    }
					
					sController ctrl = getController();
					if (ctrl)
						ctrl->boxHasBeenReplaced(oldbox, newbox);
					
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
					
					sController ctrl = getController();
					if (ctrl)
						ctrl->linkHasBeenRemoved(oldlink);
                }
            }
            m_links_mutex.unlock();
            
            m_boxes.erase(m_boxes.begin()+(long)position);
            m_boxe_id = box->getId();
            m_boxes_mutex.unlock();
			
			sController ctrl = getController();
			if (ctrl)
				ctrl->boxHasBeenRemoved(box);
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
			
			sController ctrl = getController();
			if (ctrl)
				ctrl->linkHasBeenCreated(link);
            
            return link;
        }
        return nullptr;
    }
    
    sLink Page::createLink(scDico dico)
    {
        if(dico)
        {
            sLink link = Link::create(getShared(), dico);
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
			
			sController ctrl = getController();
			if (ctrl)
				ctrl->linkHasBeenRemoved(link);
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
	
	void Page::setController(sController ctrl)
	{
		m_controller = ctrl;
	}
    
    // ================================================================================ //
    //                                  PAGE CONTROLER                                  //
    // ================================================================================ //
    
    Page::Controller::Controller(sPage page) noexcept :
    m_page(page),
	m_zoom(100),
	m_locked(false),
	m_presentation(false),
	m_display_grid(true),
	m_snap_to_grid(false)
    {
        
    }
    
    Page::Controller::~Controller()
    {
        m_boxes.clear();
    }
	
	void Page::Controller::setZoom(long zoom)
	{
		m_zoom = clip(zoom, (long)1, (long)1000);
	}
	
	void Page::Controller::setLockStatus(bool locked)
	{
		if(m_locked != locked)
		{
			m_locked = locked;
			
			for(size_t i = 0; i < m_boxes.size(); i++)
				m_boxes[i]->setEditionStatus(!m_locked);
			
			unselectAll();
			
			lockStatusChanged();
		}
	}
	
	void Page::Controller::setPresentationStatus(bool presentation)
	{
		m_presentation = presentation;
	}
	
	void Page::Controller::setGridDisplayedStatus(bool display)
	{
		m_display_grid = display;
	}
	
	void Page::Controller::setSnapToGridStatus(bool snap)
	{
		m_snap_to_grid = snap;
	}
	
	void Page::Controller::addBoxController(Box::sController box)
	{
		m_boxes.push_back(box);
	}
	
	void Page::Controller::removeBoxController(Box::sController box)
	{
		for(vector<Box::sController>::size_type i = 0; i < m_boxes.size(); i++)
		{
			if(box == m_boxes[i])
			{
				m_boxes.erase(m_boxes.begin()+i);
			}
		}
	}
	
	Box::sController Page::Controller::getBoxController(sBox box) const noexcept
	{
		if(box)
		{
			for(vector<Box::sController>::size_type i = 0; i < m_boxes.size(); i++)
			{
				if(box == m_boxes[i]->getBox())
				{
					return m_boxes[i];
				}
			}
		}
		
		return nullptr;
	}
	
	Box::sController Page::Controller::getBoxController(Point const& pt) const noexcept
	{
		Box::Controller::Hit hit;
		for(size_t i = m_boxes.size(); i; i--)
		{
			if(m_boxes[i-1]->isHit(pt, hit))
			{
				return m_boxes[i-1];
			}
		}
		return nullptr;
	}
	
	void Page::Controller::addLinkController(Link::sController link)
	{
		m_links.push_back(link);
	}
	
	void Page::Controller::removeLinkController(Link::sController link)
	{
		for(vector<Link::sController>::size_type i = 0; i < m_links.size(); i++)
		{
			if(link == m_links[i])
			{
				m_links.erase(m_links.begin()+i);
			}
		}
	}
	
	Link::sController Page::Controller::getLinkController(sLink link) const noexcept
	{
		if(link)
		{
			for(vector<Link::sController>::size_type i = 0; i < m_links.size(); i++)
			{
				if(link == m_links[i]->getLink())
				{
					return m_links[i];
				}
			}
		}
		
		return nullptr;
	}
	
	void Page::Controller::getSelection(vector<Box::sController>& boxes, vector<Link::sController>& links) const noexcept
	{
		boxes.clear();
		for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
			if (Box::sController box = (*it).lock())
				boxes.push_back(box);
				
		links.clear();
		for(auto it = m_links_selected.begin(); it != m_links_selected.end(); ++it)
			if (Link::sController link = (*it).lock())
				links.push_back(link);
	}
	
	void Page::Controller::unselectAll(const bool notify)
	{
		if(isSomethingSelected())
		{
			unselectAllBoxes(false);
			unselectAllLinks(false);
			
			if (notify)
				selectionChanged();
		}
	}
	
	void Page::Controller::deleteSelection()
	{
		bool sendChange = false;
		for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
		{
			Box::sController box = (*it).lock();
			if (box)
			{
				getPage()->removeBox(box->getBox());
				sendChange = true;
			}
		}
		
		m_boxes_selected.clear();
		
		for(auto it = m_links_selected.begin(); it != m_links_selected.end(); ++it)
		{
			Link::sController link = (*it).lock();
			if (link)
			{
				getPage()->removeLink(link->getLink());
				sendChange = true;
			}
		}
		
		m_links_selected.clear();
		
		if (sendChange)
			selectionChanged();
	}
	
	bool Page::Controller::selectAllLinks()
	{
		if(m_links_selected.size() != m_links.size())
		{
			for(vector<Link::sController>::size_type i = 0; i < m_links.size(); i++)
			{
				if(m_links_selected.insert(m_links[i]).second)
				{
					m_links[i]->setSelectedStatus(true);
				}
			}
			selectionChanged();
			return true;
		}
		return false;
	}
	
	
	bool Page::Controller::selectAllBoxes()
	{
		if(m_boxes_selected.size() != m_boxes.size())
		{
			for(vector<Box::sController>::size_type i = 0; i < m_boxes.size(); i++)
			{
				if(m_boxes_selected.insert(m_boxes[i]).second)
				{
					m_boxes[i]->setSelectedStatus(true);
				}
			}
			selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::unselectAllBoxes(const bool notify)
	{
		bool sendChange = false;
		
		if(!m_boxes_selected.empty())
		{
			for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
			{
				Box::sController box = (*it).lock();
				if(box)
				{
					box->setSelectedStatus(false);
					sendChange = true;
				}
			}
			m_boxes_selected.clear();
			
			if (notify && sendChange)
				selectionChanged();
			
			return true;
		}
		return false;
	}
	
	bool Page::Controller::unselectAllLinks(const bool notify)
	{
		bool sendChange = false;
		
		if(!m_links_selected.empty())
		{
			for(auto it = m_links_selected.begin(); it != m_links_selected.end(); ++it)
			{
				Link::sController link = (*it).lock();
				if(link)
				{
					link->setSelectedStatus(false);
					sendChange = true;
				}
			}
			m_links_selected.clear();
			
			if (notify && sendChange)
				selectionChanged();
			
			return true;
		}
		return false;
	}
	
	bool Page::Controller::isSelected(Box::sController box)
	{
		return box && (m_boxes_selected.find(box) != m_boxes_selected.end());
	}
	
	bool Page::Controller::isSelected(Link::sController link)
	{
		return link && (m_links_selected.find(link) != m_links_selected.end());
	}
	
	void Page::Controller::select(vector<Box::sController>& boxes)
	{
		bool notify = false;
		if(!boxes.empty())
		{
			for (auto it = boxes.begin() ; it != boxes.end(); ++it)
			{
				Box::sController box = *it;
				
				if (!isSelected(box))
				{
					select(box, false);
					notify = true;
				}
			}
			
			if (notify)
				selectionChanged();
		}
	}
	
	void Page::Controller::select(vector<Link::sController>& links)
	{
		bool notify = false;
		if(!links.empty())
		{
			for (auto it = links.begin() ; it != links.end(); ++it)
			{
				Link::sController link = *it;
				
				if (!isSelected(link))
				{
					select(link, false);
					notify = true;
				}
			}
			
			if (notify)
				selectionChanged();
		}
	}
	
	bool Page::Controller::select(Box::sController box, const bool notify)
	{
		if(box && m_boxes_selected.insert(box).second)
		{
			box->setSelectedStatus(true);
			if (notify)
				selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::select(Link::sController link, const bool notify)
	{
		if(link && m_links_selected.insert(link).second)
		{
			link->setSelectedStatus(true);
			if (notify)
				selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::selectOnly(Box::sController box)
	{
		if (box)
		{
			if(!m_boxes_selected.empty())
			{
				for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
				{
					Box::sController box2 = (*it).lock();
					if(box2)
					{
						box2->setSelectedStatus(false);
					}
				}
				m_boxes_selected.clear();
			}
			
			if(m_boxes_selected.insert(box).second)
			{
				box->setSelectedStatus(true);
				return true;
			}
			
			selectionChanged();
		}
		
		return false;
	}
	
	bool Page::Controller::selectOnly(Link::sController link)
	{
		if (link)
		{
			if(!m_links_selected.empty())
			{
				for(auto it = m_links_selected.begin(); it != m_links_selected.end(); ++it)
				{
					Link::sController link2 = (*it).lock();
					if(link2)
					{
						link2->setSelectedStatus(false);
					}
				}
				m_links_selected.clear();
			}
			
			if(m_links_selected.insert(link).second)
			{
				link->setSelectedStatus(true);
				return true;
			}
			
			selectionChanged();
		}
		
		return false;
	}
	
	void Page::Controller::unselect(vector<Box::sController>& boxes)
	{
		bool notify = false;
		if(isSomethingSelected() && !boxes.empty())
		{
			for (auto it = boxes.begin() ; it != boxes.end(); ++it)
			{
				Box::sController box = *it;
				
				if (isSelected(box))
				{
					unselect(box, false);
					notify = true;
				}
			}
			
			if (notify)
				selectionChanged();
		}
	}
	
	void Page::Controller::unselect(vector<Link::sController>& links)
	{
		bool notify = false;
		if(isSomethingSelected() && !links.empty())
		{
			for (auto it = links.begin() ; it != links.end(); ++it)
			{
				Link::sController link = *it;
				
				if (isSelected(link))
				{
					unselect(link, false);
					notify = true;
				}
			}
			
			if (notify)
				selectionChanged();
		}
	}
	
	bool Page::Controller::unselect(Box::sController box, const bool notify)
	{
		if(box && m_boxes_selected.erase(box))
		{
			box->setSelectedStatus(false);
			
			if(notify)
				selectionChanged();
			
			return true;
		}
		return false;
	}
	
	bool Page::Controller::unselect(Link::sController link, const bool notify)
	{
		if(link && m_links_selected.erase(link))
		{
			link->setSelectedStatus(false);
			selectionChanged();
			return true;
		}
		return false;
	}
	
	void Page::Controller::updateSelectedBoxesBounds()
	{
		m_boxes_bounds.clear();
		
		for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
		{
			Box::sController jbox = (*it).lock();
			if(jbox)
			{
				m_boxes_bounds.push_back(jbox->getBox()->getBounds());
			}
		}
	}
	
	Rectangle Page::Controller::getSelectionBounds()
	{
		// to do !
		return Rectangle();
	}
	
	void Page::Controller::getBoxesInRect(vector<Box::sController>& boxes, Rectangle const& rect) const noexcept
	{
		boxes.clear();
		
		for(int i=0; i < m_boxes.size(); i++)
		{
			sBox box = m_boxes[i]->getBox();
			if(box)
			{
				const Rectangle boxBounds = box->getBounds();
				if(rect.overlaps(boxBounds))
				{
					boxes.push_back(m_boxes[i]);
				}
			}
		}
	}
	
	void Page::Controller::moveSelectedBoxes(Point const& delta)
	{
		if (isAnyBoxSelected())
		{
			unsigned int i = 0;
			for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
			{
				Box::sController box = (*it).lock();
				if(box)
				{
					Point boxpos = box->getBox()->getPosition();
					boxpos += delta;
					box->getBox()->setAttributeValue(AttrBox::Tag_position, {boxpos.x(), boxpos.y()});
				}
				i++;
			}
		}
	}
	
	void Page::Controller::getSelectedBoxesDico(sDico dico)
	{
		string text;
		if (isAnyBoxSelected())
		{
			ElemVector elements;
			vector<sBox> boxes;
			unsigned int i = 0;
			for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
			{
				Box::sController boxctrl = (*it).lock();
				if(boxctrl)
				{
					sBox box = boxctrl->getBox();
					if (box)
					{
						sDico boxdico = Dico::create();
						sDico subbox = Dico::create();
						if(boxdico && subbox)
						{
							box->write(subbox);
							boxdico->set(Tag_box, subbox);
							elements.push_back(boxdico);
							boxes.push_back(box);
						}
					}
				}
				i++;
			}
			dico->set(Tag_boxes, elements);
			
			elements.clear();
			
			for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
			{
				sDico linkdico = Dico::create();
				sDico sublinkdico = Dico::create();
				if(linkdico && sublinkdico)
				{
					sLink link = m_links[i]->getLink();
					if (link)
					{
						sBox boxFrom = link->getBoxFrom();
						sBox boxTo = link->getBoxTo();
						if (find(boxes.begin(), boxes.end(), boxFrom) != boxes.end() &&
							find(boxes.begin(), boxes.end(), boxTo) != boxes.end())
						{
							link->write(sublinkdico);
							linkdico->set(Tag_link, sublinkdico);
							elements.push_back(linkdico);
						}
					}
				}
			}
			dico->set(Tag_links, elements);
			boxes.clear();
		}
	}
	
	bool Page::Controller::addBoxesFromDico(sDico dico, Point const& shift)
	{
		sPage page = getPage();
		bool pageModified = false;
		if (page)
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
						sBox box = page->createBox(subdico);
						if (box)
						{
							const Point pos = box->getPosition();
							box->setAttributeValue(AttrBox::Tag_position, {pos.x() + shift.x(), pos.y() + shift.y()});
							
							if(dico->has(Tag_links) && box && subdico->has(Box::Tag_id))
							{
								unsigned long _id = subdico->get(Box::Tag_id);
								if(box->getId() != _id)
								{
									m_ids_mapper[_id] = box->getId();
								}
							}
							
							pageModified = true;
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
						page->createLink(subdico);
					}
				}
			}
		}
		
		return pageModified;
	}
}




