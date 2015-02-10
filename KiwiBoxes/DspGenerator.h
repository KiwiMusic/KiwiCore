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

#ifndef __DEF_KIWI_GENERATOR_TILDE__
#define __DEF_KIWI_GENERATOR_TILDE__

#include "../KiwiBase/Core.h"
#include "../KiwiDsp/Dsp.h"

namespace Kiwi
{
    class SigTilde : public Object, public Dsp::Sig
    {
    public:
        SigTilde(sPage page);
        SigTilde(sPage page, const double value);
        ~SigTilde();
        void receive(ulong index, ElemVector const& elements) override;
        sObject allocate(sPage page, sDico dico) const override;
    };
    
    inline void GeneratorTildeInit()
    {
        Object::addPrototype(unique_ptr<Object>(new SigTilde(sPage())));
    }
}

#endif


