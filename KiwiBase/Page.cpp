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
#include "../KiwiBase/Connection.h"

namespace Kiwi
{
    Page::Page(shared_ptr<Instance> kiwi, string file, string directory) :
    m_kiwi(kiwi)
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
        }
    }
    
    Page::~Page()
    {
        m_connections.clear();
        m_boxes.clear();
    }

    sTag Page::createTag(string const& name) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createTag(name);
        else
            return nullptr;
    }
    
    sObject Page::createObject(sDico dico) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createObject(dico);
        else
            return nullptr;
    }
    
    sDico Page::createDico() const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createDico();
        else
            return nullptr;
    }
    
    sBox Page::createBox(string const& text)
    {
        sDico dico = toDico(text);
        if(dico)
        {
            sObject object = createObject(dico);
            if(object)
            {
                if(object->type() == Object::BOX)
                {
                    sBox box = static_pointer_cast<Box>(object);
                    box->setId(m_boxes.size()+1);
                    box->read(dico);
                    m_boxes.insert(box);
                    //box->setPage(shared_from_this());
                    return box;
                }
                else
                {
                    //error(string("The object " + (string)*object->name() + " isn't patchable !"));
                }
            }
        }
        return nullptr;
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
    
    bool Page::compatible(shared_ptr<Box> from, int outletIndex, shared_ptr<Box> to, int inletIndex)
    {
        if(from != to && m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Inlet>   inlet   = to->getInlet(inletIndex);
            shared_ptr<Outlet>  outlet  = from->getOutlet(outletIndex);
            if(inlet && outlet && !outlet->has(inlet))
            {
                return true;
            }
        }
        return false;
    }
    
    bool Page::connect(shared_ptr<Box> from, int outletIndex, shared_ptr<Box> to, int inletIndex)
    {
        if(m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Inlet>   inlet   = to->getInlet(inletIndex);
            shared_ptr<Outlet>  outlet  = from->getOutlet(outletIndex);
            if(inlet && outlet && !outlet->has(inlet))
            {
                shared_ptr<Connection> connect = make_shared<Connection>(from, outlet, to, inlet);
                outlet->connect(inlet, to);
                m_connections.insert(connect);
                return true;
            }
            return false;
        }
        return false;
    }
    
    void Page::disconnect(shared_ptr<Box> from, int outletIndex, shared_ptr<Box> to, int inletIndex)
    {
        if(m_boxes.find(from) != m_boxes.end() && m_boxes.find(to) != m_boxes.end())
        {
            shared_ptr<Inlet>   inlet   = to->getInlet(inletIndex);
            shared_ptr<Outlet>  outlet  = from->getOutlet(outletIndex);
            for(set<shared_ptr<Connection>>::const_iterator it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                if((*it)->inlet() == inlet && (*it)->outlet() == outlet)
                {
                    m_connections.erase(it);
                    outlet->disconnect(inlet);
                    return;
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
            box->error(box, "something appened with me... sniff !");
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
        sDico main = createDico();
        
        ElemVector boxes;
        for(set<shared_ptr<Box>>::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it)
        {
            boxes.push_back(static_pointer_cast<Object>(*it));
        }
        main->set(createTag("boxes"), boxes);
        
        ElemVector connections;
        for(set<shared_ptr<Connection>>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            sDico connection = createDico();
            sDico subconnect = createDico();
            (*it)->write(subconnect);
            connection->set(createTag("connection"), subconnect);
            connections.push_back(connection);
        }
        main->set(createTag("connections"), connections);
        
        main->write(m_file, m_directory);
    }
	
	sDico Page::getDico()
	{
		sDico main = createDico();
		ElemVector elements;
		
		for(set<shared_ptr<Box>>::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it)
		{
			sDico box = createDico();
			(*it)->write(box);
			elements.push_back(box);
		}
		main->set(createTag("boxes"), elements);
		return main;
	}
	
    sDico Page::toDico(string const& text)
    {
        sDico dico = createDico();
        if(dico && text.size())
        {
            bool mode = false;
            string word;
            string key = "name";
            ElemVector elements;
            istringstream iss(text);
            while(iss >> word)
            {
                if(mode)
                {
                    if(word[0] == '@')
                    {
                        dico->set(createTag(key), elements);
                        elements.clear();
                        key = word.c_str()+1;
                    }
                    else
                    {
                        if(isdigit(word[0]))
                        {
                            if(word.find('.'))
                            {
                                elements.push_back(atof(word.c_str()));
                            }
                            else
                            {
                                elements.push_back(atol(word.c_str()));
                            }
                        }
                        else
                        {
                            elements.push_back(createTag(word));
                        }
                    }
                }
                else
                {
                    dico->set(createTag(key), createTag(word));
                    key = "arguments";
                    mode = true;
                }
            }
            dico->set(createTag(key), elements);
            dico->set(createTag("text"), createTag(text));
        }
        return dico;
    }
}
















