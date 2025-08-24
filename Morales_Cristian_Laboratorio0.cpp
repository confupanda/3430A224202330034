#include<iostream>
#include<cstring>

using namespace std;

double par(int numeroestandar){

    return numeroestandar %2==0;


}
int longitud(const string &texto){

    return texto.length();
}


int main (){

    int numeroestandar;


    cout<<"\ningrese un numero: ";cin>>numeroestandar;

    cout<<"\n el numero es: "<<numeroestandar;

    if(par(numeroestandar)){

    cout<<"\n par";

    }else{
    cout<<"\n no es par";
    }

    for(int i=1;i<=10;i++){

        cout<<i<<(par(i)? "\nSI es par" : "\nNO es par");

    }

    string textousuario;

    cout<<"\ningresa el string: "; cin.ignore(); cin>>textousuario;
    //getline(cin,textousuario);

    cout<<"\nla longuitud del string es "<<longitud(textousuario)<<endl;

    string determinado = "\nholaQuetalJovencillo";

    cout<<"\nla longuitud del determinado es: "<<longitud(determinado)<<endl;





    return 0;
}