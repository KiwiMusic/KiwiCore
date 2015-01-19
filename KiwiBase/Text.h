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
    //									  TEXTEDITOR                                    //
    // ================================================================================ //
    
    class TextEditor
    {
    public:
        enum Behavior
        {
            Truncated   = 0,
            Wrapped     = 1
        };
        
    private:
		wstring             m_text;
		wstring::size_type  m_marker_start;
		wstring::size_type  m_marker_end;
		
        Font                m_font;
        Font::Justification m_justification;
        Color               m_color;
        vector<string>      m_displayed_text;
        
        Point               m_size;
        double              m_padding_top;
        double              m_padding_left;
        double              m_padding_bottom;
        double              m_padding_right;
		
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
		
		//! Constructor.
		/** The function initialize a texteditor with an empty text.
		 */
        TextEditor();
		
		//! Constructor.
		/** The function initialize a texteditor with a string.
		 */
		TextEditor(string const& text) noexcept;
		
		//! Destructor.
		/** The function initialize a text with a string.
		 */
		virtual ~TextEditor();
		
		//std::wstring str = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes("some string");
		
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
		
		//! Retrieves the text size.
		/** The function retrieves the text size.
		 @return The text size.
		 */
		inline wstring::size_type size() const noexcept
		{
			return m_text.size();
		}
		
		inline wstring const& to_wstring() noexcept
		{
			return m_text;
		}
		
		/*
		 wstring s2ws(const std::string& str)
		 {
			typedef codecvt_utf8<wchar_t> convert_typeX;
			std::wstring_convert<convert_typeX, wchar_t> converterX;
			
			return converterX.from_bytes(str);
		 }
		 
		 string ws2s(const std::wstring& wstr)
		 {
			typedef std::codecvt_utf8<wchar_t> convert_typeX;
			std::wstring_convert<convert_typeX, wchar_t> converterX;
			
			return converterX.to_bytes(wstr);
		 }
		 */
		
        void setFont(Font const& font) noexcept;
        void setJustification(Font::Justification const& j) noexcept;
        void setColor(Color const& color) noexcept;
        
        void setBehavior(Behavior behavior) noexcept;
        void setSize(Point const& size) noexcept;
		
        void setPadding(double const top, double const left, double const bottom, double const right) noexcept;
        void setPaddingTop(double const top) noexcept;
        void setPaddingLeft(double const left) noexcept;
        void setPaddingBottom(double const bottom) noexcept;
        void setPaddingRight(double const right) noexcept;
		
        void setLineSpacing(double const linespacing) noexcept;
        void setText(string const& text);
        
        bool receive(Event::Mouse const& event);
        bool receive(Event::Keyboard const& event);
        bool receive(Event::Focus::Type event);
        bool draw(Doodle& doodle) const;
        
		static string getStringSelection(Font const& font, string const& text, double const x1, double const x2) noexcept;
		
		static Point getStringPosition(Font const& font, string const& text, double const x1, double const x2) noexcept;
		
		bool isSelectionEmpty() const noexcept;
		
		void selectAll() noexcept;
		void addPreviousCharacterToSelection() noexcept;
		void addNextCharacterToSelection() noexcept;
		void addPreviousWordToSelection() noexcept;
		void addNextWordToSelection() noexcept;
		
		void eraseSelection();
		void insertCharacter(const wchar_t c);
    };
	
	typedef shared_ptr<Link>        sTextEditor;
}

#endif