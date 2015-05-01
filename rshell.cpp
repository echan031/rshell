#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
using namespace std;
using namespace boost;

void date_output(tm* date)
{
    if(date->tm_mon == 0)
    {
        cout << " Jan";
    }
    else if(date->tm_mon == 1)
    {
        cout << " Feb";
    }
    else if(date->tm_mon == 2)
    {
        cout << " Mar";
    }
    else if(date->tm_mon == 3)
    {
        cout << " Apr";
    }
    else if(date->tm_mon == 4)
    {
        cout << " May";
    }
    else if(date->tm_mon == 5)
    {
        cout << " Jun";
    }
    else if(date->tm_mon == 6)
    {
        cout << " Jul";
    }
    else if(date->tm_mon == 7)
    {
        cout << " Aug";
    }
    else if(date->tm_mon == 8)
    {
        cout << " Sep";
    }
    else if(date->tm_mon == 9)
    {
        cout << " Oct";
    }
    else if(date->tm_mon == 10)
    {
        cout << " Nov";
    }
    else if(date->tm_mon == 11)
    {
        cout << " Dec";
    }
    cout << " " << date->tm_mday;
    cout << " " << date->tm_hour;
    cout << ":" << date->tm_min;
}

void exec_ls(char **argv, string a[])
{
    /*if(argv[1] == NULL)
    {
        cout << "Nothing passed in to argv." << endl;
        //exit(1);
        return;
    }*/
    //else
    //{
        DIR *dirp;
        if(argv[1] == NULL)
        {
            string z[1];
            z[0] = ".";
            char *b;
            b = new char[1];
            memcpy(b, z[0].c_str(), z[0].size() +1);
            argv[1] = b;
        }
        else if(argv[2] == NULL && a[1].at(0) == '-')
        {
            a[2] = a[1]; 
            string y[1];
            y[0] = ".";
            char *d;
            d = new char[1];
            memcpy(d, y[0].c_str(), y[0].size() +1);
            argv[1] = d;
        }
        if(NULL == (dirp = opendir(argv[1])))
        {
            perror("There was an error with opendir(). ");
            //exit(1);
            return;
        }
        struct dirent *filespecs;
        errno = 0;
        if(a[2] == "-a")///////////////
        {
            while(NULL != (filespecs = readdir(dirp)))
            {
                cout << filespecs->d_name << " ";
            }
        }
        else if(a[2] == "-l")
        {
            struct stat s;
            struct passwd* p;
            struct group* g;

            while(NULL != (filespecs = readdir(dirp)))
            {
                if(filespecs->d_name[0] != '.')
                {
                    stat(filespecs->d_name, &s);
                    if(s.st_mode & S_IFDIR)
                    {
                        cout << "d";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IRUSR)
                    {
                        cout << "r";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IWUSR)
                    {
                        cout << "w";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IXUSR)
                    {
                        cout << "x";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IRGRP)
                    {
                        cout << "r";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IWGRP)
                    {
                        cout << "w";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IXGRP)
                    {
                        cout << "x";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IROTH)
                    {
                        cout << "r";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IWOTH)
                    {
                        cout << "w";
                    }
                    else
                    {
                        cout << "-";
                    }
                    if(s.st_mode & S_IXOTH)
                    {
                        cout << "x";
                    }
                    else
                    {
                    cout << "-";
                    }
                    cout << " " << s.st_nlink;
                    p = getpwuid(s.st_uid);
                    g = getgrgid(s.st_gid);
                    cout << " " << p->pw_name;
                    cout << " " << g->gr_name;
                    cout << " " << s.st_size;
                    time_t t = s.st_mtime;
                    struct tm * dt;  
                    dt = localtime(&t);
                    date_output(dt);
                    cout << " " << filespecs->d_name << endl;
                }
            }
        }
        else
        {
            while(NULL != (filespecs = readdir(dirp)))
            {
                if(filespecs->d_name[0] != '.')
                {
                    cout << filespecs->d_name << " ";
                }
            }
        }
        if(errno != 0)
        {
            perror("There was an error with readdir(). ");
            exit(1);
        }
        cout << endl;
        if(-1 == closedir(dirp))
        {
            perror("There was an error with closedir(). ");
            exit(1);
        }
    //}
}

