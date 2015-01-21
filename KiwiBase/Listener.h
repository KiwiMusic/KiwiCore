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

#ifndef __DEF_KIWI_CONTROLLER__
#define __DEF_KIWI_CONTROLLER__

#include "Attribute.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      CONTROLLER                                  //
    // ================================================================================ //
    class Listener;
    typedef shared_ptr<Listener>   sListener;
    typedef weak_ptr<Listener>     wListener;
    
    //! The controller.
    /**
     The controller...
     */
    class Listener
    {
    public:
        Listener() noexcept
        {
            ;
        }
        
        virtual ~Listener()
        {
            ;
        }
        
        //! Receives notification when an attribute value has changed.
        /** The function receives notification when an attribute value has changed.
         @param attr The attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        //virtual bool attributeChanged(sAttr attr) = 0;
        
        class Manager
        {
        private:
            set<wListener,
            owner_less<wListener>>  m_ctrls;
            mutable mutex           m_mutex;
        public:
            
            //! The constructor.
            /** You should never use this method.
             */
            Manager() noexcept
            {
                ;
            }
            
            //! The destructor.
            /** You should never use this method.
             */
            ~Manager();
            
            //! Add a controller to the controller owner.
            /** The function adds a controller to the controller owner.
             @param ctrl    The controller.
             */
            void addListener(sListener ctrl);
            
            //! Remove a controller to the controller owner.
            /** The function removes a controller to the controller owner.
             @param ctrl    The controller.
             */
            void removeListener(sListener ctrl);
        };
    };
}


#endif


