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
#include "Instance.h"

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
            for(auto it = m_boxes.begin(); it != m_boxes.end(); )
            {
                sBox other = (*it).lock();
                if(other)
                {
                    if(other == box)
                    {
                        return;
                    }
                    ++it;
                }
                else
                {
                    it = m_boxes.erase(it);
                }
            }
        }
    }
    
    void Beacon::unbind(sBox box)
    {
        if(box)
        {
            lock_guard<mutex> guard(m_mutex);
            for(auto it = m_boxes.begin(); it != m_boxes.end(); )
            {
                sBox other = (*it).lock();
                if(other && other != box)
                {
                    ++it;
                }
                else
                {
                    it = m_boxes.erase(it);
                }
            }
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
    
    sBeacon Beacon::create(sBox box, string const& name)
    {
        sInstance instance = box->getInstance();
        if(instance)
        {
            return instance->createBeacon(name);
        }
        else
        {
            return nullptr;
        }
    }
}









