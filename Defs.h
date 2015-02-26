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

#ifndef __DEF_KIWI_DEFS__
#define __DEF_KIWI_DEFS__

#include "../KiwiDsp/Dsp.h"
#include "../KiwiGui/Gui.h"

namespace Kiwi
{
    
    class Clock;
    typedef shared_ptr<Clock>           sClock;
    typedef weak_ptr<Clock>             wClock;
    
    class Atom;
    class Tag;
    typedef shared_ptr<const Tag>       sTag;
    typedef vector<Atom>                Vector;
    typedef map<sTag, Atom>             Dico;
    
    class Beacon;
    typedef shared_ptr<Beacon>          sBeacon;
    typedef weak_ptr<Beacon>            wBeacon;
    typedef shared_ptr<const Beacon>    scBeacon;
    typedef weak_ptr<const Beacon>      wcBeacon;
    
    class Link;
    typedef shared_ptr<Link>            sLink;
    typedef shared_ptr<const Link>      scLink;
    typedef weak_ptr<Link>              wLink;
    typedef weak_ptr<const Link>        wcLink;
    
    class Object;
    typedef shared_ptr<Object>          sObject;
    typedef weak_ptr<Object>            wObject;
    typedef shared_ptr<const Object>    scObject;
    typedef weak_ptr<const Object>      wcObject;
    
    class Patcher;
    typedef shared_ptr<Patcher>        sPatcher;
    typedef weak_ptr<Patcher>          wPatcher;
    typedef shared_ptr<const Patcher>  scPatcher;
    typedef weak_ptr<const Patcher>    wcPatcher;
    
    class Instance;
    typedef shared_ptr<Instance>        sInstance;
    typedef weak_ptr<Instance>          wInstance;
    typedef shared_ptr<const Instance>  scInstance;
    typedef weak_ptr<const Instance>    wcInstance;
    
    static inline string jsonUnescape(string const& text)
    {
        bool state = false;
        ostringstream ss;
        for(auto iter = text.cbegin(); iter != text.cend(); iter++)
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
    
};

#endif


