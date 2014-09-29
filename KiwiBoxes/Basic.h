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

#ifndef __DEF_KIWI_BASIC__
#define __DEF_KIWI_BASIC__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                       BASIC                                      //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                       newobj                                     //
    // ================================================================================ //
    
    class NewObj : public Box
    {
    private:
        typedef shared_ptr<NewObj> This;
        friend class Basic;
    public:
        NewObj(sInstance kiwi);
        ~NewObj();
        
        static sObject create(sInstance kiwi, sTag name);
    };
    
    // ================================================================================ //
    //                                   BASIC LOADER                                   //
    // ================================================================================ //

    class Basic
    {
    public:
        static void load(shared_ptr<Instance> kiwi)
        {
            kiwi->addObjectPrototype(unique_ptr<Box>(new NewObj(kiwi)));
        }
    };
    
}

#endif


