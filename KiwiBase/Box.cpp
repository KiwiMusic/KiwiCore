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
    
    Box::Box(sPage page, string const& name, ulong flags) :
    m_instance(page ? page->m_instance : weak_ptr<Instance>()),
    m_page(page),
    m_name(Tag::create(name)),
    m_id(page ? page->m_boxe_id : 0),
    m_flags(0 | flags),
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
                    if(!dico->has(Tag_size) && !(box->isGUI()))
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
    
    void Box::send(ulong index, ElemVector const& elements) const noexcept
    {
        m_io_mutex.lock();
        if(index < m_outlets.size())
        {
            m_outlets[(vector<uOutlet>::size_type)index]->send(elements);
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
    
    void Box::addInlet(Iolet::Type type, Iolet::Polarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        sInlet inlet = Inlet::create(type, polarity, description);
        if(inlet)
        {
            m_inlets.push_back(inlet);
            sController controller = getController();
            if(controller)
            {
                controller->inletsChanged();
            }
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
        
    }
    
    void Box::insertInlet(ulong index, Iolet::Type type, Iolet::Polarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        sInlet inlet = Inlet::create(type, polarity, description);
        if(inlet)
        {
            if(index >= m_inlets.size())
            {
                m_inlets.push_back(inlet);
            }
            else
            {
                m_inlets.insert(m_inlets.begin()+(long)index, inlet);
            }
            
            sController controller = getController();
            if(controller)
            {
                controller->inletsChanged();
            }
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
    }
    
    void Box::removeInlet(ulong index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_inlets.size())
        {
            sPage page = getPage();
            if(page)
            {
                int zaza;
                /*
                for(ulong i = 0; i < m_inlets[index]->getNumberOfLinks(); i++)
                {
                    page->removeLink(m_inlets[index]->getLink(i));
                }
                 */
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
    
    void Box::addOutlet(Iolet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        sOutlet outlet = Outlet::create(type, description);
        if(outlet)
        {
            m_outlets.push_back(outlet);
            sController controller = getController();
            if(controller)
            {
                controller->outletsChanged();
            }
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
    }
    
    void Box::insertOutlet(ulong index, Iolet::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_io_mutex);
        sOutlet outlet = Outlet::create(type, description);
        if(outlet)
        {
            if(index >= m_outlets.size())
            {
                m_outlets.push_back(outlet);
            }
            else
            {
                m_outlets.insert(m_outlets.begin()+(long)index, outlet);
            }
            sController controller = getController();
            if(controller)
            {
                controller->outletsChanged();
            }
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
        
    }
    
    void Box::removeOutlet(ulong index)
    {
        lock_guard<mutex> guard(m_io_mutex);
        if(index < m_outlets.size())
        {
            sPage page = getPage();
            if(page)
            {
                int zaza;
                /*
                for(ulong i = 0; i < m_outlets[index]->getNumberOfLinks(); i++)
                {
                    page->removeLink(m_outlets[index]->getLink(i));
                }*/
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

	Rectangle Box::Controller::getBounds() const noexcept
	{
		return m_box->getBounds().expanded(m_framesize);
	}
	
	Point Box::Controller::getPosition() const noexcept
	{
		return getBounds().position();
	}

	Point Box::Controller::getSize() const noexcept
	{
		return getBounds().size();
	}

#define KIO_HEIGHT 3.
#define KIO_WIDTH 5.
    
    Point Box::Controller::getInletPosition(ulong index) const noexcept
    {
        const ulong ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
    }
    
    Point Box::Controller::getOutletPosition(ulong index) const noexcept
    {
        const ulong ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
        }
        return Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
    }
    
    bool Box::Controller::contains(Point const& pt, Knock& knock) const noexcept
    {
		// test resizer
		const Rectangle bounds = m_box->getBounds();
		if(getBounds().contains(pt))
		{
			bool inside = false;
			
			const Point rectSize = Point(8, 8);
			const Rectangle top_left_rect = Rectangle::withCentre(bounds.position(), rectSize);
			const Rectangle top_right_rect = Rectangle::withCentre(Point(bounds.x() + bounds.width(), bounds.y()), rectSize);
			const Rectangle bottom_right_rect = Rectangle::withCentre(Point(bounds.x() + bounds.width(), bounds.y() + bounds.height()), rectSize);
			const Rectangle bottom_left_rect = Rectangle::withCentre(Point(bounds.x(), bounds.y() + bounds.height()), rectSize);
			
			// test corners
			if(top_left_rect.contains(pt))
			{
				inside = true;
				knock.m_part = Knock::Corner;
				knock.m_corner = Knock::TopLeft;
			}
			else if(top_right_rect.contains(pt))
			{
				inside = true;
				knock.m_part = Knock::Corner;
				knock.m_corner = Knock::TopRight;
			}
			else if(bottom_right_rect.contains(pt))
			{
				inside = true;
				knock.m_part = Knock::Corner;
				knock.m_corner = Knock::BottomRight;
			}
			else if(bottom_left_rect.contains(pt))
			{
				inside = true;
				knock.m_part = Knock::Corner;
				knock.m_corner = Knock::BottomLeft;
			}
			
			if(inside)
			{
				knock.m_box = m_box;
				return true;
			}
		}
		
		
        if(bounds.contains(pt))
        {
            if(pt.y() < bounds.y() + KIO_HEIGHT)
            {
                const ulong ninlets = m_box->getNumberOfInlets();
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
                    for(ulong i = 1; i < ninlets; i++)
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
                const ulong noutlets = m_box->getNumberOfOutlets();
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
                    for(ulong i = 1; i < noutlets; i++)
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
		if (box)
		{
			if (Box::sController boxctrl = box->getController())
			{
				Rectangle boxFrame = boxctrl->getBounds();
				Rectangle boxBounds = box->getBounds() - boxFrame.position();
				boxFrame.position(Point());
				
				double frameSize = boxctrl->getFrameSize();
				
				if(edit)
				{
					if(!selected)
					{
						const ulong ninlets = box->getNumberOfInlets();
						const ulong noutlets= box->getNumberOfOutlets();
						
						if(ninlets)
						{
							d.setColor({0.3, 0.3, 0.3, 1.});
							d.fillRectangle(boxBounds.x(), boxBounds.y(), KIO_WIDTH, KIO_HEIGHT);
						}
						if(ninlets > 1)
						{
							const double ratio = (boxBounds.width() - KIO_WIDTH) / (double)(ninlets - 1);
							for(ulong i = 1; i < ninlets; i++)
							{
								d.fillRectangle(boxBounds.x() + ratio * i, boxBounds.y(), KIO_WIDTH, KIO_HEIGHT);
							}
						}
						
						if(noutlets)
						{
							d.setColor({0.3, 0.3, 0.3, 1.});
							d.fillRectangle(boxBounds.x(), boxBounds.y() + boxBounds.height() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT);
						}
						if(noutlets > 1)
						{
							const double ratio = (boxBounds.width() - KIO_WIDTH) / (double)(noutlets - 1);
							for(ulong i = 1; i < noutlets; i--)
							{
								d.fillRectangle(boxBounds.x() + ratio * i, boxBounds.y() + boxBounds.height() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT);
							}
						}
					}

					if(selected)
					{
						Color selectedColor = Color(0., 0.6, 0.9);
						d.setColor(selectedColor);
						d.drawRectangle(boxFrame.reduced(frameSize*0.5), frameSize*0.5);
						
						d.setColor(selectedColor.darker(0.3));
						d.drawRectangle(boxFrame, 1);
					}
				}
			}
		}
    }
	
	void Box::Controller::paintBox(sBox box, Doodle& d)
	{
		if (box)
		{
			if(!(box->draw(d)))
			{
				double borderSize = 1.5;
				d.setColor(box->getBorderColor());
				d.drawRectangle(d.getBounds().reduced(borderSize), borderSize, 0);
				
				d.setColor(box->getBackgroundColor());
				d.fillRectangle(d.getBounds().reduced(borderSize));
				
				d.setColor(box->getTextColor());
				d.drawText(toString(box->getText()), 3, 0, d.getWidth(), d.getHeight(), box->getFontJustification());
			}
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
        if(m_prototypes.find(tname) != m_prototypes.end())
        {
            Console::error("The box " + toString(box->getName()) + " already exist !");
        }
        else
        {
            m_prototypes[tname] = move(box);
        }
    }
}
















