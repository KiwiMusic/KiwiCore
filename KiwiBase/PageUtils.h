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

#ifndef __DEF_KIWI_KNOCK__
#define __DEF_KIWI_KNOCK__

#include "Box.h"
#include "Link.h"

namespace Kiwi
{
	class BoxResizer;
    // ================================================================================ //
    //                                      KNOCK                                       //
    // ================================================================================ //
	
    //! The knock is a small class to test if a box, a link or a page has been touch by a point or if a rectangle overlaps them.
    /**
     The knock...
     */
    class Knock
    {
        friend class Link;
        friend class Box;
		friend class BoxResizer;
    public:
		
        enum Border
        {
            Left  = 0,
            Right = 1,
            Top   = 2,
            Bottom= 3,
        };
        
        enum Corner
        {
			none		= 0,
            TopLeft     = 1,
            TopRight    = 2,
            BottomLeft  = 3,
            BottomRight = 4
        };
        
        enum Part
        {
            Outside     = 0,
            Inside      = 1,
            Inlet       = 2,
            Outlet      = 3,
            Corner      = 4,
            Border      = 5
        };
        
        enum Target
        {
            Nothing = 0,
            Page	= 1,
            Box		= 2,
            Link	= 3
        };
        
    private:
        const wPage     m_page;
        wBox            m_box;
        wLink           m_link;
        enum Target     m_target	= Nothing;
        enum Part       m_part		= Outside;
		enum Corner		m_corner	= TopRight;
		enum Border		m_border	= Right;
        ulong   m_index = 0;
		
    public:
        
        //! The contructor.
        /** The contructor initialize an empty knock.
         @param page The page that owns the knock.
         */
        Knock(sPage page) noexcept;
        
        //! The destructor.
        /** The destructor does nothing.
         */
        ~Knock();
        
        //! Reset the touch test.
        /** The function resets the knock as it has never touch any box, any link or any page.
         */
        void knockReset() noexcept;
        
        //! Test a point.
        /** The function try to find a box or then link under the point otherwise it will consider that the page has been touched.
         @param point The point.
         */
        void knockAll(Point const& point) noexcept;
        
        //! Test a point.
        /** The function try to find a box under the point.
         @param page The page to test.
         @param point The point.
         */
        void knockBoxes(Point const& point) noexcept;
        
        //! Test a point.
        /** The function try to find a box .
         @param point The point.
         */
        void knockLinks(Point const& point) noexcept;
        
        //! Test a rectangle.
        /** TThe function try to find the boxes and the links that overlaps the rectangle.
         @param rect The rectangle.
         @param boxes The vector of boxes that will be filled.
         @param links The vector of links that will be filled.
         */
        void knockAll(Rectangle const& rect, vector<Box::sController>& boxes, vector<Link::sController>& links) noexcept;
        
        //! Test a rectangle.
        /** The function try to find the boxes that overlaps the rectangle.
         @param rect The rectangle.
         @param boxes The vector of boxes that will be filled.
         */
        void knockBoxes(Rectangle const& rect, vector<Box::sController>& boxes) noexcept;
        
        //! Test a rectangle.
        /** The function try to find the links that overlaps the rectangle.
         @param rect The rectangle.
         @param links The vector of links that will be filled.
         */
        void knockLinks(Rectangle const& rect, vector<Link::sController>& links) noexcept;
        
        //! Retrieve the last target touched.
        /** The function retrieves the last target touched by a point.
         @return The target.
         */
        inline Target knockGetTarget() const noexcept
        {
            return m_target;
        }
        
        //! Retrieve if the last target was nothing.
        /** The function retrieves if the last target was nothing.
         @return true if the last target was nothing, otherwise false.
         */
        inline bool knockHasHitNothing() const noexcept
        {
            return m_target == Nothing;
        }
        
        //! Retrieve if the last target was a box.
        /** The function retrieves if the last target was a box.
         @return true if the last target was a box, otherwise false.
         */
        inline bool knockHasHitBox() const noexcept
        {
            return m_target == Box;
        }
        
        //! Retrieve if the last target was a link.
        /** The function retrieves if the last target was a link.
         @return true if the last target was a link, otherwise false.
         */
        inline bool knockHasHitLink() const noexcept
        {
            return m_target == Link;
        }
        
