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


#include "Console.h"
#include "Instance.h"

namespace Kiwi
{
    sDspDeviceManager Console::device;
    map<sTag, sInstance> Console::m_instances;
    
    set<Console::wListener,
    owner_less<Console::wListener>> Console::m_listeners;
    mutex Console::m_mutex;
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    void Console::bind(shared_ptr<Console::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_mutex);
            m_listeners.insert(listener);
        }
    }
    
    void Console::unbind(shared_ptr<Console::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_mutex);
            m_listeners.erase(listener);
        }
    }

    bool Console::receive(string const& message)
    {
        sInstance instance;
        Vector args;//(Atom::createVector(message));
        if(!args.empty())
        {
            
        }
        string target;
        string command;
        string word;
        //vector<string> args;
        istringstream iss(message);
        iss >> command;
        iss >> target;
        
        if(command == "exit")
        {
            return false;
        }
        if(target == "instance")
        {
            if(command == "create")
            {
                string name;
                iss >> name;
                Instance::create(device, name);
            }
            else if(command == "delete")
            {
                
            }
        }
        else
        {
            auto it = m_instances.find(Tag::create(target));
            if(it != m_instances.end())
            {
                instance = it->second;
            }
            else
            {
                error("The target \"" + target + "\" doesn't exist.");
            }
        }
        if(instance)
        {
            if(command == "create")
            {
                
            }
            else if(command == "delete")
            {
                
            }
            else if(command == "set")
            {
                Vector inputs;
                instance->set(inputs);
            }
            else if(command == "get")
            {
                
            }
            else
            {
                error("The console accepts only the commands \"create\", \"delete\", \"set\", \"get\" and \"exit\".");
            }
        }
        
        return true;
    }
    
    void Console::post(string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cout << message << endl;
#endif
        shared_ptr<const Message> mess = make_shared<Message>(nullptr, nullptr, nullptr, Message::Post, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    void Console::post(scObject object, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << object->getText() << " : " << message << endl;
#endif
        scInstance instance = nullptr;
        scPatcher  patcher  = nullptr;
        if(object)
        {
            instance= object->getInstance();
            patcher = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, patcher, object, Message::Post, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    void Console::warning(string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "warning : " << message << endl;
#endif
        shared_ptr<const Message> mess = make_shared<Message>(nullptr, nullptr, nullptr, Message::Warning, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    void Console::warning(scObject object, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "warning : " << object->getName()->getName() << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Patcher> patcher = nullptr;
        if(object)
        {
            instance    = object->getInstance();
            patcher     = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, patcher, object, Message::Warning, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    void Console::error(string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "error : " << message << endl;
#endif
        shared_ptr<const Message> mess = make_shared<Message>(nullptr, nullptr, nullptr, Message::Error, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    void Console::error(scObject object, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "error : " << object->getName()->getName() << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Patcher> patcher = nullptr;
        if(object)
        {
            instance    = object->getInstance();
            patcher        = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, patcher, object, Message::Error, message);
        lock_guard<mutex> guard(m_mutex);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->receive(mess);
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
    
}









