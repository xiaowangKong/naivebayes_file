//
// Created by kxw on 11/21/16.
//
#ifndef  NAIVEBAYES_FILE_NAIVEBAYESMODEL_H      //表示在组合使用include多个.h头文件时，file3.h未在之前被include过。
#include "NaiveBayesModel.h"
#endif
/*
 *
 *  vector<struct  feature*> features;
    vector<double> pc;
    map<string,vector<map<string,double >>> model;
 *
 *
 * */
string* NaiveBayesModel::splitByMyChar(string line,string mychar,int dimension){

    string::size_type index = line.find_first_of(mychar,0);//找到空格、回车、换行等空白符
    if(index<line.size()){
        string *res = new string[dimension];//先new一个结果string数组
       int i=0;
        while (index != string::npos) {
            //      cout<<"放入list元素是"<<line.substr(0,index)<<endl;
            res[i]=line.substr(0,index);
          //s  cout<<res[i]<<endl;
            //  feature_value.push_back(line.substr(0, index));
            line = line.substr(index + 1);
            //     cout<<"去掉一个dest_id剩余子串是："<<line<<endl;
            index = line.find_first_of(mychar, 0);
            //    cout<<"空格键下标是"<<index<<endl;
            ++i;
        }
        res[i]=line;
        //cout<<res[i]<<endl;
        return  res;
    }
    else{//给定字符串line中没有要分割的字符mychar,返回长度为1的源字符串
        string *res = new string[1];//先new一个结果string数组
        res[0]=line;
        return res;
    }
}

int NaiveBayesModel:: getFeatureList(char * fname){
    ifstream source_in;
    // ofstream source_out;
    source_in.open(fname, ios::in);//首先打开属性描SS述文件
    if(!source_in){
        cout<<"文件"<<fname<<"不能打开！"<<endl;
        exit(0);
    }
    string line;
    int count=0;
    // char pg_message[256]="/home/kxw/研一开题/data/pagerank/pg_message";
  getline(source_in,line);//以文件句柄读取一行
    while(!source_in.eof()){
        count++;//记录文件里有多少条记录
        string::size_type index = line.find_first_of("\t",0);//找到空格、回车、换行等空白符
        string feature_name= line.substr(0,index);
       struct feature * current_feature ;//定义结构体指针
        //current_feature=(struct feature *)malloc(sizeof(struct feature));//为结构体指针分配空间
        current_feature = new feature;
         current_feature->name=feature_name;
       // feature_names.push_back(feature_name);//将属性名字分别放入vector features中
        //  cout<<"tab键下标是"<<index<<endl;
        // cout<<"source_id = "<<source_id.length()<<endl;

        if(index<=line.size()) {
            line = line.substr(index+1);
            //  cout<<"去掉source_id剩余子串是："<<line<<endl;
            index = line.find_first_of(",", 0);
            //  cout<<"空格键下标是"<<index<<endl;

            while (index != string::npos) {
                //      cout<<"放入list元素是"<<line.substr(0,index)<<endl;
                current_feature->values.push_back(line.substr(0, index));
              //  feature_value.push_back(line.substr(0, index));
                line = line.substr(index + 1);
                //     cout<<"去掉一个dest_id剩余子串是："<<line<<endl;
                index = line.find_first_of(",", 0);
                //    cout<<"空格键下标是"<<index<<endl;
            }
            current_feature->values.push_back(line.substr(0, index));
            current_feature->continuous = false;
        }//否则说明是连续的，values没有赋值
        else{
            current_feature->continuous = true;//说明是连续的
        }
        features.push_back(current_feature);
   getline(source_in,line);//以文件句柄读取一行
    }
    source_in.close();
    return count;
}

