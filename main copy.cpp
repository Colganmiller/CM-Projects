#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
//#include "class headers.h"

using namespace std;


string _cols;
string _rows;
string _mines;
int newcols;
int newrows;
int newmines;
void loadConfig(const std::string& filename);




class Tile {

public:

    sf::Texture hidden_tile;
    sf::Texture revealed_tile;
    sf::Sprite sprite;
    sf::Texture numTextures[8];
    //sf::Texture num1;
    //sf::Texture num2;
    //sf::Texture num3;
    //sf::Texture num4;
    //sf::Texture num5;
    //sf::Texture num6;
    //sf::Texture num7;
    //sf::Texture num8;
    sf::Texture flagTexture; //
    sf::Texture mineTexture; //
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;

    sf::Vector2f position;
    void loadimages();
    void setposition(float x,float y, string filename, sf::Texture &image);

    void loadImages() {
        hidden_tile.loadFromFile("images/tile_hidden.png");
        revealed_tile.loadFromFile("images/tile_revealed.png");
        flagTexture.loadFromFile("images/flag.png");
        mineTexture.loadFromFile("images/mine.png");
        numTextures[0].loadFromFile("images/number_1.png");
        numTextures[1].loadFromFile("images/number_2.png");
        numTextures[2].loadFromFile("images/number_3.png");
        numTextures[3].loadFromFile("images/number_4.png");
        numTextures[4].loadFromFile("images/number_5.png");
        numTextures[5].loadFromFile("images/number_6.png");
        numTextures[6].loadFromFile("images/number_7.png");
        numTextures[7].loadFromFile("images/number_8.png");
    }

    Tile() : isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0), position(0, 0){}
};

class Board{
    sf::Texture digitsTexture;
    std::vector<sf::Sprite> digitSprites;

public:
    sf::Sprite sprite;
    sf::Sprite test3sprite;
    sf::Sprite test2sprite;
    sf::Sprite test1sprite;
    sf::Sprite debug_sprite;
    sf::Sprite digits_sprite;
    sf::Sprite winsprite;

    sf::Texture debug;
    sf::Texture digits;
    sf::Texture happyface;
    sf::Texture loseface;
    sf::Texture winface;


    sf::Texture test1;
    sf::Texture test2;
    sf::Texture test3;
    int initialFlags;
    int columns;
    int rows;
    int minecount;
    int flagsplaced;
    vector<vector<Tile>> tiles;
    bool gameOver;
    bool debugMode;
    bool newGame;
    bool win;
    bool boardload;



    const int digitWidth = 21;
    const int digitHeight = 32;
    void wincondition(sf::RenderWindow &window);

    void drawRemainingMines(sf::RenderWindow &window, int remainingMines);

    Board(int cols, int rows, int mines);

    void initializeBoard();
    void revealTile(int x, int y, sf::RenderWindow &window);
    void toggleFlag(int x, int y, sf::RenderWindow &window);
    void restart(sf::RenderWindow &window);
    void toggleDebugMode(sf::RenderWindow &window);
    int countAdjacentMines(int x, int y);
    //void loadConfig(const string& filename);
    void loadBoard(const string& filename, sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);
    void loadimages();
    void setposition(float x,float y, string filename, sf::Texture &image);
    void resetBoard(sf::RenderWindow &window);
    void gameover(sf::RenderWindow &window);
    void drawDigit(sf::RenderWindow &window, int digit, int x, int y);

    void setRemainingMines(int remainingMines);
};


void Tile::setposition(float x, float y, string filename, sf::Texture &image) {
    image.loadFromFile(filename);
    sprite.setTexture(image);
    sprite.setPosition(x, y);
}




Board::Board(int cols, int rows, int mines)
        : columns(cols), rows(rows), minecount(mines), gameOver(false), debugMode(false), newGame(false), win(false), boardload(false) {
    initializeBoard();
}

void Board::initializeBoard() {


    tiles.resize(rows, vector<Tile>(columns));
    int placedMines = 0;
    if (boardload == false)
    {
        while (placedMines < minecount) { //places the mines here
            int x = rand() % columns;
            int y = rand() % rows;
            if (!tiles[y][x].isMine) {
                tiles[y][x].isMine = true;
                placedMines++;
            }
        }
    }


    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            if (!tiles[y][x].isMine) {
                tiles[y][x].adjacentMines = countAdjacentMines(x, y); //counts adjacent mines here
            }
        }
    }
}

