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
#include "Font.h"

namespace Kiwi
{
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
        
        //! Fill the doodle with a color.
        /** The function fills the entire doodle with a color.
         */
        virtual void fillAll() = 0;
        
        //! Draw a line of text within a rectangle.
        /** The function draws a line of text within a rectangle.
         @param text The text.
         @param x The abscissa of the rectangle.
         @param y The ordinate of the rectangle.
         @param w The width of the rectangle.
         @param h The height of the rectangle.
         @param j The justification.
         @param truncated If the text should be truncated if it goes out the boundaries.
         */
        virtual void drawText(string const& text, double x, double y, double w, double h, Kiwi::Font::Justification j, bool truncated = false) = 0;
        
        //! Draw a line of text within a rectangle.
        /** The function draws a line of text within a rectangle.
         @param text The text.
         @param rect The rectangle.
         @param j The justification.
         @param truncated If the text should be truncated if it goes out the boundaries.
         */
        virtual void drawText(string const& text, Rectangle const& rect, Font::Justification j, bool wrap = false);
        
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