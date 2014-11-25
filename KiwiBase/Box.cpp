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

#include "Box.h"
#include "Page.h"
#include "Instance.h"

namespace Kiwi
{
    map<sTag, unique_ptr<Box>>  Box::m_prototypes;
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    Box::Box(sPage page, string const& name,  unsigned long type) :
    m_instance(page ? page->m_instance : weak_ptr<Instance>()),
    m_page(page),
    m_name(Tag::create(name)),
    m_id(page ? page->m_boxe_id : 0),
    m_type(0 | type),
    m_stack_count(0)
    {
		addAttribute(Attr::create<AttrFont::Name>());
        addAttribute(Attr::create<AttrFont::Size>());
        addAttribute(Attr::create<AttrFont::Face>());
        addAttribute(Attr::create<AttrFont::Justification>());
        
        AttrAppearance::addAttributes(this);
		/*
		// Color attributes
		elems = {1., 1., 1, 1.};
		addAttribute<AttributePoint>(Tag::create("bgcolor"), elems, "Background Color", "Color");
		elems = {0., 0., 0, 1.};
		addAttribute<AttributePoint>(Tag::create("bdcolor"), elems, "Border Color", "Color");
		elems = {0., 0., 0, 1.};
		addAttribute<AttributePoint>(Tag::create("textcolor"), elems, "Text Color", "Color");
         */
    }
    
    Box::~Box()
    {
        m_outlets.clear();
        m_inlets.clear();
    }
    
    sBox Box::create(sPage page, sDico dico)
    {
        sTag name = dico->get(Tag::name);
        if(name)
        {
            lock_guard<mutex> guard(m_prototypes_mutex);
            auto it = m_prototypes.find(name);
            if(it != m_prototypes.end())
            {
                sTag text = dico->get(Tag::text);
                if(text)
                {
                    sDico other = Dico::create();
                    other->read(toString(text));
                    ElemVector keys;
                    other->keys(keys);
                    for(ElemVector::size_type i = 0; i < keys.size(); i++)
                    {
                        ElemVector values;
                        other->get(keys[i], values);
                        dico->set(keys[i], values);
                    }
                }
                
                sBox box = it->second->allocate(page, dico);
                if(box)
                {
                    box->m_text = dico->get(Tag::text);
                    box->load(dico);
					box->Attr::Manager::read(dico);
                    return box;
                }
            }
            else
            {
                Console::error("The box " + toString(name) + " doesn't exist !");
                return nullptr;
            }
        }
        Console::error("The dico isn't valid for box creation !");
        return nullptr;
    }
    
    void Box::write(sDico dico) const
    {
        this->save(dico);
        Attr::Manager::write(dico);
        dico->set(Tag::name, getName());
        dico->set(Tag::ninlets, getNumberOfInlets());
        dico->set(Tag::noutlets,getNumberOfOutlets());
        dico->set(Tag::text, getText());
        dico->set(Tag::id, getId());
    }
    
    void Box::redraw()
    {
        sControler controler = getControler();
        if(controler)
        {
            controler->redraw();
        }
    }
    
