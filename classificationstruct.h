#ifndef CLASSIFICATIONSTRUCT_H
#define CLASSIFICATIONSTRUCT_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
class ClassificationStruct
{
public:
    string filename;
    string label;
    int top_x;
    int top_y;
    int bottom_x;
    int bottom_y;


    ClassificationStruct();
};


struct CSList
{
    std::vector<ClassificationStruct> data;

    void addData(ClassificationStruct& d)
    {
        for(unsigned int i = 0; i < data.size(); ++i)
        {
            if(data[i].filename == d.filename)
            {
                data[i] = d;
                return;
            }

        }
        data.push_back(d);
    }

    void save(const std::string& filename)
    {
        std::ofstream myfile;
        myfile.open (filename);
        for(unsigned int i = 0; i < data.size(); ++i)
        {
            ClassificationStruct& d = data[i];
            myfile << d.filename<<" ";
            myfile << d.label<<" ";
            myfile << d.top_x<<" ";
            myfile << d.top_y<<" ";
            myfile << d.bottom_x<<" ";
            myfile << d.bottom_y<<"\n";
        }
        myfile.close();
    }

    void read(const std::string& filename)
    {

        string line;
        string name;
        string label;
        int top_x;
        int top_y;
        int bottom_x;
        int bottom_y;

        ifstream myfile (filename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                stringstream stream(line);
                stream >> name;
                stream >> label;
                stream >> top_x;
                stream >> top_y;
                stream >> bottom_x;
                stream >> bottom_y;
                ClassificationStruct d;
                d.filename = name;
                d.label = label;
                d.top_x = top_x;
                d.top_y = top_y;
                d.bottom_x = bottom_x;
                d.bottom_y = bottom_y;
                this->addData(d);
            }
            myfile.close();
        }

        else cout << "Unable to open file";
    }
};


#endif // CLASSIFICATIONSTRUCT_H
