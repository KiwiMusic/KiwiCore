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
    shared_ptr<Instance> kiwi = Instance::create();
    {
        kiwi->post("---------");
        shared_ptr<Page> page = kiwi->createPage("test.kiwi", "/Users/Pierre/Desktop");
        {
            sBox obj1 = page->createBox("+");
            obj1 = page->createBox("+");
            obj1 = page->createBox("+");
            sBox obj2 = page->createBox("+ 2. @bgcolor 0.1 2. 1 2");
            page->connect(obj1, 0, obj2, 0);
            page->write();
            
            sTag ftf = kiwi->createTag("tttt");
            sDico tata = kiwi->createDico();
            tata->append(kiwi->createTag("zaza"), {1, 1.2, 360, kiwi->createTag("zaza~ \n \""), 12});
            sDico zozo = kiwi->createDico();
            zozo->append(ftf, 1);
            zozo->append(kiwi->createTag("jjjj"), 2);
            zozo->append(kiwi->createTag("llll"), 3);
            sDico zizi = kiwi->createDico();
            zizi->append(ftf, kiwi->createTag("mm"));
            zizi->append(kiwi->createTag("jjjj"), kiwi->createTag("pp"));
            zizi->append(kiwi->createTag("llll"), kiwi->createTag("ff"));
            zozo->append(kiwi->createTag("oooo"), zizi);
            tata->append(kiwi->createTag("z°oz,,o"), {zozo, 1.2, 360, kiwi->createTag("zaza"), 12});
            tata->write("Testt.kiwi", "/Users/Pierre/Desktop");
            
            sDico tre = kiwi->createDico();
            tre->read("Testt.kiwi", "/Users/Pierre/Desktop");
            tre->write("Testt2.kiwi", "/Users/Pierre/Desktop");
            
            cout << stod("2.3e-3") << " " << stod("1.2.2") << "\n";
            string nana("zaza~ \n \\\"\" 23 zazazazazaz");
            cout << tre->jsonUnescape(nana) << "\n";
        }
    }
    return 0;
}