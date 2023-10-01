#ifndef CLIENTE_H
#define CLIENTE_H


class Cliente{
    private:
        int demanda;
        int custoTerceirizacao;
        bool atendido = false;

    public:
        Cliente(int demanda, int custs_terceirizacao); // Construtor
        ~Cliente(){} // Destrutor

        // Métodos gets e sets para os atributos da classe Cliente
        int get_demanda(){
            return this->demanda;
        }
        void set_demanda(int d){
            this->demanda = d;
        }

        int get_custoTerceirizacao(){
            return this->custoTerceirizacao;
        }
        void set_custoTerceirizacao(int c){
            this->custoTerceirizacao = c;
        }

        bool get_atendido(){
            return this->atendido;
        }
        void set_atendido(bool a){
            this->atendido = a;
        }
};

#endif