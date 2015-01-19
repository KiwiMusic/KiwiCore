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

#include "Text.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  TEXT EDITOR                                     //
    // ================================================================================ //
    
    TextEditor::TextEditor() :
	m_marker_start(0),
	m_marker_end(0),
    m_justification(Font::Justification::VerticallyCentred),
    m_color(0., 0., 0, 1.),
    m_padding_top(0.),
    m_padding_left(0.),
    m_padding_bottom(0.),
    m_padding_right(0.),
    m_line_spacing(0.),
    m_behavior(Wrapped)
    {
        ;
    }
    
    TextEditor::~TextEditor()
    {
        m_text.clear();
    }
	
	bool TextEditor::isEmpty() const noexcept
	{
		return m_text.empty();
	}
	
	ulong TextEditor::getNumberOfLines() const noexcept
	{
		if(isEmpty())
		{
			return 0;
		}
		else
		{
			ulong nlines = 1;
			string::size_type pos = string::npos;
			while((pos = m_text.find('\n', pos)) != string::npos)
			{
				nlines++;
			}
			return nlines;
		}
	}
	
	bool TextEditor::isSelectionEmpty() const noexcept
	{
		return m_marker_end == m_marker_start;
	}
	
	void TextEditor::selectAll() noexcept
	{
		m_marker_start  = 0;
		m_marker_end    = m_text.size() - 1;
	}
	
	void TextEditor::addPreviousCharacterToSelection() noexcept
	{
		if(!isEmpty() && m_marker_start)
		{
			m_marker_start--;
		}
	}
	
	void TextEditor::addNextCharacterToSelection() noexcept
	{
		if(!isEmpty() && m_marker_end != m_text.size() - 1)
		{
			m_marker_end++;
		}
	}
	
	void TextEditor::addPreviousWordToSelection() noexcept
	{
		if(!isEmpty() && m_marker_start)
		{
			while(m_text[m_marker_start] == ' ' && m_marker_start)
			{
				m_marker_start--;
			}
			if(m_marker_start)
			{
				m_marker_start = m_text.rfind(' ', m_marker_start);
				if(m_marker_start == string::npos)
				{
					m_marker_start = 0;
				}
				else
				{
					m_marker_start++;
				}
			}
		}
	}
	
	void TextEditor::addNextWordToSelection() noexcept
	{
		if(!isEmpty() && m_marker_end != m_text.size() - 1)
		{
			while(m_text[m_marker_end] == ' ' && m_marker_end != m_text.size() - 1)
			{
				m_marker_end++;
			}
			if(m_marker_end != m_text.size() - 1)
			{
				m_marker_end = m_text.find(' ', m_marker_end);
				if(m_marker_end == string::npos)
				{
					m_marker_end = m_text.size() - 1;
				}
				else
				{
					m_marker_end--;
				}
			}
		}
	}
	
	void TextEditor::eraseSelection()
	{
		if(!isEmpty() && !isSelectionEmpty())
		{
			auto it = m_text.erase(m_text.begin() + m_marker_start, m_text.begin() + m_marker_end);
			m_marker_end = m_marker_start = it - m_text.begin();
		}
	}
	
	void TextEditor::insertCharacter(const wchar_t c)
	{
		m_text.replace(m_marker_start, m_marker_end - m_marker_start, &c);
	}
	
    void TextEditor::setFont(Font const& font) noexcept
    {
        m_font = font;
        updateBoundaries();
    }
	
    void TextEditor::setJustification(Font::Justification const& j) noexcept
    {
        m_justification = j;
    }
	
    void TextEditor::setColor(Color const& color) noexcept
    {
        m_color = color;
    }
	
    void TextEditor::setSize(Point const& size) noexcept
    {
        m_size = size;
        m_displayed_width   = m_size.x() - m_padding_left - m_padding_right;
        m_displayed_height  = m_size.y() - m_padding_top  - m_padding_bottom;
        updateBoundaries();
    }
	
    void TextEditor::setPadding(double const top, double const left, double const bottom, double const right) noexcept
    {
        m_padding_top    = top;
        m_padding_left   = left;
        m_padding_bottom = bottom;
        m_padding_right  = right;
        m_displayed_width   = m_size.x() - m_padding_left - m_padding_right;
        m_displayed_height  = m_size.y() - m_padding_top  - m_padding_bottom;
        updateBoundaries();
    }
    
    void TextEditor::setPaddingTop(double const top) noexcept
    {
        m_padding_top    = top;
        m_displayed_height  = m_size.y() - m_padding_top  - m_padding_bottom;
        updateBoundaries();
    }
    
    void TextEditor::setPaddingLeft(double const left) noexcept
    {
        m_padding_left   = left;
        m_displayed_width   = m_size.x() - m_padding_left - m_padding_right;
        updateBoundaries();
    }
    
    void TextEditor::setPaddingBottom(double const bottom) noexcept
    {
        m_padding_bottom = bottom;
        m_displayed_height  = m_size.y() - m_padding_top  - m_padding_bottom;
        updateBoundaries();
    }
    
    void TextEditor::setPaddingRight(double const right) noexcept
    {
        m_padding_right  = right;
        m_displayed_width   = m_size.x() - m_padding_left - m_padding_right;
        updateBoundaries();
    }
    
    void TextEditor::setLineSpacing(double const linespacing) noexcept
    {
        m_line_spacing = linespacing;
        updateBoundaries();
    }
    
    void TextEditor::setBehavior(Behavior behavior) noexcept
    {
        if(behavior != m_behavior)
        {
            m_behavior = behavior;
            if(m_behavior)
            {
                wrap();
            }
            else
            {
                truncate();
            }
        }
    }
    
    void TextEditor::setText(string const& text)
    {
        for(string::size_type i = 0; i < text.size(); i++)
        {
            insertCharacter(text[i]);
        }
        updateBoundaries();
    }
	
	string TextEditor::getStringSelection(Font const& font, string const& text, double const x1, double const x2) noexcept
	{
		string rtext, ctext;
		ulong pos = 0;
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
	
	Point TextEditor::getStringPosition(Font const& font, string const& text, double const x1, double const x2) noexcept
	{
		Point rpoint(0, 0.);
		string ctext;
		ulong pos = 0;
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
	}
	
    void TextEditor::truncate()
    {
        for(vector<string>::size_type i = 0; i < getNumberOfLines(); i++)
        {
            string line;
            //getLine(i, line);
            Point size = Font::getStringSize(m_font, line);
            if(size.x() > m_displayed_width)
            {
                line.replace(line.size() - 3, 3, "...");
                size = Font::getStringSize(m_font, line);
                while(size.x() > m_displayed_width)
                {
                    line.pop_back();
                    line.replace(line.size() - 3, 3, "...");
                }
            }
            m_displayed_text.push_back(line);
        }
    }
    
    void TextEditor::wrap()
    {
        m_text_size = Font::getStringSize(m_font, m_text);
        for(vector<string>::size_type i = 0; i < m_text.size(); i++)
        {
            string line;
            //getLine(i, line);
            Point size = Font::getStringSize(m_font, line);
            if(size.x() > m_displayed_width)
            {
                string nline, templine, word;
                string::size_type pos = 0, pos_next;
                pos_next = line.find(' ', pos);
                while(pos_next != string::npos)
                {
                    word.assign(line, pos, pos_next - pos);
                    size = Font::getStringSize(m_font, word);
                    if(size.x() > m_displayed_width)
                    {
                        for(string::size_type i = 0; i < word.size(); i++)
                        {
                            nline += word[i];
                            size = Font::getStringSize(m_font, nline);
                            if(size.x() > m_displayed_width)
                            {
                                nline.pop_back();
                                m_displayed_text.push_back(nline);
                                pos += i - 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        templine += word;
                        size = Font::getStringSize(m_font, templine);
                        if(size.x() > m_displayed_width)
                        {
                            m_displayed_text.push_back(nline);
                            nline.clear();
                            templine.clear();
                        }
                        else
                        {
                            nline.append(line, pos, pos_next - pos);
                            pos = pos_next;
                            pos_next = line.find(' ', pos);
                        }
                    }
                }
            }
            else
            {
                m_displayed_text.push_back(line);
            }
        }
    }
    
    void TextEditor::updateBoundaries()
    {
        if(m_behavior)
        {
            wrap();
        }
        else
        {
            truncate();
        }
    }
    
    bool TextEditor::receive(Event::Mouse const& event)
    {
        if(event.isDown())
        {
            return true;
        }
        if(event.isDoubleClick())
        {
            if(event.getNumberOfClicks() == 2)
            {
                
            }
            else if(event.getNumberOfClicks() == 3)
            {
                
            }
            return true;
        }
        else if(event.isDrag())
        {
            return true;
        }
        else
        {
            return true;
        }
        return true;
    }
    
    bool TextEditor::receive(Event::Keyboard const& event)
    {
        if(event.hasCmd() && event.getCharacter() == 'a')
        {
            selectAll();
            return true;
        }
        else if(event.hasShift() & event.hasLeft())
        {
            ;
        }
        else if(event.isBackspace() && !isEmpty())
        {
            if(isSelectionEmpty())
            {
                addPreviousCharacterToSelection();
            }
            eraseSelection();
            updateBoundaries();
            return true;
        }
        else if(event.isReturn())
        {
            insertCharacter(L'\n');
            updateBoundaries();
            return true;
        }
        else if(event.isTab())
        {
            insertCharacter(L'\t');
            updateBoundaries();
            return true;
        }
        else if(event.isCharacter())
        {
            insertCharacter(event.getWideCharacter());
            updateBoundaries();
            return true;
        }
        return false;
    }
    
    bool TextEditor::receive(Event::Focus::Type event)
    {
        return true;
    }
    
    bool TextEditor::draw(Doodle& doodle) const
    {
        if(!isEmpty())
        {
            doodle.setFont(m_font);
            doodle.setColor(m_color);
            const double width  = doodle.getWidth() - m_padding_left - m_padding_right;
            const double height = Font::getStringSize(m_font, "/").y();
            for(vector<string>::size_type i = 0; i < m_displayed_text.size(); i++)
            {
                doodle.drawText(m_displayed_text[i], m_padding_left, height * i + m_padding_top, width, height, m_justification);
                cout << "line " << i << " " << m_displayed_text[i] << endl;
            }
            
            cout << endl;
        }
        return true;
    }
}


