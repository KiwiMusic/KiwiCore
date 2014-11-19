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

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <set>
#include <unordered_set>
#include <deque>
#include <thread>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

using namespace std;

namespace Kiwi
{
#ifdef KIWI_DOUBLE
    typedef double sample;
#else
    typedef float sample;
#endif
    
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
    
    class Box;
    typedef shared_ptr<Box>         sBox;
    typedef weak_ptr<Box>           wBox;
    typedef shared_ptr<const Box>   scBox;
    typedef weak_ptr<const Box>     wcBox;
    
    class Page;
    typedef shared_ptr<Page>        sPage;
    typedef weak_ptr<Page>          wPage;
    typedef shared_ptr<const Page>  scPage;
    typedef weak_ptr<const Page>    wcPage;
    
    class Instance;
    typedef shared_ptr<Instance>    sInstance;
    typedef weak_ptr<Instance>      wInstance;
    
};

#endif


