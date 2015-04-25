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

#ifndef __DEF_KIWI_ATTR__
#define __DEF_KIWI_ATTR__

#include "KiwiAtom.h"

namespace Kiwi
{
    class Attr;
    typedef shared_ptr<Attr>       sAttr;
    typedef weak_ptr<Attr>         wAttr;
    typedef shared_ptr<const Attr> scAttr;
    typedef weak_ptr<const Attr>   wcAttr;
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
    /** The attribute manages a set of values that should be displayed in an inspector. The setter and getter must be override.
     */
    class Attr : public enable_shared_from_this<Attr>
    {
    public:
        class Manager;
        typedef shared_ptr<Manager>         sManager;
        typedef weak_ptr<Manager>           wManager;
        typedef shared_ptr<const Manager>   scManager;
        typedef weak_ptr<const Manager>     swManager;
        
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        typedef weak_ptr<Listener>          wListener;
        typedef shared_ptr<const Listener>  scListener;
        typedef weak_ptr<const Listener>    wcListener;
        
        template <class T> class Typed;
        
        /** Flags describing the behavior of the attribute.
         @see setInvisible, setDisabled, setSaveable, setNotifyChanges
         */
        enum Behavior
        {
            Invisible			= 1<<0,///< Indicates that the attribute is invisible.
            Disabled			= 1<<1,///< Indicates that the attribute can't be changed.
            Unsaved             = 1<<2,///< Indicates that the attribute is not saved.
            Silent              = 1<<3,///< Indicates that the attribute should not notify its changes.
            All                 = Invisible | Disabled | Unsaved | Silent
        };
        
    protected:
        const sTag      m_name;				///< The name of the attribute.
        const string    m_label;			///< The label of the attribute.
        const string    m_category;			///< The category of the attribute.
        const ulong		m_order;			///< The order of the attribute.
        ulong           m_behavior;			///< The behavior of the attribute.
        bool            m_frozen;           ///< The frozen state of the attribute.
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual inline void setValue(Atom const& atom) = 0;
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        virtual inline void freeze(const bool frozen) = 0;
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        virtual inline void resetDefault()  = 0;
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        virtual inline void resetFrozen() = 0;
        
    public:
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute (usually only letters and undescore characters).
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        inline Attr(sTag name, string const& label, string const& category, const ulong behavior = 0, const ulong order = 0) noexcept :
        m_name(name), m_label(label), m_category(category), m_order(order), m_behavior(behavior), m_frozen(false) {}
        
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual inline ~Attr() noexcept {};
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        virtual type_index getTypeIndex() const noexcept = 0;
        
        //! Retrieve the attribute value an atom.
        /** The function retrieves the attribute value as  an atom.
         @return The atom.
         */
        virtual inline Atom getValue() const noexcept = 0;
        
        //! Retrieve if the attribute is from a specific template.
        /** The function retrieves if the attribute is from a specific template.
         @return true if the attribute is from a specific template.
         */
        template<class T> inline bool isType() const noexcept {return (type_index)typeid(T) == getTypeIndex();}
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline sTag getName() const noexcept {return m_name;}
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
         */
        inline string getLabel() const noexcept {return m_label;}
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
         */
        inline string getCategory() const noexcept {return m_category;}
        
        //! Retrieve the attribute order.
        /** The function retrieves the attribute order.
         @return The attribute order.
         */
        inline ulong getOrder() const noexcept {return m_order;}
        
        //! Retrieves the whole behavior flags field of the attribute.
        /** The function retrieves the whole behavior flags field of the attribute.
         @return behavior	A combination of the flags which define the attribute's behaviors.
         */
        inline ulong getBehavior() const noexcept{return m_behavior;}
        
        //! Retrieve if the attribute is invisible.
        /** The function retrieves if the attribute is invisible.
         @return True if the attribute is invisible otherwise false.
         */
        inline bool isInvisible() const noexcept {return m_behavior & Invisible;}
        
        //! Retrieve if the attribute is disable.
        /** The function retrieves if the attribute is disable.
         @return True if the attribute is disabled otherwise false.
         */
        inline bool isDisabled() const noexcept {return m_behavior & Disabled;}
        
        //! Retrieve if the attribute is saved.
        /** The function retrieves if the attribute is saved.
         @return True if the attribute is saveable otherwise false.
         */
        inline bool isUnsaved() const noexcept {return m_behavior & Unsaved;}
        
        //! Retrieve if the attribute should notify changes.
        /** The function retrieves if the attribute should notify changes.
         @return True if the attribute should notify changes otherwise false.
         */
        inline bool isSilent() const noexcept {return m_behavior & Silent;}
        
        //! Retrieve if the attribute is frozen.
        /** The function retrieves if the attribute is frozen.
         @return True if the attribute is frozen, false otherwise.
         */
        inline bool isFrozen() const noexcept {return m_frozen;}
        
