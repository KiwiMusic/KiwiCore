//
//  Events.h
//  KiwiCore
//
//  Created by Guillot Pierre on 10/11/2014.
//  Copyright (c) 2014 PierreGuillot. All rights reserved.
//

#ifndef __KiwiCore__Events__
#define __KiwiCore__Events__

#include <iostream>

namespace Kiwi
{
    class Events
    {
    public:
        enum Type
        {
            Unknown     = 0,
            Enter       = 1,
            Leave       = 2,
            Move        = 3,
            Drag        = 4,
            Down        = 5,
            Up          = 6,
            DoubleClick = 7,
            Wheel       = 8
        };
        
        enum Modifier
        {
            Nothing     = 0,
            Shift       = 1,
            Ctrl        = 2,
            Alt         = 4,
            Left        = 16,
            Right       = 32,
            Middle      = 64,
            
#ifdef __APPLE__
            Cmd          = 8,
            Popup        = Right | Ctrl,
#else
            Cmd          = Ctrl,
            Popup        = Right
#endif
        };
        
        const Type      type;
        const int       x;
        const int       y;
        const long      modifiers;
        const double    wheel;
    };
}
#endif /* defined(__KiwiCore__Events__) */
