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

#include "Link.h"
#include "Box.h"
#include "Page.h"

namespace Kiwi
{
    
    const sTag Link::Tag_from        = Tag::create("from");
    const sTag Link::Tag_to          = Tag::create("to");
    
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    Link::Link(const sBox from, const unsigned outlet, const sBox to, const unsigned inlet) noexcept :
    m_box_from(from), m_box_to(to), m_index_outlet(outlet), m_index_intlet(inlet)
    {
        if(from && to)
        {
            Box::sController from_ctrl   = getBoxFrom()->getController();
            Box::sController to_ctrl     = getBoxTo()->getController();
            if(from_ctrl && to_ctrl)
            {
                m_path.add(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.add(to_ctrl->getInletPosition(getInletIndex()));
            }
        }
    }
    
    Link::~Link()
    {
        
    }
    
    sLink Link::create(const sBox from, const unsigned outlet, const sBox to, const unsigned inlet)
    {
        return make_shared<Link>(from, outlet, to, inlet);
    }
    
    sLink Link::create(scPage page, scDico dico)
    {
        if(page && dico)
        {
            sBox from, to;
            unsigned long outlet, inlet;
            unsigned long from_id, to_id;
            
            ElemVector elements;
            dico->get(Tag_from, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                from_id = elements[0];
                outlet  = elements[1];
            }
            else
            {
                return nullptr;
            }
            
            dico->get(Tag_to, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                to_id   = elements[0];
                inlet   = elements[1];
            }
            else
            {
                return nullptr;
            }
        
            if(from_id != to_id)
            {
                vector<sBox> boxes;
                page->getBoxes(boxes);
                for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
                {
                    if(boxes[i]->getId() == from_id)
                    {
                        from = boxes[i];
                        if(from->getNumberOfOutlets() <=  outlet)
                        {
                            return nullptr;
                        }
                    }
                    else if(boxes[i]->getId() == to_id)
                    {
                        to = boxes[i];
                        if(to->getNumberOfInlets() <=  inlet)
                        {
                            return nullptr;
                        }
                    }
                }
                
                if(from && to)
                {
                    return Link::create(from, outlet, to, inlet);
                }
            }
            
        }
        return nullptr;
    }
    
    sLink Link::create(scLink link, const sBox oldbox, const sBox newbox)
    {
        if(link && link->getBoxFrom() == oldbox)
        {
            if(link->getOutletIndex() < newbox->getNumberOfOutlets())
            {
                return create(newbox, link->getOutletIndex(), link->getBoxTo(), link->getInletIndex());
            }
        }
        else if(link && link->getBoxTo() == oldbox)
        {
            if(link->getInletIndex() < newbox->getNumberOfInlets())
            {
                return create(link->getBoxFrom(), link->getOutletIndex(), newbox, link->getInletIndex());
            }
        }
        return nullptr;
    }
    
    bool Link::connect() noexcept
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            if(from->connectOutlet(shared_from_this()) && to->connectInlet(shared_from_this()))
            {
                return true;
            }
            else
            {
                from->disconnectOutlet(shared_from_this());
                to->disconnectInlet(shared_from_this());
                return false;
            }
        }
        return false;
    }
    

    bool Link::disconnect() noexcept
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            if(from->disconnectOutlet(shared_from_this()))
            {
                if(to->disconnectInlet(shared_from_this()))
                {
                   return true;
                }
            }
        }
        return false;
    }
    
    void Link::write(sDico dico) const noexcept
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            dico->set(Tag_from, {from->getId(), getOutletIndex()});
            dico->set(Tag_to, {to->getId(), getInletIndex()});
        }
        else
        {
            dico->clear(Tag_from);
            dico->clear(Tag_to);
        }
    }
    
    void Link::inletChanged() noexcept
    {
        sBox from   = getBoxFrom();
        sBox to     = getBoxTo();
        if(from && to)
        {
            Box::sController from_ctrl   = getBoxFrom()->getController();
            Box::sController to_ctrl     = getBoxTo()->getController();
            if(from_ctrl && to_ctrl)
            {
                m_path.clear();
                m_path.add(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.add(to_ctrl->getInletPosition(getInletIndex()));
                
                sController ctrl = getController();
                if(ctrl)
                {
                    ctrl->boundsChanged();
                }
            }
        }
    }
    
    void Link::outletChanged() noexcept
    {
        sBox from   = getBoxFrom();
        sBox to     = getBoxTo();
        if(from && to)
        {
            Box::sController from_ctrl   = getBoxFrom()->getController();
            Box::sController to_ctrl     = getBoxTo()->getController();
            if(from_ctrl && to_ctrl)
            {
                m_path.clear();
                m_path.add(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.add(to_ctrl->getInletPosition(getInletIndex()));
                
                sController ctrl = getController();
                if(ctrl)
                {
                    ctrl->boundsChanged();
                }
            }
        }
    }
    
    void Link::setController(sController ctrl)
    {
        m_controller = ctrl;
    }
    
    // ================================================================================ //
    //                                  LINK CONTROLER                                  //
    // ================================================================================ //
    
    void Link::Controller::paint(sLink link, Doodle& d, bool selected)
    {
        
    }
	
	void Link::Controller::setSelectedStatus(bool status)
	{
		if(m_selected != status)
		{
			m_selected = status;
			redraw();
		}
	}
    
    bool Link::compareBoxToPositions(sLink link1, sLink link2)
    {
        if(link1 && link2)
        {
            sBox box1 = link1->getBoxTo();
            sBox box2 = link2->getBoxTo();
            if(box1 && box2)
            {
                const Point pt1 = box1->getPosition();
                const Point pt2 = box2->getPosition();
                if(pt1.x() > pt2.x())
                {
                    return true;
                }
                else if(pt1.x() == pt2.x())
                {
                    return pt1.y() >= pt2.y();
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }
    
}
















