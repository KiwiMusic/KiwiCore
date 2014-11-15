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

#ifndef __DEF_KIWI_OBJECT_ATTRIBUTE__
#define __DEF_KIWI_OBJECT_ATTRIBUTE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Dico.h"

namespace Kiwi
{
    class Instance;
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! Attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
	/** Holds a set of values of differents kinds and differents sizes.
     The attribute manages a set of values.
     */
    class Attribute
    {
    public:
		
		/** Flags describing the behavior of the attribute.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setNotifyChanges
		 */
        enum Behavior
        {
			/** Indicates that the attribute is visible
			 @see Attribute::setInvisible
			 */
            Invisible			= (1<<0),
			
			/** Indicates that the attribute is opaque
			 An opaque attribute will not be displayed in the Kiwi properties inspector
			 @see Attribute::setOpaque
			 */
            Opaque				= (1<<1),
			
			/** Indicates that the attribute is disabled
			 When the Disabled flag of an attribute is set it should be displayed in the Kiwi properties inspector,
			 but any changes are not allowed
			 @see Attribute::setOpaque
			 */
			Disabled			= (1<<2),
			
			/** Indicates that the attribute can not be frozen
			 @see Attribute::freeze
			 */
			NotFreezable		= (1<<3),
			
			/** Indicates that the attribute is not saveable
			 @see Attribute::setSaveable
			 */
            NotSaveable			= (1<<4),
			
			/** Indicates that the attribute should not broadcast a notify message when it is modified.
			 @see Attribute::setNotifyChanges
			 */
            NotNotifyChanges	= (1<<5)
        };
        
		/** Flags describing the display style of the attribute.
		 @see setStyle
		 */
		enum Style
		{
			Default = 0,	///< Indicates that the attribute should be displayed in a default style depending on its Type.
			Text,			///< Indicates that the attribute should be displayed in a text style
			List,			///< Indicates that the attribute should be displayed in a list of same or mixed elements style
			Enum,			///< Indicates that the attribute should be displayed in an enum style
			Number,			///< Indicates that the attribute should be displayed in a number style
			Toggle,			///< Indicates that the attribute should be displayed in a onoff toggle style
			Color,			///< Indicates that the attribute should be displayed in a color style
			Filepath		///< Indicates that the attribute should be displayed in a filepath style
		};
		
    private:
        sTag            m_name;				///< The name of the attribute.
		ElemVector		m_default_value;	///< The default value of the attribute.
        string          m_label;			///< The label of the attribute.
		Style			m_style;			///< The style of the attribute.
		string          m_category;			///< A named category that the attribute fits into.
        long            m_behavior;			///< The behavior of the attribute.
		ElemVector		m_frozen_value;		///< The frozen value of the attribute.
		bool			m_frozen;			///< Is the attribute currently frozen.

    public:
		
        //! Constructor.
        /** Allocate and initialize the member values.
		 @param name		The name of the attribute.
		 @param label		A short description of the attribute in a human readable style.
		 @param style		The style of the attribute specified in the Attribute::Style enum.
		 @param category	A named category that the attribute fits into.
		 @param behavior	A combination of the flags specified in the Attribute::Behavior enum,
							which define the attribute's behavior.
         */
        Attribute(sTag name,
				  ElemVector defaultValue = {},
                  string const& label = string(),
				  Style style = Style::Default,
                  string const& category = string(),
                  long behavior = 0);
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual ~Attribute();
		
		class Manager;
		
	public:
		
		//! Retrieves the values.
		/** The Attribute subclasses must implement this function to retrieve the values.
		 @param elements A vector of elements to fill.
		 @see set
		 */
		virtual void get(ElemVector& elements) const noexcept = 0;
		
		//! Retrieves the default values.
		/** Retrieve the default values.
		 @param elements A vector of elements to fill.
		 @see set
		 */
		void getDefault(ElemVector& elements) const noexcept		{elements = m_default_value;}
		
		//! Retrieves the enum values.
		/** The Attribute subclasses than want to be displayed in an enum style must implement this function to retrieve enum values.
		 @param elements A vector of elements to fill.
		 @see AttributeEnum
		 */
		virtual void getEnumValues(ElemVector& elements) const		{elements.clear();}
		
