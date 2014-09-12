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

#ifndef __DEF_KIWI_PAGE__
#define __DEF_KIWI_PAGE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"
#include "Box.h"

// Later figure out if we use connection pointer or not

namespace Kiwi
{
    class DspContext;
    class Connection;
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and connections.
    /**
     The page is the counterpart of the max patcher or the pd canvas.
     */
    class Page : public Object, public ObjectListener
    {
    private:
        string                      m_file;
        string                      m_directory;
        
        set<shared_ptr<Box>>        m_boxes;
        set<shared_ptr<Connection>> m_connections;
        shared_ptr<DspContext>      m_dsp_context;
        
    public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Page(shared_ptr<Instance> kiwi, string file, string directory);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Page();
        
        void write();
        
        //! Create a box.
        /** The function instantiates a box in the page.
         @param name        The name of the box.
         @param elements    A vector of elements.
         @return A pointer to the box.
         */
        shared_ptr<Box> createBox(shared_ptr<Tag> name, vector<Element> const& elements);
        
        //! Create a box.
        /** The function instantiates a box in the page.
         @param name        The name of the box.
         @param elements    A vector of elements.
         @return A pointer to the box.
         */
        shared_ptr<Box> createBox(string name, vector<Element> const& elements);

        //! Create a box.
        /** The function instantiates a box in the page.
         @param name        The name of the box.
         @param element     An element.
         @return A pointer to the box.
         */
        shared_ptr<Box> createBox(shared_ptr<Tag> name, Element const& element);
        
        //! Create a box.
        /** The function instantiates a box in the page.
         @param name        The name of the box.
         @param element     An element.
         @return A pointer to the box.
         */
        shared_ptr<Box> createBox(string name, Element const& element);

        //! Free a box.
        /** The function removes a box from the page.
         @param box        The pointer to the box.
         */
        void freeBox(shared_ptr<Box> box);
        
        //! Check if an inlet is compatible with an outlet.
        /** The function checks if the inlet is compatible with the outlet.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         @return True if the inlet is compatible otherwise false.
         */
        bool compatible(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
        
        //! Create a connection between an outlet to an inlet.
        /** The function connects an inlet to the outlet.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         @return True if the inlet has been connected otherwise false.
         */
        bool connect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
        
        //! Delete a connection or a set of connection.
        /** The function disconnects an inlet from an outlet if all the arguments are specified, an inlet from all the outlets if the outlet's box is null, an outlet from all the inlets if the inlet's box is null, all the inlets of a box if only the inlet's box is specified and the inlet index is negative and all the outlets if only the outlet's box is specified and the outlet index is negative.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         */
        void disconnect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
        
        void startDsp(double samplerate, long vectorsize);
        
        void tickDsp();
        
        void stopDsp();
        
        //! Retrieve the file name of the page.
        /** The function retrieves the file name of the page.
         @return The file name as a string.
         */
		inline string getFileName() const noexcept
        {
            return m_file;
        }
        
        //! Retrieve the directory name of the page.
        /** The function retrieves the directory name of the page.
         @return The directory name as a string.
         */
        inline string getDirectoryName() const noexcept
        {
            return m_directory;
        }
    };
}


#endif


