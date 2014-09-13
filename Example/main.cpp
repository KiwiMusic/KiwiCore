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
    clock_t t;
    shared_ptr<Instance> kiwi = Instance::create();
    {
        kiwi->post("---------");
        shared_ptr<Page> page = kiwi->createPage("test.kiwi", "/Users/Pierre/Desktop");
        {
            shared_ptr<Box> obj1 = page->createBox("+~ 1 @text +~ 1");
            shared_ptr<Box> obj2 = page->createBox("+~ 1.2");
            shared_ptr<Box> obj3 = page->createBox("+~ 1.25");
            shared_ptr<Box> obj4 = page->createBox("+~ 1");
            shared_ptr<Box> obj5 = page->createBox("+~ 1");
            shared_ptr<Box> obj6 = page->createBox("+~ 1.");
            
            page->connect(obj1, 0, obj2, 0);
            page->connect(obj2, 0, obj3, 0);
            page->connect(obj3, 0, obj4, 0);
            page->connect(obj4, 0, obj5, 0);
            page->connect(obj5, 0, obj6, 0);

            kiwi->startDsp(44100., 2048);
            t = clock();
            
            for(int i = 0; i < 44100; i += 2048)
                kiwi->tickDsp();
            
            t = clock() - t;
            double tt = ((float)t)/CLOCKS_PER_SEC;
            cout << t << " clicks (" << tt << "seconds).\n";
            
            kiwi->stopDsp();
            page->write();
        }
        kiwi->post("---------");
        
        sDico dico1 = kiwi->createDico();
        sDico dico2 = kiwi->createDico();
        sDico dico3 = kiwi->createDico();
        
        dico3->append("aa", {9, 8, 7});
        dico3->append("bb", kiwi->createTag("zozo"));
        
        dico2->append("vd", dico3);
        dico2->append("va", {3, 8, 1});
        dico2->append("vb", 1.6666);
        dico2->append("vc", {12, 9, 3, kiwi->createTag("nona"), 11});
        
        dico1->append("value1", kiwi->createTag("zaza"));
        dico1->append("value2", 1.2);
        dico1->append("value3", 1);
        dico1->append("value4", dico2);
        
        //kiwi->post("---------");
        dico1->write("zaza.kiwi", "/Users/Pierre/Desktop");
        dico1->read("zaza.kiwi", "/Users/Pierre/Desktop");
        //dico1->post();
    }
    return 0;
}