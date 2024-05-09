#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <optional>
#include "SFML/Graphics.hpp"
#include "board.h"
#include "move.h"


#define Length 7
#define BLACK_PAWN 1
#define WHITE_PAWN -1
#define BLACK_ROOK 2
#define WHITE_ROOK -2
#define WHITE_KNIGHT -3
#define BLACK_KNIGHT 3
#define BLACK_BISHOP 4
#define WHITE_BISHOP -4
#define BLACK_QUEEN 5
#define WHITE_QUEEN -5
#define BLACK_KING 6
#define WHITE_KING -6


using namespace std;
using namespace sf;

struct pos
{
    int x, y;
} oldPos, whiteKing, blackKing, transformWHITE, transformBlack;


int board[8][8] =
{
    2,3,4,5,6,4,3,2,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    -1,-1,-1,-1,-1,-1,-1,-1,
    -2,-3,-4,-5,-6,-4,-3,-2 };

//array 64 elements, board is 8 x
/*
#
*/

int rightWhiteRook, leftWhiteRookM, whiteKingFirstMove;
int rightBlackRook, leftBlackRookM, blackKingfirstMove;
int move; // 0-> white is moving, 1 -> black is moving
int checkWhite, checkBlack;
int transformationWhite, trasnformationBlack;
const int mastersize = 100;
int isMoving;

const int rectsize = 52;
const int screenWidth = 800, screenHeight = 800;


/// loadPosition() ///
/*
void loadPosition()
{
    int k = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int n = board[i][j];
            if (!n) continue;
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            f[k].setTextureRect(IntRect(rectsize *x, rectsize * y, rectsize, rectsize));
            f[k].setPosition(rectsize * j, rectsize * i);
            k++;
        }
    }
}// end loadPosition()
*/


//----------------------------------------------------------
// MAIN GUI FUNCTION
//---------------------------------------------------------

const string NAME = "ds_chess";
const string AUTHOR = "Justin and Cora";
//Forsyth Edwards notation for position: black pieces, empty spaces, white pieces
const string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

vector<string> split(string input, char delimiter); //function prototype


