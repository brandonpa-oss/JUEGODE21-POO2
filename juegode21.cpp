#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<fstream>
using namespace std;

class Configuracion {
public:
    string palos[4];
    int nro_jugadores;
    int valor_ganar;
    
    int valores_base[13]={11,2,3,4,5,6,7,8,9,10,10,10,10};
    string nombres_base[13]={"As","2","3","4","5","6","7","8","9","10","J","Q","K"};

    Configuracion(){
        nro_jugadores=2;
        valor_ganar=21;
        palos[0]="Corazones";palos[1]="Diamantes";palos[2]="Espadas";palos[3]="Treboles";
    }

    void leer_archivo(){
        ifstream archivo("setup.txt");
        if (!archivo.is_open()) {
            cout<<"No se encontro setup.txt. Usando reglas normales de 21."<<endl<<endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            if (linea.find("tipos de palos:") != string::npos) {
                size_t inicio = linea.find('{');
                size_t fin = linea.find('}');
                if (inicio != string::npos && fin != string::npos) {
                    string palos_str = linea.substr(inicio + 1, fin - inicio - 1);
                    
                    string temp = "";
                    int indice_palo = 0;
                    for (int i = 0; i < palos_str.length(); i++) {
                        if (palos_str[i] == ',') {
                            palos[indice_palo] = temp;
                            indice_palo++;
                            temp = "";
                        } else if (palos_str[i] != ' ') {
                            temp += palos_str[i];
                        }
                    }
                    if (indice_palo < 4) palos[indice_palo] = temp;}
            }
            else if (linea.find("nro_jugadores:") != string::npos) {
                size_t dos_puntos = linea.find(':');
                nro_jugadores = stoi(linea.substr(dos_puntos + 1));
            }
            else if (linea.find("Valor_ganar:") != string::npos || linea.find("Valor ganar:") != string::npos) {
                size_t dos_puntos = linea.find(':');
                valor_ganar = stoi(linea.substr(dos_puntos + 1));
            }
        }
        archivo.close();
    }
};

class Carta{
private:
    int valor;
    string simbolo;
    string nombre;
public:
    void definir(int v, string s, string n){
        valor = v; simbolo = s; nombre = n;
    }
    int getvalor(){ return valor; }
    string getsimbolo(){ return simbolo; }
    string getnombre(){ return nombre; }
    void mostrar(){
        cout << nombre << " de " << simbolo << endl;
    }
};

class Mazo {
private:
    Carta cartas[52];
public:
    void crear_baraja(Configuracion &conf) {
        int contador = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 13; j++){
                cartas[contador].definir(conf.valores_base[j], conf.palos[i], conf.nombres_base[j]);
                contador++;
            }
        }
    }
    void barajar() {
        for(int i = 0; i < 52; i++){
            int pos_aleatoria = rand() % 52;
            Carta temporal = cartas[i];
            cartas[i] = cartas[pos_aleatoria];
            cartas[pos_aleatoria] = temporal;
        }
    }
    Carta repartir(int indice) {
        return cartas[indice];
    }
};

class Persona{
private:
    string nombre;
    int puntaje;
    Carta c[10];
    int cantidad;
public:
    Persona(){ 
        nombre = ""; puntaje = 0; cantidad = 0; }
    
    void set_nombre(string n){ nombre = n; }
    string get_nombre(){ return nombre; }
    int get_cantidad(){ return cantidad; }
    Carta get_carta(int indice) { return c[indice]; }

    void sacar_carta(Carta nuevacarta){
        if(cantidad<10){
            c[cantidad] = nuevacarta;
            cout<<nombre<<" saco:"<<endl;
            c[cantidad].mostrar();
            cantidad++;
        }
    }
    int get_puntaje(){
        puntaje = 0;
        for(int i=0;i<cantidad;i++){
            puntaje = puntaje + c[i].getvalor();
        }
        return puntaje;
    }
};

void GanadorMulti(Persona jugadores[], int num_jugadores, int meta) {
    int max_puntos = 0;
    int indice_ganador = -1;
    bool empate = false;

    for (int i = 0; i < num_jugadores; i++) {
        int pts=jugadores[i].get_puntaje();
        cout<<jugadores[i].get_nombre() << " tiene " << pts << " puntos." << endl;
        
        if (pts <= meta) {
            if (pts > max_puntos) {
                max_puntos = pts;
                indice_ganador = i;
                empate=false;
            } else if (pts==max_puntos) {
                empate=true;
            }
        }
    }
    if (indice_ganador==-1) {
        cout<<"Todos se pasaron de "<<meta<<". Nadie gana."<<endl;
    } else if(empate){
        cout<<"Hay un Empate con "<<max_puntos<<" puntos."<<endl;
    } else{
        cout<<"Gana "<<jugadores[indice_ganador].get_nombre()<<" con "<<max_puntos<<" puntos!"<< endl;
    }
}

int main(){
    srand(time(0));

    Configuracion config;
    config.leer_archivo();

    cout<<"Jugadores: "<<config.nro_jugadores<<endl;
    cout<<" Limite para ganar: "<<config.valor_ganar<<endl;

    Mazo mazo;
    mazo.crear_baraja(config);
    mazo.barajar();

    Persona jugadores[10];
    jugadores[0].set_nombre("Mesa");
    for(int i=1;i<config.nro_jugadores;i++) {
        jugadores[i].set_nombre("Jugador " + to_string(i));
    }

    int carta_a_repartir=0; 
    for(int i=0;i<config.nro_jugadores;i++) {
        jugadores[i].sacar_carta(mazo.repartir(carta_a_repartir)); carta_a_repartir++;
        jugadores[i].sacar_carta(mazo.repartir(carta_a_repartir)); carta_a_repartir++;
        cout<<endl;
    }

    GanadorMulti(jugadores, config.nro_jugadores, config.valor_ganar);

    ofstream archivo("partida_guardada.txt", ios::app); 
    if(archivo.is_open()){
        archivo << "Mazos de la Partida" << endl;
        
        for(int p=0;p<config.nro_jugadores;p++) {
            archivo<<"Jugador: "<<jugadores[p].get_nombre()<<endl;
            for(int i=0;i<jugadores[p].get_cantidad();i++){
                Carta carta_actual=jugadores[p].get_carta(i);
                archivo << "- " << carta_actual.getnombre() << " de " << carta_actual.getsimbolo() << endl;
            }
            archivo << endl;
        }
        archivo.close();
    }
    return 0;
}