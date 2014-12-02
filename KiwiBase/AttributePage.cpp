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

#include "AttributePage.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ATTRIBUTE BOX                                   //
    // ================================================================================ //
	
	// Font //
    const sTag AttrPage::Tag_Font								= Tag::create("Font");
    const sTag AttrPage::Tag_fontname                           = Tag::create("fontname");
    const sTag AttrPage::Tag_Font_Name                          = Tag::create("Font Name");
    const sTag AttrPage::Tag_Arial                              = Tag::create("Arial");
    const sTag AttrPage::Tag_Menelo                             = Tag::create("Menelo");
    const sTag AttrPage::Tag_fontsize                           = Tag::create("fontsize");
    const sTag AttrPage::Tag_Font_Size                          = Tag::create("Font Size");
    const sTag AttrPage::Tag_fontface                           = Tag::create("fontface");
    const sTag AttrPage::Tag_Font_Face                          = Tag::create("Font Face");
    const sTag AttrPage::Tag_normal                             = Tag::create("normal");
    const sTag AttrPage::Tag_bold                               = Tag::create("bold");
    const sTag AttrPage::Tag_italic                             = Tag::create("italic");
    const sTag AttrPage::Tag_bold_italic                        = Tag::create("bold_italic");
    const sTag AttrPage::Tag_fontjustification                  = Tag::create("fontjustification");
    const sTag AttrPage::Tag_Font_Justification                 = Tag::create("Font Justification");
    const sTag AttrPage::Tag_left                               = Tag::create("left");
    const sTag AttrPage::Tag_center                             = Tag::create("center");
    const sTag AttrPage::Tag_right                              = Tag::create("right");
	
	// Color //
	const sTag AttrPage::Tag_Color                              = Tag::create("Color");
	const sTag AttrPage::Tag_editing_bgcolor                    = Tag::create("editing_bgcolor");
	const sTag AttrPage::Tag_Unlocked_Background_Color          = Tag::create("Unlocked Background Color");
	const sTag AttrPage::Tag_locked_bgcolor						= Tag::create("locked_bgcolor");
	const sTag AttrPage::Tag_Locked_Background_Color			= Tag::create("Locked Background Color");
	
	// Editing //
	const sTag AttrPage::Tag_Editing							= Tag::create("Editing");
	const sTag AttrPage::Tag_gridsize							= Tag::create("gridsize");
	const sTag AttrPage::Tag_Grid_Size							= Tag::create("Grid Size");
}
















