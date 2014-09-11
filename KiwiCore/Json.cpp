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

#include "Json.h"
#include "Instance.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                          JSON                                    //
    // ================================================================================ //
    
    Json::Json(shared_ptr<Instance> kiwi) : Object(kiwi, "json")
    {
        ;
    };
    
    Json::~Json()
    {
    };
    
    void Json::post(shared_ptr<Dico> dico)
    {
        string text;
        dopost(dico, text);
        Object::post(text);
    }
    
    void Json::dopost(shared_ptr<Dico> dico, string& text, string line)
    {
        vector<Element> elements;
        dico->keys(elements);
        if(elements.size())
        {
            text.append("{\n");
            for(int i = 0; i < elements.size(); i++)
            {
                shared_ptr<Tag> key = (shared_ptr<Tag>)elements[i];
                text.append(line + "    \"" + (string)*key + "\" : ");
                
                if(dico->isLong(key))
                {
                    text.append(to_string((long)dico->get(key)) + ",\n");
                }
                else if(dico->isDouble(key))
                {
                    text.append(to_string((double)dico->get(key)) + ",\n");
                }
                else if(dico->isTag(key))
                {
                    text.append("\"" + (string)*((shared_ptr<Tag>)dico->get(key)) + "\",\n");
                }
                else if(dico->isObject(key))
                {
                    dopost((shared_ptr<Dico>)dico->get(key), text, line  + "    ");
                }
                else
                {
                    vector<Element> elements2;
                    dico->get(key, elements2);
                    text.append("[ ");
                    for(int i = 0; i < elements2.size(); i++)
                    {
                        if(elements2[i].isLong())
                            text.append(to_string((long)elements2[i]));
                        else if(elements2[i].isDouble())
                            text.append(to_string((double)elements2[i]));
                        else if(elements2[i].isTag())
                            text.append("\"" + (string)*((shared_ptr<Tag>)elements2[i]) + "\"");
                        else
                            dopost((shared_ptr<Dico>)elements2[i], text, line  + "    ");
                        
                        if(i < elements2.size() - 1)
                            text.append(", ");
                        else
                            text.append(" ]");
                    }
                    text.append(",\n");
                }
            }
            text.append(line);
            text.append("}\n");
        }
    }

    
    void Json::write(shared_ptr<Dico> dico, string filename, string directoryname)
    {
        m_file.close();
        
        if(directoryname.size() && filename.size())
        {
#ifdef _WINDOWS
            m_file.open(directoryname + "\\" + filename);
#else
            m_file.open(directoryname + "/" + filename, ios_base::out);
#endif
        }
        else if(filename.size())
        {
#ifdef _WINDOWS
            m_file.open(filename);
#else
            m_file.open(filename, ios_base::out);
#endif
        }
        
        if(m_file.is_open())
        {
            m_file.clear();
            m_file << "{\n";
            dowrite(dico);
            m_file << "}";
        }
        
        m_file.close();
    }
    
    void Json::dowrite(shared_ptr<Dico> dico, string line)
    {
        vector<Element> elements;
        dico->keys(elements);
        if(elements.size())
        {
            for(int i = 0; i < elements.size(); i++)
            {
                shared_ptr<Tag> key = (shared_ptr<Tag>)elements[i];
                Type type = dico->type(key);
                
                m_file << line << "    \"" << (string)*key << "\" : ";
                
                if(type == T_LONG)
                {
                    m_file << to_string((long)dico->get(key)) << ",\n";
                }
                else if(type == T_DOUBLE)
                {
                    m_file << to_string((double)dico->get(key)) << ",\n";
                }
                else if(type == T_TAG)
                {
                    shared_ptr<Tag> tag = (shared_ptr<Tag>)dico->get(key);
                    if(tag)
                        m_file << "\"" << (string)*tag << "\",\n";
                    else
                        m_file << "\"\",\n";
                }
                else if(type == T_OBJECT)
                {
                    m_file << "{\n";
                    dowrite((shared_ptr<Dico>)dico->get(key), line  + "    ");
                    m_file << line << "    }\n";
                }
                else
                {
                    vector<Element> elements2;
                    dico->get(key, elements2);
                    m_file << "[ ";
                    for(int i = 0; i < elements2.size(); i++)
                    {
                        if(elements2[i].isLong())
                        {
                            m_file << to_string((long)elements2[i]);
                        }
                        else if(elements2[i].isDouble())
                        {
                            m_file << to_string((double)elements2[i]);
                        }
                        else if(elements2[i].isTag())
                        {
                            m_file << "\"" << (string)*((shared_ptr<Tag>)elements2[i]) << "\"";
                        }
                        else
                        {
                            m_file << "{\n";
                            dowrite((shared_ptr<Dico>)elements2[i], line  + "    ");
                            m_file << line  << "    }";
                        }
                        
                        if(i < elements2.size() - 1)
                            m_file << ", ";
                        else
                            m_file << " ]";
                    }
                    m_file << ",\n";
                }
            }
        }
    }
    
    void Json::read(shared_ptr<Dico> dico, string filename, string directoryname)
    {
        m_file.close();
        
        if(directoryname.size() && filename.size())
        {
#ifdef _WINDOWS
            m_file.open(directoryname + "\\" + filename);
#else
            m_file.open(directoryname + "/" + filename, ios_base::in);
#endif
        }
        else if(filename.size())
        {
#ifdef _WINDOWS
            m_file.open(filename);
#else
            m_file.open(filename, ios_base::in);
#endif
        }
        
        if(m_file.is_open())
        {
            dico->clear();
            doread(dico);
        }
        
        m_file.close();
    }
    
    void Json::doread(shared_ptr<Dico> dico, string line)
    {
        while(getline(m_file, line))
        {
            if(line.find('}') != string::npos)
                return;
            
            size_t pos1;
            shared_ptr<Tag> key;
            // Find the key of the dico
            pos1 = line.find('"');
            if(pos1 != string::npos)
            {
                pos1++;
                size_t next = line.find('"', pos1);
                if(next != string::npos)
                {
                    string text;
                    text.assign(line, pos1, next-pos1);
                    key = createTag(text);
                    pos1 = next;
                }
            }
            
            if(pos1 != string::npos)
            {
                pos1 = line.find(':', pos1);
                if(pos1 != string::npos && pos1 < line.size()-1)
                {
                    pos1 = line.find_first_not_of(' ', pos1+1);
                    if(pos1 != string::npos)
                    {
                        if(line[pos1] == '{')
                        {
                            shared_ptr<Dico> subdico = createDico();
                            doread(subdico);
                            dico->set(key, subdico);
                        }
                        else if(line[pos1] == '[')
                        {
                            vector<Element> elements;
                            while(pos1 < line.size()-1)
                            {
                                pos1 = line.find_first_not_of(' ', ++pos1);
                                if(line[pos1] == '{')
                                {
                                    shared_ptr<Dico> subdico = createDico();
                                    doread(subdico);
                                    elements.push_back(subdico);
                                }
                                else if(line[pos1] == '"')
                                {
                                    size_t next = line.find('"', pos1+1);
                                    if(next != string::npos)
                                    {
                                        string text;
                                        text.assign(line, pos1, next-pos1);
                                        elements.push_back(createTag(text));
                                    }
                                    pos1 = next;
                                }
                                else if(isdigit(line[pos1]))
                                {
                                    if(line.find('.', pos1) != string::npos)
                                    {
                                        elements.push_back(atof(line.c_str()+pos1));
                                    }
                                    else
                                    {
                                        elements.push_back(atol(line.c_str()+pos1));
                                    }
                                    if(pos1 < line.size()-1)
                                        pos1 = line.find_first_not_of(' ', pos1+1);
                                    
                                }
                            }
                            if(elements.size())
                            {
                                 dico->set(key, elements);
                            }
                        }
                        else if(line[pos1] == '"')
                        {
                            size_t next = line.find('"', pos1+1);
                            if(next != string::npos)
                            {
                                string text;
                                text.assign(line, pos1+1, next-pos1-1);
                                dico->set(key, createTag(text));
                            }
                        }
                        else if(isdigit(line[pos1]))
                        {
                            if(line.find('.', pos1) != string::npos)
                            {
                                dico->set(key, atof(line.c_str()+pos1));
                            }
                            else
                            {
                                dico->set(key, atol(line.c_str()+pos1));
                            }
                        }
                    }
                }
            }
        }
    }
}
















