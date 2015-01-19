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

#include "DspGenerator.h"

namespace Kiwi
{
    SigTilde::SigTilde(sPage page) : Box(page, "sig~")
    {
        addInlet(Iolet::Message, Iolet::Hot, "Value (long or double)");
        addOutlet(Iolet::Signal, "Value (signal)");
    }
    
    SigTilde::SigTilde(sPage page, const double value) : Box(page, "sig~")
    {
        Dsp::Sig::setValue(value);
        addOutlet(Iolet::Signal, "Value (signal)");
    }

    SigTilde::~SigTilde()
    {
        ;
    }
    
    bool SigTilde::receive(ulong index, ElemVector const& elements)
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
    
    sBox SigTilde::allocate(sPage page, sDico dico) const
    {
        ElemVector elements;
        dico->get(Tag_arguments, elements);
        if(!elements.empty() && elements[0].isNumber())
        {
            return make_shared<SigTilde>(page, (double)elements[0]);
        }
        else
        {
            return make_shared<SigTilde>(page);
        }
    }
}