        //! Set the whole behavior flags field of the attribute.
        /** The function sets the whole behavior flags field of the attribute.
         @param behavior	A combination of the flags which define the attribute's behaviors.
         */
        inline void setBehavior(const ulong behavior) noexcept {if(m_behavior != behavior) {m_behavior = behavior;}}
        
        //! Set if the attribute is visible or not.
        /** The function sets if the attribute is visible or not.
         @param state If true, the attribute will be invisible, otherwise it will be visible.
         */
        inline void setInvisible(const bool state) noexcept {state ? m_behavior |= Invisible : m_behavior &= ~Invisible;}
        
        //! Set if the attribute is disabled or not.
        /** The function sets if the attribute is disabled or not.
         @param state If true, the attribute will be disabled, otherwise it will be enabled.
         */
        inline void setDisabled(const bool state) noexcept {state ? m_behavior |= Disabled : m_behavior &= ~Disabled;}
        
        //! Set if the attribute is saved or not.
        /** The function sets if the attribute is saved or not.
         @param state If true, the attribute will be saved, otherwise it won't be saved.
         */
        inline void setUnsaved(const bool state) noexcept {state ? m_behavior |= Unsaved : m_behavior &= ~Unsaved;}
        
        //! Set if the attribute is notifier or not.
        /** The function sets if the attribute is notifier or not.
         @param state If true, the attribute will notify changes, otherwise it won't notify changes.
         */
        inline void setSilent(const bool state) noexcept {state ? m_behavior |= Silent : m_behavior &= ~Silent;};
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE TYPED                                 //
    // ================================================================================ //
    
