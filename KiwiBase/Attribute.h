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

#ifndef __DEF_KIWI_ATTRIBUTE__
#define __DEF_KIWI_ATTRIBUTE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Dico.h"
#include "Event.h"
#include "Doodle.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
	/** The attribute manages a set of values that should be displayed in an inspector. The setter and getter must be override.
     */
    class Attr
    {
    public:
        class Manager;
        typedef shared_ptr<Attr::Manager> sManager;
		typedef shared_ptr<const Attr::Manager> scManager;
        
        class Listener;
        typedef shared_ptr<Listener>    sListener;
        typedef weak_ptr<Listener>      wListener;
        
        /** Flags describing the type of the notification
         @see Listener
         */
        enum Notification
        {
            Added           = 1<<1,     ///< Indicates that an attribute has been added.
            Removed         = 1<<2,     ///< Indicates that an attribute has been removed.
            ValueChanged	= 1<<3,     ///< Indicates that an attribute value has changed.
            BehaviorChanged	= 1<<4,		///< Indicates that the behavior of an attribute has changed.
            Anything        = 1<<1 | 1<<2 | 1<<3 | 1<<4,
        };
        
		/** Flags describing the behavior of the attribute.
		 @see setInvisible, setDisabled, setSaveable, setNotifyChanges
		 */
        enum Behavior
        {
            Invisible			= 1<<0,///< Indicates that the attribute is invisible.
			Disabled			= 1<<1,///< Indicates that the attribute can't be changed.
            Unsaved             = 1<<2,///< Indicates that the attribute is not saved.
            Notifier            = 1<<3,///< Indicates that the attribute should not notify its changes.
            All                 = Invisible | Disabled | Unsaved | Notifier
        };
        
		/** Flags describing the display style of the attribute.
		 @see setStyle
		 */
		enum Style
		{
			Default			= 0,	///< Indicates that the attribute should be displayed in a default style depending on its Type.
			Text			= 1,	///< Indicates that the attribute should be displayed in a text style
			List			= 2,	///< Indicates that the attribute should be displayed in a list of same or mixed elements style
			Enum			= 3,	///< Indicates that the attribute should be displayed in an enum style
			NumberLong		= 4,	///< Indicates that the attribute should be displayed in a number style (without floating point)
			NumberDouble	= 5,	///< Indicates that the attribute should be displayed in a number style (with floating point)
			Toggle			= 6,	///< Indicates that the attribute should be displayed in a onoff toggle style
			Color			= 7,	///< Indicates that the attribute should be displayed in a color style
			Filepath		= 8		///< Indicates that the attribute should be displayed in a filepath style
		};
		
    private:
        static const sTag frozen_attributes;
        
        const sTag          m_name;				///< The name of the attribute.
        const sTag          m_label;			///< The label of the attribute.
        const sTag          m_category;			///< The category of the attribute.
        const Style         m_style;			///< The style of the attribute.
		const long			m_order;			///< The order of the attribute.
		ElemVector          m_default_values;	///< The default value of the attribute.
        atomic_long         m_behavior;			///< The behavior of the attribute.
		ElemVector          m_frozen_values;    ///< The frozen value of the attribute.

    public:
		
        //! Constructor.
        /** Allocate and initialize the member values.
		 @param name			The name of the attribute (usually only letters and undescore characters).
		 @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
		 @param style			The style of the attribute specified in the Attr::Style enum.
         @param defaultValues	The default values.
		 @param behavior		A combination of the flags which define the attribute's behavior.
		 @param order			The attribute order.
         */
        Attr(sTag name, sTag label, sTag category, Style style = Style::Default, ElemVector defaultValues = {}, long behavior = 0, long order = 0);
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual ~Attr();
        
        //! Attribute maker.
		/** The function creates an attribute with arguments.
		 */
		template<class AttrClass, class ...Args> static shared_ptr<AttrClass> create(Args&& ...arguments)
		{
            shared_ptr<AttrClass> attr = make_shared<AttrClass>(forward<Args>(arguments)...);
            if(attr)
            {
                attr->setDefaultValues();
            }
			return attr;
        }
        
        //! Attribute maker.
		/** The function creates an attribute.
		 */
		template<class AttrClass> static shared_ptr<AttrClass> create()
		{
			shared_ptr<AttrClass> attr = make_shared<AttrClass>();
            if(attr)
            {
                attr->setDefaultValues();
            }
			return attr;
		}
		
		//! Retrieve the name of the attribute.
		/** The function retrieves the name of the attribute.
		 @return The name of the attribute.
		 */
		inline sTag getName() const noexcept
        {
            return m_name;
        }
		
		//! Retrieve the attribute label.
		/** The function retrieves the attribute label.
		 @return The attribute label.
		 */
		inline sTag getLabel() const noexcept
        {
            return m_label;
        }
        
        //! Retrieve the attribute category.
		/** The function retrieves the attribute category.
		 @return The attribute category.
		 */
		inline sTag getCategory() const noexcept
        {
            return m_category;
        }
		
		//! Retrieve the attribute order.
		/** The function retrieves the attribute order.
		 @return The attribute order.
		 */
		inline long getOrder() const noexcept
		{
			return m_order;
		}
		
        //! Retrieves the whole behavior flags field of the attribute.
		/** The function retrieves the whole behavior flags field of the attribute.
		 @return behavior	A combination of the flags which define the attribute's behaviors.
		 */
		inline long getBehavior() const noexcept
        {
            return m_behavior;
        }
		
		//! Retrieve the attribute style.
		/** The function retrieves the attribute style.
		 @return The attribute style.
		 */
		inline Style getStyle() const noexcept
        {
            return m_style;
        }
        
        //! Retrieve if the attribute is invisible.
		/** The function retrieves if the attribute is invisible.
		 @return True if the attribute is invisible otherwise false.
		 */
		inline bool isInvisible() const noexcept
        {
            return m_behavior & Invisible;
        }
		
		//! Retrieve if the attribute is disable.
		/** The function retrieves if the attribute is disable.
		 @return True if the attribute is disabled otherwise false.
		 */
		inline bool isDisabled() const noexcept
        {
            return m_behavior & Disabled;
        }
		
		//! Retrieve if the attribute is saved.
		/** The function retrieves if the attribute is saved.
		 @return True if the attribute is saveable otherwise false.
		 */
		inline bool isSaved() const noexcept
        {
            return !(m_behavior & Unsaved);
        }
		
		//! Retrieve if the attribute should notify changes.
		/** The function retrieves if the attribute should notify changes.
		 @return True if the attribute should notify changes otherwise false.
		 */
		inline bool isNotifier() const noexcept
        {
            return !(m_behavior & Notifier);
        }
		
		//! Retrieve if the attribute is frozen.
		/** The function retrieves if the attribute is frozen.
		 @return True if the attribute is frozen, false otherwise.
		 */
		bool isFrozen() const noexcept
        {
            return !m_frozen_values.empty();
        }
        
        //! Retrieves the default values.
		/** Retrieve the default values.
		 @param elements A vector of elements to fill.
		 */
		inline void getDefaultValues(ElemVector& elements) const noexcept
        {
            elements = m_default_values;
        }
        
		//! Retrieve the frozen value.
		/** Retrieve the frozen value, if the attribute is not frozen the vector will be empty.
		 @param elements A vector of Element to be replaced by the frozen value.
		 */
		inline void getFrozenValues(ElemVector& elements) const noexcept
        {
            elements = m_frozen_values;
        }
        
        //! Retrieves the enum values.
		/** The Attr subclasses than want to be displayed in an enum style must implement this function to retrieve enum values.
		 @param elements A vector of elements to fill.
		 */
		virtual void getEnumValues(ElemVector& elements) const
        {
            elements.clear();
        }
        
        //! Retrieves the values.
		/** The attribute must implement this function to retrieve the values.
		 @param elements A vector of elements to fill.
		 @see set
		 */
		virtual void get(ElemVector& elements) const noexcept = 0;
		
		//! Attempts to write the attribute in a dico.
		/** The function attempts to write the attribute in a dico.
		 If the behavior of the attribute is to not save the value and the attribute is not frozen, nothing will happen
		 @param dico The dico to write into.
		 @see read
		 */
		void write(sDico dico) const noexcept;
	
	protected:
		
		//! Sets the values with a vector of elements.
		/** The attribute must implement this function to set the values.
		 @param elements The vector of elements.
		 @see get
		 */
		virtual void set(ElemVector const& elements) = 0;
		
		//! Resets the attribute values to default values.
		/** Resets the attribute values to its default values.
		 */
		void setDefaultValues();
        
        //! Resets the attribute values to default values.
		/** Resets the attribute values to its default values.
		 */
		void setDefaultValues(ElemVector const& elements);
        
        //! Resets the attribute values to frozen values.
		/** Resets the attribute values to its frozen values.
		 */
		void setFrozenValues();
        
        //! Freezes or unfreezes the attribute.
		/** If you freeze an attribute, it will stores its current value as the saved value. When an attribute is frozen it can still be changed, but when the attribute will be saved it will take the frozen value rather than the current one.
		 @param frozen If true the attribute will be frozen, if false it will be unfrozen.
		 */
		void freeze(bool frozen);
        
        //! Read the attribute in a dico.
        /** The function reads the attribute in a dico.
         @param dico The dico.
         */
        void read(scDico dico);
		
        //! Set the whole behavior flags field of the attribute.
        /** The function sets the whole behavior flags field of the attribute.
		 @param behavior	A combination of the flags which define the attribute's behaviors.
		 */
		void setBehavior(long behavior) noexcept;
        
        //! Set if the attribute is visible or not.
		/** The function sets if the attribute is visible or not.
		 @param invisible If true, the attribute will be invisible, if false it will be visible.
		 */
		void setInvisible(bool invisible) noexcept;
		
        //! Set if the attribute is disabled or not.
		/** The function sets if the attribute is disabled or not.
		 @param disable If true, the attribute will be disabled, if false it will be enabled.
		 */
		void setDisabled(bool disable) noexcept;
		
        //! Set if the attribute is saved or not.
		/** The function sets if the attribute is saved or not.
		 @param disable If true, the attribute will be saved, if false it won't be saved.
		 */
		void setSaved(bool saved) noexcept;
		
		//! Set if the attribute is notifier or not.
		/** The function sets if the attribute is notifier or not.
		 @param disable If true, the attribute will be notify changes, if false it won't notify changes.
		 */
		void setNotifier(bool notifier) noexcept;
        
    public:
        
        //! Attribute comparaison.
        /** This function compare the attributes by their order or by the name of their label.
         @param  attr1  The first attribute.
         @param  attr2  The second attribute.
         @return true if the the first attribute before the sedond attribute.
         */
        static inline bool compareOrderOrLabel(sAttr attr1, sAttr attr2)
        {
            if(attr1->getCategory() == attr2->getCategory())
            {
                if(attr1->getOrder() > 0 && attr2->getOrder() > 0)
                {
                    return attr1->getOrder() < attr2->getOrder();
                }
            }
            return attr1->getLabel()->getName() < attr2->getLabel()->getName();
        }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** The attribute manager manages a set of attributes, it allows the setting and the getting of their values and to retrieve them by name or by category.
     @see Attr
     */
    class Attr::Manager : public enable_shared_from_this<Attr::Manager>
    {
    private:
        struct ListenerAttrList
        {
            map<sTag, ulong> attrs;
            
            ListenerAttrList(sTag name, ulong notification)
            {
                attrs[name] = notification;
            }
            
            ~ListenerAttrList()
            {
                attrs.clear();
            }
        };
        
        unordered_map<sTag, sAttr>  m_attrs;
        mutable mutex               m_attrs_mutex;
        map<wListener,
        ListenerAttrList,
        owner_less<wListener>>      m_listeners;
        mutex                       m_listeners_mutex;
        
    public:
        
        //! Constructor.
        /** Creates a new attribute manager.
         */
        Manager() noexcept;
        
        //! Descrutor.
        /** Free the attributes.
         */
        virtual ~Manager();
        
        //! Add an attribute.
        /** The function adds an attribute .
         @param attr the attribute to add.
         */
        void addAttribute(sAttr attr);
        
    protected:
        
        //! Remove an attribute.
        /** The function removes an attribute.
         @param attr The attribute to remove.
         */
        void removeAttribute(sAttr attr);
        
        //! The an attribute.
        /** The function removes an attribute.
         @param name The name of the attribute to remove.
         */
        void removeAttribute(sTag name);
        
        //! Set the attribute's default values.
        /** The function sets the attribute's  default values.
         @param name The name of the attribute.
         @param elements The new default values of the attribute.
         */
        void setAttributeDefaultValues(sTag name, ElemVector const& elements);
        
        //! Set the attribute behavior.
        /** The function sets the attribute behaviors.
         @param name The name of the attribute.
         @param behavior The behavior of the attribute.
         */
        void setAttributeBehavior(sTag name, Attr::Behavior behavior);
        
        //! Set if the attribute should be visible.
        /** The function sets if the attribute should be visible.
         @param invisible If true, the attribute will be invisible, if false it will be visible.
         */
        void setAttributeInvisible(sTag name, bool invisible) noexcept;
        
        //! Set if the attribute should be disabled.
        /** The function sets if the attribute should be disabled.
         @param disabled If true, the attribute will be disable, if false it will be enable.
         */
        void setAttributeDisabled(sTag name, bool disabled) noexcept;
        
        //! Set if the attribute should be saved.
        /** The function sets if the attribute should be saved.
         @param saved If true, the attribute will be saved, if false it won't be saved.
         */
        void setAttributeSaved(sTag name, bool saved) noexcept;
        
        //! Set if the attribute should notify changes.
        /** The function sets if the attribute should notify changes.
         @param shouldNotify If true, the attribute will notify changes, if false it won't notify changes.
         */
        void setAttributeNotifier(sTag name, bool notifier) noexcept;
        
        //! Set the attributes values with a dico.
        /** The function sets the attributes values with a dico.
         @param dico A dico.
         */
        void read(scDico dico) noexcept;
        
    public:
        
        //! Retrieve the number of attributes.
        /** The function retrieves the numbers of attributes. The attributes invisibles won't be counted.
         @return The number of attributes.
         */
        ulong getNumberOfAttributes() const noexcept;
        
        //! Retrieve the names of the attributes.
        /** The function retrieves the names of the attributes. The name attributes invisibles won't be retrieved.
         @param names A vector of tags that will contain the names of the attributes;
         */
        void getAttributeNames(vector<sTag>& names) const noexcept;
        
        //! Check if a given attribute exist.
        /** The function checks if a given attribute exist. If the attribute is invisible the function returns false.
         @param name The name of the attribute.
         @return true if an attribute exist, otherwise false.
         */
        bool hasAttribute(sTag name) const noexcept;
        
        //! Retrieve an attribute.
        /** The function retrieves an attribute. If the attribute is invisible the function returns a pointer null.
         @param name The name of the attribute.
         @return The attribute or null if the attribute doesn't exist.
         */
        sAttr getAttribute(sTag name) const noexcept;
        
        //! Set the values of an attribute.
        /** The function sets the value of an attribute.
         @param name		The name of the attribute.
         @param elements    A vector of elements to pass.
         @return true if the attribute value has setted its values, otherwise false.
         @see getAttributeValue
         */
        bool setAttributeValue(sTag name, ElemVector const& elements);
        
        //! Get the values of an attribute.
        /** The function gets the values of an attribute.
         @param name        The name of the attribute.
         @param elements    A vector of elements to pass.
         @return true if the attribute value retrieved its values, otherwise false.
         @see setAttrValue
         */
        bool getAttributeValue(sTag name, ElemVector& elements);
        
        //! Retrieve the number of attribute categories.
        /** The function retrieves the number of attribute categories. If a category have only invisibles attributes, the category won't be counted.
         @return The number of attribute categories.
         */
        ulong getNumberOfCategories() const noexcept;
        
        //! Retrieve the names of the categories.
        /** The function retrieves the names of the categories. If a category have only invisibles attributes, the name of the category won't be retrieved.
         @param names				A vector of tag containing the names of the categories.
         @param sortAlphabetically	If true, categories name will be sorted alphabetically.
         */
        void getCategoriesNames(vector<sTag>& names, bool sortAlphabetically = false) const noexcept;
        
        //! Check if a category exist.
        /** The function checks if a category exist.  If a category have only invisibles attributes, the function returns false.
         @param name The name of the category.
         @return true if the category exist, otherwise false.
         */
        bool hasCategory(sTag name) const noexcept;
        
        //! Retrieve the attributes of a category.
        /** The function retrieves the attributes of a category. If an attribute is invisible, the attribute won't be retrieved.
         @param name	The name of the category.
         @param attrs	A vector of attributes to pass.
         @param sorted	If true, attributes will be sorted alphabetically and according to their order.
         */
        void getAttributesInCategory(sTag name, vector<sAttr>& attrs, bool sorted = false) const;
        
        //! Write the attributes in a dico.
        /** The function writes the attributes in a dico.
         @param dico A dico.
         */
        void write(sDico dico) const noexcept;
        
        //! Notify the manager that the values of an attribute has changed.
        /** The function notifies the manager that the values of an attribute has changed.
         @param attr An attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        virtual bool attributeChanged(sAttr attr)
        {
            return true;
        }
        
        //! Adds an attribute manager listener in the binding list of the attribute manager.
        /** The function adds an attribute manager listener in the binding list of the attribute manager. If the attribute manager listener is already in the binding list, the function has no effect.
         @param listener  The pointer of the attribute manager listener.
         @see              unbind()
         */
        void bind(sListener listener, sTag name = nullptr, Notification type = Anything);
        
        //! Removes an attribute manager listener from the binding list of the attribute manager.
        /** The function removes an attribute manager listener from the binding list of the attribute manager. If the attribute manager listener was not in the binding list, the function has no effect.
         @param listener  The pointer of the attribute manager listener.
         @see              unbind()
         */
        void unbind(sListener listener, sTag name = nullptr, Notification type = Anything);
        
    private:
        
        //! @internal Trigger notification to subclasses and listeners.
        void send(sAttr attr, Notification type);
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
         @param manager		The manager that manages the attribute.
         @param attr		The attribute that has been modified.
         @param type		The type of notification.
         */
        virtual void notify(sManager manager, sAttr attr, Notification type) = 0;
        
    };
	
	
    
    // ================================================================================ //
    //                                      ATTRIBUTE TYPED                             //
    // ================================================================================ //
    
    //! The bool attribute is an attribute that holds a boolean value.
    /** The bool attribute holds a boolean value. Its default display style is obviously an Attr::Style::Toggle.
     @see Attr.
     */
    class AttrBool : public Attr
    {
    private:
        bool m_value;
    public:
        AttrBool(sTag name, sTag label, sTag category, bool default_value = 0, long behavior = 0) :
        Attr(name, label, category, Attr::Style::Toggle, {(long)default_value}, behavior) {;}
        virtual ~AttrBool() {};
        virtual void get(ElemVector& elements) const noexcept;
        virtual void set(ElemVector const& elements) override;
        inline bool get() const noexcept
        {
            return m_value;
        }
    };

    //! The bool attribute is an attribute that holds a long value.
    /** The bool attribute holds a long value. Its default display style is obviously an Attr::Style::Number
	 @see Attr.
	 */
    class AttrLong : public Attr
    {
    private:
        long m_value;
    public:
		AttrLong(sTag name, sTag label, sTag category, long default_value = 0, long behavior = 0) :
        Attr(name, label, category, Attr::Style::NumberLong, {default_value}, behavior) {;}
		virtual ~AttrLong() {};
        virtual void get(ElemVector& elements) const noexcept;
        virtual void set(ElemVector const& elements) override;
        inline long get() const noexcept
        {
            return m_value;
        }
    };
	
    //! The bool attribute is an attribute that holds a double value.
    /** The bool attribute holds a double value. Its default display style is obviously an Attr::Style::Number
	 @see Attr.
	 */
    class AttrDouble : public Attr
    {
    private:
        double m_value;
    public:
		AttrDouble(sTag name, sTag label, sTag category, double default_value = 0, long behavior = 0) :
        Attr(name, label, category, Attr::Style::NumberDouble, {default_value}, behavior) {;}
		virtual ~AttrDouble() {};
        virtual void get(ElemVector& elements) const noexcept;
        virtual void set(ElemVector const& elements) override;
        inline double get() const noexcept
        {
            return m_value;
        }
    };
	
	//! The bool attribute is an attribute that holds a tag.
    /** The bool attribute holds a tag. Its default display style is obviously an Attr::Style::Text
	 @see Attr.
	 */
    class AttrTag : public Attr
    {
    private:
        sTag m_value;
    public:
		AttrTag(sTag name, sTag label, sTag category, sTag default_value = nullptr, long behavior = 0) :
        Attr(name, label, category, Attr::Style::Text, {default_value}, behavior) {;}
		virtual ~AttrTag() {};
        virtual void get(ElemVector& elements) const noexcept;
        virtual void set(ElemVector const& elements) override;
        inline sTag get() const noexcept
        {
            return m_value;
        }
    };
	
	//! The enum attribute is an attribute that represent an enumeration.
	/** The enum attribute holds a long value and represents a position in an enumeration. The value can be set either by a number or a Tag, and will be clipped between 0 and the size of the enumeration. Its default display style is obviously an Attr::Style::Enum.
     @see Attr.
	 */
	class AttrEnum : public Attr
	{
	private:
        ElemVector::size_type m_value;
		ElemVector            m_enum_values;
	public:
		AttrEnum(sTag name, sTag label, sTag category, ElemVector const& enum_vals, Element const& default_value = 0, long behavior = 0) :
        Attr(name, label, category, Attr::Style::Enum, {default_value}, behavior), m_enum_values(enum_vals) {;}
		virtual ~AttrEnum() {};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
		virtual void getEnumValues(ElemVector& elements) const
        {
            elements = m_enum_values;
        }
        inline ulong get() const noexcept
        {
            return (ulong)m_value;
        }
	};
	
	//! The color attribute is an attribute that holds a color value.
	/** The color attribute holds a four double values suitable to represent a RGBA type color. Its default display style will obviously be a Attr::Style::Color. Each value of the vector will be clipped between 0 and 1.
	 @see Attr.
	 */
	class AttrColor : public Attr
	{
	private:
        Gui::Color m_value;
	public:
		AttrColor(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0., 0., 1.}, long behavior = 0) :
        Attr(name, label, category, Attr::Style::Color, {default_value}, behavior) {;}
		virtual ~AttrColor(){};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
        
        inline Gui::Color get() const noexcept
        {
            return m_value;
        }
	};
	
    //! The point attribute is an attribute that is particulary suitable to represent a position.
	/** The point attribute holds two double values suitable to represent a point, its default display style will obviously be a Attr::Style::List.
	 @see Attr
	 */
	class AttrPoint : public Attr
	{
	protected:
        Gui::Point m_value = {0., 0.};
	public:
		AttrPoint(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0.}, long behavior = 0) :
		Attr(name, label, category, Attr::Style::List, {default_value}, behavior) {;}
		virtual ~AttrPoint() {};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
        
        inline Gui::Point get() const noexcept
        {
            return m_value;
        }
	};
	
	//! The size attribute is a point attribute that is particulary suitable to represent a size.
	/** The size attribute holds two double values suitable to represent a size
	 @see Attr
	 */
	class AttrSize : public AttrPoint
	{
	private:
        Gui::Point	m_min_limits = {5., 5.};
		Gui::Point	m_max_limits = {0., 0.};
		double	m_ratio = 0.;
		
		void clipValue() noexcept
		{
			m_value.x(m_max_limits.x() > 0. ? clip(m_value.x(), m_min_limits.x(), m_max_limits.x()) : max(m_value.x(), m_min_limits.x()));
			m_value.y(m_max_limits.y() > 0. ? clip(m_value.y(), m_min_limits.y(), m_max_limits.y()) : max(m_value.y(), m_min_limits.y()));
		}
		
	public:
		AttrSize(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0.}, long behavior = 0) :
		AttrPoint(name, label, category, {default_value}, behavior) {;}
		virtual ~AttrSize() {};
		virtual void set(ElemVector const& elements) override;
		virtual void get(ElemVector& elements) const noexcept;
		inline Gui::Point get() const noexcept
		{
			return m_value;
		}
		
		//! Sets a minimum width and height limit.
        void setMinLimits(Gui::Point const& minLimits) noexcept
		{
			if(minLimits != m_min_limits)
			{
				m_min_limits.x(m_max_limits.x() > 0. ? min(max(minLimits.x(), 0.), m_max_limits.x()) : max(minLimits.x(), 0.));
				m_min_limits.y(m_max_limits.y() > 0. ? min(max(minLimits.y(), 0.), m_max_limits.y()) : max(minLimits.y(), 0.));
				setMaxLimits(m_max_limits);
			}
		}
		
		//! Sets a maximum width and height limit.
		/** Pass a 0 point if you don't want to limit width or height.
		 @param maxLimits The maximum width and height limit.
		 */
        void setMaxLimits(Gui::Point const& maxLimits) noexcept
		{
			m_max_limits.x(maxLimits.x() > 0. ? max(maxLimits.x(), m_min_limits.x()) : 0.);
			m_max_limits.y(maxLimits.y() > 0. ? max(maxLimits.y(), m_min_limits.y()) : 0.);
		}
		
		//! Retrieves the minimum width and height limit.
        Gui::Point getMinLimits() const noexcept
		{
			return m_min_limits;
		}
		
		//! Retrieves the maximum width and height limit.
		Gui::Point getMaxLimits() const noexcept
		{
			return m_max_limits;
		}
		
		//! Specifies a width-to-height ratio that the box should always maintain when it is resized.
		/** If the value is 0, no aspect ratio is enforced. If it's non-zero, the width
		 will always be maintained as this multiple of the height.
		 @see setSizeLimits
		 */
		void setSizeRatio(const double widthOverHeight) noexcept
		{
			m_ratio = max(widthOverHeight, 0.);
		}
		
		//! Returns the aspect ratio that was set with setSizeRatio().
		/** If no aspect ratio is being enforced, this will return 0.
		 */
		double getSizeRatio() const noexcept
		{
			return m_ratio;
		}
	};
	
	//! The rectangle attribute is an attribute that is particulary suitable to represent a position and a size.
	/** The rectangle attribute holds four double values suitable to represent a rectangle, its default display style will obviously be a Attr::Style::List.
	 @see Attr
	 */
	class AttrRect : public Attr
	{
	private:
		Gui::Rectangle m_value;
	public:
		AttrRect(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0., 0., 0.}, long behavior = 0) :
		Attr(name, label, category, Attr::Style::List, {default_value}, behavior) {;}
		virtual ~AttrRect() {};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
	};
    
    struct AttrFont
    {
        const shared_ptr<AttrTag>       name;
        const shared_ptr<AttrDouble>    size;
        const shared_ptr<AttrEnum>      face;
        const shared_ptr<AttrEnum>      justification;
        
        AttrFont() :
        name(Attr::create<AttrTag>(Tag::List::fontname, Tag::List::Font_Name, Tag::List::Font, Tag::List::Menelo)),
        size(Attr::create<AttrDouble>(Tag::List::fontsize, Tag::List::Font_Size, Tag::List::Font, 13)),
        face(Attr::create<AttrEnum>(Tag::List::fontface, Tag::List::Font_Face, Tag::List::Font, (ElemVector){Tag::List::normal, Tag::List::bold, Tag::List::italic, Tag::List::bold_italic}, 0)),
        justification(Attr::create<AttrEnum>(Tag::List::fontjustification, Tag::List::Font_Justification, Tag::List::Font, (ElemVector){Tag::List::left, Tag::List::center, Tag::List::right}, 0))
        {
            ;
        }
        
        //! Retrieve the font.
        /** The function retrieves font.
         @return The font.
         */
        inline Gui::Font getFont() const noexcept
        {
            return Gui::Font(toString(name->get()), size->get(), (Gui::Font::Style)face->get());
        }
        
        //! Retrieve the font justification.
        /** The function retrieves the font justification.
         @return The font justification.
         */
        inline Gui::Font::Justification getFontJustification() const noexcept
        {
            return (Gui::Font::Justification)justification->get();
        }
        
    };
    
    // ================================================================================ //
    //                                 ATTRIBUTE PAGE                                   //
    // ================================================================================ //
    
    //! The AttrPage class manages attributes of a page.
    /** The AttrPage class manages attributes of a page.
     */
    
    class AttrPage : public Attr::Manager
    {
    public:
        // Color //
        static const sTag Tag_editing_bgcolor;
        static const sTag Tag_Unlocked_Background_Color;
        static const sTag Tag_locked_bgcolor;
        static const sTag Tag_Locked_Background_Color;
        
        // Editing //
        static const sTag Tag_Editing;
        static const sTag Tag_gridsize;
        static const sTag Tag_Grid_Size;
        
        //-----------------------------------------------------
    protected:
        const AttrFont attr_font;
        const shared_ptr<AttrColor>     attr_color_editing_background;
        const shared_ptr<AttrColor>     attr_color_locked_background;
        const shared_ptr<AttrLong>		attr_editing_grid_size;
        
    public:
        AttrPage() :
        attr_color_editing_background(Attr::create<AttrColor>(Tag_editing_bgcolor, Tag_Unlocked_Background_Color, Tag::List::Color, (ElemVector){0.88, 0.89, 0.88, 1.})),
        attr_color_locked_background(Attr::create<AttrColor>(Tag_locked_bgcolor, Tag_Locked_Background_Color, Tag::List::Color, (ElemVector){0.88, 0.89, 0.88, 1.})),
        attr_editing_grid_size(Attr::create<AttrLong>(Tag_gridsize, Tag_Grid_Size, Tag_Editing, 15))
        {
            addAttribute(attr_font.name);
            addAttribute(attr_font.size);
            addAttribute(attr_font.face);
            addAttribute(attr_font.justification);
            addAttribute(attr_color_editing_background);
            addAttribute(attr_color_locked_background);
            addAttribute(attr_editing_grid_size);
        }
        
        ~AttrPage()
        {
            ;
        }
        
        //! Retrieve the default font for boxes of the page.
        /** The function retrieves the default font for boxes of the page.
         @return The default font for boxes of the page.
         */
        inline Gui::Font getFont() const noexcept
        {
            return attr_font.getFont();
        }
        
        //! Retrieve the default font justification for boxes of the page.
        /** The function retrieves the default font justification for boxes of the page.
         @return The default font justification for boxes of the page.
         */
        inline Gui::Font::Justification getFontJustification() const noexcept
        {
            return attr_font.getFontJustification();
        }
        
        //! Retrieve if the background color of the page when unlocked.
        /** The function retrieves the background color of the page when unlocked
         @return The Unlocked background color of the page.
         */
        inline Gui::Color getEditingBgColor() const noexcept
        {
            return attr_color_editing_background->get();
        }
        
        //! Retrieve if the locked background color.
        /** The function retrieves the locked background color of the page.
         @return The locked background color of the page.
         */
        inline Gui::Color getLockedBgColor() const noexcept
        {
            return attr_color_locked_background->get();
        }
        
        //! Retrieve the grid size attribute value.
        /** The function retrieve the grid size attribute value.
         @return The grid size attribute value.
         */
        inline long getGridSize() const noexcept
        {
            return attr_editing_grid_size->get();
        }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE BOX                                   //
    // ================================================================================ //
    
    //! The font attribute is the default attributes for the font.
    /** Holds a vector of two double values suitable to represent a position or a size,
     */
    
    class AttrBox : public Attr::Manager
    {
    public:
        // Internal //
        static const sTag Tag_ninlets;
        static const sTag Tag_noutlets;
        static const sTag Tag_id;
        static const sTag Tag_text;
        
        // Appearance //
        static const sTag Tag_Appearance;
        static const sTag Tag_position;
        static const sTag Tag_Position;
        static const sTag Tag_size;
        static const sTag Tag_Size;
        static const sTag Tag_presentation_position;
        static const sTag Tag_Presentation_Position;
        static const sTag Tag_presentation_size;
        static const sTag Tag_Presentation_Size;
        static const sTag Tag_hidden;
        static const sTag Tag_Hide_on_Lock;
        static const sTag Tag_presentation;
        static const sTag Tag_Include_in_Presentation;
        
        // Behavior //
        static const sTag Tag_Behavior;
        static const sTag Tag_ignoreclick;
        static const sTag Tag_Ignore_Click;
        
        // Color //
        static const sTag Tag_bgcolor;
        static const sTag Tag_Background_Color;
        static const sTag Tag_bdcolor;
        static const sTag Tag_Border_Color;
        static const sTag Tag_textcolor;
        static const sTag Tag_Text_Color;
        
        // Internal //
        const shared_ptr<AttrLong>      attr_ninlets;
        const shared_ptr<AttrLong>      attr_noutlets;
        const shared_ptr<AttrLong>      attr_id;
        const shared_ptr<AttrTag>       attr_text;
        
        // Appearance //
        const shared_ptr<AttrPoint>     attr_position;
        const shared_ptr<AttrSize>		attr_size;
        const shared_ptr<AttrPoint>     attr_presentation_position;
        const shared_ptr<AttrSize>		attr_presentation_size;
        const shared_ptr<AttrBool>      attr_hidden;
        const shared_ptr<AttrBool>      attr_presentation;
        
        // Behavior //
        const shared_ptr<AttrBool>		attr_ignoreclick;
        
        // Font //
        const shared_ptr<AttrTag>       attr_font_name;
        const shared_ptr<AttrDouble>    attr_font_size;
        const shared_ptr<AttrEnum>      attr_font_face;
        const shared_ptr<AttrEnum>      attr_font_justification;
        
        // Color //
        const shared_ptr<AttrColor>     attr_color_background;
        const shared_ptr<AttrColor>     attr_color_border;
        const shared_ptr<AttrColor>     attr_color_text;
        
        AttrBox() :
        // Internal //
        attr_ninlets(Attr::create<AttrLong>(Tag_ninlets, nullptr, nullptr, 0, Attr::Invisible)),
        attr_noutlets(Attr::create<AttrLong>(Tag_noutlets, nullptr, nullptr, 0, Attr::Invisible)),
        attr_id(Attr::create<AttrLong>(Tag_id, nullptr, nullptr, 0, Attr::Invisible | Attr::Disabled)),
        attr_text(Attr::create<AttrTag>(Tag_text, nullptr, nullptr, nullptr, Attr::Invisible | Attr::Disabled)),
        
        // Appearance //
        attr_position(Attr::create<AttrPoint>(Tag_position, Tag_Position, Tag_Appearance)),
        attr_size(Attr::create<AttrSize>(Tag_size, Tag_Size, Tag_Appearance, (ElemVector){100., 20.})),
        attr_presentation_position(Attr::create<AttrPoint>(Tag_presentation_position, Tag_Presentation_Position, Tag_Appearance)),
        attr_presentation_size(Attr::create<AttrSize>(Tag_presentation_size, Tag_Presentation_Size, Tag_Appearance)),
        attr_hidden(Attr::create<AttrBool>(Tag_hidden, Tag_Hide_on_Lock, Tag_Appearance, false)),
        attr_presentation(Attr::create<AttrBool>(Tag_presentation,  Tag_Include_in_Presentation, Tag_Appearance, false)),
        
        // Appearance //
        attr_ignoreclick(Attr::create<AttrBool>(Tag_ignoreclick, Tag_Ignore_Click, Tag_Behavior, false)),
        
        // Font //
        attr_font_name(Attr::create<AttrTag>(Tag::List::fontname, Tag::List::Font_Name, Tag::List::Font, Tag::List::Menelo)),
        attr_font_size(Attr::create<AttrDouble>(Tag::List::fontsize, Tag::List::Font_Size, Tag::List::Font, 13)),
        attr_font_face(Attr::create<AttrEnum>(Tag::List::fontface, Tag::List::Font_Face, Tag::List::Font, (ElemVector){Tag::List::normal, Tag::List::bold, Tag::List::italic, Tag::List::bold_italic}, 0)),
        attr_font_justification(Attr::create<AttrEnum>(Tag::List::fontjustification, Tag::List::Font_Justification, Tag::List::Font, (ElemVector){Tag::List::left, Tag::List::center, Tag::List::right}, 0)),
        
        // Color //
        attr_color_background(Attr::create<AttrColor>(Tag_bgcolor, Tag_Background_Color, Tag::List::Color, (ElemVector){1., 1., 1, 1.})),
        attr_color_border(Attr::create<AttrColor>(Tag_bdcolor, Tag_Border_Color, Tag::List::Color, (ElemVector){0.4, 0.4, 0.4, 1.})),
        attr_color_text(Attr::create<AttrColor>(Tag_textcolor, Tag_Text_Color, Tag::List::Color, (ElemVector){0.3, 0.3, 0.3, 1.}))
        {
            // Internal //
            addAttribute(attr_ninlets);
            addAttribute(attr_noutlets);
            addAttribute(attr_id);
            addAttribute(attr_text);
            
            // Appearance //
            addAttribute(attr_position);
            addAttribute(attr_size);
            addAttribute(attr_presentation_position);
            addAttribute(attr_presentation_size);
            addAttribute(attr_hidden);
            addAttribute(attr_presentation);
            
            // Appearance //
            addAttribute(attr_ignoreclick);
            
            // Font //
            addAttribute(attr_font_name);
            addAttribute(attr_font_size);
            addAttribute(attr_font_face);
            addAttribute(attr_font_justification);
            
            // Color //
            addAttribute(attr_color_background);
            addAttribute(attr_color_border);
            addAttribute(attr_color_text);
        }
        
        ~AttrBox()
        {
            ;
        }
        
        //! Retrieve the id of the box.
        /** The function retrieves the id of the box as a tag.
         @return The text of the id as a tag.
         */
        inline long getId() const noexcept
        {
            return attr_id->get();
        }
        
        //! Retrieve the text of the box.
        /** The function retrieves the text of the box as a tag.
         @return The text of the box as a tag.
         */
        inline sTag getText() const noexcept
        {
            return attr_text->get();
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Gui::Point getPosition(const bool presentation = false) const noexcept
        {
            if(!presentation)
			{
                return attr_position->get();
			}
			else
			{
				return attr_presentation_position->get();
			}
		}
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @param edition
         @return The size of the box as a point.
         */
        inline Gui::Point getSize(const bool presentation = false) const noexcept
        {
            if(!presentation)
			{
                return attr_size->get();
			}
			else
			{
				return attr_presentation_size->get();
			}
		}
        
        //! Retrieve the bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Gui::Rectangle getBounds(const bool presentation = false) const noexcept
        {
            if(!presentation)
			{
                return Gui::Rectangle(attr_position->get(), attr_size->get());
			}
            else
			{
                return Gui::Rectangle(attr_presentation_position->get(), attr_presentation_size->get());
			}
        }
        
        //! Sets a minimum and a maximum width and height limit.
        /** Pass a 0 point if you don't want to limit width or height.
         @param min The minimum width and height limit.
         @param max The maximum width and height limit.
         */
        inline void setSizeLimits(Gui::Point const& min, Gui::Point const& max = Gui::Point()) noexcept
        {
            attr_size->setMinLimits(min);
            attr_presentation_size->setMinLimits(min);
            attr_size->setMaxLimits(max);
            attr_presentation_size->setMaxLimits(max);
        }
        
        //! Retrieves the minimum width and height limit.
        /** The function retrieves the minimum width and height limit.
         @return The minimum width and height limit.
         */
        inline Gui::Point getSizeMinLimits() const noexcept
        {
            return attr_size->getMinLimits();
        }
        
        //! Retrieves the maximum width and height limit.
        /** The function retrieves the maximum width and height limit.
         @return The maximum width and height limit.
         */
        inline Gui::Point getSizeMaxLimits() const noexcept
        {
            return attr_size->getMaxLimits();
        }
        
        //! Specifies a width-to-height ratio that the box should always maintain when it is resized.
        /** If the value is 0, no aspect ratio is enforced. If it's non-zero, the width
         will always be maintained as this multiple of the height.
         @see setSizeLimits
         */
        inline void setSizeRatio(const double ratio) noexcept
        {
            attr_size->setSizeRatio(ratio);
            attr_presentation_size->setSizeRatio(ratio);
        }
        
        //! Retrieves the aspect ratio that was set with setSizeRatio().
        /** If no aspect ratio is being enforced, this will return 0.
         */
        inline bool getSizeRatio() const noexcept
        {
            return attr_size->getSizeRatio();
        }
        
        //! Retrieves if the box should be hidden when the page is locked.
        /** The function retrieves if the box should be hidden when the page is locked.
         @return True if the box should be hidden when the page is locked, false otherwise.
         */
        inline bool isHiddenOnLock() const noexcept
        {
            return attr_hidden->get();
        }
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isIncludeInPresentation() const noexcept
        {
            return attr_presentation->get();
        }
        
        //! Retrieve if the box should ignore mouse click.
        /** The function retrieves if the box should ignore mouse click.
         @return True if the box should ignore mouse click, false otherwise.
         */
        inline bool getIgnoreClick() const noexcept
        {
            return attr_ignoreclick->get();
        }
        
        //! Retrieve if the font of the box.
        /** The function retrieves the font of the box.
         @return The font of the box.
         */
        inline Gui::Font getFont() const noexcept
        {
            return Gui::Font(toString(attr_font_name->get()), attr_font_size->get(), (Gui::Font::Style)attr_font_face->get());
        }
        
        //! Retrieve if the font justification of the box.
        /** The function retrieves the font justification of the box.
         @return The font justification of the box.
         */
        inline Gui::Font::Justification getFontJustification() const noexcept
        {
            return (Gui::Font::Justification)attr_font_justification->get();
        }
        
        //! Retrieve if the background color of the box.
        /** The function retrieves the background color of the box.
         @return The background color of the box.
         */
        inline Gui::Color getBackgroundColor() const noexcept
        {
            return attr_color_background->get();
        }
        
        //! Retrieve if the border color of the box.
        /** The function retrieves the   color of the box.
         @return The border color of the box.
         */
        inline Gui::Color getBorderColor() const noexcept
        {
            return attr_color_border->get();
        }
        
        //! Retrieve if the text color of the box.
        /** The function retrieves the text of the box.
         @return The text color of the box.
         */
        inline Gui::Color getTextColor() const noexcept
        {
            return attr_color_text->get();
        }
    };
}


#endif


