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
    // ================================================================================ //
    //                                      CLOCK                                       //
    // ================================================================================ //
    
    //! The clock is used to defer the execution of a function.
    /**
     The clock can be used by a box to call of one of the tick funtions after a specific delay. The clock creates a new thread and detach a new thread that will sleep for a specific time before calling the tick funtion of the box.
     @see Box.
     */
    class Clock
    {
    private:
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick(Clock* clock, unsigned long ms, sBox box);
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick_elements(Clock* clock, unsigned long ms, sBox box, ElemVector const& elements);
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Clock(sBox box, const unsigned long ms)
        {
            if(box)
            {
                thread(tick, this, ms, box).detach();
            }
        }
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Clock(sBox box, const unsigned long ms, ElemVector const& elements)
        {
            if(box)
            {
                thread(tick_elements, this, ms, box, elements).detach();
            }
        }
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Clock()
        {
            ;
        }
    
    public:
        
        //! Clock creator.
        /** This function create a new clock.
         @param  box    The box that will be used.
         @param  ms     The delay time in milliseconds.
         */
        static inline void create(sBox box, const unsigned long ms)
        {
            Clock clock(box, ms);
        }
        
        //! Clock creator.
        /** This function create a new clock.
         @param  box        The box that will be used.
         @param  elements   The elements that will be send to the function.
         @param  ms         The delay time in milliseconds.
         */
        static inline void create(sBox box, ElemVector const& elements, const unsigned long ms)
        {
            Clock clock(box, ms, elements);
        }
    };
};


#endif


