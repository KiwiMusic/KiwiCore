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
#include "Json.h"

namespace Kiwi
{
    class Instance;
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    //! The dico holds the values in the json format type
    /**
     The dico blablabli blablabla...
     */
    class Dico : public Object
    {
    private:
        map<shared_ptr<Tag>, shared_ptr<Dico>>  m_entries;
        vector<Element>                         m_elements;
        
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
        void clear();
        
        //! Retrieve the keys of the entries from a dico.
        /** The function retrieves the keys of the entries from a dico.
         @param elements The vector of elements that will owns the key of the entries.
         */
        void getKeys(vector<Element>& elements);
        
        //! Check if an entry exists.
        /** This function checks if an entry exists.
         @param key The name of the entry.
         @return True if the entry exists otherwise it returns false.
         */
        bool has(shared_ptr<Tag> key);
        
        //! Retrieve the entry from a dico.
        /** The function retrieves the entry from a dico.
         @param key The name of the entry.
         @return The entry if it exists.
         */
        shared_ptr<Dico> get(shared_ptr<Tag> key);
        
        //! Clear the entry of a dico.
        /** The function clears the entry of a dico.
         @param key The name of the entry.
         */
        void clear(shared_ptr<Tag> key);
        
        //! Retrieve the type of an entry.
        /** The function retrieves the type of an entry.
         @param key The name of the entry.
         @return    The type of the entry.
         */
        Type type(shared_ptr<Tag> key);
        
        //! Check if an entry is of type long.
        /** The function checks if an entry is of type long.
         @param key The name of the entry.
         @return    True if the entry is a long.
         */
        bool isLong(shared_ptr<Tag> key);
        
        //! Check if an entry is of type double.
        /** The function checks if an entry is of type double.
         @param key The name of the entry.
         @return    True if the entry is a double.
         */
        bool isDouble(shared_ptr<Tag> key);
        
        //! Check if an entry is of type tag.
        /** The function checks if an entry is of type tag.
         @param key The name of the entry.
         @return    True if the entry is a tag.
         */
        bool isTag(shared_ptr<Tag> key);
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type object.
         @param key The name of the entry.
         @return    True if the entry is a object.
         */
        bool isObject(shared_ptr<Tag> key);
        
        //! Check if an entry is of type elements.
        /** The function checks if an entry is of type elements.
         @param key The name of the entry.
         @return    True if the entry is elements.
         */
        bool isElements(shared_ptr<Tag> key);
        
        //! Check if an entry is of type entries.
        /** The function checks if an entry is of type entries.
         @param key The name of the entry.
         @return    True if the entry is a vector of entries.
         */
        bool isEntries(shared_ptr<Tag> key);
        
        //! Retrieve the long value from a dico.
        /** The function retrieves the long value from a dico.
         @param key The name of the entry.
         @return    The long value from a dico or zero if the type is neither long or double.
         */
        long getLong(shared_ptr<Tag> key);
        
        //! Retrieve the double value from a dico.
        /** The function retrieves the double value from a dico.
         @param key The name of the entry.
         @return   The double value from a dico or zero if the type is neither long or double.
         */
        double getDouble(shared_ptr<Tag> key);
        
        //! Retrieve the tag from a dico.
        /** The function retrieves the tag from a dico.
         @param key The name of the entry.
         @return    The tag from a dico or nullptr if the type isn't a tag.
         */
        shared_ptr<Tag> getTag(shared_ptr<Tag> key);
        
        //! Retrieve the dico from a dico.
        /** The function retrieves the dico from a dico.
         @param key The name of the entry.
         @return    The dico from a dico or nullptr if the type isn't a dico.
         */
        shared_ptr<Object> getObject(shared_ptr<Tag> key);
        
        //! Retrieve the elements from a dico.
        /** The function retrieves the elements from a dico.
         @param key The name of the entry.
         @param elements The elements from a dico or empty element list if the type isn't a vector of elements.
         */
        const vector<Element>& getElements(shared_ptr<Tag> key);
        
