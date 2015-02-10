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

#ifndef __DEF_KIWI_OBJECT__
#define __DEF_KIWI_OBJECT__

#include "Dico.h"
#include "Beacon.h"
#include "Clock.h"

// TODO
// - See how to format the expression
// - Object should deletes it owns links at deletion
// - Links and Iolets behavior (resize ect..)
namespace Kiwi
{
    struct Initializer
    {
        const sInstance     instance;
        const sPage         page;
        const ulong         lid;
        const string        name;
        const string        text;
        const scDico        dico;
        const ElemVector    args;
        
        Initializer() :
        instance(nullptr), page(nullptr), lid(0), name(""), text(""), dico(nullptr), args({})
        {
            ;
        }
        
        Initializer(sInstance _instance, sPage _page, const ulong _id, const string _name, const string _text, scDico _dico, ElemVector const& _args) :
        instance(_instance), page(_page), lid(_id), name(_name), text(_text), dico(_dico), args(_args)
        {
            ;
        }
    };
    
    // ================================================================================ //
    //                                      OBJECT                                         //
    // ================================================================================ //
    
    //! The object is a graphical object.
    /**
     The object is a graphical class that aims to be instantiate in a page.
     */
    class Object : virtual public Sketcher
    {
    public:
        
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
            wObject object;
            ulong index;
        };
        
        const wInstance			m_instance;
        const wPage				m_page;
        const sTag				m_name;
        const string            m_text;
        const ulong             m_id;
        
        vector<sOutlet>			m_outlets;
        vector<sInlet>			m_inlets;
        atomic_ullong			m_stack_count;
        mutable mutex			m_mutex;
        
    protected:
        const sAttrPoint        m_presentation_position;
        const sAttrSize         m_presentation_size;
        const sAttrBool         m_hidden;
        const sAttrBool         m_presentation;
        const sAttrBool         m_ignoreclick;
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Object(Initializer const& initiliazer, string const& name);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Object();
        
    protected:
        
        //! Retrieve the shared pointer of the attribute manager.
        /** The function retrieves the shared pointer of the attribute manager.
         @return The shared pointer of the attribute manager.
         */
        sObject getShared() noexcept
        {
            return dynamic_pointer_cast<Object>(shared_from_this());
        }
        
        //! Retrieve the shared pointer of the attribute manager.
        /** The function retrieves the shared pointer of the attribute manager.
         @return The shared pointer of the attribute manager.
         */
        scObject getShared() const noexcept
        {
            return dynamic_pointer_cast<const Object>(shared_from_this());
        }
        
    public:
        
        //! Retrieve the instance that manages the page of the object.
        /** The function retrieves the instance that manages the page of the object.
         @return The instance that manages the page of the object.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
        
        //! Retrieve the page that manages the object.
        /** The function retrieves the page that manages the object.
         @return The page that manages the object.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the text of the object.
        /** The function retrieves the text of the object.
         @return The text of the object.
         */
        inline string getText() const noexcept
        {
            return m_text;
        }
        
        //! Retrieve the id of the object.
        /** The function retrieves the id of the object.
         @return The id of the object.
         */
        inline ulong getId() const noexcept
        {
            return m_id;
        }
        
        //! Retrieve the expression of the object.
        /** The function retrieves the expression of the object as a string.
         @return The expression of the object as a string.
         */
        virtual string getExpression() const noexcept
        {
            return "error";
        }
        
        //! Retrieve the number of inlets of the object.
        /** The functions retrieves the number of inlets of the object.
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
        
        //! Retrieve the number of outlets of the object.
        /** The functions retrieves the number of outlets of the object.
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
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual void receive(ulong index, ElemVector const& elements) = 0;
        
        //! Write the object in a dico.
        /** The function writes the object in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
        //! Retrieves if the object should be hidden when the page is locked.
        /** The function retrieves if the object should be hidden when the page is locked.
         @return True if the object should be hidden when the page is locked, false otherwise.
         */
        inline bool isHiddenOnLock() const noexcept
        {
            return m_hidden->getValue();
        }
        
        //! Retrieve if the object should be displayed in presentation.
        /** The function retrieves if the object should be displayed in presentation.
         @return True if the object should be displayed in presentation, otherwise false.
         */
        inline bool isIncludeInPresentation() const noexcept
        {
            return m_presentation->getValue();
        }
        
