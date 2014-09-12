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

#ifndef __DEF_KIWI_CONNECTION__
#define __DEF_KIWI_CONNECTION__

#include "Box.h"

namespace Kiwi
{
    class Box;
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    //! The connection owns to a page and is used to create a patch lines.
    /**
     The connection is opaque, you shouldn't have to use it at all.
     */
    class Connection
    {
    private:

        const weak_ptr<Box> m_from;
        const int           m_outlet;
        const weak_ptr<Box> m_to;
        const int           m_inlet;
        bool                m_selected;
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Connection(const shared_ptr<Box> from, int outlet, const shared_ptr<Box> to, int inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Connection();
        
        //! Check if the connection carries signal.
        /** The function checks if the connection carries signal.
         @return True is the connection carries signal otherwise it returns false.
         */
        bool isDsp() const noexcept;
        
        //! Retrieve the output box.
        /** The function retrieves the output box.
         @return The output box.
         */
        shared_ptr<Box> getFrom();
        
        //! Retrieve the index of the outlet.
        /** The function retrieves the index of the outlet.
         @return The index of the outlet.
         */
        int getOutletIndex();
        
        //! Retrieve the input box.
        /** The function retrieves the input box.
         @return The input box.
         */
        shared_ptr<Box> getTo();
        
        //! Retrieve the index of the inlet.
        /** The function retrieves the index of the inlet.
         @return The index of the inlet.
         */
        int getInletIndex();
        
        //! Write the connection in a dico.
        /** The function writes the connection in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico);
        
        //! Compare two connections
        /** The function compares the connections with another connections.
         @param other   The connections.
         @return true if the connections share some similar values otherwise false.
         */
        bool operator==(const Connection& other) noexcept;
    };
}


#endif


