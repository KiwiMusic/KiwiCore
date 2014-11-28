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

#ifndef __DEF_KIWI_BOX__
#define __DEF_KIWI_BOX__

#include "Iolets.h"
#include "AttributeBox.h"
#include "Event.h"
#include "Doodle.h"
#include "Beacon.h"

// TODO
// - See how to format the expression
// - Box should deletes it owns links at deletion
namespace Kiwi
{    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical box.
    /**
     The box is a graphical class that aims to be instantiate in a page.
     */
	class Box : public AttrBox
    {
    public:
        class Controler;
        typedef shared_ptr<Controler>   sControler;
        typedef weak_ptr<Controler>     wControler;
        
        enum Behavior
        {
            Signal      = 1<<1,
            Mouse       = 1<<2,
            Keyboard    = 1<<3,
            Graphic     = 1<<4
        };
        
    private:
        friend bool Link::connect() noexcept;
        friend bool Link::disconnect() noexcept;
        
        const wInstance     m_instance;
        const wPage         m_page;
        const sTag          m_name;
        const unsigned long m_id;
        const unsigned long m_type;
        sTag                m_text;
        
        vector<uOutlet>     m_outlets;
        vector<uInlet>      m_inlets;
        atomic_ullong       m_stack_count;
        mutable mutex       m_io_mutex;
        
        wControler          m_controler;
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Box(sPage page, string const& name, unsigned long type = 1<<0);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Box();
        
        //! The box creation method.
        /** The function allocates a box and initialize the defaults members.
         */
        static sBox create(sPage page, sDico dico);
		
        //! Retrieve the sBox.
        /** The function sBox.
         @return The sBox.
         */
		inline sBox getShared() noexcept
		{
			return static_pointer_cast<Box>(shared_from_this());
		}
        
        //! Retrieve the scBox.
        /** The function scBox.
         @return The scBox.
         */
		inline scBox getShared() const noexcept
		{
			return static_pointer_cast<const Box>(shared_from_this());
		}
        
        //! Retrieve the instance that manages the page of the box.
        /** The function retrieves the instance that manages the page of the box.
         @return The instance that manages the page of the box.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
        
        //! Retrieve the page that manages the box.
        /** The function retrieves the page that manages the box.
         @return The page that manages the box.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
        //! Retrieve the controler that manages the box.
        /** The function retrieves the controler that manages the box.
         @return The controler that manages the box.
         */
        inline sControler getControler() const noexcept
        {
            return m_controler.lock();
        }
        
        //! Retrieve the name of the box.
        /** The function retrieves the name of the box as a tag.
         @return The name of the box as a tag.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the text of the box.
        /** The function retrieves the text of the box as a tag.
         @return The text of the box as a tag.
         */
        inline unsigned long getId() const noexcept
        {
            return m_id;
        }
        
        //! Retrieve the type of the box.
        /** The function retrieves the type of the box.
         @return The type of the box as a tag.
         */
        inline unsigned long getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the text of the box.
        /** The function retrieves the text of the box as a tag.
         @return The text of the box as a tag.
         */
        inline sTag getText() const noexcept
        {
            return m_text;
        }
        
        //! Retrieve the expression of the box.
        /** The function retrieves the expression of the box as a string.
         @return The expression of the box as a string.
         */
        virtual string getExpression() const noexcept
        {
            return "error";
        }
        
