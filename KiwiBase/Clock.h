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

#include "Tools.h"

namespace Kiwi
{
    class Clock
    {
    private:
        sBox m_box;
        //template<class... arguments> static void tick(Clock* clock, unsigned long ms, sBox box, arguments&&... argument);
        static void tick(Clock* clock, unsigned long ms, sBox box);
    public:
        Clock(sBox box) :
        m_box(box)
        {
            ;
        }
        
        Clock()
        {
            ;
        }
        
        inline void delay(const unsigned long ms)
        {
            if(m_box)
            {
                thread(tick, this, ms, m_box).detach();
            }
        }
        /*
        template<class... Args> inline void delay(const unsigned long ms, Args&&... arguments)
        {
            if(m_box)
            {
                thread(this, ms, m_box, forward<Args>(arguments)...).detach();
            }
        }*/
    };
};


#endif


