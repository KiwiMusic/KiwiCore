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
        
    void Console::post(scBox box, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << toString(box->getText()) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Page> page = nullptr;
        if(box)
        {
            instance    = box->getInstance();
            page        = box->getPage();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, box, Message::Post, message);
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
        
    void Console::warning(scBox box, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "warning : " << toString(box) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Page> page = nullptr;
        if(box)
        {
            instance    = box->getInstance();
            page        = box->getPage();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, box, Message::Warning, message);
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
        
    void Console::error(scBox box, string const& message) noexcept
    {
#if defined(DEBUG) || defined(NO_GUI)
        cerr << "error : " << toString(box) << " : " << message << endl;
#endif
        shared_ptr<const Kiwi::Instance> instance = nullptr;
        shared_ptr<const Kiwi::Page> page = nullptr;
        if(box)
        {
            instance    = box->getInstance();
            page        = box->getPage();
        }
        shared_ptr<const Message> mess = make_shared<Message>(instance, page, box, Message::Error, message);
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
        
    // ================================================================================ //
    //                                  CONSOLE HISTORY                                 //
    // ================================================================================ //
        
    Console::History::History() :
    m_sort(Console::History::Index)
    {
        ;
    }

    Console::History::~History()
    {
        m_messages.clear();
    }
    
    shared_ptr<Console::History> Console::History::create()
    {
        shared_ptr<Console::History> history = make_shared<Console::History>();
        Console::bind(history);
        return history;
    }
    
    void Console::History::receive(shared_ptr<const Message> message)
    {
        lock_guard<mutex> guard(m_hmutex);
        size_t index = m_messages.size() + 1;
        m_messages.push_back({message, index});
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::History::Listener> to = (*it).lock();
            if(to)
            {
                to->historyHasChanged(shared_from_this());
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
    
    void Console::History::clear()
    {
        lock_guard<mutex> guard(m_hmutex);
        m_messages.clear();
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::History::Listener> to = (*it).lock();
            if(to)
            {
                to->historyHasChanged(shared_from_this());
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
    
    size_t Console::History::size()
    {
        //lock_guard<mutex> guard(m_hmutex);
        return m_messages.size();
    }
    
    shared_ptr<const Console::Message> Console::History::get(size_t index)
    {
        lock_guard<mutex> guard(m_hmutex);
        if(index < m_messages.size())
        {
            return m_messages[index].m_message;
        }
        else
        {
            return nullptr;
        }
    }
    
    void Console::History::erase(size_t index)
    {
        lock_guard<mutex> guard(m_hmutex);
        if(index < m_messages.size())
        {
            m_messages.erase(m_messages.begin()+(long)index);
            std::sort(m_messages.begin(), m_messages.end(), compareIndex);
            for(size_t i = 0; i < m_messages.size(); i++)
            {
                m_messages[i].m_index = i+1;
            }
            sort(m_sort);
            for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
            {
                shared_ptr<Console::History::Listener> to = (*it).lock();
                if(to)
                {
                    to->historyHasChanged(shared_from_this());
                }
                else
                {
                    ++it;
                    m_listeners.erase(to);
                }
            }
        }
    }
    
    void Console::History::erase(size_t begin, size_t last)
    {
        lock_guard<mutex> guard(m_hmutex);
        if(begin < last && last < m_messages.size())
        {
            m_messages.erase(m_messages.begin()+(long)begin, m_messages.begin()+(long)last);
            std::sort(m_messages.begin(), m_messages.end(), compareIndex);
            for(size_t i = 0; i < m_messages.size(); i++)
            {
                m_messages[i].m_index = i+1;
            }
            sort(m_sort);
            for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
            {
                shared_ptr<Console::History::Listener> to = (*it).lock();
                if(to)
                {
                    to->historyHasChanged(shared_from_this());
                }
                else
                {
                    ++it;
                    m_listeners.erase(to);
                }
            }
        }
    }
        
    void Console::History::erase(vector<size_t>& indices)
    {
        lock_guard<mutex> guard(m_hmutex);
        size_t max = m_messages.size();
        std::sort(indices.begin(), indices.end());
        std::reverse(indices.begin(), indices.end());
        for(size_t i = 0; i < indices.size(); i++)
        {
            if(indices[i] < max)
            {
                m_messages.erase(m_messages.begin()+(long)indices[i]);
                --max;
            }
        }
        std::sort(m_messages.begin(), m_messages.end(), compareIndex);
        for(size_t i = 0; i < m_messages.size(); i++)
        {
            m_messages[i].m_index = i+1;
        }
        sort(m_sort);
        for(auto it = m_listeners.begin(); it !=  m_listeners.end(); ++it)
        {
            shared_ptr<Console::History::Listener> to = (*it).lock();
            if(to)
            {
                to->historyHasChanged(shared_from_this());
            }
            else
            {
                ++it;
                m_listeners.erase(to);
            }
        }
    }
        
    bool Console::History::compareIndex(Console::History::MessageHolder const& i, Console::History::MessageHolder const& j)
    {
        return i.m_index < j.m_index;
    }
        
    bool Console::History::compareName(Console::History::MessageHolder const& i, Console::History::MessageHolder const& j)
    {
        string text1, text2;
        shared_ptr<const Message> mess;
        mess = i.m_message;
        if(mess)
        {
            scBox box = mess->box.lock();
            if(box)
            {
                text1 = toString(box);
            }
        }
        mess = j.m_message;
        if(mess)
        {
            scBox box = mess->box.lock();
            if(box)
            {
                text2 = toString(box);
            }
        }
        return text1.compare(text2);
    }
    
    bool Console::History::compareContent(Console::History::MessageHolder const& i, Console::History::MessageHolder const& j)
    {
        string text1, text2;
        shared_ptr<const Message> mess;
        mess = i.m_message;
        if(mess)
        {
            text1 = mess->content;
        }
        mess = j.m_message;
        if(mess)
        {
            text2 = mess->content;
        }
        return text1.compare(text2);
    }
        
    bool Console::History::compareKind(Console::History::MessageHolder const& i, Console::History::MessageHolder const& j)
    {
        Console::Message::Kind kind1 = Message::Kind::Empty, kind2 =  Message::Kind::Empty;
        shared_ptr<const Message> mess;
        mess = i.m_message;
        if(mess)
        {
            kind1 = mess->kind;
        }
        mess = j.m_message;
        if(mess)
        {
            kind2 = mess->kind;
        }
        return kind1 < kind2;
    }
    
    void Console::History::sort(Sort type)
    {
        m_sort = type;
        switch(m_sort)
        {
            case Index:
                std::sort(m_messages.begin(), m_messages.end(), compareIndex);
                break;
            case Name:
                std::sort(m_messages.begin(), m_messages.end(), compareName);
                break;
            case Kind:
                std::sort(m_messages.begin(), m_messages.end(), compareKind);
                break;
            case Content:
                std::sort(m_messages.begin(), m_messages.end(), compareContent);
                break;
        }
    }
    
    void Console::History::bind(shared_ptr<Console::History::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_hmutex);
            m_listeners.insert(listener);
        }
    }
    

    void Console::History::unbind(shared_ptr<Console::History::Listener> listener)
    {
        if(listener)
        {
            lock_guard<mutex> guard(m_hmutex);
            m_listeners.erase(listener);
        }
    }
    
}









