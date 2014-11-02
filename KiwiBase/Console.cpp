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
#include "Box.h"

namespace Kiwi
{
    unordered_set<weak_ptr<Console::Listener>,
    weak_ptr_hash<Console::Listener>,
    weak_ptr_equal<Console::Listener>> Console::m_listeners;
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    void Console::bind(weak_ptr<Console::Listener> listener)
    {
        m_listeners.insert(listener);
    }
    
    void Console::unbind(weak_ptr<Console::Listener> listener)
    {
        m_listeners.erase(listener);
    }
    
    void Console::post(string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cout << message << "\n";
#endif
        auto it = m_listeners.begin();
        while(it != m_listeners.end())
        {
            shared_ptr<Console::Listener> to = (*it).lock();
            if(to)
            {
                to->post(nullptr, nullptr, message);
            }
            ++it;
        }
        }
        
        void Console::post(shared_ptr<const Box> box, string const& message) noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << toString(box) << " : " << message << "\n";
#endif
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<Console::Listener> to = (*it).lock();
                if(to)
                {
                    to->post(nullptr, box, message);
                }
                ++it;
            }
        }
        
        void Console::warning(string const& message) noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << "warning : " << message << "\n";
#endif
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<Console::Listener> to = (*it).lock();
                if(to)
                {
                    to->warning(nullptr, nullptr, message);
                }
                ++it;
            }
        }
        
        void Console::warning(shared_ptr<const Box> box, string const& message) noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << toString(box) << " : " << message << "\n";
#endif
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<Console::Listener> to = (*it).lock();
                if(to)
                {
                    to->warning(nullptr, box, message);
                }
                ++it;
            }
        }
        
        void Console::error(string const& message) noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << "error : " << message << "\n";
#endif
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<Console::Listener> to = (*it).lock();
                if(to)
                {
                    to->error(nullptr, nullptr, message);
                }
                ++it;
            }
        }
        
        void Console::error(shared_ptr<const Box> box, string const& message) noexcept
        {
#if defined(DEBUG) || defined(NO_GUI)
            cerr << toString(box) << " : " << message << "\n";
#endif
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                shared_ptr<Console::Listener> to = (*it).lock();
                if(to)
                {
                    to->error(nullptr, box, message);
                }
                ++it;
            }
        }

}









