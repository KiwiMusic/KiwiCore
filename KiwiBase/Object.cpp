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

#include "Object.h"
#include "Instance.h"

namespace Kiwi
{
    sTag tagName = make_shared<Tag>("name");
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    Object::Object(shared_ptr<Instance> kiwi, string const& name) noexcept :
    m_kiwi(kiwi),
    m_name(kiwi->createTag(name))
    {
        
    }
    
    Object::~Object()
    {
        ;
    }
    
    sTag Object::name() const noexcept
    {
        return m_name;
    }
    
    Object::Type Object::type() const noexcept
    {
        return OBJECT;
    }
    
    shared_ptr<Object> Object::create(shared_ptr<Dico> dico) const
    {
        return makeObject<Object>("");
    }
    
    void Object::receive(ElemVector const& elements)
    {
        switch(elements.size())
        {
            case 0:
                error(shared_from_this(), "isn't expecting nothing.");
                break;
                
            case 1:
                error(shared_from_this(), string("isn't expecting " + toString(elements[0].type()) + "."));
                break;
                
            default:
                error(shared_from_this(), string("isn't expecting elements."));
                break;
        }
    }
    
    void Object::write(shared_ptr<Dico> dico) const noexcept
    {
        dico->append(tagName, m_name);
    }
    
    void Object::read(shared_ptr<const Dico> dico) noexcept
    {
        ;
    }
    
    void Object::wrong(sTag const& name) const noexcept
    {
        error(shared_from_this(), string("isn't expecting " + toString(name) + "."));
    }
    
    void Object::wrong(sTag const& name, Element::Type type, ElemVector const& elements) const noexcept
    {
        error(shared_from_this(), "is expecting " + toString(type) + " for the message " + toString(name) + " but received " + toString(elements) + ".");
    }
    
    sTag Object::createTag(string const& name) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createTag(name);
        else
            return nullptr;
    }
    
    sObject Object::createObject(sDico dico) const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createObject(dico);
        else
            return nullptr;
    }
    
    sDico Object::createDico() const
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->createDico();
        else
            return nullptr;
    }
    
    // ================================================================================ //
    //                                      MESSAGES                                    //
    // ================================================================================ //
    
    void Object::post(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->post(message);
            }
    
    void Object::warning(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->warning(message);
            }
    
    void Object::error(string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->error(message);
            }
    
    void Object::post(shared_ptr<const Object> object, string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->post(object, message);
    }
    
    void Object::warning(shared_ptr<const Object> object, string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->warning(object, message);
    }
    
    void Object::error(shared_ptr<const Object> object, string message) const noexcept
    {
        shared_ptr<Instance> kiwi = m_kiwi.lock();
        if(kiwi)
            return kiwi->error(object, message);
    }
}


















