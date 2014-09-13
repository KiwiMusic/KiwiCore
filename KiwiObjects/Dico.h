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
#include "Tag.h"
#include "Element.h"
#include "Object.h"

namespace Kiwi
{
    class Instance;
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    //! The dico is an associative container that manages elements with keys like in the JSON format.
    /**
     The dico is just a map container that allows to manages vector of elements with tags. The dico can be used to parse JSON files.
     */
    class Dico : public Object
    {
    private:
        map<const shared_ptr<Tag>, vector<Element>>  m_entries;
        
        void doread(shared_ptr<Dico> dico, ifstream& file, string& line);
        void dowrite(shared_ptr<Dico> dico, ofstream& file, string& line);
        void dopost(shared_ptr<Dico> dico, string& text, string line = "");
    public:
        
        //! Constructor.
        /** Create a new dictionary.
         */
        Dico(shared_ptr<Instance> kiwi);
        
        //! Destructor.
        /** Free the dictionary.
         */
        ~Dico();
        
        //! Clear the dico.
        /** The function clears the dico.
         */
        void clear() noexcept;
        
        //! Retrieve the keys of the entries from a dico.
        /** The function retrieves the keys of the entries from a dico.
         @param elements The vector of elements that will owns the key of the entries.
         */
        void keys(vector<Element>& elements) const noexcept;
        
        //! Clear the entry of a dico.
        /** The function clears the entry of a dico.
         @param key The name of the entry.
         */
        void clear(shared_ptr<Tag> key) noexcept;
        
        //! Clear the entry of a dico.
        /** The function clears the entry of a dico.
         @param key The name of the entry.
         */
        inline void clear(string const& key) noexcept
        {
            clear(createTag(key));
        }
        
        //! Check if an entry exists.
        /** This function checks if an entry exists.
         @param key The name of the entry.
         @return True if the entry exists otherwise it returns false.
         */
        bool has(shared_ptr<Tag> key) const noexcept;
        
        //! Check if an entry exists.
        /** This function checks if an entry exists.
         @param key The name of the entry.
         @return True if the entry exists otherwise it returns false.
         */
        inline bool has(string const& key) const noexcept
        {
            return has(createTag(key));
        }
        
        //! Retrieve the type of an entry.
        /** The function retrieves the type of an entry.
         @param key The name of the entry.
         @return    The type of the entry.
         */
        Type type(shared_ptr<Tag> key) const noexcept;
        
        //! Retrieve the type of an entry.
        /** The function retrieves the type of an entry.
         @param key The name of the entry.
         @return    The type of the entry.
         */
        inline Type type(string const& key) const noexcept
        {
            return type(createTag(key));
        }
        
        //! Check if an entry is of type long.
        /** The function checks if an entry is of type long.
         @param key The name of the entry.
         @return    True if the entry is a long.
         */
        inline bool isLong(shared_ptr<Tag> key) const noexcept
        {
            return type(key) == T_LONG;
        }
        
        //! Check if an entry is of type long.
        /** The function checks if an entry is of type long.
         @param key The name of the entry.
         @return    True if the entry is a long.
         */
        inline bool isLong(string const& key) const noexcept
        {
            return type(key) == T_LONG;
        }
        
        //! Check if an entry is of type double.
        /** The function checks if an entry is of type double.
         @param key The name of the entry.
         @return    True if the entry is a double.
         */
        inline bool isDouble(shared_ptr<Tag> key) const noexcept
        {
            return type(key) == T_DOUBLE;
        }
        
        //! Check if an entry is of type double.
        /** The function checks if an entry is of type double.
         @param key The name of the entry.
         @return    True if the entry is a double.
         */
        inline bool isDouble(string const& key) const noexcept
        {
            return type(key) == T_DOUBLE;
        }
        
        //! Check if an entry is of type tag.
        /** The function checks if an entry is of type tag.
         @param key The name of the entry.
         @return    True if the entry is a tag.
         */
        inline bool isTag(shared_ptr<Tag> key) const noexcept
        {
            return type(key) == T_TAG;
        }
        
