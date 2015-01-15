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
    public:
        enum Mode
        {
            Move        = 0,
            Linear      = 1,
            Quadratic   = 2,
            Cubic       = 3
        };
        
        struct Node
        {
            Point point;
            Mode  mode;
        };
        
    private:
        vector<Node> m_points;
        
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
        
        //! Retrieve the number of nodes of the path.
        /** The function retrieves the number of nodes of the path.
         @return The number of nodes of the path.
         */
        ulong size() const noexcept
        {
            return (ulong)m_points.size();
        }
        
        //! Retrieve if the path is empty.
        /** The function retrieves if the path is empty.
         @return True if the path is empty, otherwise false.
         */
        bool empty() const noexcept
        {
            return m_points.empty();
        }
        
        //! Retrieve the node of the path at a defined index.
        /** The function retrieves the node of the path at a defined index.
         @param  index The index of the node.
         @return The node.
         */
        Node getNode(ulong index) const noexcept
        {
            if(index < (ulong)m_points.size())
            {
                return m_points[(vector<Node>::size_type)index];
            }
            else
            {
                return {Point(0., 0.), Move};
            }
        }
        
        //! Retrieve the mode of the path at a defined index.
        /** The function retrieves the mode of the path at a defined index.
         @param  index The index of the mode.
         @return The mode.
         */
        Mode getMode(ulong index) const noexcept
        {
            if(index < (ulong)m_points.size())
            {
                return m_points[(vector<Node>::size_type)index].mode;
            }
            else
            {
                return Move;
            }
        }
        
        //! Retrieve a point of the path at a defined index.
        /** The function retrieves a point of the path at a defined index.
         @param  index The index of the point.
         @return The point.
         */
        Point getPoint(ulong index) const noexcept
        {
            if(index < (ulong)m_points.size())
            {
                return m_points[(vector<Node>::size_type)index].point;
            }
            else
            {
                return Point(0., 0.);
            }
        }
        
        //! Clear the path.
        /** The function clears a point to the path.
         */
        void clear() noexcept;
        
        //! Add a node to the path that won't be linked to the previous point.
        /** The function adds a node to the path that won't be linked to the previous node.
         @param point The point to add.
         */
        void moveTo(Point const& point) noexcept;
        
        //! Add a node to the path that will be linked to the previous point linearly.
        /** The function adds a node to the path that will be linked to the previous node linearly.
         @param point The point to add.
         */
        void lineTo(Point const& point) noexcept;
        
        //! Add a node to the path that will be linked to the previous node with a quadratic bezier curve.
        /** The function adds a node to the path that will be linked to the previous node with a quadratic bezier curve.
         @param control The control point.
         @param end     The end point.
         */
        void quadraticTo(Point const& control, Point const& end);
        
        //! Add a node to the path that will be linked to the previous node with a cubic bezier curve.
        /** The function adds a node to the path that will be linked to the previous node with a cubic bezier curve.
         @param control1 The first control point.
         @param control2 The seconf control point.
         @param end     The end point.
         */
        void cubicTo(Point const& control1, Point const& control2, Point const& end);
		
		//! Sets a point of the path.
		/** The function sets a point of the path.
		 @param index The index of the point to replace.
		 @param The new point.
		 */
		void setPoint(ulong index, Point const& pt) noexcept;
        
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
        
        bool contains(Point const& point, double const tolerance) const noexcept;
        
        bool near(Point const& pt, double const distance) const noexcept;
        
        bool overlaps(Rectangle const& rect) const noexcept;
    };
}

#endif