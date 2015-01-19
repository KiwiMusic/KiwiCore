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
#include "Link.h"
#include "AttributePage.h"

// TODO
// - Add the attributes
// - Dsp & Mutex for Dsp
namespace Kiwi
{
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and links.
    /**
     The page is the counterpart of the max patcher or the pd canvas...
     */
    class Page : public AttrPage
	{
    public:
        class Listener;
        friend Box::Box(sPage page, string const& name, ulong flags);
		
		class Controller;
		typedef shared_ptr<Controller>			sController;
		typedef weak_ptr<Controller>			wController;
		typedef shared_ptr<const Controller>	scController;
		
    private:
        const wInstance             m_instance;
        
        Dsp::sContext               m_dsp_context;
        atomic_bool                 m_dsp_running;
        
        vector<sBox>                m_boxes;
        mutable mutex               m_boxes_mutex;
        ulong						m_boxe_id;
        
        vector<sLink>               m_links;
        mutable mutex               m_links_mutex;
		
		wController					m_controller;
        
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
        static sPage create(sInstance instance, sDico dico = nullptr);
		
		//! Retrieve the sPage.
		/** The function sPage.
		 @return The sPage.
		 */
		inline sPage getShared() noexcept
		{
			return static_pointer_cast<Page>(shared_from_this());
		}
		
		//! Retrieve the scPage.
		/** The function scPage.
		 @return The scPage.
		 */
		inline scPage getShared() const noexcept
		{
			return static_pointer_cast<const Page>(shared_from_this());
		}
		
        //! Retrieve the instance that manages the page.
        /** The function retrieves the instance that manages the page.
         @return The instance that manages the page.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
		
		//! Retrieve the controller that manages the page.
		/** The function retrieves the controller that manages the page.
		 @return The controller that manages the page.
		 */
		inline sController getController() const noexcept
		{
			return m_controller.lock();
		}
		
        //! Get the number of boxes.
        /** The function retrieves the number of boxes in the page.
         @return The number of boxes in the page.
         */
        inline ulong getNumberOfBoxes() const noexcept
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
        inline ulong getNumberOfLinks() const noexcept
        {
            lock_guard<mutex> guard(m_links_mutex);
            return m_links.size();
        }
        
        //! Get the links.
        /** The function retrieves the links from the page.
         @param links   A vector of links.
         */
        void getLinks(vector<sLink>& links) const
        {
            lock_guard<mutex> guard(m_links_mutex);
            links = m_links;
        }
		
		//! Receives notification when an attribute value has changed.
		/** The function receives notification when an attribute value has changed.
		 @param attr The attribute.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		bool attributeValueChanged(sAttr attr) override;
        
        //! Create a beacon.
        /** This function retrieves a beacon in the scope of the instance.
         @param     The name of the beacon to retrieve.
         @return    The beacon that match with the name.
         */
        sBeacon createBeacon(string const& name) const;
        
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
        
        //! Bring a box to the front of the page.
        /** The function brings a box to the front of the page. The box will be setted as if it was the last box created and will be the last box of the vector of boxes.
         @param box        The pointer to the box.
         */
        void bringToFront(sBox box);
        
        //! Bring a box to the back of the page.
        /** The function brings a box to the back of the page. The box will be setted as if it was the first box created and will be the first box of the vector of boxes.
         @param box        The pointer to the box.
         */
        void bringToBack(sBox box);
        
        //! Add a link.
        /** The function add a link.
         @param link The link to add.
         @return A pointer to the link.
         */
        sLink addLink(sLink link);
        
        //! Create a link.
        /** The function creates a link with a dico.
         @param dico        The dico that defines a link.
         @return A pointer to the link.
         */
        sLink createLink(scDico dico);
        
        //! Free a link.
        /** The function removes a link from the page.
         @param link        The pointer to the link.
         */
        void removeLink(sLink link);
        
        //! Append a dico.
        /** The function reads a dico and add the boxes and links to the page.
         @param dico The dico.
         */
        void append(sDico dico);
        
        //! Read a dico.
        /** The function reads a dico that initializes the page.
         @param dico The dico.
         */
        void read(sDico dico);
        
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
        bool startDsp(ulong samplerate, ulong vectorsize);
        
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
		
		//! Set the controller of the box.
		/** The function sets the controller of the box.
		 @param ctrl    The controller.
		 */
		void setController(sController ctrl);
        
        // ================================================================================ //
        //                                  PAGE CONTROLER                                  //
        // ================================================================================ //
        
        //! The page controller is an abstract class that facilitates the control of a page in an application.
        /**
         The page controller should be a shared pointer to be able to bind itself to a page. Thus, like in all the kiwi classes, you should use another creation method and call the bind function in it. The page controller owns a vector of box controllers and facilitates managements of boxes like the creation, the deletion, the selection, etc.
         @see Page, Page::Listener, Box::Controller
         */
		class Controller
        {            
        private:
            const sPage m_page;
			
        public:
			
			//! Constructor.
			/** You should never call this method except if you really know what you're doing.
			 Please use the create method instead.
             @param page The page to control.
             */
            Controller(sPage page) noexcept;
            
            //! The destructor.
            /** The destructor.
             */
            virtual ~Controller();
            
            //! Retrieve the page.
            /** The funtion retrieves the page.
             @return The page.
             */
            inline sPage getPage() const noexcept
            {
                return m_page;
            }
			
			//! Receive the notification that a box has been created.
			/** The function is called by the page when a box has been created.
			 @param box     The box.
			 */
			virtual void boxHasBeenCreated(sBox box) = 0;
			
			//! Receive the notification that a box has been removed.
			/** The function is called by the page when a box has been removed.
			 @param box     The box.
			 */
			virtual void boxHasBeenRemoved(sBox box) = 0;
			
			//! Receive the notification that a box has been replaced.
			/** The function is called by the page when a box has been replaced.
			 @param oldbox  The old box.
			 @param newbox	The new box.
			 */
			virtual void boxHasBeenReplaced(sBox oldbox, sBox newbox) = 0;
			
			//! Receive the notification that a link has been created.
			/** The function is called by the page when a link has been created.
			 @param link     The link.
			 */
			virtual void linkHasBeenCreated(sLink link) = 0;
			
			//! Receive the notification that a link has been removed.
			/** The function is called by the page when a link has been removed.
			 @param link    The link.
			 */
			virtual void linkHasBeenRemoved(sLink link) = 0;
			
			//! Receive the notification that a connection has been replaced.
			/** The function is called by the page when a connection has been replaced.
			 @param oldlink  The old link.
			 @param newlink	The new link.
			 */
			virtual void linkHasBeenReplaced(sLink oldlink, sLink newlink) = 0;
			
			//! Receives notification when an attribute value has changed.
			/** The function receives notification when an attribute value has changed.
			 @param attr The attribute.
			 @return pass true to notify changes to listeners, false if you don't want them to be notified
			 */
			virtual bool pageAttributeValueChanged(sAttr attr) = 0;
        };
		
		static const sTag Tag_page;
        static const sTag Tag_box;
        static const sTag Tag_boxes;
        static const sTag Tag_link;
        static const sTag Tag_links;
    };    
}


#endif