        //! Retrieve the number of inlets of the box.
        /** The functions retrieves the number of inlets of the box.
         @return The number of inlets.
         */
        inline unsigned long getNumberOfInlets() const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            return (unsigned long)m_inlets.size();
        }
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The inlet index.
         @return The description.
         */
        inline string getInletDescription(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_inlets.size())
            {
                return m_inlets[(vector<uInlet>::size_type)index]->getDescription();
            }
            else
            {
                return "";
            }
        }
        
        //! Retrieve the type of an inlet.
        /** The functions retrieves the type of an inlet.
         @param index The inlet index.
         @return The type.
         */
        inline Inlet::Type getInletType(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_inlets.size())
            {
                return m_inlets[(vector<uInlet>::size_type)index]->getType();
            }
            else
            {
                return Inlet::Type::Error;
            }
        }
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        inline unsigned long getNumberOfOutlets() const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            return (unsigned long)m_outlets.size();
        }
        
        //! Retrieve the description of an outlet.
        /** The functions retrieves the description of an outlet.
         @param index The index of the outlet.
         @return The descrition.
         */
        inline string getOutletDescription(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_outlets.size())
            {
                return m_outlets[(vector<uOutlet>::size_type)index]->getDescription();
            }
            else
            {
                return "";
            }
        }
        
        //! Retrieve the type of an inlet.
        /** The functions retrieves the type of an inlet.
         @param index The inlet index.
         @return The type.
         */
        inline Outlet::Type getOutletType(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_outlets.size())
            {
                return m_outlets[(vector<uOutlet>::size_type)index]->getType();
            }
            else
            {
                return Outlet::Type::Error;
            }
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if the vector of element doesn't match with your method then the box will check if the vector match with attributes methods, othersize return true.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(unsigned long index, ElemVector const& elements)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if you don't want the mouse event then the box manager will notify other mouse listener if needed, othersize return true.
         @param event    A mouse event.
         */
        virtual bool receive(Event::Mouse const& event)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if you don't want the keyboard event then the box manager will notifiy other keyboard listener if needed, othersize return true.
         @param event    A keyboard event.
         */
        virtual bool receive(Event::Keyboard const& event)
        {
            return false;
        }
        
        //! The paint method that should be override.
        /** The function shoulds draw some stuff in the doodle. Return false if you don't want to draw then the box manager will draw the text of the box, othersize return true.
         @param doodle    A doodle to draw.
         */
        virtual bool draw(Doodle& doodle) const
        {
            return false;
        }
        
        //! Write the box in a dico.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
    protected:
        
        //! Send a notification to the page that the box should be redraw.
        /** The function sends a notification to the page that the box should be redraw.
         */
        void    redraw() const noexcept;
        
        //! Send a vector of elements via an outlet.
        /** The function sends a vector of elements via an outlet and dispatches it to all the connected inlets.
         @param index The index of the outlet.
         @param elements A list of elements to pass.
         */
        void    send(unsigned long index, ElemVector const& elements) const noexcept;
        
        //! Add a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    addInlet(Inlet::Type type, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param index The index of the inlet;
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    insertInlet(unsigned long index, Inlet::Type type, string const& description = "");
        
        //! Remove an inlet from the box.
        /** The function removes an inlet from the box.
         @param index The index of the inlet
         */
        void    removeInlet(unsigned long index);
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    addOutlet(Outlet::Type type, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @pram index The index of the outlet.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    insertOutlet(unsigned long index, Outlet::Type type, string const& description = "");
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The index of the outlet.
         */
        void    removeOutlet(unsigned long index);

    private:
        
        //! The virtual constructor called by the page to create an instance of box.
        /** The function retrieves an instance of the box.
         @param dico        The dico that defines the box.
         */
        virtual sBox allocate(sPage page, sDico dico) const = 0;
        
        //! The write method that should be override.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        virtual void save(sDico dico) const
        {
            
        }
        
        //! The read method that should be override.
        /** The function reads a dico to initalize the boxe.
         @param dico The dico.
         */
        virtual void load(scDico dico)
        {
            
        }
		
		//! Notify the manager that the values of an attribute has changed.
		/** The function notifies the manager that the values of an attribute has changed.
		 @param attr The attribute that has changed.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		bool attributeValueChanged(sAttr attr);
        
        //! Connect an inlet to a box's outlet.
        /** The function connects an inlet to a box's outlet. Note that the link of the inlet isn't very necessary, it only to  facilitate the retrieving of the input boxes.
         @param link  The link.
         @return true if the link has been done, otherwise false.
         */
        bool connectInlet(sLink link);
        
        //! Connect an outlet to a box's inlet.
        /** The function connects an outlet to a box's inlet. This part of the link is the only one really important, because this is the one used by the send method.
         @param link  The link.
         @return true if the link has been done, otherwise false.
         */
        bool connectOutlet(sLink link);
        
        //! Disconnect an inlet to a box's outlet.
        /** The function disconnects an inlet to a box's outlet. Note that the link of the inlet isn't very necessary, it only to  facilitate the retrieving of the input boxes.
         @param link  The link.
         @return true if the link has been removed, otherwise false.
         */
        bool disconnectInlet(sLink link);
        
        //! Disconnect an outlet to a box's inlet.
        /** The function disconnects an outlet to a box's inlet. This part of the dislink is the only one really important, because this is the one used by the send method.
         @param link  The link.
         @return true if the link has been removed, otherwise false.
         */
        bool disconnectOutlet(sLink link);
        
        //! Set the controler of the box.
        /** The function sets the controler of the box.
         @param ctrl    The controler.
         */
        void setControler(sControler ctrl);
        
    public:
        
        // ================================================================================ //
        //                                  BOX CONTROLER                                   //
        // ================================================================================ //
        
        //! The box controler .
        /**
         The box controler...
         */
        class Controler
        {
        public:
            enum Type
            {
                Inside  = 0,
                Inlet   = 1,
                Outlet  = 2,
                Corner  = 3,
                Border  = 4
            };
            
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
            
            struct Hit
            {
                Type            type;
                unsigned long   index;
            };
            
        private:
            
            const sBox  m_box;
            const bool  m_want_mouse_focus;
            const bool  m_want_keyboard_focus;
            
            bool    m_edition;
            bool    m_selected;
        public:
            
            //! Constructor.
            /** You should never call this method except if you really know what you're doing.
             */
            Controler(sBox box) :
            m_box(box),
            m_want_mouse_focus(box->getType() & Box::Mouse),
            m_want_keyboard_focus(box->getType() & Box::Keyboard),
            m_edition(true),
            m_selected(false)
            {
                ;
            }
            
            //! The destructor.
            /** You should never call this method except if you really know what you're doing.
             */
            virtual ~Controler()
            {
                ;
            }
            
            //! The controler maker.
            /** The function creates an controler with arguments.
             */
            template<class CtrlClass, class ...Args> static shared_ptr<CtrlClass> create(Args&& ...arguments)
            {
                shared_ptr<CtrlClass> ctrl = make_shared<CtrlClass>(forward<Args>(arguments)...);
                if(ctrl && ctrl->m_box)
                {
                    ctrl->m_box->setControler(ctrl);
                }
                return ctrl;
            }
            
            //! Retrieve the box.
            /** The funtion retrieves the box.
             @return The box.
             */
            inline sBox getBox() const noexcept
            {
                return m_box;
            }
            
            //! Retrieve the text of the box.
            /** The funtion retrieves the text of the box.
             @return The text of the box.
             */
            inline string getText() const noexcept
            {
                return toString(m_box->getText());
            }
            
            //! Retrieve if the page is in edition.
            /** The function retrieves if the page is in edition.
             @param true if the page is in edition, otherwise false.
             */
            inline bool getEditionStatus() const noexcept
            {
                return m_edition;
            }
            
            //! Retrieve if the box is selected.
            /** The function retrieves if the box is selected.
             @param true if the box is selected, otherwise false.
             */
            inline bool getSelectedStatus() const noexcept
            {
                return m_selected;
            }
            
            //! Retrieve if the box wants the mouse focus.
            /** The function retrieves if the box wants the mouse focus.
             @return true if the box wants the mouse focus otherwise false.
             */
            inline bool isMouseListener() const noexcept
            {
                return m_want_mouse_focus;
            }
            
            //! Retrieve if the box wants the keyboard focus.
            /** The function retrieves if the box wants the keyboard focus.
             @return true if the box wants the keyboard focus otherwise false.
             */
            inline bool isKeyboardListener() const noexcept
            {
                return m_want_keyboard_focus;
            }
            
            //! Retrieve the position of an inlet.
            /** The function retrieves the position of an inlet.
             @param index The index of the inlet.
             @return the position of the inlet as a point.
             */
            Point getInletPosition(unsigned long index) const noexcept;
            
            //! Retrieve the position of an outlet.
            /** The function retrieves the position of an outlet.
             @param index The index of the outlet.
             @return the position of the outlet as a point.
             */
            Point getOutletPosition(unsigned long index) const noexcept;
            
            //! Retrieve if the box is hit by a point.
            /** The function retrieves if the box is hit by a point.
             @return true if the box is hit by a point otherwise false.
             */
            virtual bool isHit(Point const& pt, Hit& hit) const noexcept;
            
            //! Notify that the page is in edition.
            /** The function notifies that page is in edition to redraw the box.
             @param true if page is in edition, otherwise false.
             */
            void setEditionStatus(bool status);
            
            //! Notify that the box is selected.
            /** The function notifies that the box is selected to redraw the box.
             @param true if the box is selected, otherwise false.
             */
            void setSelectedStatus(bool status);
            
            //! The inlets notification function that should be override.
            /** TThe function is called by the box when its inlets changed.
             */
            void inletsChanged();
            
            //! The outlets notification function that should be override.
            /** TThe function is called by the box when its outlets changed.
             */
            void outletsChanged();
            
            //! The position notification function that should be override.
            /** The function is called by the box when its position changed.
             */
            virtual void positionChanged() = 0;
            
            //! The size notification function that should be override.
            /** The function is called by the box when its size changed.
             */
            virtual void sizeChanged() = 0;
            
            //! The redraw function that should be override.
            /** The function is called by the box when it should be repainted.
             */
            virtual void redraw() = 0;
            
            //! The default paint method.
            /** The default function paint a default box with the background, border, inlets, outlets and text.
             @param paper       A doodle to draw.
             @param edit        If the page is in edition mode.
             @param selected    If the box is selected
             */
            static void paint(sBox box, Doodle& d, bool edit = false, bool selected = false);
        };
        
        // ================================================================================ //
        //                                      BOX FACTORY                                 //
        // ================================================================================ //
        
    private:
        
        static map<sTag, unique_ptr<Box>>  m_prototypes;
        static mutex m_prototypes_mutex;
    public:
    
        //! Box factory
        /** This function adds a new prototype of a box. If the prototype already exists, the function doesn't do anything otherwise the box is added to the prototype list.
         @param     box The prototype of the box.
         */
        static void addPrototype(unique_ptr<Box> box, string const& name = "");
    };
    
    inline string toString(scBox box)
    {
        return toString(box->getName());
    }
}


#endif