        //! Check if an entry is of type tag.
        /** The function checks if an entry is of type tag.
         @param key The name of the entry.
         @return    True if the entry is a tag.
         */
        inline bool isTag(string const& key) const noexcept
        {
            return type(key) == T_TAG;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type object.
         @param key The name of the entry.
         @return    True if the entry is a object.
         */
        inline bool isObject(shared_ptr<Tag> key) const noexcept
        {
            return type(key) == T_OBJECT;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type object.
         @param key The name of the entry.
         @return    True if the entry is a object.
         */
        inline bool isObject(string const& key) const noexcept
        {
            return type(key) == T_OBJECT;
        }
        
        //! Check if an entry is of type elements.
        /** The function checks if an entry is of type elements.
         @param key The name of the entry.
         @return    True if the entry is elements.
         */
        inline bool isElements(shared_ptr<Tag> key) const noexcept
        {
            return type(key) == T_ELEMENTS;
        }
        
        //! Check if an entry is of type elements.
        /** The function checks if an entry is of type elements.
         @param key The name of the entry.
         @return    True if the entry is elements.
         */
        inline bool isElements(string const& key) const noexcept
        {
            return type(key) == T_ELEMENTS;
        }
        
        //! Retrieve the element from a dico.
        /** The function retrieves the element from a dico.
         @param key The name of the entry.
         @return    The element from a dico.
         */
        Element get(shared_ptr<Tag> key) const noexcept;
        
        //! Retrieve the element from a dico.
        /** The function retrieves the element from a dico.
         @param key The name of the entry.
         @return    The element from a dico.
         */
        inline Element get(string const& key) const noexcept
        {
            return get(createTag(key));
        }
        
        //! Retrieve the elements from a dico.
        /** The function retrieves the elements from a dico.
         @param key The name of the entry.
         @param elements The elements from a dico or empty element list if the type isn't a vector of elements.
         */
        void get(shared_ptr<Tag> key, vector<Element>& elements) const noexcept;
        
        //! Retrieve the elements from a dico.
        /** The function retrieves the elements from a dico.
         @param key The name of the entry.
         @param elements The elements from a dico or empty element list if the type isn't a vector of elements.
         */
        inline void get(string const& key, vector<Element>& elements) const noexcept
        {
            get(createTag(key), elements);
        }
        
        //! Add a new entry with an elements.
        /** The function adds a new entry or replace an old entry with an elements.
         @param key The name of the entry.
         @param element The element.
         */
        void set(shared_ptr<Tag> key, Element const& element) noexcept;
        
        //! Add a new entry with an elements.
        /** The function adds a new entry or replace an old entry with an elements.
         @param key The name of the entry.
         @param element The element.
         */
        inline void set(string const& key, Element const& element) noexcept
        {
            set(createTag(key), element);
        }
        
        //! Add a new entry with a vector of elements.
        /** The function adds a new entry or replace an old entry with a vector of elements.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void set(shared_ptr<Tag> key, vector<Element> const& elements) noexcept;
        
        //! Add a new entry with a vector of elements.
        /** The function adds a new entry or replace an old entry with a vector of elements.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        inline void set(string const& key, vector<Element> const& elements) noexcept
        {
            set(createTag(key), elements);
        }
        
        //! Append an element to an entry.
        /** The function adds a new entry with an element or append an element to an old entry.
         @param key The name of the entry.
         @param element The element.
         */
        void append(shared_ptr<Tag> key, Element const& element) noexcept;
        
        //! Append an element to an entry.
        /** The function adds a new entry with an element or append an element to an old entry.
         @param key The name of the entry.
         @param element The element.
         */
        inline void append(string const& key, Element const& element) noexcept
        {
            append(createTag(key), element);
        }

        //! Append a vector of elements to an entry.
        /** The function adds a new entry with a vector of elements or append a vector of elements to an old entry.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void append(shared_ptr<Tag> key, vector<Element> const& elements) noexcept;
        
        //! Append a vector of elements to an entry.
        /** The function adds a new entry with a vector of elements or append a vector of elements to an old entry.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        inline void append(string const& key, vector<Element> const& elements) noexcept
        {
            append(createTag(key), elements);
        }
        
        //! Write the dico in a text file.
        /** The function writes the dico in a text file.
         @param file The name of the file.
         @param directory The name of the directory.
         */
        void write(string const& filename, string const& directoryname = "");
        
        //! Read a text file to fill the dico.
        /** The function reads a text file to fill the dico.
         @param filename        The name of the file.
         @param directoryname   The name of the directory.
         */
        void read(string const& filename, string const&directoryname = "");
        
        //! Read a string to fill the dico.
        /** The function reads a string to fill the dico.
         @param text The string to read.
         */
        void readFormatted(string const& text);
        
        //! Post the content of the dico.
        /** The posts the content of the dico.
         */
        void post();
    };
}


#endif


