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
                sBox newbox = Box::create(shared_from_this(), dico);
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
			
			unSelectAll();
			
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
	
	void Page::Controller::unSelectAll()
	{
		if(isSomethingSelected())
		{
			unSelectAllBoxes(false);
			unSelectAllLinks(false);
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
	
	bool Page::Controller::addAllLinksToSelection()
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
	
	
	bool Page::Controller::addAllBoxesToSelection()
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
	
	bool Page::Controller::unSelectAllBoxes(const bool notify)
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
	
	bool Page::Controller::unSelectAllLinks(const bool notify)
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
	
	bool Page::Controller::isBoxInSelection(Box::sController box)
	{
		return box && (m_boxes_selected.find(box) != m_boxes_selected.end());
	}
	
	bool Page::Controller::isLinkInSelection(Link::sController link)
	{
		return link && (m_links_selected.find(link) != m_links_selected.end());
	}
	
	bool Page::Controller::addBoxToSelection(Box::sController box)
	{
		if(box && m_boxes_selected.insert(box).second)
		{
			box->setSelectedStatus(true);
			selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::addLinkToSelection(Link::sController link)
	{
		if(link && m_links_selected.insert(link).second)
		{
			link->setSelectedStatus(true);
			selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::selectOnlyBox(Box::sController box)
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
	
	bool Page::Controller::selectOnlyLink(Link::sController link)
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
	
	bool Page::Controller::removeBoxFromSelection(Box::sController box)
	{
		if(box && m_boxes_selected.erase(box))
		{
			box->setSelectedStatus(false);
			selectionChanged();
			return true;
		}
		return false;
	}
	
	bool Page::Controller::removeLinkFromSelection(Link::sController link)
	{
		if(link && m_links_selected.erase(link))
		{
			link->setSelectedStatus(false);
			selectionChanged();
			return true;
		}
		return false;
	}
	
	void Page::Controller::addToSelectionBasedOnModifiers(Box::sController box, bool selOnly)
	{
		if (selOnly)
		{
			selectOnlyBox(box);
		}
		else if (isBoxInSelection(box))
		{
			removeBoxFromSelection(box);
		}
		else
		{
			addBoxToSelection(box);
		}
	}
	
	void Page::Controller::addToSelectionBasedOnModifiers(Link::sController link, bool selOnly)
	{
		if (selOnly)
		{
			selectOnlyLink(link);
		}
		else if (isLinkInSelection(link))
		{
			removeLinkFromSelection(link);
		}
		else
		{
			addLinkToSelection(link);
		}
	}
	
	bool Page::Controller::selectOnMouseDown(Box::sController box, bool selOnly)
	{
		/*
		 if (isBoxInSelection(box))
			return ! modifiers.isPopupMenu();
		 */
		
		if (isBoxInSelection(box))
			return true;
		
		addToSelectionBasedOnModifiers(box, selOnly);
		return false;
	}
	
	bool Page::Controller::selectOnMouseDown(Link::sController link, bool selOnly)
	{
		/*
		 if (isLinkInSelection(link))
			return ! modifiers.isPopupMenu();
		 */
		
		if (isLinkInSelection(link))
			return true;
		
		addToSelectionBasedOnModifiers(link, selOnly);
		return false;
	}
	
	void Page::Controller::selectOnMouseUp (Box::sController box, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if (resultOfMouseDownSelectMethod && ! boxWasDragged)
			addToSelectionBasedOnModifiers(box, selOnly);
	}
	
	
	void Page::Controller::selectOnMouseUp(Link::sController link, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if (resultOfMouseDownSelectMethod && ! boxWasDragged)
			addToSelectionBasedOnModifiers(link, selOnly);
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
	
	sTag positiona = Tag::create("position");
	
	void Page::Controller::dragSelectedBoxes(double dx, double dy)
	{
		unsigned int i = 0;
		for(auto it = m_boxes_selected.begin(); it != m_boxes_selected.end(); ++it)
		{
			Box::sController box = (*it).lock();
			if(box)
			{
				if(i < m_boxes_bounds.size())
				{
					ElemVector pos = {m_boxes_bounds[i].x() + dx, m_boxes_bounds[i].y() + dy};
					box->getBox()->setAttributeValue(positiona, pos);
				}
			}
			i++;
		}
	}
	
	void Page::Controller::mouseDown(const Event::Mouse& e)
	{
		m_box_is_dragging = false;
		
		m_templinks.clear();
		
		bool isShiftDown = (e.modifiers & Event::Mouse::Modifier::Shift);
		bool isCmdDown = (e.modifiers & Event::Mouse::Modifier::Cmd);
		
		Point pt(e.x, e.y);
		HitTest hit(getPage()->getController(), pt);
		
		if (hit.hasHittedBox())
		{
			Box::sController box = hit.getHittedBox();
			Box::Controller::Hit boxhit = hit.getBoxHit();
			
			if(boxhit.type == Box::Controller::Inlet)
			{
				m_templinks.push_back(TempLink(box->getBox(), pt, pt, boxhit.index, false));
				Console::post("templink created");
			}
			else if(boxhit.type == Box::Controller::Outlet)
			{
				m_templinks.push_back(TempLink(box->getBox(), pt, pt, boxhit.index, true));
				Console::post("templink created");
			}
			else if(boxhit.type == Box::Controller::Inside)
			{
				if (isCmdDown)
				{
					box->getBox()->receive(e);
				}
				else
				{
					m_mousedown_select_status = selectOnMouseDown(box, !isShiftDown);
					
					if(e.modifiers & Event::Mouse::Modifier::Popup)
					{
						// show box contextual menu
					}
				}
			}
		}
		else if(hit.hasHittedLink())
		{
			Console::post("link Hitted !!!");
			
		}
		else if(isCmdDown)
		{
			setLockStatus(!getLockStatus());
		}
		else if(!isShiftDown)
		{
			unSelectAll();
		}
		
		updateSelectedBoxesBounds();
	}
	
	void Page::Controller::mouseUp(const Event::Mouse& e)
	{
		bool isShiftDown = (e.modifiers & Event::Mouse::Modifier::Shift);
		HitTest hit(getPage()->getController(), Point(e.x, e.y));
		
		if (hit.hasHittedBox())
		{
			Box::sController box = hit.getHittedBox();
			Box::Controller::Hit boxhit = hit.getBoxHit();
			
			if(hasTempLinks())
			{
				TempLink templink = m_templinks[0];
				sBox from, to;
				long in, out;
				
				if(boxhit.type == Box::Controller::Inlet && templink.isAttachedToOutlet())
				{
					from = templink.getAttachedBox();
					to = box->getBox();
					in = boxhit.index;
					out = templink.getAttachedIOIndex();
				}
				else if(boxhit.type == Box::Controller::Outlet && !templink.isAttachedToOutlet())
				{
					from = box->getBox();
					to = templink.getAttachedBox();
					in = templink.getAttachedIOIndex();
					out = boxhit.index;
				}
				sLink link = Link::create(from, out, to, in);
				getPage()->addLink(link);
				
				Console::post("link created");
			}
			else if(boxhit.type == Box::Controller::Inside)
			{
				selectOnMouseUp(box, !isShiftDown, m_box_is_dragging, m_mousedown_select_status);
			}
		}
		
		if(hasTempLinks())
		{
			m_templinks.clear();
			Console::post("temp link deleted");
			redraw();
		}
	}
	
	void Page::Controller::mouseDrag(const Event::Mouse& e)
	{
		m_box_is_dragging = ! e.wasClicked;
		
		if(hasTempLinks())
		{
			HitTest hit(getPage()->getController(), Point(e.x, e.y));
			
			if (hit.hasHittedBox())
			{
				Box::sController box = hit.getHittedBox();
				Box::Controller::Hit boxhit = hit.getBoxHit();
				
				if(boxhit.type == Box::Controller::Inlet)
					mouseHoverIO(true, boxhit.index);
				
				else if(boxhit.type == Box::Controller::Outlet)
					mouseHoverIO(false, boxhit.index);
			}
			
			m_templinks[0].setEndCoord(Point(e.x, e.y));
			redraw();
			return;
		}
		else
		{
			if(!(e.modifiers & Event::Mouse::Modifier::Popup))
			{
				dragSelectedBoxes(e.x - e.down_x, e.y - e.down_y);
			}
		}
	}
	
	void Page::Controller::mouseMove(const Event::Mouse& e)
	{
		HitTest hit(getPage()->getController(), Point(e.x, e.y));
		
		if (hit.hasHittedBox())
		{
			Box::sController box = hit.getHittedBox();
			Box::Controller::Hit boxhit = hit.getBoxHit();
			
			if(boxhit.type == Box::Controller::Inlet)
				mouseHoverIO(true, boxhit.index);
				
			else if(boxhit.type == Box::Controller::Outlet)
				mouseHoverIO(false, boxhit.index);
		}
		else if(hit.hasHittedLink())
		{
			Console::post("link Hitted !!!");
		}
	}
	
	void Page::Controller::mouseDoubleClick(const Event::Mouse& e)
	{
		
	}
}
















