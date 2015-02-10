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
    using namespace Gui;
    using namespace Dsp;
    
    class Clock;
    typedef shared_ptr<Clock>       sClock;
    typedef weak_ptr<Clock>         wClock;
    
    class Tag;
    typedef shared_ptr<const Tag>   sTag;
    typedef weak_ptr<const Tag>     wTag;
    
    class Beacon;
    typedef shared_ptr<Beacon>      sBeacon;
    typedef weak_ptr<Beacon>        wBeacon;
    
    class Dico;
    typedef shared_ptr<Dico>        sDico;
    typedef shared_ptr<const Dico>  scDico;
    typedef weak_ptr<Dico>          wDico;
    typedef weak_ptr<const Dico>    wcDico;
    
    class Link;
    typedef shared_ptr<Link>        sLink;
    typedef shared_ptr<const Link>  scLink;
    typedef weak_ptr<Link>          wLink;
    typedef weak_ptr<const Link>    wcLink;
    
    class Object;
    typedef shared_ptr<Object>      sObject;
    typedef weak_ptr<Object>        wObject;
    typedef shared_ptr<const Object>scObject;
    typedef weak_ptr<const Object>  wcObject;
    
    class Page;
    typedef shared_ptr<Page>        sPage;
    typedef weak_ptr<Page>          wPage;
    typedef shared_ptr<const Page>  scPage;
    typedef weak_ptr<const Page>    wcPage;
    
    class Instance;
    typedef shared_ptr<Instance>        sInstance;
    typedef weak_ptr<Instance>          wInstance;
    typedef shared_ptr<const Instance>  scInstance;
    typedef weak_ptr<const Instance>    wcInstance;
    
};

#endif


