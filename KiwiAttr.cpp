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

#include "KiwiAttr.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                 ATTRIBUTE MANAGER								//
    // ================================================================================ //
    
    void Attr::Manager::write(Dico& dico) const
    {
        ;
    }
    
    void Attr::Manager::read(Dico const& dico)
    {
        for(auto it = dico.begin(); it != dico.end(); ++it)
        {
            setAttrValue(it->first, it->second);
        }
    }
    
    void Attr::Manager::addListener(sListener listener, vector<sTag> const& names)
    {
        lock_guard<mutex> guard(m_list_mutex);
        auto it = m_list.begin();
        while(it != m_list.end())
        {
            if((*it))
            {
                if((*it) == listener)
                {
                    //(*it).attrs = names;
                    return;
                }
                ++it;
            }
            else
            {
                it = m_list.erase(it);
            }
        }
        m_list.insert(SpecListener{listener, names});
    }
    
    void Attr::Manager::removeListener(sListener listener, vector<sTag> const& names)
    {
        
    }
}
















