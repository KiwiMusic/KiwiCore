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

#ifndef __DEF_KIWI_DICO__
#define __DEF_KIWI_DICO__

#include "Defs.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    //! The dico is an associative container that manages atoms with keys.
    /**
     The dico is ...
     */
    class Dico
    {
    public:
        typedef map<sTag, Atom>::iterator iterator;
        typedef map<sTag, Atom>::const_iterator const_iterator;
        typedef map<sTag, Atom>::reverse_iterator reverse_iterator;
        typedef map<sTag, Atom>::const_reverse_iterator const_reverse_iterator;
        typedef map<sTag, Atom>::value_type value_type;
        typedef map<sTag, Atom>::size_type size_type;
    private:
        map<sTag, Atom> m_map;
    public:
        
        //! Empty constructor.
        /** Create a new empty dico.
         */
        Dico();
        
        //! Copy constructor.
        /** Create a copy of a dico.
         @param other A dico to copy.
         */
        Dico(const Dico& other);
        
        //! Range constructor.
        /** Create a dictionary with the elements between first an last.
         @param first The iterator to the first element to copy.
         @param last The iterator to the last element to copy.
         */
        Dico(iterator first, iterator last);
        
        //! Move constructor.
        /** Create a dictionary from by acquiring the elements from the other dico.
         @param other A dico to copy.
         */
        Dico(Dico&& other);
        
        //! List constructor.
        /** Create a dictionary from an initializer list.
         @param il The initializer list.
         */
        Dico(initializer_list<value_type> il);
        
        //! Destructor.
        /** Free the dictionary.
         */
        ~Dico() noexcept;
        
        //! Clear the dico.
        /** The function clears the dico.
         */
        void clear() noexcept;
        
        //! Return an iterator referring to the first element.
        /** The function returns an iterator referring to the first element.
         @return The iterator referring to the first element.
         */
        iterator begin() noexcept;
        
        //! Return an iterator referring to the first element.
        /** The function returns an iterator referring to the first element.
         @return The iterator referring to the first element.
         */
        const_iterator begin() const noexcept;
        
        //! Return a constant iterator referring to the first element.
        /** The function returns a constant iterator referring to the first element but it cannot be used to modify the contents.
         @return The constant iterator referring to the first element.
         */
        const_iterator cbegin() const noexcept;
        
        //! Return an reverse iterator referring to the last element.
        /** The function returns an reverse iterator referring to the last element.
         @return The reverse iterator referring to the last element.
         */
        reverse_iterator rbegin() noexcept;
        
        //! Return an constant reverse iterator referring to the last element.
        /** The function returns an constant reverse iterator referring to the last element.
         @return The constant reverse iterator referring to the last element.
         */
        const_reverse_iterator crbegin() const noexcept;
        
        //! Return an iterator referring to the last element.
        /** The function returns an iterator referring to the last element.
         @return The iterator referring to the last element.
         */
        iterator end() noexcept;
        
        //! Return an iterator referring to the last element.
        /** The function returns an iterator referring to the last element.
         @return The iterator referring to the last element.
         */
        const_iterator end() const noexcept;
        
        //! Return a constant iterator referring to the last element.
        /** The function returns a constant iterator referring to the last element but it cannot be used to modify the contents.
         @return The constant iterator referring to the last element.
         */
        const_iterator cend() const noexcept;
        
        //! Return an reverse iterator referring to the first element.
        /** The function returns an reverse iterator referring to the first element.
         @return The reverse iterator referring to the first element.
         */
        reverse_iterator rend() noexcept;
        
        //! Return an constant reverse iterator referring to the first element.
        /** The function returns an constant reverse iterator referring to the first element.
         @return The constant reverse iterator referring to the first element.
         */
        const_reverse_iterator crend() const noexcept;
        
        //! Search the container for a specific key.
        /** The function searches the container for a specific key.
         @param key The key to search.
         @return true if the key is found, otherwise false.
         */
        bool has(const sTag& key) const noexcept;
        
        //! Return whether the dico is empty.
        /** The function returns whether the dico is empty.
         @return true if the dico is empty, otherwise false.
         */
        bool empty() const noexcept;
        
        //! Return the maximum number of elements that the dico can hold.
        /** The function returns the maximum number of elements that the dico can hold.
         @return The maximum number of elements that the dico can hold.
         */
        size_type max() const noexcept;
        
        //! Return the number of elements in the dico.
        /** The function returns the number of elements in the dico.
         @return The number of elements in the dico.
         */
        size_type size() const noexcept;
        
        //! Remove from the dico a single element.
        /** The function removes from the dico a single element.
         @param position The iterator pointing to a single element to be removed.
         @return An iterator to the element that follows the last element removed.
         */
        iterator erase(const_iterator position);
        
        //! Remove from the dico a single element.
        /** The function removes from the dico a single element.
         @param key The key of the single element to be removed.
         @return true if the element has been removed, othewise false.
         */
        bool erase(const sTag& key);
        
        //! Remove from the dico a range of elements.
        /** The function removes from the dico a range of elements.
         @param first The iterator to the first element to remove.
         @param last The iterator to the last element to remove.
         @return An iterator to the element that follows the last element removed.
         */
        iterator erase(const_iterator first, const_iterator last);
        
        //! Search for a specific key and returns an iterator to it.
        /** The function searches for a specific key and returns an iterator to it.
         @param key The key to search.
         @return The iterator of the key if the key has been found, otherwise the end iterator.
         */
        iterator find(const sTag& key);
        
        //! Search for a specific key and returns an iterator to it.
        /** The function searches for a specific key and returns an iterator to it.
         @param key The key to search.
         @return The iterator of the key if the key has been found, otherwise the end iterator.
         */
        const_iterator find(const sTag& key) const;
        
        //! Insert a new element.
        /** The function inserts a new elements. The insertion operation checks if the dico has a already the key equivalent to the element, and if so, the element is not inserted.
         @param value The element to insert.
         @return A pair with an iterator to this existing element and a boolean with true if the element has been inserted, otherwise false.
         */
        pair<iterator,bool> insert(const value_type& value);
        
        //! Insert a range of elements.
        /** The function inserts a range of elements. The insertion operation checks if the dico has a already the key equivalent to the elements, and if not, the element is inserted.
         @param first The iterator to the first element to insert.
         @param last The iterator to the last element to insert.
         */
        void insert(iterator first, iterator last);
        
        //! Insert a range of elements with an initializer list.
        /** The function insertsa range of elements with an initializer list. The insertion operation checks the dico has a already the key equivalent to the element, and if so, the element is not inserted.
         @param il The initializer list.
         */
        void insert(initializer_list<value_type> il);
        
        Atom& operator[](const sTag& k);
        Atom& operator[](sTag&& k);
        
        Atom const& operator[](const sTag& k) const;
        Atom const& operator[](sTag&& k) const;
        
        Dico& operator=(const Dico& x);
        Dico& operator=(Dico&& x);
        Dico& operator=(initializer_list<value_type> il);
        void swap(Dico& other);
    };
}


#endif