int Board::countAdjacentMines(int x, int y) {
    int count = 0;
    for (int y2 = -1; y2 <= 1; y2++) {    //around the perimeter logic
        for (int x2 = -1; x2 <= 1; x2++) {
            if (x2 == 0 && y2 == 0) continue;
            int newx = x + x2;
            int newy = y + y2;
            if (newx >= 0 && newy >= 0 && newx < columns && newy < rows) {
                if (tiles[newy][newx].isMine) {
                    count++;
                }
            }
        }
    }
    return count;
}

void Board::toggleFlag(int x, int y, sf::RenderWindow &window) {

    if (x >= 0 && y >= 0 && x < columns && y < rows && !tiles[y][x].isRevealed) {
        tiles[y][x].isFlagged = !tiles[y][x].isFlagged;
        if (tiles[y][x].isFlagged == true)
        {
            //flagsplaced++;
            //initialFlags++;

            tiles[y][x].setposition(x*32, y*32, "images/tile_hidden.png",tiles[y][x].hidden_tile);
            window.draw(tiles[y][x].sprite);
            tiles[y][x].setposition(x*32, y*32, "images/flag.png", tiles[y][x].flagTexture);
            window.draw(tiles[y][x].sprite);
        }
        //else if (tiles[y][x].isFlagged == false)
        //{
        //    flagsplaced--;
        //}
    }
}



void Board::revealTile(int x, int y, sf::RenderWindow &window) {
    if (x < 0 || y < 0 || x >= columns || y >= rows || tiles[y][x].isRevealed || tiles[y][x].isFlagged) {
        return;
    }


    tiles[y][x].isRevealed = true;
    if (tiles[y][x].isFlagged) {
        tiles[y][x].isFlagged = false;
    }


    //tiles[y][x].setposition(x * 32, y * 32, "images/tile_revealed.png", tiles[y][x].revealed_tile);
    //tiles[y][x].setposition(x*32, y*32, "images/number_1.png", tiles[y][x].num1);



    // Draw the tile
    //window.draw(tiles[y][x].sprite);

    // Handle game over if it's a mine
    if (tiles[y][x].isMine) {
        gameOver = true;
        // Optionally: Show all mines or perform other game over actions
    } else if (tiles[y][x].adjacentMines == 0) {
        // Reveal adjacent tiles
        for (int y2 = -1; y2 <= 1; y2++) {
            for (int x2 = -1; x2 <= 1; x2++) {
                if (x2 != 0 || y2 != 0) {

                    revealTile(x + x2, y + y2, window);
                }
            }
        }
    }
}

