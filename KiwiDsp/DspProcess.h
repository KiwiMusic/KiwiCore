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

#ifndef __DEF_KIWI_DSP_PROCESS__
#define __DEF_KIWI_DSP_PROCESS__

#include "../KiwiBase/Defs.h"
#include "../KiwiBase/Box.h"
#include "Signal.h"

namespace Kiwi
{
    class Box;
    
    // ================================================================================ //
    //                                      DSP PROCESS                                 //
    // ================================================================================ //
    
    //! The dsp process owns the method and the necessary members of one digital signal processing
    /**
     The dsp process performs the digital signal processing of a box, it is configured by the dsp node and called by the dsp context.
     @see DspNode
     @see DspContext
     */
    class DspProcess
    {
    private:
        typedef void (*MethodProcess)(sBox, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize);
        
        const MethodProcess     m_process;
        const sBox   m_box;
        const int               m_nins;
        const int               m_nouts;
        const long              m_vectorsize;
        sample**                m_ins = nullptr;
        sample**                m_outs = nullptr;
    public:
        
        //! Constructor.
        /** This function retrieves the default members and allocates the inputs and outputs matrices.
         @param process     The process to call.
         @param box         The box to linked ot the process.
         @param nins        The number of input signals.
         @param nouts       The number of output signals.
         @param vectorsize  The vector size.
         */
        DspProcess(MethodProcess process, sBox box, int nins, int nouts, long vectorsize);
        
        //! Destructor.
        /** This function shoulds free the matrices but we have an small error for the moment...
         */
        ~DspProcess();
        
        //! Set the input signal.
        /** This function sets the input signal at a specific index.
         @param index The input index.
         @param input The input signal.
         */
        void setInput(int index, shared_ptr<Signal> &input);
        
        //! Set the input signals.
        /** This function sets the input signals.
         @param inputs The input signals.
         */
        void setInputs(vector<shared_ptr<Signal>> &inputs);
        
        //! Set the output signal.
        /** This function sets the output signal at a specific index.
         @param index The output index.
         @param input The output signal.
         */
        void setOutput(int index, shared_ptr<Signal> &output);
        
        //! Set the output signals.
        /** This function sets the output signals.
         @param inputs The output signals.
         */
        void setOutputs(vector<shared_ptr<Signal>> &outputs);
      
        //! Call the process.
        /** This function calls the process method.
         */
        inline void tick()
        {
            m_process(m_box, m_nins, m_ins, m_nouts, m_outs, m_vectorsize);
        }
        
        //! The copy default method.
        /** The default process method to copy a vector to another one.
         */
        static void copy(sBox none, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize);
    };
}


#endif


