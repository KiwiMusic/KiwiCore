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

#include "Box.h"
#include "Iolet.h"
#include "Page.h"

namespace Kiwi
{
    Box::Box(shared_ptr<Instance> kiwi, const shared_ptr<Tag> name) : ObjectExtented(kiwi, name), m_last_inlet(0)
    {
        createAttribute(createTag("position"), T_LONG, 2);
        setAttributeAppearance(createTag("Position"), createTag("list"), createTag("Appearance"));
        setAttributeBehavior(false, true, true);
        
        createAttribute(createTag("size"), T_LONG, 2);
        setAttributeAppearance(createTag("Size"), createTag("list"), createTag("Appearance"));
        setAttributeBehavior(false, true, true);
        
        createAttribute(createTag("fontname"), T_TAG, 1);
        createAttribute(createTag("fontsize"), T_LONG, 1);
        createAttribute(createTag("fontstyle"), T_LONG, 1);
        createAttribute(createTag("justification"), T_TAG, 1);
    }
    
    Box::Box(shared_ptr<Instance> kiwi, const string name) : ObjectExtented(kiwi, name), m_last_inlet(0)
    {
        createAttribute(createTag("position"), T_LONG, 2);
        setAttributeAppearance(createTag("Position"), createTag("list"), createTag("Appearance"));
        setAttributeBehavior(false, true, true);
        
        createAttribute(createTag("size"), T_LONG, 2);
        setAttributeAppearance(createTag("Size"), createTag("list"), createTag("Appearance"));
        setAttributeBehavior(false, true, true);
        
        createAttribute(createTag("fontname"), T_TAG, 1);
        createAttribute(createTag("fontsize"), T_LONG, 1);
        createAttribute(createTag("fontstyle"), T_LONG, 1);
        createAttribute(createTag("justification"), T_TAG, 1);
    }
    
    Box::~Box()
    {
        m_outlets.clear();
        m_inlets.clear();
    }
    
