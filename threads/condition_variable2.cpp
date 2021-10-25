#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id(int id){
    std::unique_lock<std::mutex> lck(mtx);
    /*
    while(!ready){
            cv.wait(lck);
    }
    */

    cv.wait(lck, []{return ready;});


    std::cout << "Thread " << id << std::endl;
}

void go(){
    std::unique_lock<std::mutex> lck(mtx);
    ready = true; 
    //cv.notify_all();
}

int main(){
    std::cout << "TESTE" << std::endl;

    std::thread threads[10];

    for (int i = 0; i< 10; i++){
        threads[i] = std::thread(print_id, i);
    }

    std::cout << "10 threads ready" << std::endl;
    go();

    for (auto& th : threads){
        th.join();
    }
    return 0;
}
