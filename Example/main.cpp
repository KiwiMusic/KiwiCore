//
//  main.cpp
//  KiwiBase
//
//  Created by Guillot Pierre on 05/09/2014.
//  Copyright (c) 2014 PierreGuillot. All rights reserved.
//

#include "../Kiwi.h"
#include <time.h>

using namespace Kiwi;

//==============================================================================
int main (int argc, char* argv[])
{
    sInstance kiwi = Instance::create();
    {
        Console::post("---------");
        sDico dico = Dico::create();
        sDico dico2 = Dico::create();
        dico->read("test2.kiwi", "/Users/Pierre/Desktop");
        sPage page = kiwi->createPage(dico);
        {
            vector<sBox> boxes;
            page->getBoxes(boxes);
            if(boxes.size())
            {
                sBox box = boxes[0];
                box->receive(1, {1});
                box->receive(0, {2});
            }
            
            /*
            sBox obj1 = page->createBox("+");
            sBox obj2 = page->createBox("+ 2");
            sBox obj3 = page->createBox("+ 1");
            page->connect(obj1, 0, obj2, 0);
            page->connect(obj3, 0, obj2, 1);
             */
            //obj1->receive(1, {12});
            //obj1->receive(0, {1});
            page->write(dico2);
            dico2->write("test.kiwi", "/Users/Pierre/Desktop");
        }
        
    }
    return 0;
}