        //! Retrieve if the last target was a page.
        /** The function retrieves if the last target was a page.
         @return true if the last target was a page, otherwise false.
         */
        inline bool knockHasHitPage() const noexcept
        {
            return m_target == Page;
        }
        
        //! Retrieve the box that has been touched.
        /** The function retrieves the box that has been touched.
         @return The box that has been touched if the last target was a box.
         */
        inline sBox knockGetBox() const noexcept
        {
            if(m_target == Box)
            {
                return m_box.lock();
            }
            return nullptr;
        }
        
        //! Retrieve the link that has been touched.
        /** The function retrieves the link that has been touched.
         @return The link that has been touched if the last target was a link.
         */
        inline sLink knockGetLink() const noexcept
        {
            if(m_target == Link)
            {
                return m_link.lock();
            }
            return nullptr;
        }
        
        //! Retrieve the page that has been touched.
        /** The function retrieves the page that has been touched.
         @return The page that has been touched if the last target was a page.
         */
        inline sPage knockGetPage() const noexcept
        {
            if(m_target == Page)
            {
                return m_page.lock();
            }
            return nullptr;
        }
        
        //! Retrieve the part of the target that has been touched.
        /** The function retrieves the part of the target that has been touched. If a box has been touched, it can be one of all the parts. If a link has been touched, it can only be the four first parts. If a page has been touched, it can be inside or outside. It nothing has been touched yet, it will be outside.
         @return The part of the target that has been touched.
         */
        inline Part knockGetPart() const noexcept
        {
            if(m_target == Box)
            {
                return m_part;
            }
            else if(m_target == Link)
            {
                return clip(m_part, Outside, Outlet);
            }
            else if(m_target == Page)
            {
                return clip(m_part, Outside, Inside);
            }
            return Outside;
        }
		
		//! Retrieve the type of corner if a corner of a box has been touched.
		/** The function retrieves the type of corner if a corner of a box has been touched.
		 @return The type of corner if a corner of a box has been touched.
		 */
		inline enum Corner knockGetCorner() const noexcept
		{
			if(m_target == Box)
			{
				return m_corner;
			}
			return none;
		}
		
        //! Retrieve the index of the part of the box.
        /** The function retrieves the index of the part of the box. If the part is an inlet or an outlet, it will be their index. If the part is a border or a corner it will be their position.
         @return The index of the part of the box.
         */
        inline ulong knockGetIndex() const noexcept
        {
            if(m_target == Box)
            {
                return m_index;
            }
            return 0;
        }
    };
    
    // ================================================================================ //
    //										IOLET MAGNET                                //
    // ================================================================================ //
    
    class IoletMagnet
    {
    private:
        const wPage     m_page;
        wBox            m_box;
        ulong   m_index;
        
    public:
        
        //! The contructor.
        /** The contructor initialize an empty iolet magnet.
         @param page The page that owns the iolet magnet.
         */
        IoletMagnet(sPage page) noexcept;
        
        //! The destructor.
        /** The destructor does nothing.
         */
        ~IoletMagnet();
        
        //! Find the closest iolet to a point.
        /** The function finds in a page the closest iolet to a page.
         @param point The point to compute.
         @param box  The box that won't be check (the box that owns the link).
         @param inlet If true function will look for an inlet, otherwise it will look for an outlet.
         @param distance The maximum distance to for.
         @return True if an iolet has been find.
         */
        bool magnetFindIolet(Point const& point, sBox box, bool inlet, double const distance);
        
        //! Retrieve the box that has been found.
        /** The function retrieves the box that has been found.
         @return The box that has been found.
         */
        inline sBox magnetGetBox() const noexcept
        {
            return m_box.lock();
        }
        
        //! Retrieve the index of the iolet.
        /** The function retrieves the index of the iolet.
         @return The index of the iolet.
         */
        inline ulong magnetGetIndex() const noexcept
        {
            return m_index;
        }
        
    };
    
    // ================================================================================ //
    //										LASSO                                       //
    // ================================================================================ //
    
    class Lasso
    {
    private:
        const wPage                 m_page;
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
        Lasso(sPage page) noexcept;
        
