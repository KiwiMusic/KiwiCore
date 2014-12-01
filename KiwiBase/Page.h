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
    class Page : public enable_shared_from_this<Page>
    {
    public:
        class Listener;
        friend Box::Box(sPage page, string const& name, unsigned long type);
		
		class Controller;
		typedef shared_ptr<Controller>   sController;
		typedef weak_ptr<Controller>     wController;
		
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
        class Controller
        {
		public:
			class TempLink;
        private:
            const sPage						m_page;
			
			vector<Box::sController>		m_boxes;
			set<Box::wController,
			owner_less<Box::wController>>	m_boxes_selected;
			vector<Rectangle>				m_boxes_bounds;
			
			vector<Link::sController>		m_links;
			set<Link::wController,
			owner_less<Link::wController>>	m_links_selected;
			
			vector<TempLink>				m_templinks;
			
			long m_zoom;
			bool m_locked;
			bool m_presentation;
			bool m_display_grid;
			bool m_snap_to_grid;
			
			bool	m_box_is_dragging;
			bool	m_mousedown_select_status;
			
        public:
			class TempLink
			{
			private:
				const sBox	m_attached_box;
				const bool	m_attached_to_outlet;
				const long	m_io_index;
				Point		m_coord_start;
				Point		m_coord_end;
			public:
				TempLink(sBox attachedBox, Point startCoord, Point endCoord, long index, bool fromOutlet) :
				m_attached_box(attachedBox),
				m_attached_to_outlet(fromOutlet),
				m_io_index(index),
				m_coord_start(startCoord),
				m_coord_end(startCoord)
				{
					;
				}
				
				inline Point getStartCoord() const noexcept
				{
					return m_coord_start;
				}
				
				inline Point getEndCoord() const noexcept
				{
					return m_coord_end;
				}
				
				inline void setStartCoord(Point const& pt) noexcept
				{
					m_coord_start = pt;
				}
				
				inline void setEndCoord(Point const& pt) noexcept
				{
					m_coord_end = pt;
				}
				
				inline bool isAttachedToOutlet() const noexcept
				{
					return m_attached_to_outlet;
				}
				
				inline long getAttachedIOIndex() const noexcept
				{
					return m_io_index;
				}
				
				inline sBox getAttachedBox() const noexcept
				{
					return m_attached_box;
				}
			};
			
			class HitTest
			{
			public:
				enum Type
				{
					Nothing = 0,
					Page	= 1,
					Box		= 2,
					Link	= 3
				};
				
				HitTest(sController owner, Point const& pt) : m_owner(owner)
				{
					m_hittype = Type::Nothing;
					m_box.reset();
					m_link.reset();
					
					bool hit = false;
					hit = doBoxHitTest(pt);
					
					if (!hit)
						hit = doLinkHitTest(pt);
					
					if (!hit)
						m_hittype = Type::Page;
				}
				
				bool doBoxHitTest(Point const& pt)
				{
					m_hittype = Type::Nothing;
					m_box.reset();

					for(size_t i = m_owner->m_boxes.size(); i; i--)
					{
						if(m_owner->m_boxes[i-1]->isHit(pt, m_box_hit))
						{
							m_box = m_owner->m_boxes[i-1];
							m_hittype = Type::Box;
							return true;
						}
					}
					return false;
				}
				
				bool doLinkHitTest(Point const& pt)
				{
					m_hittype = Type::Nothing;
					m_link.reset();

					Link::Controller::Hit hit;
					for(size_t i = m_owner->m_links.size(); i; i--)
					{
						if(m_owner->m_links[i-1]->isHit(pt, hit))
						{
							m_link = m_owner->m_links[i-1];
							m_hittype = Type::Link;
							break;
						}
					}
					return false;
				}
				
				Box::sController getHittedBox()
				{
					if (m_hittype == Type::Box && !m_box.expired())
					{
						return m_box.lock();
					}
					return nullptr;
				}
				
				Box::Controller::Hit getBoxHit()
				{
					if (m_hittype == Type::Box && !m_box.expired())
					{
						return m_box_hit;
					}
					return Box::Controller::Hit();
				}
				
				Link::Controller::Hit getLinkHit()
				{
					if (m_hittype == Type::Link && !m_link.expired())
					{
						return m_link_hit;
					}
					return Link::Controller::Hit();
				}
				
				Link::sController getHittedLink()
				{
					if (m_hittype == Type::Link && !m_link.expired())
					{
						return m_link.lock();
					}
					return nullptr;
				}
				
				inline bool hasHittedBox() const noexcept
				{
					return m_hittype == Type::Box;
				}
				
				inline bool hasHittedLink() const noexcept
				{
					return m_hittype == Type::Link;
				}
				
			private:
				Type					m_hittype;
				Box::wController		m_box;
				Box::Controller::Hit	m_box_hit;
				Link::wController		m_link;
				Link::Controller::Hit	m_link_hit;
				const sController		m_owner;
			};
			
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
			
			//! Retrieves if some boxes or links are currently selected.
			/** The function retrieves if some boxes or links are currently selected.
			 @return True if some boxes or links are currently selected, false if nothing is selected.
			 */
			inline bool isSomethingSelected()
			{
				return !m_boxes_selected.empty();
			}
			
			//! Retrieves if there are one or more temporary links.
			/** The function retrieves if there are one or more temporary links.
			 @return True if there are one or more temporary links, false there is any temporary links.
			 */
			inline bool hasTempLinks()
			{
				return !m_templinks.empty();
			}
			
			//! Retrieves if there are one or more temporary links.
			/** The function retrieves if there are one or more temporary links.
			 @return True if there are one or more temporary links, false there is any temporary links.
			 */
			inline TempLink* getTempLink(const unsigned long index = 0) const
			{
				if (index < m_templinks.size())
				{
					return (TempLink*)&m_templinks[index];
				}
				
				return nullptr;
			}
			
        protected:
			void mouseDown(const Event::Mouse& e);
			void mouseUp(const Event::Mouse& e);
			void mouseDrag(const Event::Mouse& e);
			void mouseMove(const Event::Mouse& e);
			void mouseDoubleClick(const Event::Mouse& e);
			//void mouseWheelMove(const Event::Mouse& e, const MouseWheelDetails& wheel);
			//bool keyPressed(const KeyPress& key, Component* origin);
			
			void addBoxController(Box::sController box);
			void removeBoxController(Box::sController box);
			
			Box::sController getBoxController(sBox box) const noexcept;
			Box::sController getBoxController(Point const& pt) const noexcept;
			
			void addLinkController(Link::sController link);
			void removeLinkController(Link::sController link);
			
			Link::sController getLinkController(sLink link) const noexcept;
			
			//! Deletes all selected links and boxes.
			/** The function deletes all selected links and boxes.
			 */
			void deleteSelection();
			
			//! Unselects all boxes and links.
			/** The function unselects all boxes and links.
			 */
			void unSelectAll();
			
			bool addAllBoxesToSelection();
			bool addAllLinksToSelection();
			
			bool unSelectAllBoxes(bool notify);
			bool unSelectAllLinks(bool notify);
			
			bool isBoxInSelection(Box::sController box);
			bool addBoxToSelection(Box::sController box);
			bool selectOnlyBox(Box::sController box);
			bool removeBoxFromSelection(Box::sController box);
			
			void addToSelectionBasedOnModifiers(Box::sController box, bool selOnly);
			bool selectOnMouseDown(Box::sController box, bool selOnly);
			void selectOnMouseUp(Box::sController box, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod);
			
			bool isLinkInSelection(Link::sController link);
			bool addLinkToSelection(Link::sController link);
			bool selectOnlyLink(Link::sController link);
			bool removeLinkFromSelection(Link::sController link);
			
			void addToSelectionBasedOnModifiers(Link::sController link, bool selOnly);
			bool selectOnMouseDown(Link::sController link, bool selOnly);
			void selectOnMouseUp(Link::sController link, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod);

			
			void updateSelectedBoxesBounds();
			
			void dragSelectedBoxes(double dx, double dy);
			
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
			
			//! Called when the mouse enter in an box's inlet or outlet zone.
			/** The function is called when the mouse enter in an box's inlet or outlet zone.
			 */
			virtual void mouseHoverIO(const bool overInlet, const unsigned long index) {};
			
			//! Called when the mouse enter in an box's inlet or outlet zone.
			/** The function is called when the mouse enter in an box's inlet or outlet zone.
			 */
			virtual void mouseExitIO() {};
        };
    };
    
}


#endif


