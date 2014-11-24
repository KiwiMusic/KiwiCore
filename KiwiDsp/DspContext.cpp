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

#include "DspContext.h"
#include "../KiwiBase/Box.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    DspContext::DspContext()
    {
        ;
    }
    
    DspContext::~DspContext()
    {
        clear();
    }
    
    void DspContext::setSamplerate(double samplerate)
    {
        m_samplerate = samplerate;
    }
    
    void DspContext::setVectorsize(long vectorsize)
    {
        m_vectorsize = vectorsize;
    }
    
    double DspContext::getSamplerate() const noexcept
    {
        return m_samplerate;
    }
    
    long DspContext::getVectorsize() const noexcept
    {
        return m_vectorsize;
    }
    
    size_t DspContext::getNumberOfProcess() const noexcept
    {
        return m_processes.size();
    }
    
    void DspContext::addBox(sBox box)
    {
        /*
        if(box->type() == Box::DSPBOX)
        {
            m_nodes.push_back(make_shared<DspNode>(box));
        }
         */
    }
    
    void DspContext::removeBox(sBox box)
    {
        int size = (int)m_nodes.size();
        for(int i = 0; i < size; i++)
        {
            if(m_nodes[i]->m_box == box)
            {
                m_nodes.erase(m_nodes.begin()+i);
                return;
            }
        }
    }
    
    void DspContext::addConnection(sLink link)
    {
        int size = (int)m_nodes.size();
        shared_ptr<DspNode> nodeFrom, nodeTo;
        /*
        sBox boxFrom = link->getFrom();
        sBox boxTo = link->getTo();
        
        if(nodeFrom && nodeTo)
        {
            for(int i = 0; i < size; i++)
            {
                if(m_nodes[i]->m_box == boxFrom)
                    nodeFrom = m_nodes[i];
                else if(m_nodes[i]->m_box == boxTo)
                    nodeTo = m_nodes[i];
                
                nodeFrom->addOutput(nodeTo, boxFrom->getOutletSignalIndex(link->getOutletIndex()));
                nodeTo->addInput(nodeFrom, boxTo->getInletSignalIndex(link->getInletIndex()));
                return;
            }
        }*/
        
    }
    
    void DspContext::removeLink(sLink link)
    {
        /*
        if(link->isDsp())
        {
            int size = (int)m_nodes.size();
            shared_ptr<DspNode> nodeFrom, nodeTo;
            sBox boxFrom = link->getFrom();
            sBox boxTo = link->getTo();
            
            for(int i = 0; i < size; i++)
            {
                if(m_nodes[i]->m_box == boxFrom)
                    nodeFrom = m_nodes[i];
                else if(m_nodes[i]->m_box == boxTo)
                    nodeTo = m_nodes[i];
                
                if(nodeFrom && nodeTo)
                {
                    nodeFrom->removeOutput(nodeTo, link->getOutletIndex());
                    nodeTo->removeInput(nodeFrom, link->getInletIndex());
                    return;
                }
            }
        }*/
    }
    
    void DspContext::addProcess(shared_ptr<DspProcess> process)
    {
        m_processes.push_back(process);
    }
    
    void DspContext::addSignal(shared_ptr<Signal> signal)
    {
        m_signals.push_back(signal);
    }
    
    void DspContext::indexNode(shared_ptr<DspNode> node)
    {
        if(node->m_index >= 0)
            return;
        
        int ninputs = node->getNumberOfInputs();
        set<weak_ptr<DspNode>>::iterator it;
        
        m_nodes_temp.insert(node);
        
        for(int i = 0; i < ninputs; i++)
        {
            for(it = node->m_inputs_nodes[i].begin(); it != node->m_inputs_nodes[i].end(); ++it)
            {
                shared_ptr<DspNode> pnode = (*it).lock();
                if(pnode && pnode->m_index < 0)
                {
                    if(m_nodes_temp.find(pnode) != m_nodes_temp.end())
                    {
                        throw pnode;
                    }
                    else
                    {
                        indexNode(pnode);
                    }
                }
            }
        }
        node->m_index = m_index++;
        m_nodes_temp.erase(node);
    }
    
    bool DspContext::compareNode(shared_ptr<DspNode> node1, shared_ptr<DspNode> node2)
    {
        return node1->m_index < node2->m_index;
    }
    
    void DspContext::compile()
    {
        m_index = 0;
        int size = (int)m_nodes.size();
        m_nodes_temp.clear();
        m_processes.clear();
        m_signals.clear();
        
        // Sort the nodes
        try
        {
            for(int i = 0; i < size; i++)
            {
                indexNode(m_nodes[i]);
            }
            sort(m_nodes.begin(), m_nodes.end(), compareNode);
        }
        catch(shared_ptr<DspNode> node)
        {
            throw node->m_box;
        }
        
        // Compile the nodes
        try
        {
            for(int i = 0; i < size; i++)
            {
                m_nodes[i]->prepare(shared_from_this());
                if(!m_nodes[i]->isValid())
                {
                    m_nodes.erase(m_nodes.begin()+i);
                    i--;
                    size--;
                }
            }
        }
        catch(shared_ptr<DspNode> node)
        {
            throw node->m_box;
        }
        
        m_nodes.clear();
        m_nodes_temp.clear();
    }
    
    void DspContext::tick()
    {
        const int size = (int)m_processes.size();
        for(int i = 0; i < size; i++)
            m_processes[i]->tick();
    }
    
    void DspContext::clear()
    {
        m_nodes.clear();
        m_nodes_temp.clear();
        m_processes.clear();
        m_signals.clear();
    }
}
















