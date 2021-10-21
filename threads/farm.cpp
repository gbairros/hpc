#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <mutex>
#include <vector>
#include <chrono> 

#define MAX_WORKERS 10
#define FILE_STREAM_PATH "./data/cnpj.csv"


std::vector <std::thread> workers;
std::vector <std::string> workersDataStream;

std::mutex mtx;
std::ifstream myFileStream;


void read(int id_thread){
    std::cout << "Begin Thread: " << id_thread << std::endl;

    while (workersDataStream.size() == 0){
        std::cout << "Thread " << id_thread << " Sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Waking up " << id_thread << std::endl;

    mtx.lock();
    if (!workersDataStream.empty()){
        std::string item = workersDataStream.back();
        workersDataStream.pop_back();

        std::cout << "Reading Element: "<< item << ", Thread: " << id_thread << std::endl;
    }
    mtx.unlock();
}

int main(){
    std::cout << "Starting Farm Program" << std::endl;
    myFileStream.open(FILE_STREAM_PATH);

    for (int i = 0; i < MAX_WORKERS; i++){
         workers.push_back(std::thread(read, i));
    }
   
    if (myFileStream.is_open()){
        while(  !myFileStream.eof() ){
            std::string sLine;
            std::getline(myFileStream, sLine);
            
            //std::cout << sLine << std::endl;

            workersDataStream.push_back(sLine);
        }
    }

     //MERGE   
     for(auto& thread : workers){
         thread.join();
     }

    return 0;
}