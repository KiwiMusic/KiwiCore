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

#include "Knock.h"
#include "Page.h"

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      KNOCK                                       //
    // ================================================================================ //
    
    Knock::Knock(sPage page) noexcept :
    m_page(page)
    {
        ;
    }
    
    Knock::~Knock()
    {
        ;
    }
    
    void Knock::knockReset() noexcept
    {
        m_box.reset();
        m_link.reset();
        m_target= Nothing;
        m_part  = Outside;
        m_index = 0;
    }
    
    void Knock::knockAll(Point const& point) noexcept
    {
        knockReset();
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Box::sController> boxes;
                ctrl->getBoxes(boxes);
                for(vector<Box::sController>::size_type i = boxes.size(); i; i--)
                {
                    Box::sController box = boxes[i-1];
                    if(box && box->contains(point, *this))
                    {
                        m_target = Box;
                        return;
                    }
                }
                
                vector<Link::sController> links;
                ctrl->getLinks(links);
                for(vector<Link::sController>::size_type i = links.size(); i; i--)
                {
                    Link::sController link = links[i-1];
                    if(link && link->contains(point, *this))
                    {
                        m_target = Link;
                        return;
                    }
                }
                
                m_target = Page;
                m_part   = Inside;
                return;

            }
        }
    }
    
    void Knock::knockBoxes(Point const& point) noexcept
    {
        knockReset();
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Box::sController> boxes;
                ctrl->getBoxes(boxes);
                for(vector<Box::sController>::size_type i = boxes.size(); i; i--)
                {
                    Box::sController box = boxes[i-1];
                    if(box && box->contains(point, *this))
                    {
                        return;
                    }
                }
            }
        }
    }
    
    void Knock::knockLinks(Point const& point) noexcept
    {
        knockReset();
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Link::sController> links;
                ctrl->getLinks(links);
                for(vector<Link::sController>::size_type i = links.size(); i; i--)
                {
                    Link::sController link = links[i-1];
                    if(link && link->contains(point, *this))
                    {
                        return;
                    }
                }
            }
        }
    }
    

    void Knock::knockAll(Rectangle const& rect, vector<Box::sController>& boxes, vector<Link::sController>& links) noexcept
    {
        knockBoxes(rect, boxes);
        knockLinks(rect, links);
    }
    
    void Knock::knockBoxes(Rectangle const& rect, vector<Box::sController>& boxes) noexcept
    {
        boxes.clear();
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Box::sController> vboxes;
                ctrl->getBoxes(vboxes);
                for(vector<Box::sController>::size_type i = 0; i < vboxes.size(); i++)
                {
                    if(vboxes[i] && vboxes[i]->overlaps(rect))
                    {
                        boxes.push_back(vboxes[i]);
                    }
                }
            }
        }
    }
    
    void Knock::knockLinks(Rectangle const& rect, vector<Link::sController>& links) noexcept
    {
        links.clear();
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Link::sController> vlinks;
                ctrl->getLinks(vlinks);
                for(vector<Link::sController>::size_type i = 0; i < vlinks.size(); i++)
                {
                    if(vlinks[i] && vlinks[i]->overlaps(rect))
                    {
                        links.push_back(vlinks[i]);
                    }
                }
            }
        }
    }
    
    // ================================================================================ //
    //										IOLET MAGNET                                //
    // ================================================================================ //
    
    IoletMagnet::IoletMagnet(sPage page) noexcept : m_page(page)
    {
        
    }
    
    IoletMagnet::~IoletMagnet()
    {
        
    }
    
    bool IoletMagnet::findIolet(Point const& point, sBox box, bool inlet, double const distance)
	{
        m_box.reset();
        sPage page = m_page.lock();
		if(page && box)
		{
            double ndistance = distance;
            vector<sBox> boxes;
            page->getBoxes(boxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                if(boxes[i] && boxes[i] != box)
                {
                    Box::sController boxctrl = boxes[i]->getController();
                    if(boxctrl && inlet)
                    {
                        for(unsigned long j = 0; j <  boxes[i]->getNumberOfInlets(); j++)
                        {
                            const double cdistance = point.distance(boxctrl->getInletPosition(j));
                            if(cdistance < ndistance)
                            {
                                m_box   = boxes[i];
                                m_index = j;
                                ndistance = cdistance;
                            }
                        }
                    }
                    else if(boxctrl)
                    {
                        for(unsigned long j = 0; j <  boxes[i]->getNumberOfOutlets(); j++)
                        {
                            const double cdistance = point.distance(boxctrl->getOutletPosition(j));
                            if(cdistance < ndistance)
                            {
                                m_box   = boxes[i];
                                m_index = j;
                                ndistance = cdistance;
                            }
                        }
                    }
                }
            }
		}
		return !m_box.expired();
	}
    
    // ================================================================================ //
    //										LASSO                                       //
    // ================================================================================ //
    
    Lasso::Lasso(sPage page) noexcept : m_page(page)
	{
        ;
	}
    
    Lasso::~Lasso()
    {
        m_boxes.clear();
		m_links.clear();
    }
    
	void Lasso::begin(Point const& point, bool preserve)
	{
        if(m_dragging)
        {
            m_boxes.clear();
            m_links.clear();
        }
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                m_preserve = preserve;
                if(!m_preserve)
                {
                    ctrl->unselectAll();
                }
                else
                {
                    ctrl->getSelection(m_boxes);
                    ctrl->getSelection(m_links);
                }
            }
        }
        m_start = point;
		m_bounds = Rectangle(m_start, Point(0., 0));
		m_dragging = true;
	}
    
	void Lasso::perform(Point const& point, bool boxes, bool links)
	{
        
        if(m_start.x() < point.x())
        {
            if(m_start.y() < point.y())
            {
                m_bounds = Rectangle(m_start, point - m_start);
            }
            else
            {
                m_bounds = Rectangle(m_start.x(), point.y(), point.x() - m_start.x(), m_start.y() - point.y());
            }
        }
        else
        {
            if(m_start.y() < point.y())
            {
                m_bounds = Rectangle(point.x(), m_start.y(), m_start.x() - point.x(), point.y() - m_start.y());
            }
            else
            {
                m_bounds = Rectangle(point, m_start - point);
            }
        }
		
        sPage page = m_page.lock();
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                if(m_preserve)
                {
                    if(boxes)
                    {
                        vector<Box::sController> nboxes;
                        ctrl->knockBoxes(m_bounds, nboxes);
                        for(vector<Box::sController>::size_type i = 0; i < nboxes.size(); i++)
                        {
                            if(m_boxes.find(nboxes[i]) != m_boxes.end())
                            {
                                ctrl->unselect(nboxes[i], false);
                            }
                            else
                            {
                                ctrl->select(nboxes[i], false);
                            }
                        }
                    }
                    if(links)
                    {
                        vector<Link::sController> nlinks;
                        ctrl->knockLinks(m_bounds, nlinks);
                        for(vector<Link::sController>::size_type i = 0; i < nlinks.size(); i++)
                        {
                            if(m_links.find(nlinks[i]) != m_links.end())
                            {
                                ctrl->unselect(nlinks[i], false);
                            }
                            else
                            {
                                ctrl->select(nlinks[i], false);
                            }
                        }
                    }
                    ctrl->selectionChanged();
                }
                else
                {
                    ctrl->unselectAll(false);
                    if(boxes)
                    {
                        vector<Box::sController> nBoxes;
                        ctrl->knockBoxes(m_bounds, nBoxes);
                        ctrl->select(nBoxes);
                    }
                    if(links)
                    {
                        vector<Link::sController> nLinks;
                        ctrl->knockLinks(m_bounds, nLinks);
                        ctrl->select(nLinks);
                    }
                }
            }
            boundsHasChanged();
        }
	}
    
	void Lasso::end()
	{
		m_dragging = false;
        m_boxes.clear();
		m_links.clear();
	}
	
	void Lasso::draw(Doodle &d)
	{
        d.setColor(Color(0.96, 0.96, 0.96, 0.5));
        d.fillAll();
        d.setColor(Color(0.96, 0.96, 0.96, 1.));
        d.drawRectangle(0., 0., d.getWidth(), d.getHeight(), 1.);
	}
    
    // ================================================================================ //
    //                                 IOLET HIGHLIGHTER                                //
    // ================================================================================ //
    
    IoletHighlighter::IoletHighlighter() noexcept
	{
		;
	}
    
    IoletHighlighter::~IoletHighlighter()
	{
		;
	}
	
	void IoletHighlighter::highlightInlet(sBox box, unsigned long index)
	{
		if(box)
		{
            Box::sController ctrl = box->getController();
			if(ctrl)
			{
				if(index < box->getNumberOfInlets())
				{
					const Point pos = ctrl->getInletPosition(index);
					m_polarity = box->getInletPolarity(index);
                    m_bounds = Rectangle(pos.x() - 8.,  pos.y() - 8., 16., 16.);
                    boundsHasChanged();
				}
			}
		}
	}
	
	void IoletHighlighter::highlightOutlet(sBox box, unsigned long index)
	{
		if(box)
		{
            Box::sController ctrl = box->getController();
			if(ctrl)
			{
				if(index < box->getNumberOfOutlets())
				{
					const Point pos = ctrl->getOutletPosition(index);
					m_polarity = IoPolarity::Hot;
                    m_bounds = Rectangle(pos.x() - 8.,  pos.y() - 8., 16., 16.);
                    boundsHasChanged();
				}
			}
		}
	}
	
	void IoletHighlighter::draw(Doodle &d)
	{
		const Point size = d.getSize();
        Color color;
        if(m_polarity == IoPolarity::Cold)
        {
            color = Color(0.28, 0.28, 0.88, 1);
        }
        else
        {
            color = Color(0.88, 0.28, 0.28, 1);
        }
		d.setColor(color.brighter(0.3));
        d.fillEllipse(4.5, 4.5, size.x() - 9., size.y() - 9.);
        d.setColor(color);
        d.drawEllipse(4.5, 4.5, size.x() - 9., size.y() - 9., 1.5);
	}
}
















