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

#include "AttributeBox.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ATTRIBUTE BOX                                   //
    // ================================================================================ //
    
    const sTag AttrBox::Tag_Font               = Tag::create("Font");
    const sTag AttrBox::Tag_fontname                           = Tag::create("fontname");
    const sTag AttrBox::Tag_Font_Name                          = Tag::create("Font Name");
    const sTag AttrBox::Tag_Arial                              = Tag::create("Arial");
    const sTag AttrBox::Tag_Menelo                             = Tag::create("Menelo");
    const sTag AttrBox::Tag_fontsize                           = Tag::create("fontsize");
    const sTag AttrBox::Tag_Font_Size                          = Tag::create("Font Size");
    const sTag AttrBox::Tag_fontface                           = Tag::create("fontface");
    const sTag AttrBox::Tag_Font_Face                          = Tag::create("Font Face");
    const sTag AttrBox::Tag_normal                             = Tag::create("normal");
    const sTag AttrBox::Tag_bold                               = Tag::create("bold");
    const sTag AttrBox::Tag_italic                             = Tag::create("italic");
    const sTag AttrBox::Tag_bold_italic                        = Tag::create("bold_italic");
    const sTag AttrBox::Tag_fontjustification                  = Tag::create("fontjustification");
    const sTag AttrBox::Tag_Font_Justification                 = Tag::create("Font Justification");
    const sTag AttrBox::Tag_left                               = Tag::create("left");
    const sTag AttrBox::Tag_center                             = Tag::create("center");
    const sTag AttrBox::Tag_right                              = Tag::create("right");
    
    const sTag AttrBox::Tag_Appearance   = Tag::create("Appearance");
    const sTag AttrBox::Tag_position                     = Tag::create("position");
    const sTag AttrBox::Tag_Position                     = Tag::create("Position");
    const sTag AttrBox::Tag_size                         = Tag::create("size");
    const sTag AttrBox::Tag_Size                         = Tag::create("Size");
    const sTag AttrBox::Tag_presentation_position        = Tag::create("presentation_position");
    const sTag AttrBox::Tag_Presentation_Position        = Tag::create("Presentation Position");
    const sTag AttrBox::Tag_presentation_size            = Tag::create("presentation_size");
    const sTag AttrBox::Tag_Presentation_Size            = Tag::create("Presentation Size");
    const sTag AttrBox::Tag_hidden                       = Tag::create("hidden");
    const sTag AttrBox::Tag_Hide_on_Lock                 = Tag::create("Hide on Lock");
    const sTag AttrBox::Tag_presentation                 = Tag::create("presentation");
    const sTag AttrBox::Tag_Include_in_Presentation      = Tag::create("Include in Presentation");
    
    const sTag AttrBox::Tag_Color       = Tag::create("Color");
    const sTag AttrBox::Tag_bgcolor                             = Tag::create("bgcolor");
    const sTag AttrBox::Tag_Background_Color                    = Tag::create("Background Color");
    const sTag AttrBox::Tag_bdcolor                             = Tag::create("bdcolor");
    const sTag AttrBox::Tag_Border_Color                        = Tag::create("Border Color");
    const sTag AttrBox::Tag_textcolor                           = Tag::create("textcolor");
    const sTag AttrBox::Tag_Text_Color                          = Tag::create("Text Color");
}
















