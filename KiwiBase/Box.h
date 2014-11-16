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

namespace Kiwi
{
    class Instance;
    class Page;
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical box.
    /**
     The box is a graphical box that aims to be instantiate in a page.
     */
	class Box : public Attribute::Manager, public enable_shared_from_this<Box>
    {
    public:
        class Listener;
        
        enum Type
        {
            SignalProcessor         = 1<<1,
            MouseListener           = 1<<2,
            KeyboardMouseListener   = 1<<3,
            Drawer                  = 1<<4
        };
    protected:
        
        class Inlet
        {
        public:
            enum Type
            {
                DataCold    = 0,
                DataHot     = 1,
                Signal      = 2
            };

            const Type      m_type;
            const string    m_description;
            
            Inlet(Type type, string description) :
            m_type(type),
            m_description(description)
            {
                ;
            }
            
            ~Inlet()
            {
                ;
            }
        };
        
        
        class Outlet
        {
        public:
            struct Connection
            {
                sBox m_box;
                size_t          m_index;
            };
            
            enum Type
            {
                Data    = 0,
                Signal  = 1
            };
        
            vector<Connection>  m_conns;
            const Type          m_type;
            const string        m_description;
            
            Outlet(Type type, string description) :
            m_type(type),
            m_description(description)
            {
                ;
            }
            
            ~Outlet()
            {
                m_conns.clear();
            }
        };
        
    private:
            
        const wPage                 m_page;
        const sTag                  m_name;
        const atomic_ulong          m_id;
        const atomic_long           m_type;
        
        sTag                        m_text;
        vector<unique_ptr<Outlet>>  m_outlets;
        vector<unique_ptr<Inlet>>   m_inlets;
        size_t                      m_stack_count;
        
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>   m_listeners;
        
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Box(sPage page, string const& name, long type = 1<<0);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Box();
        
        //! The box creation method.
        /** The function allocates a box and initialize the defaults members.
         */
        static sBox create(sPage page, sDico dico);
        
        //! Retrieve the page that manages the box.
        /** The function retrieves the page that manages the box.
         @return The page that manages the box.
         */
        sPage getPage() const noexcept;
        
        //! Retrieve the instance that manages the page of the box.
        /** The function retrieves the instance that manages the page of the box.
         @return The instance that manages the page of the box.
         */
        sInstance getInstance() const noexcept;
        
        //! Retrieve the name of the box.
        /** The function retrieves the name of the box as a tag.
         @return The name of the box as a tag.
         */
        sTag getName() const noexcept;
        
        //! Retrieve the text of the box.
        /** The function retrieves the text of the box as a tag.
         @return The text of the box as a tag.
         */
        sTag getText() const noexcept;
        
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
        long getType() const noexcept;
        
        //! Retrieve the expression of the box.
        /** The function retrieves the expression of the box as a string.
         @return The expression of the box as a string.
         */
        virtual string getExpression() const noexcept;
        
        //! Write the box in a dico.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;

        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(size_t index, ElemVector const& elements);
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A mouse event.
         */
        virtual bool receive(Event::Mouse const& event);
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A keyboard event.
         */
        virtual bool receive(Event::Keyboard const& event);
        
        //! The paint method that should be override.
        /** The function shoulds draw some stuff in the doodle.
         @param doodle    A doodle to draw.
         */
        virtual bool draw(Doodle& doodle) const;
        
        //! The draw method.
        /** The function performs some stuff before to call the paint method.
         @param paper    A doodle to draw.
         */
        void paint(Doodle& d, bool edit = 0, bool selected = 0) const;
        
        //! Retrieve the number of inlets of the box.
        /** The functions retrieves the number of inlets of the box.
         @return The number of inlets.
         */
        size_t getNumberOfInlets() const noexcept;
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The inlet index.
         @return The description.
         */
        string getInletDescription(size_t index) const noexcept;
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        size_t  getNumberOfOutlets() const noexcept;
        
        //! Retrieve the description of an outlet.
        /** The functions retrieves the description of an outlet.
         @param index The index of the outlet.
         @return The descrition.
         */
        string getOutletDescription(size_t index) const noexcept;
        
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
        void    send(size_t index, ElemVector const& elements) const noexcept;
        
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
        void    insertInlet(size_t index, Inlet::Type type, string const& description = "");
        
