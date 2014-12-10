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
    
    Knock::Knock() noexcept
    {
        ;
    }
    
    Knock::~Knock()
    {
        ;
    }
    
    void Knock::reset() noexcept
    {
        m_box.reset();
        m_link.reset();
        m_target= Nothing;
        m_part  = Outside;
        m_index = 0;
    }
    
    void Knock::process(sPage page, Point const& point) noexcept
    {
        reset();
        
        if(page)
        {
            Page::sController ctrl = page->getController();
            if(ctrl)
            {
                vector<Box::sController> boxes;
                ctrl->getBoxes(boxes);
                for(vector<Box::sController>::size_type i = boxes.size(); i; i--)
                {
                    int zaza;
                    Box::sController box = boxes[i-1];
                    if(box)// && box->isHit(point, this))
                    {
                        return;
                    }
                }
                
                vector<Link::sController> links;
                ctrl->getLinks(links);
                for(vector<Link::sController>::size_type i = links.size(); i; i--)
                {
                    int zaza;
                    Link::sController link = links[i-1];
                    if(link)// && link->isHit(point, this))
                    {
                        return;
                    }
                }
                
                m_target = Page;
                return;

            }
        }
    }
}
















