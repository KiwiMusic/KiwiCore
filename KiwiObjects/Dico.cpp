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

#include "Dico.h"
#include "Instance.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    Dico::Dico(shared_ptr<Instance> kiwi) : Object(kiwi, "dico")
    {
        ;
    }
    
    Dico::~Dico()
    {
        clear();
    }
    
    void Dico::clear() noexcept
    {
        m_entries.clear();
    }
    
    void Dico::clear(shared_ptr<Tag> key) noexcept
    {
        m_entries.erase(key);
    }
    
    void Dico::keys(vector<Element>& elements) const noexcept
    {
        elements.clear();
        for(map<const shared_ptr<Tag>, vector<Element>>::const_iterator it = m_entries.begin(); it != m_entries.end(); ++it)
            elements.push_back(it->first);
    }
    
    bool Dico::has(shared_ptr<Tag> key) const noexcept
    {
        return m_entries.find(key) != m_entries.end();
    }
    
    Type Dico::type(shared_ptr<Tag> key) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size() == 1)
            {
                return elements[0].type();
            }
            else
            {
                return T_ELEMENTS;
            }
        }
        else
            return T_NOTHING;
    }
    
    Element Dico::get(shared_ptr<Tag> key) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            vector<Element> elements = it->second;
            if(elements.size())
            {
                return elements[0];
            }
        }
        return 0;
    }
    
    void Dico::get(shared_ptr<Tag> key, vector<Element>& elements) const noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            elements = it->second;
        }
    }
    
    void Dico::set(shared_ptr<Tag> key, Element const& element) noexcept
    {
        vector<Element> elements;
        elements.push_back(element);
        m_entries[key] = elements;
    }
    
    void Dico::set(shared_ptr<Tag> key, vector<Element> const& elements) noexcept
    {
        if(elements.size() == 0)
            return;
        m_entries[key] = elements;
    }

    void Dico::append(shared_ptr<Tag> key, Element const& element) noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(element);
        else
            set(key, element);
    }
    
    void Dico::append(shared_ptr<Tag> key, vector<Element> const& elements) noexcept
    {
        map<const shared_ptr<Tag>, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            for(int i = 0; i < elements.size(); i++)
            {
                it->second.push_back(elements[i]);
            }
        }
        else
            set(key, elements);
    }
    
    void Dico::write(string const& filename, string const& directoryname)
    {
        ofstream file;
        if(directoryname.size() && filename.size())
        {
#ifdef _WINDOWS
            file.open(directoryname + "\\" + filename);
#else
            file.open(directoryname + "/" + filename, ios_base::out);
#endif
        }
        else if(filename.size())
        {
#ifdef _WINDOWS
            file.open(filename);
#else
            file.open(filename, ios_base::out);
#endif
        }
        
        if(file.is_open())
        {
            file.clear();
            file << "{\n";
            string line;
            dowrite(static_pointer_cast<Dico>(shared_from_this()), file, line);
            file << "}";
        }
        
        file.close();
    }
    
    void Dico::read(string const& filename, string const& directoryname)
    {
        ifstream file;
        if(directoryname.size() && filename.size())
        {
#ifdef _WINDOWS
            file.open(directoryname + "\\" + filename);
#else
            file.open(directoryname + "/" + filename, ios_base::in);
#endif
        }
        else if(filename.size())
        {
#ifdef _WINDOWS
            file.open(filename);
#else
            file.open(filename, ios_base::in);
#endif
        }
        
        if(file.is_open())
        {
            clear();
            string line;
            doread(static_pointer_cast<Dico>(shared_from_this()), file, line);
        }
        
        file.close();
    }
    
    void Dico::readFormatted(string const& text)
    {
        if(text.size())
        {
            clear();
            bool mode = false;
            string word;
            string key = "name";
            vector<Element> elements;
            istringstream iss(text);
            while(iss >> word)
            {
                if(mode)
                {
                    if(word[0] == '@')
                    {
                        set(createTag(key), elements);
                        elements.clear();
                        key = word.c_str()+1;
                    }
                    else
                    {
                        if(isdigit(word[0]))
                        {
                            if(word.find('.'))
                            {
                                elements.push_back(atof(word.c_str()));
                            }
                            else
                            {
                                elements.push_back(atol(word.c_str()));
                            }
                        }
                        else
                        {
                            elements.push_back(createTag(word));
                        }
                    }
                }
                else
                {
                    set(createTag(key), createTag(word));
                    key = "arguments";
                    mode = true;
                }
            }
            set(createTag(key), elements);
        }
    }
    
    void Dico::post()
    {
        string text;
        dopost(static_pointer_cast<Dico>(shared_from_this()), text);
        Object::post(text);
    }
    
    void Dico::dowrite(shared_ptr<Dico> dico, ofstream& file, string& line)
    {
        vector<Element> elements;
        dico->keys(elements);
        if(elements.size())
        {
            for(int i = 0; i < elements.size(); i++)
            {
                shared_ptr<Tag> key = (shared_ptr<Tag>)elements[i];
                Type type = dico->type(key);
                
                file << line << "    \"" << (string)*key << "\" : ";
                
                if(type == T_LONG)
                {
                    file << to_string((long)dico->get(key)) << ",\n";
                }
                else if(type == T_DOUBLE)
                {
                    file << to_string((double)dico->get(key)) << ",\n";
                }
                else if(type == T_TAG)
                {
                    shared_ptr<Tag> tag = (shared_ptr<Tag>)dico->get(key);
                    if(tag)
                        file << "\"" << (string)*tag << "\",\n";
                    else
                        file << "\"\",\n";
                }
                else if(type == T_OBJECT)
                {
                    file << "{\n";
                    string nline = line + "    ";
                    dowrite((shared_ptr<Dico>)dico->get(key), file, nline);
                    file << line << "    }\n";
                }
                else
                {
                    vector<Element> elements2;
                    dico->get(key, elements2);
                    file << "[ ";
                    for(int i = 0; i < elements2.size(); i++)
                    {
                        if(elements2[i].isLong())
                        {
                            file << to_string((long)elements2[i]);
                        }
                        else if(elements2[i].isDouble())
                        {
                            file << to_string((double)elements2[i]);
                        }
                        else if(elements2[i].isTag())
                        {
                            file << "\"" << (string)*((shared_ptr<Tag>)elements2[i]) << "\"";
                        }
                        else
                        {
                            file << "{\n";
                            string nline = line + "    ";
                            dowrite((shared_ptr<Dico>)elements2[i], file, nline);
                            file << line  << "    }";
                        }
                        
                        if(i < elements2.size() - 1)
                            file << ", ";
                        else
                            file << " ]";
                    }
                    file << ",\n";
                }
            }
        }
    }
    
    void Dico::doread(shared_ptr<Dico> dico, ifstream& file, string& line)
    {
        while(getline(file, line))
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
                            doread(subdico, file, line);
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
                                    doread(subdico, file, line);
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
    
    void Dico::dopost(shared_ptr<Dico> dico, string& text, string line)
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
}
















