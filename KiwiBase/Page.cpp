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
#include "../KiwiDsp/DspContext.h"

namespace Kiwi
{    
    Page::Page(shared_ptr<Instance> instance) :
    m_instance(instance),
    m_dsp_context(make_shared<DspContext>())
    {
        ;
    }
    
    Page::~Page()
    {
        m_connections.clear();
        m_boxes.clear();
    }
    
    shared_ptr<Instance> Page::getInstance() const noexcept
    {
        return m_instance.lock();
    }
    
    sPage Page::create(shared_ptr<Instance> instance, sDico dico)
    {
        sPage page = make_shared<Page>(instance);
        if(page && dico)
        {
            page->read(dico);
        }
        return page;
    }
    
    sBox Page::createBox(sDico dico)
    {
        if(dico)
        {
            sBox box = Box::create(shared_from_this(), dico);
            if(box)
            {
                m_boxes.push_back(box);
				box->bind(shared_from_this());
                return box;
            }
        }
        return nullptr;
    }
    
    void Page::removeBox(shared_ptr<Box> box)
    {
        auto it = find(m_boxes.begin(), m_boxes.end(), box);
        if(it != m_boxes.end())
        {
            for(auto it2 = m_connections.begin(); it2 != m_connections.end(); ++it2)
            {
                if((*it2)->getFrom() == box || (*it2)->getTo() == box)
                {
                    removeConnection(*it2);
                }
            }
            m_boxes.erase(it);
            box->unbind(shared_from_this());
        }
    }
    
    void Page::getBoxes(vector<sBox>& boxes)
    {
        boxes = m_boxes;
    }
    
    sConnection Page::createConnection(sDico dico)
    {
        if(dico)
        {
            sBox from, to;
            size_t outlet, inlet;
            
            ElemVector elements;
            dico->get(Tag::from, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                long index = (long)elements[0] - 1;
                outlet = elements[1];
                if(index >= 0 && index < m_boxes.size())
                {
                    from = m_boxes[index];
                }
            }
            
            elements.clear();
            dico->get(Tag::to, elements);
            if(elements.size() == 2 && elements[0].isLong() && elements[1].isLong())
            {
                long index = (long)elements[0] - 1;
                inlet  = elements[1];
                if(index >= 0 && index < m_boxes.size())
                {
                    to = m_boxes[index];
                }
            }
            if(Box::connect(from, outlet, to, inlet))
            {
                sConnection connection = make_shared<Connection>(from, outlet, to, inlet);
                if(connection)
                {
                    m_connections.push_back(connection);
                    return connection;
                }
                else
                {
                    Box::disconnect(from, outlet, to, inlet);
                }
            }
        }
        return nullptr;
    }
    
    void Page::removeConnection(sConnection connection)
    {
        auto it = find(m_connections.begin(), m_connections.end(), connection);
        if(it != m_connections.end())
        {
            Box::disconnect(connection->getFrom(), connection->getOutletIndex(), connection->getTo(), connection->getInletIndex());
            m_connections.erase(it);
        }
    }
    
    void Page::getConnections(vector<sConnection>& connections)
    {
        connections = m_connections;
    }
    