        //! Destructor.
        /** You should never have to use this method.
         */
        virtual ~Lasso();
        
        //! The lasso creation method.
        /** The function allocates a lasso.
         @param page The page that used the lasso.
         */
        template<class LassoClass, class ...Args> static shared_ptr<LassoClass> create(sPage page, Args&& ...arguments)
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
        
        //! Retrieve the notification that the bounds has changed.
        /** The function retrieves the notification that the bounds has changed.
         */
        virtual void boundsHasChanged() = 0;
    };
    
    // ================================================================================ //
    //                                 IOLET HIGHLIGHTER                                //
    // ================================================================================ //
    
    class IoletHighlighter
    {
    private:
        Iolet::Polarity m_polarity;
        Rectangle  m_bounds;
    public:
        //! Contructor.
        /** You should never have to use this method.
         */
        IoletHighlighter() noexcept;
        
        //! Destrcutor.
        /** You should never have to use this method.
         */
        virtual ~IoletHighlighter();
        
        //! The lasso creation method.
        /** The function allocates a lasso.
         @param page The page that use the lasso.
         */
        template<class IoletHighlighterClass, class ...Args> static shared_ptr<IoletHighlighterClass> create(Args&& ...arguments)
        {
            return make_shared<IoletHighlighterClass>(forward<Args>(arguments)...);
        }
        
        //! Defines an inlet to be highlighted.
        /** The function defines an inlet to be highlighted.
         @param box The box that owns the inlet.
         @param index The index of the inlet.
         */
        void highlightInlet(sBox box, ulong index);
        
        //! Defines an outlet to be highlighted.
        /** The function defines an outlet to be highlighted.
         @param box The box that owns the outlet.
         @param index The index of the outlet.
         */
        void highlightOutlet(sBox box, ulong index);
        
        //! The draw method that could be override.
        /** The function draws the iolet hightlighter in a doodle.
         @param d The doodle.
         */
        virtual void draw(Doodle& d);
        
        //! Retrieve the polarity of the hightlighter.
        /** The function retrieves the polarity of the hightlighter.
         @return The the polarity of the hightlighter.
         */
        inline Iolet::Polarity getPolarity() const noexcept
        {
            return m_polarity;
        }
        
        //! Retrieve the bounds of the lasso.
        /** The function retrieves the bounds of the lasso.
         @return The bounds of the lasso.
         */
        inline Rectangle getBounds() const noexcept
        {
            return m_bounds;
        }
        
        //! Retrieve the notification that the bounds has changed.
        /** The function retrieves the notification that the bounds has changed.
         */
        virtual void boundsHasChanged() = 0;
    };
	
	// ================================================================================ //
	//										BOX RESIZER									//
	// ================================================================================ //
	
	class BoxResizer : public Attr::Listener
	{
	private:
		wBox		m_box;
		Rectangle	m_bounds;
	public:
		
		//! Contructor.
		/** You should never have to use this method.
		 */
		BoxResizer(sBox box) noexcept;
		
		//! Destrcutor.
		/** You should never have to use this method.
		 */
		virtual ~BoxResizer();
		
		//! The box resizer creation method.
		/** The function allocates a box resizer.
		 @param box The box on which that resizer must be attached.
		 */
		template<class BoxResizerClass, class ...Args> static shared_ptr<BoxResizerClass> create(sBox box, Args&& ...arguments)
		{
			shared_ptr<BoxResizerClass> resizer;
			
			if (box)
			{
				resizer = make_shared<BoxResizerClass>(box, forward<Args>(arguments)...);
				box->bind(resizer);
				resizer->m_bounds = box->getBounds().expanded(6);
				resizer->boundsHasChanged();
			}
			
			return resizer;
		}
		
		void notify(Attr::sManager manager, sAttr attr, Attr::Notification type) override;
		
		//! The draw method that could be overrided.
		/** The function draws the box resizer in a doodle.
		 @param d The doodle.
		 */
		virtual void draw(Doodle& d);
		
		//! Retrieve if the box resizer is touch by a point.
		/** The function retrieves if the box resizer is touch by a point and fill the knock with the knock informations.
		 @param point The point.
		 @param knock The knock.
		 @return true if the box resizer is touch by the point, otherwise false.
		 */
		virtual bool contains(Point const& point, Knock& knock) const noexcept;
		
