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
        cout << "Port Audio Device : Sample Rate (" << manager->getSampleRate() << ") Vector Size (" << manager->getVectorSize() << ")" << endl;
        sInstance instance = Instance::create(manager);
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
            
            sPatcher page = instance->createPatcher(Dico::create());
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
                    }                }
            }
        }
    }
    return 0;
}
