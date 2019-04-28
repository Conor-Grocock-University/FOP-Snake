//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 23 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

#include "FoP_Task1c_SKELETON.h"

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------


int main()
{
    //local variable declarations 
    char   grid[SIZEY][SIZEX];        //grid for display
    char   maze[SIZEY][SIZEX];        //structure of the maze
    string message("LET'S START..."); //current message to player
    string playerName;                // Players chosen name

    player   spot     = {0, 0}; //spot's position
    mouse    mouse    = {3, 3}; //mouse's position
    pill     pill     = {0, 0}; //power up pill's position
    mongoose mongoose = {0, 0};

    //action...
    seed(); //seed the random number generator
    SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
    int key = 0; // Store the keycode (Used for automatic movement)

    //Store the current key press. Default to up so the snake moves up when the game starts
    do
    {
        clrscr(); // Clear the screen after each game

        getPlayerInformation(playerName);
        // Show a screen to get the player name, return it into the variable playerName

        const score highest_score = score::get_high_score(); // Get the current highest score from a file, 

        spot     = {0, 0}; //spot's position
        mouse    = {3, 3}; //mouse's position
        pill     = {0, 0}; //power up pill's position    
        mongoose = {0, 0};

        initialiseGame(grid, maze, spot, mouse, pill, mongoose); //initialise grid (incl. walls and spot)

        if (saveFileExists(playerName)) // Run a check to see if a save file exists
        {
            if (askToloadSave()) // Ask if the player wants to load the save file
            {
                loadSaveFile(playerName, spot, mouse, pill);
                // Run the function to load the data from the save file into the 3 'actors'
                renderGame(grid, message, spot, pill); //Render the saved game
            }
        }
        clrscr(); // Clear the screen before the game begins
        key = 0;  // Reset the key to 0 before the game

        while (spot.alive && !wantsToQuit(key))
        {
            Sleep(GAMEDELAY);

            handle_input(message, playerName, spot, mouse, pill, key);

            if (isArrowKey(key)) // Update the game if the current key is an arrow key, if not, re run the loop
                updateGame(grid, maze, spot, mouse, pill, mongoose, key, message);

            renderGame(grid, message, spot, pill);         //display game info, modified grid and messages
            displayPlayerInformation(spot, highest_score); // Show the score information on the side of the screen
        }

        showGameOver(spot);
        score::record_high_score(playerName, spot);
    }
    while (!wantsToQuit(key));             //while user does not want to quit
    renderGame(grid, message, spot, pill); //display game info, modified grid and messages
    return 0;
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], player& spot, mouse& mouse, pill& pill, mongoose& mongoose)
{
    setInitialMazeStructure(maze);                       //initialise maze
    setRandomItemPosition(maze, spot);                   // Place the player in a random position on the grid
    setRandomItemPosition(maze, mouse);                  // Place the mouse in a random position
    setRandomItemPosition(maze, pill);                   // Place the pill in a random position
    setRandomItemPosition(maze, mongoose);               // Place the mongoose in a random position
    updateGrid(grid, maze, spot, mouse, pill, mongoose); //prepare grid
}

void setSpotInitialCoordinates(player& spot, const char maze[][SIZEX])
{
    //set spot coordinates inside the grid at random at beginning of game
    const Position playerPosition = getRandomPosition(maze);

    spot.x = playerPosition.x;
    spot.y = playerPosition.y;
}