		//! Retrieve the bounds of the box resizer.
		/** The function retrieves the bounds of the box resizer.
		 @return The bounds of the box resizer.
		 */
		inline Rectangle getBounds() const noexcept
		{
			return m_bounds;
		}
		
		//! Retrieve the notification that the bounds has changed.
		/** The function retrieves the notification that the bounds has changed.
		 */
		virtual void boundsHasChanged() = 0;
		
		/** Represents the different zones of a resizable box, which allow it to resize in different ways.
		 */
		class Zone
		{
		private:
			int zone;
			
		public:
			enum Zones
			{
				centre  = 0,
				left    = 1,
				top     = 2,
				right   = 4,
				bottom  = 8
			};
			
			/** Creates a Zone from a combination of the flags in \enum Zones. */
			explicit Zone (int zoneFlags) noexcept;
			
			Zone() noexcept;
			Zone(const Zone&) noexcept;
			Zone& operator=(const Zone&) noexcept;
			
			bool operator==(const Zone&) const noexcept;
			bool operator!=(const Zone&) const noexcept;
			
			/** Returns an appropriate mouse-cursor for this resize zone. */
			//MouseCursor getMouseCursor() const noexcept;
			
			/** Returns true if dragging this zone will move the entire object without resizing it. */
			bool isDraggingWholeBox() const noexcept
			{
				return zone == centre;
			}
			
			/** Returns true if dragging this zone will move the object's left edge. */
			bool isDraggingLeftEdge() const noexcept
			{
				return (zone & left) != 0;
			}
			
			/** Returns true if dragging this zone will move the object's right edge. */
			bool isDraggingRightEdge() const noexcept
			{
				return (zone & right) != 0;
			}
			
			/** Returns true if dragging this zone will move the object's top edge. */
			bool isDraggingTopEdge() const noexcept
			{
				return (zone & top) != 0;
			}
			
			/** Returns true if dragging this zone will move the object's bottom edge. */
			bool isDraggingBottomEdge() const noexcept
			{
				return (zone & bottom) != 0;
			}
			
			/** Resizes this rectangle by the given amount, moving just the edges that this zone
			 applies to.
			 */
			Rectangle resizeRectangleBy (Rectangle original, const Point& distance) const noexcept
			{
				if (isDraggingWholeBox())
					return original + distance;
				/*
				if (isDraggingLeftEdge())
					original.setLeft (jmin (original.getRight(), original.getX() + distance.x));
					
				if (isDraggingRightEdge())
					original.setWidth (jmax (ValueType(), original.getWidth() + distance.x));
					
				if (isDraggingTopEdge())
					original.setTop (jmin (original.getBottom(), original.getY() + distance.y));
				
				if (isDraggingBottomEdge())
					original.setHeight (jmax (ValueType(), original.getHeight() + distance.y));
				*/
				
				return original;
			}
			/** Returns the raw flags for this zone. */
			int getZoneFlags() const noexcept               { return zone; }
		};
		
		void resizeComponentFromResizerStart();
		//void resizeComponentFromResizer (int zoneFlags, const MouseEvent& e);
		void resizeComponentFromResizerEnd();
		
		/** Returns the zone in which the mouse was last seen. */
		Zone getCurrentZone() const noexcept                { return m_mouseZone; }
		
		int getSquareSize() const noexcept					{ return m_square_size; }
		
		/** Returns the square resizer size. */
		void setSquareSize(int size);
		
	private:
		Rectangle									m_originalBounds;
		Zone										m_mouseZone;
		
		int											m_square_size;
		bool										m_show_resizer;
		
		Rectangle		m_bottomRightRect;
		Rectangle		m_topLeftCornerRect, m_topRightCornerRect, m_bottomRightCornerRect, m_bottomLeftCornerRect;
		Rectangle		m_topRect, m_rightRect, m_bottomRect, m_leftRect;
		
		//void updateMouseZone(const MouseEvent&);
		Zone getZoneFromPos(Point pos);
		//void drawSquareResizer(Graphics& g, Rectangle<int>& rect, Colour& color);
	};
}


#endif


