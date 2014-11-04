/*
 *  Kiwi.cp
 *  Kiwi
 *
 *  Created by Guillot Pierre on 02/11/2014.
 *  Copyright (c) 2014 PierreGuillot. All rights reserved.
 *
 */

#include <iostream>
#include "Kiwi.h"
#include "KiwiPriv.h"

void Kiwi::HelloWorld(const char * s)
{
	 KiwiPriv *theObj = new KiwiPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void KiwiPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

