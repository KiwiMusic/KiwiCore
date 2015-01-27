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

#include "Attribute.h"
#include "Event.h"
#include "Doodle.h"
#include "Beacon.h"
#include "Clock.h"

// TODO
// - See how to format the expression
// - Box should deletes it owns links at deletion
// - Links and Iolets behavior (resize ect..)
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
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        typedef weak_ptr<Listener>          wListener;
        typedef shared_ptr<const Listener>  scListener;
        typedef weak_ptr<const Listener>    wcListener;

        enum Behavior
        {
            Mouse       = 1<<2,
            Keyboard    = 1<<3,
            Graphic     = 1<<4
        };
        
        struct Io
        {
            enum Type
            {
                Message = 0 | 1<<1,
                Signal  = 0 | 1<<2,
                Both    = 0 | Message | Signal
            };
            
            enum Polarity : bool
            {
                Cold   = 0,
                Hot    = 1
            };
        };
        
    
        class Iolet;
        
        class Inlet;
        typedef shared_ptr<Inlet>       sInlet;
        typedef weak_ptr<Inlet>         wInlet;
        typedef shared_ptr<const Inlet> scInlet;
        typedef weak_ptr<const Inlet>   wcInlet;
        
        class Outlet;
        typedef shared_ptr<Outlet>      sOutlet;
        typedef weak_ptr<Outlet>        wOutlet;
        typedef shared_ptr<const Outlet>scOutlet;
        typedef weak_ptr<const Outlet>  wcOutlet;
    
    private:
        struct Connection
        {
            wBox box;
            ulong index;
            bool operator<(Connection const& other) const noexcept;
        };
        
        const wInstance			m_instance;
        const wPage				m_page;
        const sTag				m_name;
        
        vector<sOutlet>			m_outlets;
        vector<sInlet>			m_inlets;
        atomic_ullong			m_stack_count;
        mutable mutex			m_mutex;
		
		set<wListener,
		owner_less<wListener>>  m_listeners;
		mutable mutex           m_listeners_mutex;
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Box(sPage page, string const& name);
        
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
        
        //! Retrieve the name of the box.
        /** The function retrieves the name of the box as a tag.
         @return The name of the box as a tag.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
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
        inline ulong getNumberOfInlets() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_inlets.size();
        }
        
        //! Retrieve an inlet.
        /** The functions retrieves an inlet.
         @param index The inlet's index.
         @return The inlet.
         */
        inline sInlet getInlet(ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < m_inlets.size())
            {
                return m_inlets[(vector<sInlet>::size_type)index];
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        inline ulong getNumberOfOutlets() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_outlets.size();
        }
        
        //! Retrieve an outlet.
        /** The functions retrieves an outlet.
         @param index The outlet's index.
         @return The outlet.
         */
        inline sOutlet getOutlet(ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < m_outlets.size())
            {
                return m_outlets[(vector<sOutlet>::size_type)index];
            }
            else
            {
                return nullptr;
            }
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if the vector of element doesn't match with your method then the box will check if the vector match with attributes methods, othersize return true.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(ulong index, ElemVector const& elements)
        {
            return false;
        }
        
        //! Write the box in a dico.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
    protected:
        
        //! Send a vector of elements via an outlet.
        /** The function sends a vector of elements via an outlet and dispatches it to all the connected inlets.
         @param index The index of the outlet.
         @param elements A list of elements to pass.
         */
        void    send(ulong index, ElemVector const& elements) const noexcept;
        
        //! Add a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    addInlet(Io::Type type, Io::Polarity polarity, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param index The index of the inlet;
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    insertInlet(ulong index, Io::Type type, Io::Polarity polarity, string const& description = "");
        
        //! Remove an inlet from the box.
        /** The function removes an inlet from the box.
         @param index The index of the inlet
         */
        void    removeInlet(ulong index);
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    addOutlet(Io::Type type, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @pram index The index of the outlet.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    insertOutlet(ulong index, Io::Type type, string const& description = "");
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The index of the outlet.
         */
        void    removeOutlet(ulong index);

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
		virtual void save(sDico dico) const {};
		
        //! The read method that should be override.
        /** The function reads a dico to initalize the boxe.
         @param dico The dico.
         */
		virtual void load(scDico dico) {};
		
        //! Notify that an attribute changed.
		/** The function notifies that an attribute changed.
		 @param attr The attribute.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
        virtual bool notify(sAttr attr)
        {
            return true;
        }
		
		//! Notify the manager that the values of an attribute has changed.
		/** The function notifies the manager that the values of an attribute has changed.
		 @param attr The attribute that has changed.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		bool attributeChanged(sAttr attr);

    public:
        
        //! Add the listener of the box.
        /** The function adds the listener of the box.
         @param list The listener.
         */
        void addListener(sListener list);
        
        //! Remove the listener of the box.
        /** The function removes the listener of the box.
         @param list The listener.
         */
        void removeListener(sListener list);
        
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
		
		//! Retrieves all loaded prototype names.
		/** This function retrieves all loaded prototype names.
		 @param names A vector of Tag to be filled.
		 */
		static void getPrototypeNames(vector<sTag>& names);        
    };
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Box::Iolet : public Attr::Listener, public enable_shared_from_this<Iolet>
    {
    protected:
        friend Link;
        
        vector<Connection>  m_connections;
        const Io::Type      m_type;
        const Io::Polarity  m_polarity;
        const string        m_description;
        mutable mutex       m_mutex;
        
        //! Check if a connection is in the iolet.
        /** The functions checks if a connection is in the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection is in the iolet, otherwise false.
         */
        bool has(sBox box, ulong index) const noexcept;
        
        //! Append a new connection to the iolet.
        /** The functions appends a new connection to the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection has been added, otherwise false.
         */
        bool append(sBox box, ulong index) noexcept;
        
        //! Remove a connection from the iolet.
        /** The functions removes a connection from the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection has been removed, otherwise false.
         */
        bool erase(sBox box, ulong index) noexcept;
        
    public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Iolet(Io::Type type, Io::Polarity polarity, string const& description) noexcept :
        m_type(type),
        m_polarity(polarity),
        m_description(description)
        {
            ;
        }
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Iolet()
        {
            m_connections.clear();
        }
        
        //! Retrieve the type of the iolet.
        /** The functions retrieves the type of the iolet.
         @return The type of the iolet.
         */
        inline Io::Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the polarity of the iolet.
        /** The functions retrieves the polarity of the iolet.
         @return The polarity of the iolet.
         */
        inline Io::Polarity getPolarity() const noexcept
        {
            return m_polarity;
        }
        
        //! Retrieve the description of the iolet.
        /** The functions retrieves the description of the iolet.
         @return The description of the iolet.
         */
        inline string getDescription() const noexcept
        {
            return m_description;
        }
        
        //! Retrieve the number of connections.
        /** The functions retrieves the number of connections of the iolet.
         @return The number of connections.
         */
        inline ulong getNumberOfConnection() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_connections.size();
        }
        
        //! Retrieve the a connection.
        /** The functions retrieves a connection.
         @param index The index of the connection.
         @return The connection.
         */
        inline Connection getConnection(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (ulong)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index];
            }
            else
            {
                return {sBox(), 0};
            }
        }
        
        //! Retrieve the box of a connection.
        /** The functions retrieves the box of a connection.
         @param index The index of the connection.
         @return The box of a connection.
         */
        inline sBox getBox(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (ulong)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index].box.lock();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the iolet's index of a connection.
        /** The functions retrieves the iolet's index of a connection.
         @param index The index of the connection.
         @return The iolet's index of a connection.
         */
        inline ulong getIndex(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (ulong)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index].index;
            }
            else
            {
                return 0;
            }
        }
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager		The manager that manages the attribute.
         @param attr		The attribute that has been modified.
         @param type		The type of notification.
         */
        void notify(Attr::sManager manager, sAttr attr, Attr::Notification type);
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of links.
    /**
     The inlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Box::Inlet : public Iolet
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Inlet(Io::Type type, Io::Polarity polarity, string const& description) noexcept;
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Inlet();
        
        //! The creation method.
        /** The function creates a shared pointer of an inlet.
         @param type        The type of the inlet.
         @param polarity    The polarity of the inlet.
         @param description The description of the inlet.
         @return The inlet.
         */
        static sInlet create(Io::Type type, Io::Polarity polarity, string const& description);
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Box::Outlet : public Iolet
    {
    public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Outlet(Io::Type type, string const& description) noexcept;
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Outlet();
        
        //! The creation method.
        /** The function creates a shared pointer of an outlet.
         @param type        The type of the outlet.
         @param polarity    The polarity of the outlet.
         @param description The description of the outlet.
         @return The outlet.
         */
        static sOutlet create(Io::Type type, string const& description);
        
        //! Send a vector of elements to the connected inlets.
        /** The function sends of elements to the connected inlets.
         @param elements The vector of elements.
         */
        void send(ElemVector const& elements) const noexcept;
    };
    
    // ================================================================================ //
    //                                  BOX LISTENER                                    //
    // ================================================================================ //
    
    //! The box listener .
    /**
     The box listener...
     */
    class Box::Listener
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Listener() noexcept
        {
            ;
        }
        
        //! The destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Listener()
        {
            ;
        }
        
        //! The inlets notification function that should be override.
        /** The function is called by the box when its inlets changed.
         */
        virtual void inletsChanged() = 0;
        
        //! The outlets notification function that should be override.
        /** TThe function is called by the box when its outlets changed.
         */
        virtual void outletsChanged() = 0;
        
        //! The position notification function that should be override.
        /** The function is called by the box when its position changed.
         */
        virtual void positionChanged() = 0;
        
        //! The size notification function that should be override.
        /** The function is called by the box when its size changed.
         */
        virtual void sizeChanged() = 0;
        
        //! The lock notification function that should be override.
        /** The function is called by the box when its size changed.
         */
        virtual void pageLockStatusChanged() = 0;
        
        //! The page presentation notification function.
        /** The function is called by the box when page presentation mode changed.
         */
        virtual void pagePresentationStatusChanged() = 0;
        
        //! Called by the box when the box selection status changed.
        /** The function is called by the box selection status changed.
         */
        virtual void selectionStatusChanged() = 0;
        
        //! Called by the box when the presentation status changed.
        /** The function is called by the box when the presentation status changed.
         */
        virtual void presentationStatusChanged() = 0;
        
        //! The redraw function that should be override.
        /** The function is called by the box when it should be repainted.
         */
        virtual void redraw() = 0;
        
        //! The grab focus function that should be override.
        /** The function is called by the box when it want to grab keyboard focus.
         */
        virtual void grabKeyboardFocus() = 0;
    };
    
    inline string toString(scBox box)
    {
        return toString(box->getName());
    }
}


#endif