void split(string input, char **argv, string a[])
{
    int j = 0;
    char *b;
    char_separator<char> delim(" ",";#&|");
    tokenizer< char_separator<char> > mytok(input, delim);
    for(tokenizer< char_separator<char> >::iterator it=mytok.begin(); it != mytok.end(); ++it, ++j)
    {
        a[j] = *it;
        b = new char[a[j].size() + 1];
        memcpy(b, a[j].c_str(), a[j].size() + 1);
        argv[j] = b;
    }
    argv[j] = NULL;
}

void join(char **argv, string a[])
{
    char *b;
    int i;
    for(i=0; a[i] != ""; ++i)
    {
        b = new char[a[i].size() + 1];
        memcpy(b, a[i].c_str(), a[i].size()+1);
        argv[i] = b;
    }
    argv[i] = NULL;
}

int command(char **argv)
{
    int pid = fork();
    if(pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        if(execvp(argv[0], argv) == -1)
        {
            perror("execvp");
        }
        exit(1);
    }
    else if(pid > 0)
    {
        if(wait(0) == -1)
        {
            perror("wait");
        }
    }
}

void create(char **argv, int i)
{
    argv[i] = NULL;
}

int main()
{
    char *argv[100] = {0};
    string input;
    string a[100] = {""};
    bool cont = true;
    while(true)
    {
        getline(cin, input);
        split(input, argv, a);

        if(a[0] == "ls")
        {
            exec_ls(argv, a);
            for(int i = 0; i<100; ++i)
            {
                a[i] = "";
            }
        }
        else
        {


            for(int i=0; a[i] != ""; ++i)
            {
                if(a[i] == "#")
                {
                    create(argv, i);
                    break;
                }
            
                else if(a[i] == "&" && a[i+1] == "&")       //new
                {
                    create(argv, i);
                    create(argv, i+1);
                    if( a[0] == "exit")
                    {
                        return 0;
                    }
                    else
                    {
                        cout << "$ ";
                        command(argv);
                        /*if(command(argv) == 1)              //here          
                        {
                            cout << "wrong" << endl;
                            break;
                        }*/
                        cout << "AND" << endl;
                        int j;
                        for(j=0; argv[j+i+2] != NULL; ++j)
                        {
                            a[j] = a[j+i+2];                
                        }
                        a[j] = "";
                        a[j+1] = "";
                        join(argv, a);
                        i = 0;
                    }
                }

                else if(a[i] == "|" && a[i+1] == "|")       //new
                {
                    create(argv, i);
                    create(argv, i+1);
                    if( a[0] == "exit")
                    {
                        return 0;
                    }
                    else
                    {
                        cout << "$ ";
                        command(argv);
                        /*if(    )                  //here
                        {
                            break;
                        }*/
                        cout << "OR" << endl;
                        int j;
                        for(j=0; argv[j+i+2] != NULL; ++j)
                        {
                            a[j] = a[j+i+2];                
                        }
                        a[j] = "";
                        a[j+1] = "";
                        join(argv, a);
                        i = 0;
                    }
                }

                else if(a[i] == ";")
                { 
                    create(argv, i);
                    if( a[0] == "exit")
                    {
                        return 0;
                    }
                    else
                    {
                        cout << "$ ";
                        command(argv);
                        int j;
                        for(j=0; argv[j+i+1] != NULL; ++j)
                        {
                            a[j] = a[j+i+1];                
                        }
                        a[j] = "";
                        join(argv, a);
                        i = 0;
                    }
                }
            }
        }
        if( a[0] == "exit" )
        {
            return 0;
        }
        else 
        {
            cout << "$ ";
            //command(argv);
        }
    }
}
