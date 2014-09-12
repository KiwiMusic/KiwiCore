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

#ifndef __DEF_KIWI_SIGNAL__
#define __DEF_KIWI_SIGNAL__

#include "../KiwiBase/Defs.h"

namespace Kiwi
{
    class DspProcess;
    // ================================================================================ //
    //                                      SIGNAL                                      //
    // ================================================================================ //
    
    //! The signal manages the sample vectors.
    /**
     The signal owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
     */
    class Signal
    {
    private:
        friend DspProcess;
        sample* m_vector    = nullptr;
        bool m_borrowed     = false;
        bool m_owner        = false;
        
        sample* getVector();
    public:
        
        //! Constructor.
        /** This function allocates a new vector and give the onwership and give the ownership to the instance.
         @param The size of the vector.
         */
        Signal(int size);
        
        //! Constructor.
        /** This function retieves the vector of the other signal. If borrow is setted, the other signal won't be able to share its vector.
         @other The signal that gives its vector.
         @borrow The borrow status.
         */
        Signal(shared_ptr<Signal> other, bool borrow);
        
        //! Destructor.
        /** This function free the vector if the signal have the ownership.
         */
        ~Signal();
        
        //! Check if the vector is borrowed.
        /** This function checks if the vector is borrowed.
         @return The borrowed status.
         */
        bool isBorrowed();
        
        inline bool operator==(Signal &other) noexcept
        {
            return other.m_vector == m_vector;
        }
        
        inline bool operator!=(Signal &other) noexcept
        {
            return other.m_vector != m_vector;
        }
    };
}


#endif