void Board::draw(sf::RenderWindow &window) {
    Tile sample;
    sample.loadImages();

    wincondition(window);
    if (win == true)
    {

        winface.loadFromFile("images/face_win.png");
        sprite.setTexture(winface);
        sprite.setPosition(((columns * 32) / 2) - 60, rows * 32);
        window.draw(sprite);



    }

    happyface.loadFromFile("images/face_happy.png");
    test3.loadFromFile("images/test_3.png");
    test2.loadFromFile("images/test_2.png");
    test1.loadFromFile("images/test_1.png");
    debug.loadFromFile("images/debug.png");
    digits.loadFromFile("images/digits.png");


    sprite.setTexture(happyface);
    sprite.setPosition(((columns * 32) / 2) - 60, rows * 32);


    test3sprite.setTexture(test3);
    test3sprite.setPosition((columns * 32) - 60, rows * 32);

    test2sprite.setTexture(test2);
    test2sprite.setPosition((columns * 32) - 120, rows * 32);

    test1sprite.setTexture(test1);
    test1sprite.setPosition((columns * 32) - 180, rows * 32);

    debug_sprite.setTexture(debug);
    debug_sprite.setPosition((columns * 32) - 240, rows * 32);


    digits_sprite.setTexture(digits);
    digits_sprite.setPosition(10, rows * 32);


    for (int b = 0; b < rows; b++) {
        for (int a = 0; a < columns; a++) {
            string filename;
            if (tiles[b][a].isRevealed) {

                //if (newGame == true)
                //{
                //    tiles[b][a].sprite.setTexture(sample.hidden_tile);
                //    tiles[b][a].sprite.setPosition(a*32, b*32);
                //    window.draw(tiles[b][a].sprite);
                //  //tiles[b][a].setposition(a * 32, b * 32, "images/tile_hidden.png", tiles[b][a].hidden_tile);
                //}

                if (tiles[b][a].isMine) {
                    filename = "images/tile_revealed.png";
                    tiles[b][a].setposition(a * 32, b * 32, filename, sample.revealed_tile);
                    window.draw(tiles[b][a].sprite);
                    if (tiles[b][a].isMine) {

                        tiles[b][a].sprite.setTexture(sample.mineTexture);
                        tiles[b][a].sprite.setPosition(a * 32, b * 32);
                        window.draw(tiles[b][a].sprite);

                        gameover(window);

                    }


                } else if (!tiles[b][a].isMine) {

                    tiles[b][a].isFlagged = false;
                    filename = "images/tile_revealed.png";
                    tiles[b][a].setposition(a * 32, b * 32, filename, tiles[b][a].revealed_tile);
                    window.draw(tiles[b][a].sprite);
                    tiles[b][a].adjacentMines = countAdjacentMines(a, b);
                    int minesadj = tiles[b][a].adjacentMines;

                    if (minesadj > 0) {
                        tiles[b][a].sprite.setTexture(sample.numTextures[minesadj - 1]);
                        tiles[b][a].sprite.setPosition(a * 32, b * 32);
                    }
                }


            } else  {
                if (win == true)
                {

                    winface.loadFromFile("images/face_win.png");
                    sprite.setTexture(winface);
                    sprite.setPosition(((columns * 32) / 2) - 60, rows * 32);
                    window.draw(sprite);

                }

                tiles[b][a].setposition(a * 32, b * 32, "images/tile_hidden.png", tiles[b][a].hidden_tile);
                if (debugMode)
                {
                    if (tiles[b][a].isFlagged) {
                        tiles[b][a].sprite.setTexture(tiles[b][a].hidden_tile);
                        window.draw(tiles[b][a].sprite);
                        tiles[b][a].sprite.setTexture(tiles[b][a].flagTexture);
                        window.draw(tiles[b][a].sprite);
                    }
                    if (tiles[b][a].isMine)
                    {
                        tiles[b][a].sprite.setTexture(sample.revealed_tile);
                        window.draw(tiles[b][a].sprite);
                        tiles[b][a].sprite.setTexture(sample.mineTexture);
                        window.draw(tiles[b][a].sprite);

                    }


                }
                else if (tiles[b][a].isFlagged) {
                    tiles[b][a].sprite.setTexture(tiles[b][a].hidden_tile);
                    window.draw(tiles[b][a].sprite);
                    tiles[b][a].sprite.setTexture(tiles[b][a].flagTexture);
                    window.draw(tiles[b][a].sprite);
                } else {
                    tiles[b][a].sprite.setTexture(sample.hidden_tile);
                }
            }


            window.draw(tiles[b][a].sprite);


        }
    }
    //newGame = false;


    window.draw(sprite);
    window.draw(test3sprite);
    window.draw(test2sprite);
    window.draw(test1sprite);
    window.draw(debug_sprite);
    drawRemainingMines(window, minecount);

    //window.draw(digits_sprite);
}



void Board::drawRemainingMines(sf::RenderWindow &window, int remainingMines) {

    flagsplaced = 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (tiles[row][col].isFlagged) {
                flagsplaced++;
            }

        }
    }
    remainingMines -= flagsplaced;
    int xOffset = 10; // Starting x position for displaying digits
    int yOffset = window.getSize().y - digitHeight - 50; // Position to draw digits

    // Extract and draw each digit
    std::vector<int> digits;
    bool isNegative = false;
    if (remainingMines < 0) {
        isNegative = true;
        remainingMines = -remainingMines; // Make the number positive for digit extraction
    }

    if (remainingMines == 0) {
        digits.push_back(0);
    } else {
        while (remainingMines > 0) {
            digits.push_back(remainingMines % 10);
            remainingMines /= 10;
        }
        std::reverse(digits.begin(), digits.end()); // Reverse to get the correct order
    }

    if (isNegative) {
        drawDigit(window, -1, xOffset, yOffset); // Draw the negative sign as a special case
        xOffset += digitWidth; // Move to the next position for the next digit
    }

    for (int digit : digits) {
        drawDigit(window, digit, xOffset, yOffset);
        xOffset += digitWidth; // Move to the next position for the next digit
    }
}

