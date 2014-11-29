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

#include "Instance.h"

#define Allocate(boxclass) sBox allocate(sPage page, sDico dico) const override { return make_shared<boxclass>(page); }

#define AllocateLong(boxclass) sBox allocate(sPage page, sDico dico) const override { return make_shared<boxclass>(page, (long)dico->get(Tag::arguments)); }

#define AllocateDouble(boxclass) sBox allocate(sPage page, sDico dico) const override { return make_shared<boxclass>(page, (double)dico->get(Tag::arguments)); }

#define AllocateElement(boxclass) sBox allocate(sPage page, sDico dico) const override { return make_shared<boxclass>(page, dico->get(Tag::arguments)); }

#define AllocateElemVector(boxclass) sBox allocate(sPage page, sDico dico) const override { ElemVector elements; dico->get(Tag::arguments, elements);return make_shared<boxclass>(page, elements); }

#endif


