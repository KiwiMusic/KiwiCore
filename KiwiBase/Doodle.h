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

#ifndef __DEF_KIWI_DOODLE__
#define __DEF_KIWI_DOODLE__

#include "Element.h"

namespace Kiwi
{
    class Doodle;
    
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    class Point
    {
    public:
        double x;
        double y;
        
        Point() noexcept :
        x(0.), y(0.)
        {
            ;
        }
        
        Point(const double _x, const double _y) noexcept :
        x(_x), y(_y)
        {
            ;
        }
        
        Point(ElemVector const& elements) noexcept :
        x(0.), y(0.)
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                y = (double)elements[1];
            }
        }
        
        Point(Point const& pt) noexcept :
        x(pt.x), y(pt.y)
        {
            ;
        }
        
        ~Point()
        {
            ;
        }
        
        inline Point& operator=(ElemVector const& elements) noexcept
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                x = (double)elements[0];
            }
            if(elements.size() > 1 && elements[1].isNumber())
            {
                y = (double)elements[1];
            }
            return *this;
        }
        
        inline Point& operator=(Point const& pt) noexcept
        {
            x = pt.x;
            y = pt.y;
            return *this;
        }
    };
    
    // ================================================================================ //
    //                                      COLOR                                       //
    // ================================================================================ //
    
    class Color
    {
    public:
        double red;
        double green;
        double blue;
        double alpha;
        
        Color() noexcept :
        red(0.), green(0.), blue(0.), alpha(1.)
        {
            ;
        }
        
        Color(const double _red, const double _green, const double _blue, const double _alpha = 1.) noexcept :
        red(clip(_red, 0., 1.)), green(clip(_green, 0., 1.)), blue(clip(_blue, 0., 1.)), alpha(clip(_alpha, 0., 1.))
        {
            ;
        }
        
        Color(ElemVector const& elements) noexcept :
        red(0.), green(0.), blue(0.), alpha(1.)
        {
            const ElemVector::size_type size = elements.size();
            if(size && elements[0].isNumber())
            {
                red = (double)elements[0];
            }
            if(size > 1 && elements[1].isNumber())
            {
                green = (double)elements[1];
            }
            if(size > 2 && elements[2].isNumber())
            {
                blue = (double)elements[1];
            }
            if(size > 2 && elements[3].isNumber())
            {
                alpha = (double)elements[1];
            }
        }
        
        Color(Color const& color) noexcept :
        red(clip(color.red, 0., 1.)), green(clip(color.green, 0., 1.)), blue(clip(color.blue, 0., 1.)), alpha(clip(color.alpha, 0., 1.))
        {
            ;
        }
        
        ~Color()
        {
            ;
        }
    };
    
    // ================================================================================ //
    //                                      FONT                                        //
    // ================================================================================ //
    
    class Font
    {
    public:
        enum Justification
        {
            Left                    = 1,
            Right                   = 2,
            HorizontallyCentered    = 4,
            Top                     = 8,
            TopLeft                 = 9,
            TopRight                = 10,
            CentredTop              = 12,
            Bottom                  = 16,
            BottomLeft              = 17,
            BottomRight             = 18,
            CentredBottom           = 20,
            VerticallyCentred       = 32,
            CentredLeft             = 33,
            CentredRight            = 34,
            Centred                 = 36,
            HorizontallyJustified   = 64,
        };
        
        enum Face
        {
            Normal      = 0,
            Bold        = 1,
            Italic      = 2,
            Underlined  = 4
        };
        
        string          name;
        size_t          size;
        Face            face;
        
        Font(string const& _name, size_t _size, Face _face = Normal) noexcept :
        name(_name), size(_size), face(_face)
        {
            ;
        }
        
        ~Font()
        {
            ;
        }
    };
    
    // ================================================================================ //
    //                                      RECTANGLE                                   //
    // ================================================================================ //
    
    class Rectangle
    {
    public:
        double x;
        double y;
        double width;
        double height;
        
        Rectangle(double _x, double _y, double _width, double _height) noexcept :
        x(_x), y(_y), width(_width), height(_height)
        {
            ;
        }
        
        ~Rectangle()
        {
            ;
        }
    };
    
    // ================================================================================ //
    //                                      DOODLE                                      //
    // ================================================================================ //
    
    class Doodle
    {
    public:
        class Point;
        
        Doodle()
        {
            ;
        }
        
        virtual ~Doodle()
        {
            ;
        }
        
        virtual inline double getX() = 0;
        
        virtual inline double getY() = 0;
        
        virtual inline double getWidth() = 0;
        
        virtual inline double getHeight() = 0;
        
        virtual inline void fillAll() = 0;
        
        virtual inline void setColor(Color const& color) = 0;
        
        virtual inline void setFont(const Font& font) = 0;
        
        virtual inline void drawText(string const& text, double x, double y, double w, double h, Font::Justification j, bool wrap = true) = 0;
        
        virtual inline void drawText(string const& text, Rectangle const& rect, Font::Justification j, bool wrap = true)
        {
            drawText(text, rect.x, rect.y, rect.width, rect.height, j, wrap);
        }
        
        virtual inline void drawRectangle(double x, double y, double w, double h, double thickness, double rounded = 0.) = 0;
        
        virtual inline void drawRectangle(Rectangle const& rect, double thickness, double rounded = 0.)
        {
            drawRectangle(rect.x, rect.y, rect.width, rect.height, thickness, rounded);
        }
        
        virtual inline void fillRectangle(double x, double y, double w, double h, double rounded = 0.) = 0;
        
        virtual inline void fillRectangle(Rectangle const& rect, double rounded = 0.)
        {
            drawRectangle(rect.x, rect.y, rect.width, rect.height, rounded);
        }
    };
}

#endif