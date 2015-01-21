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
    class Link : public AttrLink, public Attr::Listener
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
        Link(sPage page, sBox from, ulong outlet, sBox to, ulong inlet) noexcept;
        
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
        static sLink create(sPage page, const sBox from, const unsigned outlet, const sBox to, const unsigned inlet);
        
        //! The link creation method with a dico.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(sPage page, scDico dico);
        
        //! The link creation method with another link but change one of the boxes with another one.
        /** The function allocates a link with another link.
         @param link The other link.
         @param oldbox  The old box to replace.
         @param newbox  The newbox box that replace.
         @return The link.
         */
        static sLink create(scLink link, const sBox oldbox, const sBox newbox);
        
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
        
        //! Retrieve if the link owns the boxes.
        /** The function retrieves if the link owns the two boxes.
         @return true if the link owns the two boxes, otherwise false.
         */
        inline bool hasBoxes() const noexcept
        {
            return m_box_from.expired() && m_box_to.expired();
        }
        
        //! Retrieve if the link owns the output box.
        /** The function retrieves if the link owns the output box.
         @return true if the link owns the output box, otherwise false.
         */
        inline bool hasBoxFrom() const noexcept
        {
            return m_box_from.expired();
        }
        
        //! Retrieve if the link owns the input box.
        /** The function retrieves if the link owns the input box.
         @return true if the link owns the input box, otherwise false.
         */
        inline bool hasBoxTo() const noexcept
        {
            return m_box_to.expired();
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
        
        //! Retrieve the outlet of the link.
        /** The function retrieves the outlet of the link.
         @return The outlet of the link.
         */
        inline sOutlet getOutlet() const noexcept
        {
            return nullptr;
        }
        
        //! Retrieve the inlet of the link.
        /** The function retrieves the inlet of the link.
         @return The inlet of the link.
         */
        inline sInlet getInlet() const noexcept
        {
            return nullptr;
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
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager		The manager that manages the attribute.
         @param attr		The attribute that has been modified.
         @param type		The type of notification.
         */
        void notify(Attr::sManager manager, sAttr attr, Attr::Notification type);
        
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
        
        static const sTag Tag_from;
        static const sTag Tag_to;
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


