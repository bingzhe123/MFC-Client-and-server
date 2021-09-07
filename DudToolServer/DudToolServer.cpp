// DudToolServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include "HttpsServer.h"
int main()
{
    HttpsServer httServer;
    httServer.StartServer();
    return 0;
}         