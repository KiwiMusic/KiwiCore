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
    
    Beacon::Beacon(string const& name) : m_name(name)
    {
        
    }
    
    Beacon::~Beacon()
    {
        m_boxes.clear();
    }
    
    void Beacon::bind(sBox box)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            m_boxes.insert(box);
        }
    }
    
    void Beacon::unbind(sBox box)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            m_boxes.erase(box);
        }
    }
    
    // ================================================================================ //
    //                                  BEACON FACTORY                                  //
    // ================================================================================ //
    
    Beacon::Factory::Factory()
    {
        
    }
    
    Beacon::Factory::~Factory()
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
            sBeacon newBeacon = make_shared<Beacon>(name);
            m_beacons[name] = newBeacon;
            return newBeacon;
        }
    }
}









