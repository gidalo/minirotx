
#!/bin/bash

# File di log da monitorare
LOG_FILE="/dev/shm/runcommand.log"
DB_FILE="/home/pi/database/arcade_orientation.txt"
PROGRAMMA_ESTERNO="/home/pi/rdm_motore_stepper_v4"  # Il tuo eseguibile C
LAST_GAME=""

# Funzione che controlla se e stato avviato un nuovo gioco
check_new_game() {
    if grep -q "Executing:" "$LOG_FILE"; then
        # Estrai il nome del gioco dal file di log
        ROM_PATH=$(grep "Executing:" "$LOG_FILE" | awk '{print $3}')
        GAME_NAME=$(basename "$ROM_PATH" | sed 's/\..*//')

        # Se e un nuovo gioco (non lo stesso di prima), procedi
        if [ "$GAME_NAME" != "$LAST_GAME" ]; then
            echo "Nuovo gioco avviato: $GAME_NAME"
            LAST_GAME="$GAME_NAME"  # Aggiorna l'ultimo gioco avviato

            # Cerca il gioco nel database di orientamento
            ORIENTATION=$(grep "$GAME_NAME" "$DB_FILE" | cut -d '=' -f2 | tr -d ' ')

            # Esegui il programma con il parametro corretto
            if [ "$ORIENTATION" == "vertical" ]; then
                echo "Il gioco $GAME_NAME e in modalita verticale."
                sudo $PROGRAMMA_ESTERNO "verticale" &
            elif [ "$ORIENTATION" == "horizontal" ]; then
                echo "Il gioco $GAME_NAME e in modalita orizzontale."
                sudo $PROGRAMMA_ESTERNO "orizzontale" &
            else
                echo "Non e stato trovato l'orientamento per il gioco $GAME_NAME nel database."
            fi
        fi
    fi
}

# Ciclo infinito per monitorare continuamente il log
while true; do
    check_new_game
    sleep 1  # Intervallo di controllo (in secondi)
done
