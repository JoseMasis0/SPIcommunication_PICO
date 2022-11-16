#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define BUF_LEN         0x100

// FUNCIONES NORMALES
void pause ( void )  //FUNCION PARA UNA PAUSA PEEEEERO NO FUNKAAAAAAAAA EN WINDOWS< TAL VEZ EN LA OTRA FUNQUE
{ 
  printf ( "Press [Enter] to continue . . ." );
  fflush ( stdout );
  getchar();
} 

void printbuf(uint8_t buf[], size_t len) { //FUNCION QUE PERMITE PRINTEAR EL BUFFER/
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
//MAIN
int main()
{
    //INICIALIZACION DE LAS VARIABLES A USAR
    char opcion[1]; //OPCION PARA EL MENU
    bool continuar=true; //SI ES FALSO SE SALE DEL PROGRAMA
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN]; //BUFERS MOSI Y MISO RESPECTIVAMENTE
    char data[100]="Esto es un lindo ejemplo de SPI :)"; //DATO A ENVIAR
    char message[100]; //MENSAJE RECIBIDO
    int contador=0; //CONTADOR PARA LLEGAR A TODAS LAS LETRAS
    do {
        system("cls");
        printf("======================================================================================================\n");
        printf( "SPI MASTER in C++ \n");
        printf( "======================================================================================================\n");
        printf( "1. Cambiar mensaje a transmitir\n");
        printf( "2. Enviar mensaje.\n");
        printf( "3. Salir.\n");
        printf( "MENSAJE ACTUAL:",data,"\n");
        printf( "Digite la opcion a realizar:");
        scanf("%c",opcion);
        switch(atoi(opcion)) //OPCCIONES DEL MENU
        {
            case 1: //CAMBIAR EL MENSAJE
            system("cls");
            printf("======================================================================================================\n");
            printf("CAMBIO DE MENSAJE A TRANSMITIR\n");
            printf("======================================================================================================\n");
            printf("Digite el nuevo mensaje para transmitir:");
            scanf("%s",data);
            //pause(); //PAUSA
            break;
            case 2: // ENVIAR EL MENSAJE
            //SENDER
            //ACOMODA EL BUFFER DE ENVIADA RESPECTO A DATA (MENSAJE A ENVIAR)
            contador=0;
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(contador>=sizeof(data))
                    out_buf[i]=0;
                else out_buf[i] = (int)data[contador];
                ++contador;
            }
            printbuf(out_buf, BUF_LEN); //PRINTEA EL BUFER DE SALIDA (MOSI)

            //RECEIVER (HAY QUE CAMBIARLO POR EL INPUT BUFF (MISO), PERO ES PARA VER COMO ESTA EL RICE)
            
            contador=0;
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(out_buf[i]!=0)
                {           
                    message[contador] = (char)out_buf[i];
                }
                else message[contador] = 0;
                contador++;
            }
            printf("MENSAJE DESCIFRADO: ",message,"\n"); //PRINTEA EL MENSAJE DECIFRADO
            system("pause");
            break;
            case 3: //OPCION PARA SALIR DEL PROGRAMA
            system("cls");
            printf( "Saliendo del programa SPI . . .\n");
            system("pause");
            continuar=false;
            break;
            default: //DEFAUT
            printf("\nERROR: DIGITE UNA OPCION VALIDA\n");
            pause();
            system("cls");
        }
    }
    while (continuar);
   return 0;
}