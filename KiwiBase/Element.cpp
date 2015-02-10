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

#include "Element.h"
#include "Object.h"
#include "Dico.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      ELEMENT                                     //
    // ================================================================================ //
    string toString(Element const& __val)
    {
        switch(__val.type())
        {
            case Element::LONG:
                return to_string((long)__val);
                break;
            case Element::DOUBLE:
                return toString((double)__val);
                break;
            case Element::TAG:
                return toString((sTag)__val);
                break;
            case Element::OBJECT:
                return toString((scObject)__val);
                break;
            default:
                return toString((scDico)__val);
                break;
        }
    }
    
    string toString(ElemVector const& __val)
    {
        if(__val.size())
        {
            string desc;
            for(size_t i = 0; i < __val.size() - 1; i++)
            {
                desc += toString(__val[i]) + " ";
            }
            desc += toString(__val[__val.size() - 1]);
            return desc;
        }
        return "";
    }
    
    string toString(Element::Type __val)
    {
        switch(__val)
        {
            case Element::NOTHING:
                return "nothing";
                break;
            case Element::LONG:
                return "long";
                break;
            case Element::DOUBLE:
                return "double";
                break;
            case Element::TAG:
                return "tag";
                break;
            case Element::OBJECT:
                return "object";
                break;
            case Element::DICO:
                return "dico";
                break;
            default:
                return "elements";
                break;
        }
    }
}
















