#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <stack>
using namespace std;


vector<string> topThree(string filename) {
    string line;
    ifstream infile (filename);
    vector<string> ret;
    map<string, unsigned> strings;
    stack<string> stack;
    if (infile.is_open()) {
        while (getline(infile, line)) {
            if(strings.find(line) == strings.end())//if the element does not
                strings[line] = 1;
            else
                strings[line]++;
        }
    }
    infile.close();
    string str1,str2,str3;
    unsigned freq1=0,freq2=0,freq3=0;
    map<string, unsigned>::iterator it;
    for(it = strings.begin(); it != strings.end(); it++){
        if(it->second > freq1){
            freq1 = it->second;
            str1 = it->first;
        }
    }
    for(it = strings.begin(); it != strings.end(); it++){
        if(it->second > freq2 && it->second < freq1){
            freq2 = it->second;
            str2 = it->first;
        }
    }
    for(it = strings.begin(); it != strings.end(); it++){
        if(it->second > freq3 && it->second < freq2){
            freq3 = it->second;
            str3 = it->first;
        }
    }
    ret.push_back(str1);
    ret.push_back(str2);
    ret.push_back(str3);
    return ret;
}
