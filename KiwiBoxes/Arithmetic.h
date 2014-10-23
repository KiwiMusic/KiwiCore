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

#ifndef __DEF_KIWI_ARITHMETIC__
#define __DEF_KIWI_ARITHMETIC__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ARITHMETIC                                      //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    class Plus : public Box
    {
    private:
        typedef shared_ptr<Plus> sPlus;
        friend class Arithmetic;
        const bool      m_double;
        double          m_augend;
        double          m_addend;
    public:
        Plus(sInstance kiwi, ElemVector const& elements = {0});
        ~Plus();
        void receive(ElemVector const& elements) override;
        sObject create(sDico dico) const override;
    };
    
    // ================================================================================ //
    //                              ARITHMETIC LOADER                                   //
    // ================================================================================ //
    
    class Arithmetic
    {
    public:
        static void load(sInstance kiwi)
        {
            kiwi->addObjectPrototype(unique_ptr<Box>(new Plus(kiwi)));
        }
    };
    
}

#endif