        //! Retrieve the position of the object.
        /** The function retrieves the position of the object.
         @param presentation The presentation state.
         @return The position of the object.
         */
        inline Point getPosition(const bool presentation = false) const noexcept
        {
            if(!presentation)
            {
                return m_position->getValue();
            }
            else
            {
                return m_presentation_position->getValue();
            }
        }
        
        //! Retrieve the size of the object.
        /** The function retrieves the size of the object.
         @param presentation The presentation state.
         @return The size of the object.
         */
        inline Size getSize(const bool presentation = false) const noexcept
        {
            if(!presentation)
            {
                return m_size->getValue();
            }
            else
            {
                return m_presentation_size->getValue();
            }
        }
        
        //! Retrieve the bounds of the object.
        /** The function retrieves the bounds of the object.
         @param presentation The presentation state.
         @return The bounds of the object.
         */
        inline Rectangle getBounds(const bool presentation = false) const noexcept
        {
            if(!presentation)
            {
                return Rectangle(m_position->getValue(), m_size->getValue());
            }
            else
            {
                return Rectangle(m_presentation_position->getValue(), m_presentation_size->getValue());
            }
        }
        
    protected:
        
        //! Send a vector of elements via an outlet.
        /** The function sends a vector of elements via an outlet and dispatches it to all the connected inlets.
         @param index The index of the outlet.
         @param elements A list of elements to pass.
         */
        void    send(ulong index, ElemVector const& elements) const noexcept;
        
        //! Add a new inlet to the object.
        /** The function adds a new inlet to the object.
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    addInlet(Io::Type type, Io::Polarity polarity, string const& description = "");
        
        //! Add a new outlet the the object.
        /** The function adds a new outlet the the object.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    addOutlet(Io::Type type, string const& description = "");

    private:
        
        //! The write method that should be override.
        /** The function writes the object in a dico.
         @param dico The dico.
         */
		virtual void save(sDico dico) const {};
		
        //! The read method that should be override.
        /** The function reads a dico to initalize the objecte.
         @param dico The dico.
         */
		virtual void load(scDico dico) {};
    };
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a object. It also have a type and a description.
     */
    class Object::Iolet : public enable_shared_from_this<Iolet>
    {
    protected:
        friend Page;
        friend Link;
        
        vector<Connection>  m_connections;
        const Io::Type      m_type;
        const Io::Polarity  m_polarity;
        const string        m_description;
        mutable mutex       m_mutex;
        
        //! Check if a connection is in the iolet.
        /** The functions checks if a connection is in the iolet.
         @param object The object.
         @param index the iolet's index.
         @return true if the connection is in the iolet, otherwise false.
         */
        bool has(sObject object, ulong index) const noexcept;
        
        //! Append a new connection to the iolet.
        /** The functions appends a new connection to the iolet.
         @param object The object.
         @param index the iolet's index.
         @return true if the connection has been added, otherwise false.
         */
        bool append(sObject object, ulong index) noexcept;
        
        //! Remove a connection from the iolet.
        /** The functions removes a connection from the iolet.
         @param object The object.
         @param index the iolet's index.
         @return true if the connection has been removed, otherwise false.
         */
        bool erase(sObject object, ulong index) noexcept;
        
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
                return {sObject(), 0};
            }
        }
        
        //! Retrieve the object of a connection.
        /** The functions retrieves the object of a connection.
         @param index The index of the connection.
         @return The object of a connection.
         */
        inline sObject getBox(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (ulong)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index].object.lock();
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
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of links.
    /**
     The inlet owns a set of links that are used to manage links in a object. It also have a type and a description.
     */
    class Object::Inlet : public Iolet
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
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a object. It also have a type and a description.
     */
    class Object::Outlet : public Iolet
    {
    public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         @param type        The type of the outlet.
         @param description The description of the outlet.
         */
        Outlet(Io::Type type, string const& description) noexcept;
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Outlet();
        
        //! Send a vector of elements to the connected inlets.
        /** The function sends of elements to the connected inlets.
         @param elements The vector of elements.
         */
        void send(ElemVector const& elements) const noexcept;
    };
    
    inline string toString(scObject object)
    {
        return toString(object->getName());
    }
}


#endif

