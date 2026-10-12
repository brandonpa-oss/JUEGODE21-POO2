#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;

class Carta{
private:

int valor;
string simbolo;
string nombre;
public:

void aleatorio(){
int valores[13]={11,2,3,4,5,6,7,8,9,10,10,10,10};
string simbolos[4]={"Corazones","Diamante","Espada","Treboles"};
string nombres[13]={"As","2","3","4","5","6","7","8","9","10","J","Q","K"};

int _valor=rand()%13;
int _simbolo=rand()%4;
valor=valores[_valor];
simbolo=simbolos[_simbolo];
nombre=nombres[_valor];
}
int getvalor(){
    return valor;
}
string getsimbolo(){
    return simbolo;
}

string getnombre(){
    return nombre;
}
void mostrar(){
    cout<<nombre<<" de "<<simbolo<<endl;
}
};

class Persona{
private:

string nombre;
int puntaje;
Carta c[10];
int cantidad;
public:
Persona(string n){
    nombre=n;
    puntaje=0;
    cantidad=0;
}
void sacar_carta(){
    if(cantidad<10){
Carta nuevacarta;
nuevacarta.aleatorio();
c[cantidad]=nuevacarta;
cout<<nombre<<" saco:"<<endl;
c[cantidad].mostrar();
cantidad++;

}
else{
    cout<<"Alcanzo el maximo de cartas"<<endl;
}
}
int get_puntaje(){
    puntaje=0;
    for(int i=0;i<cantidad;i++){
    puntaje=puntaje+c[i].getvalor();
}
return puntaje;}

string get_nombre(){
    return nombre;
}
};

void Ganador(Persona &p1, Persona &p2) {
    int pts1=p1.get_puntaje();
    int pts2=p2.get_puntaje();
if (pts1 > 21 && pts2 > 21) {
        cout<<"Ambos se pasaron de 21. Nadie gana."<<endl;
    } 
else if (pts1>21){
        cout<<"Gana "<<p2.get_nombre()<<" ("<< p1.get_nombre()<<" se paso de 21)."<<endl;
    } 
else if (pts2>21){
        cout<<"Gana "<<p1.get_nombre()<<" ("<<p2.get_nombre()<<" se paso de 21)."<<endl;
    } 
else if (pts1>pts2) {
        cout<<"Gana "<<p1.get_nombre()<<" con "<<pts1<<" puntos"<< endl;
    } 
else if (pts2>pts1) {
        cout <<"Gana "<<p2.get_nombre()<<" con "<<pts2<<" puntos"<< endl;
    } 
else {
        cout<<"Empate Ambos tienen "<<pts1<<" puntos."<< endl;
    }
}

int main(){
    srand(time(0));
Persona p1("Juan");
p1.sacar_carta();
p1.sacar_carta();
Persona p2("Mesa");
p2.sacar_carta();
p2.sacar_carta();
Ganador(p1,p2);

return 0;
}