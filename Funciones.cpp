#include <iostream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <sstream>
using namespace std;

#define BUF_LEN         0x100

void printbuf(uint8_t buf[], size_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        if (i % 16 == 15)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 16) {
        putchar('\n');
    }
}

int main()
{
    int opcion;
    bool continuar=true;
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];
    string data="Esto es un lindo ejemplo de SPI :)";
    string message="";
    int decimal;
    int contador=0;
    do {
        system("cls");
        printf("======================================================================================================\n");
        printf( "SPI MASTER in C++ \n");
        printf( "======================================================================================================\n");
        printf( "1. Cambiar mensaje a transmitir\n");
        printf( "2. Enviar mensaje.\n");
        printf( "3. Salir.\n");
        printf( "MENSAJE ACTUAL: \n");
        cout<<data<<endl;
        printf("\n");
        printf( "Digite la opcion a realizar:");
        cin>>opcion;
        switch(opcion) //donde opción es la variable a comparar
        {
            case 1: //Bloque de instrucciones 1;
            system("cls");
            printf("======================================================================================================\n");
            printf("CAMBIO DE MENSAJE A TRANSMITIR\n");
            printf("======================================================================================================\n");
            printf("Digite el nuevo mensaje para transmitir:");
            cin>>data;
            break;
            case 2: 
            //SENDER
            //ACOMODA EL BUFFER DE ENVIADA
            contador=0;
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(contador>sizeof(data))
                    out_buf[i]=0;
                else out_buf[i] = int(data[contador]);
                ++contador;
            }
            
            printbuf(out_buf, BUF_LEN);
            //RECEIVER 
            printf("MENSAJE DESCIFRADO");
            contador=0;
            message="";
            printf("\n");
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(out_buf[i]!=0)
                {
                    message=message+char(stoi(to_string(out_buf[i])));
                }
                else continue;
                contador++;
            }
            cout << message <<endl;
            printf("\n");
            system("pause");
            break;
            case 3: //Bloque de instrucciones 3;
            system("cls");
            printf( "Saliendo del programa SPI . . .\n");
            system("pause");
            continuar=false;
            break;
            default: 
            printf("\nERROR: DIGITE UNA OPCION VALIDA\n");
            system("pause");
            system("cls");
        }

    }
    while (continuar);
   return 0;
}