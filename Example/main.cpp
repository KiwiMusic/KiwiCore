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
        dico->read("test2.kiwi", "/Users/Pierre/Desktop");
        sPage page = kiwi->createPage(dico);
        {
            dico->read("- 1.2");
            page->createBox(dico);
            dico->clear();
            dico->set(Tag::from, {3, 0});
            dico->set(Tag::to, {4, 0});
            page->createConnection(dico);
            dico->clear();
            
            dico->read("test2.kiwi", "/Users/Pierre/Desktop");
            page->append(dico);
            vector<sBox> boxes;
            page->getBoxes(boxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                if(boxes[i]->getName() == Tag::create("+"))
                {
                    boxes[i]->receive(0, {i+1});
                    cout << endl;
                }
            }
            page->bringToFront(boxes[0]);
            page->getBoxes(boxes);
            for(vector<sBox>::size_type i = 0; i < boxes.size(); i++)
            {
                if(boxes[i]->getName() == Tag::create("+"))
                {
                    boxes[i]->receive(0, {i+1});
                    cout << endl;
                }
            }
            
            dico->clear();
            page->write(dico);
            dico->write("test.kiwi", "/Users/Pierre/Desktop");
        }
    }
    return 0;
}