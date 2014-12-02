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

#ifndef __DEF_KIWI_PATH__
#define __DEF_KIWI_PATH__

#include "Rectangle.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PATH                                        //
    // ================================================================================ //
    
    //! The path holds a set of points.
    /**
     The point is used to represent a set a segment in a space and allows several modification.
     */
    class Path
    {
    private:
        vector<Point> m_points;
        
    public:
        //! Constructor.
        /** The function initialize an empty path.
         */
        Path() noexcept;
        
        //! Constructor.
        /** The function initialize a path with another.
         */
        Path(Path const& path) noexcept;
        
        //! Constructor.
        /** The function initialize a point.
         */
        Path(Point const& pt) noexcept;
        
        //! Constructor.
        /** The function initialize a point a vector of elements.
         */
        Path(ElemVector const& elements) noexcept;
        
        //! Destructor.
        /** The function initialize another point.
         */
        ~Path();
        
        //! Add a point to the path.
        /** The function adds a point to the path.
         @param The point to add.
         */
        void add(Point const& pt) noexcept;
        
        //! Clear the path.
        /** The function clears a point to the path.
         */
        void clear() noexcept;
        
        //! Retrieve the number of points of the path.
        /** The function retrieves the number of points of the path.
         @return The number of points of the path.
         */
        unsigned long size() const noexcept
        {
            return (unsigned long)m_points.size();
        }
        
        //! Retrieve if the path is empty.
        /** The function retrieves if the path is empty.
         @return True if the path is empty, otherwise false.
         */
        bool empty() const noexcept
        {
            return m_points.empty();
        }
        
        //! Retrieve a point of the path.
        /** The function retrieves a point of the path.
         @param  index The index of the point.
         @return The point.
         */
        Point get(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_points.size())
            {
                return m_points[index];
            }
            else
            {
                return Point(0., 0.);
            }
        }
        
        //! Retrieve the position of the path.
        /** The function retrieves the position of the path. The position point will be the top left point of the smallest rectangle that contains all the points.
         @return The position of the path.
         */
        Point getPosition() const noexcept;
        
        //! Retrieve the size of the path.
        /** The function retrieves the size of the path. The size point will be the size of the smallest rectangle that contains all the points.
         @return The size of the path.
         */
        Point getSize() const noexcept;
        
        //! Retrieve the bounds of the path.
        /** The function retrieves the bounds of the path. The bounds rectangle is the smallest rectangle that contains all the points.
         @return The bounds of the path.
         */
        Rectangle getBounds() const noexcept;
        
        bool contains(Point const& pt) const noexcept;
        
        bool intersect(Point const& pt) const noexcept;
    };
}

#endif