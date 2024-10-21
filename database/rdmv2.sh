
#!/bin/bash

# For MiniRotx RDMC "Rotate Display Motor Control" Ver.1.0beta
# for use stepper motor 28BYJ-48 and driver ULN2003
# By Salvatore Balestrieri (MatixVision) oct2024 
# e-mail:matixvision@gidalo.it
# youtube: @matixvision
# License: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
# Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>

# Function to check if a new game has started
LOG_FILE="/dev/shm/runcommand.log"
DB_FILE="/home/pi/database/arcade_orientation.txt"
PROGRAMMA_ESTERNO="/home/pi/rdmc_motore_stepper_v4"  # External program rdmc_motore_stepper_v4
LAST_GAME=""

# Function that checks whether a new game has been started
check_new_game() {
    if grep -q "Executing:" "$LOG_FILE"; then
        # Extract game name from log file
        ROM_PATH=$(grep "Executing:" "$LOG_FILE" | awk '{print $3}')
        GAME_NAME=$(basename "$ROM_PATH" | sed 's/\..*//')

        # If it's a new game (not the same as before), proceed
        if [ "$GAME_NAME" != "$LAST_GAME" ]; then
            echo "New game started: $GAME_NAME" # comment this line without tested
            LAST_GAME="$GAME_NAME"  # Update the last game you started

            # Search for the game in the orientation database
            ORIENTATION=$(grep "$GAME_NAME" "$DB_FILE" | cut -d '=' -f2 | tr -d ' ')

            # Run the program with the correct parameter
            if [ "$ORIENTATION" == "vertical" ]; then
                echo "The game $GAME_NAME is in portrait mode." # comment this line without tested
                sudo $PROGRAMMA_ESTERNO "verticale" &
            elif [ "$ORIENTATION" == "horizontal" ]; then
                echo "The game $GAME_NAME is in landscape mode." # comment this line without tested
                sudo $PROGRAMMA_ESTERNO "orizzontale" &
            else
                echo "No orientation found for the game $GAME_NAME in the database."
            fi
        fi
    fi
}

# Infinite loop to continuously monitor the log
while true; do
    check_new_game
    sleep 1  # Check interval (in seconds)
done

