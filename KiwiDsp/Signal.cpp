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

#include "Signal.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIGNAL                                      //
    // ================================================================================ //
    
    Signal::Signal(int size)
    {
        m_vector            = new sample[size];
        m_borrowed          = false;
        m_owner             = true;
    }
    
    Signal::Signal(shared_ptr<Signal> sig, bool borrow)
    {
        m_vector            = sig->m_vector;
        m_borrowed          = false;
        m_owner             = false;
        sig->m_borrowed     = borrow;
    }
    
    Signal::~Signal()
    {
        if(m_owner && m_vector)
        {
            delete [] m_vector;
        }
    }
    
    sample* Signal::getVector()
    {
        return m_vector;
    }
    
    bool Signal::isBorrowed()
    {
        return m_borrowed;
    }
}
















