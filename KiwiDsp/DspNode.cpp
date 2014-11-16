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
#include "../KiwiBase/Box.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    DspNode::DspNode(sBox box) :
    
    m_box(box),
    m_nins(0),
    m_nouts(0),
    
    m_vectorsize(0),
    m_samplerate(0),
    
    m_inplace(false),
    m_valid(false),
    m_index(-1)
    
    {
        m_inputs_nodes.resize(m_nins);
        m_outputs_nodes.resize(m_nouts);
    }
    
    DspNode::~DspNode()
    {
        int size;
        size = (int)m_inputs_nodes.size();
        for(int i = 0; i < size; i++)
            m_inputs_nodes[i].clear();
        m_inputs_nodes.clear();
        
        size = (int)m_outputs_nodes.size();
        for(int i = 0; i < size; i++)
            m_outputs_nodes[i].clear();
        m_outputs_nodes.clear();
        
        m_processes.clear();
        m_sig_ins.clear();
        m_sig_outs.clear();
    }
    
    void DspNode::addInput(weak_ptr<DspNode> node, int inlet)
    {
        if(inlet >= 0 && inlet < m_nins)
            m_inputs_nodes[inlet].insert(node);
    }
    
    void DspNode::addOutput(weak_ptr<DspNode> node, int outlet)
    {
        if(outlet >= 0 && outlet < m_nouts)
            m_outputs_nodes[outlet].insert(node);
    }
    
    void DspNode::removeInput(weak_ptr<DspNode> node, int inlet)
    {
        if(inlet >= 0 && inlet < m_nins)
            m_inputs_nodes[inlet].erase(node);
    }
    
    void DspNode::removeOutput(weak_ptr<DspNode> node, int outlet)
    {
        if(outlet >= 0 && outlet < m_nouts)
            m_outputs_nodes[outlet].erase(node);
    }
    /*
    void DspNode::addMethod(Method method)
    {
        m_processes.push_back((MethodProcess)method);
    }*/
    
    double DspNode::getSamplerate() const noexcept
    {
        return m_samplerate;
    }
    
    long DspNode::getVectorsize() const noexcept
    {
        return m_vectorsize;
    }
    
    int DspNode::getNumberOfInputs() const noexcept
    {
        return m_nins;
    }
    
    int DspNode::getNumberOfOutputs() const noexcept
    {
        return m_nouts;
    }
    
    bool DspNode::isInputConnected(long index) const noexcept
    {
        if(index >= 0 && index < m_nins)
            return m_inputs_nodes[index].size();
        else
            return 0;
    }
    
    bool DspNode::isOutputConnected(long index) const noexcept
    {
        if(index >= 0 && index < m_nouts)
            return m_outputs_nodes[index].size();
        else
            return 0;
    }
    
    bool DspNode::isInplace() const noexcept
    {
        return m_inplace;
    }
    
    void DspNode::setInplace(bool status)
    {
        m_inplace = status;
    }
    
    bool DspNode::isValid() const noexcept
    {
        return m_valid;
    }
    
    shared_ptr<Signal> DspNode::getOutputSignal(shared_ptr<DspNode> inputnode)
    {
        int size = (int)m_outputs_nodes.size();
        for(int i = 0; i < size; i++)
        {
            if(m_outputs_nodes[i].find(inputnode) != m_outputs_nodes[i].end())
            {
                return m_sig_outs[i];
            }
        }
        return nullptr;
    }
    
    void DspNode::clean()
    {
        set<weak_ptr<DspNode>>::iterator it;
        int size = (int)m_inputs_nodes.size();
        for(int i = 0; i < size; i++)
        {
            for(it = m_inputs_nodes[i].begin(); it != m_inputs_nodes[i].end(); )
            {
                shared_ptr<DspNode> node = (*it).lock();
                // If the node has already been removed
                if(!node)
                {
                    m_inputs_nodes[i].erase(it++);
                }
                // If the node isn't valid (it should has been already removed but... for secure)
                else if(!node->isValid())
                {
                    m_inputs_nodes[i].erase(it++);
                }
                // If the node sample rate or vector size are wrong
                else if((node->m_samplerate != m_samplerate) || (node->m_vectorsize != m_vectorsize))
                {
                    throw node;
                    return;
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    
    void DspNode::allocSignals(shared_ptr<DspContext> context)
    {
        set<weak_ptr<DspNode>>::iterator it;
        
        // Inputs signals
        for(int i = 0; i < m_nins; i++)
        {
            // We try to find a signal not borrowed
            for(it = m_inputs_nodes[i].begin(); it != m_inputs_nodes[i].end() && !m_sig_ins[i]; ++it)
            {
                shared_ptr<DspNode> node = (*it).lock();
                if(node)
                {
                    shared_ptr<Signal> sig = node->getOutputSignal(shared_from_this());
                    if(sig && !sig->isBorrowed()) // Can do this more if no inplace
                    {
                        m_sig_ins[i] = make_shared<Signal>(sig, m_inplace);
                        break;
                    }
                }
            }
            
            // If we didn't find one
            if(!m_sig_ins[i])
            {
                shared_ptr<Signal> sig = make_shared<Signal>(m_vectorsize);
                m_sig_ins[i] = sig;
                context->addSignal(sig);
            }
            
            // We add copy
            for(it = m_inputs_nodes[i].begin(); it != m_inputs_nodes[i].end(); ++it)
            {
                shared_ptr<DspNode> node = (*it).lock();
                if(node)
                {
                    shared_ptr<Signal> sig = node->getOutputSignal(shared_from_this());
                    if(sig && (*(sig.get()) != *(m_sig_ins[i].get())))
                    {
                        shared_ptr<DspProcess> process = make_shared<DspProcess>(DspProcess::copy, nullptr, 1, 1, m_vectorsize);
                        process->setInput(0, sig);
                        process->setOutput(0, m_sig_ins[i]);
                        context->addProcess(process);
                    }
                }
            }
            
         }
        
        // Outputs signals
        if(m_inplace)
        {
            for(int i = 0; i < m_nouts; i++)
            {
                if(i < m_nins)
                {
                    m_sig_outs[i] = make_shared<Signal>(m_sig_ins[i], false);
                }
                else
                {
                    shared_ptr<Signal> sig = make_shared<Signal>(m_vectorsize);
                    m_sig_outs[i] = sig;
                    context->addSignal(sig);
                }
            }
        }
        else
        {
            for(int i = 0; i < m_nouts; i++)
            {
                shared_ptr<Signal> sig = make_shared<Signal>(m_vectorsize);
                m_sig_outs[i] = sig;
                context->addSignal(sig);
            }
        }
    }
    
    void DspNode::prepare(shared_ptr<DspContext> context)
    {
        m_valid         = false;
        m_samplerate    = context->getSamplerate();
        m_vectorsize    = context->getVectorsize();
        
        m_processes.clear();
        m_sig_ins.clear();
        m_sig_outs.clear();
        m_sig_ins.resize(m_nins);
        m_sig_outs.resize(m_nouts);
        
        clean();
        /*
        MethodDsp dspmethod = (MethodDsp)m_box->getMethod("dsp");
        if(dspmethod)
            dspmethod(m_box, shared_from_this());
        
        if(m_processes.size())
        {
            allocSignals(context);
            for(int i = 0; i < m_processes.size(); i++)
            {
                shared_ptr<DspProcess> process = make_shared<DspProcess>(m_processes[i], m_box, m_nins, m_nouts, m_vectorsize);
                process->setInputs(m_sig_ins);
                process->setOutputs(m_sig_outs);
                context->addProcess(process);
            }
            m_valid = true;
        }*/
    }
}
















