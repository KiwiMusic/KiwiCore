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
        vector<string>              m_lines;
        string::size_type           m_start_marker;
        vector<string>::size_type   m_start_line;
        string::size_type           m_end_marker;
        vector<string>::size_type   m_end_line;
        
    public:
        
        //! Constructor.
        /** The function initialize a text with an empty text.
         */
        Text() noexcept;
        
        //! Constructor.
        /** The function initialize a text with a defined text.
         @param text The text.
         */
        Text(string const& text) noexcept;
        
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
        unsigned long getNumberOfLines() const noexcept;
        
        //! Get the text of a line.
        /** The function retrieves the text of a line.
         @param index   The line index.
         @param text    The string to fill.
         */
        void getLine(unsigned long index, string& text) const noexcept;
        
        //! Get the text of all the lines.
        /** The function retrieves the text of all the lines.
         @param text    The string to fill.
         */
        void getAllLines(string& text) const noexcept;

        //! Get the text of the selected lines.
        /** The function retrieves the text of the selected lines.
         @param text    The string to fill.
         */
        void getSelectedLines(string& text) const noexcept;

        
        bool isSelectionEmpty() const noexcept;
        
        void selectAll() noexcept;
        void addPreviousCharacterToSelection() noexcept;
        void addNextCharacterToSelection() noexcept;
        void addPreviousWordToSelection() noexcept;
        void addNextWordToSelection() noexcept;
        
        void eraseSelection();
        void insertCharacter(const char c);
        
        static Point getStringSize(Font const& font, string const& text) noexcept;
    };
    
    class Text::Editor : public Text
    {
    public:
        enum Behavior
        {
            WidthFixe       = 1<<0,
            HeightFixe      = 1<<0,
            SizeFixe        = 1<<0,
            WidthExpandable = 1<<1,
            WidthReducible  = 1<<2,
            WidthResizable  = 1<<1 | 1<<2,
            HeightExpandable= 1<<3,
            HeightReducible = 1<<4,
            HeightResizable = 1<<3 | 1<<4
        };
    private:
        Font                m_font;
        Font::Justification m_justification;
        Color               m_color;
        
        Point               m_size;
        double              m_margin_top;
        double              m_margin_left;
        double              m_margin_bottom;
        double              m_margin_right;
        double              m_displayed_width;
        double              m_displayed_height;
        
        long                m_behavior;
        vector<string> m_displayed_text;
        
        void updateBoundaries();
        void breakLine(string const& text);
        virtual bool shouldResize(double const width, double const height);
    public:
        
        Editor();
        
        virtual ~Editor();
        
        void setFont(Font const& font) noexcept;
        void setJustification(Font::Justification const& j) noexcept;
        void setColor(Color const& color) noexcept;
        
        void setBehavior(long flags) noexcept;
        void setSize(Point const& size) noexcept;
        void setMargins(double const top, double const left, double const bottom, double const right) noexcept;
        void setMarginTop(double const top) noexcept;
        void setMarginLeft(double const left) noexcept;
        void setMarginBottom(double const bottom) noexcept;
        void setMarginRight(double const right) noexcept;
        
        void setText(string const& text);
        
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
}

#endif