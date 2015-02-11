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

#ifndef __DEF_KIWI_CLOCK__
#define __DEF_KIWI_CLOCK__

#include "Atom.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      CLOCK                                       //
    // ================================================================================ //
    
    //! The clock is used to defer the execution of a function.
    /**
     The clock can be used by a clack maker to call one of the tick funtions after a specific delay. The clock creates a new thread that will sleep for a specific time before calling the tick funtion of the clock maker.
     */
    class Clock : public enable_shared_from_this<Clock>
    {
    public:
        class Maker;
        typedef shared_ptr<Maker> sMaker;
        typedef weak_ptr<Maker>   wMaker;
    private:
        atomic_ulong m_used;
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick(wClock clock, ulong ms, wMaker maker);
        
        //! The function that will be call be the thread.
        /** You should never use this method except if you really know what you do.
         */
        static void tick_atoms(wClock clock, ulong ms, wMaker maker, vector<Atom> const& atoms);
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Clock() : m_used(0)
        {
            ;
        }
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Clock()
        {
            ;
        }
        
        //! Clock creator.
        /** This function create a new clock.
         @return The clock.
         */
        static inline sClock create()
        {
            return make_shared<Clock>();
        }
        
        //! Delay the call of the tick function of a clock maker.
        /** This function delay the call of the tick function of a clock maker.
         @param  maker      The maker that will be used.
         @param  ms         The delay time in milliseconds.
         */
        void delay(sMaker maker, const ulong ms);
        
        //! Delay the call of the tick function of a clock maker.
        /** This function delay the call of the tick function of a clock maker.
         @param  maker      The maker that will be used.
         @param  atoms   The atoms that will be send to the function.
         @param  ms         The delay time in milliseconds.
         */
        void delay(sMaker maker, vector<Atom> const& atoms, const ulong ms);
        
        //! Delay the call of the tick function of a clock maker.
        /** This function delay the call of the tick function of a clock maker.
         @param  object        The object that will be used.
         @param  ms         The delay time in milliseconds.
         */
        void delay(sObject object, const ulong ms);
        
        //! Delay the call of the tick function of a clock maker.
        /** This function delay the call of the tick function of a clock maker.
         @param  object        The object that will be used.
         @param  atoms   The atoms that will be send to the function.
         @param  ms         The delay time in milliseconds.
         */
        void delay(sObject object, vector<Atom> const& atoms, const ulong ms);
        
        // ================================================================================ //
        //                                      MAKER                                       //
        // ================================================================================ //
        
        class Maker
        {
        public:
            //! The constructor.
            /** The constructor does nothing.
             */
            Maker() noexcept
            {
                ;
            }
            
            //! The destructor.
            /** The destructor does nothing.
             */
            virtual ~Maker()
            {
                ;
            }
            
            //! The tick function that must be override.
            /** The tick function is called by a clock after a delay.
             */
            virtual void tick()
            {
                ;
            }
            
            //! The tick function that must be override.
            /** The tick function is called by a clock after a delay.
             @param  atoms   The atoms that sent by the clock.
             */
            virtual void tick(vector<Atom> const& atoms)
            {
                ;
            }
        };
        
    };
};


#endif


