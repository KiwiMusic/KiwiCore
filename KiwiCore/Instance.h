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

#ifndef __DEF_KIWI_INSTANCE__
#define __DEF_KIWI_INSTANCE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"
#include "Dico.h"
#include "Iolet.h"
#include "Box.h"
#include "Page.h"

namespace Kiwi
{
    class InstanceListener;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    class Instance :  public TagFactory, public enable_shared_from_this<Instance>
    {
    private:
        
        int                                                                     m_untitled_pages;
        vector<shared_ptr<Page>>                                                m_pages;
        map<shared_ptr<Tag>, unique_ptr<Object>>                                m_prototypes;
        set<weak_ptr<InstanceListener>, owner_less<weak_ptr<InstanceListener>>> m_listeners;
        
    public:
        Instance();
        ~Instance();
        
        void init();
        
        shared_ptr<Object> createObject(string name, vector<Element> const& elements);
        shared_ptr<Object> createObject(shared_ptr<Tag> name, vector<Element> const& elements);
        shared_ptr<Object> createObject(string name, Element const& element);
        shared_ptr<Object> createObject(shared_ptr<Tag> name, Element const& element);
        shared_ptr<Object> createObject(string name);
        shared_ptr<Object> createObject(shared_ptr<Tag> name);
        
        shared_ptr<Connection> createConnection(shared_ptr<Box> from, int oulet, shared_ptr<Box> to, int inlet);
        shared_ptr<Dico> createDico();
        
        
        shared_ptr<Page> createPage(string file = string(""), string directory = string(""));
        void closePage(shared_ptr<Page> page);
        void addObjectPrototype(unique_ptr<Object>);
        
        void startDsp(double samplerate, long vectorsize);
        
        void tickDsp();
        
        void stopDsp();
        
        //! Add an console listener in the binding list of the console.
        /** The function adds an console listener in the binding list of the console. If the console listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the console listener.
         @see              unbind()
         */
        void bind(weak_ptr<InstanceListener> listener);
        
        //! Remove an console listener from the binding list of the console.
        /** The function removes an console listener from the binding list of the console. If the console listener isn't in the binding list, the function doesn't do anything.
         @param object  The pointer of the console listener.
         @see           bind()
         */
        void unbind(weak_ptr<InstanceListener> listener);
        
        //! Post a standard message.
        /** The function post a message and notify the console listeners that a message has been received.
         @param message  The message in the string format.
         */
        void post(string message) const noexcept;
        
        //! Post a standard message that refers to a specific object.
        /** The function post a message and notify the console listeners that a message has been received from this object.
         @param object  The object sender.
         @param message The message in the string format.
         */
        void post(const shared_ptr<const Object> object, string message) const noexcept;
        
        //! Post a warning.
        /** The function post a warning and notify the console listeners that a warning has been received.
         @param message  The warning in the string format.
         */
        void warning(string message) const noexcept;
        
        //! Post a warning that refers to a specific object.
        /** The function post a warning and notify the console listeners that a warning has been received from this object.
         @param object  The object sender.
         @param message The warning in the string format.
         */
        void warning(const shared_ptr<const Object> object, string message) const noexcept;
        
        //! Post an error.
        /** The function post a error and notify the console listeners that a error has been received.
         @param message  The error in the string format.
         */
        void error(string message) const noexcept;
        
        //! Post an error that refers to a specific object.
        /** The function post a error and notify the console listeners that a error has been received from this object.
         @param object  The object sender.
         @param message The error in the string format.
         */
        void error(const shared_ptr<const Object> object, string message) const noexcept;
    };
    
    
    // ================================================================================ //
    //                                  INSTANCE LISTENER                               //
    // ================================================================================ //
    
    //! The instance listener is a virtual class that can bind itself to a instance and be notified of the sevreal messages.
    /**
     The instance is a very light class with three methods that can receive the post, warning and error messages with the pointer of the object sender if it exists.
     @see Instance
     */
    class InstanceListener
    {
    public:
        InstanceListener(){};
        virtual ~InstanceListener(){};
        
        //! Receive the post messages.
        /** The function is called by the instance when a message has been post.
         @param kiwi    The instance.
         @param object  The object sender.
         @param message The message in the string format.
         */
        virtual void post(shared_ptr<const Instance> kiwi, const shared_ptr<const Object> object, string message) {};
        
        //! Receive the warning messages.
        /** The function is called by the instance when a warning has been post.
         @param kiwi    The instance.
         @param object  The object sender.
         @param message The warning in the string format.
         */
        virtual void warning(shared_ptr<const Instance> kiwi, const shared_ptr<const Object> object, string message) {};
        
        //! Receive the error messages.
        /** The function is called by the instance when a error has been post.
         @param kiwi    The instance.
         @param object  The object sender.
         @param message The error in the string format.
         */
        virtual void error(shared_ptr<const Instance> kiwi, const shared_ptr<const Object> object, string message) {};
    };
}


#endif


