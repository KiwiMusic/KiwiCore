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

#include "Tools.h"

namespace Kiwi
{
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
            TopLeft     = 0,
            TopRight    = 1,
            BottomLeft  = 2,
            BottomRight = 3
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
        wPage           m_page;
        wBox            m_box;
        wLink           m_link;
        Target          m_target= Nothing;
        Part            m_part  = Outside;
        unsigned long   m_index = 0;
        
    public:
        
        //! The contructor.
        /** The contructor initialize an empty hit.
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
         @param page The page to test.
         @param point The point.
         */
        void knockAll(Point const& point) noexcept;
        
        //! Test a point.
        /** The function try to find a ink under the point.
         @param page The page to test.
         @param point The point.
         */
        void knockBoxes(Point const& point) noexcept;
        
        //! Test a point.
        /** The function try to find a box .
         @param page The page to test.
         @param point The point.
         */
        void knockLinks(Point const& point) noexcept;
        
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
        
        //! Retrieve the index of the part of the box.
        /** The function retrieves the index of the part of the box. If the part is an inlet or an outlet, it will be their index. If the part is a border or a corner it will be their position.
         @return The index of the part of the box.
         */
        inline unsigned long knockGetIndex() const noexcept
        {
            if(m_target == Box)
            {
                return m_index;
            }
            return 0;
        }
    };
}


#endif