int gui_main()
{
    //std::cout << std::experimental::filesystem::v1::exists("images/chessboard2.jpg") << std::endl;    
    RenderWindow window(VideoMode(screenWidth, screenHeight), "Chess with Minimax!");
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;


    if (!t1.loadFromFile("chess_images\\chessboard2.jpg")) {
        std::cout << "fuck fuck fuck" << std::endl;
    }
    //LOAD PIECES IMAGES TO TEXTURE 
    t2.loadFromFile("chess_images\\blackpawn.png");
    t3.loadFromFile("chess_images\\blackrook.png");
    t4.loadFromFile("chess_images\\blackbishop.png");
    t5.loadFromFile("chess_images\\blackknight.png");
    t6.loadFromFile("chess_images\\blackqueen.png");
    t7.loadFromFile("chess_images\\blackking.png");
    t8.loadFromFile("chess_images\\whitepawn2.png");
    t9.loadFromFile("chess_images\\whiterook.png");
    t10.loadFromFile("chess_images\\whitebishop.png");
    t11.loadFromFile("chess_images\\whiteknight.png");
    t12.loadFromFile("chess_images\\whitequeen.png");
    t13.loadFromFile("chess_images\\whiteking.png");
    //t14.loadFromFile("chess_images\\blacktransformation.png");    //need this image
    //t15.loadFromFile("chess_images\\whitetransformation.png");    //need this image

    //initialize sprites with images
    sf::Sprite chessboard(t1);
    sf::Sprite BlackPawn(t2);
    sf::Sprite BlackRook(t3);
    sf::Sprite BlackBishop(t4);
    sf::Sprite BlackKnight(t5);
    sf::Sprite BlackQueen(t6);
    sf::Sprite BlackKing(t7);
    sf::Sprite WhitePawn(t8);
    sf::Sprite WhiteRook(t9);
    sf::Sprite WhiteBishop(t10);
    sf::Sprite WhiteKnight(t11);
    sf::Sprite WhiteQueen(t12);
    sf::Sprite WhiteKing(t13);
    //sf::Sprite WhiteTransformation(t14);  //still need image for texture
    //sf::Sprite BlackTransformation(t15);  //still need image for texture
    sf::Sprite MoveImages;

    sf::Sprite testSprite(BlackPawn);

    Sprite spArray[12] = { BlackPawn, WhitePawn, BlackBishop, WhiteBishop,
                            BlackRook, WhiteRook, BlackKnight, WhiteKnight,
                            BlackQueen, WhiteQueen, BlackKing, WhiteKing };

    const float boardscale = 0.4167;    //set board image dimensions
    float boardX = boardscale, boardY = boardscale;
    const float pscale = 0.3; //scale pieces
    float pscaleX = pscale, pscaleY = pscale;
    chessboard.setTexture(t1);
    chessboard.scale(Vector2f(boardX, boardY));
    testSprite.scale(Vector2f(pscaleX, pscaleY));

    //for (int i = 0; i < 32; i++) 
    // { f[i].setTexture(tfigures); }
    // loadPosition();
    //   int x = pos.x / mastersize;
    //   int y = pos.y / mastersize;

    bool isMove = false;
    float dx = 0, dy = 0;
    int noMovedPiece = 0;
    int n = 0;


    //enable GUI
    Event e;
    //WHILE WINDOW IS OPEN
    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        //closes window
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
        }
        ////// DRAG AND DROP ///////
        if (e.type == Event::MouseButtonPressed)
        {
            if (e.key.code == Mouse::Left)
            {
                //for (int i = 0; i < 32; i++)
                //{
                if (testSprite.getGlobalBounds().contains(pos.x, pos.y))  //transformation
                {
                    isMove = true; //n = i;
                    dx = pos.x - testSprite.getPosition().x;
                    dy = pos.y - testSprite.getPosition().y;
                }
                //} // end for
                //other type of board moving algorithm
                /*
                // (board[y][x] != 0)
                {
                    // dx = pos.x - x * mastersize;
                    // dy = pos.y - y * mastersize;
                    dx = pos.x - s.getPosition().x;
                    dy = pos.y - s.getPosition().y;
                }//end transformation
                */
            }//end LeftButton
        }
        if (e.type == Event::MouseButtonReleased)
        {
            if (e.key.code == Mouse::Left)
            {
                isMove = false;
                // Vector2f p = f[n].getPosition() + Vector2f(rectsize / 2, rectsize / 2);
                    //Vector2f newPos = Vector2f(rectsize * int(p.x / rectsize), rectsize * int(p.y / rectsize));
                    //testSprite.setPosition(newPos);
            }
        }
        if (isMove) {
            testSprite.setPosition(pos.x - dx, pos.y - dy);
        }

        //}//end while

        // sprites
        /*
        sf::Sprite chessboard;
        sf::Sprite BlackPawn(t2);
        sf::Sprite BlackRook(t3);
        sf::Sprite BlackBishop(t4);
        sf::Sprite BlackKnight(t5);
        sf::Sprite BlackQueen(t6);
        sf::Sprite BlackKing(t7);
        sf::Sprite WhitePawn(t8);
        sf::Sprite WhiteRook(t9);
        sf::Sprite WhiteBishop(t10);
        sf::Sprite WhiteKnight(t11);
        sf::Sprite WhiteQueen(t12);
        sf::Sprite WhiteKing(t13);
        //sf::Sprite WhiteTransformation(t14);  //Need the image
        //sf::Sprite BlackTransformation(t15);  //need the image
        sf::Sprite MoveImages;
        //------------------------------------------------------------
        */

        //displays board
        //------------------------------------------------------------
        window.clear();
        //float boardX = .5, boardY = .5;   //board scaling dimensions
        //chessboard.setTexture(t1);
        //chessboard.scale(Vector2f(boardX, boardY));
        //chessboard.setPosition(sf::Vector2f(1, 1.5));
        window.draw(chessboard);
        window.draw(testSprite);
        // for (int i = 0; i < 32; i++) window.draw(f[i]);  //<---need to rename f to something more readable

        window.display();
        //------------------------------------------------------------


    }//end while WINDOW



    return 0;
}//END MAIN


int tui_main() {


    std::optional<Board::Board> chessboard;

    while (true) {
        string input;
        getline(cin, input);
        vector <string> args = split(input, ' ');

        if (args[0] == "uci") {
            //do uci logic
            std::cout << "id name " << NAME << "\n";
            std::cout << "id author" << AUTHOR << std::endl;
        }//end if
        else if (args[0] == "isready") {
            cout << "readyok" << std::endl;
        }
        else if (args[0] == "position") {
            size_t i = 1;
            //do position logic
            // check if 
            if (args[i] == "startpos") {
                //set board to startpos
                i += 1;
            }
            else if (args[i] == "fen") {
                //create a board using args[2]
                string fen = args[2] + " " + args[3] + " " + args[4] + " " + args[5] + " " + args[6] + " " + args[7];
                chessboard = Board::Board(fen);
                i += 6;
            }

            if (i < args.size() && args[i] == "moves") {
                i += 1;
                while (i < args.size()) {
                    auto move = Move::Move::string_to_move(args[i]);
                    if (chessboard.has_value() && std::holds_alternative<Board::SuccessfulOperation>(
                        chessboard.value().is_valid_move(&move)
                    )) {
                        chessboard.value().make_move(&move);
                    } else {
                        continue;
                    }
                }
            }
        }//end else if "position"

        else if (args[0] == "go") {
            //go will run search function using chessboard as an input

        }
        else if (args[0] == "quit") {
            break;
        }
    } //end while
    return 0;
}


vector<string> split(string input, char delimiter) {
    vector <string> output = {};

    size_t start = 0;
    size_t index = input.find(delimiter);
    if (index != -1) {
        output.push_back(input.substr(start, index - start));
        start += index;
    }
    return output;
}