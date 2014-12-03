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

#ifndef __DEF_KIWI_TEXT__
#define __DEF_KIWI_TEXT__

#include "Doodle.h"
#include "Event.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      TEXT                                        //
    // ================================================================================ //
    
    //!
    /**
     */
    class Text
    {
    public:
        
        //! Constructor.
        /** The function does nothing.
         */
        Text()
        {
            ;
        }
        
        //! Destrcutor.
        /** The function does nothing.
         */
        ~Text()
        {
            ;
        }
        
        static Point getStringSize(Font const& font, string const& text) noexcept;
        
        class Editor
        {
        private:
            Font                m_font;
            string              m_text;
            string::iterator    m_marker_start;
            string::iterator    m_marker_end;
            double  m_maker_position;
            Point   m_selection;
            
        public:
            
            Editor();
            
            ~Editor();
            
            void setFont(Font const& font);
            void getText(string& text) const noexcept;
            
            bool receive(Event::Mouse const& event);
            bool receive(Event::Keyboard const& event);
            bool receive(Event::Focus::Type event);
            
            
            
            bool draw(Doodle& doodle) const;
            
            static string getStringSelection(Font const& font, string const& text, double const x1, double const x2) noexcept
            {
                string rtext, ctext;
                unsigned long pos = 0;
                while(pos < text.size())
                {
                    ctext += text[pos];
                    Point size = getStringSize(font, ctext);
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
                    Point size = getStringSize(font, ctext);
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
            }
        };
    };
}

#endif