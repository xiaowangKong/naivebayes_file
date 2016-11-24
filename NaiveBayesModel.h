//
// Created by kxw on 11/21/16.
//
#ifndef  NAIVEBAYESMODEL
#define  NAIVEBAYESMODEL



#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#define pi 3.1415926
using namespace std;
 struct feature{
     string name;
    vector<string> values;
     bool continuous;
};
class NaiveBayesModel {
public:
~NaiveBayesModel(){//析构函数
    for( vector<struct feature*>::iterator iter = features.begin(); iter != features.end(); iter++ )
    {
        delete *iter;
    }
}

    vector<struct  feature*> features;
    map<string,double> pc;
    map<string,vector<map<string,double >>> model;
   int getFeatureList(char * fname);//获取属性值及属性列表
    int fill_frequency(char* fname);//生成频数表并根据参数指定的输入文件进行填充。
    string* splitByMyChar(string line,string mychar,int dimension);//将一个字符串以某个字符分割后返回数组
    int  predictLabel(char* sourcename,char * outfile,int N);

};

#endif