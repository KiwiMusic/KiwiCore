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

// - Clear and add the links and boxes managemennt (Clean the id of the boxes)
// - Add the attributes
// - Make everything threadsafe
// - Mutex for Dsp
// - Clear the listeners

namespace Kiwi
{
    class DspContext;
    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and links.
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
        
        vector<sLink>               m_links;
        mutable mutex               m_links_mutex;
        
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
        
        //! Get the number of boxes.
        /** The function retrieves the number of boxes in the page.
         @return The number of boxes in the page.
         */
        inline unsigned long getNumberOfBoxes() const noexcept
        {
            lock_guard<mutex> guard(m_boxes_mutex);
            return m_boxes.size();
        }
        
        //! Get the boxes.
        /** The function retrieves the boxes from the page.
         @param boxes   A vector of elements.
         */
        void getBoxes(vector<sBox>& boxes) const
        {
            lock_guard<mutex> guard(m_boxes_mutex);
            boxes = m_boxes;
        }
        
        //! Get the number of links.
        /** The function retrieves the number of links in the page.
         @return The number of links in the page.
         */
        inline unsigned long getNumberOfConnections() const noexcept
        {
            lock_guard<mutex> guard(m_links_mutex);
            return m_links.size();
        }
        
        //! Get the links.
        /** The function retrieves the links from the page.
         @param links   A vector of links.
         */
        void getConnections(vector<sLink>& links) const
        {
            lock_guard<mutex> guard(m_links_mutex);
            links = m_links;
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
        
        //! Add a link.
        /** The function add a link.
         @param link The link to add.
         @return A pointer to the link.
         */
        sLink addConnection(sLink link);
        
        //! Create a link.
        /** The function creates a link with a dico.
         @param dico        The dico that defines a link.
         @return A pointer to the link.
         */
        sLink createConnection(scDico dico);
        
        //! Free a link.
        /** The function removes a link from the page.
         @param link        The pointer to the link.
         */
        void removeConnection(sLink link);
        
        //! Append a dico.
        /** The function reads a dico and add the boxes and links to the page.
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
        bool startDsp(unsigned long samplerate, unsigned long vectorsize);
        
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
            
            //! Receive the notification that a link has been created.
            /** The function is called by the page when a link has been created.
             @param page        The page.
             @param link  The box.
             */
            virtual void linkHasBeenCreated(sPage page, sLink link){};
            
            //! Receive the notification that a link has been removed.
            /** The function is called by the page when a link has been removed.
             @param page        The page.
             @param link  The link.
             */
            virtual void linkHasBeenRemoved(sPage page, sLink link){};
            
            //! Receive the notification that a link has been replaced by another one.
            /** The function is called by the page when a link has been replaced by another one.
             @param page            The page.
             @param oldlink   The link that has been replaced.
             @param newlink   The link that has replaced.
             */
            virtual void linkHasBeenReplaced(sPage page, sLink oldlink, sLink newlink){};
        };
        
        typedef shared_ptr<Listener>    sListener;
        
        // ================================================================================ //
        //                                  PAGE CONTROLER                                  //
        // ================================================================================ //
        
        //! The page controler is a virtual class that facilitates the control of a page in an application.
        /**
         The page controler should be a shared pointer to be able to bind itself to a page. Thus, like in all the kiwi classes, you should use another creation method and call the bind function in it. The page controler owns a vector of box controlers and facilitates managements of boxes like the creation, the deletion, the selection, etc.
         @see Page
         @see Page::Listener
         @see Box::Controler
         */
        class Controler : public Page::Listener, public enable_shared_from_this<Controler>
        {
        private:
            const sPage     m_page;
            vector<sBox>    m_boxes;
            
        public:
            //! The constructor.
            /** The constructor.
             @param page The page to control.
             */
            Controler(sPage page) noexcept;
            
            //! The destructor.
            /** The destructor.
             */
            ~Controler();
            
        protected:
            
            //! Bind the controler to the page.
            /** The function binds the controler to the page. You should call the method outside the contructor.
             */
            void bind();
            
            //! Unbind the controler to the page.
            /** The function unbinds the controler to the page. You should call the method outside the destructor. More often the call of this method isn't necessary because if you controler is deleted, the page will automaticaly remove it from the listener list.
             */
            void unbind();
            
            void selectAll();
            
            void deselectAll();
        };
        
        typedef shared_ptr<Controler>    sControler;
    };
    
}


#endif