		//! Retrieve the name of the attribute.
		/** The function retrieves the name of the attribute.
		 @return The name of the attribute.
		 @see getLabel, getStyle, getCategory
		 */
		inline sTag getName() const noexcept						{return m_name;}
		
		//! Retrieve the attribute label.
		/** The function retrieves the attribute label.
		 @return The attribute label.
		 @see getName, getStyle, getCategory
		 */
		inline string getLabel() const noexcept						{return m_label;}
		
		//! Retrieve the attribute style.
		/** The function retrieves the attribute style.
		 @return The attribute style.
		 @see getName, getLabel, getCategory
		 */
		inline Style getStyle() const noexcept						{return m_style;}
		
		//! Retrieve the attribute category.
		/** The function retrieves the attribute category.
		 @return The attribute category.
		 @see setCategory, getName, getLabel, getStyle
		 */
		inline string getCategory() const noexcept					{return m_category;}
		
		/** Is the attribute Invisible ?
		 @return True if the attribute is invisible, false if it is visible.
		 @see setInvisible, isOpaque, isDisabled, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isInvisible() const noexcept					{return (m_behavior & Invisible);}
		
		/** Is the attribute opaque ?
		 @return True if the attribute is opaque, false if not.
		 @see setOpaque, isInvisible, isDisabled, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isOpaque() const noexcept						{return (m_behavior & Opaque);}
		
		/** Is the attribute disabled ?
		 @return True if the attribute is disabled, false if it is not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isDisabled() const noexcept						{return (m_behavior & Disabled);}
		
		/** Is the attribute isSaveable ?
		 @return True if the attribute is saveable, false if it is not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isSaveable() const noexcept						{return !(m_behavior & NotSaveable);}
		
		/** Should the attribute notify its changes ?
		 @return True if the attribute should notify its changes, false if it should not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool shouldNotifyChanges() const noexcept			{return !(m_behavior & NotNotifyChanges);}
		
		/** Retrieves the whole behavior flags field of the attribute.
		 @param behavior	A combination of the flags specified in the Attribute::Behavior enum,
		 which define the attribute's behaviors.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setNotifyChanges
		 */
		inline long getBehavior() const noexcept				{return m_behavior;}
		
		//! Is the attribute currently frozen ?
		/** Is the attribute currently frozen ?
		 @return True if the attribute is frozen, false otherwise.
		 @see freeze, getFrozenValue
		 */
		bool isFrozen() const noexcept {return m_frozen;}
		
		//! Retrieve the frozen value.
		/** Retrieve the frozen value, if the attribute is not frozen the vector will be empty.
		 @param elements A vector of Element to be replaced by the frozen value.
		 @see freeze, isFrozen
		 */
		void getFrozenValue(ElemVector& elements) const noexcept	{elements = m_frozen_value;}
		
		//! Attempts to write the attribute in a dico.
		/** The function attempts to write the attribute in a dico.
		 If the behavior of the attribute is to not save the value and the attribute is not frozen, nothing will happen
		 @param dico The dico to write into.
		 @see read
		 */
		virtual void write(sDico dico) const noexcept;
	
	protected:
		
		//! Sets the values with a vector of elements.
		/** The function sets the values with a vector of elements and resize the values if necessary.
		 Attribute subclasses must implement this function
		 @param elements The vector of elements.
		 @see get
		 */
		virtual void set(ElemVector const& elements) = 0;
		
		//! Resets the attribute value to default.
		/** Resets the attribute value to its default value.
		 @see set
		 */
		void reset();
		
		//! Sets the attribute label.
		/** The function sets the attribute label.
		 @param newLabel The new label of the attribute.
		 @see getLabel
		 */
		inline void setLabel(const string& newLabel) noexcept		{m_label = newLabel;}
		
		//! Sets the attribute style.
		/** The function sets the attribute style.
		 @param newStyle The new style of the attribute.
		 @see getStyle
		 */
		inline void setStyle(Style newStyle) noexcept				{m_style = newStyle;}
		
