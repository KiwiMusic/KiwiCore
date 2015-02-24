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
#include "Patcher.h"

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
    
    sDico Dico::create(scDico dico)
    {
        sDico ndico = make_shared<Dico>();
        if(ndico && dico)
        {
            ndico->m_entries = dico->m_entries;
        }
        return ndico;
    }
    
    sDico Dico::evaluateForJson(string const& text)
    {
        sDico dico = make_shared<Dico>();
        if(dico)
        {
            size_t pos = 0;
            if(getType(text, pos) == Atom::DICO)
            {
                fromJson(dico, text, pos);
            }
        }
        return dico;
    }
    
    void Dico::evaluateObject(sDico dico, string const& text)
    {
        if(dico)
        {
            sDico object = Dico::create();
            if(object)
            {
                bool mode = false;
                string word;
                string key = string("name");
                istringstream iss;
                vector<Atom> atoms;
                while(iss >> word)
                {
                    if(mode)
                    {
                        if(word[0] == '@')
                        {
                            object->set(Tag::create(key), atoms);
                            atoms.clear();
                            key = word.c_str()+1;
                        }
                        else
                        {
                            if(isdigit(word[0]))
                            {
                                if(word.find('.') != string::npos)
                                {
                                    atoms.push_back(atof(word.c_str()));
                                }
                                else
                                {
                                    atoms.push_back(atol(word.c_str()));
                                }
                            }
                            else
                            {
                                atoms.push_back(Tag::create(word));
                            }
                        }
                    }
                    else
                    {
                        object->set(Tag::create(key), Tag::create(word));
                        key = "arguments";
                        mode = true;
                    }
                }
                if(mode)
                {
                    object->set(Tag::create(key), atoms);
                    object->set(Tag::List::text, Tag::create(text));
                    dico->set(Tag::List::objects, vector<Atom>({object}));
                }
            }
        }
    }
    
    void Dico::evaluateLink(sDico dico, string const& text)
    {
        if(dico)
        {
            sDico link = Dico::create();
            if(link)
            {
                vector<Atom> from, to;
                size_t pos = text.find_first_not_of(' ', 0);
                size_t type = getType(text, pos);
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    from.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    from.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    to.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    to.push_back(stol(text.c_str()+pos));
                }
                if(from.size() == 2 && to.size() == 2)
                {
                    link->set(Tag::List::from, from);
                    link->set(Tag::List::to, to);
                    dico->set(Tag::List::links, vector<Atom>({link}));
                }
                
            }
        }
    }
    
    sDico Dico::evaluateForPatcher(string& text)
    {
        sDico dico = make_shared<Dico>();
        if(dico)
        {
            string word;
            istringstream iss(text);
            while(iss >> word)
            {
                sTag command = Tag::create(word);
                if(command)
                {
                    text.erase(text.begin(), text.begin()+word.size());
                    if(command == Tag::List::newobject)
                    {
                        evaluateObject(dico, text);
                    }
                    else if(command == Tag::List::newlink)
                    {
                        evaluateLink(dico, text);
                    }
                    else if(command == Tag::List::removeobject)
                    {
                        evaluateObject(dico, text);
                    }
                    else if(command == Tag::List::removelink)
                    {
                        evaluateLink(dico, text);
                    }
                    dico->set(Tag::List::command, command);
                }
            }
        }
        return dico;
    }
    
    sDico Dico::evaluateForObject(string const& text)
    {
        sDico dico = make_shared<Dico>();
        if(dico)
        {
            sDico object = Dico::create();
            if(object)
            {
                bool mode = false;
                string word;
                string key = "name";
                vector<Atom> atoms;
                istringstream iss(text);
                while(iss >> word)
                {
                    if(mode)
                    {
                        if(word[0] == '@')
                        {
                            object->set(Tag::create(key), atoms);
                            atoms.clear();
                            key = word.c_str()+1;
                        }
                        else
                        {
                            if(isdigit(word[0]))
                            {
                                if(word.find('.') != string::npos)
                                {
                                    atoms.push_back(atof(word.c_str()));
                                }
                                else
                                {
                                    atoms.push_back(atol(word.c_str()));
                                }
                            }
                            else
                            {
                                atoms.push_back(Tag::create(word));
                            }
                        }
                    }
                    else
                    {
                        object->set(Tag::create(key), Tag::create(word));
                        key = "arguments";
                        mode = true;
                    }
                }
                if(mode)
                {
                    object->set(Tag::create(key), atoms);
                    object->set(Tag::List::text, Tag::create(text));
                    dico->set(Tag::List::objects, vector<Atom>({object}));
                    return dico;
                }
            }
        }
        return nullptr;
    }
    
    sDico Dico::evaluateForLink(string const& text)
    {
        sDico dico = make_shared<Dico>();
        if(dico)
        {
            sDico link = Dico::create();
            if(link)
            {
                vector<Atom> from, to;
                size_t pos = text.find_first_not_of(' ', 0);
                size_t type = getType(text, pos);
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    from.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    from.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    to.push_back(stol(text.c_str()+pos));
                    pos = text.find(' ', pos);
                    pos = text.find_first_not_of(' ', pos);
                    type = getType(text, pos);
                }
                if(type == Atom::LONG || type == Atom::DOUBLE)
                {
                    to.push_back(stol(text.c_str()+pos));
                }
                if(from.size() == 2 && to.size() == 2)
                {
                    link->set(Tag::List::from, from);
                    link->set(Tag::List::to, to);
                    dico->set(Tag::List::links, vector<Atom>({link}));
                    return dico;
                }
                
            }
        }
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
    
    void Dico::keys(vector<Atom>& atoms) const noexcept
    {
        atoms.clear();
        for(auto it = m_entries.begin(); it != m_entries.end(); ++it)
        {
            atoms.push_back(it->first);
        }
    }
    
    bool Dico::has(sTag key) const noexcept
    {
        return m_entries.find(key) != m_entries.end();
    }
    
    size_t Dico::type(sTag key) const noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
        {
            if(it->second.size() == 1)
            {
                return it->second[0].getType();
            }
            else
            {
                return Atom::VECTOR;
            }
        }
        else
            return Atom::NOTHING;
    }
    
    Atom Dico::get(sTag key) const noexcept
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
    
    void Dico::get(sTag key, vector<Atom>& atoms) const noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
        {
            atoms = it->second;
        }
        else
        {
            atoms.clear();
        }
    }
    
    void Dico::set(sTag key, Atom const& atom) noexcept
    {
        m_entries[key] = {atom};
    }
    
    void Dico::set(sTag key, vector<Atom> const& atoms) noexcept
    {
        if(atoms.size() == 0)
            return;
        m_entries[key] = atoms;
    }

    void Dico::append(sTag key, Atom const& atom) noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
            it->second.push_back(atom);
        else
            set(key, atom);
    }
    
    void Dico::append(sTag key, vector<Atom> const& atoms) noexcept
    {
        auto it = m_entries.find(key);
        if(it != m_entries.end())
        {
            for(vector<Atom>::size_type i = 0; i < atoms.size(); i++)
            {
                it->second.push_back(atoms[i]);
            }
        }
        else
            set(key, atoms);
    }
    /*
    void Dico::read(string const& text)
    {
        clear();
        size_t pos = 0;
        if(getType(text, pos) == Atom::DICO)
        {
            size_t pos = 0;
            fromJson(shared_from_this(), text, pos);
        }
    }
    */
    void Dico::read(string const& filename, string const& directoryname)
    {
        clear();
        ifstream file;
        if(!directoryname.empty() && !filename.empty())
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
    
    void Dico::toJson(Atom const& atom, string& text, string line)
    {
        switch(atom.getType())
        {
            case Atom::LONG:
                text.append(to_string((long)atom));
                break;
            case Atom::DOUBLE:
                text.append(to_string((double)atom));
                break;
            case Atom::TAG:
            {
                sTag tag = (sTag)atom;
                if(tag)
                    text.append(jsonEscape(toString(tag)));
                else
                    text.append("null");
                break;
            }
            case Atom::OBJECT:
            {
                scObject obj = atom;
                if(obj)
                    text.append(toString(obj->getName()));
                else
                    text.append("null");
                break;
            }
            case Atom::DICO:
            {
                scDico dico = atom;
                if(dico)
                    toJson(dico, text, line + "    ");
                else
                    text.append("null");
                break;
            }
            default:
                text.append("null");
                break;
        }
    }
    
    void Dico::toJson(vector<Atom> const& atoms, string& text, string line)
    {
        if(atoms.size() == 1)
        {
            toJson(atoms[0], text, line);
        }
        else if(atoms.size())
        {
            text.append("[ ");
            for(size_t i = 0; i < atoms.size() - 1; i++)
            {
                toJson(atoms[i], text, line);
                text.append(", ");
            }
            toJson(atoms[atoms.size() - 1], text, line);
            text.append(" ]");
        }
    }
    
    void Dico::toJson(scDico dico, string& text, string line)
    {
        vector<Atom> _keys;
        dico->keys(_keys);
        if(_keys.size())
        {
            text.append("{\n");
            for(size_t i = 0; i < _keys.size() - 1; i++)
            {
                vector<Atom> _values;
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
            
            vector<Atom> _values;
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
    
    size_t Dico::getType(string const& text, string::size_type pos)
    {
        if(isdigit(text[pos]))
        {
            pos = text.find_first_not_of("-0123456789", pos);
            if(text[pos] == '.')
                return Atom::DOUBLE;
            else
                return Atom::LONG;
        }
        else if(text[pos] == '"')
        {
            return Atom::TAG;
        }
        else if(text[pos] == '{')
        {
            return Atom::DICO;
        }
        else if(text[pos] == '[')
        {
            return Atom::VECTOR;
        }
        else
        {
            return Atom::NOTHING;
        }
    }
    
    bool getNextPosition(string const& text, string::size_type& pos, size_t type)
    {
        char end = '}';
        if(type == Atom::VECTOR)
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
    
    void Dico::fromJson(vector<Atom>& atoms, string const& text, string::size_type& pos)
    {
        pos = text.find_first_not_of(' ', pos);
        switch(getType(text, pos))
        {
            case Atom::LONG:
            {
                atoms.push_back(stol(text.c_str()+pos));
                pos = text.find_first_not_of("-0123456789", pos);
            }
                break;
            case Atom::DOUBLE:
            {
                atoms.push_back(stod(text.c_str()+pos));
                pos = text.find_first_not_of("-0123456789.", pos);
            }
                break;
            case Atom::TAG:
            {
                atoms.push_back(Tag::create(jsonUnescape(text, pos)));
            }
                break;
            case Atom::DICO:
            {
                sDico dico = Dico::create();
                if(dico)
                {
                    fromJson(dico, text, pos);
                    atoms.push_back(dico);
                }
            }
                break;
            case Atom::VECTOR:
            {
                while(getNextPosition(text, pos, Atom::VECTOR))
                {
                    switch(getType(text, pos))
                    {
                        case Atom::LONG:
                        {
                            atoms.push_back(stol(text.c_str()+pos));
                            pos = text.find_first_not_of("-0123456789", pos);
                        }
                            break;
                        case Atom::DOUBLE:
                        {
                            atoms.push_back(stod(text.c_str()+pos));
                            pos = text.find_first_not_of("-0123456789.", pos);
                        }
                            break;
                        case Atom::TAG:
                        {
                            atoms.push_back(Tag::create(jsonUnescape(text, pos)));
                        }
                            break;
                        case Atom::DICO:
                        {
                            sDico dico = Dico::create();
                            if(dico)
                            {
                                fromJson(dico, text, pos);
                                atoms.push_back(dico);
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
        while(getNextPosition(text, pos, Atom::DICO))
        {
            if(getType(text, pos) == Atom::TAG)
            {
                sTag key = Tag::create(jsonUnescape(text, pos));
                if(pos != string::npos)
                {
                    pos = text.find(':', pos);
                    if(pos != string::npos)
                    {
                        vector<Atom> values;
                        fromJson(values, text, ++pos);
                        dico->set(key, values);
                    }
                }
            }
        }
    }
}
















