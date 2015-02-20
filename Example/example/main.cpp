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
#include "../../KiwiDsp/Implementation/DspPortAudio.h"

using namespace Kiwi;

int main(int argc, const char * argv[])
{
    shared_ptr<PortAudioDeviceManager> manager = make_shared<PortAudioDeviceManager>();
    if(manager)
    {
        manager->setSampleRate(44100);
        manager->setVectorSize(64);
 
        sInstance instance = Instance::create(manager);
        if(instance)
        {
            cout << "Instance created" << endl;
            try
            {
                instance->start();
            }
            catch (exception& e)
            {
                cout << e.what() << endl;
            }
            
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
                    cout << "size : " << objs[i]->getSize().width() << " " << objs[i]->getSize().height() << " " << objs[i]->getSize().ratio() <<  endl;
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
                            pt = point->getValue();
                            cout << "new pos "<< pt.x() << " " << pt.y() << endl;
                        }
                        if(attrs[i]->isType<SizeValue>())
                        {
                            sAttrSize point = attrs[i]->getShared<SizeValue>();
                        }
                        if(attrs[i]->isType<BoolValue>())
                        {
                            sAttrBool point = attrs[i]->getShared<BoolValue>();
                            bool pt = point->getValue();
                            point->setValue(!pt);
                        }
                    }
                }
 
                sDico dic = Dico::create();
                page->write(dic);
                dic->write("kiwi_test.kiwi", "/Users/Pierre/Desktop/");
                
            }
        }
    }
    return 0;
}
