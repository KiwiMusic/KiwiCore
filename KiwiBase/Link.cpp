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
    
    sLink Link::create(const sSocket from, const sSocket to)
    {
        return make_shared<Link>(from, to);
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
                    return Link::create(make_shared<Socket>(from, outlet), make_shared<Socket>(to, inlet));
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
                return create(make_shared<Socket>(newbox, link->getOutletIndex()), link->getSocketTo());
            }
        }
        else if(link && link->getBoxTo() == oldbox)
        {
            if(link->getInletIndex() < newbox->getNumberOfInlets())
            {
                return create(link->getSocketFrom(), make_shared<Socket>(newbox, link->getInletIndex()));
            }
        }
        return nullptr;
    }

    bool Link::isConnectable() const noexcept
    {
        sBox from   = getBoxFrom();
        sBox to     = getBoxTo();
        if(from && to && from != to && from->getPage() == to->getPage() && getOutletIndex() < from->getNumberOfOutlets() && getInletIndex() < to->getNumberOfInlets())
        {
            vector<shared_ptr<Socket>> sockets;
            from->getOutletSockets(getOutletIndex(), sockets);
            for(vector<shared_ptr<Socket>>::size_type i = 0; i < sockets.size(); i++)
            {
                sBox receiver = sockets[i]->getBox();
                if(receiver && receiver == to && sockets[i]->getIndex() == getInletIndex())
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    
    bool Link::connect() const noexcept
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            if(from->connectOutlet(getOutletIndex(), m_to) && to->connectInlet(getInletIndex(), m_from))
            {
                return true;
            }
            else
            {
                from->disconnectOutlet(getOutletIndex(), m_from);
                to->disconnectInlet(getInletIndex(), m_from);
                return false;
            }
        }
        return false;
    }
    

    bool Link::disconnect() const noexcept
    {
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
    
    // ================================================================================ //
    //                                  LINK CONTROLER                                  //
    // ================================================================================ //
    
    void Link::Controler::attributeNotify(Attr::sManager manager, sAttr attr, Attr::Manager::Notification type)
    {
        if(manager == m_link->getBoxFrom())
        {
            m_start = m_link->getBoxFrom()->getControler()->getOutletPosition(m_link->getOutletIndex());
            Point bs = m_start;
            Point be = m_end;
            if(bs.x() > be.x())
            {
                bs.x(be.x());
                be.x(m_start.x());
            }
            if(bs.y() > be.y())
            {
                bs.y(be.y());
                be.y(m_start.y());
            }
            //setBounds(bs.x() - 1., bs.y() - 1., max(be.x() - bs.x() + 2., 2.), max(be.y() - bs.y() + 2., 2.));
        }
        if(manager == m_link->getBoxTo())
        {
            m_end   = m_link->getBoxTo()->getControler()->getInletPosition(m_link->getInletIndex());
            Point bs = m_start;
            Point be = m_end;
            if(bs.x() > be.x())
            {
                bs.x(be.x());
                be.x(m_start.x());
            }
            if(bs.y() > be.y())
            {
                bs.y(be.y());
                be.y(m_start.y());
            }
            bs.y(bs.y() - 10);
            be.y(be.y() + 10);
            //setBounds(bs.x() - 1., bs.y() - 1., max(be.x() - bs.x() + 2., 2.), max(be.y() - bs.y() + 2., 2.));
        }
    }
}
















