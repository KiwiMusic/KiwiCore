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


#include "Tag.h"

namespace Kiwi
{
    unordered_map<string, sTag> Tag::m_tags;
    mutex Tag::m_mutex;
    
    const sTag Tag::List::arguments             = Tag::create("arguments");
    const sTag Tag::List::bang                  = Tag::create("bang");
    const sTag Tag::List::box                   = Tag::create("box");
    const sTag Tag::List::color                 = Tag::create("color");
    const sTag Tag::List::Color                 = Tag::create("Color");
    const sTag Tag::List::boxes                 = Tag::create("boxes");
    const sTag Tag::List::from                  = Tag::create("from");
    const sTag Tag::List::focus                 = Tag::create("focus");
    const sTag Tag::List::link                  = Tag::create("link");
    const sTag Tag::List::links                 = Tag::create("links");
    const sTag Tag::List::mescolor              = Tag::create("mescolor");
    const sTag Tag::List::Message_Color         = Tag::create("Message Color");
    const sTag Tag::List::name                  = Tag::create("name");
    const sTag Tag::List::page                  = Tag::create("page");
    const sTag Tag::List::set                   = Tag::create("set");
    const sTag Tag::List::sigcolor              = Tag::create("sigcolor");
    const sTag Tag::List::Signal_Color          = Tag::create("Signal Color");
    const sTag Tag::List::to                    = Tag::create("to");
}









