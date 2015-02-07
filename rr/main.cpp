#include <iostream>
#include<math.h>
#include<stdlib.h>
#include<string>
using namespace std;
string* parse(string s)
{
    while(s[s.length()-1]==' ')
    {
        s = s.substr(0,s.length()-1);
    }
    while(s[0]==' ')
    {
        s = s.substr(1);
    }
    string c(s);
    int spaceCount = 0;
    while(c.length()>0)
    {
        if((int)c[0]==32)
        {
            spaceCount++;
            while((int)c[0]==32)
            c = c.substr(1);
        }else
        c = c.substr(1);
    }
    c = s;
    string * outwords = new string[spaceCount+2]();
    outwords[spaceCount+1]="@";
    int currentwordindex = 0;
    int characterindex = -1;
    while(c.length()>0)
    {
        characterindex++;
        if(c[characterindex]==32)
        {
            outwords[currentwordindex] = c.substr(0,characterindex);
            c = c.substr(characterindex+1);
            currentwordindex++;
            while(c[0]==32)
            {
                c = c.substr(1);
            }
            characterindex = -1;
        }

            if(currentwordindex == spaceCount){outwords[currentwordindex] = c;return outwords;}
    }
    return outwords;
}
int main()
{
    //string* s = new string[2]();
    int i = 0;
    string s;
    getline(cin,s);
    string* words = parse(s);
    while(words[i]!="@")
    {
        cout<<words[i]<<" "<<endl;
        i++;
    }
    return 0;
}
