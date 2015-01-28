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
    // ================================================================================ //
    //                                      IOLET                                       //
    // ================================================================================ //
    
    bool Box::Iolet::has(sBox box, ulong index) const noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Box::Iolet::append(sBox box, ulong index) noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    return false;
                }
            }
            m_connections.push_back({box, index});
            sort(m_connections.begin(), m_connections.end());
            box->bind(shared_from_this(), Box::Tag_position, Attr::ValueChanged);
            return true;
        }
        return false;
    }
    
    bool Box::Iolet::erase(sBox box, ulong index) noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    m_connections.erase(m_connections.begin() + i);
                    box->unbind(shared_from_this(), Box::Tag_position, Attr::ValueChanged);
                    return false;
                }
            }
        }
        return false;
    }
    
    void Box::Iolet::notify(Attr::sManager manager, sAttr attr, Attr::Notification type)
    {
        lock_guard<mutex> guard(m_mutex);
        sort(m_connections.begin(), m_connections.end());
    }
    
    Box::Outlet::Outlet(Io::Type type, string const& description) noexcept :
    Iolet(type, Io::Polarity::Hot, description)
    {
        ;
    }
    
    Box::Outlet::~Outlet()
    {
        ;
    }
    
    Box::sOutlet Box::Outlet::create(Io::Type type, string const& description)
    {
        return make_shared<Outlet>(type, description);
    }
    
    void Box::Outlet::send(ElemVector const& elements) const noexcept
    {
        lock_guard<mutex> guard(m_mutex);
        for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
        {
            sBox receiver       = m_connections[i].box.lock();
            ulong inlet  = m_connections[i].index;
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
        }
    }
    
    Box::Inlet::Inlet(Io::Type type, Io::Polarity polarity, string const& description) noexcept :
    Iolet(type, polarity, description)
    {
        ;
    }
    
    Box::Inlet::~Inlet()
    {
        ;
    }
    
    Box::sInlet Box::Inlet::create(Io::Type type, Io::Polarity polarity, string const& description)
    {
        return make_shared<Inlet>(type, polarity, description);
    }
    
    bool Box::Connection::operator<(Connection const& other) const noexcept
    {
        sBox box1 = this->box.lock();
        sBox box2 = other.box.lock();
        if(box1 && box2)
        {
            if(box1->getPosition().x() == box2->getPosition().x())
            {
                return box1->getPosition().y() < box2->getPosition().y();
            }
            return box1->getPosition().x() < box2->getPosition().x();
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    map<sTag, unique_ptr<Box>>  Box::m_prototypes;
    
    Box::Box(sPage page, string const& name) :
    m_instance(page ? page->getInstance() : weak_ptr<Instance>()),
    m_page(page),
    m_name(Tag::create(name)),
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
        sTag name = dico->get(Tag::List::name);
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
                    int todo;
                    /*
                    if(!dico->has(Tag_size) && !(box->isGUI()))
                    {
                        Gui::Point size = Gui::Font::getStringSize(box->getFont(), toString(box->getText()));
                        box->setAttributeValue(Tag_size, {max(ceil(size.x()) + 6., 25.), box->getSize().y()});
                    }
                     */
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
        dico->set(Tag::List::name, getName());
    }
    
    void Box::send(ulong index, ElemVector const& elements) const noexcept
    {
        m_mutex.lock();
        if(index < m_outlets.size())
        {
            m_outlets[(vector<sOutlet>::size_type)index]->send(elements);
        }
        m_mutex.unlock();
    }
	
	bool Box::attributeChanged(sAttr attr)
	{
        if(!this->notify(attr))
        {
            return false;
        }
		
		m_listeners_mutex.lock();
		auto it = m_listeners.begin();
		while(it != m_listeners.end())
		{
			if((*it).expired())
			{
				it = m_listeners.erase(it);
			}
			else
			{
				Box::sListener listener = (*it).lock();
				listener->attributeChanged(attr);
				++it;
			}
		}
		m_listeners_mutex.unlock();
		
		return true;
	}
	
    // ================================================================================ //
    //                                      INLETS                                      //
    // ================================================================================ //
    
    void Box::addInlet(Io::Type type, Io::Polarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
        sInlet inlet = Inlet::create(type, polarity, description);
        if(inlet)
        {
            m_inlets.push_back(inlet);
			
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->inletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
    }
    
    void Box::insertInlet(ulong index, Io::Type type, Io::Polarity polarity, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
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
            
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->inletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
    }
    
    void Box::removeInlet(ulong index)
    {
        lock_guard<mutex> guard(m_mutex);
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
            
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->inletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_ninlets, {m_inlets.size()});
        }
    }
    
    // ================================================================================ //
    //                                      OUTLETS                                     //
    // ================================================================================ //
    
    void Box::addOutlet(Io::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
        sOutlet outlet = Outlet::create(type, description);
        if(outlet)
        {
            m_outlets.push_back(outlet);
			
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->outletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
    }
    
    void Box::insertOutlet(ulong index, Io::Type type, string const& description)
    {
        lock_guard<mutex> guard(m_mutex);
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
			
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->outletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
        
    }
    
    void Box::removeOutlet(ulong index)
    {
        lock_guard<mutex> guard(m_mutex);
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
			
			m_listeners_mutex.lock();
			auto it = m_listeners.begin();
			while(it != m_listeners.end())
			{
				if((*it).expired())
				{
					it = m_listeners.erase(it);
				}
				else
				{
					Box::sListener listener = (*it).lock();
					listener->outletsChanged();
					++it;
				}
			}
			m_listeners_mutex.unlock();
			
            setAttributeValue(Tag_noutlets, {m_outlets.size()});
        }
    }
	
	void Box::addListener(sListener listener)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
			m_listeners.insert(listener);
		}
	}
	
	void Box::removeListener(sListener listener)
	{
		if(listener)
		{
			lock_guard<mutex> guard(m_listeners_mutex);
			m_listeners.erase(listener);
		}
	}
	
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
















