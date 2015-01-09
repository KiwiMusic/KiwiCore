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

#include "Iolets.h"
#include "Box.h"

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    bool Iolet::has(sBox box, unsigned long index) const noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Iolet::append(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    return false;
                }
            }
            m_connections.push_back({box, index});
            sort(m_connections.begin(), m_connections.end());
            box->bind(shared_from_this(), Box::Tag_position, Attr::ValueChanged);
            return true;
        }
        return false;
    }
    
    bool Iolet::erase(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
            {
                sBox cbox = m_connections[i].box.lock();
                if(cbox && box == cbox && m_connections[i].index == index)
                {
                    m_connections.erase(m_connections.begin() + i);
                    box->unbind(shared_from_this(), Box::Tag_position, Attr::ValueChanged);
                    return false;
                }
            }
        }
        return false;
    }
    
    void Iolet::notify(Attr::sManager manager, sAttr attr, Attr::Notification type)
    {
        lock_guard<mutex> guard(m_mutex);
        sort(m_connections.begin(), m_connections.end());
    }
    
    void Outlet::send(ElemVector const& elements) const noexcept
    {
        lock_guard<mutex> guard(m_mutex);
        for(vector<Connection>::size_type i = 0; i < m_connections.size(); i++)
        {
            sBox receiver       = m_connections[i].box.lock();
            unsigned long inlet  = m_connections[i].index;
            if(receiver)
            {
                if(++receiver->m_stack_count < 256)
                {
                    if(!elements.empty() && elements[0].isTag() && toString(elements[0])[0] == '@')
                    {
                        ElemVector attrvec;
                        attrvec.assign(elements.begin()+1, elements.end());
                        if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                        {
                            Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                        }
                    }
                    else if(!receiver->receive(inlet, elements))
                    {
                        Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                    }
                }
                else if(receiver->m_stack_count  == 256)
                {
                    Console::error(receiver, "Stack overflow");
                    if(!elements.empty() && elements[0].isTag() && toString(elements[0])[0] == '@')
                    {
                        ElemVector attrvec;
                        attrvec.assign(elements.begin()+1, elements.end());
                        if(!receiver->Attr::Manager::setAttributeValue(elements[0], attrvec))
                        {
                            Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                        }
                    }
                    else if(!receiver->receive(inlet, elements))
                    {
                        Console::error(receiver, "wrong elements \"" + toString(elements) + "\"");
                    }
                }
                else
                {
                    Console::error(receiver, "Stack overflow");
                }
                receiver->m_stack_count--;
            }
        }
    }
    
    bool Iolet::Connection::operator<(Connection const& other) const noexcept
    {
        sBox box1 = this->box.lock();
        sBox box2 = other.box.lock();
        if(box1 && box2)
        {
            if(box1->getPosition().x() == box2->getPosition().x())
            {
                return box1->getPosition().y() < box2->getPosition().y();
            }
            return box1->getPosition().x() < box2->getPosition().x();
        }
        return false;
    }
}
















