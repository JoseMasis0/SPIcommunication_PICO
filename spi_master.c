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

    printf("SPI master example\n");
    int contador;
    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(spi_default, 10 * 100);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));
    //
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];
    char data[100]="Hola mundo, soy un lindo ejemplo de SPI :)";

    // Initialize output buffer
    /*for (size_t i = 0; i < BUF_LEN; ++i) {
        out_buf[i] = i;
    }*/
    //SENDER
    //ACOMODA EL BUFFER DE ENVIADA RESPECTO A DATA (MENSAJE A ENVIAR)
    contador=0;
    for (size_t i = 0; i < BUF_LEN; ++i) { 
        if(contador>=sizeof(data))
            out_buf[i]=0;
        else out_buf[i] = (int)data[contador];
        ++contador;
    }

    printf("SPI master envia:\n");
    printbuf(out_buf, BUF_LEN);

    char message[100];

    for (size_t i = 0; ; ++i) {
        // Write the output buffer to MOSI, and at the same time read from MISO.
        spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);

        contador=0;
        bool acknowledge = 1;

        for (size_t i = 0; i < BUF_LEN; ++i) { 
            if(in_buf[i]!=0)
            {           
                message[contador] = (char)in_buf[i];
            }
            else message[contador] = 0;
            contador++;
        }
        printf("MENSAJE RECIBIDO: "); //PRINTEA EL MENSAJE DECIFRADO 
        printf(message);
        printf("\nEN LA LECTURA: ",i); //PRINTEA EL MENSAJE DECIFRADO 
        printf("\n");

        for (size_t i = 0; i < BUF_LEN; ++i) { 
            if(data[i]!= message[i])
            {           
                acknowledge = false;
                break;

            }
        }

        if (acknowledge == false){
            printf("Los mensajes NO son iguales. Error en la transmision! \n");
        }
        else{
            printf("Los mensajes SI son iguales. Transmision exitosa! \n");
        }

        // Write to stdio whatever came in on the MISO line.
        printf("SPI master recibe:\n", i);
        printbuf(out_buf, BUF_LEN);

        // Sleep for ten seconds so you get a chance to read the output.
        sleep_ms(10 * 1000);
    }
#endif
}
