/* 
* MiniRotx RDMC "Rotate Display Motor Control" Ver.1.0beta
* for use stepper motor and driver
* By Salvatore Balestrieri (MatixVision) oct2024 
* e-mail:matixvision@gidalo.it
* youtube: @matixvision
* License: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
* 
* sudo apt-get update
* sudo apt install -y wiringpi
* gpio -v
* gcc -o rdmc minirotx_rdmc minirotx_rdmc.c -lwiringPi
* sudo ./rdmc minirotx_rdmc forward
* sudo ./rdmc minirotx_rdmc backward
*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> // Inclusione per le funzioni di sistema

#define STATE_FILE "/home/pi/motor_state.txt"  // Percorso assoluto per il file di stato

// Definizione dei pin per il motore passo-passo
#define IN1 0  // Pin WiringPi 0 (GPIO 17)
#define IN2 1  // Pin WiringPi 1 (GPIO 18)
#define IN3 2  // Pin WiringPi 2 (GPIO 27)
#define IN4 3  // Pin WiringPi 3 (GPIO 22)

// Definizione dei pin per i finecorsa
#define ENDSTOP_FORWARD 4   // Pin WiringPi 4 (GPIO 23)
#define ENDSTOP_BACKWARD 5  // Pin WiringPi 5 (GPIO 24)

// Parametri motore
int step_count = 4096; // Numero di passi per un giro completo (360°)
int step_sleep = 2000; // Microsecondi tra i passi
int motor_step_counter = 0;

// Sequenza del motore passo-passo
int step_sequence[8][4] = {
    {1, 0, 0, 1},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1}
};

// Funzione di pulizia
void cleanup() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Funzione per controllare il motore passo-passo
void step_motor(int direction) {
    for (int pin = 0; pin < 4; pin++) {
        digitalWrite(IN1 + pin, step_sequence[motor_step_counter][pin]);
    }

    if (direction == 1) {
        motor_step_counter = (motor_step_counter + 1) % 8;
    } else {
        motor_step_counter = (motor_step_counter - 1 + 8) % 8;
    }
}

// Funzione per leggere lo stato dal file di testo
char* read_last_state() {
    FILE *file = fopen(STATE_FILE, "r");
    if (file == NULL) {
        return NULL;  // Se non esiste il file, non c'è stato precedente
    }

    static char last_state[10];
    if (fgets(last_state, sizeof(last_state), file) != NULL) {
        // Rimuove eventuale newline alla fine della stringa
        size_t len = strlen(last_state);
        if (len > 0 && last_state[len-1] == '\n') {
            last_state[len-1] = '\0';
        }
    }
    fclose(file);
    return last_state;
}

// Funzione per scrivere il nuovo stato nel file di testo
void write_new_state(char* state) {
    FILE *file = fopen(STATE_FILE, "w");
    if (file == NULL) {
        printf("Errore durante la scrittura dello stato.\n");
        return;
    }
    fprintf(file, "%s\n", state);
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Controllo degli argomenti da linea di comando
    if (argc != 2) {
        printf("Uso: %s <forward|backward>\n", argv[0]);
        return 1;
    }

    char* requested_direction = argv[1];
    int direction;

    // Determina la direzione basata sull'input CLI
    if (strcmp(requested_direction, "forward") == 0) {
        direction = 1;  // Avanti (senso orario)
    } else if (strcmp(requested_direction, "backward") == 0) {
        direction = 0;  // Indietro (senso antiorario)
    } else {
        printf("Comando non valido. Usa 'forward' o 'backward'.\n");
        return 1;
    }

    // Leggi lo stato precedente dal file
    char* last_state = read_last_state();

    // Se lo stato richiesto è uguale a quello precedente e i finecorsa non sono attivati, non eseguire
    if (last_state != NULL && strcmp(last_state, requested_direction
