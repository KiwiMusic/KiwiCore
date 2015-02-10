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

#ifndef __DEF_KIWI_TIME__
#define __DEF_KIWI_TIME__

#include "../KiwiBase/Core.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     TIME                                         //
    // ================================================================================ //
    
    // ================================================================================ //
    //                                     METRO                                        //
    // ================================================================================ //
    
    class Metro : public Object, public Clock::Maker
    {
    private:
		atomic_bool	m_active;
		double      m_interval;
        sClock      m_clock;
    public:
        
        Metro(sPage page, ElemVector const& elements = {});
        ~Metro();
        void receive(ulong index, ElemVector const& elements) override;
        void tick() override;
        AllocateElemVector(Metro);
    };
    
    inline void timing()
    {
		Object::addPrototype(unique_ptr<Object>(new Metro(sPage())));
    }
}

#endif


