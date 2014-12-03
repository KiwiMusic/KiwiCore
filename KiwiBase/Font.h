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

#ifndef __DEF_KIWI_FONT__
#define __DEF_KIWI_FONT__

#include "Defs.h"
#include "Tools.h"
#include "Point.h"

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
        
        Font() noexcept :
        name("Arial"), size(12.), face(Normal)
        {
            
        }
        
        Font(string const& _name, unsigned long _size, Face _face = Normal) noexcept :
        name(_name), size(_size), face(_face)
        {
            ;
        }
        
        ~Font()
        {
            ;
        }
        /*
        static string getStringSelection(Font const& font, string const& text, double const x1, double const x2) noexcept
        {
            string rtext, ctext;
            unsigned long pos = 0;
            while(pos < text.size())
            {
                ctext += text[pos];
                Point size = Font::getStringSize(font, ctext);
                if(size.x() >= x1 && size.x() <= x2)
                {
                    rtext += text[pos++];
                }
                else if(size.x() > x2)
                {
                    return rtext;
                }
            }
            return rtext;
        }
        
        static Point getStringPosition(Font const& font, string const& text, double const x1, double const x2) noexcept
        {
            Point rpoint(0, 0.);
            string ctext;
            unsigned long pos = 0;
            while(pos < text.size())
            {
                ctext += text[pos];
                Point size = Font::getStringSize(font, ctext);
                if(size.x() <= x1)
                {
                    rpoint.x(size.x());
                }
                else if(size.x() > x1 && size.x() <= x2)
                {
                    rpoint.y(size.x());
                }
                else if(size.x() > x2)
                {
                    return rpoint;
                }
            }
            return rpoint;
        }*/
    };
}

#endif
