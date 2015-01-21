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
    const sTag Box::Tag_focus       = Tag::create("focus");
    const sTag Box::Tag_name        = Tag::create("name");
    const sTag Box::Tag_set         = Tag::create("set");
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    Box::Box(sPage page, string const& name, ulong flags) :
    m_instance(page ? page->getInstance() : weak_ptr<Instance>()),
    m_page(page),
    m_name(Tag::create(name)),
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
                int zaza_pourquoi;
                /*
                sTag text = dico->get(Tag_text);
                if(text)
                {
                    sDico other = Dico::evaluateForBox(toString(text));
                    ElemVector keys;
                    other->keys(keys);
                    for(ElemVector::size_type i = 0; i < keys.size(); i++)
                    {
                        ElemVector values;
                        other->get(keys[i], values);
                        dico->set(keys[i], values);
                    }
                }*/
                
                sBox box = it->second->allocate(page, dico);
                if(box)
                {
                    box->load(dico);
					box->Attr::Manager::read(dico);
                    if(!dico->has(Tag_size) && !(box->isGUI()))
                    {
                        Gui::Point size = Gui::Font::getStringSize(box->getFont(), toString(box->getText()));
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
	
	bool Box::attributeChanged(sAttr attr)
	{
        if(!this->notify(attr))
        {
            return false;
        }
        sController controller = getController();
		if(controller)
		{
			if(attr == AttrBox::attr_position || attr == AttrBox::attr_presentation_position)
			{
				int TODO_notify_depending_on_page_presentation_mode;
				controller->positionChanged();
			}
			else if(attr == AttrBox::attr_size || attr == AttrBox::attr_presentation_size)
			{
				int TODO_notify_depending_on_page_presentation_mode;
				controller->sizeChanged();
			}
			else if(attr == AttrBox::attr_presentation)
			{
				controller->presentationStatusChanged();
			}
			else if(attr == AttrBox::attr_color_background || attr == AttrBox::attr_color_border || attr == AttrBox::attr_color_text)
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
    
    void Box::Controller::setPageEditionStatus(const bool edition)
    {
        if(m_page_edition_status != edition)
        {
            m_page_edition_status = edition;
            pageEditionStatusChanged();
        }
    }
	
	void Box::Controller::setPagePresentationStatus(const bool presentation)
	{
		if(m_page_presentation_status != presentation)
		{
			m_page_presentation_status = presentation;
			pagePresentationStatusChanged();
		}
	}
    
    void Box::Controller::setSelectedStatus(bool status)
    {
        if(m_selected != status)
        {
            m_selected = status;
            selectionStatusChanged();
        }
    }

	Gui::Rectangle Box::Controller::getBounds(const bool presentation) const noexcept
	{
		return m_box->getBounds(presentation).expanded(m_framesize);
	}
	
	Gui::Point Box::Controller::getPosition(const bool presentation) const noexcept
	{
		return getBounds(presentation).position();
	}

	Gui::Point Box::Controller::getSize(const bool presentation) const noexcept
	{
		return getBounds(presentation).size();
	}

#define KIO_HEIGHT 3.
#define KIO_WIDTH 5.
    
    Gui::Point Box::Controller::getInletPosition(ulong index) const noexcept
    {
        const ulong ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Gui::Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
        }
        return Gui::Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + KIO_HEIGHT * 0.5);
    }
    
    Gui::Point Box::Controller::getOutletPosition(ulong index) const noexcept
    {
        const ulong ninlets = m_box->getNumberOfInlets();
        if(index && ninlets > 1)
        {
            const double x = index * (m_box->getSize().x() - KIO_WIDTH) / (double)(ninlets - 1);
            return Gui::Point(m_box->getPosition().x() + x + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
        }
        return Gui::Point(m_box->getPosition().x() + KIO_WIDTH * 0.5, m_box->getPosition().y() + m_box->getSize().y() - KIO_HEIGHT * 0.5);
    }
    
    bool Box::Controller::contains(Gui::Point const& pt, Knock& knock, const bool presentation) const noexcept
    {
        /*
		const Gui::Rectangle bounds = m_box->getBounds(presentation);
		const Gui::Rectangle ctrlbounds = getBounds(presentation);
		
		// test resizer
		if(isSelected() && ctrlbounds.contains(pt))
		{
			const Gui::Point localPoint = pt - ctrlbounds.position();
			const double framesize = getFrameSize();
			
			if(localPoint.y() <= framesize)
			{
				knock.m_part = Knock::Border;
				knock.m_border |= Knock::Top;
			}
			if(localPoint.x() >= ctrlbounds.width() - framesize)
			{
				knock.m_part = Knock::Border;
				knock.m_border |= Knock::Right;
			}
			if(localPoint.y() >= ctrlbounds.height() - framesize)
			{
				knock.m_part = Knock::Border;
				knock.m_border |= Knock::Bottom;
			}
			if(localPoint.x() <= framesize)
			{
				knock.m_part = Knock::Border;
				knock.m_border |= Knock::Left;
			}

			if(knock.m_part == Knock::Border)
			{
				knock.m_border &= ~Knock::None;
				knock.m_box = m_box;
				return true;
			}
		}
		
        if(bounds.contains(pt))
        {
			knock.m_box     = m_box;
			knock.m_part    = Knock::Inside;
			knock.m_index   = 0;
			
			if(!isSelected() && !presentation)
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
							const double val = ratio * i + bounds.x();
							if(pt.x() >= val && pt.x() <= val + KIO_WIDTH)
							{
								knock.m_box     = m_box;
								knock.m_part    = Knock::Outlet;
								knock.m_index   = i;
								return true;
							}
						}
					}
				}
			}
			return true;
        }
        knock.m_box.reset();
        knock.m_part    = Knock::Outside;
        knock.m_index   = 0;
         */
        return false;
    }
    
    bool Box::Controller::overlaps(Gui::Rectangle const& rect, const bool presentation) const noexcept
    {
        return m_box->getBounds(presentation).overlaps(rect);
    }
    
    void Box::Controller::inletsChanged()
    {
        if(m_page_presentation_status)
        {
            redraw();
        }
    }
    
    void Box::Controller::outletsChanged()
    {
        if(m_page_presentation_status)
        {
            redraw();
        }
    }
    void Box::Controller::paintBoxFrame(sBox box, Gui::Doodle& d, const bool selected, const bool edit, const bool presentation)
    {
		if (box)
		{
			if (Box::sController boxctrl = box->getController())
			{
				if(edit)
				{
					Gui::Rectangle boxFrame = boxctrl->getBounds(presentation);
					double frameSize = boxctrl->getFrameSize();
					Gui::Rectangle boxBounds = box->getBounds(presentation) - boxFrame.position();
                    boxFrame.position({0, 0});
                    const Gui::Color ioColor = Gui::Color(0.3, 0.3, 0.3);
					const Gui::Color presentationColor = Gui::Color(0., 0.8, 0.);
					const Gui::Color selectionColor = presentation ? presentationColor : Gui::Color(0., 0.6, 0.9);
					
					if(selected)
					{
						d.setColor(selectionColor);
						d.drawRectangle(boxFrame.reduced(frameSize*0.5), frameSize*0.5);
						
						d.setColor(selectionColor.darker(0.3));
						d.drawRectangle(boxFrame, 1);
					}
					else if(!presentation)
					{
						const ulong ninlets = box->getNumberOfInlets();
						const ulong noutlets= box->getNumberOfOutlets();
						
						if(ninlets)
						{
							d.setColor(ioColor);
							d.fillRectangle(boxBounds.x(), boxBounds.y(), KIO_WIDTH, KIO_HEIGHT);
							
							if(ninlets > 1)
							{
								const double ratio = (boxBounds.width() - KIO_WIDTH) / (double)(ninlets - 1);
								for(ulong i = 1; i < ninlets; i++)
								{
									d.fillRectangle(boxBounds.x() + ratio * i, boxBounds.y(), KIO_WIDTH, KIO_HEIGHT);
								}
							}
						}
						
						if(noutlets)
						{
							d.setColor(ioColor);
							d.fillRectangle(boxBounds.x(), boxBounds.y() + boxBounds.height() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT);
							
							if(noutlets > 1)
							{
								const double ratio = (boxBounds.width() - KIO_WIDTH) / (double)(noutlets - 1);
								for(ulong i = 1; i < noutlets; i--)
								{
									d.fillRectangle(boxBounds.x() + ratio * i, boxBounds.y() + boxBounds.height() - KIO_HEIGHT, KIO_WIDTH, KIO_HEIGHT);
								}
							}
						}
						
						if (box->isInPresentation())
						{
							d.setColor(presentationColor.withAlpha(0.2));
							d.drawRectangle(boxBounds, 3);
						}
					}
				}
			}
		}
    }
	
	void Box::Controller::paintBox(sBox box, Gui::Doodle& d)
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
	
	void Box::getPrototypeNames(vector<sTag>& names)
	{
		lock_guard<mutex> guard(m_prototypes_mutex);
		
		for(auto it = m_prototypes.begin(); it !=  m_prototypes.end(); ++it)
		{
			names.push_back(it->first);
		}
	}
}
















