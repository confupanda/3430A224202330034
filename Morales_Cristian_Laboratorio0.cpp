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

    cout<<" par\n";

    }else{
    cout<<" no es par\n";
    }

    for(int i=1;i<=10;i++){

        cout<<i<<(par(i)? " SI es par\n" : " NO es par\n");

    }

    string textousuario;

    cout<<"\ningresa el string: "; cin.ignore(); cin>>textousuario;
    /*getline(cin,textousuario);*/

    cout<<"\nla longuitud del string es "<<longitud(textousuario)<<endl;

    string determinado = "\nholaQuetalJovencillo";

    cout<<"\nla texto y la longitud determinada respectivamente es: "<<" "
    <<determinado<<" 'y' "<<longitud(determinado)<<endl;





    return 0;

}
