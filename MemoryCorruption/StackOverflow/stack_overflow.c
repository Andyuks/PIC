#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// Funcion que imprime el flag asociado al exploit simple de Stack Overflow
void imprimir_flag() {
    printf("Flag: 8c8b36b409f18b51\n");
    exit(0);
}

// Funcion vulnerable a Stack Overflow
void funcion_vulnerable(char *name) {
    char buffer[64];
    strcpy(buffer, name); // Uso de strcpy sin comprobar tamanho
    printf("Direccion de inicio del buffer %p\n", &buffer);
}

// Funcion principal
int main(int argc, char *argv[]) {
    if(argc != 2) // Comprobacion de parametros
    {
        printf("Uso: %s propuesta\n", argv[0]);
        return 1;
    }

    setuid(0); 
    printf("== Bienvenides a SendProposals ==\nProcesando su propuesta...\n");
    funcion_vulnerable(argv[1]);
    printf("Propuesta recibida correctamente\n");
    return 0;
}
