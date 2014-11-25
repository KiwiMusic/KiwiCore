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
		/** Flags describing the behavior of the attribute.
		 @see setInvisible, setDisabled, setSaveable, setNotifyChanges
		 */
        enum Behavior
        {
            Invisible			= (1<<0),///< Indicates that the attribute is invisible.
			Disabled			= (1<<1),///< Indicates that the attribute can't be changed.
            Unsaved             = (1<<2),///< Indicates that the attribute is not saved.
            Notifier            = (1<<3) ///< Indicates that the attribute should not notify its changes.
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
        static const sTag frozen_attributes;
        
        const sTag          m_name;				///< The name of the attribute.
        const sTag          m_label;			///< The label of the attribute.
        const sTag          m_category;			///< The category of the attribute.
        const Style         m_style;			///< The style of the attribute.
		const ElemVector    m_default_values;	///< The default value of the attribute.
        atomic_long         m_behavior;			///< The behavior of the attribute.
		ElemVector          m_frozen_values;    ///< The frozen value of the attribute.
        
    public:
		
        //! Constructor.
        /** Allocate and initialize the member values.
		 @param name		The name of the attribute.
		 @param label		A short description of the attribute in a human readable style.
         @param category	A named category that the attribute fits into.
		 @param style		The style of the attribute specified in the Attr::Style enum.
         @param defaultValues The default values.
		 @param behavior	A combination of the flags which define the attribute's behavior.
         */
        Attr(sTag name,  sTag label, sTag category, Style style = Style::Default, ElemVector defaultValues = {}, long behavior = 0);
        
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
        // ================================================================================ //
        //                                  ATTRIBUTE MANAGER                               //
        // ================================================================================ //
        
        //! The attribute manager manages a set of attributes.
        /** The attribute manager manages a set of attributes, it allows the setting and the getting of their values and to retrieve them by name or by category.
         @see Attr
         */
		class Manager : public enable_shared_from_this<Manager>
		{
		public:
			class Listener;
		private:
			unordered_map<sTag, sAttr>          m_attrs;
			mutable mutex                       m_attrs_mutex;
			
			unordered_set<weak_ptr<Listener>,
			weak_ptr_hash<Listener>,
			weak_ptr_equal<Listener>>           m_listeners;
			mutex                               m_listeners_mutex;
			
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
            unsigned long getNumberOfAttributes() const noexcept;
            
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
            unsigned long getNumberOfCategories() const noexcept;
            
            //! Retrieve the names of the categories.
            /** The function retrieves the names of the categories. If a category have only invisibles attributes, the name of the category won't be retrieved.
             @param A vector of tag containing the names of the categories.
             */
            void getCategoriesNames(vector<sTag>& names) const noexcept;
            
            //! Check if a category exist.
            /** The function checks if a category exist.  If a category have only invisibles attributes, the function returns false.
             @param name The name of the category.
             @return true if the category exist, otherwise false.
             */
            bool hasCategory(sTag name) const noexcept;
            
            //! Retrieve the attributes of a category.
            /** The function retrieves the attributes of a category. If an attribute is invisible, the attribute won't be retrieved.
             @param name The name of the category.
             @param attrs A vector of attributes to pass.
             */
            void getCategory(sTag name, vector<sAttr>& attrs) const;
            
            //! Write the attributes in a dico.
            /** The function writes the attributes in a dico.
             @param dico A dico.
             */
            void write(sDico dico) const noexcept;
			
            //! Notify the manager that the values of an attribute has changed.
            /** The function notifies the manager that the values of an attribute has changed.
             @param dico A dico.
			 @return pass true to notify changes to listeners, false if you don't want them to be notified
             */
			virtual bool attributeValueChanged(sAttr attr)
			{
				return false;
			}
			
            // ================================================================================ //
            //								ATTRIBUTE MANAGER LISTENER                          //
            // ================================================================================ //
			
			/** Flags describing the type of the notification
			 @see Manager::Listener
			 */
			enum Notification
			{
				AttrAdded		= 0,	///< Indicates that an attribute has been added.
				AttrRemoved		= 1,	///< Indicates that an attribute has been removed.
				ValueChanged	= 2,	///< Indicates that an attribute value has changed.
				BehaviorChanged	= 3		///< Indicates that the behavior of an attribute has changed.
			};
			
            //! The attribute manager listener is a virtual class that can be binded to an attribute manager to be notified of various changes.
            /** The attribute manager listener is a very light class that allows to be notified of the attributes modification.
             */
            class Listener
            {
            public:
                virtual ~Listener() {}
                
                //! Receive the notification that an attribute has changed.
                /** Sublass of Attr::Manager::Listener must implement this virtual function to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
                 @param manager		The Attr::Manager that manages the attribute.
                 @param attr		The attribute that has been modified.
                 @param type		The type of notification as specified in the Attr::Manager::NotificationType enum,
                 */
                virtual void attributeNotify(shared_ptr<Manager> manager, sAttr attr, Notification type) = 0;
            };
			
			//! Adds an attribute manager listener in the binding list of the attribute manager.
			/** The function adds an attribute manager listener in the binding list of the attribute manager. If the attribute manager listener is already in the binding list, the function has no effect.
			 @param listener  The pointer of the attribute manager listener.
			 @see              unbind()
			 */
			void bind(shared_ptr<Listener> listener);
			
			//! Removes an attribute manager listener from the binding list of the attribute manager.
			/** The function removes an attribute manager listener from the binding list of the attribute manager. If the attribute manager listener was not in the binding list, the function has no effect.
			 @param listener  The pointer of the attribute manager listener.
			 @see              unbind()
			 */
			void unbind(shared_ptr<Listener> listener);
			
			typedef shared_ptr<Listener>    sListener;
			
		private:
			//! @internal Trigger notification to subclasses and listeners.
			void sendNotification(sAttr attr, Notification type);
        };
		
		typedef shared_ptr<Attr::Manager> sManager;
		typedef shared_ptr<const Attr::Manager> scManager;
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
        Attr(name, label, category, Attr::Style::Number, {default_value}, behavior) {;}
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
        Attr(name, label, category, Attr::Style::Number, {default_value}, behavior) {;}
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
		AttrTag(sTag name, sTag label, sTag category, sTag default_value = 0, long behavior = 0) :
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
        inline unsigned long get() const noexcept
        {
            return (unsigned long)m_value;
        }
	};
	
	//! The color attribute is an attribute that holds a color value.
	/** The color attribute holds a four double values suitable to represent a RGBA type color. Its default display style will obviously be a Attr::Style::Color. Each value of the vector will be clipped between 0 and 1.
	 @see Attr.
	 */
	class AttrColor : public Attr
	{
	private:
        Kiwi::Color m_value;
	public:
		AttrColor(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0., 0., 1.}, long behavior = 0) :
        Attr(name, label, category, Attr::Style::Color, {default_value}, behavior) {;}
		virtual ~AttrColor(){};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
        
        inline Kiwi::Color get() const noexcept
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
	private:
        Point m_value = {0, 0};
	public:
		AttrPoint(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0.}, long behavior = 0) :
		Attr(name, label, category, Attr::Style::List, {default_value}, behavior) {;}
		virtual ~AttrPoint() {};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
        
        inline Point get() const noexcept
        {
            return m_value;
        }
	};
    
    typedef shared_ptr<AttrPoint>       sAttrPoint;
    typedef weak_ptr<AttrPoint>         wAttrPoint;
    typedef shared_ptr<const AttrPoint> scAttrPoint;
    typedef weak_ptr<const AttrPoint>   wcAttrPoint;
    
	//! The rectangle attribute is an attribute that is particulary suitable to represent a position and a size.
	/** The rectangle attribute holds four double values suitable to represent a rectangle, its default display style will obviously be a Attr::Style::List.
	 @see Attr
	 */
	class AttrRect : public Attr
	{
	private:
		Rectangle m_value;
	public:
		AttrRect(sTag name, sTag label, sTag category, ElemVector const& default_value = {0., 0., 0., 0.}, long behavior = 0) :
		Attr(name, label, category, Attr::Style::List, {default_value}, behavior) {;}
		virtual ~AttrRect() {};
		virtual void get(ElemVector& elements) const noexcept;
		virtual void set(ElemVector const& elements) override;
	};
    
    //! The font attribute is the default attributes for the font.
	/** Holds a vector of two double values suitable to represent a position or a size,
	 */
    class AttrFont
    {
    private:
        static const sTag Tag_Font;
        static const sTag Tag_fontname;
        static const sTag Tag_Font_Name;
        static const sTag Tag_Arial;
        static const sTag Tag_fontsize;
        static const sTag Tag_Font_Size;
        static const sTag Tag_fontface;
        static const sTag Tag_Font_Face;
        static const sTag Tag_normal;
        static const sTag Tag_bold;
        static const sTag Tag_italic;
        static const sTag Tag_bold_italic;
        static const sTag Tag_fontjustification;
        static const sTag Tag_Font_Justification;
        static const sTag Tag_left;
        static const sTag Tag_center;
        static const sTag Tag_right;
    public:
        class Name : public AttrTag
        {
        public:
            Name() : AttrTag(Tag_fontname, Tag_Font_Name, Tag_Font, Tag_Arial){};
            ~Name(){};
        };
        
        class Size : public AttrDouble
        {
        public:
            Size() : AttrDouble(Tag_fontsize, Tag_Font_Size, Tag_Font, 12){};
            ~Size(){};
        };
        
        class Face : public AttrEnum
        {
        public:
            Face() : AttrEnum(Tag_fontface, Tag_Font_Face, Tag_Font, {Tag_normal, Tag_bold, Tag_italic, Tag_bold_italic}, 0){};
            ~Face(){};
        };
        
        class Justification : public AttrEnum
        {
        public:
            Justification() : AttrEnum(Tag_fontjustification, Tag_Font_Justification, Tag_Font, {Tag_left, Tag_center, Tag_right}, 0){};
            ~Justification(){};
        };
    private:
        Kiwi::Font                  m_font;
        shared_ptr<Name>            m_name;
        shared_ptr<Face>            m_face;
        shared_ptr<Size>            m_size;
        shared_ptr<Justification>   m_just;
    public:
        
        AttrFont();
        ~AttrFont();
        
        Kiwi::Font getFont();
        Justification getJustification();
    };
    
    class AttrAppearance
    {
    private:
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
        
    public:
        
        class Position : public AttrPoint
        {
        public:
            Position() : AttrPoint(Tag_position, Tag_Position, Tag_Appearance, {0., 0.}){};
            ~Position(){};
        };
        
        class Size : public AttrPoint
        {
        public:
            Size() : AttrPoint(Tag_size, Tag_Size, Tag_Appearance, {100., 20.}){};
            ~Size(){};
        };
        
        class PresentationPosition : public AttrPoint
        {
        public:
            PresentationPosition() : AttrPoint(Tag_presentation_position, Tag_Presentation_Position, Tag_Appearance, {0., 0.}){};
            ~PresentationPosition(){};
        };
        
        class PresentationSize : public AttrPoint
        {
        public:
            PresentationSize() : AttrPoint(Tag_presentation_size, Tag_Presentation_Size, Tag_Appearance, {0., 0.}){};
            ~PresentationSize(){};
        };
        
        class Hidden : public AttrBool
        {
        public:
            Hidden() : AttrBool(Tag_hidden, Tag_Hide_on_Lock, Tag_Appearance, false){};
            ~Hidden(){};
        };
        
        class Presentation : public AttrBool
        {
        public:
            Presentation() : AttrBool(Tag_presentation,  Tag_Include_in_Presentation, Tag_Appearance, false){};
            ~Presentation(){};
        };
        
    private:
        shared_ptr<Position>                m_position;
        shared_ptr<Size>                    m_size;
        shared_ptr<PresentationPosition>    m_presentation_position;
        shared_ptr<PresentationSize>        m_presentation_size;
        shared_ptr<Hidden>                  m_hidden;
        shared_ptr<Presentation>            m_presentation;
    public:
        
        AttrAppearance() :
        m_position(Attr::create<Position>()),
        m_size(Attr::create<Size>()),
        m_presentation_position(Attr::create<PresentationPosition>()),
        m_presentation_size(Attr::create<PresentationSize>()),
        m_hidden(Attr::create<Hidden>()),
        m_presentation(Attr::create<Presentation>())
        {
            ;
        }
        
        ~AttrAppearance()
        {
            ;
        }
        
        void addAttributes(Attr::Manager* manager)
        {
            manager->addAttribute(m_position);
            manager->addAttribute(m_size);
            manager->addAttribute(m_presentation_position);
            manager->addAttribute(m_presentation_size);
            manager->addAttribute(m_hidden);
            manager->addAttribute(m_presentation);
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPosition() const noexcept
        {
            return m_position->get();
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getSize() const noexcept
        {
            return m_size->get();
        }
        
        
        //! Retrieve the bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Rectangle getBounds() const noexcept
        {
            return Rectangle(m_position->get(), m_size->get());
        }
    };
}


#endif


