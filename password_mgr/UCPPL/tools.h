/*
    Author: May Draskovics
    Date: 9/6/2021
*/
#pragma once

#ifndef TOOLS_H
#define TOOLS_H

#define MAX_STRING_SIZE 256

#if defined _WIN32
    #define CURRENT_OS "WIN32"
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #define CURRENT_OS "LINUX"
#elif defined(__APPLE__)
    #define CURRENT_OS "APPLE"
#endif


#include <iostream>
#include <stdarg.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <string>
#include <locale>

using namespace std;


template <typename T>
void removeByIndex(vector<T>& vec, size_t pos)
{
    typename vector<T>::iterator it = vec.begin();
    advance(it, pos);
    vec.erase(it);
}

void termClear( void )
{
    if (CURRENT_OS == "WIN32") { system("cls"); }
    else if (CURRENT_OS == "LINUX" || CURRENT_OS == "APPLE") { system("clear"); }
}

string getPass( const string prompt=">", int maxStringSize = MAX_STRING_SIZE)
{
    string pass = "";
    char ch;
    int size = 0;
    cout << prompt;

    ch = _getch();
    while (ch != 13 || size > maxStringSize)
    {
        pass.push_back(ch);
        cout << "*";
        ch = _getch();
        size++;
    }

    return pass;
}

string toLower( const string inpt)
{
    locale loc;
    string retStr = "";
    for (string::size_type i = 0; i < inpt.size(); i++)
        retStr += tolower(inpt[i], loc);

    return retStr;
}

string strToHex( const string inpt)
{
    const char* cStr = inpt.c_str();
    string retStr = "";

    for(int i = 0; i < inpt.size(); i++)
    {
        cout << cStr[i] << " " <<(int) cStr[i] << endl;
    }

    return retStr;
}

string hexToString( const string hexStr)
{
    const char* cHexStr = hexStr.c_str();
    string retStr = "";
}

string toUpper( const string inpt)
{
    locale loc;
    string retStr = "";
    for (string::size_type i = 0; i < inpt.size(); i++)
        retStr += toupper(inpt[i], loc);
    
    return retStr;
}

vector<string> split(string str, string delim)
{
    vector<string> split_str;
    string token;
    size_t pos = 0;

    while((pos = str.find(delim)) != string::npos)
    {
        token = str.substr(0, pos);
        split_str.push_back(token);
        str.erase(0, pos + delim.length());
    }
    split_str.push_back(str);

    return split_str;
}

int writeFile(string filename, vector<string> data, bool autoNewLine)
{
    ofstream _file(filename);

    if (_file.is_open())
    {
        for (string& d : data)
        {
            if (!autoNewLine)
                _file << d;
            else
                _file << d << endl;
        }

        _file.close();
        return 0;
    }

    return 1;
}

string uInput(string prompt=">")
{
    string _userinp = "";
    cout << prompt;
    getline(cin, _userinp);
    return _userinp;
}

vector<string> readFile(string Filename)
{
    vector<string> lines;
    ifstream file_to_read(Filename);

    if (file_to_read.is_open())
    {
        string line;
        while(getline(file_to_read, line))
        {
            if (line == "") { continue; }
            lines.push_back(line);
        }
        file_to_read.close();
        return lines;
    }

    return lines;
}
#endif