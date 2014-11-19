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

#ifndef __DEF_KIWI_DSP_NODE__
#define __DEF_KIWI_DSP_NODE__

#include "../KiwiBase/Defs.h"
#include "../KiwiBase/Box.h"
#include "Signal.h"
#include "DspProcess.h"

namespace Kiwi
{
    class Box;
    class Link;
    class DspContext;

    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    //! The dsp nodes manages the digital signal processing of a box.
    /**
     The dsp node manages the creaton of the dsp process of a box and should be configure during the call of the dsp method of the box.
     @see DspContext
     */
    class DspNode: public enable_shared_from_this<DspNode>
    {
    private:
        friend DspContext;
        
        typedef void (*MethodDsp)(sBox, shared_ptr<DspNode> node);
        typedef void (*MethodProcess)(sBox, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize);
        
        const sBox   m_box;
        const int               m_nins;
        const int               m_nouts;
        long                    m_vectorsize;
        double                  m_samplerate;
        
        vector<shared_ptr<Signal>> m_sig_ins;
        vector<shared_ptr<Signal>> m_sig_outs;
        
        bool                    m_inplace;
        
        bool                    m_valid;
        int                     m_index;
        
        vector< set<weak_ptr<DspNode>, owner_less< weak_ptr<DspNode> > > >  m_inputs_nodes;
        vector< set<weak_ptr<DspNode>, owner_less< weak_ptr<DspNode> > > >  m_outputs_nodes;
        vector<MethodProcess>                                               m_processes;
        
        void prepare(shared_ptr<DspContext> context);
        void allocSignals(shared_ptr<DspContext> context);
        
        void addInput(weak_ptr<DspNode>, int inlet);
        void addOutput(weak_ptr<DspNode>, int outlet);
        
        void removeInput(weak_ptr<DspNode>, int inlet);
        void removeOutput(weak_ptr<DspNode>, int outlet);
        
        shared_ptr<Signal> getOutputSignal(shared_ptr<DspNode> inputnode);
        
        void clean();

    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        DspNode(sBox);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~DspNode();
        
        //! Add a process method to the node.
        /** This function adds a process method to the node.
         @param method The process method to add.
         */
        //void    addMethod(Method method);
        
        //! Retrieve the sample rate of the node.
        /** This function retrieves the sample rate of the node.
         @return The sample rate of the node.
         */
        double  getSamplerate() const noexcept;
        
        //! Retrieve the vector size of the node.
        /** This function retrieves the vector size of the node.
         @return The vector size of the node.
         */
        long    getVectorsize() const noexcept;
        
        //! Retrieve the number of inputs.
        /** This function retrieves the number of inputs.
         @return The number of inputs.
         */
        int  getNumberOfInputs() const noexcept;
        
        //! Retrieve the number of outputs.
        /** This function retrieves the number of outputs.
         @return The number of outputs.
         */
        int    getNumberOfOutputs() const noexcept;
        
        //! Check if a signal inlet is connected with signal.
        /** This function checks if a signal inlet is connected with signal.
         @return True if the inlet is connected otherwise it returns false.
         */
        bool    isInputConnected(long index) const noexcept;
        
        //! Check if a signal outlet is connected with signal.
        /** This function checks if a signal outlet is connected with signal.
         @return True if the outlet is connected otherwise it returns false.
         */
        bool    isOutputConnected(long index) const noexcept;
        
        //! Check if the inputs and outputs signals owns the same vectors.
        /** This function checks if the signals owns the same vectors.
         @return True if the signals owns the same vectors it returns false.
         */
        bool isInplace() const noexcept;
        
        //! Set if the inputs and outputs signals owns the same vectors.
        /** This function sets if the signals owns the same vectors.
         @param status The inplace status.
         */
        void setInplace(bool status);
        
        //! Check if the node is valid.
        /** This checks if the node is valid.
         @return True if the node is valid otherwise it returns false.
         */
        bool    isValid() const noexcept;
    };
}


#endif


