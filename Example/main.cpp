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

mutex coutmutex;

class IListener : public Instance::Listener
{
public:
    IListener(){};
    ~IListener(){};
    
    //! Receive the notification that a page has been created.
    /** The function is called by the instance when a page has been created.
     @param instance    The instance.
     @param page        The page.
     */
    void pageHasBeenCreated(sInstance instance, sPage page) override
    {
        lock_guard<mutex> guard(coutmutex);
        cout << "Page created : "<< (long)page.get() << endl;
    };
    
    //! Receive the notification that a page has been closed.
    /** The function is called by the instance when a page has been closed.
     @param instance    The instance.
     @param page        The page.
     */
    void pageHasBeenRemoved(sInstance instance, sPage page) override
    {
        lock_guard<mutex> guard(coutmutex);
        cout << "Page deleted : "<< (long)page.get() << endl;
    }
    
    //! Receive the notification that the dsp has been started.
    /** The function is called by the instance when the dsp has been started.
     @param instance    The instance.
     */
    void dspHasBeenStarted(sInstance instance) override {};
    
    //! Receive the notification that the dsp has been stopped.
    /** The function is called by the instance when the dsp has been stopped.
     @param instance    The instance.
     */
    void dspHasBeenStopped(sInstance instance) override {};
    
};

void createPageFomInstance(sInstance instance, int i)
{
    //cout << "Index : " << i << endl;
    instance->createPage();
}

void deletePageFomInstance(sInstance instance, sPage page, int i)
{
    //cout << "Index : " << i << endl;
    instance->removePage(page);
}

void tickDspFromInstance(sInstance instance, int i)
{
    for(int z = 0; z < i; z++)
    {
        instance->tickDsp();
        lock_guard<mutex> guard(coutmutex);
        cout << "dsp : " << z << endl;
    }
}

void bindListFomInstance(sInstance instance, shared_ptr<Instance::Listener> list)
{
    instance->bind(list);
    lock_guard<mutex> guard(coutmutex);
    cout << "bind " << endl;
}

void unbindListFomInstance(sInstance instance, shared_ptr<Instance::Listener> list)
{
    instance->bind(list);
    lock_guard<mutex> guard(coutmutex);
    cout << "unbind : " << endl;
}

//==============================================================================
int main (int argc, char* argv[])
{
    shared_ptr<IListener> lis =  make_shared<IListener>();

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
            /*
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
            kiwi->removePage(page);
             */
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
            
            /*
            const int nthread  = 6;
            const int nliste  = 4;
            vector<shared_ptr<Instance::Listener>> liste;
            for(int i = 0; i < nliste; i++)
            {
                liste.push_back(make_shared<Instance::Listener>());
            }
            
            kiwi->startDsp(44100, 1024);
            thread tt(tickDspFromInstance, kiwi, 50);
            
            kiwi->bind(lis);
            thread t[nthread];
            thread t2[nliste];
            for(int i = 0; i < nthread; ++i)
            {
                t[i] = thread(createPageFomInstance, kiwi, i);
            }
            
            for(int i = 0; i < nliste; ++i)
            {
                t2[i] = thread(bindListFomInstance, kiwi, liste[i]);
            }
            
            for (int i = 0; i < nthread; ++i)
            {
                t[i].join();
            }
            
            for(int i = 0; i < nliste; ++i)
            {
                t2[i].join();
            }
            
            vector<sPage> pages;
            kiwi->getPages(pages);
            cout << endl;
            {
                lock_guard<mutex> guard(coutmutex);
                cout << "npages " << pages.size() << endl;
                for(int i = 0; i < pages.size(); ++i)
                {
                    cout << "Page : "<< (long)pages[i].get() << endl;
                }
            }
            
            for(int i = 0; i < nliste; ++i)
            {
                t2[i] = thread(unbindListFomInstance, kiwi, liste[i]);
            }
            
            for(int i = 0; i < nthread; ++i)
            {
                t[i] = thread(deletePageFomInstance, kiwi, pages[i], i);
            }
            
            for(int i = 0; i < nliste; ++i)
            {
                t2[i].join();
            }
            
            for (int i = 0; i < nthread; ++i)
            {
                t[i].join();
            }
            
            tt.join();
             */
            
        }
    }
    return 0;
}