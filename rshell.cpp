#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
using namespace std;
using namespace boost;

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

        if( a[0] == "exit" )
        {
            return 0;
        }
        else 
        {
            cout << "$ ";
            command(argv);
        }
    }
}
