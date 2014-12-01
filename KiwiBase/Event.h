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
#include "Tools.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      EVENTS                                      //
    // ================================================================================ //
    
    //! The events
    /**
     The evennts
     */
    class Event
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
            const double    wheel_x;
			const double	wheel_y;
			const bool		wasClicked;
			const int		down_x;
			const int		down_y;
            
            Mouse(Type _type, int _x, int _y, long _mod, double _wheel_x, double _wheel_y, bool _wasClicked, int _down_x, int _down_y) noexcept
            : type(_type), x(_x), y(_y), modifiers(_mod), wheel_x(_wheel_x), wheel_y(_wheel_y), wasClicked(_wasClicked), down_x(_down_x), down_y(_down_y)
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
            
            //const Type      type;
            const char      letter;
            const long      modifiers;
            
            Keyboard(char _letter, long _mod) noexcept
            : letter(_letter), modifiers(_mod)
            {
                ;
            }
            
            ~Keyboard()
            {
                ;
            }
        };
    
    };
    
    inline string toString(Event::Mouse const& event)
    {
        string text = "Mouse ";
        switch(event.type)
        {
            case Event::Mouse::Unknown:
                text += "Unknown ";
                break;
            case Event::Mouse::Enter:
                text += "Enter ";
                break;
            case Event::Mouse::Leave:
                text += "Leave ";
                break;
            case Event::Mouse::Move:
                text += "Move ";
                break;
            case Event::Mouse::Drag:
                text += "Drag ";
                break;
            case Event::Mouse::Down:
                text += "Down ";
                break;
            case Event::Mouse::Up:
                text += "Up ";
                break;
            case Event::Mouse::DoubleClick:
                text += "DoubleClick ";
                break;
            default:
                text += "Wheel (" + toString(event.wheel_x) + ", " + toString(event.wheel_y) + ") ";
                break;
        }
        text += "[" + toString(event.x) + " " + toString(event.y) + "] ";
        if(event.modifiers & Event::Mouse::Shift)
        {
            text += "Shift ";
        }
        if(event.modifiers & Event::Mouse::Ctrl)
        {
            text += "Ctrl ";
        }
        if(event.modifiers & Event::Mouse::Alt)
        {
            text += "Alt ";
        }
        if(event.modifiers & Event::Mouse::Left)
        {
            text += "Left ";
        }
        if(event.modifiers & Event::Mouse::Right)
        {
            text += "Right ";
        }
        if(event.modifiers & Event::Mouse::Middle)
        {
            text += "Middle ";
        }
        if(event.modifiers & Event::Mouse::Cmd)
        {
            text += "Cmd ";
        }
        if(event.modifiers & Event::Mouse::Popup)
        {
            text += "Popup ";
        }
        return text;
    }

}

#endif
