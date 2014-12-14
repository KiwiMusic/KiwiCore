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

#ifndef __DEF_KIWI_LINK__
#define __DEF_KIWI_LINK__

#include "Attribute.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    //! The link is used to create a connection between boxes.
    /**
     The link is a combination of two sockets used to create the connection between boxes in a page.
     */
    class Link : public enable_shared_from_this<Link>
    {
    public:
        class Controller;
        typedef shared_ptr<Controller>   sController;
        typedef weak_ptr<Controller>     wController;
        
    private:
        const wBox			m_box_from;
        const wBox			m_box_to;
        const unsigned long m_index_outlet;
        const unsigned long m_index_intlet;
        Path				m_path;
        
        wController			m_controller;
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(const sBox from, const unsigned outlet, const sBox to, const unsigned inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Link();
        
        //! The link creation method with sockets.
        /** The function allocates a link, checks if the link is valid and returns it. If the link isn't valid it returns an invalid pointer.
         @param from    The box that send.
         @param outlet  The index of the outlet.
         @param to      The box that receive.
         @param inlet   The index of the inlet.
         @return The link.
         */
        static sLink create(const sBox from, const unsigned outlet, const sBox to, const unsigned inlet);
        
        //! The link creation method with a dico.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(scPage page, scDico dico);
        
        //! The link creation method with another link but change one of the boxes with another one.
        /** The function allocates a link with another link.
         @param link The other link.
         @param oldbox  The old box to replace.
         @param newbox  The newbox box that replace.
         @return The link.
         */
        static sLink create(scLink link, const sBox oldbox, const sBox newbox);
        
        //! Retrieve the output box.
        /** The function retrieves the output box of the link.
         @return The output box.
         */
        inline sBox getBoxFrom() const noexcept
        {
            return m_box_from.lock();
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the link.
         @return The input box.
         */
        inline sBox getBoxTo() const noexcept
        {
            return m_box_to.lock();
        }
        
        //! Retrieve the index of the outlet of the link.
        /** The function retrieves the index of the outlet of the link.
         @return The index of the outlet of the link.
         */
        inline unsigned long getOutletIndex() const noexcept
        {
            return m_index_outlet;
        }
        
        //! Retrieve the index of the inlet of the link.
        /** The function retrieves the index of the inlet of the link.
         @return The index of the inlet of the link.
         */
        inline unsigned long getInletIndex() const noexcept
        {
            return m_index_intlet;
        }
        
        //! Retrieve the controller that manages the box.
        /** The function retrieves the controller that manages the box.
         @return The controller that manages the box.
         */
        inline sController getController() const noexcept
        {
            return m_controller.lock();
        }
        
        //! Connect the link.
        /** The function connects link.
         @return True if the link has been connected, otherwise false.
         */
        bool connect() noexcept;
        
        //! Disconnect the link.
        /** The function disconnects link.
         @return True if the link has been disconnected, otherwise false.
         */
        bool disconnect() noexcept;
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept;
        
        //! Notify that the inlet has changed.
        /** The function is called by the inlet when it changed.
         */
        void inletChanged() noexcept;
        
        //! Notify that the outlet has changed.
        /** The function is called by the outlet when it changed.
         */
        void outletChanged() noexcept;
        
        //! Retrieve the position of the link.
        /** The function retrieves the position of the link as a point.
         @return The position of the link as a point.
         */
        inline Point getPosition() const noexcept
        {
            return m_path.getPosition();
        }
        
        //! Retrieve the size of the link.
        /** The function retrieves the size of the link as a point.
         @return The size of the link as a point.
         */
        inline Point getSize() const noexcept
        {
            return m_path.getSize();
        }
        
        //! Retrieve the bounds of the link.
        /** The function retrieves the bounds of the link as a rectangle.
         @return The bounds of the link as a rectangle.
         */
        inline Rectangle getBounds() const noexcept
        {
            return m_path.getBounds();
        }
        
        //! Retrieve the path of the link.
        /** The function retrieves the path of the link.
         @return The path of the link.
         */
        inline void getPath(Path &path) const noexcept
        {
            path = m_path;
        }
        
        //! Set the controller of the box.
        /** The function sets the controller of the box.
         @param ctrl    The controller.
         */
        void setController(sController ctrl);
        
        // ================================================================================ //
        //                                  LINK CONTROLER                                  //
        // ================================================================================ //
        
        //! The link controller .
        /**
         The link controller...
         */
        class Controller
        {
        private:
            const sLink		m_link;
			bool			m_selected;

        public:
			
            //! Constructor.
            /** You should never call this method except if you really know what you're doing.
             */
            Controller(sLink link) :
            m_link(link)
            {
                ;
            }
			
            //! The destructor.
            /** You should never call this method except if you really know what you're doing.
             */
            virtual ~Controller()
            {
                ;
            }
            
            //! The controller maker.
            /** The function creates an controller with arguments.
             */
            template<class CtrlClass, class ...Args> static shared_ptr<CtrlClass> create(Args&& ...arguments)
            {
                shared_ptr<CtrlClass> ctrl =  make_shared<CtrlClass>(forward<Args>(arguments)...);
                if(ctrl && ctrl->m_link)
                {
                    ctrl->m_link->setController(ctrl);
                }
                return ctrl;
            }
            
            //! Retrieve the link.
            /** The funtion retrieves the link.
             @return The link.
             */
            sLink getLink() const noexcept
            {
                return m_link;
            }
			
			//! Notify that the link is selected.
			/** The function notifies that the link is selected and needs to be redrawn.
			 @param status true if the link is selected, otherwise false.
			 */
			void setSelectedStatus(bool status);
			
			//! Retrieve if the link is selected.
			/** The function retrieves if the link is selected.
			 @param True if the link is selected, otherwise false.
			 */
			inline bool getSelectedStatus() const noexcept
			{
				return m_selected;
			}
            
            //! Retrieve if the link is touch by a point.
			/** The function retrieves if the link is touch by a point and fill the knock with the knock informations.
             @param point The point.
             @param knock The knock.
			 @return true if the link is touch by the point, otherwise false.
			 */
			virtual bool contains(Point const& point, Knock& knock) const noexcept;
            
            //! Retrieve if the link is overlaps the rectangle.
			/** The function retrieves if the link is overlaps the rectangle.
             @param rect The Rectangle.
			 @return true if the link overlaps the rectangle, otherwise false.
			 */
			virtual bool overlaps(Rectangle const& rect) const noexcept;
			
            //! The bounds notification function that should be override.
            /** The function is called by the link when its bounds changed.
             */
            virtual void boundsChanged() = 0;
            
            //! The redraw function that should be override.
            /** The function is called by the link when it should be repainted.
             */
            virtual void redraw() = 0;
            
            //! The default paint method.
            /** The default function paint a default link.
             @param link        The link to draw.
             @param d           The doodle to draw in.
             @param selected    If the link is selected
             */
            static void paint(sLink link, Doodle& d, bool selected = false);
        };
        
        // ================================================================================ //
        //                                  LINK CREATOR                                    //
        // ================================================================================ //
        
        static const sTag Tag_from;
        static const sTag Tag_to;
        
        static bool compareBoxToPositions(sLink link1, sLink link2);
    };
    
    
}


#endif


