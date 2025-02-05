#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef void (*function_ptr)();

struct Usuario {
    char nombre[16];
    function_ptr callback;
};

// Funcion para imprimir el flag
void imprimir_flag() {
    uint64_t dec = 0xaaaaaaaaaaaaaaaaULL;  // Inicializar con valor arbitrario para recoger consecuencias del desbordamiento
    uint64_t key = 0x7356c1470f92f1beULL;
    uint64_t enc_flag = 0xfda9f7ff9abb4616ULL;
    dec = enc_flag ^ key;
    printf("Flag: %016llx\n", dec);
    exit(0);
}


// Funcion asociada al funcinoamiento correcto
void funcionamiento_correcto(void) {
    printf("Funcionamiento correcto comprobado.\n");
}

// Funcion principal
int main() {
    // Reserva de la estructura Usuario
    struct Usuario* usuario = (struct Usuario*)malloc(sizeof(struct Usuario));
    usuario->callback = funcionamiento_correcto;
    char input_buffer[100];

    printf("Bienvenide al formulario de registro de HeapApp\nIntroduzca su nombre de usuario: ");
    gets(usuario->nombre);
	printf("\nNombre de usuario: %s\n", usuario->nombre);
	usuario->callback();

	free(usuario);
    return 0;
}
