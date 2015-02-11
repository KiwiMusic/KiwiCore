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


#include "Clock.h"
#include "Object.h"

namespace Kiwi
{
    void Clock::tick_atoms(wClock clock, ulong ms, wMaker maker, vector<Atom> const& atoms)
    {
        sClock nclock = clock.lock();
        if(nclock)
        {
            nclock->m_used++;
            nclock.reset();
            
            this_thread::sleep_for(chrono::milliseconds(ms));
            
            nclock = clock.lock();
            if(nclock)
            {
                nclock->m_used--;
                sMaker nmaker = maker.lock();
                if(!nclock->m_used && nmaker)
                {
                    nmaker->tick(atoms);
                }
            }
        }
    }
    
    void Clock::tick(wClock clock, ulong ms, wMaker maker)
    {
        sClock nclock = clock.lock();
        if(nclock)
        {
            nclock->m_used++;
            nclock.reset();
            
            this_thread::sleep_for(chrono::milliseconds(ms));
            
            nclock = clock.lock();
            if(nclock)
            {
                nclock->m_used--;
                sMaker nmaker = maker.lock();
                if(!nclock->m_used && nmaker)
                {
                    nmaker->tick();
                }
            }
        }
    }
    
    void Clock::delay(sMaker maker, const ulong ms)
    {
        thread(tick, shared_from_this(), ms, maker).detach();
    }
    

    void Clock::delay(sMaker maker, vector<Atom> const& atoms, const ulong ms)
    {
        thread(tick_atoms, shared_from_this(), ms, maker, atoms).detach();
    }
    
    
    void Clock::delay(sObject object, const ulong ms)
    {
        sMaker maker = dynamic_pointer_cast<Clock::Maker>(object);
        if(maker)
        {
            thread(tick, shared_from_this(), ms, maker).detach();
        }
    }
    
    void Clock::delay(sObject object, vector<Atom> const& atoms, const ulong ms)
    {
        sMaker maker = dynamic_pointer_cast<Clock::Maker>(object);
        if(maker)
        {
            thread(tick_atoms, shared_from_this(), ms, maker, atoms).detach();
        }
    }
}