void Board::drawDigit(sf::RenderWindow &window, int digit, int x, int y) {
    int digitIndex = digit;
    if (digit == -1) {
        digitIndex = 10; // Assuming the negative sign is at the 11th position (index 10) in the texture
    }
    sf::IntRect textureRect(digitIndex * digitWidth, 0, digitWidth, digitHeight);
    digits_sprite.setTextureRect(textureRect);
    digits_sprite.setPosition(x, y);
    window.draw(digits_sprite);
}



void Board::wincondition(sf::RenderWindow &window) {
    int counter = 0;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            if (!tiles[y][x].isMine && !tiles[y][x].isRevealed)
            {
                counter++;
            }
        }
    }
    if (counter == 0)
    {
        win = true;
    }

}


void Board::resetBoard(sf::RenderWindow &window) {
    Tile sample;
    sample.loadImages();
    win = false;
    gameOver = false;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            tiles[row][col].isRevealed = false;
            tiles[row][col].isMine = false;
            tiles[row][col].isFlagged = false;
            tiles[row][col].sprite.setTexture(sample.hidden_tile);
            window.draw(tiles[row][col].sprite);

        }
    }
    happyface.loadFromFile("images/face_happy.png");
    sprite.setTexture(happyface);
    sprite.setPosition(((columns * 32) / 2) - 60, rows * 32);
    window.draw(sprite);

}

void Board::restart(sf::RenderWindow &window) {
    //newGame = true;
    resetBoard(window);

    initializeBoard();

}

void loadConfig(const std::string& filename) {
    ifstream file(filename);

    //string col_read;
    //string row_read;
    //string mine_read;

    if (file.is_open()) {
        file >> _cols;
        file >> _rows;
        file >> _mines;

        newcols = stoi(_cols);
        newrows = stoi(_rows);
        newmines = stoi(_mines);
        //columns = stoi(col_read);
        //rows = stoi(row_read);
        //minecount = stoi(mine_read);

        //initializeBoard();
        file.close();
    }

}





void Board::loadBoard(const string& filename,sf::RenderWindow &window) {

    ifstream file(filename);
    boardload = true;
    string line;
    resetBoard(window);
    int row = 0;
    while (getline(file, line) && row <= rows) {
        for (int col = 0; col < line.size(); ++col) {

            if (line[col] == '1')
            {
                tiles[row][col].isMine = true;
            }

        }
        row++;

    }

    //rows = row;
    //columns = temp_col;
    initializeBoard();
    boardload = false;

}



//void Board::setposition(float x, float y, string filename) {
//
//    sprite.setTexture();
//    sprite.setPosition(x, y);
//}

void Board::toggleDebugMode(sf::RenderWindow &window) {


    if (debugMode == true)
    {

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                if (tiles[y][x].isMine) {
                    // Reveal mines
                    //tiles[y][x].isRevealed = true;
                    tiles[y][x].setposition(x * 32, y * 32, "images/tile_revealed.png", tiles[y][x].revealed_tile);
                    tiles[y][x].mineTexture.loadFromFile("images/mine.png");
                    tiles[y][x].sprite.setTexture(tiles[y][x].mineTexture);
                    tiles[y][x].sprite.setPosition(x * 32, y * 32);
                    window.draw(tiles[y][x].sprite);
                }
            }
        }

    }
    else if (debugMode == false) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                if (tiles[y][x].isMine) {
                    // Reveal mines
                    //tiles[y][x].isRevealed = true;
                    tiles[y][x].setposition(x * 32, y * 32, "images/tile_hidden.png", tiles[y][x].hidden_tile);

                    //tiles[y][x].sprite.setTexture(tiles[y][x].mineTexture);
                    //tiles[y][x].sprite.setPosition(x * 32, y * 32);
                    window.draw(tiles[y][x].sprite);

                }
            }
        }
    }

}





