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

#include "Page.h"
#include "Instance.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Page::Page(sInstance instance) :
    m_instance(instance)
    {
        ;
    }
    
    Page::~Page()
    {
        m_links.clear();
        m_objects.clear();
        m_lists.clear();
    }
    
    sPage Page::create(sInstance instance, sDico dico)
    {
        sPage page = make_shared<Page>(instance);
        if(page && dico)
        {
            sDico pageDico = Dico::create();
            if(pageDico)
            {
                if(dico->has(Tag::List::page))
                {
                    pageDico = dico->get(Tag::List::page);
                    page->add(pageDico);
                }
            }
        }
        return page;
    }
    
    sObject Page::createObject(scDico dico)
    {
        sObject object;
        if(dico)
        {
            sTag name = dico->get(Tag::List::name);
            sTag text = dico->get(Tag::List::text);
            ulong _id = dico->get(Tag::List::id);
            if(!_id)
            {
                lock_guard<mutex> guard(m_mutex);
                _id = m_objects.size() + 1;
            }
            ElemVector args;
            dico->get(Tag::List::arguments, args);
            object = Prototypes::create(name, Initializer(getInstance(), getShared(), _id, name->getName(), text->getName(), dico, args));
            object->initialize();
        }
        return object;
    }
    
    sLink Page::createLink(scDico dico)
    {
        sLink link;
        if(dico)
        {
            ulong indexo, indexi, ido, idi;
            ElemVector elements;
            dico->get(Tag::List::from, elements);
            if(elements.size() > 1 && elements[0].isNumber() && elements[1].isNumber())
            {
                ido     = elements[0];
                indexo  = elements[1];
            }
            else
            {
                return nullptr;
            }
            
            dico->get(Tag::List::to, elements);
            if(elements.size() > 1 && elements[0].isNumber() && elements[1].isNumber())
            {
                idi     = elements[0];
                indexi  = elements[1];
            }
            else
            {
                return nullptr;
            }
            
            sObject from, to;
            lock_guard<mutex> guard(m_mutex);
            if(ido < m_objects.size() + 1 && idi <  m_objects.size() + 1 && ido != idi)
            {
                for(vector<sObject>::size_type i = 0; i < m_objects.size(); i++)
                {
                    if(m_objects[i]->getId() == ido)
                    {
                        from = m_objects[i];
                    }
                    else if(m_objects[i]->getId() == idi)
                    {
                        to = m_objects[i];
                    }
                    else if(from.use_count() && to.use_count())
                    {
                        break;
                    }
                }
            }
            
            if(from && to)
            {
                Object::sOutlet outlet  = from->getOutlet(indexo);
                Object::sInlet inlet    = to->getInlet(indexi);
                if(outlet && inlet)
                {
                    if(outlet->getType() & inlet->getType() || inlet->getType() & outlet->getType())
                    {
                        Object::Io::Type type = min(outlet->getType(), inlet->getType());
                        if(type & Object::Io::Signal)
                        {
                            Dsp::sProcess pfrom = dynamic_pointer_cast<Dsp::Process>(from);
                            Dsp::sProcess pto   = dynamic_pointer_cast<Dsp::Process>(to);
                            if(from && to)
                            {
                                ulong poutlet = 0, pinlet = 0;
                                for(ulong i = 0; i < from->getNumberOfOutlets(); i++)
                                {
                                    Object::sOutlet out = from->getOutlet(poutlet);
                                    if(out)
                                    {
                                        if(out == outlet)
                                        {
                                            break;
                                        }
                                        else if(out->getType() & Object::Io::Signal)
                                        {
                                            poutlet++;
                                        }
                                    }
                                }
                                if(poutlet >= pfrom->getNumberOfOutputs())
                                {
                                    return nullptr;
                                }
                                
                                for(ulong i = 0; i < from->getNumberOfInlets(); i++)
                                {
                                    Object::sInlet in = from->getInlet(poutlet);
                                    if(in)
                                    {
                                        if(in == inlet)
                                        {
                                            break;
                                        }
                                        else if(in->getType() & Object::Io::Signal)
                                        {
                                            pinlet++;
                                        }
                                    }
                                }
                                if(pinlet >= pfrom->getNumberOfInputs())
                                {
                                    return nullptr;
                                }
                                
                                outlet->append(to, indexo);
                                inlet->append(from, indexi);
                                return make_shared<Link::DspLink>(shared_from_this(), from, indexo, to, indexi, type, pfrom, poutlet, pto, pinlet);
                            }
                        }
                        else
                        {
                            outlet->append(to, indexo);
                            inlet->append(from, indexi);
                            return make_shared<Link>(shared_from_this(), from, indexo, to, indexi, type);
                        }
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
        }
        
        return link;
    }
    
    void Page::add(scDico dico)
    {
        sDico rdico = Dico::create(dico);
        if(rdico)
        {
            ElemVector objects;
            dico->get(Tag::List::objects, objects);
            for(vector<sObject>::size_type i = 0; i < objects.size(); i++)
            {
                sDico subdico = objects[i];
                if(subdico)
                {
                    sObject object = createObject(subdico->get(Tag::List::object));
                    if(object)
                    {
                        lock_guard<mutex> guard(m_mutex);
                        m_objects.push_back(object);
                    }
                    
                    send(object, Notification::Added);
                }
            }
            
            ElemVector links;
            dico->get(Tag::List::links, links);
            for(vector<sLink>::size_type i = 0; i < links.size(); i++)
            {
                sDico subdico = links[i];
                if(subdico)
                {
                    sLink link = createLink(subdico->get(Tag::List::link));
                    if(link)
                    {
                        lock_guard<mutex> guard(m_mutex);
                        m_links.push_back(link);
                    }
                    send(link, Notification::Added);
                }
            }
        }
    }
    
    void Page::remove(sObject object)
    {
        vector<sLink> links;
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_objects.begin(), m_objects.end(), object);
            if(it != m_objects.end())
            {
                auto li = find(m_links.begin(), m_links.end(), object);
                while(li != m_links.end())
                {
                    links.push_back((*li));
                    m_links.erase(li);
                    li = find(m_links.begin(), m_links.end(), object);
                }
                
                m_objects.erase(it);
            }
        }
        for(vector<sLink>::size_type i = 0; i < links.size(); i++)
        {
            send(links[i], Notification::Removed);
        }
        send(object, Notification::Removed);
    }
    
    void Page::remove(sLink link)
    {
        if(link)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_links.begin(), m_links.end(), link);
            if(it != m_links.end())
            {
                m_links.erase(it);
            }
        }
        send(link, Notification::Removed);
    }
    
    sObject Page::replace(sObject oldobject, sDico dico)
    {
        return nullptr;
    }
    
    void Page::toFront(sObject object)
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_objects.begin(), m_objects.end(), object);
            if(it != m_objects.end())
            {
                m_objects.erase(it);
                m_objects.push_back(object);
            }
        }
    }
    
    void Page::toBack(sObject object)
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_objects.begin(), m_objects.end(), object);
            if(it != m_objects.end())
            {
                m_objects.erase(it);
                m_objects.insert(m_objects.begin(), object);
            }
        }
    }
	
    void Page::write(sDico dico) const
    {
        if(dico)
        {
			sDico subpage = Dico::create();
			if(subpage)
			{
                ElemVector elements;
				lock_guard<mutex> guard(m_mutex);
                
				for(vector<sObject>::size_type i = 0; i < m_objects.size(); i++)
				{
					sDico object = Dico::create();
					sDico subobject = Dico::create();
					if(object && subobject)
					{
						m_objects[i]->write(subobject);
						object->set(Tag::List::object, subobject);
						elements.push_back(object);
					}
				}
				subpage->set(Tag::List::objects, elements);
				
				elements.clear();
				
				for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
				{
					sDico link = Dico::create();
					sDico sublink = Dico::create();
					if(link && sublink)
					{
						m_links[i]->write(sublink);
						link->set(Tag::List::link, sublink);
						elements.push_back(link);
					}
				}
				subpage->set(Tag::List::links, elements);
				dico->set(Tag::List::page, subpage);
			}
        }
    }
	
    void Page::dspStart(const ulong samplerate, const ulong vectorsize)
    {
        dspStop();
        m_dsp_context = Dsp::Context::create(samplerate, vectorsize);

        for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
        {
            Dsp::sProcess process = dynamic_pointer_cast<Dsp::Process>((*it));
            if(process)
            {
                m_dsp_context->add(process);
            }
        }

        for(auto it = m_links.begin(); it != m_links.end(); ++it)
        {
            Dsp::sConnection connection = dynamic_pointer_cast<Dsp::Connection>((*it));
            if(connection)
            {
                m_dsp_context->add(connection);
            } 
        }
        
        try
        {
            m_dsp_context->compile();
        }
        catch(Dsp::sProcess object)
        {
            Console::error(dynamic_pointer_cast<Object>(object), "something appened with me... sniff !");
            throw shared_from_this();
        }
    }
    
    void Page::dspStop()
    {
        if(m_dsp_context)
        {
            m_dsp_context->stop();
            m_dsp_context.reset();
        }
    }
    
    void Page::addListener(sListener list)
    {
        if(list)
        {
            lock_guard<mutex> guard(m_mutex);
            m_lists.insert(list);
        }
    }
    
    void Page::removeListener(sListener list)
    {
        if(list)
        {
            lock_guard<mutex> guard(m_mutex);
            m_lists.erase(list);
        }
    }

    void Page::send(sObject object, Page::Notification type)
    {
        if(object)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            for(auto it = m_lists.begin(); it != m_lists.end(); )
            {
                sListener list = (*it).lock();
                if(list)
                {
                    if(type == Notification::Added)
                    {
                        list->objectCreated(getShared(), object);
                    }
                    else
                    {
                        list->objectRemoved(getShared(), object);
                    }
                    
                    ++it;
                }
                else
                {
                    it = m_lists.erase(it);
                }
            }
        }
    }
    
    void Page::send(sLink link, Page::Notification type)
    {
        if(link)
        {
            lock_guard<mutex> guard(m_lists_mutex);
            for(auto it = m_lists.begin(); it != m_lists.end(); )
            {
                sListener list = (*it).lock();
                if(list)
                {
                    if(type == Notification::Added)
                    {
                        list->linkCreated(getShared(), link);
                    }
                    else
                    {
                        list->linkRemoved(getShared(), link);
                    }
                    
                    ++it;
                }
                else
                {
                    it = m_lists.erase(it);
                }
            }
        }
    }
}




