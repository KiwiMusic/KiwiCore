/*
 *  kiwi_api.cp
 *  kiwi_api
 *
 *  Created by Guillot Pierre on 02/11/2014.
 *  Copyright (c) 2014 PierreGuillot. All rights reserved.
 *
 */

#include <iostream>
#include "kiwi_api.h"
#include "kiwi_apiPriv.h"

void kiwi_api::HelloWorld(const char * s)
{
	 kiwi_apiPriv *theObj = new kiwi_apiPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void kiwi_apiPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

