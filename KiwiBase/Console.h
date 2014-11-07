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
    class Instance;
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    //! The console is an interface that receives messages and dispatches them to console listeners.
    /**
     The console is created with a kiwi instance then retrieve it and bind a console listener to get the posts, warnings and errors.
     @see Console::Message
     @see Console::Listener
     @see Console::History
     */
    class Console : public enable_shared_from_this<Console>
    {
    public:
        class Listener;
        class Message;
            
    private:
        
        static unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>> m_listeners;
        static mutex              m_mutex;
    public:
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        //! Add an console listener in the binding list of the console.
        /** The function adds an console listener in the binding list of the console. If the console listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the console listener.
         @see              unbind()
         */
        static void bind(shared_ptr<Listener> listener);
        
        //! Remove an console listener from the binding list of the console.
        /** The function removes an console listener from the binding list of the console. If the console listener isn't in the binding list, the function doesn't do anything.
         @param box  The pointer of the console listener.
         @see           bind()
         */
        static void unbind(shared_ptr<Listener> listener);
        
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
         The console listener is a very light class with three methods that can receive the post, warning and error messages notifications from consoles.
         @see Console
         @see Console::Message
         @see Console::History
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
            
            //! Receive the messages.
            /** The function is called by the console when a message has been received.
             @param console The console that received the message.
             @param message The message.
             */
            virtual void receive(shared_ptr<const Message> message)
            {
                ;
            }
        };
        
        // ================================================================================ //
        //                                  CONSOLE HISTORY                                 //
        // ================================================================================ //
        
        //! The console history is a console listener that keeps an history of the messages.
        /**
         The console history keeps and retrieves an history of messages. It offers functions to sort the messages by date, message, name of the sender an type.
         @see Console
         @see Console::Listener
         @see Console::Message
         */
        class History : public Console::Listener, public enable_shared_from_this<History>
        {
        public:
            class Listener;
        
            enum Sort
            {
                Index   = 0,
                Name    = 1,
                Kind    = 2,
                Content = 3
            };
        private:
            struct MessageHolder
            {
                shared_ptr<const Console::Message> m_message;
                size_t                             m_index;
            };
            mutex                  m_mutex;
            vector<MessageHolder>  m_messages;
            unordered_set<weak_ptr<Listener>,
            weak_ptr_hash<Listener>,
            weak_ptr_equal<Listener>> m_listeners;
            
            static bool compareIndex(MessageHolder const& i, MessageHolder const& j);
            static bool compareName(MessageHolder const& i, MessageHolder const& j);
            static bool compareContent(MessageHolder const& i, MessageHolder const& j);
            static bool compareKind(MessageHolder const& i, MessageHolder const& j);
            
            //! Receive the messages.
            /** The function is called by the console when a message has been received.
             @param console The console that received the message.
             @param message The message.
             */
            void receive(shared_ptr<const Console::Message> message) override;
        public:
            
            //! The constructor.
            /** The constructor bind itself to the console.
             */
            History();
            
            //! The destructor.
            /** The destructor clear all the messages.
             */
            ~History();
            
            //! The console history creation method.
            /** The function allocates a console history and bind it to the console.
             */
            static shared_ptr<History> create();
        
            //! Clear the messages.
            /** The function clears the history of messages.
             */
            void clear();
            
            //! Retreives the number of messages in the history.
            /** The function retreives the number of messages in the history.
             @return The number of messages.
             */
            size_t size();
            
            //! Retreives a message from the history.
            /** The function retreives a message from the history.
             @param index The index of the message.
             @return The message.
             */
            shared_ptr<const Console::Message> get(size_t index);
            
            //! Erase a message from the history.
            /** The function erases a message from the history.
             @param index The index of the message.
             */
            void erase(size_t index);
            
            //! Erase a range of messages from the history.
            /** The function a range of messages from the history.
             @param begin The index of the message of the first message.
             @param last The index of the message of the last message.
             */
            void erase(size_t begin, size_t last);
            
            //! Sort the message.
            /** The function sorts the message by index, name, kind or content.
             @param sort The type of sorting.
             */
            void sort(Sort type = Index);
            
            //! Add an history listener in the binding list of the console history.
            /** The function adds an history listener in the binding list of the console history. If the history listener is already in the binding list, the function doesn't do anything.
             @param listener  The pointer of the history listener.
             @see              unbind()
             */
            void bind(shared_ptr<Listener> listener);
            
            //! Remove an history listener from the binding list of the console history.
            /** The function removes an history listener from the binding list of the console history. If the history listener isn't in the binding list, the function doesn't do anything.
             @param box  The pointer of the history listener.
             @see           bind()
             */
            void unbind(shared_ptr<Listener> listener);
            
            // ================================================================================ //
            //                                  INSTANCE LISTENER                               //
            // ================================================================================ //
            
            //! The console listener is a virtual class that can bind itself to a console and be notified of the sevreal messages.
            /**
             The console listener is a very light class with three methods that can receive the post, warning and error messages notifications from consoles.
             @see Console
             @see Console::Message
             @see Console::History
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
                
                //! Receive the notification that an history has changed.
                /** The function is called by an hisotry when it has changed.
                 @param history The console history.
                 */
                virtual void historyHasChanged(shared_ptr<History> history)
                {
                    ;
                }
            };
        };
        
        // ================================================================================ //
        //                                  CONSOLE MESSAGE                                 //
        // ================================================================================ //
        
        //! The console message owns all the informations of a message posted via a console.
        /**
         The console message is used to send the content, the type and the sender of a message posted via a console to the console listeners.
         @see Console
         @see Console::Listener
         @see Console::History
         */
        class Message
        {
        public:
            enum Kind
            {
                Empty   = 0,
                Post    = 1,
                Error   = 2,
                Warning = 3
            };
            
            enum Sender
            {
                Unknown = 0,
                Box     = 1,
                Page    = 2,
                Instance= 3
            };
            
        private:
            const string                          m_message;
            const Kind                            m_kind;
            const weak_ptr<const Kiwi::Box>       m_box;
            const weak_ptr<const Kiwi::Page>      m_page;
            const weak_ptr<const Kiwi::Instance>  m_instance;
            
        public:
            //! The constructor.
            /** The constructor initialize the members.
             */
            Message(shared_ptr<const Kiwi::Instance> instance, shared_ptr<const Kiwi::Page> page, shared_ptr<const Kiwi::Box> box, Kind kind, string const& message) noexcept;
            
            //! The destructor.
            /** The destructor free the members.
             */
            ~Message();
            
            //! Retrieve the content of the message.
            /** The function retrieves the content of the message as a string.
             @return The content of the message.
             */
            inline string getContent() const noexcept
            {
                return m_message;
            }
            
            //! Retrieve the type of the message.
            /** The function retrieves the type of the message. It could a post, a warning or an error.
             @return The type of the message.
             */
            inline Kind getKind() const noexcept
            {
                return m_kind;
            }
            
            //! Retrieve the name of the sender.
            /** The function retrieves the name of the sender if it exists.
             @return The name of the sender.
             */
            string getName() const noexcept;
            
            //! Retrieve the class of the sender.
            /** The function retrieves the class of the sender of the message. It could a box, a page, an instance or unknown. If the sender is a box, you should also be able to retrieve the page and and the instance and if the sender is a page, you should be able to retrieve the instance.
             @return The class of the sender of the message.
             */
            Sender getSenderClass() const noexcept;
            
            //! Retrieve the box of the message.
            /** The function retrieves the box that has sent the message. If the sender wasn't a box or if the box has been deleted it will returns null.
             @return The box of the message.
             */
            inline weak_ptr<const Kiwi::Box> getBox() const noexcept
            {
                return m_box;
            }
            
            //! Retrieve the page of the message.
            /** The function retrieves the page that has sent the message or the page of the box that has sent the message. If the sender wasn't a page or a box or if the page has been deleted it will returns null.
             @return The page of the message.
             */
            inline weak_ptr<const Kiwi::Page> getPage() const noexcept
            {
                return m_page;
            }
            
            //! Retrieve the instance that of the message.
            /** The function retrieves the instance that has sent the message or the instance of the  page or the box that has sent the message. If the sender is unkown or if the instance has been deleted it will returns null.
             @return The instance that of the message.
             */
            inline weak_ptr<const Kiwi::Instance> getInstance() const noexcept
            {
                return m_instance;
            }
        };
    };
};


#endif


