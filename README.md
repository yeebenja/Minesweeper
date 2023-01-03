# Minesweeper
Minesweeper Video Game

https://user-images.githubusercontent.com/95982168/210435763-41371848-175e-4609-afcd-3f3a3d804d84.mp4

Simulates a game of minesweeper inside the terminal. Minesweeper.cpp can accept command line arguements specifying the width, length, and number of mines on the board. For example, after compiling minesweeper.cpp, the user can invoke "./a.exe 15 18 30" to specify that the width = 15, height = 18, and number of mines = 30. Or, the user can invoke "./a.exe 15 18" to specify that the width = 15 and height = 18. The number of mines is default 20. Or for simplicity's sake, the user can invoke "./a.exe" to run a game where width = 12, height = 12, and number of mines = 20. These command line arguements use separate constructors.

[gif]

When the player has revealed all tiles that don't contain mines, the user is notified that they one the game and the minesweeper.exe ends.

[gif]

When the player choses a tile that conatians a mine, the user is notified that they lost the game. The whole board is then revealed to the user.
