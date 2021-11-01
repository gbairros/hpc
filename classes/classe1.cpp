#include <iostream>

class Aviao{
    private:
        int velocidadeMaxima;
    public: 
        int velocidade = 0;

        Aviao(){
            velocidade = 10;
            std::cout << "Construtor. Velocidade: " << velocidade << "\n";


        };


};

int main(){
    Aviao *a1 = new Aviao();
    Aviao a2 = Aviao();

    a1->velocidade = 100;
    a2.velocidade = 200;


    std::cout << "Velocidade do Avião1: " << a1->velocidade << std::endl;
    std::cout << "Velocidade do Avião2: " << a2.velocidade << std::endl;
    
    return 0;
}