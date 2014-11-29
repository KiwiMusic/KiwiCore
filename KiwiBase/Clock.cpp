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
#include "Box.h"

namespace Kiwi
{
    /*
    template<class... arguments> void Clock::tick(Clock* clock, unsigned long ms, sBox box, arguments&&... argument)
    {
        if(clock && box)
        {
            this_thread::sleep_for(std::chrono::milliseconds(ms));
            box->tick();
        }
    }*/
    
    void Clock::tick(Clock* clock, unsigned long ms, sBox box)
    {
        if(clock && box)
        {
            this_thread::sleep_for(std::chrono::milliseconds(ms));
            box->tick();
        }
    }
}