		//! Sets the attribute category.
		/** The function sets the attribute category.
		 @param Category The new category of the attribute.
		 @see tyle
		 */
		inline void setCategory(const string& newCategory) noexcept	{m_category = newCategory;}
		
		/** An easy way to set or remove the Invisible bit in the attribute behavior flags field.
		 @param isVisible If true, the attribute will be invisible, if false it will be visible.
		 @see setOpaque, setDisabled, setSaveable, setNotifyChanges, setBehavior
		 */
		void setInvisible(const bool isInvisible) noexcept;
		
		/** An easy way to set or remove the Opaque bit in the attribute behavior flags field.
		 @param isOpaque If true, the attribute will be opaque, if false it will not.
		 @see setInvisible, setDisabled, setSaveable, setNotifyChanges, setBehavior
		 */
		void setOpaque(const bool isOpaque) noexcept;
		
		/** An easy way to set or remove the Disabled bit in the attribute behavior flags field.
		 @param isDisabled If true, the attribute will be disabled, if false it will not.
		 @see setInvisible, setSaveable, setNotifyChanges, setBehavior
		 */
		void setDisabled(const bool isDisabled) noexcept;
		
		/** An easy way to set or remove the NotSaveable bit in the attribute behavior flags field.
		 @param saveable If true, the attribute will be saved, if false it will not.
		 @see setInvisible, setOpaque, setDisabled, setNotifyChanges, setBehavior
		 */
		void setSaveable(const bool saveable) noexcept;
		
		/** An easy way to set or remove the NotNotifyChanges bit in the attribute behavior flags field.
		 @param shouldNotify If true, the attribute will broadcast a notify message to its listeners when changed, if false it will not.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setBehavior
		 */
		void setNotifyChanges(const bool shouldNotify) noexcept;
		
		/** An easy way to set the whole behavior flags field of the attribute.
		 @param behavior	A combination of the flags specified in the Attribute::Behavior enum,
							which define the attribute's behaviors.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setNotifyChanges
		 */
		void setBehavior(long behavior);
        
        //! Read the attribute in a dico.
        /** The function reads the attribute in a dico.
         @param dico The dico.
		 @see write
         */
        virtual void read(shared_ptr<const Dico> dico);
		
		//! Freezes or unfreezes the attribute.
		/** If you freeze an attribute, it will stores its current value as the saved value.
		 When an attribute is frozen it can still be changed,
		 but when the attribute will be saved it will take the frozen value rather than the current one.
		 @param frozen If true the attribute will be frozen, if false it will be unfrozen.
		 @see isFrozen, getFrozenValue
		 */
		void freeze(const bool frozen);
    };
    
    //! The shared pointer of an attribute.
    /**
     The sAttribute is shared pointer of an attribute.
     */
    typedef shared_ptr<Attribute>         sAttribute;
    
    // ================================================================================ //
    //                                  ATTRIBUTE::MANAGER                              //
    // ================================================================================ //
	
	//! The attribute set manages a set of attributes.
	/** The attribute set manages a set of attributes.
	 @see Kiwi::Attribute, Kiwi::Tag, Kiwi::Element
	 
	 @todo	- trigger notifications
			- function to determine if it is different that the default value
			- only save if attr != default
	 */
	class Attribute::Manager
	{
	public:
		class Listener;
		
	private:
		map<sTag, sAttribute>       m_attributes;
		unordered_set<weak_ptr<Listener>,
		weak_ptr_hash<Listener>,
		weak_ptr_equal<Listener>>   m_listeners;
		
		typedef shared_ptr<Attribute::Manager> sAttributeManager;
		
	public:
		
		//! Constructor.
		/** Creates a new attribute set.
		 */
		Manager() noexcept;
		
		//! Descrutor.
		/** Free the attributes.
		 */
		virtual ~Manager();
		
		//! Try to set an attribute value.
		/** This method try to set the value of a named attribute.
		 @param name		The name to find in the attribute set.
		 @param elements    A list of elements to pass.
		 @return			true if the attribute value has been set, false otherwise.
		 @see getValue
		 */
		bool setValue(sTag name, ElemVector const& elements);
		
