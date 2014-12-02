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

#include "Point.h"
#include "Rectangle.h"
#include "Path.h"
#include "Color.h"

namespace Kiwi
{
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
        unsigned long   size;
        Face            face;
        
        Font(string const& _name, unsigned long _size, Face _face = Normal) noexcept :
        name(_name), size(_size), face(_face)
        {
            ;
        }
        
        ~Font()
        {
            ;
        }
        
        static Point getStringSize(Font const& font, string const& text) noexcept;
    };
    
    // ================================================================================ //
    //                                      DOODLE                                      //
    // ================================================================================ //
    
    //! The doodle is almost a full pure virtual class that should be override depending on the graphical implementation.
    /**
     The doodle...
     */
    class Doodle
    {
    public:
        
        //! Constructor.
        /** The function does nothing.
         */
        Doodle()
        {
            ;
        }
        
        //! Destrcutor.
        /** The function does nothing.
         */
        virtual ~Doodle()
        {
            ;
        }
        
        //! Retrieve the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        virtual inline double getX() = 0;
        
        //! Retrieve the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        virtual inline double getY() = 0;
        
        //! Retrieve the width.
        /** The function retrieves the width.
         @return The width.
         */
        virtual inline double getWidth() = 0;
        
        //! Retrieve the height.
        /** The function retrieves the height.
         @return The height.
         */
        virtual inline double getHeight() = 0;
        
        //! Retrieve the position.
        /** The function retrieves the position.
         @return The position.
         */
        virtual inline Point getPosition() = 0;
        
        //! Retrieve the size.
        /** The function retrieves the size.
         @return The size.
         */
        virtual inline Point getSize() = 0;
        
        //! Retrieve the bounds.
        /** The function retrieves the bounds.
         @return The bounds.
         */
        virtual inline Rectangle getBounds() = 0;
        
        //! Set the color.
        /** The sets the color that now will be used by the doodle.
         @param colot The color.
         */
        virtual void setColor(Color const& color) = 0;
        
        //! Set the font.
        /** The sets the font that now will be used by the doodle.
         @param font The font.
         */
        virtual void setFont(Font const& font) = 0;
        
        //! Fills the doodle with a color.
        /** The function fill the entire doodle with a color.
         */
        virtual void fillAll() = 0;
        
        virtual void drawText(string const& text, double x, double y, double w, double h, Font::Justification j, bool wrap = true) = 0;
        
        virtual void drawText(string const& text, Rectangle const& rect, Font::Justification j, bool wrap = true)
        {
            drawText(text, rect.x(), rect.y(), rect.width(), rect.height(), j, wrap);
        }
        
        virtual inline void drawLine(double x1, double y1, double x2, double y2, double thickness) = 0;
        
        virtual inline void drawRectangle(double x, double y, double w, double h, double thickness, double rounded = 0.) = 0;
        
        virtual inline void drawRectangle(Rectangle const& rect, double thickness, double rounded = 0.)
        {
            drawRectangle(rect.x(), rect.y(), rect.width(), rect.height(), thickness, rounded);
        }
        
        virtual inline void fillRectangle(double x, double y, double w, double h, double rounded = 0.) = 0;
        
        virtual inline void fillRectangle(Rectangle const& rect, double rounded = 0.)
        {
            drawRectangle(rect.x(), rect.y(), rect.width(), rect.height(), rounded);
        }
        
        virtual inline void drawEllipse(double x, double y, double width, double height, double thickness = 0.) = 0;
        
        virtual inline void fillEllipse(double x, double y, double width, double height) = 0;
    };
}

#endif