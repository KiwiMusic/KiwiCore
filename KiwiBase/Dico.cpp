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
#include "Box.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    Dico::Dico() noexcept
    {
        ;
    }
    
    Dico::~Dico()
    {
        clear();
    }
    
    sDico Dico::create()
    {
        return make_shared<Dico>();
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
        for(auto it = m_entries.begin(); it != m_entries.end(); ++it)
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
        auto it = m_entries.find(key);
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
        auto it = m_entries.find(key);
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
        auto it = m_entries.find(key);
        if(it != m_entries.end())
        {
            elements = it->second;
        }
        else
        {
            elements.clear();
        }
    }
    
    void Dico::set(sTag key, Element const& element) noexcept
    {
        m_entries[key] = {element};
    }
    
    void Dico::set(sTag key, ElemVector const& elements) noexcept
    {
        if(elements.size() == 0)
            return;
        m_entries[key] = elements;
    }

    void Dico::append(sTag key, Element const& element) noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(element);
        else
            set(key, element);
    }
    
    void Dico::append(sTag key, ElemVector const& elements) noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
        {
            for(ElemVector::size_type i = 0; i < elements.size(); i++)
            {
                it->second.push_back(elements[i]);
            }
        }
        else
            set(key, elements);
    }
    
    void Dico::read(string const& text)
    {
        clear();
        size_t pos = 0;
        if(getType(text, pos) == Element::DICO)
        {
            fromJson(shared_from_this(), text, pos);
        }
        else
        {
            fromText(shared_from_this(), text);
        }
    }
    
    void Dico::read(string const& filename, string const& directoryname)
    {
        clear();
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
            string text;
            string line;
            while (getline(file, line))
            {
                text += line + "\n";
            }
            string::size_type pos = 0;
            fromJson(shared_from_this(), text, pos);
        }
        
        file.close();
    }

    void Dico::write(string& text) const
    {
        text.clear();
        toJson(shared_from_this(), text);
    }
    
    void Dico::write(string const& filename, string const& directoryname) const
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
            case Element::BOX:
            {
                sBox obj = element;
                if(obj)
                    text.append(toString(obj->getName()));
                else
                    text.append("null");
                break;
            }
            case Element::DICO:
            {
                sDico dico = element;
                if(dico)
                    toJson((scDico)dico, text, line + "    ");
                else
                    text.append("null");
                break;
            }
            default:
                text.append("null");
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
    
    void Dico::toJson(scDico dico, string& text, string line)
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
    
    string Dico::jsonUnescape(string const& text, string::size_type& pos)
    {
        bool state = false;
        ostringstream ss;
        pos++;
        for(auto iter = text.cbegin()+(long)pos; iter != text.cend(); iter++)
        {
            ++pos;
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
    
    Element::Type Dico::getType(string const& text, string::size_type pos)
    {
        if(isdigit(text[pos]))
        {
            pos = text.find_first_not_of("-0123456789", pos);
            if(text[pos] == '.')
                return Element::DOUBLE;
            else
                return Element::LONG;
        }
        else if(text[pos] == '"')
        {
            return Element::TAG;
        }
        else if(text[pos] == '{')
        {
            return Element::DICO;
        }
        else if(text[pos] == '[')
        {
            return Element::VECTOR;
        }
        else
        {
            return Element::NOTHING;
        }
    }
    
    bool getNextPosition(string const& text, string::size_type& pos, Element::Type type)
    {
        char end = '}';
        if(type == Element::VECTOR)
        {
            end = ']';
        }
        
        if(pos == string::npos)
        {
            return false;
        }
        else
        {
            pos = text.find_first_not_of(' ', pos+1);
            if(pos == string::npos || text[pos] == end)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    
    void Dico::fromJson(ElemVector& elements, string const& text, string::size_type& pos)
    {
        pos = text.find_first_not_of(' ', pos);
        switch(getType(text, pos))
        {
            case Element::LONG:
            {
                elements.push_back(stol(text.c_str()+pos));
                pos = text.find_first_not_of("-0123456789", pos);
            }
                break;
            case Element::DOUBLE:
            {
                elements.push_back(stod(text.c_str()+pos));
                pos = text.find_first_not_of("-0123456789.", pos);
            }
                break;
            case Element::TAG:
            {
                elements.push_back(Tag::create(jsonUnescape(text, pos)));
            }
                break;
            case Element::DICO:
            {
                sDico dico = Dico::create();
                if(dico)
                {
                    fromJson(dico, text, pos);
                    elements.push_back(dico);
                }
            }
                break;
            case Element::VECTOR:
            {
                while(getNextPosition(text, pos, Element::VECTOR))
                {
                    switch(getType(text, pos))
                    {
                        case Element::LONG:
                        {
                            elements.push_back(stol(text.c_str()+pos));
                            pos = text.find_first_not_of("-0123456789", pos);
                        }
                            break;
                        case Element::DOUBLE:
                        {
                            elements.push_back(stod(text.c_str()+pos));
                            pos = text.find_first_not_of("-0123456789.", pos);
                        }
                            break;
                        case Element::TAG:
                        {
                            elements.push_back(Tag::create(jsonUnescape(text, pos)));
                        }
                            break;
                        case Element::DICO:
                        {
                            sDico dico = Dico::create();
                            if(dico)
                            {
                                fromJson(dico, text, pos);
                                elements.push_back(dico);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
                break;
            default:
                pos = text.find_first_not_of(' ', ++pos);
                break;
        }
    }

    void Dico::fromJson(sDico dico, string const& text, string::size_type& pos)
    {
        pos = text.find('{', pos);
        while(getNextPosition(text, pos, Element::DICO))
        {
            if(getType(text, pos) == Element::TAG)
            {
                sTag key = Tag::create(jsonUnescape(text, pos));
                if(pos != string::npos)
                {
                    pos = text.find(':', pos);
                    if(pos != string::npos)
                    {
                        ElemVector values;
                        fromJson(values, text, ++pos);
                        dico->set(key, values);
                    }
                }
            }
        }
    }
    
    void Dico::fromText(sDico dico, string const& text)
    {
        bool mode = false;
        string word;
        string key = "name";
        ElemVector elements;
        istringstream iss(text);
        while(iss >> word)
        {
            if(mode)
            {
                if(word[0] == '@')
                {
                    dico->set(Tag::create(key), elements);
                    elements.clear();
                    key = word.c_str()+1;
                }
                else
                {                    
                    if(isdigit(word[0]))
                    {
                        if(word.find('.') != string::npos)
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
                        elements.push_back(Tag::create(word));
                    }
                }
            }
            else
            {
                dico->set(Tag::create(key), Tag::create(word));
                key = "arguments";
                mode = true;
            }
        }
        dico->set(Tag::create(key), elements);
        dico->set(Tag::text, Tag::create(text));
    }
}
















