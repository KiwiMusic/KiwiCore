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
#include "Atom.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    Dico::Dico()
    {
        ;
    }
    
    Dico::Dico(const Dico& other) :
    m_map(other.m_map)
    {
        
    }
    
    Dico::Dico(iterator first, iterator last) :
    m_map(first, last)
    {
        
    }
    
    Dico::Dico(Dico&& other) :
    m_map(move(other.m_map))
    {
        
    }
    
    Dico::Dico(initializer_list<value_type> il) :
    m_map(il)
    {
        
    }
    
    Dico::~Dico() noexcept
    {
        clear();
    }
    
    void Dico::clear() noexcept
    {
        m_map.clear();
    }
    
    Dico::iterator Dico::begin() noexcept
    {
        return m_map.begin();
    }
    
    Dico::const_iterator Dico::begin() const noexcept
    {
        return m_map.begin();
    }
    
    Dico::const_iterator Dico::cbegin() const noexcept
    {
        return m_map.cbegin();
    }
    
    Dico::reverse_iterator Dico::rbegin() noexcept
    {
        return m_map.rbegin();
    }

    Dico::const_reverse_iterator Dico::crbegin() const noexcept
    {
        return m_map.crbegin();
    }

    Dico::iterator Dico::end() noexcept
    {
        return m_map.end();
    }

    Dico::const_iterator Dico::end() const noexcept
    {
        return m_map.end();
    }

    Dico::const_iterator Dico::cend() const noexcept
    {
        return m_map.cend();
    }

    Dico::reverse_iterator Dico::rend() noexcept
    {
        return m_map.rend();
    }

    Dico::const_reverse_iterator Dico::crend() const noexcept
    {
        return m_map.crend();
    }

    bool Dico::has(const sTag& key) const noexcept
    {
        return (bool)m_map.count(key);
    }

    bool Dico::empty() const noexcept
    {
        return m_map.empty();
    }
    
    Dico::size_type Dico::max() const noexcept
    {
        return m_map.max_size();
    }
    
    Dico::size_type Dico::size() const noexcept
    {
        return m_map.size();
    }

    Dico::iterator Dico::erase(const_iterator position)
    {
        return m_map.erase(position);
    }

    bool Dico::erase(const sTag& key)
    {
        return (bool)m_map.erase(key);
    }

    Dico::iterator Dico::erase(const_iterator first, const_iterator last)
    {
        return m_map.erase(first, last);
    }

    Dico::iterator Dico::find(const sTag& key)
    {
        return m_map.find(key);
    }

    Dico::const_iterator Dico::find(const sTag& key) const
    {
        return m_map.find(key);
    }
    
    pair<Dico::iterator,bool> Dico::insert(const value_type& val)
    {
        return m_map.insert(val);
    }
    
    void Dico::insert(iterator first, iterator last)
    {
        m_map.insert(first, last);
    }
    
    void Dico::insert(initializer_list<value_type> il)
    {
        m_map.insert(il);
    }
    
    Atom& Dico::operator[](const sTag& key)
    {
        return m_map[key];
    }
    
    Atom& Dico::operator[](sTag&& key)
    {
        return m_map[key];
    }
    
    Atom const& Dico::operator[](const sTag& key) const
    {
        try
        {
            return m_map.at(key);
        }
        catch(exception e)
        {
            throw e;
        }
        
    }
    
    Atom const& Dico::operator[](sTag&& key) const
    {
        try
        {
            return m_map.at(key);
        }
        catch(exception e)
        {
            throw e;
        }
    }
}
