    void Box::send(unsigned long index, ElemVector const& elements) const noexcept
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_outlets.size())
        {
            for(vector<unique_ptr<Outlet>>::size_type i = 0; i < m_outlets[index]->m_sockets.size(); i++)
            {
                sBox receiver       = m_outlets[index]->m_sockets[i].box.lock();
                if(receiver)
                {
                    unsigned long inlet = m_outlets[index]->m_sockets[i].index;
                    if(++receiver->m_stack_count < 256)
                    {
                        if(!receiver->receive(inlet, elements))
                        {
                            if(!elements.empty() && elements[0].isTag())
                            {
                                ElemVector attrvec;
                                attrvec.assign(elements.begin()+1, elements.end());
                                if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                                {
                                    Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                                }
                            }
                            else
                            {
                                Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                            }
                        }
                    }
                    else if(receiver->m_stack_count  == 256)
                    {
                        Console::error(receiver, "Stack overflow");
                        if(!receiver->receive(inlet, elements))
                        {
                            if(!elements.empty() && elements[0].isTag())
                            {
                                ElemVector attrvec;
                                attrvec.assign(elements.begin()+1, elements.end());
                                if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                                {
                                    Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                                }
                            }
                            else
                            {
                                Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                            }
                        }
                    }
                    else
                    {
                        Console::error(receiver, "Stack overflow");
                    }
                    receiver->m_stack_count--;
                }
            }
        }
    }
    
    // ================================================================================ //
    //                                      INLETS                                      //
    // ================================================================================ //
    
    void Box::addInlet(Inlet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        m_inlets.push_back(unique_ptr<Inlet>(new Inlet(type, description)));
        redraw();
    }
    
    void Box::insertInlet(unsigned long index, Inlet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index >= m_inlets.size())
        {
            index = m_inlets.size();
            m_inlets.push_back(unique_ptr<Inlet>(new Inlet(type, description)));
        }
        else
        {
            m_inlets.insert(m_inlets.begin()+(long)index, unique_ptr<Inlet>(new Inlet(type, description)));
        }
        redraw();
    }
    
    void Box::removeInlet(unsigned long index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_inlets.size())
        {
            m_inlets.erase(m_inlets.begin()+(long)index);
            
            vector<sLink> links;
            sPage page = getPage();
            sBox me = getShared();
            if(page)
            {
                page->getLinks(links);
                for(vector<sLink>::size_type i = 0; i < links.size(); i++)
                {
                    if(links[i]->getBoxTo() == me && links[i]->getInletIndex() == index)
                    {
                        page->removeLink(links[i]);
                    }
                }
            }
            redraw();
        }
    }
    
    // ================================================================================ //
    //                                      OUTLETS                                     //
    // ================================================================================ //
    
    void Box::addOutlet(Outlet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        m_outlets.push_back(unique_ptr<Outlet>(new Outlet(type, description)));
        redraw();
    }
    
    void Box::insertOutlet(unsigned long index, Outlet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index >= m_outlets.size())
        {
            m_outlets.push_back(unique_ptr<Outlet>(new Outlet(type, description)));
        }
        else
        {
            m_outlets.insert(m_outlets.begin()+(long)index, unique_ptr<Outlet>(new Outlet(type, description)));
        }
        redraw();
    }
    
    void Box::removeOutlet(unsigned long index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_outlets.size())
        {
            m_outlets.erase(m_outlets.begin()+(long)index);
            
            vector<sLink> links;
            sPage page = getPage();
            sBox me = getShared();
            if(page)
            {
                page->getLinks(links);
                for(vector<sLink>::size_type i = 0; i < links.size(); i++)
                {
                    if(links[i]->getBoxFrom() == me && links[i]->getOutletIndex() == index)
                    {
                        page->removeLink(links[i]);
                    }
                }
            }
            redraw();
        }
        
    }
    
    bool Box::connectInlet(unsigned long inlet, sBox box, unsigned long outlet)
    {
        if(box)
        {
            if(inlet < getNumberOfInlets())
            {
                lock_guard<mutex> guard(m_io_mutex);
                for(vector<Socket>::size_type i = 0; i < m_inlets[inlet]->m_sockets.size(); i++)
                {
                    sBox sender  = m_inlets[inlet]->m_sockets[i].box.lock();
                    if(sender && sender == box && m_inlets[inlet]->m_sockets[i].index == outlet)
                    {
                        return false;
                    }
                }
                
                m_inlets[inlet]->m_sockets.push_back({box, outlet});
                return true;
            }
        }
        return false;
    }

    bool Box::connectOutlet(unsigned long outlet, sBox box, unsigned long inlet)
    {
        if(box)
        {
            if(outlet < getNumberOfOutlets())
            {
                lock_guard<mutex> guard(m_io_mutex);
                for(vector<Socket>::size_type i = 0; i < m_outlets[outlet]->m_sockets.size(); i++)
                {
                    sBox receiver  = m_outlets[outlet]->m_sockets[i].box.lock();
                    if(receiver && receiver == box && m_outlets[outlet]->m_sockets[i].index == inlet)
                    {
                        return false;
                    }
                }
                m_outlets[outlet]->m_sockets.push_back({box, inlet});
                return true;
            }
        }
        return false;
    }
    
    bool Box::disconnectInlet(unsigned long inlet, sBox box, unsigned long outlet)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(inlet < m_inlets.size())
            {
                for(vector<Socket>::size_type i = 0; i < m_inlets[inlet]->m_sockets.size(); i++)
                {
                    sBox sender  = m_inlets[inlet]->m_sockets[i].box.lock();
                    if(sender && sender == box && m_inlets[inlet]->m_sockets[i].index == outlet)
                    {
                        m_inlets[inlet]->m_sockets.erase(m_inlets[inlet]->m_sockets.begin()+(long)i);
                        return true;
                    }
                }
                return false;
            }
        }
        return false;
    }
    
    bool Box::disconnectOutlet(unsigned long outlet, sBox box, unsigned long inlet)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(outlet < m_outlets.size())
            {
                for(vector<Socket>::size_type i = 0; i < m_outlets[outlet]->m_sockets.size(); i++)
                {
                    sBox receiver  = m_outlets[outlet]->m_sockets[i].box.lock();
                    if(receiver && receiver == box && m_outlets[outlet]->m_sockets[i].index == inlet)
                    {
                        m_outlets[outlet]->m_sockets.erase(m_outlets[outlet]->m_sockets.begin()+(long)i);
                        return true;
                    }
                }
                return false;
            }
        }
        return false;
    }
    
    void Box::setControler(sControler ctrl)
    {
        m_controler = ctrl;
    }
    
    // ================================================================================ //
    //                                  BOX CONTROLER                                   //
    // ================================================================================ //
    
    void Box::Controler::setEditionStatus(bool status)
    {
        if(m_edition != status)
        {
            m_edition = status;
            redraw();
        }
    }
    
    void Box::Controler::setSelectedStatus(bool status)
    {
        if(m_selected != status)
        {
            m_selected = status;
            redraw();
        }
    }

