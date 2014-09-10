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

#ifndef __DEF_KIWI_JSON__
#define __DEF_KIWI_JSON__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                          JSON                                    //
    // ================================================================================ //
    
    //! The json class is a parser for JSON files from/to dico.
    /**
        The json class facilitates the writting of kiwi stuff in the JSON format. The class is relatively straightforward and doesn't check every potential errors in the file.
     */
    class Json : public Object
    {
    private:
        fstream         m_file;
        
        void doread(shared_ptr<Dictionary> dico, string line = "");
        void dowrite(shared_ptr<Dictionary> dico, string line = "");
        void dopost(shared_ptr<Dictionary> dico, string& text, string line = "");
        
    public:
        //! Constructor.
        /** 
         Create a new json.
         */
        Json(shared_ptr<Instance> kiwi);
        
        //! Destructor.
        /**
         Free the json.
         */
        ~Json();
        
        //! Post a dico in the JSON format.
        /** The function parses a dico to a JSON file and posts it.
         @param dico            The dico.
         */
        void post(shared_ptr<Dictionary> dico);
        
        //! Read a JSON file and fill a dico.
        /** The function parses a JSON file to a dico.
         @param dico            The dico.
         @param filename        The name of the file.
         @param filedirectory   The name of the directory.
         */
        void read(shared_ptr<Dictionary> dico, string filename, string directoryname = "");
        
        //! Write a JSON file from dico.
        /** The function parses a dico to a JSON file.
         @param dico            The dico.
         @param filename        The name of the file.
         @param filedirectory   The name of the directory.
         */
        void write(shared_ptr<Dictionary> dico, string filename, string directoryname = "");
    };
}
#endif
