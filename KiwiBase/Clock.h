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

#ifndef __DEF_KIWI_CLOCK__
#define __DEF_KIWI_CLOCK__

#include "Element.h"

namespace Kiwi
{
    class Clock
    {
    private:
        static void tick_elements(Clock* clock, unsigned long ms, sBox box, ElemVector const& elements);
        static void tick(Clock* clock, unsigned long ms, sBox box);
    public:
        Clock(sBox box, const unsigned long ms)
        {
            if(box)
            {
                thread(tick, this, ms, box).detach();
            }
        }
        
        Clock(sBox box, const unsigned long ms, ElemVector const& elements)
        {
            if(box)
            {
                thread(tick_elements, this, ms, box, elements).detach();
            }
        }
        
        ~Clock()
        {
            ;
        }
    };
};


#endif


