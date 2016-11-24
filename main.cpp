#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "NaiveBayesModel.h"
using namespace std;
/*
 * ALPHA_I 平滑系数 默认1.0f
 *
 *属性文件 feature_info 格式
 * 色泽	青绿，乌黑，浅白
   根蒂	蜷缩，稍蜷，硬挺
   敲声	浊响，沉闷，清脆
   纹理	清晰，稍糊，模糊
   脐部	凹陷，稍凹，平坦
   触感	硬滑，软黏
   密度
   含糖量
 *属性文件 feature_info 格式
 *
 * */

int main(int argc, char *argv[]) {
    //std::cout << "Hello, World!" << std::endl;
  //  char featurename[] = "/home/kxw/研一开题/data/naivebayes/feature_info";
  //  char sourcename[] = "/home/kxw/研一开题/data/naivebayes/watermelon";
  //  char testname[] = "/home/kxw/研一开题/data/naivebayes/watermelon_test";
  //  char predictResname[] = "/home/kxw/研一开题/data/naivebayes/watermelon_test_res";
    char featurename[] = "/home/kxw/研一开题/data/naivebayes/iris_feature";
    char sourcename[] = "/home/kxw/研一开题/data/naivebayes/iris_training";
    char testname[] = "/home/kxw/研一开题/data/naivebayes/iris_testing";
    char predictResname[] = "/home/kxw/研一开题/data/naivebayes/iris_test_res";
    int num_of_feature;//记录属性值个数
   //  vector<list<string>> feature_values;//用来保存各个属性以及每个属性对应的取值。
   // vector<string>  feature_names;
    NaiveBayesModel naivebayesmodel ;
   num_of_feature = naivebayesmodel.getFeatureList(featurename);//最后一个属性是分类标签。
   // cout<<"特征属性个数为："<<num_of_feature<<endl; 为了测试获取属性及属性值列表的函数
    ////下一步需要统计各个类别的元素出现频率，以及每个类别上每个属性不同取值的元素个数，对于连续属性是获取均值和方差
   // string* res = naivebayesmodel.splitByMyChar("青绿,蜷缩,浊响,清晰,凹陷,硬滑,0.697,0.460,是",",",naivebayesmodel.features.size());

   // string temp = res[naivebayesmodel.features.size()-1];
    //cout<<"label="<<temp<<endl;
  int num_of_sample = naivebayesmodel.fill_frequency(sourcename);//截止到现在，pc计算需要的Dc统计完了，所有分类的离散属性的取值的频率也统计完，分类的连续属性的均值mean计算好，以及方差var计算好
    int num_of_correct = naivebayesmodel.predictLabel(testname,predictResname,num_of_sample);
/*
 * =======================================================
Summary
-------------------------------------------------------
Correctly Classified Instances          :       3023       93.7946%
Incorrectly Classified Instances        :        200        6.2054%
Total Classified Instances              :       3223

=======================================================
 * */

    return 0;
}
