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

#ifndef __DEF_KIWI_PAGE__
#define __DEF_KIWI_PAGE__

#include "Box.h"

namespace Kiwi
{
    class DspContext;
    class Connection;
    typedef shared_ptr<Connection> sConnection;
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and connections.
    /**
     The page is the counterpart of the max patcher or the pd canvas.
     */
    class Page : public Box::Listener, public enable_shared_from_this<Page>
    {
    public:
        class Listener;
    private:
        const weak_ptr<Instance>    m_instance;
        vector<sBox>                m_boxes;
        vector<sConnection>         m_connections;
        shared_ptr<DspContext>      m_dsp_context;
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>   m_listeners;
        
        bool                         m_dsp_running;
        
        static bool sortBoxes(Element first, Element second);
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Page(shared_ptr<Instance> instance);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Page();
        
        //! The page creation method.
        /** The function allocates a page and initialize the defaults boxes.
         */
        static shared_ptr<Page> create(shared_ptr<Instance> instance, sDico dico);
        
        //! Retrieve the instance that manages the page.
        /** The function retrieves the instance that manages the page.
         @return The instance that manages the page.
         */
        shared_ptr<Instance> getInstance() const noexcept;
        
        //! Create a box.
        /** The function instantiates a box with a dico.
         @param dico        The dico that defines a box.
         @return A pointer to the box.
         */
        sBox createBox(sDico dico);
        
        //! Free a box.
        /** The function removes a box from the page.
         @param box        The pointer to the box.
         */
        void removeBox(sBox box);
        
        //! Get the boxes.
        /** The function retrieves the boxes from the page.
         @param boxes   A vector of elements.
         */
        void getBoxes(vector<sBox>& boxes);
        
        //! Create a connection.
        /** The function creates a connection with a dico.
         @param dico        The dico that defines a connection.
         @return A pointer to the box.
         */
        sConnection createConnection(sDico dico);
        
        //! Free a connection.
        /** The function removes a connection from the page.
         @param connection        The pointer to the connection.
         */
        void removeConnection(sConnection connection);
        
        //! Get the connections.
        /** The function retrieves the connections from the page.
         @param connections   A vector of connections.
         */
        void getConnections(vector<sConnection>& connections);
        
        //! Read a dico.
        /** The function reads a dico that initializes the page.
         @param dico The dico.
         */
        void read(sDico dico);
        
        //! Write the page in a dico.
        /** The function writes the pagein a dico.
         @param dico The dico.
         */
        void write(sDico dico);
        
        //! Start the dsp.
        /** The function start the dsp chain.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @return true if the page can process signal.
         */
        bool startDsp(double samplerate, long vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain.
         */
        void tickDsp() const noexcept;
        
        //! Stop the dsp.
        /** The function stop the dsp chain.
         */
        void stopDsp();
        
        //! Check if the dsp is running.
        /** The function checks if the dsp is running
         */
        bool isDspRunning() const noexcept;
        
        //! Receive the notification that an inlet has been created.
        /** The function is called by the box when a inlet has been created.
         @param box    The box.
         @param index  The inlet index.
         */
        void inletHasBeenCreated(shared_ptr<Box> box, size_t index) override;
        
        //! Receive the notification that an inlet has been removed.
        /** The function is called by the box when an inlet has been removed.
         @param box    The box.
         @param index  The inlet index.
         */
        void inletHasBeenRemoved(shared_ptr<Box> box, size_t index) override;
        
        //! Receive the notification that an outlet has been created.
        /** The function is called by the box when a outlet has been created.
         @param box    The box.
         @param index  The outlet index.
         */
        void outletHasBeenCreated(shared_ptr<Box> box, size_t index) override;
        
        //! Receive the notification that an outlet has been removed.
        /** The function is called by the box when an outlet has been removed.
         @param box    The box.
         @param index  The outlet index.
         */
        void outletHasBeenRemoved(shared_ptr<Box> box, size_t index) override;
        
        //! Add a page listener in the binding list of the page.
        /** The function adds a page listener in the binding list of the page. If the page listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the page listener.
         @see              unbind()
         */
        void bind(weak_ptr<Listener> listener);
        
        //! Remove a page listener from the binding list of the page.
        /** The function removes a page listener from the binding list of the page. If the page listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the page listener.
         @see           bind()
         */
        void unbind(weak_ptr<Listener> listener);
        
        // ================================================================================ //
        //                                  PAGE LISTENER                                   //
        // ================================================================================ //
        
        //! The page listener is a virtual class that can bind itself to a page and be notified of the several changes.
        /**
         The page listener is a very light class with methods that receive the notifications of the pages.
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
            
            //! Receive the notification that a box has been created.
            /** The function is called by the instance when a box has been created.
             @param page    The page.
             @param box     The box.
             */
            virtual void boxHasBeenCreated(shared_ptr<Page> page, sBox box){};
            
            //! Receive the notification that a box has been removed.
            /** The function is called by the instance when a box has been removed.
             @param page    The page.
             @param box     The box.
             */
            virtual void boxHasBeenRemoved(shared_ptr<Page> page, sBox box){};
            
            //! Receive the notification that a connection has been created.
            /** The function is called by the instance when a connection has been created.
             @param page        The page.
             @param connection  The box.
             */
            virtual void connectionHasBeenCreated(shared_ptr<Page> page, sConnection connection){};
            
            //! Receive the notification that a connection has been removed.
            /** The function is called by the instance when a connection has been removed.
             @param page        The page.
             @param connection  The connection.
             */
            virtual void connectionHasBeenRemoved(shared_ptr<Page> page, sConnection connection){};
        };
        
        typedef shared_ptr<Listener>    sListener;
    };
    
    typedef shared_ptr<Page>    sPage;
    
    typedef weak_ptr<Page>      wPage;
    
    //! The connection owns to a page and is used to create a patch lines.
    /**
     The connection is opaque, you shouldn't have to use it at all.
     */
    class Connection
    {
    private:
        
        const sBox     m_from;
        const sBox     m_to;
        const size_t   m_outlet;
        const size_t   m_inlet;
        
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Connection(const sBox from, const size_t outlet, const sBox to, const size_t inlet) noexcept :
        m_from(from),
        m_to(to),
        m_outlet(outlet),
        m_inlet(inlet)
        {
            ;
        }
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Connection()
        {
            ;
        }
        
        //! Retrieve the output box.
        /** The function retrieves the output box of the connection.
         @return The output box.
         */
        inline sBox getFrom() const noexcept
        {
            return m_from;
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the connection.
         @return The input box.
         */
        inline sBox getTo() const noexcept
        {
            return m_to;
        }
        
        //! Retrieve the outlet of the connection.
        /** The function retrieves the outlet of the connection.
         @return The outlet of the connection.
         */
        inline size_t getOutletIndex() const noexcept
        {
            return m_outlet;
        }
        
        //! Retrieve the inlet of the connection.
        /** The function retrieves the inlet of the connection.
         @return The inlet of the connection.
         */
        inline size_t getInletIndex() const noexcept
        {
            return m_inlet;
        }
    };
    
    typedef shared_ptr<Connection> sConnection;
}


#endif


