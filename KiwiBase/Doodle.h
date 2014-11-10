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

#include "Tools.h"
#include "Console.h"

namespace Kiwi
{
    class Doodle
    {
    public:
        class Color
        {
        public:
            double red;
            double green;
            double blue;
            double alpha;
        };
        
        class Font
        {
            ;
        };
        
        class Rectangle
        {
        public:
            double x;
            double y;
            double width;
            double height;
        };

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
        
        virtual inline void setColour(Color newColour) = 0;
        
        virtual inline void setFont(const Font& newFont) = 0;
        
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