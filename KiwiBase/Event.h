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
#include "Point.h"

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
        
        // ================================================================================ //
        //                                      FOCUS                                       //
        // ================================================================================ //
        
        //! The focus event
        /**
         The focus event
         */
        class Focus
        {
        public:
            enum Type
            {
                In = 0,
                Out = 1,
            };
        };
        
        
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
            
        private:
        
            const Type      m_type;
            const Point     m_position;
            const Point		m_position_down;
            const Point     m_wheel_offset;
            const long      m_modifiers;
			const bool		m_was_clicked;
            const ulong m_nclicks;
			
        public:
            Mouse(Type type, int x, int y, long mod, double wheel_x, double wheel_y, bool was_clicked, int down_x, int down_y, ulong nclicks) noexcept
            : m_type(type), m_position(x, y), m_position_down(down_x, down_y), m_wheel_offset(wheel_x, wheel_y), m_modifiers(mod), m_was_clicked(was_clicked), m_nclicks(nclicks)
            {
                ;
            }
            
            Mouse(Type type, Point position, long mod, Point wheel, bool was_clicked, Point down_position, ulong nclicks) noexcept
            : m_type(type), m_position(position), m_position_down(down_position), m_wheel_offset(wheel), m_modifiers(mod), m_was_clicked(was_clicked), m_nclicks(nclicks)
            {
                ;
            }
            
            virtual ~Mouse()
            {
                ;
            }
            
            inline Type getType() const noexcept
            {
                return m_type;
            }
            
            inline bool isUnknown() const noexcept
            {
                return m_type == Unknown;
            }
            
            inline bool isEnter() const noexcept
            {
                return m_type == Enter;
            }
            
            inline bool isLeave() const noexcept
            {
                return m_type == Leave;
            }
            
            inline bool isMove() const noexcept
            {
                return m_type == Move;
            }
            
            inline bool isDrag() const noexcept
            {
                return m_type == Drag;
            }
            
            inline bool isDown() const noexcept
            {
                return m_type == Down;
            }
            
            inline bool isUp() const noexcept
            {
                return m_type == Up;
            }
            
            inline bool isDoubleClick() const noexcept
            {
                return m_type == DoubleClick;
            }
            
            inline bool isWheel() const noexcept
            {
                return m_type == Wheel;
            }
            
            inline double getX() const noexcept
            {
                return m_position.x();
            }
            
            inline double getY() const noexcept
            {
                return m_position.y();
            }
            
            inline Point getPosition() const noexcept
            {
                return m_position;
            }
            
            inline double getDownX() const noexcept
            {
                return m_position_down.x();
            }
            
            inline double getDownY() const noexcept
            {
                return m_position_down.y();
            }
            
            inline Point getDownPosition() const noexcept
            {
                return m_position_down;
            }
            
            inline double getWheelOffsetX() const noexcept
            {
                return m_wheel_offset.x();
            }
            
            inline double getWheelOffsetY() const noexcept
            {
                return m_wheel_offset.y();
            }
            
            inline Point getWheelOffset() const noexcept
            {
                return m_wheel_offset;
            }
            
            inline long getModifiers() const noexcept
            {
                return m_modifiers;
            }
            
            inline bool hasNoModifier() const noexcept
            {
                return m_modifiers == Nothing;
            }
            
            inline bool hasShift() const noexcept
            {
                return m_modifiers & Shift;
            }
            
            inline bool hasCtrl() const noexcept
            {
                return m_modifiers & Ctrl;
            }
            
            inline bool hasAlt() const noexcept
            {
                return m_modifiers & Alt;
            }
            
            inline bool hasLeft() const noexcept
            {
                return m_modifiers & Left;
            }
            
            inline bool hasRight() const noexcept
            {
                return m_modifiers & Right;
            }
            
            inline bool hasMiddle() const noexcept
            {
                return m_modifiers & Middle;
            }
            
            inline bool hasCmd() const noexcept
            {
                return m_modifiers & Cmd;
            }
            
            inline bool hasPopup() const noexcept
            {
                return m_modifiers & Popup;
            }
            
            inline bool wasClicked() const noexcept
            {
                return m_was_clicked;
            }
            
            inline ulong getNumberOfClicks() const noexcept
            {
                return m_nclicks;
            }
            
            virtual void setMouseUnlimited(bool isLimited, bool visibleUntilLimits = false) const = 0;
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
        public:
            enum Code
            {
#ifdef __APPLE__
                Space   = ' ',
                Escape  = 0x1b,
                Return  = 0x0d,
                Tab     = 9,
                Delete  = 0xF728,
                Backspace= 0x7f,
                Insert  = -1,
                Up      = 0xF700,
                Down    = 0xF701,
                Left    = 0xF702,
                Right   = 0xF703
#else
#endif
            };

        private:
            
            const wchar_t   m_character;
            const long      m_modifiers;
        public:
            
            Keyboard(char character, long modifiers) noexcept
            : m_character(character), m_modifiers(modifiers)
            {
                ;
            }
            
            ~Keyboard()
            {
                ;
            }
            
            inline char getCharacter() const noexcept
            {
                char nchar;
                if(wcstombs(&nchar, &m_character, sizeof(char)) != -1)
                {
                    return nchar;
                }
                else
                {
                    nchar = ' ';
                    return nchar;
                }
            }
            
            inline bool isCharacter() const noexcept
            {
                char nchar;
                if(wcstombs(&nchar, &m_character, sizeof(char)) != -1)
                {
                    return true;
                }
                return false;
            }
            
            inline wchar_t getWideCharacter() const noexcept
            {
                return m_character;
            }
            
            inline bool isWideCharacter() const noexcept
            {
                char nchar;
                if(wcstombs(&nchar, &m_character, sizeof(char)) == -1)
                {
                    return true;
                }
                return false;
            }
            
            inline bool isAlphabetic() const noexcept
            {
                return isalpha(m_character);
            }
            
            inline bool isAlphanumeric() const noexcept
            {
                return isalnum(m_character);
            }
            
            inline bool isNumber() const noexcept
            {
                return isdigit(m_character);
            }
            
            inline bool isSpace() const noexcept
            {
                return m_character == Space;
            }
            
            inline bool isEscape() const noexcept
            {
                return m_character == Escape;
            }
            
            inline bool isReturn() const noexcept
            {
                return m_character == Return;
            }
            
            inline bool isTab() const noexcept
            {
                return m_character == Tab;
            }
            
            inline bool isDelete() const noexcept
            {
                return m_character == 0xF728;
            }
            
            inline bool isBackspace() const noexcept
            {
                return m_character == Backspace;
            }
            
            inline bool isInsert() const noexcept
            {
                return m_character == Insert;
            }
            
            inline bool isUp() const noexcept
            {
                return m_character == Up;
            }
            
            inline bool isDown() const noexcept
            {
                return m_character == Down;
            }
            
            inline bool isLeft() const noexcept
            {
                return m_character == Left;
            }
            
            inline bool isRight() const noexcept
            {
                return m_character == Right;
            }
            
            inline long getModifiers() const noexcept
            {
                return m_modifiers;
            }
        
            inline bool hasNoModifier() const noexcept
            {
                return m_modifiers == Nothing;
            }
            
            inline bool hasShift() const noexcept
            {
                return m_modifiers & Shift;
            }
            
            inline bool hasCtrl() const noexcept
            {
                return m_modifiers & Ctrl;
            }
            
            inline bool hasAlt() const noexcept
            {
                return m_modifiers & Alt;
            }
            
            inline bool hasLeft() const noexcept
            {
                return m_modifiers & Left;
            }
            
            inline bool hasRight() const noexcept
            {
                return m_modifiers & Right;
            }
            
            inline bool hasMiddle() const noexcept
            {
                return m_modifiers & Middle;
            }
            
            inline bool hasCmd() const noexcept
            {
                return m_modifiers & Cmd;
            }
            
            inline bool hasPopup() const noexcept
            {
                return m_modifiers & Popup;
            }
        };
    
    };
    
    inline string toString(Event::Mouse const& event)
    {
        string text = "Mouse ";
        switch(event.getModifiers())
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
                text += "Wheel (" + toString(event.getWheelOffsetX()) + ", " + toString(event.getWheelOffsetY()) + ") ";
                break;
        }
        text += "[" + toString(event.getX()) + " " + toString(event.getY()) + "] ";
        if(event.hasShift())
        {
            text += "Shift ";
        }
        if(event.hasCtrl())
        {
            text += "Ctrl ";
        }
        if(event.hasAlt())
        {
            text += "Alt ";
        }
        if(event.hasLeft())
        {
            text += "Left ";
        }
        if(event.hasRight())
        {
            text += "Right ";
        }
        if(event.hasMiddle())
        {
            text += "Middle ";
        }
        if(event.hasCmd())
        {
            text += "Cmd ";
        }
        if(event.hasPopup())
        {
            text += "Popup ";
        }
        return text;
    }

}

#endif