        //! Add a new entry with a int value.
        /** The function adds a new entry or replace an old entry with a int value.
         @param key The name of the entry.
         @param value The int value.
         */
        void set(shared_ptr<Tag> key, int value);
        
        //! Add a new entry with a long value.
        /** The function adds a new entry or replace an old entry with a long value.
         @param key The name of the entry.
         @param value The long value.
         */
        void set(shared_ptr<Tag> key, long value);
        
        //! Add a new entry with a float value.
        /** The function adds a new entry or replace an old entry with a float value.
         @param key The name of the entry.
         @param value The float value.
         */
        void set(shared_ptr<Tag> key, float value);
        
        //! Add a new entry with a double value.
        /** The function adds a new entry or replace an old entry with a double value.
         @param key The name of the entry.
         @param value The double value.
         */
        void set(shared_ptr<Tag> key, double value);
        
        //! Add a new entry with a string value.
        /** The function adds a new entry or replace an old entry with a string value.
         @param key The name of the entry.
         @param value The string.
         */
        void set(shared_ptr<Tag> key, string value);
        
        //! Add a new entry with a tag.
        /** The function adds a new entry or replace an old entry with a tag.
         @param key The name of the entry.
         @param tag The tag.
         */
        void set(shared_ptr<Tag> key, shared_ptr<Tag> tag);
        
        //! Add a new entry with an object.
        /** The function adds a new entry or replace an old entry with a object.
         @param key The name of the entry.
         @param object The object.
         */
        void set(shared_ptr<Tag> key, shared_ptr<Object> object);
        
        //! Add a new entry with a vector of elements.
        /** The function adds a new entry or replace an old entry with a vector of elements.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void set(shared_ptr<Tag> key, vector<Element> const& elements);
        
        //! Append a int value to an entry.
        /** The function adds a new entry with a int value or append an int value to an old entry.
         @param key The name of the entry.
         @param value The int value.
         */
        void append(shared_ptr<Tag> key, int value);
        
        //! Append a long value to an entry.
        /** The function adds a new entry with a long value or append a long value to an old entry.
         @param key The name of the entry.
         @param value The long value.
         */
        void append(shared_ptr<Tag> key, long value);
        
        //! Append a float value to an entry.
        /** The function adds a new entry with a float value or append a float value to an old entry.
         @param key The name of the entry.
         @param value The float value.
         */
        void append(shared_ptr<Tag> key, float value);
        
        //! Append a double value to an entry.
        /** The function adds a new entry with a double value or append a double value to an old entry.
         @param key The name of the entry.
         @param value The double value.
         */
        void append(shared_ptr<Tag> key, double value);
        
        //! Append a string to an entry.
        /** The function adds a new entry with a string or append a string to an old entry.
         @param key The name of the entry.
         @param value The string.
         */
        void append(shared_ptr<Tag> key, string value);
        
        //! Append a tag to an entry.
        /** The function adds a new entry with a tag or append a tag to an old entry.
         @param key The name of the entry.
         @param tag The tag.
         */
        void append(shared_ptr<Tag> key, shared_ptr<Tag> tag);
        
        //! Append an object to an entry.
        /** The function adds a new entry with a object or append an object to an old entry.
         @param key The name of the entry.
         @param object The object.
         */
        void append(shared_ptr<Tag> key, shared_ptr<Object> object);
        
        //! Append a vector of elements to an entry.
        /** The function adds a new entry with a vector of elements or append a vector of elements to an old entry.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void append(shared_ptr<Tag> key, vector<Element> const& elements);
        
        //! Write the dico in a text file.
        /** The function writes the dico in a text file.
         @param file The name of the file.
         @param directory The name of the directory.
         */
        void write(string filename, string directoryname = "");
        
        //! Read a dico from a text file.
        /** The function reads a dico from a text file.
         @param filename        The name of the file.
         @param directoryname   The name of the directory.
         */
        void read(string filename, string directoryname = "");
        
        //! Post the content of the dico.
        /** The posts the content of the dico.
         */
        void post();
    };
}


#endif


