# Minesweeper
💥💣Minesweeper Video Game💣💥

https://user-images.githubusercontent.com/95982168/210435763-41371848-175e-4609-afcd-3f3a3d804d84.mp4

Simulates a game of minesweeper inside the terminal. Minesweeper.cpp can accept command line arguments specifying the width, length, and number of mines on the board. For example, after compiling minesweeper.cpp, the user can invoke "./a.exe 15 18 30" to specify that the width = 15, height = 18, and number of mines = 30. Or the user can invoke "./a.exe 15 18" to specify that the width = 15 and height = 18. The number of mines is default 20. Or for simplicity's sake, the user can invoke "./a.exe" to run a game where default settings are applied. Default settings: Width = 18, height = 14, and number of mines = 40. These command line arguments use separate class constructors.

![minesweeper_demo_win](https://user-images.githubusercontent.com/95982168/210471202-32e2e4d9-e65d-4224-8b55-97f1fb7cb7e1.gif)

When the player has revealed all tiles that don't contain mines, the user is notified that they one the game and the minesweeper.exe ends.

![minesweeper_demo_lose](https://user-images.githubusercontent.com/95982168/210471701-74249bc0-bffb-40b7-8aed-c7be61af3f87.gif)

When the player choses a tile that contains a mine, the user is notified that they lost the game. The whole board is then revealed to the user.

⭐Aside: After taking my Data Structures and Algorithms course, I've realized that my implementation for this tool is highly inefficient. I should try to redo this project some other time. 😁