    bool Box::compatible(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from->getNumberOfOutlets() > outlet && to->getNumberOfInlets() > inlet)
        {
            return from->m_outlets[outlet]->compatible(to->m_inlets[inlet]);
        }
        else
            return false;
    }
    
    bool Box::connect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from->getNumberOfOutlets() > outlet && to->getNumberOfInlets() > inlet)
        {
            return from->m_outlets[outlet]->connect(to->m_inlets[inlet]);
        }
        else
            return false;
    }
    
    void Box::disconnect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet)
    {
        if(from->getNumberOfOutlets() > outlet && to->getNumberOfInlets() > inlet)
        {
            return from->m_outlets[outlet]->disconnect(to->m_inlets[inlet]);
        }
    }
    
    void Box::addInlet(int size, ...)
    {
        int index = (int)m_inlets.size();
        m_inlets.push_back(make_shared<Inlet>(shared_from_this(),index));
        va_list argp;
        va_start(argp, size);
        for(int i = 0; i < size; i++)
        {
             m_inlets[index]->m_names.insert(createTag(va_arg(argp, char*)));
        }
        va_end(argp);
    }
    
    void Box::addInlet(string name1, string name2, string name3, string name4, string name5, string name6, string name7, string name8, string name9)
    {
        int index = (int)m_inlets.size();
        m_inlets.push_back(make_shared<Inlet>(shared_from_this(),index));
        m_inlets[index]->m_names.insert(createTag(name1));
        if(!name2.empty())
            m_inlets[index]->m_names.insert(createTag(name2));
        if(!name3.empty())
            m_inlets[index]->m_names.insert(createTag(name3));
        if(!name4.empty())
            m_inlets[index]->m_names.insert(createTag(name4));
        if(!name5.empty())
            m_inlets[index]->m_names.insert(createTag(name5));
        if(!name6.empty())
            m_inlets[index]->m_names.insert(createTag(name6));
        if(!name7.empty())
            m_inlets[index]->m_names.insert(createTag(name7));
        if(!name8.empty())
            m_inlets[index]->m_names.insert(createTag(name8));
        if(!name9.empty())
            m_inlets[index]->m_names.insert(createTag(name9));
    }
    
    void Box::setInletCallName(int index, int size, ...)
    {
        assert(index >= 0 && index < m_inlets.size());
        va_list argp;
        va_start(argp, size);
        for(int i = 0; i < size; i++)
        {
            m_inlets[index]->m_names.insert(createTag(va_arg(argp, char*)));
        }
        va_end(argp);
    }
    
    void Box::setInletDescription(int index, std::string description)
    {
        assert(index >= 0 && index < m_inlets.size());
        m_inlets[index]->m_description = description;
    }
    
    void Box::setInletPolarity(int index, bool polarity)
    {
        assert(index >= 0 && index < m_inlets.size());
        m_inlets[index]->m_polarity = polarity;
    }
    
    void  Box::setInletAttributes(int index, string description, bool polarity)
    {
        assert(index >= 0 && index < m_inlets.size());
        m_inlets[index]->m_description = description;
        m_inlets[index]->m_polarity = polarity;
    }
    
    void Box::removeInlet(int index)
    {
        assert(index >= 0 && index < m_inlets.size());
        shared_ptr<Page> page = m_page.lock();
        if(page)
            page->disconnect(nullptr, 0, static_pointer_cast<Box>(shared_from_this()), index);
        
        for(int i = index+1; i < m_inlets.size(); i++)
            m_inlets[i]->m_index--;
        m_inlets.erase(m_inlets.begin()+index);
    }
    
    void Box::addOutlet(int size, ...)
    {
        int index = (int)m_outlets.size();
        m_outlets.push_back(make_shared<Outlet>(shared_from_this(), m_outlets.size()));
        va_list argp;
        va_start(argp, size);
        for(int i = 0; i < size; i++)
        {
            m_outlets[index]->m_names.insert(createTag(va_arg(argp, char*)));
        }
        va_end(argp);
    }
    
    void Box::addOutlet(string name1, string name2, string name3, string name4, string name5, string name6, string name7, string name8, string name9)
    {
        int index = (int)m_outlets.size();
        m_outlets.push_back(make_shared<Outlet>(shared_from_this(),index));
        m_outlets[index]->m_names.insert(createTag(name1));
        if(!name2.empty())
            m_outlets[index]->m_names.insert(createTag(name2));
        if(!name3.empty())
            m_outlets[index]->m_names.insert(createTag(name3));
        if(!name4.empty())
            m_outlets[index]->m_names.insert(createTag(name4));
        if(!name5.empty())
            m_outlets[index]->m_names.insert(createTag(name5));
        if(!name6.empty())
            m_outlets[index]->m_names.insert(createTag(name6));
        if(!name7.empty())
            m_outlets[index]->m_names.insert(createTag(name7));
        if(!name8.empty())
            m_outlets[index]->m_names.insert(createTag(name8));
        if(!name9.empty())
            m_outlets[index]->m_names.insert(createTag(name9));
    }
    
    void Box::setOutletCallName(int index, int size, ...)
    {
        assert(index >= 0 && index < m_outlets.size());
        va_list argp;
        va_start(argp, size);
        for(int i = 0; i < size; i++)
        {
            m_outlets[index]->m_names.insert(createTag(va_arg(argp, char*)));
        }
        va_end(argp);
    }
    
    void Box::setOutletDescription(int index, string description)
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->m_description = description;
    }
    
    void Box::removeOutlet(int index)
    {
        assert(index >= 0 && index < m_outlets.size());
        shared_ptr<Page> page = m_page.lock();
        if(page)
            page->disconnect(static_pointer_cast<Box>(shared_from_this()), index, nullptr, 0);
        
        for(int i = index+1; i < m_outlets.size(); i++)
            m_outlets[i]->m_index--;
        m_outlets.erase(m_outlets.begin()+index);
    }
    
    void Box::sendBang(int index) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendBang();
    }
    
    void Box::sendLong(int index, long value) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendLong(value);
    }
    
    void Box::sendDouble(int index, double value) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendDouble(value);
    }
    
    void Box::sendTag(int index, shared_ptr<Tag> tag) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendTag(tag);
    }
    
    void Box::sendElements(int index, vector<Element>& elements) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendElements(elements);
    }
    
    void Box::sendOther(int index, shared_ptr<Tag> name, long value) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendOther(name, value);
    }
    
    void Box::sendOther(int index, shared_ptr<Tag> name, double value) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendOther(name, value);
    }
    
    void Box::sendOther(int index, shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendOther(name, value);
    }
    
    void Box::sendOther(int index, shared_ptr<Tag> name, vector<Element>& elements) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        m_outlets[index]->sendOther(name, elements);
    }
    
    bool Box::isDsp()
    {
        return hasMethod("dsp");
    }
    
    int Box::getNumberOfInlets() const noexcept
    {
        return (int)m_inlets.size();
    }
    
    int Box::getNumberOfOutlets() const noexcept
    {
        return (int)m_outlets.size();
    }
    
    int Box::getNumberOfSignalInlets() noexcept
    {
        int ninlets = 0;
        for(vector<shared_ptr<Inlet>>::iterator it = m_inlets.begin(); it != m_inlets.end(); ++it)
        {
            if((*it)->m_names.find(m_tag_signal) != (*it)->m_names.end())
                ninlets++;
        }
        return ninlets;
    }
    
    int Box::getNumberOfSignalOutlets() noexcept
    {
        int noutlets = 0;
        for(vector<shared_ptr<Outlet>>::iterator it = m_outlets.begin(); it != m_outlets.end(); ++it)
        {
            if((*it)->m_names.find(m_tag_signal) != (*it)->m_names.end())
                noutlets++;
        }
        return noutlets;
    }
    
    string Box::getInletDescription(int index) const noexcept
    {
        assert(index >= 0 && index < m_inlets.size());
        return m_inlets[index]->m_description;
    }
    
    string Box::getOutletDescription(int index) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        return m_outlets[index]->m_description;
    }
    
    int Box::getInletSignalIndex(int index) const noexcept
    {
        assert(index >= 0 && index < m_inlets.size());
        int sig_index = 0;
        for(int i = 0; i < index; i++)
        {
            if(isInletSignal(i))
                sig_index++;
        }
        return sig_index;
    }
    
    int Box::getOutletSignalIndex(int index) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        int sig_index = 0;
        for(int i = 0; i < index; i++)
        {
            if(isOutletSignal(i))
                sig_index++;
        }
        return sig_index;
    }
    
    bool Box::isInletSignal(int index) const noexcept
    {
        assert(index >= 0 && index < m_inlets.size());
        return m_inlets[index]->m_names.find(m_tag_signal) != m_inlets[index]->m_names.end();
    }
    
    bool Box::isOutletSignal(int index) const noexcept
    {
        assert(index >= 0 && index < m_outlets.size());
        return m_outlets[index]->m_names.find(m_tag_signal) != m_outlets[index]->m_names.end();
    }
}
















