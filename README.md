# BombIt Matrix-Game
This matrix project is a robust version of the classic game Bomberman, the functionalities being heavily inspired from it.

## How to play:

The game starts with the player on the upper left of the randomly generated playfield. Using the joystick, the player moves around the matrix with care in order to avoid collision with the walls and places bombs in order to destroy them. As the bombs explode on both the line and column they're on within three second of being placed, the player has to avoid the blast in order to survive. Moreover, the player can discover three more secret rooms that have to be cleared in order to win. The challenge of the game is not only to rid the field of walls, but also to do with the lowest number of bombs possible and finish the level fast in order to recieve a higher score.

## Menu structure and options:

When powering the game, a short greeting appears on the LCD for two seconds, then the user is presented with the menu structure as follows:
The main menu:
1. **Start game**: selecting this option will generate a new field and start the counters for recording both the play time and number of bombs used; this information is displayed on the LCD in real time; if the player clears the field, they will be able to record their name or alias in three letters, then the score will appear as well; if the player's score is higher than the ones recorded in the highscore leaderboard, the message "New highscore" will appear; if the player loses, they will be given the option to try again or go back to the main menu. 
2. **Highscore**: when clicking the second menu option, the top 5 recorded scores and players will displayed on the LCD, in descending order.
3. **Settings**: this tab will open a settings submenu, having the following options:
   3.1 _Reset highscore_: this option, when clicked, deletes all the logs saved in the leaderboard.
   3.2 _LCD brightness_: by toggling the joystick left and right, the user can dim or enhance the brightness of the LCD.
   3.3 _Game brightness_: similarily to 3.1, this option is used to modify the matrix brightness.
   3.4 _Sounds on/off_: as the game contains sound effects when moving on the playfield, placing bombs or scrolling through the menu, this setting gives the possibility to mute them.
   3.5 _Main menu_: returning to the main menu.
4. **About**: selecting this tab will prompt the user to another screen where the name of the game, the author and the github of the author are displayed by scrolling.
5. **How to play**: similarily to 4, this screen shows brief instructions on playing the game.

## Components and setup

In order to build the hardware aspect of the game, I used the Arduino UNO microcontroller board, three mini breadboards, an 8 by 8 LED matrix, connected to the Arduino with the MAX7219 LED Driver, an LCD, a potentiometer for manual intensity control, an active buzzer, a joystick, as well as wires and resistors as needed.

![Setup](https://github.com/vfranci/BombIt-Matrix-Game/assets/115077321/68101ed9-8334-4b5b-920e-4ad0e2a94ea5)

Video demo: https://youtu.be/s_qTXY19jb4?si=UJOORCHuB3rpo5zU


