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

#include "Tag.h"

// - Have to recheck all and threadsafe
namespace Kiwi
{
    // ================================================================================ //
    //                                      DICO                                        //
    // ================================================================================ //
    
    //! The dico is an associative container that manages atoms with keys like in the JSON format.
    /**
     The dico is just a container that allows to manages atoms with tags. The dico can be used to parse JSON files.
     */
    class Dico : public Atom::Quark, public enable_shared_from_this<Dico>
    {        
    private:
        map<sTag, vector<Atom>> m_entries;
        
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
        
        //! The dico creation method.
        /** The function allocates a dico.
         @param dico another dico.
         */
        static sDico create(scDico dico);
        
        //! The dico creation method.
        /** The function allocates a dico.
         */
        static sDico evaluateForJson(string const& text);
        
        //! The dico creation method.
        /** The function allocates a dico.
         */
        static sDico evaluateForObject(string const& text);
        
        //! The dico creation method.
        /** The function allocates a dico.
         */
        static sDico evaluateForLink(string const& text);
        
        //! Clear the dico.
        /** The function clears the dico.
         */
        void clear() noexcept;
        
        //! Retrieve the keys of the entries from a dico.
        /** The function retrieves the keys of the entries from a dico.
         @param atoms The vector of atoms that will owns the key of the entries.
         */
        void keys(vector<Atom>& atoms) const noexcept;
        
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
        size_t type(sTag key) const noexcept;
        
        //! Check if an entry is of type long.
        /** The function checks if an entry is of type long.
         @param key The name of the entry.
         @return    True if the entry is a long.
         */
        inline bool isLong(sTag key) const noexcept
        {
            return type(key) == Atom::LONG;
        }
        
        //! Check if an entry is of type double.
        /** The function checks if an entry is of type double.
         @param key The name of the entry.
         @return    True if the entry is a double.
         */
        inline bool isDouble(sTag key) const noexcept
        {
            return type(key) == Atom::DOUBLE;
        }
        
        //! Check if an entry is of type tag.
        /** The function checks if an entry is of type tag.
         @param key The name of the entry.
         @return    True if the entry is a tag.
         */
        inline bool isTag(sTag key) const noexcept
        {
            return type(key) == Atom::TAG;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type object.
         @param key The name of the entry.
         @return    True if the entry is an object.
         */
        inline bool isObject(sTag key) const noexcept
        {
            return type(key) == Atom::OBJECT;
        }
        
        //! Check if an entry is of type object.
        /** The function checks if an entry is of type dico.
         @param key The name of the entry.
         @return    True if the entry is a dico.
         */
        inline bool isDico(sTag key) const noexcept
        {
            return type(key) == Atom::DICO;
        }
        
        //! Check if an entry is of type atoms.
        /** The function checks if an entry is of type atoms.
         @param key The name of the entry.
         @return    True if the entry is atoms.
         */
        inline bool isAtoms(sTag key) const noexcept
        {
            return type(key) == Atom::VECTOR;
        }
        
        //! Retrieve the atom from a dico.
        /** The function retrieves the atom from a dico.
         @param key The name of the entry.
         @return    The atom from a dico.
         */
        const Atom get(sTag key) const noexcept;
        
        //! Retrieve the atom from a dico.
        /** The function retrieves the atom from a dico.
         @param key The name of the entry.
         @return    The atom from a dico.
         */
        Atom get(sTag key) noexcept;
        
        //! Retrieve the atoms from a dico.
        /** The function retrieves the atoms from a dico.
         @param key The name of the entry.
         @param atoms The atoms from a dico or empty atom list if the type isn't a vector of atoms.
         */
        void get(sTag key, vector<Atom>& atoms) const noexcept;
        
        //! Add a new entry with an atoms.
        /** The function adds a new entry or replace an old entry with an atoms.
         @param key The name of the entry.
         @param atom The atom.
         */
        void set(sTag key, Atom const& atom) noexcept;
        
        //! Add a new entry with a vector of atoms.
        /** The function adds a new entry or replace an old entry with a vector of atoms.
         @param key The name of the entry.
         @param atoms The vector of atoms.
         */
        void set(sTag key, vector<Atom> const& atoms) noexcept;
        
        //! Append an atom to an entry.
        /** The function adds a new entry with an atom or append an atom to an old entry.
         @param key The name of the entry.
         @param atom The atom.
         */
        void append(sTag key, Atom const& atom) noexcept;
    
        //! Append a vector of atoms to an entry.
        /** The function adds a new entry with a vector of atoms or append a vector of atoms to an old entry.
         @param key The name of the entry.
         @param atoms The vector of atoms.
         */
        void append(sTag key, vector<Atom> const& atoms) noexcept;
        
        //! Read a text file to fill the dico.
        /** The function reads a text file to fill the dico.
         @param filename        The name of the file.
         @param directoryname   The name of the directory.
         */
        void read(string const& filename, string const& directoryname);
        
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
        
        //! Write an atom in a string with the json format .
        /** This function writes an atom in a string with the json format .
         @param     atom The atom.
         @param     text The string.
         @param     line The indetation.
         @return    The unescaped string.
         */
        static void toJson(Atom const& atom, string& text, string indetation = "");
        
        //! Write atoms in a string with the json format .
        /** This function writes atoms in a string with the json format .
         @param     atoms The atoms.
         @param     text The string.
         @param     line The indetation.
         @return    The unescaped string.
         */
        static void toJson(vector<Atom> const& atoms, string& text, string indetation = "");
        
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
        
        //! Get the atom type of a string.
        /** This function gets the atom type of a string.
         @param     text The string.
         @param     pos The position in the string.
         @return    The type of the string.
         */
        static size_t getType(string const& text, string::size_type pos);
        
        //! Get a vector of atoms from a string in the json format .
        /** This function gets a vector of atoms from a string in the json format .
         @param     atoms The vector of atoms.
         @param     text The string.
         @param     pos The position in the string.
         */
        static void fromJson(vector<Atom>& atoms, string const& text, string::size_type& pos);
        
        //! Get a dico from a string in the json format .
        /** This function gets dico from a string in the json format .
         @param     dico The dico.
         @param     text The string.
         @param     pos The position in the string.
         */
        static void fromJson(sDico dico, string const& text, string::size_type& pos);
    };
    
    inline string toString(const scDico dico)
    {
        string text;
        dico->write(text);
        return text;
    }
}


#endif


