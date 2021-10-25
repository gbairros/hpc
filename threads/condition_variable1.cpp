#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>


std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;


void worker_thread(){
    //Wait until man() sends data
    std::unique_lock<std::mutex> lk(m);
    
    //A thread fica bloqueada até a variavel "ready" atender a condição de true
    cv.wait(lk, []{return ready;});

    std::cout << "Worker thread is processing data\n";
    data+= " after processing";

    
    //é a condição para liberar o segundo bloco bloqueado na funcao main()
    processed = true;
    std::cout << "Worker threads signals data processing completed\n";

    lk.unlock();
    cv.notify_one();

}

int main(){
    std::thread worker(worker_thread);

    data = "Example data";

    {
//        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "Main signals data ready for processing\n";
    }



    //Acorda a thread
    cv.notify_one();

    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }

    std::cout << "Back in main(), data = " << data << "\n";

    worker.join();
}

