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
    
    Link::Link(const sPage page, const sBox from, const ulong outlet, const sBox to, const ulong inlet, const Box::Io::Type type) noexcept :
    m_page(page),
    m_box_from(from),
    m_box_to(to),
    m_index_outlet(outlet),
    m_index_intlet(inlet),
    m_type(type),
    m_attr_color_message(Attr::create<AttrColor>(Tag::List::mescolor,
                                                 Tag::List::Message_Color,
                                                 Tag::List::Color, (ElemVector){0.42, 0.42, 0.42, 1.},
                                                 m_type & Box::Io::Message ? 0 : Attr::All)),
    m_attr_color_signal(Attr::create<AttrColor>(Tag::List::sigcolor,
                                                Tag::List::Signal_Color,
                                                Tag::List::Color, (ElemVector){0.4, 0.4, 0.4, 1.},
                                                m_type & Box::Io::Signal ? 0 : Attr::All))
    {
        ;
    }
    
    Link::~Link()
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            Box::sOutlet outlet  = from->getOutlet(m_index_outlet);
            if(outlet)
            {
                outlet->erase(to, m_index_outlet);
            }
            Box::sInlet inlet    = to->getInlet(m_index_intlet);
            if(inlet)
            {
                inlet->erase(from, m_index_intlet);
            }
        }
    }
    
    sLink Link::create(sPage page, scDico dico)
    {
        if(page && dico)
        {
            sBox from, to;
            ulong outlet_index, inlet_index;
            ulong from_id, to_id;
            
            ElemVector elements;
            dico->get(Tag::List::from, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                from_id = elements[0];
                outlet_index  = elements[1];
            }
            else
            {
                return nullptr;
            }
            
            dico->get(Tag::List::to, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                to_id   = elements[0];
                inlet_index   = elements[1];
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
                        if(from->getNumberOfOutlets() <=  outlet_index)
                        {
                            return nullptr;
                        }
                    }
                    else if(boxes[i]->getId() == to_id)
                    {
                        to = boxes[i];
                        if(to->getNumberOfInlets() <=  inlet_index)
                        {
                            return nullptr;
                        }
                    }
                }
                
                if(from && to)
                {
                    Box::sOutlet outlet  = from->getOutlet(outlet_index);
                    Box::sInlet inlet    = to->getInlet(inlet_index);
                    if(outlet && inlet)
                    {
                        if(outlet->getType() != inlet->getType() && outlet->getType() < Box::Io::Both && inlet->getType() < Box::Io::Both)
                        {
                            return nullptr;
                        }
                        else
                        {
                            outlet->append(to, outlet_index);
                            inlet->append(from, inlet_index);
                            return make_shared<Link>(page, from, outlet_index, to, inlet_index, min(outlet->getType(), inlet->getType()));
                        }
                    } 
                }
            }
            
        }
        return nullptr;
    }
    
    void Link::write(sDico dico) const noexcept
    {
        sBox     from    = getBoxFrom();
        sBox     to      = getBoxTo();
        if(from && to)
        {
            dico->set(Tag::List::from, {from->getId(), getOutletIndex()});
            dico->set(Tag::List::to, {to->getId(), getInletIndex()});
        }
        else
        {
            dico->clear(Tag::List::from);
            dico->clear(Tag::List::to);
        }
    }
}
