void Board::gameover(sf::RenderWindow &window)
{
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            if (tiles[y][x].isMine) {
                // Reveal mines
                tiles[y][x].isRevealed = true;
                tiles[y][x].setposition(x * 32, y * 32, "images/tile_revealed.png", tiles[y][x].revealed_tile);
                tiles[y][x].mineTexture.loadFromFile("images/mine.png");
                tiles[y][x].sprite.setTexture(tiles[y][x].mineTexture);
                tiles[y][x].sprite.setPosition(x * 32, y * 32);
                window.draw(tiles[y][x].sprite);
            } else if (tiles[y][x].isFlagged && !tiles[y][x].isMine) {
                // Remove incorrect flags
                tiles[y][x].isFlagged = false;
                tiles[y][x].setposition(x * 32, y * 32, "images/tile_hidden.png", tiles[y][x].hidden_tile);
                window.draw(tiles[y][x].sprite);
            }
        }
    }
    loseface.loadFromFile("images/face_lose.png");
    sprite.setTexture(loseface);
    sprite.setPosition(((columns * 32) / 2) - 60, rows * 32);
    window.draw(sprite);

    // Set game over state
    gameOver = true;
}

int main() {


    loadConfig("boards/config.cfg");
    newcols = stoi(_cols);
    newrows = stoi(_rows);
    newmines = stoi(_mines);
    //cout << newcols << " " << newrows << " " << newmines << " " << newcols + newrows + newmines << endl;
    int num1 = newcols;
    int num2 = newrows;
    int num3 = newmines;
    Board board(num1, num2, num3);//////try

    sf::RenderWindow window(sf::VideoMode(board.columns*32, (board.rows*32)+100), "Minesweeper");

    // Draw the board initially
    window.clear(sf::Color::White);
    board.draw(window);
    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && board.win==false && board.gameOver == false) {
                    int x = event.mouseButton.x / 32;
                    int y = event.mouseButton.y / 32;
                    board.revealTile(x, y, window);
                }
                if (event.mouseButton.button == sf::Mouse::Right && board.win==false && board.gameOver == false) {
                    int x = event.mouseButton.x / 32;
                    int y = event.mouseButton.y / 32;
                    board.toggleFlag(x, y, window);  // Corrected the call
                }


                if (event.mouseButton.x >= (board.columns * 32) - 240 && event.mouseButton.x < ((board.columns * 32)-180) &&
                    event.mouseButton.y >= board.rows * 32 && event.mouseButton.y < (board.rows * 32) + 64 && board.gameOver == false && board.win==false)
                {
                    board.debugMode = !board.debugMode;
                    board.toggleDebugMode(window);  // Call the toggle debug mode function
                }
                if (event.mouseButton.x >= ((board.columns * 32)/2) - 60 && event.mouseButton.x < ((board.columns * 32)/2) &&
                    event.mouseButton.y >= board.rows * 32 && event.mouseButton.y < (board.rows * 32) + 64)
                {
                    board.restart(window);
                }


                if (event.mouseButton.x >= (board.columns * 32) - 180 && event.mouseButton.x < ((board.columns * 32)-120) &&
                    event.mouseButton.y >= board.rows * 32 && event.mouseButton.y < (board.rows * 32) + 64)
                {
                    board.loadBoard("boards/testboard1.brd", window);

                }
                if (event.mouseButton.x >= (board.columns * 32) - 120 && event.mouseButton.x < ((board.columns * 32)-60) &&
                    event.mouseButton.y >= board.rows * 32 && event.mouseButton.y < (board.rows * 32) + 64)
                {
                    board.loadBoard("boards/testboard2.brd", window);

                }
                if (event.mouseButton.x >= (board.columns * 32) - 60 && event.mouseButton.x < ((board.columns * 32)) &&
                    event.mouseButton.y >= board.rows * 32 && event.mouseButton.y < (board.rows * 32) + 64)
                {
                    board.loadBoard("boards/testboard3.brd", window);

                }


            }
        }

        // Redraw the board after any event
        window.clear(sf::Color::White);
        board.draw(window);
        window.display();

    }

    return 0;
}
//, positi

