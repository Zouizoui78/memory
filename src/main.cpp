#include "Logger.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <date/date.h>

int main(int argc, char*argv[])
{
    Renderer r;
    if(!r.init())
    {
        return -1;
    }
    
    return 0;
}