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

#include "Element.h"

// - Have to recheck all and threadsafe
namespace Kiwi
{
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    //! The dico is an associative container that manages elements with keys like in the JSON format.
    /**
     The dico is just a container that allows to manages elements with tags. The dico can be used to parse JSON files.
     */
    class Dico : public enable_shared_from_this<Dico>
    {        
    private:
        map<sTag, ElemVector> m_entries;
        
    public:
        
        //! Constructor.
        /** Create a new dictionary.
         */
        Dico() noexcept;
        
        //! Destructor.
        /** Free the dictionary.
         */
        ~Dico();
        
        //! The dico creation method.
        /** The function allocates a dico.
         */
        static sDico create();
        
        //! Clear the dico.
        /** The function clears the dico.
         */
        void clear() noexcept;
        
        //! Retrieve the keys of the entries from a dico.
        /** The function retrieves the keys of the entries from a dico.
         @param elements The vector of elements that will owns the key of the entries.
         */
        void keys(ElemVector& elements) const noexcept;
        
        //! Clear the entry of a dico.
        /** The function clears the entry of a dico.
         @param key The name of the entry.
         */
        void clear(sTag key) noexcept;
        
        //! Check if an entry exists.
        /** This function checks if an entry exists.
         @param key The name of the entry.
         @return True if the entry exists otherwise it returns false.
         */
        bool has(sTag key) const noexcept;
        
        //! Retrieve the type of an entry.
        /** The function retrieves the type of an entry.
         @param key The name of the entry.
         @return    The type of the entry.
         */
        Element::Type type(sTag key) const noexcept;
        
        //! Check if an entry is of type long.
        /** The function checks if an entry is of type long.
         @param key The name of the entry.
         @return    True if the entry is a long.
         */
        inline bool isLong(sTag key) const noexcept
        {
            return type(key) == Element::LONG;
        }
        
        //! Check if an entry is of type double.
        /** The function checks if an entry is of type double.
         @param key The name of the entry.
         @return    True if the entry is a double.
         */
        inline bool isDouble(sTag key) const noexcept
        {
            return type(key) == Element::DOUBLE;
        }
        
        //! Check if an entry is of type tag.
        /** The function checks if an entry is of type tag.
         @param key The name of the entry.
         @return    True if the entry is a tag.
         */
        inline bool isTag(sTag key) const noexcept
        {
            return type(key) == Element::TAG;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type object.
         @param key The name of the entry.
         @return    True if the entry is an object.
         */
        inline bool isObject(sTag key) const noexcept
        {
            return type(key) == Element::BOX;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type dico.
         @param key The name of the entry.
         @return    True if the entry is a dico.
         */
        inline bool isDico(sTag key) const noexcept
        {
            return type(key) == Element::DICO;
        }
        
        //! Check if an entry is of type elements.
        /** The function checks if an entry is of type elements.
         @param key The name of the entry.
         @return    True if the entry is elements.
         */
        inline bool isElements(sTag key) const noexcept
        {
            return type(key) == Element::VECTOR;
        }
        
        //! Retrieve the element from a dico.
        /** The function retrieves the element from a dico.
         @param key The name of the entry.
         @return    The element from a dico.
         */
        Element get(sTag key) const noexcept;
        
        //! Retrieve the elements from a dico.
        /** The function retrieves the elements from a dico.
         @param key The name of the entry.
         @param elements The elements from a dico or empty element list if the type isn't a vector of elements.
         */
        void get(sTag key, ElemVector& elements) const noexcept;
        
        //! Add a new entry with an elements.
        /** The function adds a new entry or replace an old entry with an elements.
         @param key The name of the entry.
         @param element The element.
         */
        void set(sTag key, Element const& element) noexcept;
        
        //! Add a new entry with a vector of elements.
        /** The function adds a new entry or replace an old entry with a vector of elements.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void set(sTag key, ElemVector const& elements) noexcept;
        
        //! Append an element to an entry.
        /** The function adds a new entry with an element or append an element to an old entry.
         @param key The name of the entry.
         @param element The element.
         */
        void append(sTag key, Element const& element) noexcept;
    
        //! Append a vector of elements to an entry.
        /** The function adds a new entry with a vector of elements or append a vector of elements to an old entry.
         @param key The name of the entry.
         @param elements The vector of elements.
         */
        void append(sTag key, ElemVector const& elements) noexcept;
        
        //! Read a text file to fill the dico.
        /** The function reads a text file to fill the dico.
         @param filename        The name of the file.
         @param directoryname   The name of the directory.
         */
        void read(string const& filename, string const& directoryname);
        
        //! Read a string to fill the dico.
        /** The function reads a string to fill the dico.
         @param text The string to read.
         */
        void read(string const& text);
        
        //! Write the dico in a text file.
        /** The function writes the dico in a text file.
         @param file The name of the file.
         @param directory The name of the directory.
         */
        void write(string const& filename, string const& directoryname) const;
        
        //! Write the dico in a string.
        /** The function writes the dico in a string.
         @param text The string.
         */
        void write(string& text) const;
    
    private:
        
        //! Escape a string for json.
        /** This function escapes a string for json.
         @param     text The string.
         @return    The escaped string.
         */
        static string jsonEscape(string const& text);
        
        //! Write an element in a string with the json format .
        /** This function writes an element in a string with the json format .
         @param     element The element.
         @param     text The string.
         @param     line The indetation.
         @return    The unescaped string.
         */
        static void toJson(Element const& element, string& text, string indetation = "");
        
        //! Write elements in a string with the json format .
        /** This function writes elements in a string with the json format .
         @param     elements The elements.
         @param     text The string.
         @param     line The indetation.
         @return    The unescaped string.
         */
        static void toJson(ElemVector const& elements, string& text, string indetation = "");
        
        //! Write a dico in a string with the json format .
        /** This function writes dico in a string with the json format .
         @param     dico The dico.
         @param     text The string.
         @param     line The indetation.
         @return    The unescaped string.
         */
        static void toJson(scDico dico, string& text, string indetation = "");
        
        //! Unescape a json string.
        /** This function unescapes a json string.
         @param     text The string.
         @return    The unescaped string.
         */
        static string jsonUnescape(string const& text, size_t& pos);
        
        //! Get the element type of a string.
        /** This function gets the element type of a string.
         @param     text The string.
         @param     pos The position in the string.
         @return    The type of the string.
         */
        static Element::Type getType(string const& text, string::size_type pos);
        
        //! Get a vector of elements from a string in the json format .
        /** This function gets a vector of elements from a string in the json format .
         @param     elements The vector of elements.
         @param     text The string.
         @param     pos The position in the string.
         */
        static void fromJson(ElemVector& elements, string const& text, string::size_type& pos);
        
        //! Get a dico from a string in the json format .
        /** This function gets dico from a string in the json format .
         @param     dico The dico.
         @param     text The string.
         @param     pos The position in the string.
         */
        static void fromJson(sDico dico, string const& text, string::size_type& pos);
        
        //! Get a dico from a string in the page format .
        /** This function gets dico from a string in the page format .
         @param     dico The dico.
         @param     text The string.
         */
        static void fromText(sDico dico, string const& text);
    };
    
    inline string toString(const sDico dico)
    {
        string text;
        dico->write(text);
        return text;
    }
}


#endif


