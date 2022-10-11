#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <chrono>


namespace fs = std::filesystem;

void sync() {
    std::vector<std::string> sourceContent;
    sourceContent.push_back("/");
    std::string source = "source";
    std::vector<std::string> replicaContent;
    replicaContent.push_back("/");
    std::string replica = "replica";
    std::string filename("log.txt");
    std::ofstream file_out;
    file_out.open(filename, std::ios_base::app);
    
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    file_out << std::endl << std::ctime(&time) << std::endl;
    
    for (auto & folders : fs::recursive_directory_iterator(source)) {
        std::string path = folders.path().string();
        sourceContent.push_back(path.replace(path.find("source"), 6, replica));
        }
    for (auto & folders : fs::recursive_directory_iterator(replica)) {
        replicaContent.push_back(folders.path().string());
        }
    
    while (!(std::equal(sourceContent.begin(), sourceContent.end(), replicaContent.begin()))) {
        std::sort (sourceContent.begin(), sourceContent.end());
        std::sort (replicaContent.begin(), replicaContent.end());

        std::pair <std::vector<std::string>::iterator, std::vector<std::string>::iterator> compare;
        compare = std::mismatch(sourceContent.begin(), sourceContent.end(), replicaContent.begin());
        std::string mismatch = *compare.first;
        std::string sourceMismatch, replicaMismatch;
        
        if(!(mismatch == "")){
            sourceMismatch = mismatch.replace(mismatch.find("replica"), 7, source);
            replicaMismatch = mismatch.replace(mismatch.find("source"), 6, replica);
        }
        
        const fs::path path(sourceMismatch);
        if (fs::is_regular_file(path)) {
            fs::copy_file(sourceMismatch, replicaMismatch);
            std::cout << "File added: " + replicaMismatch << std::endl;
            file_out << "File added: " + replicaMismatch << std::endl;
        } else if (fs::is_directory(path)) {
            fs::create_directory (replicaMismatch);
            std::cout << "Directory created: " + replicaMismatch << std::endl;
            file_out << "Directory created: " + replicaMismatch << std::endl;
        }

        replicaContent.push_back(replicaMismatch);
    }
}

int timer() {  
    std::cout << "Files synchronised" << std::endl;
    int total_time = 5;
    time_t current_time = time(0);
    
    while(time(0)-current_time<total_time)
    {
        sleep(1);
    }
    
    int completed = 1;
    return completed;
}



int main () {
    for(int i = 0; i<1;){
            sync();
            timer();
    }
}