        //! Remove an inlet from the box.
        /** The function removes an inlet from the box.
         @param index The index of the inlet
         */
        void    removeInlet(size_t index);
        
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
        void    insertOutlet(size_t index, Outlet::Type type, string const& description = "");
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The index of the outlet.
         */
        void    removeOutlet(size_t index);

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
        virtual void save(sDico dico) const;
        
        //! The read method that should be override.
        /** The function read a dico to initalize the boxe.
         @param dico The dico.
         */
        virtual void load(scDico dico);
        
    public:
        
        static bool compatible(scConnection connection) noexcept;
        
        static bool connect(scConnection connection) noexcept;
        
        static bool disconnect(scConnection connection) noexcept;
        
        // ================================================================================ //
        //                                  BOX LISTENER                                    //
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
        
    private:
		
        // ================================================================================ //
        //                                      BOX FACTORY                                 //
        // ================================================================================ //
        
        static map<sTag, unique_ptr<Box>>  m_prototypes;
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
    
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    //! The connection owns to a page and is used to create a patch lines.
    /**
     The connection is opaque, you shouldn't have to use it at all.
     */
    class Connection
    {
    private:
        
        const wBox     m_from;
        const wBox     m_to;
        const size_t   m_outlet;
        const size_t   m_inlet;
        
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Connection(sBox from, unsigned long outlet, sBox to, unsigned long inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Connection();
        
        //! The connection creation method.
        /** The function allocates a connection.
         @param from    The output box.
         @param outlet  The outlet of the connection.
         @param to      The input box.
         @param inlet   The inlet of the connection.
         @return The connection.
         */
        static shared_ptr<Connection> create(const sBox from, const size_t outlet, const sBox to, const size_t inlet);
        
        //! The connection creation method.
        /** The function allocates a connection with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the connection.
         @return The connection.
         */
        static shared_ptr<Connection> create(scPage page, scDico dico);
        
        //! The connection creation method with another connection but change one of the boxes with another one.
        /** The function allocates a connection with another connection.
         @param connect The other connection.
         @param oldbox  The old box to replace.
         @param newbox  The newbox box that replace.
         @return The connection.
         */
        static shared_ptr<Connection> create(const shared_ptr<Connection> connect, const sBox oldbox, const sBox newbox)
        {
            if(connect->getBoxFrom() == oldbox)
            {
                if(connect->getOutletIndex() < newbox->getNumberOfOutlets())
                {
                    return create(newbox, connect->getOutletIndex(), connect->getBoxTo(), connect->getInletIndex());
                }
            }
            else if(connect->getBoxTo() == oldbox)
            {
                if(connect->getInletIndex() < newbox->getNumberOfInlets())
                {
                    return create(connect->getBoxFrom(), connect->getOutletIndex(), newbox, connect->getInletIndex());
                }
            }
            return nullptr;
        }
        
        //! Retrieve the output box.
        /** The function retrieves the output box of the connection.
         @return The output box.
         */
        inline sBox getBoxFrom() const noexcept
        {
            return m_from.lock();
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the connection.
         @return The input box.
         */
        inline sBox getBoxTo() const noexcept
        {
            return m_to.lock();
        }
        
        //! Retrieve the outlet of the connection.
        /** The function retrieves the outlet of the connection.
         @return The outlet of the connection.
         */
        inline size_t getOutletIndex() const noexcept
        {
            return m_outlet;
        }
        
        //! Retrieve the inlet of the connection.
        /** The function retrieves the inlet of the connection.
         @return The inlet of the connection.
         */
        inline size_t getInletIndex() const noexcept
        {
            return m_inlet;
        }
        
        //! Retrieve the inlet of the connection.
        /** The function retrieves the inlet of the connection.
         @return The inlet of the connection.
         */
        inline bool isValid() const noexcept
        {
            sBox from   = m_from.lock();
            sBox to     = m_to.lock();
            return from && to && from != to && m_outlet  < from->getNumberOfOutlets() && m_inlet < to->getNumberOfInlets() && from->getPage() == to->getPage();
        }
        
        //! Write the page in a dico.
        /** The function writes the connection in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept
        {
            sBox     from    = getBoxFrom();
            sBox     to      = getBoxTo();
            if(from && to)
            {
                dico->set(Tag::from, {from->getId(), getOutletIndex()});
                dico->set(Tag::to, {to->getId(), getInletIndex()});
            }
            else
            {
                dico->clear(Tag::from);
                dico->clear(Tag::to);
            }
        }
            
    };
}


#endif