void setInitialMazeStructure(char maze[][SIZEX])
{
    //set the position of the walls in the maze
    //initialise maze configuration
    char initialMaze[SIZEY][SIZEX] //local array to store the maze structure
            = {
                {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
                {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
                {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
            };

    //with '#' for wall, ' ' for tunnel, etc. 
    //copy into maze structure with appropriate symbols
    for (int     row(0); row < SIZEY; ++row)
        for (int col(0); col < SIZEX; ++col)
            switch (initialMaze[row][col])
            {
                //not a direct copy, in case the symbols used are changed
            case '#': maze[row][col] = WALL;
                break;
            case ' ': maze[row][col] = TUNNEL;
                break;
            }
}

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGame(char       grid[][SIZEX],
                const char maze[][SIZEX],
                player&    spot,
                mouse&     mouse,
                pill&      pill,
                mongoose&  mongoose,
                const int  keyCode,
                string&    mess)
{
    //update game
    updateGameData(grid, spot, mouse, pill, mongoose, keyCode, mess); //move spot in required direction
    updateGrid(grid, maze, spot, mouse, pill, mongoose);              //update grid information
}

void updateGameData(const char g[][SIZEX], player& spot, mouse& mouse, pill& pill, mongoose& mongoose, const int key,
                    string&    mess)
{
    //move spot in required direction

    assert(isArrowKey(key));

    //reset message to blank
    mess = "";

    //calculate direction of movement for given key
    int dx(0), dy(0);
    setKeyDirection(key, dx, dy);

    spot.update();
    mongoose.update(g, spot);

    //check new target position in grid and update game data (incl. spot coordinates) if move is possible
    switch (g[spot.y + dy][spot.x + dx])
    {
        //...depending on what's on the target position in grid...
    case TAIL: 
        spot.alive = false;
        spot.causeOfDeath = "You resorted the cannibalism.";
        break;
    case TUNNEL: //can move
        spot.move(dy, dx);
        break;
    case WALL: //hit a wall and stay there
        //cout << '\a';	//beep the alarm
        if (spot.inInvincibleMode) { spot.wrap_player(dy, dx); }
        else
        {
            spot.alive = false;
            spot.causeOfDeath = "You tried to eat the wall.";
        }
        break;
    case MOUSE: mouse.collide(g, spot, mouse, pill, dx, dy);
        break;
    case PILL: pill.collide(spot);
        spot.move(dy, dx);
        break;
    case MONGOOSE: 
        spot.alive = false;
        spot.causeOfDeath = "Killed by Mongoose.";
        break;
    }
}

void updateGrid(char           grid[][SIZEX], const char maze[][SIZEX], const player spot, const mouse mouse,
                const pill     pill,
                const mongoose mongoose)
{
    placeMaze(grid, maze); //reset the empty maze configuration into grid

    mouse.place(grid);
    pill.place(grid);
    spot.place(grid);
    mongoose.place(grid);
}

void handle_input(string& message, string playerName, player spot, mouse mouse, pill pill, int& key)
{
    if (_kbhit())
    {
        // Detect if any key has been pressed
        // Only update the main key variable if the key is for movement
        // Otherwise run the relevent function. This mean that game play 
        // is not interrupted by keys such as quit
        int newKey = getKeyPress(); //read in  selected key: arrow or letter command

        if (isArrowKey(newKey))
            key = newKey;
            // If the key is an arrow key, set the key variable, so it can be used to direct movement
        else if (wantsToQuit(newKey))                  //if the key is a quit key
            endProgram();                              //end the game
        else if (wantsToCheat(newKey))                 // If the key is the cheat key
            spot.toggle_cheatmode();                   // Enable cheat mode
        else if (wantsToSeeScoreboard(newKey))         // If the key is the scoreboard key
            showScoreboard();                          // Clear the screen and show the scoreboard
        else if (wantsToSave(newKey))                  // IF the key is the save key
            saveToFile(playerName, spot, mouse, pill); // Save the current game to a file
        else
            message = "INVALID KEY!"; //set 'Invalid key' message
    }
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{
    //reset the empty/fixed maze configuration into grid
    for (int     row(0); row < SIZEY; ++row)
        for (int col(0); col < SIZEX; ++col)
            grid[row][col] = maze[row][col];
}

void showGameOver(const player player) 
{
    clrscr();

    showMessage(clBlack, clWhite, 0, 0, "Game over");
    showMessage(clBlack, clWhite, 0, 1, player.causeOfDeath);
    showMessage(clBlack, clWhite, 0, 2, "Press return to continue");

    bool restart = false;
    while (!restart)
    {
        int newKey = getKeyPress(); //read in  selected key: arrow or letter command
        if (newKey == '\r' || newKey == '\n')
            restart = true;
    }
}

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------

void setKeyDirection(const int key, int& dx, int& dy)
{
    assert(isArrowKey(key));

    //calculate direction indicated by key
    switch (key) //...depending on the selected key...
    {
    case LEFT:   //when LEFT arrow pressed...
        dx = -1; //decrease the X coordinate
        dy = 0;
        break;
    case RIGHT:  //when RIGHT arrow pressed...
        dx = +1; //increase the X coordinate
        dy = 0;
        break;
    case UP: //when UP arrow pressed...
        dx = 0;
        dy = -1; // decrease the Y coordinate
        break;
    case DOWN: //when DOWN arrow pressed...
        dx = 0;
        dy = +1; // increase the Y coordinate
        break;
    }
}

int getKeyPress()
{
    //get key or command selected by user
    //KEEP THIS FUNCTION AS GIVEN
    int keyPressed;
    keyPressed = _getch();    //read in the selected arrow key or command letter
    while (keyPressed == 224) //ignore symbol following cursor key
        keyPressed = _getch();
    return keyPressed;
}

bool isArrowKey(const int key)
{
    //check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
    return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool wantsToQuit(const int key)
{
    //check if the user wants to quit (when key is 'Q' or 'q')
    return key == QUIT || key == tolower(QUIT);
}

bool wantsToCheat(const int key)
{
    //check if the user wants to quit (when key is 'Q' or 'q')
    return key == CHEAT || key == tolower(CHEAT);
}

bool wantsToSeeScoreboard(const int key)
{
    //check if the user wants to quit (when key is 'Q' or 'q')
    return key == SCOREBOARD || key == tolower(SCOREBOARD);
}

void showScoreboard()
{
    clrscr();
    vector <score> scores = score::load_scores();
    scores                = score::sort_scores(scores);

    showMessage(clDarkBlue, clBlue, 0, 0, "High scores");
    for (int i = 0; i <= 6; i++)
    {
        string score_msg;

        if (scores.size() > i)
            score_msg = scores[i].name + ": " + tostring(scores[i].moves);
        else
            score_msg = "anonymous: -1";
        showMessage(clBlack, clWhite, 0, i, score_msg);
    }

    int newKey;
    do
    {
        newKey = getKeyPress(); //read in  selected key: arrow or letter command
    }
    while (!(newKey == '\n' || newKey == '\r'));
}

int show_menu(const string question, vector <string> options)
{
    bool selectionMade = false;
    int  index         = 0;

    showMessage(clBlack, clWhite, 0, 0, question);
    while (!selectionMade)
    {
        for (int i = 0; i < options.size(); i++) { showMessage(clBlack, clWhite, 0, i + 1, "  "); }

        showMessage(clBlack, clWhite, 0, index + 1, "> ");

        for (int i = 0; i < options.size(); i++) { showMessage(clBlack, clWhite, 10, i + 1, options[i]); }

        const int key = getKeyPress();
        if (key == UP)
        {
            index--;
            if (index < 0)
                index = 0;
        }
        else if (key == DOWN)
        {
            index++;
            if (index > 1)
                index = 0;
        }
        else
            if (key == RIGHT) { selectionMade = true; }
    }

    return index;
}

bool askToloadSave()
{
    clrscr();
    vector <string> items;
    items.push_back("No");
    items.push_back("Yes");

    return show_menu("Would you like to load a previous save?", items);
}

bool wantsToSave(const int key) { return key == SAVE || key == tolower(SAVE); }

//---------------------------------------------------------------------------
//----- Saving and loading game states
//---------------------------------------------------------------------------

bool saveFileExists(string playerName)
{
    ifstream file(playerName + SAVEFILEEXTENSION);
    return file.good();
}

void loadSaveFile(string player_name, player& player, mouse& mouse, pill& pill)
{
    string   objectLine;
    string   line;
    ifstream score_file(player_name + SAVEFILEEXTENSION, std::ifstream::in);
    if (score_file.is_open())
    {
        while (getline(score_file, line))
        {
            vector <string> components = split(line, ':');
            if (components[0] == "player")
            {
                if (components[1] == "x")
                    player.x = stoi(components[2]);
                else if (components[1] == "y")
                    player.y = stoi(components[2]);
                else if (components[1] == "mouse_count")
                    player.mouseCount = stoi(components[2]);
                else if (components[1] == "size")
                    player.maxSize = stoi(components[2]);
                else if (components[1] == "moves")
                    player.numberOfMoves = stoi(components[2]);
            }
            else if (components[0] == "mouse")
            {
                if (components[1] == "x")
                    mouse.x = stoi(components[2]);
                else if (components[1] == "y")
                    mouse.y = stoi(components[2]);
            }
            else if (components[0] == "pill")
            {
                if (components[1] == "x")
                    pill.x = stoi(components[2]);
                else if (components[1] == "y")
                    pill.y = stoi(components[2]);
                else if (components[1] == "show")
                    pill.show = stoi(components[2]);
            }
        }
        score_file.close();
    }
}

void saveToFile(const string player_name, const player player, const mouse mouse, const pill pill)
{
    ofstream out(player_name + SAVEFILEEXTENSION, fstream::trunc);
    if (out.is_open())
    {
        out << "player:x:" << tostring(player.x) << "\n";
        out << "player:y:" << tostring(player.y) << "\n";
        out << "player:mouse_count:" << tostring(player.mouseCount) << "\n";
        out << "player:moves:" << tostring(player.numberOfMoves) << "\n";
        out << "player:size:" << tostring(player.maxSize) << "\n";

        out << "mouse:x:" << tostring(mouse.x) << "\n";
        out << "mouse:y:" << tostring(mouse.y) << "\n";

        out << "pill:x:" << tostring(pill.x) << "\n";
        out << "pill:y:" << tostring(pill.y) << "\n";
        out << "pill:show:" << tostring(pill.show) << "\n";
        out.close();
    }
}

//---------------------------------------------------------------------------
//----- Player information
//---------------------------------------------------------------------------

void getPlayerInformation(string& name)
{
    gotoxy(0, 1);
    cout << "Please enter your name" << endl;
    cin >> name;
}

void displayPlayerInformation(player player, score highest_score)
{
    showMessage(clWhite, clBlack, 40, 7, "Score: " + tostring(player.numberOfMoves));
    showMessage(clWhite, clBlack, 40, 8, tostring(player.mouseCount) + " out of " + tostring(MAXSCORE));
    showMessage(clWhite, clBlack, 40, 9, "High score");
    showMessage(clWhite, clBlack, 40, 10, highest_score.name + ": " + tostring(highest_score.moves));
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{
    //convert an integer to a string
    std::ostringstream os;
    os << x;
    return os.str();
}

string tostring(char x)
{
    //convert a char to a string
    std::ostringstream os;
    os << x;
    return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{
    //display a string using specified colour at a given position 
    gotoxy(x, y);
    selectBackColour(backColour);
    selectTextColour(textColour);
    cout << message + string(40 - message.length(), ' ');
}

void renderGame(const char g[][SIZEX], const string& mess, const player& spot, const pill& pill)
{
    //display game title
    showMessage(clBlack, clGreen, 0, 0, "Snake Game");

    showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - " + getTime());
    showMessage(clWhite, clRed, 40, 1, "SE2 - Conor Grocock (b8022088)");
    showMessage(clWhite, clRed, 40, 2, "SE2 - Rae Hewitt (b8014125)");
    //display menu options available
    showMessage(clRed, clYellow, 40, 4, "TO MOVE - USE KEYBOARD ARROWS ");
    showMessage(clRed, clYellow, 40, 5, "TO QUIT - PRESS 'Q'           ");

    if (spot.inCheatMode)
        showMessage(clRed, clYellow, 40, 14, "CHEAT MODE ON");
    else
        showMessage(clBlack, clYellow, 40, 14, "");
    if (spot.inInvincibleMode)
        showMessage(clRed, clYellow, 40, 15, "INVINCIBLE SNAKE");
    else
        showMessage(clBlack, clYellow, 40, 15, "");

    //print auxiliary messages if any
    showMessage(clBlack, clWhite, 40, 11, mess); //display current message

    //display grid contents
    paintGrid(g, spot, pill);
}

void paintGrid(const char g[][SIZEX], const player& player, const pill& pill)
{
    //display grid content on screen
    selectBackColour(clBlack);
    selectTextColour(clWhite);
    gotoxy(0, 2);
    for (int row(0); row < SIZEY; ++row)
    {
        for (int col(0); col < SIZEX; ++col)
        {
            const char cell = g[row][col];
            if (cell == SPOT)
            {
                //check for snake 
                if (player.inInvincibleMode)
                    selectTextColour(clGreen); //if rendering the snake make it green
                else
                    selectTextColour(clYellow); //if rendering the snake make it green
            }
            else if (cell == TAIL)
            {
                if (player.inInvincibleMode)
                    selectTextColour(clYellow);
                else
                    selectTextColour(clGreen);
            }
            else if (cell == PILL)
                selectTextColour(clRed);
            else if (cell == MONGOOSE)
                switch (random(3))
                {
                case 1: selectTextColour(clBlue);
                    break;
                case 2: selectTextColour(clCyan);
                    break;
                case 3: selectTextColour(clGreen);
                    break;
                }
            else if (cell == WALL)
                selectTextColour(clGrey);
            else
                selectTextColour(clWhite); //any other cell is white

            if (cell == PILL && !pill.show)
                cout << TUNNEL;
            else
                cout << cell; //output cell content
        }
        cout << endl;
    }
}

vector <string> split(const string& s, char splitChar)
{
    vector <string> components;
    string          component;
    istringstream   componentStream(s);
    while (getline(componentStream, component, splitChar)) { components.push_back(component); }
    return components;
}

void endProgram()
{
    showMessage(clRed, clYellow, 40, 8, "Goodbye!");
    //system("pause");	//hold output screen until a keyboard key is hit
}
