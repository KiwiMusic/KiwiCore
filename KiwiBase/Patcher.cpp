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
#include "Console.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    Patcher::Patcher(sInstance instance) : DspChain(instance),
    m_instance(instance),
	m_color_unlocked_background(Attr::create("unlocked_bgcolor","Unlocked Background Color", "Appearance", ColorValue(0.88, 0.89, 0.88, 1.))),
	m_color_locked_background(Attr::create("locked_bgcolor", "Locked Background Color", "Appearance", ColorValue(0.88, 0.89, 0.88, 1.))),
	m_gridsize(Attr::create("gridsize", "Grid Size", "Editing", LongValue(20)))
    {
		addAttr(m_color_unlocked_background);
		addAttr(m_color_locked_background);
		addAttr(m_gridsize);
    }
	
    Patcher::~Patcher()
    {
        m_links.clear();
        m_objects.clear();
        m_free_ids.clear();
        m_lists.clear();
    }
    
    sPatcher Patcher::create(sInstance instance, Dico& dico)
    {
        sPatcher patcher = make_shared<Patcher>(instance);
        if(patcher)
        {
            instance->DspContext::add(patcher);
			patcher->initialize();
			
            auto it = dico.find(Tag::List::patcher);
            if(it != dico.end())
            {
                patcher->add(it->second);
            }
        }
        return patcher;
    }
    
    void Patcher::createObject(Dico const& dico)
    {
        sObject object;
        if(dico.count(Tag::List::name) &&
           dico.count(Tag::List::text) &&
           dico.count(Tag::List::id) &&
           dico.count(Tag::List::arguments))
        {
            sTag name = dico.at(Tag::List::name);
            sTag text = dico.at(Tag::List::text);
            ulong _id = dico.at(Tag::List::id);
            Vector args=dico.at(Tag::List::arguments);
            object = Factory::create(name, Detail(getInstance(), getShared(), _id, name, text->getName(), dico, args));
            if(object)
            {
                sDspNode dspnode = dynamic_pointer_cast<DspNode>(object);
                if(dspnode)
                {
                    DspChain::add(dspnode);
                }
                m_objects.push_back(object);
            }
            send(object, Notification::Added);
        }
    }
    
    void Patcher::createLink(Dico const& dico)
    {
        sLink link;
        ulong indexo, indexi, ido, idi;
        Vector atoms;
        auto it = dico.find(Tag::List::from);
        if(it != dico.end())
        {
            atoms = it->second;
        }
        if(atoms.size() > 1 && atoms[0].isNumber() && atoms[1].isNumber())
        {
            ido     = atoms[0];
            indexo  = atoms[1];
        }
        else
        {
            return;
        }
        
        atoms.clear();
        it = dico.find(Tag::List::to);
        if(it != dico.end())
        {
            atoms = it->second;
        }
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
                if(outlet->getType() >= Object::Io::Signal && inlet->getType() >= Object::Io::Signal)
                {
                    sDspNode pfrom = dynamic_pointer_cast<DspNode>(from);
                    sDspNode pto   = dynamic_pointer_cast<DspNode>(to);
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
                        
                        for(ulong i = 0; i < to->getNumberOfInlets(); i++)
                        {
                            Object::sInlet in = to->getInlet(pinlet);
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
                        if(pinlet >= pto->getNumberOfInputs())
                        {
                            return;
                        }
                        
                        outlet->append(to, indexo);
                        inlet->append(from, indexi);
                        if(outlet->getType() == Object::Io::Both && inlet->getType() == Object::Io::Both)
                        {
                            link = make_shared<Link::SignalLink>(getShared(), from, indexo, to, indexi, Object::Io::Both, pfrom, poutlet, pto, pinlet);
                        }
                        else
                        {
                            link = make_shared<Link::SignalLink>(getShared(), from, indexo, to, indexi, Object::Io::Signal, pfrom, poutlet, pto, pinlet);
                        }
                    }
                }
                else if(outlet->getType() == inlet->getType() || inlet->getType() == Object::Io::Both || outlet->getType() == Object::Io::Both)
                {
                    
                    outlet->append(to, indexo);
                    inlet->append(from, indexi);
                    link = make_shared<Link>(getShared(), from, indexo, to, indexi, Object::Io::Message);
                }
            }
        }
        
        if(link)
        {
            sDspLink dsplink = dynamic_pointer_cast<DspLink>(link);
            if(dsplink)
            {
                DspChain::add(dsplink);
            }
            m_links.push_back(link);
        }
        send(link, Notification::Added);
    }
    
    void Patcher::add(Dico const& dico)
    {
        Dico rdico = dico;
        Vector objects, links;
        auto it = dico.find(Tag::List::objects);
        if(it != dico.end())
        {
            objects = it->second;
        }
        it = dico.find(Tag::List::links);
        if(it != dico.end())
        {
            links = it->second;
        }

        lock_guard<mutex> guard(m_mutex);
        for(vector<sObject>::size_type i = 0; i < objects.size(); i++)
        {
            Dico objdico(objects[i]);
            if(!objdico.empty())
            {
                const ulong r_id = objdico[Tag::List::id];
                const ulong n_id = m_free_ids.empty() ? m_objects.size() + 1 : m_free_ids[0];
                if(!m_free_ids.empty())
                {
                    m_free_ids.erase(m_free_ids.begin());
                }
                objdico[Tag::List::id] = (long)n_id;
                Vector atoms;
                for(vector<sLink>::size_type i = 0; i < links.size(); i++)
                {
                    atoms = objdico[Tag::List::from];
                    if(atoms.size() > 1 && r_id == (ulong)atoms[0])
                    {
                        objdico[Tag::List::from] = {(long)n_id, atoms[1]};
                    }
            
                    atoms = objdico[Tag::List::to];
                    if(atoms.size() > 1 && r_id == (ulong)atoms[0])
                    {
                        objdico[Tag::List::to] = {(long)n_id, atoms[1]};
                    }
                }
                createObject(objdico);
            }
        }
        
        for(vector<sLink>::size_type i = 0; i < links.size(); i++)
        {
            Dico linkdico(links[i]);
            if(!linkdico.empty())
            {
                createLink(linkdico);
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
                        sDspLink dsplink = dynamic_pointer_cast<DspLink>((*li));
                        if(dsplink)
                        {
                            DspChain::remove(dsplink);
                        }
                        li = m_links.erase(li);
                        send((*li), Notification::Removed);
                    }
                    else
                    {
                        ++li;
                    }
                }
                sDspNode dspnode = dynamic_pointer_cast<DspNode>(object);
                if(dspnode)
                {
                    DspChain::remove(dspnode);
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
                sDspLink dsplink = dynamic_pointer_cast<DspLink>(link);
                if(dsplink)
                {
                    DspChain::remove(dsplink);
                }
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
	
    void Patcher::write(Dico& dico) const
    {
        if(1)
        {
            /*
			Dico& subpatcher = Dico::create();
			if(subpatcher)
			{
                Vector atoms;
				lock_guard<mutex> guard(m_mutex);
                
				for(vector<sObject>::size_type i = 0; i < m_objects.size(); i++)
				{
					Dico& object = Dico::create();
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
                    Dico& link = Dico::create();
					if(link)
					{
						m_links[i]->write(link);
						atoms.push_back(link);
					}
				}
				subpatcher->set(Tag::List::links, atoms);
				dico->set(Tag::List::patcher, subpatcher);
			}*/
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
    
    void Patcher::create(Vector const& inputs, Vector outputs)
    {
        if(!inputs.empty())
        {
            /*
            if(inputs[0].getType() == Atom::TAG)
            {
                if(inputs[0] == Tag::List::object)
                {
                    createObject(Dico::create());
                }
                else if(inputs[0] == Tag::List::link)
                {
                    createLink(Dico::create());
                }
                else
                {
                    Console::error("Creation command accepts only \"object\" or \"link\" argument.");
                }
            }
            else
            {
                Console::error("Creation command needs a tag as first argument.");
            }*/
        }
        else
        {
            Console::error("Creation command is empty.");
        }
    }
    
    void Patcher::remove(Vector const& inputs)
    {
        
    }
    
    void Patcher::get(Vector const& inputs, Vector outputs) const
    {
        
    }
    
    void Patcher::set(Vector const& inputs)
    {
        
    }
}




