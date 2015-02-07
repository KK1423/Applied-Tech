#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    string line;
    ifstream myfile("Gyazo.txt");
    int n = 0;
    string num = " ";
    if(myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //num = to_string(n);
            line = "wget.exe "+line+" 2>results.txt";
            system(line.c_str());
            ofstream outfile("results1.txt",ios::app);
            string outline;
            ifstream filein("results.txt");
            while(getline(filein,outline))
            {
                outfile<<outline<<endl;
            };
            outfile.close();
            filein.close();
            //return 0;
            //cout <<"succesfully downloaded"<< line << endl;;
            n++;
        }
        myfile.close();
    }
    else
    {
        cout<<"fail"<<endl;
    }
    cout << "Hello world!" << endl;
    return 0;
}
