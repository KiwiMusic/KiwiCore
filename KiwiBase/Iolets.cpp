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
    
    bool Inlet::has(sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxFrom() == getBox(i) && link->getOutletIndex() == getOutletIndex(i))
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Inlet::append(sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxFrom() == getBox(i) && link->getOutletIndex() == getOutletIndex(i))
                {
                    return false;
                }
            }
            m_links.push_back(link);
            return true;
        }
        return false;
    }
    
    bool Inlet::erase(const sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxFrom() == getBox(i) && link->getOutletIndex() == getOutletIndex(i))
                {
                    m_links.erase(m_links.begin()+(long)i);
                    return true;
                }
            }
        }
        return false;
    }
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    bool Outlet::has(sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxTo() == getBox(i) && link->getInletIndex() == getInletIndex(i))
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Outlet::append(sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxTo() == getBox(i) && link->getInletIndex() == getInletIndex(i))
                {
                    return false;
                }
            }
            m_links.push_back(link);
            return true;
        }
        return false;
    }
    
    bool Outlet::erase(sLink link) noexcept
    {
        if(link)
        {
            for(unsigned long i = 0; i < getNumberOfLinks(); i++)
            {
                if(link->getBoxTo() == getBox(i) && link->getInletIndex() == getInletIndex(i))
                {
                    m_links.erase(m_links.begin()+(long)i);
                    return true;
                }
            }
        }
        return false;
    }
}
















