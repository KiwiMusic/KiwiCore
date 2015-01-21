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
    
    Link::Link(sPage page, sBox from, ulong outlet, sBox to, ulong inlet) noexcept :
    m_page(page), m_box_from(from), m_box_to(to), m_index_outlet(outlet), m_index_intlet(inlet)
    {
        if(page)
        {
            if(from && outlet < from->getNumberOfOutlets() && to && inlet < to->getNumberOfInlets())
            {
                Box::sController from_ctrl   = from->getController();
                Box::sController to_ctrl     = to->getController();
                if(from_ctrl && to_ctrl)
                {
                    m_path.moveTo(from_ctrl->getOutletPosition(outlet));
                    m_path.lineTo(to_ctrl->getInletPosition(inlet));
                }
            }
            else if(from && outlet < from->getNumberOfOutlets())
            {
                Box::sController from_ctrl   = from->getController();
                if(from_ctrl)
                {
                    m_path.moveTo(from_ctrl->getOutletPosition(outlet));
                }
            }
            else if(to && inlet < to->getNumberOfInlets())
            {
                Box::sController to_ctrl   = to->getController();
                if(to_ctrl)
                {
                    m_path.moveTo(to_ctrl->getInletPosition(inlet));
                }
            }
        }
    }
    
    Link::~Link()
    {
        
    }
    
    sLink Link::create(sPage page, const sBox from, const unsigned outlet, const sBox to, const unsigned inlet)
    {
        if((from && outlet < from->getNumberOfOutlets()) || (to && inlet < to->getNumberOfInlets()))
        {
            sLink link = make_shared<Link>(page, from, outlet, to, inlet);
            if(link && from)
            {
                from->bind(link, Box::Tag_ninlets, Attr::ValueChanged);
                from->bind(link, Box::Tag_noutlets, Attr::ValueChanged);
                from->bind(link, Box::Tag_position, Attr::ValueChanged);
                from->bind(link, Box::Tag_size, Attr::ValueChanged);
                
            }
            if(link && to)
            {
                to->bind(link, Box::Tag_ninlets, Attr::ValueChanged);
                to->bind(link, Box::Tag_noutlets, Attr::ValueChanged);
                to->bind(link, Box::Tag_position, Attr::ValueChanged);
                to->bind(link, Box::Tag_size, Attr::ValueChanged);
            }
            return link;
        }
        else
        {
            return nullptr;
        }
    }
    
    sLink Link::create(sPage page, scDico dico)
    {
        if(page && dico)
        {
            sBox from, to;
            ulong outlet, inlet;
            ulong from_id, to_id;
            
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
                    return Link::create(page, from, outlet, to, inlet);
                }
            }
            
        }
        return nullptr;
    }
    
    sLink Link::create(scLink link, const sBox oldbox, const sBox newbox)
    {
        if(link)
        {
            sPage page = link->getPage();
            if(page && link->getBoxFrom() == oldbox)
            {
                if(link->getOutletIndex() < newbox->getNumberOfOutlets())
                {
                    return create(page, newbox, link->getOutletIndex(), link->getBoxTo(), link->getInletIndex());
                }
            }
            else if(page && link->getBoxTo() == oldbox)
            {
                if(link->getInletIndex() < newbox->getNumberOfInlets())
                {
                    return create(page, link->getBoxFrom(), link->getOutletIndex(), newbox, link->getInletIndex());
                }
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
            sOutlet outlet  = from->getOutlet(m_index_outlet);
            sInlet inlet    = to->getInlet(m_index_intlet);
            if(outlet && inlet)
            {
                if(outlet->append(to, m_index_outlet) && inlet->append(from, m_index_intlet))
                {
                    return true;
                }
                else
                {
                    outlet->erase(to, m_index_outlet);
                    inlet->erase(from, m_index_intlet);
                    return false;
                }
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
            sOutlet outlet  = from->getOutlet(m_index_outlet);
            sInlet inlet    = to->getInlet(m_index_intlet);
            if(outlet && inlet)
            {
                if(outlet->erase(to, m_index_outlet) && inlet->erase(from, m_index_intlet))
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
    
    void Link::setController(sController ctrl)
    {
        m_controller = ctrl;
    }
    
    void Link::notify(Attr::sManager manager, sAttr attr, Attr::Notification type)
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
                m_path.moveTo(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.lineTo(to_ctrl->getInletPosition(getInletIndex()));
                
                sController ctrl = getController();
                if(ctrl)
                {
                    ctrl->boundsChanged();
                }
            }
        }
    }
}
















