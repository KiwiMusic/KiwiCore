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
    
    bool Inlet::append(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            for(vector<sSocket>::size_type i = 0; i < m_sockets.size(); i++)
            {
                if(box == getBox((unsigned long)i) && index == getOutletIndex((unsigned long)i))
                {
                    return false;
                }
            }
            m_sockets.push_back(make_shared<Socket>(box, index));
            return true;
        }
        return false;
    }
    
    bool Inlet::erase(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            for(vector<sSocket>::size_type i = 0; i < m_sockets.size(); i++)
            {
                if(box == getBox((unsigned long)i) && index == getOutletIndex((unsigned long)i))
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
    
    bool Outlet::append(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            for(vector<sSocket>::size_type i = 0; i < m_sockets.size(); i++)
            {
                if(box == getBox((unsigned long)i) && index == getInletIndex((unsigned long)i))
                {
                    return false;
                }
            }
            m_sockets.push_back(make_shared<Socket>(box, index));
            return true;
        }
        return false;
    }
    
    bool Outlet::erase(sBox box, unsigned long index) noexcept
    {
        if(box)
        {
            for(vector<sSocket>::size_type i = 0; i < m_sockets.size(); i++)
            {
                if(box == getBox((unsigned long)i) && index == getInletIndex((unsigned long)i))
                {
                    m_sockets.erase(m_sockets.begin()+(long)i);
                    return true;
                }
            }
        }
        return false;
    }
}
















