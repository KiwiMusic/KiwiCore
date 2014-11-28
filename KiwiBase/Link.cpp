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
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    Link::Link(const sBox from, const unsigned outlet, const sBox to, const unsigned inlet) noexcept :
    m_box_from(from), m_box_to(to), m_index_outlet(outlet), m_index_intlet(inlet)
    {
        if(from && to)
        {
            Box::sControler from_ctrl   = getBoxFrom()->getControler();
            Box::sControler to_ctrl     = getBoxTo()->getControler();
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
            dico->get(Tag::from, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                from_id = elements[0];
                outlet  = elements[1];
            }
            else
            {
                return nullptr;
            }
            
            dico->get(Tag::to, elements);
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
            dico->set(Tag::from, {from->getId(), getOutletIndex()});
            dico->set(Tag::to, {to->getId(), getInletIndex()});
        }
        else
        {
            dico->clear(Tag::from);
            dico->clear(Tag::to);
        }
    }
    
    void Link::inletChanged() noexcept
    {
        sBox from   = getBoxFrom();
        sBox to     = getBoxTo();
        if(from && to)
        {
            Box::sControler from_ctrl   = getBoxFrom()->getControler();
            Box::sControler to_ctrl     = getBoxTo()->getControler();
            if(from_ctrl && to_ctrl)
            {
                m_path.clear();
                m_path.add(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.add(to_ctrl->getInletPosition(getInletIndex()));
                
                sControler ctrl = getControler();
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
            Box::sControler from_ctrl   = getBoxFrom()->getControler();
            Box::sControler to_ctrl     = getBoxTo()->getControler();
            if(from_ctrl && to_ctrl)
            {
                m_path.clear();
                m_path.add(from_ctrl->getOutletPosition(getOutletIndex()));
                m_path.add(to_ctrl->getInletPosition(getInletIndex()));
                
                sControler ctrl = getControler();
                if(ctrl)
                {
                    ctrl->boundsChanged();
                }
            }
        }
    }
    
    void Link::setControler(sControler ctrl)
    {
        m_controler = ctrl;
    }
    
    // ================================================================================ //
    //                                  LINK CONTROLER                                  //
    // ================================================================================ //
    
    void Link::Controler::paint(sLink link, Doodle& d, bool selected)
    {
        
    }
}
















