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
    
    sTag Tag::text        = Tag::create("text");
    sTag Tag::box         = Tag::create("box");
    sTag Tag::boxes       = Tag::create("boxes");
    sTag Tag::link        = Tag::create("link");
    sTag Tag::links       = Tag::create("links");
    sTag Tag::id          = Tag::create("id");
    sTag Tag::from        = Tag::create("from");
    sTag Tag::to          = Tag::create("to");
    sTag Tag::name        = Tag::create("name");
    sTag Tag::ninlets     = Tag::create("ninlets");
    sTag Tag::noutlets    = Tag::create("noutlets");
    sTag Tag::set         = Tag::create("set");
    sTag Tag::null        = Tag::create("null");
    sTag Tag::arguments   = Tag::create("arguments");
}









