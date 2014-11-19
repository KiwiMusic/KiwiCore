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
     The box is a graphical class that aims to be instantiate in a page.
     */
	class Box : public Attr::Manager, public enable_shared_from_this<Box>
    {
    public:
        class Listener;
        
        enum Type
        {
            SignalProcessor         = 1<<1,
            MouseListener           = 1<<2,
            KeyboardListener        = 1<<3,
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
            struct Conn
            {
                sBox    m_box;
                size_t  m_index;
            };
            
            enum Type
            {
                Data    = 0,
                Signal  = 1
            };
        
            vector<Conn>  m_conns;
            const Type    m_type;
            const string  m_description;
            
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
        const unsigned long         m_type;
        
        atomic_ulong                m_id;
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
        sInstance getInstance() const noexcept;
        
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
        inline sTag getText() const noexcept
        {
            return m_text;
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
            return (unsigned long)m_inlets.size();
        }
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The inlet index.
         @return The description.
         */
        inline string getInletDescription(unsigned long index) const noexcept
        {
            if(index < m_inlets.size())
            {
                return m_inlets[(vector<unique_ptr<Inlet>>::size_type)index]->m_description;
            }
            else
            {
                return "";
            }
        }
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        inline unsigned long getNumberOfOutlets() const noexcept
        {
            return (unsigned long)m_outlets.size();
        }
        
        //! Retrieve the description of an outlet.
        /** The functions retrieves the description of an outlet.
         @param index The index of the outlet.
         @return The descrition.
         */
        inline string getOutletDescription(unsigned long index) const noexcept
        {
            if(index < m_outlets.size())
            {
                return m_outlets[(vector<unique_ptr<Outlet>>::size_type)index]->m_description;
            }
            else
            {
                return "";
            }
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(size_t index, ElemVector const& elements)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A mouse event.
         */
        virtual bool receive(Event::Mouse const& event)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A keyboard event.
         */
        virtual bool receive(Event::Keyboard const& event)
        {
            return false;
        }
        
        //! The paint method that should be override.
        /** The function shoulds draw some stuff in the doodle.
         @param doodle    A doodle to draw.
         */
        virtual bool draw(Doodle& doodle) const
        {
            return false;
        }
        
        //! The draw method.
        /** The function performs some stuff before to call the paint method.
         @param paper    A doodle to draw.
         */
        void paint(Doodle& d, bool edit = 0, bool selected = 0) const;
        
        //! Set the id of the box.
        /** The function sets the id of the box.
         @param id The id of the box.
         */
        inline void setId(unsigned long _id) noexcept
        {
            m_id = _id;
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
        
        static bool compatible(scLink link) noexcept;
        
        static bool connect(scLink link) noexcept;
        
        static bool disconnect(scLink link) noexcept;
        
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
    
    //! The link owns to a page and is used to create a patch lines.
    /**
     The link is opaque, you shouldn't have to use it at all.
     */
    class Link
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
        Link(sBox from, unsigned long outlet, sBox to, unsigned long inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Link();
        
        //! The link creation method.
        /** The function allocates a link.
         @param from    The output box.
         @param outlet  The outlet of the link.
         @param to      The input box.
         @param inlet   The inlet of the link.
         @return The link.
         */
        static sLink create(const sBox from, const size_t outlet, const sBox to, const size_t inlet);
        
        //! The link creation method.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(scPage page, scDico dico);
        
        //! The link creation method with another link but change one of the boxes with another one.
        /** The function allocates a link with another link.
         @param connect The other link.
         @param oldbox  The old box to replace.
         @param newbox  The newbox box that replace.
         @return The link.
         */
        static sLink create(const sLink connect, const sBox oldbox, const sBox newbox)
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
        /** The function retrieves the output box of the link.
         @return The output box.
         */
        inline sBox getBoxFrom() const noexcept
        {
            return m_from.lock();
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the link.
         @return The input box.
         */
        inline sBox getBoxTo() const noexcept
        {
            return m_to.lock();
        }
        
        //! Retrieve the outlet of the link.
        /** The function retrieves the outlet of the link.
         @return The outlet of the link.
         */
        inline size_t getOutletIndex() const noexcept
        {
            return m_outlet;
        }
        
        //! Retrieve the inlet of the link.
        /** The function retrieves the inlet of the link.
         @return The inlet of the link.
         */
        inline size_t getInletIndex() const noexcept
        {
            return m_inlet;
        }
        
        //! Retrieve the inlet of the link.
        /** The function retrieves the inlet of the link.
         @return The inlet of the link.
         */
        inline bool isValid() const noexcept
        {
            sBox from   = m_from.lock();
            sBox to     = m_to.lock();
            return from && to && from != to && m_outlet  < from->getNumberOfOutlets() && m_inlet < to->getNumberOfInlets() && from->getPage() == to->getPage();
        }
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
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


