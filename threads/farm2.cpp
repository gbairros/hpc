#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <fstream>
#include <chrono>


#define MAX_WORKERS 6
#define FILE_STREAM_PATH "./data/cnpj.csv"

std::mutex my_mutex;
std::condition_variable cv;
std::string data;
bool ready = false;
std::ifstream myFileStream;

std::thread workers[MAX_WORKERS]; 
std::queue <std::string> dataStream;

void worker_process(int thread_id){
    std::unique_lock<std::mutex> mlock(my_mutex);    
    
    std::cout << ">>THREAD: "<< thread_id << ", ANTES: SIZE QUEUE " << dataStream.size() << std::endl;

    cv.wait(mlock, []{return ready;});
    while(1){
        //A thread fica bloqueada até a variavel "ready" atender a condição de true
       // cv.wait(mlock, []{return ready;});
        while(!dataStream.empty()){
            // std::cout << "DATA STREAM EMPTY. THREAD: " << thread_id << "\n" << std::endl;

            std::string item = dataStream.front();
            dataStream.pop();

            std::cout << "Thread:" << thread_id << ", Data Processed: " << item << ". SIZE QUEUE " << dataStream.size() << "\n" << std::endl;
            mlock.unlock();
            cv.notify_all();
        }
        

        std::cout << ">>THRED:  "<< thread_id << ". LIBERADO LOCK" << std::endl;
    }

}


void emiter_process(){
    myFileStream.open(FILE_STREAM_PATH);

    if (myFileStream.is_open()){
        while(1){
            while(  !myFileStream.eof() ){
                std::unique_lock<std::mutex> mlock(my_mutex);
                ready = true;
                
                std::string sLine;
                std::getline(myFileStream, sLine);
                std::cout << "Emiter, Item: " << sLine << std::endl;                
                
                dataStream.push(sLine);
                
                mlock.unlock();
                cv.notify_all();
            }

            {
                std::lock_guard<std::mutex> lck (my_mutex);
                ready = false;
            }
            
            cv.notify_all();

            //std::cout << "EMITER BLOCK ready=false" << std::endl;
        }
    }
}

void collector_process(int thread){
    //propagar uma mensagem de END_OF_STREAM
    //implementar uma struct que contenha o dado e a flag de controle
    //A struct deve conter um contador para garantir a ordem da fila

    //Implementar o limite de Buffer das filas
    //O limite do buffer sempre deve ser maior que a qtde de threads
}

int main(){

    std::thread emiter = std::thread(emiter_process);

    for (int i = 0; i<MAX_WORKERS; i++){
        workers[i] = std::thread(worker_process, i);
    } 

    std::thread collector = std::thread(collector_process);

    emiter.join();
    
    for (auto& th : workers){
        th.join();
    }

    collector.join();

    return 0;
}