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
    Console::Console()
    {
        
    }
    
    Console::~Console()
    {
        
    }
    
    void Console::bind(weak_ptr<ConsoleListener> listener)
    {
        m_listeners.insert(listener);
    }
    
    void Console::unbind(weak_ptr<ConsoleListener> listener)
    {
        m_listeners.erase(listener);
    }
    
    void Console::post(string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cout << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->post(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
            ++it;
        }
    }
    
    void Console::post(const shared_ptr<const Object> object, string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "(" + object->getName()->name() + ") : " << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->post(shared_from_this(), object, message);
            ++it;
        }
    }
    
    void Console::warning(string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "warning : " << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->warning(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
            ++it;
        }
    }
    
    void Console::warning(const shared_ptr<const Object> object, string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "warning (" + object->getName()->name() + ") : " << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->warning(shared_from_this(), object, message);
            ++it;
        }
    }
    
    void Console::error(string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "error : " << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->error(shared_from_this(), shared_ptr<Kiwi::Object>(), message);
            ++it;
        }
    }
    
    void Console::error(const shared_ptr<const Object> object, string message) const noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "error (" + object->getName()->name() + ") : " << message << "\n";
#endif
        set<weak_ptr<ConsoleListener>>::iterator it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<ConsoleListener> to = (*it).lock();
            to->error(shared_from_this(), object, message);
            ++it;
        }
    }
}
















