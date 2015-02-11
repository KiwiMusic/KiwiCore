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
#include "Object.h"

namespace Kiwi
{
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
        cerr << toString(object->getText()) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Patcher> page = nullptr;
        if(object)
        {
            instance    = object->getInstance();
            page        = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, object, Message::Post, message);
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
        cerr << "warning : " << toString(object) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Patcher> page = nullptr;
        if(object)
        {
            instance    = object->getInstance();
            page        = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, object, Message::Warning, message);
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
        cerr << "error : " << toString(object) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Patcher> page = nullptr;
        if(object)
        {
            instance    = object->getInstance();
            page        = object->getPatcher();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, object, Message::Error, message);
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









