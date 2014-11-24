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

#ifndef __DEF_KIWI_DSP_CONTEXT__
#define __DEF_KIWI_DSP_CONTEXT__

#include "../KiwiBase/Defs.h"
#include "DspNode.h"

// Later make a specific error class for DSP

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    //! The dsp context manages a set of dsp nodes and builds the dsp chain.
    /**
     The dsp context initializes a dsp chain with a set of boxes and links. To create a dsp chain, first, you should setup the sample rate and the vector size, then add the boxes, then add the links, then you have to compile the dsp context.
     */
    class DspContext: public enable_shared_from_this<DspContext>
    {
    private:
        vector<shared_ptr<DspProcess>>  m_processes;
        vector<shared_ptr<Signal>>      m_signals;
        double                          m_samplerate;
        long                            m_vectorsize;
        
        vector<shared_ptr<DspNode>>     m_nodes;
        set<shared_ptr<DspNode>>        m_nodes_temp;
        int                             m_index;
        
        void indexNode(shared_ptr<DspNode> node);
        static bool compareNode(shared_ptr<DspNode> node1, shared_ptr<DspNode> node2);
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        DspContext();
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~DspContext();
        
        //! Set the sample rate of the dsp context.
        /** The function sets the sample rate of the dsp context.
         @param samplerate The sample rate.
         */
        void setSamplerate(double samplerate);
        
        //! Set the vector size of the dsp context.
        /** The function sets the vector size of the dsp context.
         @param vectorsize The vector size.
         */
        void setVectorsize(long vectorsize);
        
        //! Retrieve the sample rate of the context.
        /** This function retrieves the sample rate of the context.
         @return The sample rate of the context.
         */
        double getSamplerate() const noexcept;
        
        //! Retrieve the vector size of the context.
        /** This function retrieves the vector size of the context.
         @return The vector size of the context.
         */
        long getVectorsize() const noexcept;
        
        //! Retrieve the number of process.
        /** The function retrieves the number of process.
         @return The number of process.
         */
        size_t getNumberOfProcess() const noexcept;
        
        //! Add a box to the dsp context.
        /** The function adds a box to the dsp context.
         @param box The box to add.
         */
        void addBox(sBox box);
        
        //! Remove a box from the dsp context.
        /** The function removes a box from the dsp context.
         @param box The box to remove.
         */
        void removeBox(sBox box);
        
        //! Add a link to the dsp context.
        /** The function adds a link to the dsp context.
         @param link The link to add.
         */
        void addConnection(sLink link);
        
        //! Remove a link from the dsp context.
        /** The function removes a link from the dsp context.
         @param box The link to remove.
         */
        void removeLink(sLink link);
        
        //! Add a process to the dsp context.
        /** The function adds a process to the dsp context.
         @param process The process to add.
         */
        void addProcess(shared_ptr<DspProcess> process);
        
        //! Add a signal to the dsp context.
        /** The function adds a signal to the dsp context.
         @param signal The signal to add.
         */
        void addSignal(shared_ptr<Signal> signal);
        
        //! Compile the dsp context.
        /** The function sorts the dsp nodes and call the dsp methods of the boxes.
         */
        void compile();
        
        //! Perform a tick on the dsp context.
        /** The function calls once all the process methods of the dsp nodes.
         */
        void tick();
        
        //! Clear the dsp context.
        /** The function clears the dsp nodes and call the dsp methods of the boxes.
         */
        void clear();
    };
}


#endif


