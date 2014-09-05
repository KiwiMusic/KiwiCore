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

#ifndef __DEF_KIWI_INSTANCE__
#define __DEF_KIWI_INSTANCE__

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Console.h"
#include "Object.h"
#include "ObjectExtented.h"
#include "Iolet.h"
#include "Box.h"
#include "Page.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    class Instance :  public TagFactory, public Console, public enable_shared_from_this<Instance>
    {
    private:
        friend shared_ptr<Box> Page::createBox(shared_ptr<Tag> name, vector<Element> &elements);
        
        int                                         m_untitled_pages;
        vector<shared_ptr<Page>>                    m_pages;
        map<shared_ptr<Tag>, unique_ptr<Object>>    m_prototypes;
        
    public:
        Instance();
        ~Instance();
        
        void init();
        
        shared_ptr<Object> allocObject(string name);
        shared_ptr<Object> allocObject(shared_ptr<Tag> name);
        shared_ptr<Object> createObject(string name);
        shared_ptr<Object> createObject(shared_ptr<Tag> name);
        shared_ptr<Dico>   createDico();
        
        shared_ptr<Page> createPage(string file = string(""), string directory = string(""));
        void closePage(shared_ptr<Page> page);
        void addObjectPrototype(unique_ptr<Object>);
        
        void startDsp(double samplerate, long vectorsize);
        
        void tickDsp();
        
        void stopDsp();
    };
}


#endif


