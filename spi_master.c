#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include <stdlib.h>
#define BUF_LEN         0x80

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

int main() {
    // Enable UART so we can print
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/spi_master example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else
    int contador;
    // Enable SPI 0 at 1 KHz and connect to GPIOs
    spi_init(spi_default, 10 * 100);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));
    //
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

    //char data[80]="Hola mundo, soy un lindo ejemplo de SPI :)";
    const char * strs[] = {
        "Hola mundo", 
        "Gruppe 3 ist die beste.", 
        "Diese Arbeit ist 100 ;)",
    };
    #define string_len 100
    #define cant_strings (sizeof (strs) / sizeof (const char *))

    /*printf("SPI master envia:\n");
    printbuf(out_buf, BUF_LEN);*/

    char message[80];

    for (size_t m = 0; ; ++m) { //LAS TRANSMISIONES SE HACEN INDEFINIDAMENTE, SIEMPRE RESPETANDO EL BIT DE AKNOWLEDGEMENT

        //SENDER
        //ACOMODA EL BUFFER DE ENVIADA RESPECTO A DATA (MENSAJE A ENVIAR)
        /*for (int j=0;j<3;j++) //POR CADA STRING VA A TRANSMITIR UN MENSAJE
        {*/
            //TRANSFORMA EL MENSAJE ACTUAL A ALGO QUE PUEDA SER ENVIADO (CHAR TO ASCII)
            contador=0;
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(contador>=string_len)
                    out_buf[i]=NULL;
                else out_buf[i] = (int)strs[0][contador];
                contador++;
            }

            //SE REALIZA LA TRANSMISION
            spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);
           /* printf("iteracioon:");
            printf((int)m);
            printf("\n");
            printf("SPI master RECIBE (MOSI):\n");
            printbuf(in_buf, BUF_LEN);
            
            // PRINTEA LA LINEA MISO.       
            printf("SPI master envia (MOSI):\n");
            
            printbuf(out_buf, BUF_LEN);*/
            //SE DESCIFRA EL MENSAJE ENVIADO POR EL SLAVE (ASCII TO CHAR)
            contador=0;
            for (size_t i = 0; i < BUF_LEN; ++i) { 
                if(in_buf[i]!=0)
                {           
                    message[contador] = (char)in_buf[i];
                    contador++;
                }
                else continue;
            }

            //SE REALIZA LA REVISION DEL BIT DE AKNOWLEDGEMENT
            /*contador=0;
            bool acknowledge = 1;
            for (int i= 0; i< string_len; i++) {  //CCMPARACION
                if(strs[0][i]!= message[i])
                {           
                    acknowledge = false;
                    break;
                }
            }
            if (acknowledge == false){ //NOTIFICACION AL USUARIO
                printf("\nLos mensajes NO son iguales. Error en la transmision! \n \0");
            }
            else{
                printf("\nLos mensajes SI son iguales. Transmision exitosa! \n \0");
            }*/
            
            //PRITEA EL MENSAJE ENVIADO DESDE MASTER (MOSI)
            printf("\n");
            printf("MENSAJE ENVIADO:\0 "); //PRINTEA EL MENSAJE DECIFRADO 
            printf(strs[0]);
            printf("\n");
            //PRITEA EL MENSAJE RECIBIDO DESDE EL SLAVE
            printf("MENSAJE RECIBIDO:\0 "); //PRINTEA EL MENSAJE DECIFRADO 
            printf(message);
            printf("\nEN LA LECTURA: \0"); //PRINTEA EL MENSAJE DECIFRADO 
            printf("\n");


            // Sleep for ten seconds so you get a chance to read the output.
            sleep_ms(10 * 1000);
        //}
    }
#endif
}
