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

#ifndef __DEF_KIWI_CORE__
#define __DEF_KIWI_CORE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Attribute.h"
#include "Box.h"
#include "Instance.h"
#include "DspNode.h"
#include "DspContext.h"

namespace Kiwi
{
    typedef shared_ptr<Instance>sInstance;
    typedef shared_ptr<Object>  sObject;
    typedef shared_ptr<Tag>     sTag;
    typedef shared_ptr<Dictionary> Dico;
    typedef vector<Element>     Elements;
};


#endif


