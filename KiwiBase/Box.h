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

#include "Link.h"
#include "Attribute.h"
#include "Event.h"
#include "Doodle.h"
#include "Beacon.h"

// TODO
// - Box connection method (see where to put it and make it threadsafe)
// - Attributes (getFont, getSize, getPosition etc...) !!
// - See how to format the expression
namespace Kiwi
{
    class Instance;
    class Page;
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical box.
    /**
     The box is a graphical class that aims to be instantiate in a page.
     */
	class Box : public Attr::Manager, public enable_shared_from_this<Box>
    {
    public:
        class Listener;

        enum Behavior
        {
            Signal      = 1<<1,
            Mouse       = 1<<2,
            Keyboard    = 1<<3,
            Graphic     = 1<<4
        };
        
        class Inlet
        {
        public:
            enum Type
            {
                DataCold    = 0,
                DataHot     = 1,
                Signal      = 2,
                Error       = 3
            };
        private:
            
            friend class Box;
            vector<Socket>  m_sockets;
            const Type      m_type;
            const string    m_description;
        
            Inlet(Type type, string description) noexcept :
            m_type(type),
            m_description(description)
            {
                ;
            }
        public:
            ~Inlet()
            {
                m_sockets.clear();
            }
        };
        
        
        class Outlet
        {
        public:
            
            enum Type
            {
                Data    = 0,
                Signal  = 2,
                Error   = 3
            };
        private:
            
            friend class Box;
            vector<Socket>  m_sockets;
            const Type      m_type;
            const string    m_description;
            
            Outlet(Type type, string description) noexcept :
            m_type(type),
            m_description(description)
            {
                ;
            }
        public:
            ~Outlet()
            {
                m_sockets.clear();
            }
        };
        
    private:
        friend bool Link::connect() const noexcept;
        friend bool Link::disconnect() const noexcept;
        
        const wInstance             m_instance;
        const wPage                 m_page;
        const sTag                  m_name;
        const unsigned long         m_id;
        const unsigned long         m_type;
        sTag                        m_text;
        
        vector<unique_ptr<Outlet>>  m_outlets;
        vector<unique_ptr<Inlet>>   m_inlets;
        atomic_ullong               m_stack_count;
        mutable mutex               m_io_mutex;
        
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>   m_listeners;
        mutex                       m_listeners_mutex;
        
        sAttrPoint                  m_attr_position;
        sAttrPoint                  m_attr_size;
        
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
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPosition() const noexcept
        {
            return m_attr_position->get();
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getSize() const noexcept
        {
            return m_attr_size->get();
        }
        
        //! Retrieve the bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Rectangle getBounds() const noexcept
        {
            return Rectangle(m_attr_position->get(), m_attr_position->get());
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
                return m_inlets[(vector<unique_ptr<Inlet>>::size_type)index]->m_description;
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
                return m_inlets[(vector<unique_ptr<Inlet>>::size_type)index]->m_type;
            }
            else
            {
                return Inlet::Type::Error;
            }
        }
        