#define KIO_HEIGHT 3.
#define KIO_WIDTH 5.
    
    Point Box::Controler::getInletPosition(unsigned long index) const noexcept
    {
        const unsigned long ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
    }
    
    Point Box::Controler::getOutletPosition(unsigned long index) const noexcept
    {
        const unsigned long ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
    }
    
    bool Box::Controler::isHit(Point const& pt, Hit& hit) const noexcept
    {
        const Rectangle bounds = m_box->getBounds();
        if(bounds.contains(pt))
        {
            if(pt.y() < bounds.y() + KIO_HEIGHT)
            {
                const unsigned long ninlets = m_box->getNumberOfInlets();
                if(ninlets && pt.x() <= bounds.x() + KIO_WIDTH)
                {
                    hit.type    = Inlet;
                    hit.index   = 0;
                    return true;
                }
                else if(ninlets > 1)
                {
                    const double ratio = (bounds.width() - KIO_WIDTH) / (double)(ninlets - 1);
                    for(unsigned long i = 1; i < ninlets; i++)
                    {
                        double val = ratio * i + bounds.x();
                        if(pt.x() >= val && pt.x() <= val + KIO_WIDTH)
                        {
                            hit.type    = Inlet;
                            hit.index   = i;
                            return true;
                        }
                    }
                }
                else
                {
                    hit.type    = Inside;
                    hit.index   = 0;
                    return true;
                }
            }
            else if(pt.y() > bounds.y() + bounds.height() - KIO_HEIGHT)
            {
                const unsigned long noutlets = m_box->getNumberOfOutlets();
                if(noutlets && pt.x() <= bounds.x() + KIO_WIDTH)
                {
                    hit.type    = Outlet;
                    hit.index   = 0;
                    return true;
                }
                else if(noutlets > 1)
                {
                    const double ratio = (bounds.width() - KIO_WIDTH) / (double)(noutlets - 1);
                    for(unsigned long i = 1; i < noutlets; i++)
                    {
                        double val = ratio * i + bounds.x();
                        if(pt.x() >= val && pt.x() <= val + KIO_WIDTH)
                        {
                            hit.type    = Outlet;
                            hit.index   = i;
                            return true;
                        }
                    }
                }
                else
                {
                    hit.type    = Inside;
                    hit.index   = 0;
                    return true;
                }
            }
            else
            {
                hit.type    = Inside;
                hit.index   = 0;
            }
            return true;
        }
        return false;
    }
    
    void Box::Controler::paint(sBox box, Doodle& d, bool edit, bool selected)
    {
        d.setFont(Font("Menelo", 13, Font::Normal));
        d.setColor(Color(1., 1., 1., 1.));
        d.fillRectangle(1., 1., d.getWidth() - 2., d.getHeight() - 2., 2.5);
        if(!(box->getType() & Behavior::Graphic))
        {
            d.setColor({0.3, 0.3, 0.3, 1.});
            d.drawText(toString(box->getText()), 3, 0, d.getWidth(), d.getHeight(), Font::Justification::CentredLeft);
        }
        else
        {
            box->draw(d);
        }
        d.setColor({0.4, 0.4, 0.4, 1.});
        d.drawRectangle(0.5, 0.5, d.getWidth()-1., d.getHeight()-1., 1., 2.5);
        
        if(edit)
        {
            const unsigned long ninlets = box->getNumberOfInlets();
            const unsigned long noutlets= box->getNumberOfOutlets();
            
            if(ninlets)
            {
                d.setColor({0.3, 0.3, 0.3, 1.});
                d.fillRectangle(0., 0., KIO_WIDTH, KIO_HEIGHT, 1.5);
            }
            if(ninlets > 1)
            {
                const double ratio = (d.getWidth() - KIO_WIDTH) / (double)(ninlets - 1);
                for(unsigned long i = 1; i < ninlets; i++)
                {
                    d.fillRectangle(ratio * i, 0., KIO_WIDTH, KIO_HEIGHT, 1.5);
                }
            }
            
            if(noutlets)
            {
                d.setColor({0.3, 0.3, 0.3, 1.});
                d.fillRectangle(0., d.getHeight() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT, 1.5);
            }
            if(noutlets > 1)
            {
                const double ratio = (d.getWidth() - KIO_WIDTH) / (double)(noutlets - 1);
                for(unsigned long i = 1; i < noutlets; i--)
                {
                    d.fillRectangle(ratio * i, d.getHeight() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT, 1.5);
                }
            }
        }
        
        if(selected)
        {
            d.setColor({0., 0.5, 0.75, 0.15});
            d.fillRectangle(0., 0., d.getWidth(), d.getHeight(), 2.5);
        }
    }
#undef KIO_HEIGHT
#undef KIO_WIDTH
    // ================================================================================ //
    //                                      BOX FACTORY                                 //
    // ================================================================================ //
    
    mutex Box::m_prototypes_mutex;
    
    void Box::addPrototype(unique_ptr<Box> box, string const& name)
    {
        sTag tname;
        if(name.empty())
        {
            tname = box->getName();
        }
        else
        {
            tname = Tag::create(name);
        }
        lock_guard<mutex> guard(m_prototypes_mutex);
        auto it = m_prototypes.find(tname);
        if(it != m_prototypes.end())
        {
            Console::error("The box " + toString(box->getName()) + " already exist !");
        }
        else
        {
            m_prototypes[tname] = move(box);
        }
    }
}
















