//
//  main.cpp
//  KiwiCore
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
    shared_ptr<Instance> kiwi = make_shared<Instance>();
    {
        kiwi->init();
        kiwi->post("---------");
        shared_ptr<Page> page = kiwi->createPage("zaza.kiwi", "/Users/Pierre/Desktop");
        {
            vector<Element> elements;
            elements.push_back(1);
            shared_ptr<Box> obj1 = page->createBox(kiwi->createTag("+~"), elements);
            elements[0] = 1;
            shared_ptr<Box> obj2 = page->createBox(kiwi->createTag("+~"), elements);
            elements[0] = 1.;
            shared_ptr<Box> obj3 = page->createBox(kiwi->createTag("+~"), elements);
            elements[0] = 1.;
            shared_ptr<Box> obj4 = page->createBox(kiwi->createTag("+~"), elements);
            elements[0] = 1.;
            shared_ptr<Box> obj5 = page->createBox(kiwi->createTag("+~"), elements);
            elements[0] = 1.;
            shared_ptr<Box> obj6 = page->createBox(kiwi->createTag("+~"), elements);
            
            string message = string("connect obj1 to obj2: ");
            message += to_string(page->connect(obj1, 0, obj2, 0));
            kiwi->post(message);
            
            message = string("connect obj2 to obj3: ");
            message += to_string(page->connect(obj2, 0, obj3, 0));
            kiwi->post(message);
            
            message = string("connect obj3 to obj4: ");
            message += to_string(page->connect(obj3, 0, obj4, 0));
            kiwi->post(message);
            
            message = string("connect obj4 to obj5: ");
            message += to_string(page->connect(obj4, 0, obj5, 0));
            kiwi->post(message);
            
            message = string("connect obj5 to obj6: ");
            message += to_string(page->connect(obj5, 0, obj6, 0));
            kiwi->post(message);
            
            kiwi->post("---------");
            
            kiwi->startDsp(44100., 2048);
            t = clock();
            
            for(int i = 0; i < 44100; i += 2048)
                kiwi->tickDsp();
            
            t = clock() - t;
            double tt = ((float)t)/CLOCKS_PER_SEC;
            cout << t << " clicks (" << tt << "seconds).\n";
            
            kiwi->stopDsp();
        }
        kiwi->post("---------");
        
        sDico dico1 = kiwi->createDico();
        sDico dico2 = kiwi->createDico();
        sDico dico3 = kiwi->createDico();
        vector<Element> elements;
        elements.push_back(9);
        elements.push_back(8);
        elements.push_back(7);
        
        dico1->append(kiwi->createTag("value1"), 1.2);
        dico1->append(kiwi->createTag("value2"), 1);
        dico1->append(kiwi->createTag("value3"), kiwi->createTag("zaza"));
        
        dico2->append(kiwi->createTag("va"), elements);
        dico2->append(kiwi->createTag("vb"), elements);
        dico2->append(kiwi->createTag("vc"), elements);
        dico3->append(kiwi->createTag("aa"), elements);
        dico2->append(kiwi->createTag("vd"), dico3);
        dico1->append(kiwi->createTag("value4"), dico2);
        
        dico1->write("zaza.kiwi", "/Users/Pierre/Desktop");
        dico1->read("zaza.kiwi", "/Users/Pierre/Desktop");
    }
    return 0;
}