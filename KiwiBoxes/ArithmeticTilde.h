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

#ifndef __DEF_KIWI_ARITHMETIC_TILDE__
#define __DEF_KIWI_ARITHMETIC_TILDE__

#include "../KiwiBase/Core.h"
#include "../KiwiDsp/Dsp.h"

namespace Kiwi
{
    class SigTilde : public Box, public Dsp::Sig
    {
    public:
        
        SigTilde(sPage page) : Box(page, "sig~")
        {
            addInlet(Iolet::Message, Iolet::Hot, "Vamllll 1");
        }

        SigTilde(sPage page, const double value) : Box(page, "sig~")
        {
            ;
        }

        
        ~SigTilde()
        {
            ;
        }
        
        bool receive(ulong index, ElemVector const& elements) override
        {
            if(!elements.empty() && elements[0].isNumber())
            {
                Dsp::Sig::setValue(elements[0]);
                return  true;
            }
            else
            {
                return  false;
            }
        }
    
        sBox allocate(sPage page, sDico dico) const override
        {
            
            return make_shared<SigTilde>(page);
        }
    };
}

#endif