int NaiveBayesModel::fill_frequency(char *fname) {///返回源文件包含的元素个数，也就是总数N
    ////首先初始化/////
    vector<int>   continuous_feature_id;
    for (int i = 0; i < features[features.size()-1]->values.size(); ++i) {//features最后一个维度存类标签
        // map<string,double> pc
        string c = features[features.size()-1]->values[i];
        pc.insert(pair<string,double>(c,0.0));
        vector<map<string,double >> c_features;

        for(int j=0;j<features.size()-1;j++){//属性j对应下标,不包括最后一个标签属性
            if(features[j]->continuous) {//说明是连续属性
                if(i==0){
                continuous_feature_id.push_back(j);}//将连续属性的下标，也就是第几个属性记录在continuous_feature_id
                map<string, double> c_features_values;
                c_features_values.insert(pair<string,double >("mean",0.0));//一个存mean，
                c_features_values.insert(pair<string,double >("var",0.0));//一个存方差
                c_features.push_back(c_features_values);//追加入c_features
            }else{//说明是离散的

                map<string, double> c_features_values;
                for (int k = 0; k < features[j]->values.size(); k++) {
                    c_features_values.insert(pair<string, double>(features[j]->values[k], 0.0));//生成<属性值，0.0>
                }
                //c_features[j] = c_features_values;
                c_features.push_back(c_features_values);
            }
        }
        model.insert(pair<string,vector<map<string,double >>>(c,c_features));//将每个类标签下的属性向量初始化，每个属性的每个属性值的频数初始化为0.0
    }
    ////首先初始化////
    ifstream source_in;//用来读源文件
   // ofstream source_out;//将文件根据分类分成几个文件
    source_in.open(fname, ios::in);//首先打开属性描SS述文件
    if(!source_in){
        cout<<"文件"<<fname<<"不能打开！"<<endl;
        exit(0);
    }
    string line;
    int count=0;
    // char pg_message[256]="/home/kxw/研一开题/data/pagerank/pg_message";
    getline(source_in,line);//以文件句柄读取一行
    while(!source_in.eof()){
        count++;//记录文件里有多少条记录
       string * res=splitByMyChar(line,",",features.size());///根据features记录下的属性个数，包括标签，将源数据的每一行都分解成单个的属性数组，分割符号是“，”，有可能会变哦，要注意！
        string temp = res[features.size()-1];//现在存的是类标签
        pc[temp]=pc.at(temp)+1.0;//res最后一个维度是标签，在map里根据这个标签作为key返回value增1后再写回。类包含样本数增1，
        //map<string,vector<map<string,double >>> model;//

        for (int i = 0; i < features.size()-1; ++i) {//对于当前样本的每一个属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值
            if(features[i]->continuous) {//连续属性统计均值，实际上现在是求和sum，求和后除以个数就是均值了
                model.at(temp)[i]["mean"] = model.at(temp)[i].at("mean") +atof(res[i].c_str());//double value = atof(str.c_str());
            }else{//离散值
                model.at(temp)[i][res[i]] = model.at(temp)[i].at(res[i]) + 1.0;
                //类对应的属性|i|的res[i]属性值对应频率增1
            }
        }//features.push_back(current_feature);
        getline(source_in,line);//以文件句柄读取一行
        }//否则说明是连续的，values没有赋值
    source_in.close();//此时应该是离散属性的频率统计和连续属性的mean求和项已经结束

     ////////////////////////////////////////////////////////计算mean//////////////////////////////////////////////
    for(int l = 0;l<features[features.size()-1]->values.size();l++)
    { string temp = features[features.size()-1]->values[l];
        for (int i = 0; i < continuous_feature_id.size(); ++i) {//对于当前样本的每一个连续属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值
            int current_continuous_id = continuous_feature_id[i];
            double mean = model.at(temp)[current_continuous_id].at("mean");
            double n = pc.at(temp);//获取均值
            model.at(temp)[current_continuous_id]["mean"]=mean/n;

        }//features.push_back(current_feature);
    }
    ////////////////////////////////////////////////////////计算mean//////////////////////////////////////////////

    ///////////////////////////////////////////////////////////下面开始统计方差求和项/////////////////////////////////
    source_in.open(fname, ios::in);//首先打开属性描SS述文件
    if(!source_in){
        cout<<"文件"<<fname<<"不能打开！"<<endl;
        exit(0);
    }
    getline(source_in,line);//以文件句柄读取一行
    while(!source_in.eof()){
        string * res=splitByMyChar(line,",",features.size());///根据features记录下的属性个数，包括标签，将源数据的每一行都分解成单个的属性数组，分割符号是“，”，有可能会变哦，要注意！
        string temp = res[features.size()-1];//现在存的是类标签
        //pc[temp]=pc.at(temp)+1.0;//res最后一个维度是标签，在map里根据这个标签作为key返回value增1后再写回。类包含样本数增1，
        //map<string,vector<map<string,double >>> model;//

        for (int i = 0; i < continuous_feature_id.size(); ++i) {//对于当前样本的每一个连续属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值
                int current_continuous_id = continuous_feature_id[i];
                double mean = model.at(temp)[current_continuous_id].at("mean");//获取均值
                double current_feature_value_subtract_mean = atof(res[current_continuous_id].c_str())-mean;
                model.at(temp)[current_continuous_id]["var"] = model.at(temp)[current_continuous_id].at("var") +current_feature_value_subtract_mean*current_feature_value_subtract_mean;//double value = atof(str.c_str());
        }//features.push_back(current_feature);
        getline(source_in,line);//以文件句柄读取一行
    }//否则说明是连续的，values没有赋值
    source_in.close();//此时应该是离散属性的频率统计和连续属性的mean求和项和方差求和项已经结束

    ///////////////////////////////////////////////////////计算var//////////////////////////////////////////////

    for(int l = 0;l<features[features.size()-1]->values.size();l++)//对于每一个类别label，
    { string temp = features[features.size()-1]->values[l];
        for (int i = 0; i < continuous_feature_id.size(); ++i) {//对于当前样本的每一个连续属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值
            int current_continuous_id = continuous_feature_id[i];
            double var = model.at(temp)[current_continuous_id].at("var");
            double n = pc.at(temp);//获取均值
            model.at(temp)[current_continuous_id]["var"]=var/n;

        }//features.push_back(current_feature);
    }
    ///////////////////////////////////////////////////////计算var//////////////////////////////////////////////
    return count;
    }

