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
    
    const sTag Box::Tag_arguments   = Tag::create("arguments");
    const sTag Box::Tag_bang        = Tag::create("bang");
    const sTag Box::Tag_id          = Tag::create("id");
    const sTag Box::Tag_focus       = Tag::create("focus");
    const sTag Box::Tag_name        = Tag::create("name");
    const sTag Box::Tag_set         = Tag::create("set");
    const sTag Box::Tag_text        = Tag::create("text");
    
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
        ;
    }
    
    Box::~Box()
    {
        m_outlets.clear();
        m_inlets.clear();
    }
    
    sBox Box::create(sPage page, sDico dico)
    {
        sTag name = dico->get(Tag_name);
        if(name)
        {
            lock_guard<mutex> guard(m_prototypes_mutex);
            auto it = m_prototypes.find(name);
            if(it != m_prototypes.end())
            {
                sTag text = dico->get(Tag_text);
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
                    box->m_text = dico->get(Tag_text);
                    box->load(dico);
					box->Attr::Manager::read(dico);
                    if(!dico->has(Tag_size) && !(box->getType() & Graphic))
                    {
                        Point size = Text::getStringSize(box->getFont(), toString(box->getText()));
                        box->setAttributeValue(Tag_size, {max(ceil(size.x()) + 6., 25.), box->getSize().y()});
                    }
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
        dico->set(Tag_name, getName());
        dico->set(Tag_ninlets, getNumberOfInlets());
        dico->set(Tag_noutlets,getNumberOfOutlets());
        dico->set(Tag_text, getText());
        dico->set(Tag_id, getId());
    }
    
    void Box::redraw() const noexcept
    {
        sController controller = getController();
        if(controller)
        {
            controller->redraw();
        }
    }
    
    void Box::grabKeyboardFocus() const noexcept
    {
        sController controller = getController();
        if(controller)
        {
            controller->grabKeyboardFocus();
        }
    }
    
    void Box::send(unsigned long index, ElemVector const& elements) const noexcept
    {
        m_io_mutex.lock();
        if(index < m_outlets.size())
        {
            for(unsigned long i = 0; i < m_outlets[index]->getNumberOfLinks(); i++)
            {
                sBox receiver       = m_outlets[index]->getBox(i);
                unsigned long inlet = m_outlets[index]->getInletIndex(i);
                m_io_mutex.unlock();
                if(receiver)
                {
                    if(++receiver->m_stack_count < 256)
                    {
                        if(!elements.empty() && elements[0].isTag() && toString(elements[0])[0] == '@')
                        {
                            ElemVector attrvec;
                            attrvec.assign(elements.begin()+1, elements.end());
                            if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                            {
                                Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                            }
                        }
                        else if(!receiver->receive(inlet, elements))
                        {
                            Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                        }
                    }
                    else if(receiver->m_stack_count  == 256)
                    {
                        Console::error(receiver, "Stack overflow");
                        if(!elements.empty() && elements[0].isTag() && toString(elements[0])[0] == '@')
                        {
                            ElemVector attrvec;
                            attrvec.assign(elements.begin()+1, elements.end());
                            if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                            {
                                Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                            }
                        }
                        else if(!receiver->receive(inlet, elements))
                        {
                            Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                        }
                    }
                    else
                    {
                        Console::error(receiver, "Stack overflow");
                    }
                    receiver->m_stack_count--;
                }
                m_io_mutex.unlock();
            }
        }
        m_io_mutex.unlock();
    }
	
	bool Box::attributeValueChanged(sAttr attr)
	{
        if(!this->attributeChanged(attr))
        {
            return false;
        }
        if(attr == AttrBox::appearance_position)
        {
            sController controller = getController();
            if(controller)
            {
                controller->positionChanged();
            }
        }
        else if(attr == AttrBox::appearance_size)
        {
            sController controller = getController();
            if(controller)
            {
                controller->sizeChanged();
            }
        }
        else if(attr == AttrBox::color_background || AttrBox::color_border || AttrBox::color_text)
        {
            sController controller = getController();
            if(controller)
            {
                controller->redraw();
            }
        }
		return true;
	}
    
    // ================================================================================ //
    //                                      INLETS                                      //
    // ================================================================================ //
    
    void Box::addInlet(IoType type, IoPolarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        m_inlets.push_back(unique_ptr<Inlet>(new Inlet(type, polarity, description)));
        sController controller = getController();
        if(controller)
        {
            controller->inletsChanged();
        }
        setAttributeValue(Tag_ninlets, {m_inlets.size()});
    }
    
    void Box::insertInlet(unsigned long index, IoType type, IoPolarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index >= m_inlets.size())
        {
            index = m_inlets.size();
            m_inlets.push_back(unique_ptr<Inlet>(new Inlet(type, polarity, description)));
        }
        else
        {
            m_inlets.insert(m_inlets.begin()+(long)index, unique_ptr<Inlet>(new Inlet(type, polarity, description)));
        }
        
        sController controller = getController();
        if(controller)
        {
            controller->inletsChanged();
        }
        setAttributeValue(Tag_ninlets, {m_inlets.size()});
    }
    
    void Box::removeInlet(unsigned long index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_inlets.size())
        {
            sPage page = getPage();
            if(page)
            {
                for(unsigned long i = 0; i < m_inlets[index]->getNumberOfLinks(); i++)
                {
                    page->removeLink(m_inlets[index]->getLink(i));
                }
            }
            m_inlets.erase(m_inlets.begin()+(long)index);
            
            sController controller = getController();
            if(controller)
            {
                controller->inletsChanged();
            }
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
    }
    
    // ================================================================================ //
    //                                      OUTLETS                                     //
    // ================================================================================ //
    
    void Box::addOutlet(IoType type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        m_outlets.push_back(unique_ptr<Outlet>(new Outlet(type, description)));
        sController controller = getController();
        if(controller)
        {
            controller->outletsChanged();
        }
        setAttributeValue(Tag_noutlets, {m_outlets.size()});
    }
    
    void Box::insertOutlet(unsigned long index, IoType type, string const& description)
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
        sController controller = getController();
        if(controller)
        {
            controller->outletsChanged();
        }
        setAttributeValue(Tag_noutlets, {m_outlets.size()});
    }
    
    void Box::removeOutlet(unsigned long index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_outlets.size())
        {
            sPage page = getPage();
            if(page)
            {
                for(unsigned long i = 0; i < m_outlets[index]->getNumberOfLinks(); i++)
                {
                    page->removeLink(m_outlets[index]->getLink(i));
                }
            }
            m_outlets.erase(m_outlets.begin()+(long)index);
            
            sController controller = getController();
            if(controller)
            {
                controller->outletsChanged();
            }
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
    }
    
    bool Box::connectInlet(sLink link)
    {
        if(link && link->getInletIndex() < getNumberOfInlets())
        {
            lock_guard<mutex> guard(m_io_mutex);
            return m_inlets[link->getInletIndex()]->append(link);
        }
        return false;
    }

    bool Box::connectOutlet(sLink link)
    {
        if(link && link->getOutletIndex() < getNumberOfOutlets())
        {
            lock_guard<mutex> guard(m_io_mutex);
            return m_outlets[link->getOutletIndex()]->append(link);
        }
        return false;
    }
    
    bool Box::disconnectInlet(sLink link)
    {
        if(link && link->getInletIndex() < getNumberOfInlets())
        {
            lock_guard<mutex> guard(m_io_mutex);
            return m_inlets[link->getInletIndex()]->erase(link);
        }
        return false;
    }
    
    bool Box::disconnectOutlet(sLink link)
    {
        if(link && link->getOutletIndex() < getNumberOfOutlets())
        {
            lock_guard<mutex> guard(m_io_mutex);
            return m_outlets[link->getOutletIndex()]->erase(link);
        }
        return false;
    }
	
	void Box::getInletLinks(vector<sLink> links, unsigned long index)
	{
		links.clear();
		
		if (index < getNumberOfInlets())
		{
			lock_guard<mutex> guard(m_io_mutex);
			const unsigned long nLinks = m_inlets[index]->getNumberOfLinks();
			for (int i = 0; i < nLinks; i++)
			{
				sLink link = m_inlets[index]->getLink(i);
				links.push_back(link);
			}
		}
	}
	
	void Box::getOutletLinks(vector<sLink> links, unsigned long index)
	{
		links.clear();
		
		if (index < getNumberOfOutlets())
		{
			lock_guard<mutex> guard(m_io_mutex);
			const unsigned long nLinks = m_outlets[index]->getNumberOfLinks();
			for (int i = 0; i < nLinks; i++)
			{
				sLink link = m_outlets[index]->getLink(i);
				links.push_back(link);
			}
		}
	}
	
    void Box::setController(sController ctrl)
    {
        m_controller = ctrl;
    }
    
    // ================================================================================ //
    //                                  BOX CONTROLER                                   //
    // ================================================================================ //
    
    void Box::Controller::setEditionStatus(bool status)
    {
        if(m_edition != status)
        {
            m_edition = status;
            editionStatusChanged();
        }
    }
    
    void Box::Controller::setSelectedStatus(bool status)
    {
        if(m_selected != status)
        {
            m_selected = status;
            editionStatusChanged();
        }
    }

