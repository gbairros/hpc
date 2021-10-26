#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <fstream>

#define MAX_WORKERS 4
#define FILE_STREAM_PATH "./data/cnpj.csv"

std::mutex mtx;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;
std::ifstream myFileStream;

std::thread workers[MAX_WORKERS]; 
std::vector <std::string> dataStream;

void process_data(int thread_id){
    //Wait until man() sends data
    std::unique_lock<std::mutex> lk(mtx);
    
    std::cout << "Begin Thread:" << thread_id << ", waiting\n" << std::endl;

    //A thread fica bloqueada até a variavel "ready" atender a condição de true
    cv.wait(lk, []{return ready;});

    if (!dataStream.empty()){
        std::string item = dataStream.back();
        dataStream.pop_back();

        std::cout << "Thread:" << thread_id << ", Data Processed: " << item << std::endl;
    }

    std::cout << "End Thread:" << thread_id << std::endl;

    if (dataStream.size() == 0){
        processed = true;
    }

    lk.unlock();
    cv.notify_all();
}


int main(){
    myFileStream.open(FILE_STREAM_PATH);

    for (int i = 0; i<MAX_WORKERS; i++){
        workers[i] = std::thread(process_data, i);
    } 

    //while(1){
        if (myFileStream.is_open()){
            while(  !myFileStream.eof() ){
                std::string sLine;
                std::getline(myFileStream, sLine);
                
                //std::cout << sLine << std::endl;
                std::unique_lock<std::mutex> lk(mtx);
                dataStream.push_back(sLine);
                ready = true;
                lk.unlock();
                cv.notify_one();
            }
            //ready = false;   
        }
    //}

    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, []{return processed;});
    }

    for (auto& th : workers){
        th.join();
    }

    return 0;
}