    void Page::read(sDico dico)
    {
        m_connections.clear();
        m_boxes.clear();
        if(dico)
        {
            ElemVector boxes;
            dico->get(Tag::boxes, boxes);
            sort(m_boxes.begin(), m_boxes.end(), sortBoxes);
            for(size_t i = 0; i < boxes.size(); i++)
            {
                sDico subdico = boxes[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag::box);
                    if(subdico)
                    {
                        createBox(subdico);
                    }
                }
            }
            
            ElemVector connections;
            dico->get(Tag::connections, connections);
            for(size_t i = 0; i < connections.size(); i++)
            {
                sDico subdico = connections[i];
                if(subdico)
                {
                    subdico = subdico->get(Tag::connection);
                    if(subdico)
                    {
                        createConnection(subdico);
                    }
                }
            }
        }
    }
    
    void Page::write(sDico dico)
    {
        if(dico)
        {
            ElemVector boxes;
            long index = 0;
            for(auto it = m_boxes.begin(); it != m_boxes.end(); ++it)
            {
                sDico box = createDico();
                sDico subbox = createDico();
                if(box && subbox)
                {
                    (*it)->write(subbox);
                    subbox->set(Tag::id, ++index);
                    box->set(Tag::box, subbox);
                    boxes.push_back(box);
                }
            }
            dico->set(Tag::boxes, boxes);
      
            ElemVector connections;
            for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                sDico connection = createDico();
                sDico subconnect = createDico();
                if(connection && subconnect)
                {
                    shared_ptr<Box>     from    = (*it)->getFrom();
                    shared_ptr<Box>     to      = (*it)->getTo();
                    
                    if(from && to)
                    {
                        long indexFrom = 0, indexTo = 0, index = 1;
                        for(auto it = m_boxes.begin(); it != m_boxes.end(); ++it, ++index)
                        {
                            if((*it) == from)
                            {
                                indexFrom = index;
                            }
                            else if((*it) == to)
                            {
                                indexTo = index;
                            }
                        }
                        if(indexFrom && indexTo)
                        {
                            subconnect->set(Tag::from, {indexFrom, (*it)->getOutletIndex()});
                            subconnect->set(Tag::to, {indexTo, (*it)->getInletIndex()});
                            connection->set(Tag::connection, subconnect);
                        }
                    }
                    connections.push_back(connection);
                }
            }
            dico->set(Tag::connections, connections);
        }
    }
    
    void Page::startDsp(double samplerate, long vectorsize)
    {
        m_dsp_context->clear();
        m_dsp_context->setSamplerate(samplerate);
        m_dsp_context->setVectorsize(vectorsize);
        
        for(auto it = m_boxes.begin(); it != m_boxes.end(); ++it)
        {
            m_dsp_context->addBox((*it));
        }
        
        for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            m_dsp_context->addConnection((*it));
        }
        
        try
        {
            m_dsp_context->compile();
        }
        catch(shared_ptr<Box> box)
        {
            Console::error(box, "something appened with me... sniff !");
        }
    }
    
    void Page::tickDsp() const noexcept
    {
        m_dsp_context->tick();
    }
    
    void Page::stopDsp()
    {
        m_dsp_context->clear();
    }
    
    void Page::inletHasBeenCreated(shared_ptr<Box> box, size_t index)
    {
        
    }
    
    void Page::inletHasBeenRemoved(shared_ptr<Box> box, size_t index)
    {
        for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            if((*it)->getTo() == box && (*it)->getInletIndex() == index)
            {
                removeConnection(*it);
            }
        }
    }
    
    void Page::outletHasBeenCreated(shared_ptr<Box> box, size_t index)
    {
        
    }
    
    void Page::outletHasBeenRemoved(shared_ptr<Box> box, size_t index)
    {
        for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            if((*it)->getFrom() == box || (*it)->getOutletIndex() == index)
            {
                removeConnection(*it);
            }
        }
    }
    
    void Page::bind(weak_ptr<Page::Listener> listener)
    {
        m_listeners.insert(listener);
    }
    
    void Page::unbind(weak_ptr<Page::Listener> listener)
    {
        m_listeners.erase(listener);
    }
    
    bool Page::sortBoxes(Element first, Element second)
    {
        sDico firstdico = first;
        sDico seconddico = second;
        if(firstdico && seconddico)
        {
            firstdico = firstdico->get(Tag::box);
            seconddico = seconddico->get(Tag::box);
            if(firstdico && seconddico)
            {
                return (long)firstdico->get(Tag::id) < (long) seconddico->get(Tag::id);
            }
        }
        return false;
    }
}
















