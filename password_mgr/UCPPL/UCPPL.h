/*
    Author: May Draskovics
    Date: 9/8/2021
*/
#pragma once

#ifndef UCPPL_H
#define UCPPL_H

#if defined _WIN32
    #include "wintools.h"
    #if defined UCPPL_GUI
        #include "wingui.h"
    #endif
#endif

#include <iostream>
#include <vector>
#include <string>

#include "config_handler.h"
#include "tools.h"

using namespace std;

#endif