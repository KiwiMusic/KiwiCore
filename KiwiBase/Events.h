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

#ifndef __DEF_KIWI_EVENTS__
#define __DEF_KIWI_EVENTS__

#include "Defs.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      EVENTS                                      //
    // ================================================================================ //
    
    //! The events
    /**
     The evennts
     */
    class Events
    {
    public:
        
        // ================================================================================ //
        //                                      MOUSE                                       //
        // ================================================================================ //
        
        //! The mouse event
        /**
         The mouse event
         */
        class Mouse
        {
        public:
            enum Type
            {
                Unknown     = 0,
                Enter       = 1,
                Leave       = 2,
                Move        = 3,
                Drag        = 4,
                Down        = 5,
                Up          = 6,
                DoubleClick = 7,
                Wheel       = 8
            };
            
            enum Modifier
            {
                Nothing     = 0,
                Shift       = 1,
                Ctrl        = 2,
                Alt         = 4,
                Left        = 16,
                Right       = 32,
                Middle      = 64,
                
#ifdef __APPLE__
                Cmd          = 8,
                Popup        = Right | Ctrl,
#else
                Cmd          = Ctrl,
                Popup        = Right
#endif
            };
            
            const Type      type;
            const int       x;
            const int       y;
            const long      modifiers;
            const double    wheel;
            
            Mouse(Type _type, int _x, int _y, long _mod, double _wheel) : type(_type), x(_x), y(_y), modifiers(_mod), wheel(_wheel)
            {
                ;
            }
            
            ~Mouse()
            {
                ;
            }
        };
        
        // ================================================================================ //
        //                                      KEYBOARD                                    //
        // ================================================================================ //
        
        //! The keyboard event
        /**
         The keyboard event
         */
        class Keyboard
        {
            
        };
    
    };
}

#endif
