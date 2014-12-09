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
    class Clock : public enable_shared_from_this<Clock>
    {
    private:
        atomic_ulong m_used;
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick(wClock clock, unsigned long ms, wBox box);
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick_elements(wClock clock, unsigned long ms, wBox box, ElemVector const& elements);
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Clock() : m_used(0)
        {
            ;
        }
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Clock()
        {
            ;
        }
        
        //! Clock creator.
        /** This function create a new clock.
         @return The clock.
         */
        static inline sClock create()
        {
            return make_shared<Clock>();
        }
        
        //! Clock creator.
        /** This function create a new clock.
         @param  box        The box that will be used.
         @param  ms         The delay time in milliseconds.
         */
        inline void delay(wBox box, const unsigned long ms)
        {
            thread(tick, shared_from_this(), ms, box).detach();
        }
        
        //! Clock creator.
        /** This function create a new clock.
         @param  box        The box that will be used.
         @param  elements   The elements that will be send to the function.
         @param  ms         The delay time in milliseconds.
         */
        inline void delay(wBox box, ElemVector const& elements, const unsigned long ms)
        {
            thread(tick_elements, shared_from_this(), ms, box, elements).detach();
        }
    };
};


#endif


