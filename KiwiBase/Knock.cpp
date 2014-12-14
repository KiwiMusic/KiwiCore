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
    //                                      INLET                                       //
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
}
















