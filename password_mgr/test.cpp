
#include <iostream>
#include <string>

#include "may_cipher.h"

using namespace std;

int main ( void )
{
    const string key = "test string for key";
    string msg = "hello world!";
    mayCipher mc;
    mc.set_key(key);
    string enc = mc.encrypt(msg);
    cout << enc << endl;
    cout << mc.decrypt(enc) << endl;
}