    template <class T> class Attr::Typed : public Attr
    {
    private:
        friend class Attr::Manager;
        const T m_default;
        T       m_value;
        T       m_freezed;
    public:
        
        //! Constructor.
        /** You should never have to use the function.
         */
        inline Typed(sTag name, string const& label, string const& category, T const& value, const ulong behavior = 0, const ulong order = 0)  noexcept :
        Attr(name, label, category, behavior, order), m_default(value), m_value(value) {}
        
        //! Destructor.
        /** You should never have to use the function.
         */
        inline ~Typed() noexcept {}
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        inline type_index getTypeIndex() const noexcept override {return typeid(T);}
    
        //! Retrieves the values.
        /** The current values.
         @return The current values.
         */
        inline T get() const {return m_value;}
        
        //! Retrieves the default value.
        /** Retrieve the default value.
         @return The the default value.
         */
        inline T getDefault() const {return m_default;}
        
        //! Retrieve the frozen value.
        /** Retrieve the frozen value.
         @return The the frozen value.
         */
        inline T getFrozen() const {return m_freezed;}
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        Atom getValue() const noexcept override {return Atom(m_value);}
        
    private:
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        inline void set(T const& value){m_value = value;}
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        void setValue(Atom const& atom) override {m_value = atom;}
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        inline void freeze(const bool frozen) override {m_frozen = frozen; m_freezed = m_value;}
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  {setValue(m_default);}
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        inline void resetFrozen() override  {setValue(m_freezed);}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE LISTENER                              //
    // ================================================================================ //
    
    //! The attribute manager listener is a virtual class that can be binded to an attribute manager to be notified of various changes.
    /** The attribute manager listener is a very light class that allows to be notified of the attributes modification.
     */
    class Attr::Listener
    {
    public:
        virtual ~Listener() {}
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager     The attribute manager.
         @param attr		The attribute that has been modified.
         */
        virtual void attrChanged(Attr::sManager manager, sAttr attr) = 0;
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** The attribute manager manages a set of attributes, it allows the setting and the getting of their values and to retrieve them by name or by category.
     @see AttrTyped
     */
    class Attr::Manager : public inheritable_enable_shared_from_this<Manager>
    {
    private:
        struct SpecListener
        {
            wListener       listener;
            vector<sTag>    attrs;
            
            inline bool operator==(sListener list) const noexcept
            {
                sListener that = listener.lock();
                return list && that && list == that;
            }
            
            inline operator bool() const noexcept
            {
                sListener that = listener.lock();
                return bool(that);
            }
        };
        
        map<sTag, sAttr>                m_attrs;
        mutable mutex                   m_mutex;
        set<SpecListener>               m_list;
        mutex                           m_list_mutex;
    public:
        
        //! Constructor.
        /** Creates a new attribute manager.
         */
        inline Manager() noexcept {};
        
        //! Destructor.
        /** Free the attributes.
         */
        virtual inline ~Manager() noexcept
        {
            {
                lock_guard<mutex> guard(mutex);
                m_attrs.clear();
            }
            {
                lock_guard<mutex> guard(m_list_mutex);
                m_list.clear();
            }
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute as a vector or an empty vector if the attribute doesn't exist.
         */
        inline Atom getAttrValue(const sTag name) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                return it->second->getValue();
            }
            return Atom();
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute or a default value if the attribute doesn't exist.
         */
        template<class T> inline T getAttrValue(const sTag name) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                shared_ptr<Typed<T>> attr = dynamic_pointer_cast<Typed<T>>(it->second);
                if(attr)
                {
                    return attr->get();
                }
            }
            return T();
        }
		
        //! Set an attribute value.
        /** The function sets an attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        inline void setAttrValue(const sTag name, Atom const& atom) noexcept
        {
            sAttr attr;
            {
                lock_guard<mutex> guard(m_mutex);
                auto it = m_attrs.find(name);
                if(it != m_attrs.end() && atom != it->second->getValue())
                {
                    attr = it->second;
                    attr->setValue(atom);
                    this->notify(attr);
                }
            }
            if(attr)
            {
                lock_guard<mutex> guard(m_list_mutex);
                auto it2 = m_list.begin();
                while(it2 != m_list.end())
                {
                    sListener listener = (*it2).listener.lock();
                    if(listener)
                    {
                        if((*it2).attrs.empty() || find((*it2).attrs.begin(), (*it2).attrs.end(), name) != (*it2).attrs.end())
                        {
                            listener->attrChanged(shared_from_this(), attr);
                        }
                        ++it2;
                    }
                    else
                    {
                        it2 = m_list.erase(it2);
                    }
                }
            }
        }
        
		//! Set an attribute value.
		/** The function sets an attribute value.
		 @param name the name of the attribute.
		 @param value The new attribute value.
		 */
		template<class T> inline void setAttrValue(const sTag name, T const& value) noexcept
		{
            shared_ptr<Typed<T>> attr;
            {
                lock_guard<mutex> guard(m_mutex);
                auto it = m_attrs.find(name);
                if(it != m_attrs.end())
                {
                    attr = dynamic_pointer_cast<Typed<T>>(it->second);
                    if(attr && attr->get() != value)
                    {
                        attr->set(value);
                        this->notify(attr);
                    }
                    else
                    {
                        attr = nullptr;
                    }
                }
            }
            if(attr)
            {
                lock_guard<mutex> guard(m_list_mutex);
                auto it2 = m_list.begin();
                while(it2 != m_list.end())
                {
                    sListener listener = (*it2).listener.lock();
                    if(listener)
                    {
                        if((*it2).attrs.empty() || find((*it2).attrs.begin(), (*it2).attrs.end(), name) != (*it2).attrs.end())
                        {
                            
                            listener->attrChanged(shared_from_this(), attr);
                        }
                        ++it2;
                    }
                    else
                    {
                        it2 = m_list.erase(it2);
                    }
                }
            }
		}
        
        //! Write the attributes in a dico.
        /** The function writes the attributes in a dico.
         @param dico The dico.
         */
        void write(Dico& dico) const;
        
        //! Write the attributes in a dico.
        /** The function writes the attributes in a dico.
         @param dico The dico.
         */
        void read(Dico const& dico);
        
        //! Add an attribute listener in the binding list of the attribute manager.
        /** The function adds an attribute listener in the binding list of the attribute manager. The attribute listener can specifies the names of the attributes, an empty vector means it will be attached to all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void addListener(sListener listener, sTag name);
        
        //! Add an attribute listener in the binding list of the attribute manager.
        /** The function adds an attribute listener in the binding list of the attribute manager. The attribute listener can specifies the names of the attributes, an empty vector means it will be attached to all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void addListener(sListener listener, vector<sTag> const& names = vector<sTag>());
        
        //! Remove an instance listener from the binding list of the attribute.
        /** The function removes an instance listener from the binding list of the attribute. The attribute listener can specifies the names of the attributes, an empty vector means it will be detached from all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void removeListener(sListener listener, vector<sTag> const& names = vector<sTag>());
        
    protected:
        
        //! Notify the manager that the values of an attribute has changed.
        /** The function notifies the manager that the values of an attribute has changed.
         @param attr An attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        virtual bool notify(sAttr attr) {return true;};
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline void createAttr(sTag name, string const& label, string const& category, T const& value, const ulong behavior = 0, const ulong order = 0)
        {
            sAttr attr = make_shared<Typed<T>>(name, label, category, value, behavior, order);
            if(attr)
            {
                m_attrs[name] = attr;
            }
        }
    };

    typedef shared_ptr<Attr::Typed<bool>>		sAttrBool;
    typedef shared_ptr<Attr::Typed<long>>		sAttrLong;
    typedef shared_ptr<Attr::Typed<double>>     sAttrDouble;
    typedef shared_ptr<Attr::Typed<sTag>>       sAttrTag;
}

#endif


