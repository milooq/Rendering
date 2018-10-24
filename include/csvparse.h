//#include <iterator>
#ifndef CSVPARSE_H
#define CSVPARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


class CSVfile
{
    public:
        std::vector<std::string>& operator[](const std::size_t& index) {
            return data[index];
        };

        std::size_t height() const {
            return data.size();
        };

        std::size_t width(const std::size_t& row) const {
            return data[row].size();
        };

        CSVfile(){};

        CSVfile(std::string path){
            open(path);
        };

        CSVfile(const CSVfile& other) = delete;

        ~CSVfile(){
             subData.clear();
             data.clear();
        }

        void open(const std::string& path);

    private: 
        std::vector<std::string> subData;
        std::vector<std::vector<std::string>> data;

        void readNextRow(std::istream& str);

        void clear(void) {
            subData.clear();
        }

};

void CSVfile::open(const std::string& path){
    this->data.clear();
    std::ifstream file(path);
    if(!file){
    std::cout<<"Could not open " + path + " file"<<std::endl;
    }else{
        while(file){
            this->readNextRow(file);
        };
    }
}

 void CSVfile::readNextRow(std::istream& str) {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            while(std::getline(lineStream, cell, ' '))
            {
                subData.push_back(cell);
            }
            // This checks for a trailing space with no data after it.
            if (!lineStream && cell.empty())
            {
                return;
            }
            data.push_back(subData);
            subData.clear();
        }
#endif