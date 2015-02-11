//
//  main.cpp
//  example
//
//  Created by Guillot Pierre on 01/02/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include <iostream>

#include "../../KiwiBase/Core.h"
#include "../../KiwiBoxes/Gui.h"

using namespace Kiwi;

int main(int argc, const char * argv[])
{
    sObject nat;
    sInstance instance = Instance::create();
    if(instance)
    {
        cout << "Instance created" << endl;
        sPage page = instance->createPage(Dico::create());
        if(page)
        {
            cout << "Page created" << endl;
            page->add(Dico::evaluateForBox("bang 12 23 @size 48 56"));
            page->add(Dico::evaluateForBox("bang 12 23 @size 48 56"));
            page->add(Dico::evaluateForBox("bang 12 23 @size 48 56"));
            page->add(Dico::evaluateForLink("1 0 2 0"));
            page->add(Dico::evaluateForLink("2 0 3 0"));
            
            vector<sObject> objs;
            page->getObjects(objs);
            cout << "Number of objects : "<< objs.size() << endl << endl;
            for(vector<sObject>::size_type i = 0; i < objs.size() && i < 1; i++)
            {
                cout << "coun : " << objs[i].use_count() << endl;
                cout << "name : " << toString(objs[i]->getName()) << endl;
                cout << "size : " << objs[i]->getSize().width() << " "<< objs[i]->getSize().height() << endl;
                objs[i]->receive(0, {Tag::List::bang});
                vector<sAttr> attrs;
                objs[i]->getAttrs(attrs);
                for(int i = 0; i < attrs.size(); i++)
                {
                    if(attrs[i]->isType<Color>())
                    {
                        sAttrColor color = attrs[i]->getShared<Color>();
                        color->setValue(Color(0.1, 0.2, 0.3, 0.4));
                    }
                }
            }
            nat = objs[0];
            sDico dic = Dico::create();
            page->write(dic);
            dic->write("kiwi_test.kiwi", "/Users/Pierre/Desktop/");
            
        }
    }
    instance.reset();
    
    cout << "end" << endl;
    while(nat.use_count() > 1)
    {
        ;
    }
    cout << "coun : " << nat.use_count() << endl;
    return 0;
}
