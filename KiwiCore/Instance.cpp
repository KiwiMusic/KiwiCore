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

#include "Instance.h"
#include "../KiwiBoxes/Arithmetic.h"
#include "../KiwiBoxes/ArithmeticTilde.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    Instance::Instance() : m_untitled_pages(0)
    {
        
    }
    
    Instance::~Instance()
    {
        m_pages.clear();
        m_prototypes.clear();
    }
    
    void Instance::init()
    {
        Arithmetic::load(enable_shared_from_this<Instance>::shared_from_this());
        ArithmeticTilde::load(enable_shared_from_this<Instance>::shared_from_this());
    }
    
    void Instance::addObjectPrototype(unique_ptr<Object> object)
    {
        map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(object->getName());
        if(it != m_prototypes.end())
        {
            string message = string("The object prototype ") + (object->getName())->name() + string(" already exist !");
            error(message);
        }
        else
        {
            m_prototypes[object->getName()] = move(object);
        }
    }
    
    shared_ptr<Object> Instance::allocObject(string name)
    {
        return allocObject(createTag(name));
    }
    
    shared_ptr<Object> Instance::allocObject(shared_ptr<Tag> name)
    {
        shared_ptr<Object> object;
        map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(name);
        if(it != m_prototypes.end())
        {
            vector<Element> element;
            return object;
        }
        else
        {
            return object;
        }
    }
    
    shared_ptr<Object> Instance::createObject(string name)
    {
        return createObject(createTag(name));
    }
    
    shared_ptr<Object> Instance::createObject(shared_ptr<Tag> name)
    {
        shared_ptr<Object> object;
        map<shared_ptr<Tag>, unique_ptr<Object>>::iterator it = m_prototypes.find(name);
        if(it != m_prototypes.end())
        {
            vector<Element> element;
            return it->second->create(enable_shared_from_this<Instance>::shared_from_this(), name, element);
        }
        else
        {
            return object;
        }
    }
    
    shared_ptr<Dico> Instance::createDico()
    {
        return make_shared<Dico>(enable_shared_from_this<Instance>::shared_from_this());
    }
    
    shared_ptr<Page> Instance::createPage(string file, string directory)
    {
        if(file.empty())
		{
			file = string("Untitled") + to_string(++m_untitled_pages);
		}
  
        shared_ptr<Page> newPage = make_shared<Page>(enable_shared_from_this<Instance>::shared_from_this(), file, directory);
        m_pages.push_back(newPage);
        return newPage;
    }
    
    void Instance::closePage(shared_ptr<Page> page)
    {
        for(int i = 0; i < m_pages.size(); i++)
        {
            if(m_pages[i] == page)
            {
                m_pages.erase(m_pages.begin()+i);
                break;
            }
        }
    }
    
    void Instance::startDsp(double samplerate, long vectorsize)
    {
        for(int i = 0; i < m_pages.size(); i++)
            m_pages[i]->startDsp(samplerate, vectorsize);
    }
    
    void Instance::tickDsp()
    {
        for(int i = 0; i < m_pages.size(); i++)
            m_pages[i]->tickDsp();
    }
    
    void Instance::stopDsp()
    {
        for(int i = 0; i < m_pages.size(); i++)
            m_pages[i]->stopDsp();
    }
}
















