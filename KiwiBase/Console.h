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

#include "Tools.h"

namespace Kiwi
{
    class Box;
    class Page;

    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    //! The console is an interface that receives messages and dispatches them to console listeners.
    /**
     The console is created with a kiwi instance then retrieve it and bind a console listener to get the posts, warnings and errors.
     @see ConsoleListener
     */
    class Console : public enable_shared_from_this<Console>
    {
    public:
        class Listener;
    private:
        
        static unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>> m_listeners;
    public:
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        //! Add an console listener in the binding list of the console.
        /** The function adds an console listener in the binding list of the console. If the console listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the console listener.
         @see              unbind()
         */
        static void bind(weak_ptr<Listener> listener);
        
        //! Remove an console listener from the binding list of the console.
        /** The function removes an console listener from the binding list of the console. If the console listener isn't in the binding list, the function doesn't do anything.
         @param box  The pointer of the console listener.
         @see           bind()
         */
        static void unbind(weak_ptr<Listener> listener);
        
        //! Post a standard message.
        /** The function post a message and notify the console listeners that a message has been received.
         @param message  The message in the string format.
         */
        static void post(string const& message) noexcept;
        
        //! Post a standard message that refers to a specific box.
        /** The function post a message and notify the console listeners that a message has been received from this box.
         @param box  The box sender.
         @param message The message in the string format.
         */
        static void post(const shared_ptr<const Box> box, string const& message) noexcept;
        
        //! Post a warning.
        /** The function post a warning and notify the console listeners that a warning has been received.
         @param message  The warning in the string format.
         */
        static void warning(string const& message) noexcept;
        
        //! Post a warning that refers to a specific box.
        /** The function post a warning and notify the console listeners that a warning has been received from this box.
         @param box  The box sender.
         @param message The warning in the string format.
         */
        static void warning(const shared_ptr<const Box> box, string const& message) noexcept;
        
        //! Post an error.
        /** The function post a error and notify the console listeners that a error has been received.
         @param message  The error in the string format.
         */
        static void error(string const& message) noexcept;
        
        //! Post an error that refers to a specific box.
        /** The function post a error and notify the console listeners that a error has been received from this box.
         @param box  The box sender.
         @param message The error in the string format.
         */
        static void error(const shared_ptr<const Box> box, string const& message) noexcept;
        
        // ================================================================================ //
        //                                  INSTANCE LISTENER                               //
        // ================================================================================ //
        
        //! The console listener is a virtual class that can bind itself to a console and be notified of the sevreal messages.
        /**
         The console is a very light class with three methods that can receive the post, warning and error messages with the pointer of the box sender if it exists.
         @see Instance
         */
        class Listener
        {
        public:
            
            //! The constructor.
            /** The constructor does nothing.
             */
            Listener()
            {
                ;
            }
            
            //! The destructor.
            /** The destructor does nothing.
             */
            virtual ~Listener()
            {
                ;
            }
            
            //! Receive the post messages.
            /** The function is called by the console when a message has been post.
             @param page    The page.
             @param box     The box.
             @param message The message in the string format.
             */
            virtual void post(shared_ptr<const Page> page, const shared_ptr<const Box> box, string const& message) {};
            
            //! Receive the warning messages.
            /** The function is called by the console when a warning has been post.
             @param page    The page.
             @param box     The box.
             @param message The warning in the string format.
             */
            virtual void warning(shared_ptr<const Page> page, const shared_ptr<const Box> box, string const& message) {};
            
            //! Receive the error messages.
            /** The function is called by the console when a error has been post.
             @param page    The page.
             @param box     The box.
             @param message The error in the string format.
             */
            virtual void error(shared_ptr<const Page> page, const shared_ptr<const Box> box, string const& message) {};
        };
    };    
};


#endif


