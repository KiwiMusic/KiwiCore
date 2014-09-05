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

#ifndef __DEF_KIWI_CONSOLE__
#define __DEF_KIWI_CONSOLE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"

// TODO a standard Console listener that write in a simple text file the history of the messages.

namespace Kiwi
{
    class Object;
    class Console;
    
    // ================================================================================ //
    //                                  CONSOLE LISTENER                                //
    // ================================================================================ //
    
    //! The console listener is a virtual class that can bind itself to a console and be notified of the sevreal messages.
    /**
     The console is a very light class with three methods that can receive the post, warning and error messages with the pointer of the object sender if it exists.
     @see Console
     */
    class ConsoleListener
    {
    public:
        ConsoleListener(){};
        virtual ~ConsoleListener(){};
        
        //! Receive the post messages.
        /** The function is called by the console when a message has been post.
         @param console The console.
         @param object  The object sender.
         @param message The message in the string format.
         */
        virtual void post(shared_ptr<const Console> console, const shared_ptr<const Object> object, string message) {};
        
        //! Receive the warning messages.
        /** The function is called by the console when a warning has been post.
         @param console The console.
         @param object  The object sender.
         @param message The warning in the string format.
         */
        virtual void warning(shared_ptr<const Console> console, const shared_ptr<const Object> object, string message) {};
        
        //! Receive the error messages.
        /** The function is called by the console when a error has been post.
         @param console The console.
         @param object  The object sender.
         @param message The error in the string format.
         */
        virtual void error(shared_ptr<const Console> console, const shared_ptr<const Object> object, string message) {};
    };
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    //! The console is an interface that receives messages and dispatches them to console listeners.
    /**
     The console is created with a kiwi instance then retrieve it and bind a console listener to get the posts, warnings and errors of the objects.
     @see ConsoleListener
     */
    class Console : public enable_shared_from_this<Console>
    {
    private:
        set<weak_ptr<ConsoleListener>, owner_less<weak_ptr<ConsoleListener>>> m_listeners;
    public:
        
        //! The constructor.
        /** You should have the necessity to use this method.
         */
        Console();
        
        //! The destructor.
        /** You should have the necessity to use this method.
         */
        ~Console();
        
        //! Add an console listener in the binding list of the console.
        /** The function adds an console listener in the binding list of the console. If the console listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the console listener.
         @see              unbind()
         */
        void bind(weak_ptr<ConsoleListener> listener);
        
        //! Remove an console listener from the binding list of the console.
        /** The function removes an console listener from the binding list of the console. If the console listener isn't in the binding list, the function doesn't do anything.
         @param object  The pointer of the console listener.
         @see           bind()
         */
        void unbind(weak_ptr<ConsoleListener> listener);
        
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
}


#endif


