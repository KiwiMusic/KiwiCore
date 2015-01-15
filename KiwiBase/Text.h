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
        class Editor;
    private:
        wstring             m_text;
        wstring::size_type  m_marker_start;
        wstring::size_type  m_marker_end;
    public:
        
        //! Constructor.
        /** The function initialize a text with an empty text.
         */
        Text() noexcept;
        
        //! Destrcutor.
        /** The function does nothing.
         */
        ~Text();
        
        //! Get is the text is empty.
        /** The function retrieves is the text is empty.
         @return true if the text is empty, otherwise false.
         */
        bool isEmpty() const noexcept;
        
        //! Get the number of lines.
        /** The function retrieves the number of lines.
         @return The number of lines in the text.
         */
        ulong getNumberOfLines() const noexcept;
        
        bool isSelectionEmpty() const noexcept;
        
        void selectAll() noexcept;
        void addPreviousCharacterToSelection() noexcept;
        void addNextCharacterToSelection() noexcept;
        void addPreviousWordToSelection() noexcept;
        void addNextWordToSelection() noexcept;
        
        void eraseSelection();
        void insertCharacter(const wchar_t c);
        
        static Point getStringSize(Font const& font, string const& text) noexcept;
        
        static Point getStringSize(Font const& font, wstring const& text) noexcept;
    };
    
    class Text::Editor : public Text
    {
    public:
        enum Behavior
        {
            Truncated   = 0,
            Wrapped     = 1
        };
        
    private:
        Font                m_font;
        Font::Justification m_justification;
        Color               m_color;
        vector<string>      m_displayed_text;
        
        Point               m_size;
        double              m_margin_top;
        double              m_margin_left;
        double              m_margin_bottom;
        double              m_margin_right;
        double              m_displayed_width;
        double              m_displayed_height;
        double              m_line_spacing;
        
        long                m_behavior;
        
        Point               m_text_size;
        Point               m_text_displayed_size;
        
        void updateBoundaries();        
        void truncate();
        void wrap();
    public:
        
        Editor();
        
        virtual ~Editor();
        
        void setFont(Font const& font) noexcept;
        void setJustification(Font::Justification const& j) noexcept;
        void setColor(Color const& color) noexcept;
        
        void setBehavior(Behavior behavior) noexcept;
        void setSize(Point const& size) noexcept;
        void setMargins(double const top, double const left, double const bottom, double const right) noexcept;
        void setMarginTop(double const top) noexcept;
        void setMarginLeft(double const left) noexcept;
        void setMarginBottom(double const bottom) noexcept;
        void setMarginRight(double const right) noexcept;
        void setLineSpacing(double const linespacing) noexcept;
        void setText(string const& text);
        
        bool receive(Event::Mouse const& event);
        bool receive(Event::Keyboard const& event);
        bool receive(Event::Focus::Type event);
        bool draw(Doodle& doodle) const;
        
        static string getStringSelection(Font const& font, string const& text, double const x1, double const x2) noexcept
        {
            string rtext, ctext;
            ulong pos = 0;
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
            ulong pos = 0;
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
}

#endif