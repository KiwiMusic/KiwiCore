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


#include "KiwiClock.h"

namespace Kiwi
{
    void Clock::clock_tick(const wClock clock, const ulong ms)
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
                if(!nclock->m_used)
                {
                    nclock->tick();
                }
            }
        }
    }
    
    void Clock::clock_tick_atoms(const wClock clock, const ulong ms, Vector const& atoms)
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
                if(!nclock->m_used)
                {
                    nclock->tick(atoms);
                }
            }
        }
    }
}









