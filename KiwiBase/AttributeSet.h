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

#ifndef __DEF_KIWI_ATTRIBUTESET__
#define __DEF_KIWI_ATTRIBUTESET__

#include "Kiwi.h"

namespace Kiwi
{
	//! The attribute set manages a set of attributes.
	/** The attribute set manages a set of attributes.
	 @see Kiwi::Attribute, Kiwi::Tag, Kiwi::Element
	 
	 @todo	- trigger notifications
	 */
	class AttributeSet
	{
	public:
		class Listener;
		
	private:
		string						m_name;
		map<sTag, sAttribute>       m_attributes;
		unordered_set<weak_ptr<Listener>,
		weak_ptr_hash<Listener>,
		weak_ptr_equal<Listener>>   m_listeners;
		
	public:
		
		//! Constructor.
		/** Creates a new attribute set.
		 */
		AttributeSet(const string& name = string()) noexcept;
		
		//! Descrutor.
		/** Free the attributes.
		 */
		virtual ~AttributeSet();
		
		//! Retrieves the name of the AttributeSet.
		/** Retrieves the name of the AttributeSet.
		 @return The name of the AttributeSet.
		 @see setName
		 */
		string getName() const noexcept				{return m_name;};
		
		//! Sets the name of the AttributeSet.
		/** Sets the name of the AttributeSet.
		 @param name The newName of the AttributeSet.
		 @see setName
		 */
		void setName(const string& name) noexcept	{m_name = name;}
		
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
		void removeAttributesWithNonCommonKey(shared_ptr<AttributeSet> attrset);
		
		// ================================================================================ //
		//								ATRIBUTESET::LISTENER                               //
		// ================================================================================ //
		
		//! The attribute listener is a virtual class that can be binded to an AttributeSet to be notified of the attributes changes.
		/** The attribute listener is a very light class that allows to be notified of the attributes modification.
			To 
		 */
		class Listener
		{
		public:
			virtual ~Listener() {}
			
			//! Receive the notification that an attribute has been added to the set.
			/** This method is called when an attribute has been added to the set.
			 @param attr   The newly added attribute.
			 @see attributeRemoved
			 */
			virtual void attributeAdded(sAttribute attr) {};
			
			//! Receive the notification that an attribute has been removed from the set.
			/** This method is called when an attribute has been removed from the set.
			 @param attr   The attribute that has been removed.
			 @see attributeAdded
			 */
			virtual void attributeRemoved(sAttribute attr) {};
			
			//! Receive the notification that an attribute of the set has been modified.
			/** This method is called when an attribute of the set has been modified.
			 @param attr   The attribute that has been modified.
			 @see attributeBehaviorChanges
			 */
			virtual void attributeValueChanged(sAttribute attr) {};
			
			//! Receive the notification that the behavior of an attribute of the set has been changed.
			/** This method is called when the behavior of an attribute of the set has been changed.
			 @param attr   The attribute that has its behavior been changed.
			 @see attributeModified
			 */
			virtual void attributeAppearanceChanged(sAttribute attr) {};
			
			//! Receive the notification that the behavior of an attribute of the set has been changed.
			/** This method is called when the behavior of an attribute of the set has been changed.
			 @param attr   The attribute that has its behavior been changed.
			 @see attributeModified
			 */
			virtual void attributeBehaviorChanged(sAttribute attr) {};
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
		
		//! Sets the attributes value with a dico.
		/** This method sets the attributes value with a dico.
		 @param dico The dico.
		 */
		void read(scDico dico) noexcept;
		
		//! Receive a notification from an attribute.
		/** The function receives the notifications from the attribute.
		 @param attr The attribute that notify.
		 */
		virtual void notify(sAttribute attr) {};
		
		//! Attribute maker.
		/** This function creates and adds an attribute to the attribute factory.
		 */
		template<class ObjectClass, class ...Args> void addAttribute(Args&& ...arguments)
		{
			sAttribute attr = make_shared<ObjectClass>(forward<Args>(arguments)...);
			m_attributes[attr->name()] = attr;
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
		void addAttributeSet(shared_ptr<AttributeSet> attrset);
		
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
	};
	
	//! The shared pointer of an attribute set.
	/**
	 The sAttributeSet is shared pointer of an attribute set.
	 */
	typedef shared_ptr<AttributeSet>         sAttributeSet;
}

#endif   // __DEF_KIWI_ATTRIBUTESET__
