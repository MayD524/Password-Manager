
#pragma once
#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "tools.h"

using namespace std;

/*
    env_name = env_string
*/


class UCPPL_ENV
{
public:
    UCPPL_ENV( string config_file )
    {
        this->read_config(config_file);
    }

    map<string, string> ENV;

    bool key_exists( string key )
    {
        map<string, string>::iterator it = this->ENV.find(key);
        if (it == this->ENV.end()) { return false; }
        return true;
    }

    string get_config_option( string key )
    {
        if (this->key_exists(key))
            return this->ENV.find(key) -> second;
    }

    int append_env( string key, string value )
    {
        map<string, string>::iterator it = this->ENV.begin();
        try
        {
            this->ENV.insert(it, pair<string, string>(key, value));
        }
        catch(const exception& e)
        {
            cerr << e.what() << endl;
            return 1;
        }

        return 0;
    }

    void read_config( string config_file_path )
    {
        vector<string> file_data = readFile(config_file_path);

        for (auto &line : file_data)
        {
            vector<string> splitLine = split(line, "=");
            int pass_fail = this->append_env(splitLine[0], splitLine[1]);

            if (pass_fail != 0) { cout << "Something went wrong with appending to ENV" << endl; }
        }
    }
};
#endif