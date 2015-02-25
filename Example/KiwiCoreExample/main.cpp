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
            
            Dico dico;
            sPatcher page = instance->createPatcher(dico);
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
