//
//  main.cpp
//  example
//
//  Created by Guillot Pierre on 01/02/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include <iostream>

#include "../../KiwiBase/Core.h"
#include "../../KiwiDsp/Implementation/DspPortAudio.h"

using namespace Kiwi;

int main(int argc, const char * argv[])
{
    sDspDeviceManager manager = make_shared<PortAudioDeviceManager>();
    
    //Atom zaza = 1;
    Atom zizi = Atom::evaluate("[1, 2, [1, 2, 5]], {\"zaza\" : 12}");
    Atom zozo({1.2, 6l, "é", 12l});
    
    
    pair<const sTag, Atom> myp1(Tag::create("zozo"), 56.8);
    pair<const sTag, Atom> myp2(Tag::create("zizi"), 56.8);
    Atom hsdj({myp1, myp2});
    Atom zouzou({pair<const sTag, Atom>(Tag::create("zozo"), 1.8), pair<const sTag, Atom>(Tag::create("zizi"), 56.8)});
    
    if(zouzou.getType() == Atom::MAP)
    {
        cout << "Map" << endl;
    }
    else
    {
        cout << zouzou.getType() << endl;
    }
    
    Atom etienne = {12, "Zozou", 56.7, 90.f, 14l, "francis"};
    
    if(etienne.getType() == Atom::VECTOR)
    {
        cout << "Vector" << endl;
    }
    else
    {
        cout << etienne.getType() << endl;
    }
    
    if(manager)
    {
        Console::device = manager;
        manager->setSampleRate(44100);
        manager->setVectorSize(64);
        cout << "Port Audio Device : Sample Rate (" << manager->getSampleRate() << ") Vector Size (" << manager->getVectorSize() << ")" << endl;
        
        while(1)
        {
            string input;
            cout << "Command : ";
            getline(cin, input);
            cout << input << endl;
            try
            {
                if(!Console::receive(input))
                {
                    return 0;
                }
            }
            catch(exception& e)
            {
                cout << "Error " << e.what() << endl;
            }
        }
        
        return 0;
        sInstance instance = Instance::create(manager, "Instance");
        if(instance)
        {
            try
            {
                instance->start();
            }
            catch (exception& e)
            {
                cout << e.what() << endl;
            }
            
            map<sTag, Atom> args;
            sPatcher page = instance->createPatcher(args);
            if(page)
            {
                try
                {
                    page->DspChain::start();
                }
                catch(exception& e)
                {
                    cout << "Error " << e.what() << endl;
                }
                
                while(1)
                {
                    vector<Atom> inputs, results;
                    string input, word, command;
                    cout << "Patcher command : ";
                    getline(cin, input);
                    
                    istringstream iss(input);
                    iss >> command;
                    
                    while(iss >> word)
                    {
                        cout << word << " ";
                    }

                    try
                    {
                        if(command == "exit")
                        {
                            return 0;
                        }
                        else if(command == "create")
                        {
                            
                        }
                        else if(command == "remove")
                        {
                            
                        }
                        else if(command == "set")
                        {
                            
                        }
                        else if(command == "get")
                        {
                            
                        }
                        else
                        {
                            Console::post("Unknown command : " + command);
                        }
                        
                    }
                    catch(exception& e)
                    {
                        cout << "Error " << e.what() << endl;
                    }
                }
            }
        }
    }
    return 0;
}
