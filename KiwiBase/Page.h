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

// - Clear and add the connections and boxes managemennt
// - Add the attributes
// - Make everything threadsafe
// - Mutex for Dsp
// - Clear the listeners
// - See how to manage connection
namespace Kiwi
{
    class DspContext;
    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and connections.
    /**
     The page is the counterpart of the max patcher or the pd canvas...
     */
    class Page : public Box::Listener, public enable_shared_from_this<Page>
    {
    public:
        class Listener;
    private:
        const wInstance             m_instance;
        
        shared_ptr<DspContext>      m_dsp_context;
        atomic_bool                 m_dsp_running;
        
        vector<sBox>                m_boxes;
        mutable mutex               m_boxes_mutex;
        atomic_ulong                m_boxes_id;
        
        vector<sConnection>         m_connections;
        mutable mutex               m_connections_mutex;
        
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>   m_listeners;
        mutex                       m_listeners_mutex;
        
        static bool sortBoxes(Element first, Element second);
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Page(sInstance instance);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Page();
        
        //! The page creation method.
        /** The function allocates a page and initialize the defaults boxes.
         @param instance The instance that will manage the page.
         @param dico The dico that will initialize the page.
         @return The page.
         */
        static sPage create(sInstance instance, scDico dico = nullptr);
        
        //! Retrieve the instance that manages the page.
        /** The function retrieves the instance that manages the page.
         @return The instance that manages the page.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
        
        //! Retrieve the current id to intialize a boxe.
        /** This function retrieves the current id to intialize a boxe.
         @return    The id.
         */
        inline unsigned long getCurrentId() const noexcept
        {
            return m_boxes_id;
        }
        
        //! Create a beacon.
        /** This function retrieves a beacon in the scope of the instance.
         @param     The name of the beacon to retrieve.
         @return    The beacon that match with the name.
         */
        sBeacon createBeacon(string const& name);
        
        //! Create a box.
        /** The function instantiates a box with a dico.
         @param dico        The dico that defines a box.
         @return A pointer to the box.
         */
        sBox createBox(sDico dico);
        
        //! Replace a box with another one.
        /** The function instantiates a box with a dico that will replace an old box.
         @param box        The box to replace.
         @param dico       The dico that defines a box.
         @return A pointer to the box.
         */
        sBox replaceBox(sBox box, sDico dico);
        
        //! Free a box.
        /** The function removes a box from the page.
         @param box        The pointer to the box.
         */
        void removeBox(sBox box);
        
        //! Get the boxes.
        /** The function retrieves the boxes from the page.
         @param boxes   A vector of elements.
         */
        void getBoxes(vector<sBox>& boxes) const
        {
            lock_guard<mutex> guard(m_boxes_mutex);
            boxes = m_boxes;
        }
        
        //! Get the number of boxes.
        /** The function retrieves the number of boxes in the page.
         @return The number of boxes in the page.
         */
        inline unsigned long getNumberOfBoxes() const noexcept
        {
            lock_guard<mutex> guard(m_boxes_mutex);
            return m_boxes.size();
        }
        
        //! Add a connection.
        /** The function add a connection.
         @param connection The connection to add.
         @return A pointer to the connection.
         */
        sConnection addConnection(sConnection connection);
        
        //! Create a connection.
        /** The function creates a connection with a dico.
         @param dico        The dico that defines a connection.
         @return A pointer to the connection.
         */
        sConnection createConnection(scDico dico);
        
        //! Free a connection.
        /** The function removes a connection from the page.
         @param connection        The pointer to the connection.
         */
        void removeConnection(sConnection connection);
        
        //! Get the connections.
        /** The function retrieves the connections from the page.
         @param connections   A vector of connections.
         */
        void getConnections(vector<sConnection>& connections) const
        {
            lock_guard<mutex> guard(m_connections_mutex);
            connections = m_connections;
        }
        
        //! Get the number of connections.
        /** The function retrieves the number of connections in the page.
         @return The number of connections in the page.
         */
        inline unsigned long getNumberOfConnections() const noexcept
        {
            lock_guard<mutex> guard(m_connections_mutex);
            return m_connections.size();
        }
        
        //! Append a dico.
        /** The function reads a dico and add the boxes and connections to the page.
         @param dico The dico.
         */
        void append(scDico dico);
        
        //! Read a dico.
        /** The function reads a dico that initializes the page.
         @param dico The dico.
         */
        void read(scDico dico);
        
        //! Write the page in a dico.
        /** The function writes the pagein a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
        //! Start the dsp.
        /** The function start the dsp chain.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @return true if the page can process signal.
         */
        bool startDsp(long samplerate, long vectorsize);
        
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
        void inletHasBeenCreated(sBox box, unsigned long index) override;
        
        //! Receive the notification that an inlet has been removed.
        /** The function is called by the box when an inlet has been removed.
         @param box    The box.
         @param index  The inlet index.
         */
        void inletHasBeenRemoved(sBox box, unsigned long index) override;
        
        //! Receive the notification that an outlet has been created.
        /** The function is called by the box when a outlet has been created.
         @param box    The box.
         @param index  The outlet index.
         */
        void outletHasBeenCreated(sBox box, unsigned long index) override;
        
        //! Receive the notification that an outlet has been removed.
        /** The function is called by the box when an outlet has been removed.
         @param box    The box.
         @param index  The outlet index.
         */
        void outletHasBeenRemoved(sBox box, unsigned long index) override;
        
        //! Add a page listener in the binding list of the page.
        /** The function adds a page listener in the binding list of the page. If the page listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the page listener.
         @see              unbind()
         */
        void bind(shared_ptr<Listener> listener);
        
        //! Remove a page listener from the binding list of the page.
        /** The function removes a page listener from the binding list of the page. If the page listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the page listener.
         @see           bind()
         */
        void unbind(shared_ptr<Listener> listener);
        
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
            /** The function is called by the page when a box has been created.
             @param page    The page.
             @param box     The box.
             */
            virtual void boxHasBeenCreated(sPage page, sBox box){};
            
            //! Receive the notification that a box has been replaced by another one.
            /** The function is called by the page when a box has been replaced by another one.
             @param page    The page.
             @param oldbox  The box that has been replaced.
             @param newbox  The box that has replaced.
             */
            virtual void boxHasBeenReplaced(sPage page, sBox oldbox, sBox newbox){};
            
            //! Receive the notification that a box has been removed.
            /** The function is called by the page when a box has been removed.
             @param page    The page.
             @param box     The box.
             */
            virtual void boxHasBeenRemoved(sPage page, sBox box){};
            
            //! Receive the notification that a connection has been created.
            /** The function is called by the page when a connection has been created.
             @param page        The page.
             @param connection  The box.
             */
            virtual void connectionHasBeenCreated(sPage page, sConnection connection){};
            
            //! Receive the notification that a connection has been removed.
            /** The function is called by the page when a connection has been removed.
             @param page        The page.
             @param connection  The connection.
             */
            virtual void connectionHasBeenRemoved(sPage page, sConnection connection){};
            
            //! Receive the notification that a connection has been replaced by another one.
            /** The function is called by the page when a connection has been replaced by another one.
             @param page            The page.
             @param oldconnection   The connection that has been replaced.
             @param newconnection   The connection that has replaced.
             */
            virtual void connectionHasBeenReplaced(sPage page, sConnection oldconnection, sConnection newconnection){};
        };
        
        typedef shared_ptr<Listener>    sListener;
    };
}


#endif


