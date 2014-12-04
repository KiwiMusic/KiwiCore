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
    Text::Editor::Editor()
    {
        m_marker_start  = m_text.begin();
        m_marker_end = m_marker_start;
    }
    
    Text::Editor::~Editor()
    {
        ;
    }
    
    void Text::Editor::setFont(Font const& font)
    {
        m_font = font;
    }
    
    void Text::Editor::getText(string& text) const noexcept
    {
        text = m_text;
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
            m_marker_start  = m_text.begin();
            m_marker_end    = m_text.end();
        }
        else if(event.isEscape())
        {
            ;
        }
        else if(event.isReturn())
        {
            if(m_marker_start != m_marker_end)
            {
                if(m_marker_start != m_text.begin())
                    --m_marker_start;
                m_marker_start = m_text.erase(m_marker_start, m_marker_end);
            }
            m_marker_start = m_text.insert(m_marker_start, '\n');
            if(m_marker_start != m_text.end())
                ++m_marker_start;
            m_marker_end = m_marker_start;
        }
        else if(event.isTab())
        {
            if(m_marker_start != m_marker_end)
            {
                if(m_marker_start != m_text.begin())
                    --m_marker_start;
                m_marker_start = m_text.erase(m_marker_start, m_marker_end);
            }
            m_marker_start = m_text.insert(m_marker_start, '\t');
            if(m_marker_start != m_text.end())
                ++m_marker_start;
            m_marker_end = m_marker_start;
        }
        else if(event.isBackspace())
        {
            if(m_marker_start != m_text.begin())
                --m_marker_start;
            m_marker_start = m_text.erase(m_marker_start, m_marker_end);
            if(m_marker_start != m_text.end())
                ++m_marker_start;
            m_marker_end = m_marker_start;
        }
        else if(event.isCharacter())
        {
            if(m_marker_start != m_marker_end)
            {
                m_marker_start = m_text.erase(m_marker_start, m_marker_end);
            }
            m_marker_start = m_text.insert(m_marker_start, event.getCharacter());
            if(m_marker_start != m_text.end())
                ++m_marker_start;
            m_marker_end = m_marker_start;
        }
        return true;
    }
    
    bool Text::Editor::receive(Event::Focus::Type event)
    {
        if(event == Event::Focus::In)
        {
            //m_marker_start = m_text.end();
            //m_marker_end = m_marker_start;
        }
        return true;
    }
    
    bool Text::Editor::draw(Doodle& doodle) const
    {
        return true;
    }
}


