#ifndef CLIENTE_H
#define CLIENTE_H


class Cliente{
    private:
        int demanda;
        int custoTerceirizacao;
        bool atendido = false;
        bool terceirizado = false; // talvez nao precise

    public:
        Cliente(int demanda, int custo_terceirizacao); // Construtor
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

        bool get_terceirizado(){
            return this->terceirizado;
        }
        void set_terceirizado(bool t){
            this->terceirizado = t;
        }
};

#endif