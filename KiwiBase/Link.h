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
    class Link : public AttrLink
    {
    public:
        class Controller;
        typedef shared_ptr<Controller>   sController;
        typedef weak_ptr<Controller>     wController;
        
    private:
        wPage           m_page;
        wBox			m_box_from;
        wBox			m_box_to;
        ulong           m_index_outlet;
        ulong           m_index_intlet;
        Gui::Path       m_path;
        wController		m_controller;
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(const sPage page, const sBox from, const ulong outlet, const sBox to, const ulong inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Link();
        
        //! The link creation method with a dico.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(sPage page, scDico dico);
        
        //! Retrieve the sLink.
        /** The function sLink.
         @return The sLink.
         */
		inline sLink getShared() noexcept
		{
			return static_pointer_cast<Link>(shared_from_this());
		}
        
        //! Retrieve the scLink.
        /** The function scLink.
         @return The scLink.
         */
		inline scLink getShared() const noexcept
		{
			return static_pointer_cast<const Link>(shared_from_this());
		}
        
        //! Retrieve the page of the link.
        /** The function retrieves the page of the link.
         @return The page of the link.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
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
        inline ulong getOutletIndex() const noexcept
        {
            return m_index_outlet;
        }
        
        //! Retrieve the index of the inlet of the link.
        /** The function retrieves the index of the inlet of the link.
         @return The index of the inlet of the link.
         */
        inline ulong getInletIndex() const noexcept
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
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept;
        
        //! Retrieve the position of the link.
        /** The function retrieves the position of the link as a point.
         @return The position of the link as a point.
         */
        inline Gui::Point getPosition() const noexcept
        {
            return m_path.getPosition();
        }
        
        //! Retrieve the size of the link.
        /** The function retrieves the size of the link as a point.
         @return The size of the link as a point.
         */
        inline Gui::Point getSize() const noexcept
        {
            return m_path.getSize();
        }
        
        //! Retrieve the bounds of the link.
        /** The function retrieves the bounds of the link as a rectangle.
         @return The bounds of the link as a rectangle.
         */
        inline Gui::Rectangle getBounds() const noexcept
        {
            return m_path.getBounds();
        }
        
        //! Retrieve the path of the link.
        /** The function retrieves the path of the link.
         @return The path of the link.
         */
        inline void getPath(Gui::Path &path) const noexcept
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
            
            //! Retrieve the link.
            /** The funtion retrieves the link.
             @return The link.
             */
            sLink getLink() const noexcept
            {
                return m_link;
            }
			
            //! The bounds notification function that should be override.
            /** The function is called by the link when its bounds changed.
             */
            virtual void boundsChanged() = 0;
        };
    };
    
    static bool operator==(scLink link, scBox box) noexcept
    {
        if(link && box)
        {
            return link->getBoxFrom() == box || link->getBoxTo() == box;
        }
        else
        {
            return false;
        }
    }
    
    static bool operator==(sLink link, sBox box) noexcept
    {
        if(link && box)
        {
            return link->getBoxFrom() == box || link->getBoxTo() == box;
        }
        else
        {
            return false;
        }
    }
    
}


#endif