		//! Retrieves an attribute value.
		/** This method try to retrieve the value of a named attribute.
		 @param elements		A list of elements in which the attribute value will be filled.
		 @param defaultValue	An optional default value to fill the \p elements
		 if the getter method fail or is not accessible.
		 @return true if the attribute value exists and is gettable, false otherwise.
		 @see setValue
		 */
		bool getValue(sTag name, ElemVector& elements, ElemVector const& defaultValue = {});
		
		//! The receive method that set the values of the attributes.
		/** The function looks for the names of that match with the attributes and call the set methods if necessary.
		 @param elements A list of elements to pass, first element must be the name of the attribute.
		 */
		void receive(ElemVector const& elements);
		
		//! Resets all attributes value to default.
		/** This method resets all attributes value to default.
		 @see setValue
		 */
		void resetAllToDefault();
		
		//! Retrieves the number of attribute in the current set.
		/** This method retrieves the number of attribute in the current set.
		 @return The number of attribute in the current set.
		 */
		inline long getNumberOfAttributes() const noexcept			{return m_attributes.size();}
		
		//! Retrieves all the names of the current attribute set.
		/** This method retrieves all the names of the current attribute set.
		 @return A vector of Tag containing the attribute names;
		 */
		vector<sTag> getAttributeNames() const noexcept;
		
		//! Checks if a given key exists in the attribute set.
		/** The method checks if a given key exists in the attribute set.
		 @param name The name to find in the attribute set.
		 @return true if an attribute is set with the passed in key, false otherwise.
		 */
		bool hasKey(sTag name) const noexcept;
		
		//! Retrieves the number of attribute categories in the current set.
		/** This method retrieves the number of attribute categories in the current set.
		 @return The number of attribute categories in the current set.
		 */
		long getNumberOfCategories() const;
		
		//! Retrieves all the categories of the current attribute set.
		/** This method retrieves all the categories of the current attribute set.
		 @return A vector of category name
		 */
		vector<string> getCategories() const;
		
		//! Retrieve an attribute.
		/** The function retrieves an attribute.
		 @param name The name of the attribute.
		 @return The attribute or nullptr if the key doesn't exist.
		 */
		sAttribute getAttribute(sTag name) const noexcept;
		
		//! Writes the whole attributes in a dico.
		/** This method writes the whole attributes in a Dico.
		 @param dico A dico std::shared_ptr.
		 @see Kiwi::Dico, read
		 */
		void write(sDico dico) const noexcept;
		
		//! Removes attributes on this current set if they are not present in an other AttributeSet.
		/** This method removes attributes on this current set if they are not present in an other AttributeSet.
		 This method is particulary suitable to get only the common properties of multiples AttributeSet.
		 @param attrset An attribute set to compare with.
		 */
		void removeAttributesWithNonCommonKey(shared_ptr<Manager> attrset);
		
		// ================================================================================ //
		//								ATRIBUTESET::LISTENER                               //
		// ================================================================================ //
		
		/** Flags describing the type of the notification
		 @see Attribute::Manager::Listener
		 */
		enum NotificationType
		{
			AttrAdded = 0,		///< Indicates that an attribute has been added.
			AttrRemoved,		///< Indicates that an attribute has been removed.
			ValueChanged,		///< Indicates that an attribute value has changed.
			AppearanceChanged,	///< Indicates that the appearance of the attribute has changed (style, category, label..).
			BehaviorChanged		///< Indicates that the behavior of an attribute has changed.
		};
		
		//! The attribute listener is a virtual class that can be binded to an AttributeSet to be notified of the attributes changes.
		/** The attribute listener is a very light class that allows to be notified of the attributes modification.
			To
		 */
		class Listener
		{
		public:
			virtual ~Listener() {}
			
			//! Receive the notification that an attribute has changed.
			/** Sublass of Attribute::Manager::Listener must implement this virtual method to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
			 @param manager		The Attribute::Manager that manages the attribute.
			 @param attr		The attribute that has been modified.
			 @param type		The type of notification as specified in the Attribute::Manager::NotificationType enum,
			 */
			virtual void attributeChanged(Manager* manager, sAttribute attr, NotificationType type) = 0;
		};
		
