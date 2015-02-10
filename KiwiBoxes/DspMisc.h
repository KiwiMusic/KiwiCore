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

#ifndef __DEF_KIWI_MISC_TILDE__
#define __DEF_KIWI_MISC_TILDE__

#include "../KiwiBase/Core.h"
#include "../KiwiDsp/Dsp.h"

namespace Kiwi
{
    /*
    class SnapshotTilde : public Object, public Dsp::Process
    {
    public:
        SnapshotTilde(sPage page);
        SnapshotTilde(sPage page, const double value);
        ~SnapshotTilde();
        bool receive(ulong index, ElemVector const& elements) override;
        sObject allocate(sPage page, sDico dico) const override;
        ulong getNumberOfInputs() const noexcept override;
        ulong getNumberOfOutputs() const noexcept override;
        void prepare(Dsp::sNode node) const noexcept override;
        void perform(Dsp::scNode node) const noexcept override;
        void release(Dsp::scNode node) const noexcept override;
    };
    
    inline void MiscTildeInit()
    {
        Object::addPrototype(unique_ptr<Object>(new SnapshotTilde(sPage())));
    }*/
}

#endif


