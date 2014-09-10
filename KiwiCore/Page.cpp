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
#include "DspContext.h"

namespace Kiwi
{
    Page::Page(shared_ptr<Instance> kiwi, string file, string directory) : ObjectExtented(kiwi, "page")
    {
        m_file = file;
        m_directory = directory;
        m_dsp_context = make_shared<DspContext>();
        
        shared_ptr<Dico> main = createDico();
        main->read(m_file, m_directory);
        if(main->has(createTag("boxes")))
        {
            vector<Element> boxes;
            main->get(createTag("boxes"), boxes);
            for(int i = 0; i < boxes.size(); i++)
                static_pointer_cast<Dico>((shared_ptr<Object>)boxes[i])->post();
        }
    }
    
    Page::~Page()
    {
        m_connections.clear();
        m_boxes.clear();
    }

    shared_ptr<Box> Page::createBox(shared_ptr<Tag> name, vector<Element> &elements)
    {
        shared_ptr<Object> object = createObject(name, elements);
        if(object->isBox())
        {
            shared_ptr<Box> box = static_pointer_cast<Box>(object);
            m_boxes.insert(box);
            return box;
        }
        else
        {
            error(string("The object " + (string)*name + " isn't patchable !"));
            return shared_ptr<Box>();
        }
    }
    
    shared_ptr<Box> Page::createBox(string name, vector<Element> &elements)
    {
        return createBox(createTag(name), elements);
    }
    
    void Page::freeBox(shared_ptr<Box> box)
    {
        if(m_boxes.find(box) != m_boxes.end())
        {
            disconnect(box, -1, nullptr, 0);
            disconnect(nullptr, 0, box, -1);
            m_boxes.erase(box);
        }
    }
    
    bool Page::compatible(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from != to && m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Connection> connect = createConnection(from, outlet, to, inlet);
            for(set<shared_ptr<Connection>>::const_iterator it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                if(*(*it) == *connect)
                {
                    return false;
                }
            }
            return Box::compatible(from, outlet, to, inlet);
        }
        return false;
    }
    
    bool Page::connect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from != to && m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Connection> connect = createConnection(from, outlet, to, inlet);
            for(set<shared_ptr<Connection>>::const_iterator it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                if(*(*it) == *connect)
                {
                    return false;
                }
            }
            if(Box::connect(from, outlet, to, inlet))
            {
                m_connections.insert(connect);
                return true;
            }
            return false;
        }
        return false;
    }
    
    void Page::disconnect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from != to && m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Connection> connect = createConnection(from, outlet, to, inlet);
            for(set<shared_ptr<Connection>>::const_iterator it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                if(*(*it) == *connect)
                {
                    m_connections.erase(it);
                    Box::disconnect(from, outlet, to, inlet);
                }
            }
        }
    }
    
    void Page::startDsp(double samplerate, long vectorsize)
    {
        m_dsp_context->clear();
        m_dsp_context->setSamplerate(samplerate);
        m_dsp_context->setVectorsize(vectorsize);
        
        for(set<shared_ptr<Box>>::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it)
            m_dsp_context->addBox((*it));
        
        for(set<shared_ptr<Connection>>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
            m_dsp_context->addConnection((*it));
        
        try
        {
            m_dsp_context->compile();
        }
        catch(shared_ptr<Box> box)
        {
            box->errorObject("something appened with me... sniff !");
        }
    }
    
    void Page::tickDsp()
    {
        m_dsp_context->tick();
    }
    
    void Page::stopDsp()
    {
        m_dsp_context->clear();
    }
    
    void Page::write()
    {
        shared_ptr<Dico> main = createDico();
        vector<Element> elements;
        ObjectExtented::write(main);
        
        for(set<shared_ptr<Box>>::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it)
        {
            shared_ptr<Dico> box = createDico();
            (*it)->write(box);
            elements.push_back(static_pointer_cast<Object>(box));
        }
        main->set(createTag("boxes"), elements);
        
        elements.clear();
        for(set<shared_ptr<Connection>>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            shared_ptr<Dico> connection = createDico();
            (*it)->write(connection);
            elements.push_back(static_pointer_cast<Object>(connection));
        }
        main->set(createTag("connections"), elements);
        
        main->write(m_file, m_directory);
    }
}
















