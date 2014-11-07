//
//  main.cpp
//  KiwiBase
//
//  Created by Guillot Pierre on 05/09/2014.
//  Copyright (c) 2014 PierreGuillot. All rights reserved.
//

#include "../Kiwi.h"
#include <time.h>
#include "muParser.h"

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
            /*
            dico->set(Tag::from, {4, 0});
            dico->set(Tag::to, {1, 0});
            if(page->createConnection(dico))
                cout << "aki";
            
            vector<sBox> boxes;
            page->getBoxes(boxes);
            if(boxes.size())
            {
                sBox box = boxes[0];
                box->receive(1, {1});
                box->receive(0, {2});
            }
            page->write(dico);
            dico->write("test.kiwi", "/Users/Pierre/Desktop");
            */
            sBox nbox;
            dico->clear();
            dico->read("expr rint( i1 ) + rint( i2 )");
            nbox = page->createBox(dico);
            nbox->receive(1, {2.3});
            nbox->receive(0, {3});
            
            dico->clear();
            dico->read("expr i1 + 2");
            nbox = page->createBox(dico);
            nbox->receive(0, {3.2});
            /*
            double fVal = 3.8;
            mu::Parser p;
            p.DefineVar("a", &fVal);
            try
            {
                p.SetExpr("1 - rint(a-0.5)");
                cout << p.GetExpr()<< "\n";
            }
            catch(mu::ParserError &exc)
            {
                ;
            }
            
            std::cout << p.Eval() << std::endl;
             */
            cout << nbox.get() << endl;
            nbox.reset();
            cout << nbox.get() << endl;
            nbox->receive(0, {});
        }
        
    }
    kiwi = Instance::create();
    
    return 0;
}