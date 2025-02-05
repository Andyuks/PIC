#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_USUARIOS 5
#define MAX_NOMBRE 64

// Flags encriptadas (flag XOR key)
const uint64_t ENC_STACK_OVERFLOW = 0x9aadf352e4096a2eULL;
const uint64_t ENC_FORMAT_STRING = 0x6df1acde262da089ULL;   
const uint64_t ENC_INTEGER_OVERFLOW = 0xdebbf5637e59ad37ULL;
const uint64_t ENC_HEAP_OVERFLOW = 0x6ff4a3f18d23a46cULL;   
const uint64_t ENC_DOUBLE_FREE = 0xef1decf3c50efe0aULL;    
const uint64_t ENC_USE_AFTER_FREE = 0x7ba99b7efd61cfceULL;  
const uint64_t ENC_OFF_BY_ONE = 0xb731da2ad45e75e0ULL;      

// Decryption key
const uint64_t KEY = 0x691c058ea97e2fb1ULL; 

// Estructura de Usuario 
typedef struct {
    int id;
    int free;
    char nombre[MAX_NOMBRE];
    int canary;  
    int privilegios;
} Usuario;

Usuario* usuarios[MAX_USUARIOS];
int num_usuarios = 0;

// Function para desencrotar flags
void desencriptar_flag(uint64_t enc_flag) {
    uint64_t dec = 0xaaaaaaaaaaaaaaaaULL;  // Inicializar con valor arbitrario para recoger consecuencias del desbordamiento
    dec = enc_flag ^ KEY;
    printf("%016llx\n", dec);
}

// Funcion para recibir feedback
void recibir_feedback() {
    char buffer[128];
    int canary =  0x1A2A3A4A;
    gets(buffer); 

    //Comprobacion: stack overflow
    if(canary != 0x1A2A3A4A){
        printf("Stack Overflow flag: ");
        desencriptar_flag(ENC_STACK_OVERFLOW);
        exit(0);
    }
    else 
        printf("Feedback recibido correctamente\n");
}

// Funcion para mostrar usuarios
void mostrar_usuario(Usuario* usuario) {
    if(usuario->free == 0)
    {
        printf("ID: %d\n", usuario->id);
        
        // Comprobacion: format string
        printf(usuario->nombre);
        if (strstr(usuario->nombre, "%") && (strchr(usuario->nombre, 'x') 
            || strchr(usuario->nombre, 'n') || strchr(usuario->nombre, 's')
            || strchr(usuario->nombre, 'd') || strchr(usuario->nombre, 'i')
            || strchr(usuario->nombre, 'u') || strchr(usuario->nombre, 'p')
            || strchr(usuario->nombre, 'f') || strchr(usuario->nombre, 'e')
            || strchr(usuario->nombre, 'g')
        
        )) {
            printf("\nFormat string flag: ");
            desencriptar_flag(ENC_FORMAT_STRING);
        }
        
        printf("\nPrivilegios: %s\n", usuario->privilegios ? "Admin" : "Usuario normal");
        printf("----------------\n");
    }
}

// Funcion para crear usuarios
void crear_usuario(int id, const char* nombre) {
	int cantidad;
    if (num_usuarios >= MAX_USUARIOS) {
        printf("No hay espacio para mas usuarios\n");
        return;
    }
	if(num_usuarios!=0){
		printf("Introducir cantidad para la reserva: ");
		scanf("%lu", &cantidad);	
	} else {
		cantidad = 1;
	}
	
    // Comprobacion: integer overflow
	if (cantidad > (INT_MAX / sizeof(Usuario))) {
        printf("Integer Overflow flag: ");
        desencriptar_flag(ENC_INTEGER_OVERFLOW);
        return;
    }
	if(num_usuarios!=0){
		printf("Cantidad establecida como 1...\n");
	}
	cantidad = 1;
	
    Usuario* nuevo = (Usuario*)malloc(cantidad * sizeof(Usuario));
    if (!nuevo) return;
    

    nuevo->id = id;
    nuevo->canary = 0x1A2A3A4A; 
    nuevo->privilegios = 0;
    nuevo->free = 0;
    strcpy(nuevo->nombre, nombre);

    // Comprobacion: Heap overflow
    if (nuevo->canary != 0x1A2A3A4A) {
        printf("Heap Overflow flag: ");
        desencriptar_flag(ENC_HEAP_OVERFLOW);
    }

    usuarios[num_usuarios++] = nuevo;
}

// Funcion para eliminar usuarios
void eliminar_usuario (int id) {
    int i;
    for (i = 0; i < num_usuarios; i++) {
        // Comprobacion: double free
        if (usuarios[i]->id == id && usuarios[i]->free == 1 ) {
            printf("Double Free flag: ");
            desencriptar_flag(ENC_DOUBLE_FREE);
            return;
        }
        else if (usuarios[i]->id == id){
            usuarios[i]->free = 1;
        }
    }
}

// Funcion para editar el nombre de un usuario
void editar_nombre(int id, const char* nuevo_nombre) {
    int i;

    for (i = 0; i < num_usuarios; i++) {
        // Comprobacion: use after free
        if (usuarios[i]->id == id && usuarios[i]->free == 1) {
            printf("Use-After-Free flag: ");
            desencriptar_flag(ENC_USE_AFTER_FREE);
            return;
        } else if (usuarios[i]->id == id) {
            sprintf(usuarios[i]->nombre, "%.64s", nuevo_nombre); 

            // Comprobacion off by one
            if (usuarios[i]->canary != 0x1A2A3A4A) {
                printf("Off-by-One flag: ");
                desencriptar_flag(ENC_OFF_BY_ONE);
            }
            return;
        }
    }
}

// Funion main
int main() {
    char input[256];
    int opcion, id;

    crear_usuario(num_usuarios, "admin");
    usuarios[0]->privilegios = 1;

    
    while (1)  // Loop infinito
    {
        // Menu de opciones
        printf("\nMenu:\n");
        printf("1. Crear usuario\n");
        printf("2. Editar nombre de usuario\n");
        printf("3. Mostrar usuarios\n");
        printf("4. Eliminar usuario \n");
        printf("5. Feedback\n");
        printf("6. Salir\n");
        printf("Opcion: ");
        
        scanf("%d", &opcion); // Obtener opcion seleccionada
		printf("\n");
        getchar();

        // Gestion de opciones
        switch (opcion) {
            case 1: // Crear usuario
                printf("Nombre: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                crear_usuario(num_usuarios, input);
                break;

            case 2: // Ediitar nombre de usuario
                printf("ID del usuario a editar: ");
                scanf("%d", &id);
                getchar();
                printf("Nuevo nombre: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                editar_nombre(id, input);
                break;

            case 3: // Mostrar usuarios
                for (int i = 0; i < num_usuarios; i++) {
                    mostrar_usuario(usuarios[i]);
                }
                break;

            case 4: // Eliminar usuario
                printf("ID del usuario a eliminar: ");
                scanf("%d", &id);
                eliminar_usuario(id);
                break;

            case 5: // Envio de feedback
                printf("Introduzca su feedback: ");
                recibir_feedback();
                break;

            case 6:
                return 0;
        }
    }
}