		//! Adds a listener to be called when this AttributeSet changes.
		/** This method adds a listener to be called when this AttributeSet changes. \n
		 If the listener is already there, the function doesn't do anything.
		 @param listener  The pointer of the attribute set listener to bind.
		 @see unbind()
		 */
		void bind(weak_ptr<Listener> listener);
		
		//! Removes a previously-registered listener.
		/** This method removes a previously-registered listener. \n
		 If the listener is not binded, the function doesn't do anything.
		 @param listener  The pointer of the attribute set listener to unbind.
		 @see bind()
		 */
		void unbind(weak_ptr<Listener> listener);
		
	protected:
		
		//! Receive the notification that an attribute has changed.
		/** Sublass of Attribute::Manager can implement this virtual method to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
		 @param attr		The attribute that has been modified.
		 @param type		The type of notification as specified in the Attribute::Manager::NotificationType enum,
		 */
		virtual void attributeChanged(sAttribute attr, NotificationType type) {};
		
		//! Sets the attributes value with a dico.
		/** This method sets the attributes value with a dico.
		 @param dico The dico.
		 */
		void read(scDico dico) noexcept;
		
		//! Attribute maker.
		/** This function creates and adds an attribute to the attribute factory.
		 */
		template<class ObjectClass, class ...Args> void addAttribute(Args&& ...arguments)
		{
			sAttribute attr = make_shared<ObjectClass>(forward<Args>(arguments)...);
			m_attributes[attr->getName()] = attr;
		}
		
		//! Adds an attribute to the current set.
		/** This function adds an attribute to the attribute set.
		 @param attr the attribute to add.
		 */
		void addAttribute(sAttribute attr);
		
		//! Adds a set of attributes to the current set.
		/** This function adds a set of attributes to the current set.
		 @param attr the attribute set to add.
		 */
		//void addAttributeSet(shared_ptr<AttributeSet> attrset);
		
		//! Removes an attribute from the current set.
		/** This function removes an attribute from the current attribute set.
		 @param attr The attribute to remove.
		 */
		void removeAttribute(sAttribute attr);
		
		//! Removes an attribute from the current set.
		/** This function removes an attribute from the current attribute set.
		 @param name The name of the attribute to remove.
		 */
		void removeAttribute(sTag name);
		
		//! Sets the label, the style and the category of an attribute.
		/** The function set the label, the style and the category of an attribute.
		 @param name The name of the attribute.
		 @param label The label of the attribute.
		 @param style The style of the attribute.
		 @param category The category of the attribute.
		 */
		void setAttributeAppearance(sTag name, string const& label, Attribute::Style style, string const& category);
		
		//! Sets the attribute behavior.
		/** The function sets the attribute behaviors.
		 @param behavior The behavior of the attribute.
		 */
		void setAttributeBehavior(sTag name, Attribute::Behavior behavior);
		
	private:
		
		//! @internal Trigger notification to subclasses and listeners.
		void triggerNotification(sAttribute attr, NotificationType type);
	};
	
	//! The shared pointer of an attribute set.
	/**
	 The sAttributeSet is shared pointer of an attribute set.
	 */
	typedef shared_ptr<Attribute::Manager>         sAttributeManager;
    
    // ================================================================================ //
    //                                      ATTRIBUTE TYPED                             //
    // ================================================================================ //
	
