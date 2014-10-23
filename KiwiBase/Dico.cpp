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
#include "Object.h"
#include "Instance.h"

namespace Kiwi
{
    sTag tagBox = make_shared<Tag>("box");
    
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    Dico::Dico(weak_ptr<Instance> kiwi) noexcept :
    m_kiwi(kiwi)
    {
        ;
    }
    
    Dico::~Dico()
    {
        clear();
    }
    
    sTag Dico::createTag(string const& name) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createTag(name);
        else
            return nullptr;
    }
    
    sDico Dico::createDico() const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createDico();
        else
            return nullptr;
    }
    
    void Dico::clear() noexcept
    {
        m_entries.clear();
    }
    
    void Dico::clear(sTag key) noexcept
    {
        m_entries.erase(key);
    }
    
    void Dico::keys(ElemVector& elements) const noexcept
    {
        elements.clear();
        for(map<const sTag, ElemVector>::const_iterator it = m_entries.begin(); it != m_entries.end(); ++it)
        {
            elements.push_back(it->first);
        }
    }
    
    bool Dico::has(sTag key) const noexcept
    {
        return m_entries.find(key) != m_entries.end();
    }
    
    Element::Type Dico::type(sTag key) const noexcept
    {
        map<const sTag, ElemVector>::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            if(it->second.size() == 1)
            {
                return it->second[0].type();
            }
            else
            {
                return Element::VECTOR;
            }
        }
        else
            return Element::NOTHING;
    }
    
    Element Dico::get(sTag key) const noexcept
    {
        map<const sTag, ElemVector>::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            if(it->second.size())
            {
                return  it->second[0];
            }
        }
        return 0;
    }
    
    void Dico::get(sTag key, ElemVector& elements) const noexcept
    {
        map<const sTag, ElemVector>::const_iterator it = m_entries.find(key);
        if(it != m_entries.end())
        {
            elements = it->second;
        }
    }
    
    void Dico::set(sTag key, Element const& element) noexcept
    {
        ElemVector elements;
        elements.push_back(element);
        m_entries[key] = elements;
    }
    
    void Dico::set(sTag key, ElemVector const& elements) noexcept
    {
        if(elements.size() == 0)
            return;
        m_entries[key] = elements;
    }

    void Dico::append(sTag key, Element const& element) noexcept
    {
        map<const sTag, vector<Element> >::iterator it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(element);
        else
            set(key, element);
    }
    
    void Dico::append(sTag key, ElemVector const& elements) noexcept
    {
        map<const sTag, ElemVector>::iterator it = m_entries.find(key);
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
    
    
    void Dico::post()
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
        {
            string text;
            toJson(shared_from_this(), text);
            kiwi->post(text);
        }
    }
    
    void Dico::write(string& text)
    {
        text.clear();
        toJson(shared_from_this(), text);
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
            string text;
            toJson(shared_from_this(), text);
            file << text;
            
        }
        
        file.close();
    }
    
    void Dico::doread(sDico dico, ifstream& file, string& line)
    {
        while(getline(file, line))
        {
            if(line.find('}') != string::npos)
                return;
            
            size_t pos1;
            sTag key;
            
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
                            sDico subdico = createDico();
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
                                    sDico subdico = createDico();
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
    
    string Dico::jsonEscape(string const& text)
    {
        ostringstream ss;
        for(auto iter = text.cbegin(); iter != text.cend(); iter++)
        {
            switch (*iter)
            {
                case '\\': ss << "\\\\"; break;
                case '"': ss << "\\\""; break;
                case '/': ss << "\\/"; break;
                case '\b': ss << "\\b"; break;
                case '\f': ss << "\\f"; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default: ss << *iter; break;
            }
        }
        return "\"" + ss.str() + "\"";
    }
    
    string Dico::jsonUnescape(string const& text)
    {
        bool state = false;
        ostringstream ss;
        for(auto iter = text.cbegin()++; iter != text.cend(); iter++)
        {
            if(state)
            {
                switch(*iter)
                {
                    case '"': ss << '\"'; break;
                    case '/': ss << '/'; break;
                    case 'b': ss << '\b'; break;
                    case 'f': ss << '\f'; break;
                    case 'n': ss << '\n'; break;
                    case 'r': ss << '\r'; break;
                    case 't': ss << '\t'; break;
                    case '\\': ss << '\\'; break;
                    default: ss << *iter; break;
                }
                state = false;
            }
            else
            {
                switch(*iter)
                {
                    case '"': return ss.str();
                    case '\\': state = true; break;
                    default: ss << *iter; break;
                }
            }
        }
        return ss.str();
    }
    
    void Dico::toJson(Element const& element, string& text, string line)
    {
        switch(element.type())
        {
            case Element::LONG:
                text.append(to_string((long)element));
                break;
            case Element::DOUBLE:
                text.append(to_string((double)element));
                break;
            case Element::TAG:
            {
                sTag tag = (sTag)element;
                if(tag)
                    text.append(jsonEscape(toString(tag)));
                else
                    text.append("null");
                break;
            }
            case Element::OBJECT:
            {
                sObject obj = element;
                if(obj)
                {
                    sDico ndico = createDico();
                    obj->write(ndico);
                    sDico box = createDico();
                    box->set(tagBox, ndico);
                    toJson(box, text, line + "    ");
                }
                break;
            }
            case Element::DICO:
                toJson((sDico)element, text, line + "    ");
                break;
            default:
                return ;
                break;
        }
    }
    
    void Dico::toJson(ElemVector const& elements, string& text, string line)
    {
        if(elements.size() == 1)
        {
            toJson(elements[0], text, line);
        }
        else if(elements.size())
        {
            text.append("[ ");
            for(size_t i = 0; i < elements.size() - 1; i++)
            {
                toJson(elements[i], text, line);
                text.append(", ");
            }
            toJson(elements[elements.size() - 1], text, line);
            text.append(" ]");
        }
    }
    
    void Dico::toJson(shared_ptr<Dico> dico, string& text, string line)
    {
        ElemVector _keys;
        dico->keys(_keys);
        if(_keys.size())
        {
            text.append("{\n");
            for(size_t i = 0; i < _keys.size() - 1; i++)
            {
                ElemVector _values;
                dico->get(_keys[i], _values);
                if(_values.size())
                {
                    text.append(line + "    ");
                    toJson(_keys[i], text, line);
                    text.append(" : ");
                    toJson(_values, text, line);
                }
                text.append(",\n");
            }
            
            ElemVector _values;
            dico->get(_keys[_keys.size() - 1], _values);
            if(_values.size())
            {
                text.append(line + "    ");
                toJson(_keys[_keys.size() - 1], text, line);
                text.append(" : ");
                toJson(_values, text, line);
            }
            text.append("\n");
            
            text.append(line + "}");
        }
    }
    
    void Dico::fromJson(Element& element, string const& text)
    {
        size_t start = text.find_first_not_of(' ');
        if(isdigit(text[start]))
        {
            size_t end = text.find_first_not_of("-0123456789", start);
            switch(text[end])
            {
                case '.':
                    element = stod(text.c_str()+start);
                    break;
                default:
                    element = stol(text.c_str()+start);
                    break;
            }
        }
        else if(text[start] == '"')
        {
            element = createTag(jsonUnescape(text));
        }
    }
    
    void fromJson(ElemVector& element, string const& text)
    {
        ;
    }
    
    void fromJson(shared_ptr<Dico> dico, string const& text)
    {
        string ntext;
        size_t pos = 0;
        
    }
}
















