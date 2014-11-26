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

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    bool Inlet::has(sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getOutletIndex(i))
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Inlet::append(sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getOutletIndex(i))
                {
                    return false;
                }
            }
            m_sockets.push_back(socket);
            return true;
        }
        return false;
    }
    
    bool Inlet::erase(const sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getOutletIndex(i))
                {
                    m_sockets.erase(m_sockets.begin()+(long)i);
                    return true;
                }
            }
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    bool Outlet::has(sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getInletIndex(i))
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Outlet::append(sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getInletIndex(i))
                {
                    return false;
                }
            }
            m_sockets.push_back(socket);
            return true;
        }
        return false;
    }
    
    bool Outlet::erase(const sSocket socket) noexcept
    {
        if(socket)
        {
            for(unsigned long i = 0; i < getNumberOfSockets(); i++)
            {
                if(socket->getBox() == getBox(i) && socket->getIndex() == getInletIndex(i))
                {
                    m_sockets.erase(m_sockets.begin()+(long)i);
                    return true;
                }
            }
        }
        return false;
    }
}
















