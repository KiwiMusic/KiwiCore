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

#ifndef __DEF_KIWI_JSON__
#define __DEF_KIWI_JSON__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                          JSON                                    //
    // ================================================================================ //
    
    class Json
    {
    private:
        string m_text;
        size_t m_increment;
        
        inline void applyTabulation()
        {
            for(size_t i = 0; i < m_increment; i++)
                m_text.append("    ");
        }
        
    public:
        Json() : m_increment(1)
        {
            m_text.append("{\n");
        };
        
        ~Json()
        {
        };
        
        inline operator string() const noexcept
        {
            return m_text;
        }
        
        inline void close()
        {
            while(--m_increment)
            {
                m_text.append("},\n");
            }
            m_text.append("}");
        }
        
        inline void newKey(shared_ptr<Tag> tag)
        {
            applyTabulation();
            m_text.append("\"" + (string)*tag + "\" : ");
        }
        
        inline void add(long value)
        {
            m_text.append(to_string(value) + ",\n");
        }
        
        inline void add(double value)
        {
            m_text.append(to_string(value) + ",\n");
        }
        
        inline void add(shared_ptr<Tag> tag)
        {
            m_text.append("\"" + (string)*tag + "\",\n");
        }
        
        inline void add(shared_ptr<Object> object)
        {
            m_text.append("{\n");
            m_increment++;
            object->write(*this);
            m_increment--;
            applyTabulation();
            m_text.append("},\n");
        }
        
        
        inline void add(Element element) noexcept
        {
            if(element.isLong())
                add((long)element);
            else if(element.isDouble())
                add((double)element);
            else if(element.isTag())
                add((shared_ptr<Tag>)element);
            else
                add((shared_ptr<Object>)element);
        }
        
        inline void add(vector<Element> elements)
        {
            if(elements.size())
            {
                if(elements.size() == 1)
                {
                    add(elements[0]);
                }
                else
                {
                    m_text.append("[ ");
                    for(int i = 0; i < elements.size(); i++)
                    {
                        if(elements[i].isLong())
                            m_text.append(to_string((long)elements[i]));
                        else if(elements[i].isDouble())
                            m_text.append(to_string((double)elements[i]));
                        else if(elements[i].isTag())
                             m_text.append("\"" + (string)*((shared_ptr<Tag>)elements[i]) + "\"");
                        else
                        {
                            m_text.append("{\n");
                            m_increment++;
                            ((shared_ptr<Object>)elements[i])->write(*this);
                            applyTabulation();
                            m_text.append("}");
                            m_increment--;
                        }
                        
                        if(i < elements.size() - 1)
                            m_text.append(", ");
                        else
                            m_text.append(" ]");
                    }
                    m_text.append(",\n");
                }
            }
        }
    };
}
#endif
