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

#include "DspNode.h"
#include "DspContext.h"
#include "../KiwiBase/Page.h"
#include "../KiwiBase/Tools.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP PROCESS                                 //
    // ================================================================================ //
    
    DspProcess::DspProcess(MethodProcess process, sBox box, int nins, int nouts, long vectorsize) :
    m_process(process),
    m_box(box),
    m_nins(nins),
    m_nouts(nouts),
    m_vectorsize(vectorsize)
    {
        m_ins = new sample*[m_nins];
        m_outs = new sample*[m_nouts];
    }
    
    DspProcess::~DspProcess()
    {
        delete [] m_ins;
        delete [] m_outs;
    }
    
    void DspProcess::setInput(int index, shared_ptr<Signal> &input)
    {
        if(index >= 0 && index < m_nins)
            m_ins[index] = input->getVector();
    }
    
    void DspProcess::setInputs(vector<shared_ptr<Signal>> &inputs)
    {
        for(int i = 0; i < inputs.size() && i < m_nins; i++)
        {
            m_ins[i] = inputs[i]->getVector();
        }
    }
    
    void DspProcess::setOutput(int index, shared_ptr<Signal> &output)
    {
        if(index >= 0 && index < m_nouts)
            m_outs[index] = output->getVector();
    }
    
    void DspProcess::setOutputs(vector<shared_ptr<Signal>> &outputs)
    {
        for(int i = 0; i < outputs.size() && i < m_nouts; i++)
            m_outs[i] = outputs[i]->getVector();
    }
    
    void DspProcess::copy(sBox none, long nins, sample const* const* ins, long nouts, sample** outs, long vectorsize)
    {
        signalAddSignal(ins[0], outs[0], vectorsize);
    }
}
