        //! Retrieve the sockets of an inlet.
        /** The functions retrieves the sockets of an inlet.
         @param index The inlet index.
         @param sockets A vetcor of socket to fill.
         */
        inline void getInletSockets(unsigned long index, vector<Socket>& sockets) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_inlets.size())
            {
                sockets = m_inlets[(vector<unique_ptr<Inlet>>::size_type)index]->m_sockets;
            }
            else
            {
                sockets.clear();
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
                return m_outlets[(vector<unique_ptr<Outlet>>::size_type)index]->m_description;
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
                return m_outlets[(vector<unique_ptr<Outlet>>::size_type)index]->m_type;
            }
            else
            {
                return Outlet::Type::Error;
            }
        }
        
        //! Retrieve the sockets of an outlet.
        /** The functions retrieves the sockets of an outlet.
         @param index The outlet index.
         @param sockets A vetcor of socket to fill.
         */
        inline void getOutletSockets(unsigned long index, vector<Socket>& sockets) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_outlets.size())
            {
                sockets = m_outlets[(vector<unique_ptr<Outlet>>::size_type)index]->m_sockets;
            }
            else
            {
                sockets.clear();
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
        void    redraw();
        
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
        
        //! Connect an inlet to a box's outlet.
        /** The function connects an inlet to a box's outlet. Note that the connection of the inlet isn't very necessary, it only to  facilitate the retrieving of the input boxes.
         @param inlet   The index of the inlet.
         @param box     The outlet's box.
         @param outlet  The index of the outlet.
         @return true if the connection has been done, otherwise false.
         */
        bool connectInlet(unsigned long inlet, sBox box, unsigned long outlet);
        
        //! Connect an outlet to a box's inlet.
        /** The function connects an outlet to a box's inlet. This part of the connection is the only one really important, because this is the one used by the send method.
         @param inlet   The index of the outlet.
         @param box     The inlet box.
         @param outlet  The index of the inlet.
         @return true if the connection has been done, otherwise false.
         */
        bool connectOutlet(unsigned long outlet, sBox box, unsigned long inlet);
        
        //! Disconnect an inlet to a box's outlet.
        /** The function disconnects an inlet to a box's outlet. Note that the connection of the inlet isn't very necessary, it only to  facilitate the retrieving of the input boxes.
         @param inlet   The index of the inlet.
         @param box     The outlet's box.
         @param outlet  The index of the outlet.
         @return true if the connection has been removed, otherwise false.
         */
        bool disconnectInlet(unsigned long inlet, sBox box, unsigned long outlet);
        
        //! Disconnect an outlet to a box's inlet.
        /** The function disconnects an outlet to a box's inlet. This part of the disconnection is the only one really important, because this is the one used by the send method.
         @param inlet   The index of the outlet.
         @param box     The inlet box.
         @param outlet  The index of the inlet.
         @return true if the connection has been removed, otherwise false.
         */
        bool disconnectOutlet(unsigned long outlet, sBox box, unsigned long inlet);
        
    public:
        
        //! The default paint method.
        /** The default function paint a default box with the background, border, inlets, outlets and text.
         @param paper       A doodle to draw.
         @param edit        If the page is in edition mode.
         @param selected    If the box is selected
         */
        static void paint(sBox box, Doodle& d, bool edit = false, bool selected = false);
        
        // ================================================================================ //
        //                                      BOX LISTENER                                //
        // ================================================================================ //
        
        //! The box listener is a virtual class that can bind itself to a box and be notified of the several changes.
        /**
         The box listener is a very light class with methods that receive the notifications of the boxes.
         @see Instance
         */
        class Listener
        {
        public:
            //! The constructor.
            /** The constructor does nothing.
             */
            Listener()
            {
                ;
            }
            
            //! The destructor.
            /** The destructor does nothing.
             */
            virtual ~Listener()
            {
                ;
            }
            
            //! Receive the notification that the box should be repainted
            /** The function is called by the box when it should be repainted.
             @param box    The box.
             */
            virtual void shouldBeRedrawn(sBox box){};
            
            //! Receive the notification that an inlet has been created.
            /** The function is called by the box when a inlet has been created.
             @param box    The box.
             @param index  The inlet index.
             */
            virtual void inletHasBeenCreated(sBox box, unsigned long index){};
            
            //! Receive the notification that an inlet has been removed.
            /** The function is called by the box when an inlet has been removed.
             @param box    The box.
             @param index  The inlet index.
             */
            virtual void inletHasBeenRemoved(sBox box, unsigned long index){};
            
            //! Receive the notification that an inlet has been connected.
            /** The function is called by the box when an inlet has been connected.
             @param box    The box.
             @param index  The inlet index.
             */
            virtual void inletHasBeenConnected(sBox box, unsigned long index){};
            
            //! Receive the notification that an outlet has been created.
            /** The function is called by the box when a outlet has been created.
             @param box    The box.
             @param index  The outlet index.
             */
            virtual void outletHasBeenCreated(sBox box, unsigned long index){};
            
            //! Receive the notification that an outlet has been removed.
            /** The function is called by the box when an outlet has been removed.
             @param box    The box.
             @param index  The outlet index.
             */
            virtual void outletHasBeenRemoved(sBox box, unsigned long index){};
            
            //! Receive the notification that an outlet has been connected.
            /** The function is called by the box when an outlet has been connected.
             @param box    The box.
             @param index  The outlet index.
             */
            virtual void outletHasBeenConnected(sBox box, unsigned long index){};
        };
		
		//! Add a box listener in the binding list of the box.
		/** The function adds a box listener in the binding list of the box. If the box listener is already in the binding list, the function doesn't do anything.
		 @param listener  The pointer of the box listener.
		 @see              unbind()
		 */
		void bind(weak_ptr<Listener> listener);
		
		//! Remove a box listener from the binding list of the box.
		/** The function removes a box listener from the binding list of the box. If the box listener isn't in the binding list, the function doesn't do anything.
		 @param listener  The pointer of the box listener.
		 @see           bind()
		 */
		void unbind(weak_ptr<Listener> listener);
		
        typedef shared_ptr<Listener>    sListener;
        
        // ================================================================================ //
        //                                  BOX CONTROLER                                   //
        // ================================================================================ //
        
        //! The box listener is a virtual class that can bind itself to a box and be notified of the several changes.
        /**
         The box listener is a very light class with methods that receive the notifications of the boxes.
         @see Instance
         */
        class Controler
        {
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
            
            inline bool isHit(Point const& pt)
            {
                if(m_box->getBounds().contains(pt))
                {
                    return true;
                }
                return false;
            }
            
            void setEditionStatus(bool status);
            
            void setSelectedStatus(bool status);
            
            //! The redraw function that should be override.
            /** The function is called by the box when it should be repainted.
             @param box    The box.
             */
            virtual void redraw(sBox box) = 0;
        };
        
    private:
		
        // ================================================================================ //
        //                                      BOX FACTORY                                 //
        // ================================================================================ //
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


