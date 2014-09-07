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
        string      m_text;
        size_t      m_increment;
        
        inline void applyTabulation()
        {
            for(size_t i = 0; i < m_increment; i++)
                m_text.append("    ");
        }
        
        size_t getKey(string& line, string& key)
        {
            size_t pos1, pos2;
            pos1 = line.find('"');
            if(pos1 != string::npos)
            {
                pos1++;
                pos2 = line.find('"', pos1);
                if(pos2 != string::npos)
                {
                    key.assign(line, pos1, pos2-pos1);
                    return pos2;
                }
            }
            return string::npos;
        }
        
        size_t getType(string& line, size_t pos, Type& type)
        {
            size_t pos1, pos2;
            pos1 = line.find(':', pos);
            if(pos1 != string::npos)
            {
                pos1++;
                if(pos1 < line.size())
                {
                    pos2 = line.find('{', pos1);
                    if(pos2 != string::npos)
                    {
                        type = T_OBJECT;
                        return pos2;
                    }
                    pos2 = line.find('[', pos1);
                    if(pos2 != string::npos)
                    {
                        type = T_ELEMENTS;
                        return pos2;
                    }
                    pos2 = line.find_first_not_of(' ', pos1);
                    if(pos2 != string::npos)
                    {
                        size_t pos3 = line.find('"', pos2);
                        if(pos3 != string::npos)
                        {
                            type = T_TAG;
                            return pos3+1;
                        }
                        else if(line.find('.', pos2) != string::npos)
                        {
                            type = T_DOUBLE;
                            return pos2;
                        }
                        else
                        {
                            type = T_LONG;
                            return pos2;
                        }
                    }
                }
            }
            type = T_NOTHING;
            return string::npos;
        }
        
    public:
        Json() : m_increment(1)
        {
            m_text.append("{\n");
        };
        
        ~Json()
        {
        };
        
        inline string getString()
        {
            return m_text;
        }
        
        inline void read(ifstream& kfile, string& line, size_t posisition)
        {
            /*
            string keyname;
            Type   keytype;
            size_t pos = getKey(line, keyname);
            if(pos != string::npos)
            {
                pos = getType(line, pos, keytype);
                if(pos != string::npos)
                {
                    if(keytype == T_OBJECT)
                    {
                        //cout << "Object\n";
                        //read(kfile, line);
                    }
                    else if(keytype == T_ELEMENTS)
                    {
                        pos += 2;
                        size_t pos3;
                        vector<Element> elements;
                        
                        pos3 = pos;
                        while(pos < line.size())
                        {
                            pos3 = line.find(',', pos);
                            if(pos3 == string::npos)
                                pos3 = line.size();
                            
                            size_t pos4 = line.find('"', pos);
                            if(pos4 != string::npos && pos4 < pos3)
                            {
                                size_t next = line.find('"', pos+1);
                                if(next != string::npos)
                                {
                                    string text;
                                    text.assign(line, pos, next-pos);
                                    return createTag(text);
                                }
                                else
                                {
                                    return createTag(&line[pos]);
                                }
                                
                                elements.push_back(getTag(line, pos));
                            }
                            else
                            {
                                pos4 = line.find('.', pos);
                                if(pos4 != string::npos && pos4 < pos3)
                                {
                                    elements.push_back(atof(line.c_str()+pos));
                                }
                                else
                                {
                                    elements.push_back(atol(line.c_str()+pos));
                                }
                            }
                            
                            pos = pos3+1;
                        }
                        set(createTag(strname), elements);
                        
                    }
                    else if(keytype == T_LONG)
                    {
                        set(createTag(strname), getLong(line, pos));
                    }
                    else if(keytype == T_DOUBLE)
                    {
                        set(createTag(strname), getDouble(line, pos));
                    }
                    else if(keytype == T_TAG)
                    {
                        set(createTag(strname), getTag(line, pos));
                    }
                }
            }
             */
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