#define KIO_HEIGHT 3.
#define KIO_WIDTH 5.
    
    Point Box::Controller::getInletPosition(unsigned long index) const noexcept
    {
        const unsigned long ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
    }
    
    Point Box::Controller::getOutletPosition(unsigned long index) const noexcept
    {
        const unsigned long ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
    }
    
    bool Box::Controller::contains(Point const& pt, Knock& knock) const noexcept
    {
        const Rectangle bounds = m_box->getBounds();
        if(bounds.contains(pt))
        {
            if(pt.y() < bounds.y() + KIO_HEIGHT)
            {
                const unsigned long ninlets = m_box->getNumberOfInlets();
                if(ninlets && pt.x() <= bounds.x() + KIO_WIDTH)
                {
                    knock.m_box     = m_box;
                    knock.m_part    = Knock::Inlet;
                    knock.m_index   = 0;
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
                            knock.m_box     = m_box;
                            knock.m_part    = Knock::Inlet;
                            knock.m_index   = i;
                            return true;
                        }
                    }
                }
                else
                {
                    knock.m_box     = m_box;
                    knock.m_part    = Knock::Inside;
                    knock.m_index   = 0;
                    return true;
                }
            }
            else if(pt.y() > bounds.y() + bounds.height() - KIO_HEIGHT)
            {
                const unsigned long noutlets = m_box->getNumberOfOutlets();
                if(noutlets && pt.x() <= bounds.x() + KIO_WIDTH)
                {
                    knock.m_box     = m_box;
                    knock.m_part    = Knock::Outlet;
                    knock.m_index   = 0;
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
                            knock.m_box     = m_box;
                            knock.m_part    = Knock::Outlet;
                            knock.m_index   = i;
                            return true;
                        }
                    }
                }
                else
                {
                    knock.m_box     = m_box;
                    knock.m_part    = Knock::Inside;
                    knock.m_index   = 0;
                    return true;
                }
            }
            else
            {
                knock.m_box     = m_box;
                knock.m_part    = Knock::Inside;
                knock.m_index   = 0;
            }
            return true;
        }
        knock.m_box.reset();
        knock.m_part    = Knock::Outside;
        knock.m_index   = 0;
        return false;
    }
    
    bool Box::Controller::overlaps(Rectangle const& rect) const noexcept
    {
        return m_box->getBounds().overlaps(rect);
    }
    
    void Box::Controller::inletsChanged()
    {
        if(m_edition)
        {
            redraw();
        }
    }
    
    void Box::Controller::outletsChanged()
    {
        if(m_edition)
        {
            redraw();
        }
    }
    
    void Box::Controller::paint(sBox box, Doodle& d, bool edit, bool selected)
    {
        d.setFont(box->getFont());
        d.setColor(box->getBackgroundColor());
        d.fillRectangle(1., 1., d.getWidth() - 2., d.getHeight() - 2., 2.5);
        if(!(box->getType() & Behavior::Graphic))
        {
            d.setColor(box->getTextColor());
            d.drawText(toString(box->getText()), 3, 0, d.getWidth(), d.getHeight(), box->getFontJustification());
        }
        else
        {
            box->draw(d);
        }
        d.setColor(box->getBorderColor());
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
















