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

#include "Patcher.h"
#include "Instance.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Patcher::Patcher(sInstance instance) :
    m_instance(instance)
    {
        ;
    }
    
    Patcher::~Patcher()
    {
        m_links.clear();
        m_objects.clear();
        m_free_ids.clear();
        m_lists.clear();
    }
    
    sPatcher Patcher::create(sInstance instance, sDico dico)
    {
        sPatcher patcher = make_shared<Patcher>(instance);
        if(patcher && dico)
        {
            sDico patcherDico = Dico::create();
            if(patcherDico)
            {
                if(dico->has(Tag::List::patcher))
                {
                    patcherDico = dico->get(Tag::List::patcher);
                    patcher->add(patcherDico);
                }
            }
        }
        return patcher;
    }
    
    void Patcher::createObject(scDico dico)
    {
        sObject object;
        if(dico)
        {
            sTag name = dico->get(Tag::List::name);
            sTag text = dico->get(Tag::List::text);
            ulong _id = dico->get(Tag::List::id);
            vector<Atom> args;
            dico->get(Tag::List::arguments, args);
            sObject object = Factory::create(name, Detail(getInstance(), getShared(), _id, name, text->getName(), dico, args));
            
            if(object)
            {
                m_objects.push_back(object);
            }
            send(object, Notification::Added);
        }
    }
    
    void Patcher::createLink(scDico dico)
    {
        sLink link;
        if(dico)
        {
            ulong indexo, indexi, ido, idi;
            vector<Atom> atoms;
            dico->get(Tag::List::from, atoms);
            if(atoms.size() > 1 && atoms[0].isNumber() && atoms[1].isNumber())
            {
                ido     = atoms[0];
                indexo  = atoms[1];
            }
            else
            {
                return;
            }
            
            dico->get(Tag::List::to, atoms);
            if(atoms.size() > 1 && atoms[0].isNumber() && atoms[1].isNumber())
            {
                idi     = atoms[0];
                indexi  = atoms[1];
            }
            else
            {
                return;
            }
            
            sObject from, to;
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
                                    return;
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
                                    return;
                                }
                                
                                outlet->append(to, indexo);
                                inlet->append(from, indexi);
                                link = make_shared<Link::DspLink>(shared_from_this(), from, indexo, to, indexi, type, pfrom, poutlet, pto, pinlet);
                            }
                        }
                        else
                        {
                            outlet->append(to, indexo);
                            inlet->append(from, indexi);
                            link = make_shared<Link>(shared_from_this(), from, indexo, to, indexi, type);
                        }
                    }
                }
            }
        }
        
        if(link)
        {
            m_links.push_back(link);
        }
        send(link, Notification::Added);
    }
    
    void Patcher::add(scDico dico)
    {
        sDico rdico = Dico::create(dico);
        if(rdico)
        {
            vector<Atom> objects, links;
            rdico->get(Tag::List::links, links);
            rdico->get(Tag::List::objects, objects);
            
            lock_guard<mutex> guard(m_mutex);
            for(vector<sObject>::size_type i = 0; i < objects.size(); i++)
            {
                sDico objdico = objects[i];
                if(objdico)
                {
                    const ulong r_id = objdico->get(Tag::List::id);
                    const ulong n_id = m_free_ids.empty() ? m_objects.size() + 1 : m_free_ids[0];
                    if(!m_free_ids.empty())
                    {
                        m_free_ids.erase(m_free_ids.begin());
                    }
                    objdico->set(Tag::List::id, n_id);
                    vector<Atom> atoms;
                    for(vector<sLink>::size_type i = 0; i < links.size(); i++)
                    {
                        objdico->get(Tag::List::from, atoms);
                        if(atoms.size() > 1 && r_id == (ulong)atoms[0])
                        {
                            objdico->set(Tag::List::from, {n_id, atoms[1]});
                        }
                        objdico->get(Tag::List::to, atoms);
                        if(atoms.size() > 1 && r_id == (ulong)atoms[0])
                        {
                            objdico->set(Tag::List::from, {n_id, atoms[1]});
                        }
                    }
                    createObject(objdico);
                }
            }
            
            for(vector<sLink>::size_type i = 0; i < links.size(); i++)
            {
                sDico linkdico = links[i];
                if(linkdico)
                {
                    createLink(linkdico);
                }
            }
        }
    }
    
    void Patcher::remove(sObject object)
    {
        if(object)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_objects.begin(), m_objects.end(), object);
            if(it != m_objects.end())
            {
                for(auto li = m_links.begin(); li != m_links.end();)
                {
                    if((*li)->getObjectFrom() == object || (*li)->getObjectTo() == object)
                    {
                        li = m_links.erase(li);
                        send((*li), Notification::Removed);
                    }
                    else
                    {
                        ++li;
                    }
                }
                m_objects.erase(it);
                m_free_ids.push_back(object->getId());
                send(object, Notification::Removed);
            }
        }
    }
    
    void Patcher::remove(sLink link)
    {
        if(link)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_links.begin(), m_links.end(), link);
            if(it != m_links.end())
            {
                m_links.erase(it);
                send(link, Notification::Removed);
            }
        }
    }
    
    void Patcher::toFront(sObject object)
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
    
    void Patcher::toBack(sObject object)
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
	
    void Patcher::write(sDico dico) const
    {
        if(dico)
        {
			sDico subpatcher = Dico::create();
			if(subpatcher)
			{
                vector<Atom> atoms;
				lock_guard<mutex> guard(m_mutex);
                
				for(vector<sObject>::size_type i = 0; i < m_objects.size(); i++)
				{
					sDico object = Dico::create();
					if(object)
					{
						m_objects[i]->write(object);
                        atoms.push_back(object);
					}
				}
				subpatcher->set(Tag::List::objects, atoms);
				
				atoms.clear();
				
				for(vector<sLink>::size_type i = 0; i < m_links.size(); i++)
				{
                    sDico link = Dico::create();
					if(link)
					{
						m_links[i]->write(link);
						atoms.push_back(link);
					}
				}
				subpatcher->set(Tag::List::links, atoms);
				dico->set(Tag::List::patcher, subpatcher);
			}
        }
    }
	
    void Patcher::dspStart(const ulong samplerate, const ulong vectorsize)
    {
        dspStop();
        m_dsp_context = Dsp::Context::create(samplerate, vectorsize);
        
        lock_guard<mutex> guard(m_mutex);
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
    
    void Patcher::dspStop()
    {
        if(m_dsp_context)
        {
            m_dsp_context->stop();
            m_dsp_context.reset();
        }
    }
    
    void Patcher::addListener(sListener list)
    {
        if(list)
        {
            lock_guard<mutex> guard(m_mutex);
            m_lists.insert(list);
        }
    }
    
    void Patcher::removeListener(sListener list)
    {
        if(list)
        {
            lock_guard<mutex> guard(m_mutex);
            m_lists.erase(list);
        }
    }

    void Patcher::send(sObject object, Patcher::Notification type)
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
    
    void Patcher::send(sLink link, Patcher::Notification type)
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




