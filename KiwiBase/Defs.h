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
#include <cwchar>
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

#define _USE_MATH_DEFINES

#ifndef M_PI
#define M_E         2.71828182845904523536028747135266250   /* e              */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */
#endif

namespace Kiwi
{    
    typedef unsigned long ulong;
    
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
    
    class Attr;
    typedef shared_ptr<Attr>        sAttr;
    typedef weak_ptr<Attr>          wAttr;
    typedef shared_ptr<const Attr>  scAttr;
    typedef weak_ptr<const Attr>    wcAttr;
    
    class AttrBool;
    typedef shared_ptr<AttrBool>        sAttrBool;
    typedef weak_ptr<AttrBool>          wAttrBool;
    typedef shared_ptr<const AttrBool>  scAttrBool;
    typedef weak_ptr<const AttrBool>    wcAttrBool;
    
    class AttrLong;
    typedef shared_ptr<AttrLong>        sAttrLong;
    typedef weak_ptr<AttrLong>          wAttrLong;
    typedef shared_ptr<const AttrLong>  scAttrLong;
    typedef weak_ptr<const AttrLong>    wcAttrLong;
    
    class AttrDouble;
    typedef shared_ptr<AttrDouble>      sAttrDouble;
    typedef weak_ptr<AttrDouble>        wAttrDouble;
    typedef shared_ptr<const AttrDouble>scAttrDouble;
    typedef weak_ptr<const AttrDouble>  wcAttrDouble;
    
    class AttrTag;
    typedef shared_ptr<AttrTag>         sAttrTag;
    typedef weak_ptr<AttrTag>           wAttrTag;
    typedef shared_ptr<const AttrTag>   scAttrTag;
    typedef weak_ptr<const AttrTag>     wcAttrTag;
    
    class AttrColor;
    typedef shared_ptr<AttrColor>       sAttrColor;
    typedef weak_ptr<AttrColor>         wAttrColor;
    typedef shared_ptr<const AttrColor> scAttrColor;
    typedef weak_ptr<const AttrColor>   wcAttrColor;
    
    class Knock;
    
    class Inlet;
    typedef shared_ptr<Inlet>       sInlet;
    typedef unique_ptr<Inlet>       uInlet;
    
    class Outlet;
    typedef shared_ptr<Outlet>      sOutlet;
    typedef unique_ptr<Outlet>      uOutlet;
    
    class Link;
    typedef shared_ptr<Link>        sLink;
    typedef shared_ptr<const Link>  scLink;
    typedef weak_ptr<Link>          wLink;
    typedef weak_ptr<const Link>    wcLink;
    
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
    
    class Point;
    
};

#endif


