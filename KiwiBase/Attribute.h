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
		
		/** Flags describing the type of the attribute.
		 */
		enum Type
		{
			Long = 0,			///< Indicates that the attribute stores a value of type long.
			LongArray,			///< Indicates that the attribute stores an array of long values.
			Double,				///< Indicates that the attribute stores a value of type double.
			DoubleArray,		///< Indicates that the attribute stores an array of double values.
			Tag,				///< Indicates that the attribute stores a value of type Tag.
			TagArray,			///< Indicates that the attribute stores an array of tag values.
			BoxPointer,			///< Indicates that the attribute stores a Box pointer.
			BoxPointerArray,	///< Indicates that the attribute stores an array of Box pointer.
			ElemArray,			///< Indicates that the attribute stores an array of Element
		};
		
		/** Flags describing the display style of the attribute.
		 @see setStyle
		 */
		enum Style
		{
			Default = 0,	///< Indicates that the attribute should be displayed in a default style depending on its Type.
			Text,			///< Indicates that the attribute should be displayed in a text style
			List,			///< Indicates that the attribute should be displayed in a list style
			Enum,			///< Indicates that the attribute should be displayed in an enum style
			Number,			///< Indicates that the attribute should be displayed in a number style
			Toggle,			///< Indicates that the attribute should be displayed in a onoff toggle style
			Color,			///< Indicates that the attribute should be displayed in a color style
			Filepath		///< Indicates that the attribute should be displayed in a filepath style
		};
		
    private:
        sTag            m_name;				///< The name of the attribute.
		const Type		m_type;				///< The type of the attribute.
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
				  Type type,
				  ElemVector defaultValue = {},
                  string const& label = string(),
				  Style style = Style::Default,
                  string const& category = string(),
                  long behavior = 0);
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual ~Attribute();
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
		 @see label, style, category
         */
        inline sTag name() const noexcept							{return m_name;}
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
		 @see name, style, category
         */
        inline string label() const noexcept						{return m_label;}
		
		//! Sets the attribute label.
		/** The function sets the attribute label.
		 @param newLabel The new label of the attribute.
		 @see label
		 */
		inline void setLabel(const string& newLabel) noexcept		{m_label = newLabel;}
		
        //! Retrieve the attribute style.
        /** The function retrieves the attribute style.
         @return The attribute style.
		 @see name, label, category
         */
        inline Style style() const noexcept							{return m_style;}
		
		//! Sets the attribute style.
		/** The function sets the attribute style.
		 @param newStyle The new style of the attribute.
		 @see style
		 */
		inline void setStyle(Style newStyle) noexcept				{m_style = newStyle;}
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
		 @see name, label, style
         */
        inline string category() const noexcept						{return m_category;}
		
		//! Sets the attribute category.
		/** The function sets the attribute category.
		 @param Category The new category of the attribute.
		 @see style
		 */
		inline void setCategory(const string& newCategory) noexcept	{m_category = newCategory;}
		
		/** An easy way to set or remove the Invisible bit in the attribute behavior flags field.
		 @param isVisible If true, the attribute will be invisible, if false it will be visible.
		 @see setOpaque, setDisabled, setSaveable, setNotifyChanges, setBehavior
		 */
		void setInvisible(const bool isInvisible) noexcept;
		
		/** Is the attribute Invisible ?
		 @return True if the attribute is invisible, false if it is visible.
		 @see setInvisible, isOpaque, isDisabled, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isInvisible() const noexcept					{return (m_behavior & Invisible);}
		
		/** An easy way to set or remove the Opaque bit in the attribute behavior flags field.
		 @param isOpaque If true, the attribute will be opaque, if false it will not.
		 @see setInvisible, setDisabled, setSaveable, setNotifyChanges, setBehavior
		 */
		void setOpaque(const bool isOpaque) noexcept;
		
		/** Is the attribute opaque ?
		 @return True if the attribute is opaque, false if not.
		 @see setOpaque, isInvisible, isDisabled, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isOpaque() const noexcept						{return (m_behavior & Opaque);}
		
		/** An easy way to set or remove the Disabled bit in the attribute behavior flags field.
		 @param isDisabled If true, the attribute will be disabled, if false it will not.
		 @see setInvisible, setSaveable, setNotifyChanges, setBehavior
		 */
		void setDisabled(const bool isDisabled) noexcept;
		
		/** Is the attribute disabled ?
		 @return True if the attribute is disabled, false if it is not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isDisabled() const noexcept						{return (m_behavior & Disabled);}
		
		/** An easy way to set or remove the NotSaveable bit in the attribute behavior flags field.
		 @param saveable If true, the attribute will be saved, if false it will not.
		 @see setInvisible, setOpaque, setDisabled, setNotifyChanges, setBehavior
		 */
		void setSaveable(const bool saveable) noexcept;
		
		/** Is the attribute isSaveable ?
		 @return True if the attribute is saveable, false if it is not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool isSaveable() const noexcept						{return !(m_behavior & NotSaveable);}
		
		/** An easy way to set or remove the NotNotifyChanges bit in the attribute behavior flags field.
		 @param shouldNotify If true, the attribute will broadcast a notify message to its listeners when changed, if false it will not.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setBehavior
		 */
		void setNotifyChanges(const bool shouldNotify) noexcept;
		
		/** Should the attribute notify its changes ?
		 @return True if the attribute should notify its changes, false if it should not.
		 @see setDisabled, isInvisible, isOpaque, isSaveable, shouldNotifyChanges, getBehavior
		 */
		inline bool shouldNotifyChanges() const noexcept			{return !(m_behavior & NotNotifyChanges);}
		
		/** An easy way to set the whole behavior flags field of the attribute.
		 @param behavior	A combination of the flags specified in the Attribute::Behavior enum,
							which define the attribute's behaviors.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setNotifyChanges
		 */
		void setBehavior(Behavior behavior);
		
		/** Retrieves the whole behavior flags field of the attribute.
		 @param behavior	A combination of the flags specified in the Attribute::Behavior enum,
		 which define the attribute's behaviors.
		 @see setInvisible, setOpaque, setDisabled, setSaveable, setNotifyChanges
		 */
		inline Behavior getBehavior() const noexcept				{return (Behavior)m_behavior;}
		
        //! Retrieves the values.
        /** The Attribute subclasses must implement this function to retrieve the values.
         @param elements The vector of elements to fill.
		 @see set
         */
        virtual void get(ElemVector& elements) const noexcept = 0;
		
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
		
		//! Freezes or unfreezes the attribute.
		/** If you freeze an attribute, it will stores its current value as the saved value.
		 When an attribute is frozen it can still be changed,
		 but when the attribute will be saved it will take the frozen value rather than the current one.
		 @param frozen If true the attribute will be frozen, if false it will be unfrozen.
		 @see isFrozen, getFrozenValue
		 */
		void freeze(const bool frozen);
		
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
        
        //! Read the attribute in a dico.
        /** The function reads the attribute in a dico.
         @param dico The dico.
		 @see write
         */
        virtual void read(shared_ptr<const Dico> dico);
    };
    
    //! The shared pointer of an attribute.
    /**
     The sAttribute is shared pointer of an attribute.
     */
    typedef shared_ptr<Attribute>         sAttribute;
    
    // ================================================================================ //
    //                                  ATTRIBUTE FACTORY                               //
    // ================================================================================ //
    
    //! The attribute factory creates and manages attributes.
    /**
     The attribute factory creates and manages attributes.
     @see Tag
     */
    class AttributeFactory
    {
    private:
        map<sTag, sAttribute>       m_attributes;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        AttributeFactory() noexcept;
        
        //! Descrutor.
        /** Free the attributes.
         */
        virtual ~AttributeFactory();
        
        //! Write the object in a dico.
        /** The function writes the name of the object in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico) const noexcept;
        
        //! Retrieve an attribute.
        /** The function retrieves an attribute.
         @param name The name of the attribute.
         @return The attribute or nullptr if the key doesn't exist.
         */
        sAttribute getAttribute(sTag name) const noexcept;
        
        //! The receive method that set the values of the attributes.
        /** The function looks for the names of that match with the attributes and call the set methods if necessary.
         @param elements    A list of elements to pass.
         */
        void receive(ElemVector const& elements);
        
    protected:
        
        //! Read a dico.
        /** The function reads a dico but doesn't do anything.
         @param dico The dico.
         */
        void read(shared_ptr<const Dico> dico) noexcept;
        
        //! Receive a notification from an attribute.
        /** The function receives the notifications from the attribute.
         @param attr The attribute that notify.
         */
		virtual void notify(sAttribute attr) {};
			
        //! Attribute maker.
        /** This function creates and adds an attribute to the attribute factory.
         */
        template<class ObjectClass, class ...Args> void createAttribute(Args&& ...arguments)
        {
            sAttribute attr = make_shared<ObjectClass>(forward<Args>(arguments)...);
            m_attributes[attr->name()] = attr;
        }
        
        //! Set the label, the style and the category of an attribute.
        /** The function set the label, the style and the category of an attribute.
         @param name The name of the attribute.
         @param label The label of the attribute.
         @param style The style of the attribute.
         @param category The category of the attribute.
         */
        void setAttributeAppearance(sTag name, string const& label, Attribute::Style style, string const& category);
        
        //! Set the attribute opaque, visible and save states of an attribute.
        /** The function sets the attribute opaque, visible and save states of an attribute.
         @param name The name of the attribute.
         @param opaque Opaque or not.
         @param visible Visible or not.
         @param save Saved or not.
         */
        void setAttributeBehavior(sTag name, Attribute::Behavior behavior);
    };
    
    // ================================================================================ //
    //                                      ATTRIBUTE TYPED                             //
    // ================================================================================ //
    
    class AttributeLong : public Attribute
    {
    private:
        long m_value;
    public:
		AttributeLong(sTag name,
					  long defaultValue = 0,
					  string const& label = string(),
					  string const& category = string(),
					  Attribute::Style style = Attribute::Style::Number,
					  long behavior = 0);
		virtual ~AttributeLong() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
	
	class AttributeBool : public Attribute
	{
	private:
		bool m_value;
	public:
		AttributeBool(sTag name,
					  int defaultValue = 0,
					  string const& label = string(),
					  string const& category = string(),
					  Attribute::Style style = Attribute::Style::Toggle,
					  long behavior = 0);
		virtual ~AttributeBool() {};
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
	
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
    
    class AttributeTag : public Attribute
    {
    private:
        sTag m_value;
    public:
		AttributeTag(sTag name,
					 string defaultValue = string(),
					 string const& label = string(),
					 string const& category = string(),
					 Attribute::Style style = Attribute::Style::Text,
					 long behavior = 0);
		virtual ~AttributeTag() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
    
    class AttributeBox : public Attribute
    {
    private:
        shared_ptr<Box> m_value;
    public:
		AttributeBox(sTag name,
					 shared_ptr<Box> defaultValue = shared_ptr<Box>(),
					 string const& label = string(),
					 string const& category = string(),
					 Attribute::Style style = Attribute::Style::Default,
					 long behavior = 0);
		virtual ~AttributeBox() {};
        void get(ElemVector& elements) const noexcept;
        void set(ElemVector const& elements) override;
    };
	
	class AttributeColor : public Attribute
	{
	private:
		ElemVector m_value;
	public:
		AttributeColor(sTag name,
					   ElemVector defaultValue = {0.f, 0.f, 0.f, 1.f},
					   string const& label = string(),
					   string const& category = string(),
					   Attribute::Style style = Attribute::Style::Color,
					   long behavior = 0);
		virtual ~AttributeColor();
		void get(ElemVector& elements) const noexcept;
		void set(ElemVector const& elements) override;
	};
}


#endif


