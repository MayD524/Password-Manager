/*
    Author : May Draskovics
    Date   : 9/5/2021
*/

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "may_cipher.h"
#include "UCPPL/UCPPL.h"

using namespace std;

class passmgr
{
private:
    struct site_data {
        string uName;
        string uPass;
        int lineNo;
    };

    // private class variables
    vector<string> _fileData = {};
    vector<string> _delc_sites = {};
    map<string, site_data> _env;
    mayCipher _cipher = mayCipher();
    UCPPL_ENV _config = UCPPL_ENV("config/config.may");
    string _DELIM = "[DELIM]";
    string _envFname;
    bool running = true;
    int _fileLine = 0;

    // for switch case later
    enum strEnum {
        eAdd,
        eList,
        eGet,
        eTermClear,
        eRemove,
        eHelp,
        eExit,
        eUnknown
    };

    map<string, strEnum> _hashitMap = {
        {"add"  , eAdd},
        {"list" , eList},
        {"get"  , eGet},
        {"clear", eTermClear},
        {"rem"  , eRemove},
        {"help" , eHelp},
        {"exit" , eExit},
    };
    
    // Turn a string to an enum value (for switching in _runCommands)
    strEnum strToEnum( string oper )
    {
        map<string, strEnum>::iterator it = this->_hashitMap.find(oper);

        if (it == this->_hashitMap.end()) { return eUnknown; }
        return it->second; 
    }

    // env functions
    bool _keyExists(string key)
    {
        map<string, site_data>::iterator it = this->_env.find(key);
        if (it == this->_env.end()) { return false; }
        return true;
    }

    void _envGetByKey(string key, site_data& ret)
    {
        if (this->_keyExists(key))
            ret = this->_env.find(key)->second;
        
    }

    int _insertENV(string key, site_data val)
    {
        map<string, site_data>::iterator it = this->_env.begin();
        try
        {
            this->_env.insert(it, pair<string,site_data>(key, val));
            return 0;
        }
        catch(const exception& e)
        {
            cerr << e.what() << '\n';
            return 1;
        }
        
    }

    // functions
    void _listSites( void )
    {
        for ( const string& site : this->_delc_sites )
        {
            cout << site << endl;
        }
    }

    // get a sites data
    void _getSite( string site = "[GET_UINPUT]" )
    {
        /*
            @params
                site -> the site we want to get the data for 
                [GET_UINPUT] just means well get it through a second step
        */
        if (site == "[GET_UINPUT]")
            site = uInput("Site> ");
        site_data dt;
        if (this->_keyExists(site))
        {
            this->_envGetByKey(site, dt);
            cout << "Name: " << dt.uName << endl;
            cout << "Password: " << this->_cipher.decrypt(dt.uPass) << endl;
        }
        else
        {
            cout << "The site " << site << "does not exist" << endl;
        }
    }

    void _removeCred( void )
    {
        string site = uInput("Remove> ");

        if (this->_keyExists(site))
        {
            // get some data so that we can clear
            site_data tmp;
            
            this->_envGetByKey(site, tmp);
            int lineNo = tmp.lineNo;

            // delete everything
            removeByIndex(this->_fileData, lineNo);
            this->_env.erase(site);

            // write
            writeFile(this->_envFname, this->_fileData, true);
        }
        else
        {
            cout << "site doesn't exist" << endl;
        }
    }

    void _addCred( void )
    {
        // get inputs
        string _credL = "";
        string site = uInput("Site> ");
        string uName = uInput("User name> ");
        string uPass = getPass("Password> ");
        
        // encrypt password
        uPass = this->_cipher.encrypt(uPass);
        _credL += site + this->_DELIM + uName + this->_DELIM + uPass;
        this->_delc_sites.push_back(site);
        
        // make new site 
        site_data new_site;
        new_site.uName = uName;
        new_site.uPass = uPass;
        new_site.lineNo = this->_fileLine;

        // write to env and file
        this->_fileLine++;
        this->_fileData.push_back(_credL);
        this->_insertENV(site, new_site);

        writeFile(this->_envFname, this->_fileData, true);
    }

public:
    int exitCode = 0;

    passmgr( string _envFileName, string _key )
    {
        this->_cipher.set_key(_key);
        this->_fileData    = readFile(_envFileName);
        this->_envFname    = _envFileName;
        this->_initEnv();
    }

    void _initEnv( void )
    {
        if (this->_fileData.size() <= 0) { return; }

        for (auto &line : this->_fileData)
        {
            vector<string> _tmpData = split(line, this->_DELIM);
            site_data site;
            
            this->_delc_sites.push_back(_tmpData[0]);
            site.uName = _tmpData[1];
            site.uPass = _tmpData[2];
            
            int pass_fail = this->_insertENV(_tmpData[0], site);

            if (pass_fail != 0) { _exit(pass_fail); return; }
        }
    }

    void _displayHelp( void )
    {
        cout << "[add] Add a new login\n[list] List all account types and names\n[rem] Remove a site\n[get] Get user name and password for a site\n[clear] clears the terminal\n[help] Displays help\n[exit] exits the program" << endl;
    }

    void _runCommands( string cmd )
    {
        switch (strToEnum(cmd))
        {
            case eAdd:
            {
                this->_addCred();
                break;
            }

            case eList:
            {
                this->_listSites();
                break;
            }

            case eRemove:
            {
                this->_removeCred();
                break;
            }

            case eGet:
            {
                this->_getSite();
                break;
            }

            case eTermClear:
            {
                termClear();
                break;
            }

            case eHelp:
            {
                this->_displayHelp();
                break;
            }

            case eExit:
            {
                this->_exit(0);
                return;
            }

            case eUnknown: 
            { 
                if (this->_keyExists(cmd))
                    this->_getSite(cmd);
                else
                    cout << "Unknown command: " << cmd << endl;
                break; 
            }
        }

    }

    void _pmgr_Main( void )
    {
        this->_displayHelp();
        while (this->running)
        {
            string userInp = toLower(uInput());
            this->_runCommands(userInp);
        }
    }

    void _exit( int _exitValue )
    {
        this->exitCode = _exitValue;
        cout << "Exiting with code " << this->exitCode << endl;
        exit(this->exitCode);
    }
};

int main( void )
{
    string _fname      = "may_passmgr.mpmgr";
    string _cfgName    = "config/config.may";
    string _passPhrase = "this is temp";
    passmgr _pmgr(_fname, _passPhrase);
    _pmgr._pmgr_Main();
}