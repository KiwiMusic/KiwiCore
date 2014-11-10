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
#include "Events.h"

namespace Kiwi
{
    class Instance;
    class Page;
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical object.
    /**
     The box is a graphical object that aims to be instantiate in a page.
     */
    
    class Box : public AttributeFactory, public enable_shared_from_this<Box>
    {
    public:
        class Listener;
        
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
                shared_ptr<Box> m_box;
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
            
        const weak_ptr<Page>        m_page;
        const sTag                  m_name;
        sTag                        m_text;
        vector<unique_ptr<Outlet>>  m_outlets;
        vector<unique_ptr<Inlet>>   m_inlets;
        size_t                      m_stack_count;
        
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>   m_listeners;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        Box(weak_ptr<Page> page, string const& name);
        
        //! Destructor.
        /** Free the members.
         */
        virtual ~Box();
        
        //! The box creation method.
        /** The function allocates a box and initialize the defaults members.
         */
        static shared_ptr<Box> create(shared_ptr<Page> page, sDico dico);
        
        //! Retrieve the page that manages the box.
        /** The function retrieves the page that manages the box.
         @return The page that manages the box.
         */
        shared_ptr<Page> getPage() const noexcept;
        
        //! Retrieve the instance that manages the page of the box.
        /** The function retrieves the instance that manages the page of the box.
         @return The instance that manages the page of the box.
         */
        shared_ptr<Instance> getInstance() const noexcept;
        
        //! Retrieve the name of the box.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        sTag getName() const noexcept;
        
        //! Retrieve the text of the box.
        /** The function retrieves the text of the object as a tag.
         @return The text of the object as a tag.
         */
        sTag getText() const noexcept;
        
        //! Retrieve the expression of the box.
        /** The function retrieves the expression of the object as a string.
         @return The expression of the object as a string.
         */
        virtual string getExpression() const noexcept;
        
        //! Write the box in a dico.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico) const;

        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(size_t index, ElemVector const& elements);
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param events    An event.
         */
        virtual bool receive(Events const& events);
        
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
        virtual shared_ptr<Box> allocate(shared_ptr<Page> page, sDico dico) const = 0;
        
        //! The write method that should be override.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        virtual void save(shared_ptr<Dico> dico) const;
        
        //! The read method that should be override.
        /** The function read a dico to initalize the boxe.
         @param dico The dico.
         */
        virtual void load(shared_ptr<const Dico> dico);
        
    public:
        
        static bool compatible(shared_ptr<Box> from, size_t outlet, shared_ptr<Box> to, size_t inlet) noexcept;
        
        static bool connect(shared_ptr<Box> from, size_t outlet, shared_ptr<Box> to, size_t inlet) noexcept;
        
        static bool disconnect(shared_ptr<Box> from, size_t outlet, shared_ptr<Box> to, size_t inlet) noexcept;
        
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
            
            //! Receive the notification that an inlet has been created.
            /** The function is called by the box when a inlet has been created.
             @param box    The box.
             @param index  The inlet index.
             */
            virtual void inletHasBeenCreated(shared_ptr<Box> box, size_t index){};
            
            //! Receive the notification that an inlet has been removed.
            /** The function is called by the box when an inlet has been removed.
             @param box    The box.
             @param index  The inlet index.
             */
            virtual void inletHasBeenRemoved(shared_ptr<Box> box, size_t index){};
            
            //! Receive the notification that an outlet has been created.
            /** The function is called by the box when a outlet has been created.
             @param box    The box.
             @param index  The outlet index.
             */
            virtual void outletHasBeenCreated(shared_ptr<Box> box, size_t index){};
            
            //! Receive the notification that an outlet has been removed.
            /** The function is called by the box when an outlet has been removed.
             @param box    The box.
             @param index  The outlet index.
             */
            virtual void outletHasBeenRemoved(shared_ptr<Box> box, size_t index){};
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
    
    typedef shared_ptr<Box>     sBox;
    
    typedef weak_ptr<Box>       wBox;
    
    inline string toString(shared_ptr<const Box> box)
    {
        return toString(box->getName());
    }
    
    class Mouse
    {
    public:
        enum Type
        {
            Unknown     = 0,
            Enter       = 1,
            Leave       = 2,
            Move        = 3,
            Drag        = 4,
            Down        = 5,
            Up          = 6,
            DoubleClick = 7,
            Wheel       = 8
        };
        
        enum Modifier
        {
            Nothing     = 0,
            Shift       = 1,
            Ctrl        = 2,
            Alt         = 4,
            Left        = 16,
            Right       = 32,
            Middle      = 64,
            
#ifdef __APPLE__
            Cmd          = 8,
            Popup        = Right | Ctrl,
#else
            Cmd          = Ctrl,
            Popup        = Right
#endif
        };
        
        const Type      type;
        const int       x;
        const int       y;
        const long      modifiers;
        const double    wheel;
    };
}


#endif


