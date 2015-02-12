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
    /*
    vector<Atom> vec;
    vec.push_back(1);
    vec.push_back(2.2);
    vec.push_back(Tag::create("zaza"));
    vec.push_back(Dico::evaluateForObject("bang 12 14 @size 56 73"));
    
    cout << "vec size is : " << vec.size() << endl;
    for(vector<Atom>::size_type i = 0; i < vec.size(); i++)
    {
        cout << "vec["<< i <<"] is number : " << toString(vec[i].isNumber()) << endl;
        cout << "vec["<< i <<"] is long : " << toString(vec[i].isLong()) << endl;
        cout << "vec["<< i <<"] is double : " << toString(vec[i].isDouble()) << endl;
        cout << "vec["<< i <<"] is tag : " << toString(vec[i].isTag()) << endl;
        cout << "vec["<< i <<"] is object : " << toString(vec[i].isObject()) << endl;
        cout << "vec["<< i <<"] is dico : " << toString(vec[i].isDico()) << endl;
        
        if(vec[i].isLong())
            cout << "vec["<< i <<"] in long : " << toString((long)vec[i]) << endl;
        else if(vec[i].isDouble())
            cout << "vec["<< i <<"] in double : " << toString((double)vec[i]) << endl;
        else if(vec[i].isTag())
            cout << "vec["<< i <<"] in tag : " << toString((sTag)vec[i]) << endl;
        else if(vec[i].isObject())
            cout << "vec["<< i <<"] in object : " << toString((scObject)vec[i]) << endl;
        else if(vec[i].isDico())
            cout << "vec["<< i <<"] in dico : " << toString((scDico)vec[i]) << endl;
        cout << endl;
    }
    
    return 0;
    */
    
    sObject nat;
    sInstance instance = Instance::create();
    if(instance)
    {
        cout << "Instance created" << endl;
        sPatcher page = instance->createPatcher(Dico::create());
        if(page)
        {
            cout << "Page created" << endl;
            page->add(Dico::evaluateForObject("bang 12 23 @position 10 10 @size 48 56"));
            page->add(Dico::evaluateForObject("bang 12 23 @position 10 10 @size 48 56"));
            page->add(Dico::evaluateForObject("bang 12 23 @position 10 10 @size 48 56"));
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
                cout << "posi : " << objs[i]->getPosition().x() << " "<< objs[i]->getPosition().y() << endl;
                objs[i]->receive(0, {Tag::List::bang, "zaza", 12.000, 4, objs[i], "stevie"});
                vector<sAttr> attrs;
                objs[i]->getAttrs(attrs);
                for(int i = 0; i < attrs.size(); i++)
                {
                    if(attrs[i]->isType<ColorValue>())
                    {
                        sAttrColor color = attrs[i]->getShared<ColorValue>();
                        Color col = color->getValue();
                        color->setValue(Color(0.1, 0.2, 0.3, 0.4));
                    }
                    if(attrs[i]->isType<PointValue>())
                    {
                        sAttrPoint point = attrs[i]->getShared<PointValue>();
                        Kiwi::Point pt = point->getValue();
                        point->setValue(Kiwi::Point(128., 19.));
                    }
                    if(attrs[i]->isType<BoolValue>())
                    {
                        sAttrBool point = attrs[i]->getShared<BoolValue>();
                        bool pt = point->getValue();
                        point->setValue(!pt);
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