    //! The AttributeBool is an Attribute that holds a boolean value.
	/** Holds a boolean value
	 Its default display style is obviously an Attribute::Style::Toggle.
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
	class AttributeBool : public Attribute
	{
	private:
		bool m_value;
	public:
		AttributeBool(sTag name,
					  int defaultValue = 0,
					  string const& label = string(),
					  string const& category = string(),
					  long behavior = 0);
		virtual ~AttributeBool() {};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
    
	//! The AttributeLong is an Attribute that holds a long value.
	/** Holds a long value.
	 Its default display style is obviously an Attribute::Style::Number
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
    class AttributeLong : public Attribute
    {
    private:
        long m_value;
    public:
		AttributeLong(sTag name,
					  long defaultValue = 0,
					  string const& label = string(),
					  string const& category = string(),
					  long behavior = 0);
		virtual ~AttributeLong() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
	
	//! The AttributeDouble is an Attribute that holds a double value.
	/** Holds a double value, its default display style will be a Attribute::Style::Number
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
    class AttributeDouble : public Attribute
    {
    private:
        double m_value;
    public:
		AttributeDouble(sTag name,
						double defaultValue = 0.f,
						string const& label = string(),
						string const& category = string(),
						Attribute::Style style = Attribute::Style::Number,
						long behavior = 0);
		virtual ~AttributeDouble() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
	
	//! The AttributeTag is an Attribute that holds a Tag value.
	/** Holds a Tag value, its default display style will be a Attribute::Style::Text
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
    class AttributeTag : public Attribute
    {
    private:
        sTag m_value;
    public:
		AttributeTag(sTag name,
					 string defaultValue = string(),
					 string const& label = string(),
					 string const& category = string(),
					 long behavior = 0);
		virtual ~AttributeTag() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
	
	//! The AttributeEnum is an Attribute that represent an enumeration.
	/** It holds a long value that represent a position in the enumeration.
	 The value can be set either by a number or a Tag, and will be clipped between 0 and enum size.
	 Its default display style is obviously an Attribute::Style::Enum.
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param enumValues		The enum values.
	 @param defaultValue	The default enum value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum, 
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
	class AttributeEnum : public Attribute
	{
	private:
		long		m_value;
		ElemVector	m_enum_values;
	public:
		AttributeEnum(sTag name,
					  ElemVector const& enumValues = {},
					  long defaultValue = 0,
					  string const& label = string(),
					  string const& category = string(),
					  long behavior = 0);
		virtual ~AttributeEnum() {};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
		void getEnumValues(ElemVector& elements) const		{elements = m_enum_values;}
	};
	
	//! The AttributeColor is an Attribute that holds a color value.
	/** Holds a vector of double values suitable to represent a RGBA type color, \n
	 its default display style will obviously be a Attribute::Style::Color.
	 Each value of the vector will be clipped between 0. and 1.
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default color value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
	class AttributeColor : public Attribute
	{
	private:
		double m_value[4];
	public:
		AttributeColor(sTag name,
					   ElemVector const& defaultValue = {0., 0., 0., 1.},
					   string const& label = string(),
					   string const& category = string(),
					   long behavior = 0);
		virtual ~AttributeColor(){};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
	
	//! The AttributeRect is an Attribute that is particulary suitable to represent a position and a size.
	/** Holds a vector of double values suitable to represent a rectangle, \n
	 its default display style will obviously be a Attribute::Style::List.
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
	class AttributeRect : public Attribute
	{
	private:
		double m_value[4];
	public:
		AttributeRect(sTag name,
					  ElemVector const& defaultValue = {0., 0., 0., 0.},
					  string const& label = string(),
					  string const& category = string(),
					  long behavior = 0);
		virtual ~AttributeRect() {};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
	
	//! The AttributePoint is an Attribute that is particulary suitable to represent a position or a size.
	/** Holds a vector of two double values suitable to represent a position or a size, \n
	 its default display style will be an Attribute::Style::List.
	 
	 Creation parameters are as follows :
	 @param name			The name of the attribute.
	 @param defaultValue	The default value (optional).
	 @param label			The label of the attribute (optional).
	 @param category		The category of the attribute (optional).
	 @param behavior		An optional combination of flags specified in the Attribute::Behavior enum,
							defining the attribute's behaviors.
	 
	 @see Attribute::Manager::addAttribute
	 */
	class AttributePoint : public Attribute
	{
	private:
		double m_value[2];
	public:
		AttributePoint(sTag name,
					   ElemVector const& defaultValue = {0., 0.},
					   string const& label = string(),
					   string const& category = string(),
					   long behavior = 0);
		virtual ~AttributePoint() {};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
}


#endif


