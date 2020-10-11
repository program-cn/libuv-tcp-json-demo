#include "server.h"

#include <unistd.h>

int main()
{
    uv::UvServer uvServer("127.0.0.1", 5550);

    while(true)
    {
        sleep(1);
    } 

    return true;
}
