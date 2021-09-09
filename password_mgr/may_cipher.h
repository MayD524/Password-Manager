/*
    Author      : May Draskovics
    Date        : 9/5/2021
    version     : 0.2
    Description : Stand alone header file for the may cipher
*/
#pragma once

#ifndef MAY_ENC_H
#define MAY_ENC_H

#include <iostream>
#include <string>

typedef unsigned char uchar;

using namespace std;

class mayCipher
{
private:
    const char* key;
    size_t key_size = 0;
    
    char getKeyChar(int index)
    {
        if (index > key_size)
        {
            int new_index = index - this->key_size;
            return this->getKeyChar(new_index);
        }
        else if (index < 0)
            return this->key[0];
        else
            return this->key[index];
    }

public:
    mayCipher() { }

    void set_key( string cipherKey )
    {
        if (cipherKey.size() == 0) { return; }
        this->key = cipherKey.c_str();
        this->key_size = cipherKey.size();
    }

    string encrypt(string plain_text)
    {
        const char* c_plainText = plain_text.c_str();
        string cipher_text = "";
        for (int i = 0; i < plain_text.size(); i++)
        {
            char plain_char = c_plainText[i];
            char key_char = this->getKeyChar(i);
            uchar cipher_char;

            cipher_char = plain_char + key_char;
            if (cipher_char % 2 == 0)
                cipher_char += 0x6;

            if (key_char == 32)
                cipher_char += 0x60;
            cipher_text += cipher_char;
        }

        return cipher_text;
    }

    string decrypt(string cipher_text)
    {
        const char* c_cipherText = cipher_text.c_str();
        string plain_text = "";

        for (int i = 0; i < cipher_text.size(); i++)
        {
            uchar cipher_char = c_cipherText[i];
            char key_char = this->getKeyChar(i);
            uchar plain_char;
            
            plain_char = cipher_char - key_char;
            if (cipher_char % 2 == 0)
                plain_char -= 0x6;
            
            if (key_char == 32)
                plain_char -= 0x60;
                 
            plain_text += plain_char;
        }

        return plain_text;
    }
};
#endif
