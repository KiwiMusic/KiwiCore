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


#include "Beacon.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      BEACON                                      //
    // ================================================================================ //
    
    Beacon::Beacon(string const& name) noexcept : m_name(name)
    {
        
    }
    
    Beacon::~Beacon() noexcept
    {
        m_castaways.clear();
    }
    
    void Beacon::bind(sCastaway castaway)
    {
        if(castaway)
        {
            lock_guard<mutex> guard(m_mutex);
            for(auto it = m_castaways.begin(); it != m_castaways.end(); )
            {
                sCastaway other = (*it).lock();
                if(other)
                {
                    if(other == castaway)
                    {
                        return;
                    }
                    ++it;
                }
                else
                {
                    it = m_castaways.erase(it);
                }
            }
        }
    }
    
    void Beacon::unbind(sCastaway castaway)
    {
        if(castaway)
        {
            lock_guard<mutex> guard(m_mutex);
            for(auto it = m_castaways.begin(); it != m_castaways.end(); )
            {
                sCastaway other = (*it).lock();
                if(other && other != castaway)
                {
                    ++it;
                }
                else
                {
                    it = m_castaways.erase(it);
                }
            }
        }
    }
    
    // ================================================================================ //
    //                                  BEACON FACTORY                                  //
    // ================================================================================ //
    
    Beacon::Factory::Factory() noexcept
    {
        
    }
    
    Beacon::Factory::~Factory() noexcept
    {
        m_beacons.clear();
    }
    
    sBeacon Beacon::Factory::createBeacon(string const& name)
    {
        lock_guard<mutex> guard(m_factory_mutex);
        auto it = m_beacons.find(name);
        if(it != m_beacons.end())
        {
            return it->second;
        }
        else
        {
            sBeacon beacon = make_shared<Beacon>(name);
            if(beacon)
            {
                m_beacons[name] = beacon;
            }
            return beacon;
        }
    }
}









