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
    //                                      TEXT                                        //
    // ================================================================================ //
    
    Text::Text() noexcept :
    m_start_line(0),
    m_start_marker(0),
    m_end_line(0),
    m_end_marker(0)
    {
        m_lines.push_back("");
    }
    
    Text::~Text()
    {
        m_lines.clear();
    }

    bool Text::isEmpty() const noexcept
    {
        return m_lines.size() == 1 && m_lines[0].empty();
    }
    
    unsigned long Text::getNumberOfLines() const noexcept
    {
        if(isEmpty())
        {
            return 0;
        }
        return m_lines.size();
    }
    
    void Text::getLine(unsigned long index, string& text) const noexcept
    {
        text.clear();
        if(index < (unsigned long)m_lines.size() - 1)
        {
            text.assign(m_lines[index], 0, m_lines[index].size() - 1);
        }
        else if(index == (unsigned long)m_lines.size() - 1)
        {
            text = m_lines[(vector<string>::size_type)index];
        }
    }
    
    void Text::getAllLines(string& text) const noexcept
    {
        text.clear();
        for(vector<string>::size_type i = 0; i < m_lines.size(); i++)
        {
            text += m_lines[i];
        }
    }
    
    void Text::getSelectedLines(string& text) const noexcept
    {
        text.clear();
        if(!isSelectionEmpty())
        {
            int zaza;
        }
    }
    
    bool Text::isSelectionEmpty() const noexcept
    {
        return m_start_line == m_end_line && m_start_marker == m_end_marker;
    }
    
    void Text::selectAll() noexcept
    {
        m_start_line   = 0;
        m_start_marker = 0;
        if(!m_lines.empty())
        {
            m_end_line   = m_lines.size() - 1;
            m_end_marker = m_lines[m_end_line].size() - 1;
        }
        else
        {
            m_end_line   = 0;
            m_end_marker = 0;
        }
    }

    void Text::addPreviousCharacterToSelection() noexcept
    {
        if(!isEmpty())
        {
            if(m_start_line && m_start_marker == 0)
            {
                m_start_line--;
                m_start_marker = m_lines[m_start_line].size() - 1;
            }
            else if(m_start_marker)
            {
                m_start_marker--;
            }
        }
    }
    
    void Text::addNextCharacterToSelection() noexcept
    {
        if(!isEmpty())
        {
            ;
        }
    }
    
    void Text::addPreviousWordToSelection() noexcept
    {
        
    }
    
    void Text::addNextWordToSelection() noexcept
    {
        
    }
    
    void Text::eraseSelection()
    {
        if(!isEmpty() && !isSelectionEmpty())
        {
            if(m_start_line == m_end_line && m_start_marker != m_end_marker)
            {
                m_lines[m_start_line].erase(m_start_marker, m_end_marker - m_start_marker);
                m_end_marker = m_start_marker;
            }
            else
            {
                if(m_start_line + 1 != m_end_line)
                {
                    m_lines.erase(m_lines.begin() + m_start_line + 1, m_lines.begin() + m_end_line - 1);
                    m_end_line = m_start_line + 1;
                }
                m_lines[m_start_line].erase(m_start_marker);
                m_lines[m_end_line].erase(0, m_end_marker);
                m_lines[m_start_line] += m_lines[m_end_line];
                m_lines.erase(m_lines.begin() + m_end_line);
                m_end_line   = m_start_line;
                m_end_marker = m_start_marker;
            }
        }
    }
    
    void Text::insertCharacter(const char c)
    {
        eraseSelection();
        if(c == '\n')
        {
            m_lines[m_start_line].insert(m_start_marker, 1, '\n');
            m_start_line++;
            m_lines.insert(m_lines.begin() + m_start_line, string(""));
            m_end_line      = m_start_line;
            m_end_marker    = m_start_marker = 0;
        }
        else
        {
            m_lines[m_start_line].insert(m_start_marker, 1, c);
            m_end_marker = ++m_start_marker;
        }
    }
    
    // ================================================================================ //
    //                                  TEXT EDITOR                                     //
    // ================================================================================ //
    
    Text::Editor::Editor() :
    m_justification(Font::Justification::VerticallyCentred),
    m_color(0., 0., 0, 1.),
    m_margin_top(0.),
    m_margin_left(0.),
    m_margin_bottom(0.),
    m_margin_right(0.),
    m_line_spacing(0.),
    m_behavior(Wrapped)
    {
        ;
    }
    
    Text::Editor::~Editor()
    {
        ;
    }
    
    void Text::Editor::setFont(Font const& font) noexcept
    {
        m_font = font;
        updateBoundaries();
    }
    
    void Text::Editor::setJustification(Font::Justification const& j) noexcept
    {
        m_justification = j;
    }
    
    void Text::Editor::setColor(Color const& color) noexcept
    {
        m_color = color;
    }
    
    void Text::Editor::setSize(Point const& size) noexcept
    {
        m_size = size;
        m_displayed_width   = m_size.x() - m_margin_left - m_margin_right;
        m_displayed_height  = m_size.y() - m_margin_top  - m_margin_bottom;
        updateBoundaries();
    }
    
    void Text::Editor::setMargins(double const top, double const left, double const bottom, double const right) noexcept
    {
        m_margin_top    = top;
        m_margin_left   = left;
        m_margin_bottom = bottom;
        m_margin_right  = right;
        m_displayed_width   = m_size.x() - m_margin_left - m_margin_right;
        m_displayed_height  = m_size.y() - m_margin_top  - m_margin_bottom;
        updateBoundaries();
    }
    
    void Text::Editor::setMarginTop(double const top) noexcept
    {
        m_margin_top    = top;
        m_displayed_height  = m_size.y() - m_margin_top  - m_margin_bottom;
        updateBoundaries();
    }
    
    void Text::Editor::setMarginLeft(double const left) noexcept
    {
        m_margin_left   = left;
        m_displayed_width   = m_size.x() - m_margin_left - m_margin_right;
        updateBoundaries();
    }
    
    void Text::Editor::setMarginBottom(double const bottom) noexcept
    {
        m_margin_bottom = bottom;
        m_displayed_height  = m_size.y() - m_margin_top  - m_margin_bottom;
        updateBoundaries();
    }
    
    void Text::Editor::setMarginRight(double const right) noexcept
    {
        m_margin_right  = right;
        m_displayed_width   = m_size.x() - m_margin_left - m_margin_right;
        updateBoundaries();
    }
    
    void Text::Editor::setLineSpacing(double const linespacing) noexcept
    {
        m_line_spacing = linespacing;
        updateBoundaries();
    }
    
    void Text::Editor::setBehavior(Behavior behavior) noexcept
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
    
    void Text::Editor::setText(string const& text)
    {
        for(string::size_type i = 0; i < text.size(); i++)
        {
            insertCharacter(text[i]);
        }
        updateBoundaries();
    }
    
    void Text::Editor::truncate()
    {
        for(vector<string>::size_type i = 0; i < getNumberOfLines(); i++)
        {
            string line;
            getLine(i, line);
            Point size = getStringSize(m_font, line);
            if(size.x() > m_displayed_width)
            {
                line.replace(line.size() - 3, 3, "...");
                size = getStringSize(m_font, line);
                while(size.x() > m_displayed_width)
                {
                    line.pop_back();
                    line.replace(line.size() - 3, 3, "...");
                }
            }
            m_displayed_text.push_back(line);
        }
    }
    
    void Text::Editor::wrap()
    {
        for(vector<string>::size_type i = 0; i < getNumberOfLines(); i++)
        {
            string line;
            getLine(i, line);
            Point size = getStringSize(m_font, line);
            if(size.x() > m_displayed_width)
            {
                string nline, templine, word;
                string::size_type pos = 0, pos_next;
                pos_next = line.find(' ', pos);
                while(pos_next != string::npos)
                {
                    word.assign(line, pos, pos_next - pos);
                    size = getStringSize(m_font, word);
                    if(size.x() > m_displayed_width)
                    {
                        for(string::size_type i = 0; i < word.size(); i++)
                        {
                            nline += word[i];
                            size = getStringSize(m_font, nline);
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
                        size = getStringSize(m_font, templine);
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
    
    void Text::Editor::updateBoundaries()
    {
        m_text_size = Point(0., 0.);
        for(vector<string>::size_type i = 0; i < getNumberOfLines(); i++)
        {
            string line;
            getLine(i, line);
            const Point line_size = getStringSize(m_font, line);
            m_text_size.x(max(line_size.x(), m_text_size.x()));
            m_text_size.y(m_text_size.y() + line_size.y() + m_line_spacing);
        }
        m_text_size.x(m_text_size.x() + m_margin_right + m_margin_left);
        m_text_size.y(m_text_size.y() + m_margin_top + m_margin_bottom);
        
        if(m_behavior)
        {
            wrap();
        }
        else
        {
            truncate();
        }
    }
    
    bool Text::Editor::receive(Event::Mouse const& event)
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
    
    bool Text::Editor::receive(Event::Keyboard const& event)
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
            insertCharacter('\n');
            updateBoundaries();
            return true;
        }
        else if(event.isTab())
        {
            insertCharacter('\t');
            updateBoundaries();
            return true;
        }
        else if(event.isCharacter())
        {
            insertCharacter(event.getCharacter());
            updateBoundaries();
            return true;
        }
        return false;
    }
    
    bool Text::Editor::receive(Event::Focus::Type event)
    {
        return true;
    }
    
    bool Text::Editor::draw(Doodle& doodle) const
    {
        if(!isEmpty())
        {
            doodle.setFont(m_font);
            doodle.setColor(m_color);
            const double width  = doodle.getWidth() - m_margin_left - m_margin_right;
            const double height = getStringSize(m_font, "/").y();
            for(vector<string>::size_type i = 0; i < m_displayed_text.size(); i++)
            {
                doodle.drawText(m_displayed_text[i], m_margin_left, height * i + m_margin_top, width, height, m_justification);
                cout << "line " << i << " " << m_displayed_text[i] << endl;
            }
            
            cout << endl;
        }
        return true;
    }
}

