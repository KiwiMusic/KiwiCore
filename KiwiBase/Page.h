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
#include "AttributePage.h"

// TODO
// - Add the attributes
// - Dsp & Mutex for Dsp
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
    class Page : public AttrPage
	{
    public:
        class Listener;
        friend Box::Box(sPage page, string const& name, unsigned long type);
		
		class Controller;
		typedef shared_ptr<Controller>			sController;
		typedef weak_ptr<Controller>			wController;
		typedef shared_ptr<const Controller>	scController;
		
    private:
        const wInstance             m_instance;
        
        shared_ptr<DspContext>      m_dsp_context;
        atomic_bool                 m_dsp_running;
        
        vector<sBox>                m_boxes;
        mutable mutex               m_boxes_mutex;
        unsigned long               m_boxe_id;
        
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
		
		//! Retrieves box with id.
		/** The function attempts to retrieves a box from the page with an id.
		 @param boxID   The box id to find
		 @return		The  box with this id, or nullptr if no any box has this id.
		 */
		sBox getBoxWithId(const unsigned long boxID) const
		{
			lock_guard<mutex> guard(m_boxes_mutex);
			for(vector<sBox>::size_type i = 0; i < m_boxes.size(); i++)
			{
				if(boxID == m_boxes[i]->getId())
				{
					return m_boxes[i];
				}
			}
			return nullptr;
		}
		
        //! Get the number of links.
        /** The function retrieves the number of links in the page.
         @return The number of links in the page.
         */
        inline unsigned long getNumberOfLinks() const noexcept
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
         @see Page
         @see Page::Listener
         @see Box::Controller
         */
		class Controller : public Knock, public enable_shared_from_this<Controller>
        {
        public:
            class Lasso;
        private:
            const sPage						m_page;
			
			vector<Box::sController>		m_boxes;
			mutable mutex					m_boxes_mutex;
			set<Box::wController,
			owner_less<Box::wController>>	m_boxes_selected;
			mutable mutex					m_boxes_selected_mutex;
			vector<Rectangle>				m_boxes_bounds;
			
			vector<Link::sController>		m_links;
			mutable mutex					m_links_mutex;
			set<Link::wController,
			owner_less<Link::wController>>	m_links_selected;
			mutable mutex					m_links_selected_mutex;
			
			long m_zoom;
			bool m_locked;
			bool m_presentation;
			bool m_display_grid;
			bool m_snap_to_grid;
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
			
			//! The controller maker.
			/** The function creates a page controller with arguments.
			 */
			template<class CtrlClass, class ...Args> static shared_ptr<CtrlClass> create(Args&& ...arguments)
			{
				shared_ptr<CtrlClass> ctrl = make_shared<CtrlClass>(forward<Args>(arguments)...);
				if(ctrl && ctrl->m_page)
				{
					ctrl->m_page->setController(ctrl);
				}
				return ctrl;
			}
			
			//! Retrieve the page.
			/** The funtion retrieves the page.
			 @return The page.
			 */
			inline sPage getPage() const noexcept
			{
				return m_page;
			}
			
			//! Get box controllers.
			/** The function retrieves the box controllers of the page.
			 @param boxes   A vector of box controllers.
			 */
			void getBoxes(vector<Box::sController>& boxes) const
			{
				lock_guard<mutex> guard(m_boxes_mutex);
				boxes = m_boxes;
			}
			
			//! Get link controllers.
			/** The function retrieves the link controllers of the page.
			 @param boxes   A vector of link controllers.
			 */
			void getLinks(vector<Link::sController>& links) const
			{
				lock_guard<mutex> guard(m_links_mutex);
				links = m_links;
			}
			
			//! Retrieve the zoom of the page.
			/** The function retrieves the zoom of the page.
			 @return the zoom of the page in percent.
			 @see setZoom
			 */
			inline long getZoom() const noexcept
			{
				return m_zoom;
			}
			
			//! Set the zoom of the page.
			/** The function sets the zoom of the page.
			 @param zoom The zoom of the page in percent.
			 @see getZoom
			 */
			void setZoom(long zoom);
			
			//! Retrieve if the page is locked or unlocked.
			/** The function retrieves if the page is locked or unlocked.
			 @return True if the page is locked, false if it is unlocked.
			 @see setLockStatus
			 */
			inline bool getLockStatus() const noexcept
			{
				return m_locked;
			}
			
			//! Lock/Unlock the page.
			/** The function locks or unlocks the page.
			 @param locked True to lock the page, false to unlock it.
			 @see getLockStatus
			 */
			void setLockStatus(bool locked);
			
			//! Retrieve if the presentation mode of the page is active.
			/** The function retrieves if the presentation mode of the page is active.
			 @return True if the presentation mode of the page is active otherwise false.
			 */
			inline bool getPresentationStatus() const noexcept
			{
				return m_presentation;
			}
			
			//! Active the presentation mode of the page.
			/** The function actives the presentation mode of the page.
			 @param presentation True to active the presentation mode, otherwise false.
			 */
			void setPresentationStatus(bool presentation);
			
			//! Retrieve if the page is displays the grid.
			/** The function retrieves if the page is displays the grid.
			 @return True if the page is displays the grid is locked otherwise false.
			 */
			inline bool getGridDisplayedStatus() const noexcept
			{
				return m_display_grid;
			}
			
			//! Shows/Hides the grid of the page.
			/** The function shows or hides the grid of the page.
			 @param display True to show the grid, false to hide it.
			 */
			void setGridDisplayedStatus(bool display);
			
			//! Retrieve if the snap to grid mode of the page is active.
			/** The function retrieves if the snap to grid mode of the page is active.
			 @return True if the snap to grid mode of the page is active, otherwise false.
			 */
			inline bool getSnapToGridStatus() const noexcept
			{
				return m_snap_to_grid;
			}

			//! Active the snap to grid in the page.
			/** The function actives the snap to grid in the page.
			 @param snap True to active the snap to grid, otherwise false.
			 */
			void setSnapToGridStatus(bool snap);
			
			// ================================================================================ //
			//										SELECTION									//
			// ================================================================================ //
			
			//! Retrieves if some boxes or links are currently selected.
			/** The function retrieves if some boxes or links are currently selected.
			 @return True if some boxes or links are currently selected, false if nothing is selected.
			 */
			inline bool isSomethingSelected()
			{
				return isAnyBoxSelected() || isAnyLinksSelected();
			}
			
			//! Retrieves if some boxes are currently selected.
			/** The function retrieves if some boxes are currently selected.
			 @return True if some boxes are currently selected, false if no box is selected.
			 */
			inline bool isAnyBoxSelected()
			{
				lock_guard<mutex> guard(m_boxes_selected_mutex);
				return !m_boxes_selected.empty();
			}
			
			//! Retrieves if some links are currently selected.
			/** The function retrieves if some links are currently selected.
			 @return True if some links are currently selected, false if no box is selected.
			 */
			inline bool isAnyLinksSelected()
			{
				lock_guard<mutex> guard(m_links_selected_mutex);
				return !m_links_selected.empty();
			}
			
			//! Retrieves the selected boxes.
			/** The function retrieves the selected boxes.
			 */
			void getSelection(vector<Box::sController>& boxes) const noexcept;
			
			//! Retrieves the selected links.
			/** The function retrieves the selected links.
			 */
			void getSelection(vector<Link::sController>& links) const noexcept;
            
            //! Retrieves the selected boxes.
			/** The function retrieves the selected boxes.
			 */
			void getSelection(set<Box::wController,
                              owner_less<Box::wController>>& boxes) const noexcept;
			
			//! Retrieves the selected links.
			/** The function retrieves the selected links.
			 */
			void getSelection(set<Link::wController,
                              owner_less<Link::wController>>& links) const noexcept;
			
			//! Deletes all selected links and boxes.
			/** The function deletes all selected links and boxes.
			 */
			void deleteSelection();
			
			//! Retrieves if a box is selected.
			/** The function retrieve if a box is selected.
			 */
			bool isSelected(Box::sController box);
			
			//! Retrieves if a link is selected.
			/** The function retrieve if a link is selected.
			 */
			bool isSelected(Link::sController link);
			
			//! Adds all boxes to selection.
			/** The function adds all boxes to selection.
			 */
			bool selectAllBoxes();
			
			//! Adds all links to selection.
			/** The function adds all links to selection.
			 */
			bool selectAllLinks();
			
			//! Selects a set of boxes.
			/** The function selects a set of boxes.
			 */
			void select(vector<Box::sController>& boxes);
			
			//! Selects a set of links.
			/** The function selects a set of links.
			 */
			void select(vector<Link::sController>& links);
			
			//! Adds a box to the selection.
			/** The function adds a box to the selection.
			 */
			bool select(Box::sController box, const bool notify = true);
			
			//! Adds a link to the selection.
			/** The function adds a link to the selection.
			 */
			bool select(Link::sController link, const bool notify = true);
			
			//! Clears the selection then the selects a box.
			/** The function clears the selection then the selects a box.
			 */
			bool selectOnly(Box::sController box);
			
			//! Clears the selection then the selects a box.
			/** The function clears the selection then the selects a box.
			 */
			bool selectOnly(Link::sController link);
			
			//! Unselects all boxes and links.
			/** The function unselects all boxes and links.
			 */
			void unselectAll(const bool notify = true);
			
			//! Unselects all boxes.
			/** The function unselects all boxes.
			 */
			bool unselectAllBoxes(const bool notify = true);
			
			//! Unselects all links.
			/** The function unselects all links.
			 */
			bool unselectAllLinks(const bool notify = true);
			
			//! Unselects a set of boxes.
			/** The function unselects a set of boxes.
			 */
			void unselect(vector<Box::sController>& boxes);
			
			//! Unselects a set of links.
			/** The function unselects a set of links.
			 */
			void unselect(vector<Link::sController>& links);
			
			//! Removes a box from the selection.
			/** The function unselects box.
			 */
			bool unselect(Box::sController box, const bool notify = true);
			
			//! Removes a link from the selection.
			/** The function unselects link.
			 */
			bool unselect(Link::sController link, const bool notify = true);
			
			void updateSelectedBoxesBounds();
			
			//! Retrieves the selected boxes bounds.
			/** The function retrieves the selected boxes bounds.
			 @return The selected boxes bounds as a rectangle.
			 */
			Rectangle getSelectionBounds();
			
			//! Gets the boxes contained in a given rectangle.
			/** The function retrieves the boxes contained in a given rectangle.
			 @param boxes A vector of boxes to fill.
			 @param rect  The rectangle.
			 */
			void getBoxesInRect(vector<Box::sController>& boxes, Rectangle const& rect) const noexcept;
			
			//! Gets the links overlapped by a given rectangle.
			/** The function retrieves the links overlapped by a given rectangle.
			 @param links A vector of links to fill.
			 @param rect  The rectangle.
			 */
			void getLinksInRect(vector<Link::sController>& links, Rectangle const& rect) const noexcept;
			
			//! Moves the boxes that are currently selected by given value.
			/** The function moves the boxes that are currently selected by given value.
			 @param delta A shift amount delta.
			 */
			void moveSelectedBoxes(Point const& delta);
			
			//! Retrieve the selected boxes (including links) as a dico
			/** The function retrieve the selected boxes (including links) as a dico.
			 You may use it to copy selection to clipboard.
			 @param dico The dico to fill.
			 */
			void getSelectedBoxesDico(sDico dico);
			
			//! Adds boxes to the page from a dico
			/** The function adds boxes to the page from a dico
			 The dico must be formated with getSelectedBoxesDico
			 @param dico The dico.
			 @param shift Shift the position of the boxes.
			 @return True if the page has been modified, false otherwise
			 */
			bool addBoxesFromDico(sDico dico, Point const& shift = Point());
			
			//! Receive the notification that a box has been created.
			/** The function is called by the page when a box has been created.
			 @param page    The page.
			 @param box     The box.
			 */
			virtual void boxHasBeenCreated(sBox box) {};
			
			//! Receive the notification that a box has been removed.
			/** The function is called by the page when a box has been removed.
			 @param page    The page.
			 @param box     The box.
			 */
			virtual void boxHasBeenRemoved(sBox box) {};
			
			//! Receive the notification that a box has been replaced by another one.
			/** The function is called by the page when a box has been replaced by another one.
			 @param page    The page.
			 @param oldbox  The box that has been replaced.
			 @param newbox  The box that has replaced.
			 */
			virtual void boxHasBeenReplaced(sBox oldbox, sBox newbox) {};
			
			//! Receive the notification that a connection has been created.
			/** The function is called by the page when a connection has been created.
			 @param page        The page.
			 @param connection  The box.
			 */
			virtual void linkHasBeenCreated(sLink connection) {};
			
			//! Receive the notification that a connection has been removed.
			/** The function is called by the page when a connection has been removed.
			 @param page        The page.
			 @param connection  The connection.
			 */
			virtual void linkHasBeenRemoved(sLink connection) {};
			
			//! Receive the notification that a connection has been removed.
			/** The function is called by the page when a connection has been removed.
			 @param page        The page.
			 @param connection  The connection.
			 */
			virtual void linkHasBeenReplaced(sLink oldlink, sLink newlink) {};
			
			//! Receives notification when an attribute value has changed.
			/** The function receives notification when an attribute value has changed.
			 @param attr The attribute.
			 @return pass true to notify changes to listeners, false if you don't want them to be notified
			 */
			virtual bool pageAttributeValueChanged(sAttr attr) { return true; };
			
        protected:
			
			void addBoxController(Box::sController box);
			void removeBoxController(Box::sController box);
			
			Box::sController getBoxController(sBox box) const noexcept;
			
			void addLinkController(Link::sController link);
			void removeLinkController(Link::sController link);
			
			Link::sController getLinkController(sLink link) const noexcept;
			
			//! The redraw function that must be overriden to be notified when the page needs to be redrawn.
			/** The function is called by the page when it needs to be repainted.
			 */
			virtual void redraw() {};
			
			//! Called when the selection has changed.
			/** The function is called when the selection has changed.
			 */
			virtual void selectionChanged() {};
			
			//! Called when the page has been locked/unlocked.
			/** The function is called when the page has been locked/unlocked.
			 */
			virtual void lockStatusChanged() {};
        };
        
        static const sTag Tag_box;
        static const sTag Tag_boxes;
        static const sTag Tag_link;
        static const sTag Tag_links;
    };
    
    // ================================================================================ //
    //										LASSO                                       //
    // ================================================================================ //
    
    class Page::Controller::Lasso
    {
    private:
        Page::wController			m_page_ctrl;
        Rectangle                   m_bounds;
        Point                       m_start;
        bool						m_dragging;
        bool                        m_preserve;
        
        set<Box::wController,
        owner_less<Box::wController>>   m_boxes;
        set<Link::wController,
        owner_less<Link::wController>>	m_links;
        
    public:
        
        //! Contructor.
        /** You should never have to use this method.
         */
        Lasso(Page::sController page);
        
        //! Destructor.
        /** You should never have to use this method.
         */
        virtual ~Lasso();
        
        //! The lasso creation method.
        /** The function allocates a lasso.
         @param page The page controler that used the lasso/
         */
        template<class LassoClass, class ...Args> static shared_ptr<LassoClass> create(Page::sController page, Args&& ...arguments)
        {
            return make_shared<LassoClass>(page, forward<Args>(arguments)...);
        }
        
        //! Initialize the selection of the links and the boxes.
        /** The function initialize the selection of the links and the boxes.
         @param point The starting point.
         @param preserve The lasso should preserve the last selection.
         */
        void begin(Point const& point, bool preserve);
        
        //! Perform the selection of the links and the boxes.
        /** The function performs the selection of the links and the boxes.
         @param point The draging point.
         @param preserve The lasso should preserve the last selection.
         @param boxes The lasso should add boxes to the selection.
         @param links The lasso should add links to the selection.
         */
        void perform(Point const& point, bool boxes, bool links);
        
        //! Finish the selection of the links and the boxes.
        /** The function finishes the selection of the links and the boxes.
         @param preserve The lasso should preserve the last selection.
         */
        void end();
        
        //! The draw method that could be override.
        /** The function draws the lasso in a doodle.
         @param d The doodle.
         */
        virtual void draw(Doodle& d);
        
        //! Retrieve if the lasso is performing the selection.
        /** The function retrieves if the lasso is performing the selection.
         @return True if the lasso is performing the selection.
         */
        inline bool isPerforming() const noexcept
        {
            return m_dragging;
        }
        
        //! Retrieve the bounds of the lasso.
        /** The function retrieves the bounds of the lasso.
         @return The bounds of the lasso.
         */
        inline Rectangle getBounds() const noexcept
        {
            return m_bounds;
        }

        virtual void boundsHasChanged() = 0;
    };
    
}


#endif