int NaiveBayesModel::predictLabel(char *sourcename, char *outfile,int N) {
    ifstream source_in;//用来读源文件
    ofstream source_out;//将文件根据分类分成几个文件
    source_in.open(sourcename, ios::in);//首先打开属性描SS述文件
   // source_out.open(outfile);
   // if(source_out){//判断输出文件是不是存在，存在就删除
   //     if(remove(outfile))  cout<<"删除文件source_out失败！"<<endl;
   // }
   // else source_out.close();
    if (!source_in) {
        cout << "文件" << sourcename << "不能打开！" << endl;
        exit(0);
    }
    string line;
    int count_correct = 0;
    int count = 0;
    // char pg_message[256]="/home/kxw/研一开题/data/pagerank/pg_message";
    getline(source_in, line);//以文件句柄读取一行
    while (!source_in.eof()) {
        count++;
        string *res = splitByMyChar(line, ",",
                                    features.size());///根据features记录下的属性个数，包括标签，将源数据的每一行都分解成单个的属性数组，分割符号是“，”，有可能会变哦，要注意！
        //map<string,vector<map<string,double >>> model;//
        double  max_logp = 0.0;

        string c = features[features.size() - 1]->values[0];//当前类别c
        string max_c =c;
        double Dc = pc.at(c);//取出当前类别所含有的样本个数
        for (int i = 0; i < features.size() - 1; ++i) {//对于当前样本的每一个属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值

            if (features[i]->continuous) {//连续属性统计均值，实际上现在是求和sum，求和后除以个数就是均值了
                double  current_mean = model.at(c)[i].at("mean");
                double  current_var = model.at(c)[i].at("var");
               max_logp+=log( exp ((-0.5)*(atof(res[i].c_str())-current_mean)*(atof(res[i].c_str())-current_mean)/current_var)/sqrt(2*pi*current_var));
            } else {//离散值求logPc=log（）
                max_logp+=log((model.at(c)[i].at(res[i])+1.0)/(Dc+features[i]->values.size()));//取出当前待预测样本的在类别c中属性i上的取值为res[i]的样本频率
                //类对应的属性|i|的res[i]属性值对应频率增1
            }
        }//features.push_back(current_feature);
        max_logp+=log((Dc+1.0)/(N+features[features.size() - 1]->values.size()));//计算pc

        for (int l = 1; l < features[features.size() - 1]->values.size(); l++) {//对于每一个类别label，
             c = features[features.size() - 1]->values[l];//当前类别c
             Dc = pc.at(c);//取出当前类别所含有的样本个数
            double current_logp=0.0;
            for (int i = 0; i < features.size() - 1; ++i) {//对于当前样本的每一个属性都有一个值，对应属于哪一个类标签的p（xi|c）需要频率计数，而对于连续属性需要对值求和sum/n得到均值

                if (features[i]->continuous) {//连续属性统计均值，实际上现在是求和sum，求和后除以个数就是均值了,
                    double  current_mean = model.at(c)[i].at("mean");
                    double  current_var = model.at(c)[i].at("var");
                    current_logp+=log( exp ((-0.5)*(atof(res[i].c_str())-current_mean)*(atof(res[i].c_str())-current_mean)/current_var)/sqrt(2*pi*current_var));
                } else {//离散值求logPc=log（）
                    current_logp+=log((model.at(c)[i].at(res[i])+1.0)/(Dc+features[i]->values.size()));//取出当前待预测样本的在类别c中属性i上的取值为res[i]的样本频率
                    //类对应的属性|i|的res[i]属性值对应频率增1
                }
            }//features.push_back(current_feature);
            current_logp+=log((Dc+1.0)/(N+features[features.size() - 1]->values.size()));//计算pc
            if(current_logp>max_logp) {
                max_logp = current_logp;
                max_c=c;
               }
            }
        ///////////////////////////////////////将当前预测完的属性值和预测标签值写入输出文件（作为一行）

        source_out.open(outfile,ios::ate|ios::app);//,ios::app);
        if(!source_out){
            cout<<"不能打开要写的文件"<<outfile<<endl;
            exit(0);
        }
        if(res[features.size()-1].compare(max_c)==0){
            count_correct++;
            source_out<<line<<endl;
        }
        else{ source_out<<line.substr(0,line.size()-res[features.size()-1].size())<<","<<max_c<<endl;//将本点指向本点值置为0，防止丢失pagerank，并且不影响分数统计
        }
        source_out.flush();
        source_out.close();
        getline(source_in, line);//以文件句柄读取一行
    }
    source_in.close();//此时应该是离散属性的频率统计和连续属性的mean求和项已经结束
    cout<<"======================================================="<<endl;
    cout<<"Summary                                                "<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    cout<<"Correctly Classified Instances          :       "<<count_correct<<"       "<<(100.0*count_correct/count)<<"%"<<endl;
    cout<<"Incorrectly Classified Instances        :       "<<count-count_correct<<"       "<<((100.0-100.0*count_correct/count))<<"%"<<endl;
    cout<<"Total Classified Instances              :       "<<count<<"       "<<endl;
    cout<<"======================================================="<<endl;
   return count_correct;
}
