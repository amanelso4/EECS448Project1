
#include "Executive.h"
#include "player.h"
#include "display.h"
#include "machine.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

/**
 * @brief Get an int from a user between the given bounds. Repeat until successful.
 *
 * @param message A message to give the user before attempting.
 * @param lowerBound The minimum bound which the user may input.
 * @param upperBound The maximum bound the user may input.
 * @return int The integer the user selected.
 */
int getInt(string message, int lowerBound, int upperBound) {

    bool inputSuccess;
    int userInt = 0;
    string boundMsg = " (" + std::to_string(lowerBound) + " : " + std::to_string(upperBound) + ")";
    cout << message << boundMsg << ": ";

    while (true) {
        cin >> userInt;
        inputSuccess = !cin.fail();
        if (!inputSuccess) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (!inputSuccess || userInt < lowerBound || userInt > upperBound) {
            cout << "Please enter a number in" << boundMsg << ":";
        } else {
            break;
        }
    }

    return userInt;
}

/**
 * @brief Get a char from a user between the given bounds. Repeat until successful. Only allows uppercase characters.
 *
 * @param message A message to give the user before attempting.
 * @param lowerBound The minimum character bound which the user may input.
 * @param upperBound The maximum character bound the user may input.
 * @return char The character the user selected.
 */
char getChar(string message, char lowerBound, char upperBound) {

    string lowerStr(1, lowerBound);
    string upperStr(1, upperBound);

    bool inputSuccess;
    char userChar = 0;
    string boundMsg = " (" + lowerStr + " : " + upperStr + ")";
    cout << message << boundMsg << ": ";

    while (true) {
        cin >> userChar;
        inputSuccess = !cin.fail();
        if (!inputSuccess) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            userChar = toupper(userChar);
        }
        if (!inputSuccess || userChar < lowerBound || userChar > upperBound) {
            cout << "Please enter a character in" << boundMsg << ":";
        } else {
            break;
        }
    }

    return userChar;
}

/**
 * @brief Get a char from a user from the options in the given string. Repeat until successful.
 *
 * @param message A message to give the user before attempting.
 * @param options A string of characters for the user to choose between.
 * @return char The character the user selected.
 */
char getCharInOptions(string message, string options) {

    bool inputSuccess;
    char userChar = 0;
    int numOptions = static_cast<int>(options.length());
    string boundMsg = " (";
    for (int optionNum = 0; optionNum < numOptions; optionNum++) {
        boundMsg += options.substr(optionNum, 1);
        if (optionNum < numOptions - 1) {
            boundMsg +=  ", ";
        }
    }
    boundMsg += ")";
    cout << message << boundMsg << ": ";

    while (true) {
        cin >> userChar;
        inputSuccess = !cin.fail();
        if (!inputSuccess) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Please enter a character in" << boundMsg << ":";
            continue;
        } else {
            userChar = toupper(userChar);
        }

        bool found = false;
        for (int checkChar = 0; checkChar < numOptions; checkChar++) {
            if (userChar == options[checkChar]) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Please enter a character in" << boundMsg << ":";
        } else {
            break;
        }
    }

    return userChar;
}

int Executive::charToInt(char c) {return ((toupper(c) - 65));}

int Executive::numShipCoords(int shipNum)
{
	int n = 0;

	for(int i = 1; i <= shipNum; i++)
	{
		n = n+i;
	}

	return n;
}

void Executive::WaitEnter()
{
	cin.ignore();
	cout << "Press ENTER to end turn...";
	cin.get();
	for (int i = 0; i <= 50; i++) cout << endl;
}

bool Executive::validColumn(bool big, char c)
{
  if(big){
	if (!isalpha(c) || (toupper(c) < 65 || toupper(c) > 84))
	{
		cout << "Invalid input! Column must be A-T!: ";
		return false;
	}
	else
	{
		return true;
	}
  }
  else{
    if (!isalpha(c) || (toupper(c) < 65 || toupper(c) > 73))
  	{
  		cout << "Invalid input! Column must be A-I!: ";
  		return false;
  	}
  	else
  	{
  		return true;
  	}
  }
}

void Executive::run()
{
	int shipnum = 0;
  Player player1;
  Player player2;

  int row, col;
  char c_col; // char version of the column

  machine machine;

  int maxShips = 5;

  char diff = getCharInOptions("What level of difficulty do you want to play: Easy, Medium, Hard?", "E,M,H");
	machine.setDifficultyLevel(diff);

	char gamemode = getCharInOptions("Would you like to play normal Battleship or BattleshipXL?", "NX");
    if (gamemode == 'X') {
        maxShips = 10;
        m_big = true;
    }
    else
    {
      m_big = false;
    }

    bool big = m_big;

	Display display = Display(big);

    bool humanOpponent = false;

    char humanInput = getCharInOptions("Would you like to play against a Human or AI?", "HA");
    humanOpponent = (humanInput == 'H');
    if (!humanOpponent) {
        char diff = getCharInOptions("What level of difficulty do you want to play: Easy, Medium, Hard?", "EMH");
        machine.setDifficultyLevel(diff);
    	machine.setGameMode(gamemode);
    }


    shipnum = getInt("How many ships do you want to place in the grid?", 1, maxShips);

    player1.my_ships.updateNumShips(shipnum);
    player1.enemy_ships.updateNumShips(shipnum);
    player2.my_ships.updateNumShips(shipnum);
    player2.enemy_ships.updateNumShips(shipnum);

    Player* currentPlayer = &player1;
    for (int currentPlayerNum = 1; currentPlayerNum <= 2; currentPlayerNum++) {
        cout << "Player " << currentPlayerNum << "\n";

        for (int currentShip = 1; currentShip <= shipnum; currentShip++)
        {
                //blank regular Board
                if(big == false) {

                display.friendlyBoard(currentPlayer->my_ships);

                char direction = 'u'; //default direction is up

                if (currentShip == 1)
                {
                    row = getInt("Input the row in which you wish to place your 1x" + std::to_string(currentShip) + " ship", 1, 9);
                    c_col = getChar("Input the column in which you wish to place your 1x" + std::to_string(currentShip) + " ship", 'A', 'I');
                }
                else
                {
                    row = getInt("Input the row in which you wish to place your 1x" + std::to_string(currentShip) + " ship's pivot point", 1, 9);
                    c_col = getChar("Input the column in which you wish to place your 1x" + std::to_string(currentShip) + " ship's pivot point", 'A', 'I');
                    direction = getCharInOptions("Up, Down, Left, or Right from pivot? (U, D, L, R): ", "UDLR");
                }
                col = charToInt(c_col); // convert char to int
                row--; // decrement row by 1 for indexing array
                direction = toupper(direction);

                if (!currentPlayer->PlaceShip(currentShip, row, col, direction))
                {
                    cout << "Ship could not be placed there. \n";
                } else {
                    break;
                }
            }
            //blank XL Board
            else {
              display.friendlyBoard(currentPlayer->my_ships); //used to be my_ships.m_boardXL

              char direction = 'u'; //default direction is up

              if (currentShip == 1)
              {
                  row = getInt("Input the row in which you wish to place your 1x" + std::to_string(currentShip) + " ship", 1, 20);
                  c_col = getChar("Input the column in which you wish to place your 1x" + std::to_string(currentShip) + " ship", 'A', 'T');
              }
              else
              {
                  row = getInt("Input the row in which you wish to place your 1x" + std::to_string(currentShip) + " ship's pivot point", 1, 20);
                  c_col = getChar("Input the column in which you wish to place your 1x" + std::to_string(currentShip) + " ship's pivot point", 'A', 'T');
                  direction = getCharInOptions("Up, Down, Left, or Right from pivot? (U, D, L, R): ", "UDLR");
              }
              col = charToInt(c_col); // convert char to int
              row--; // decrement row by 1 for indexing array
              direction = toupper(direction);

              if (!currentPlayer->PlaceShip(currentShip, row, col, direction))
              {
                  cout << "Ship could not be placed there. \n";
              } else {
                  break;
              }
            }

        }
        if (humanOpponent) {
            cout <<"Switch to next Player!\n";
            WaitEnter();

        //print last time so player can see 1x5 ship placed
        display.friendlyBoard(currentPlayer->my_ships);
	       cin.ignore();
	        cout << "Press Enter to play!";
	         cin.get();
	          for (int i = 0; i <= 50; i++) {cout << endl;}
              currentPlayer = &player2;
        }
        else {
            break;
        }


    if(!humanOpponent)
    {
        //blank Board
        display.friendlyBoard(currentPlayer->my_ships);
        char direction = 'u'; //default direction is up


        cin.ignore();
        cout << "Press Enter to play!";
        cin.get();
        for (int i = 0; i <= 50; i++) cout << endl;


        //AI places ships:

        //print last time so player can see 1x5 ship placed
        display.friendlyBoard(currentPlayer->my_ships);



        for (int currentShip = 1; currentShip <= shipnum; currentShip++)
        {
            while(true)
            {
                char direction;
                if (currentShip == 1)
                {
                    row = machine.randomNum();
                    c_col = machine.randomChar();
                }
                else
                {
                    row = machine.randomNum();
                    c_col = machine.randomChar();
                    direction = machine.getRandomDirection();
                }
                row--;
                direction = toupper(direction);

                if (currentPlayer->PlaceShip(currentShip, row, col, direction))
                {
                    break;
                }
            }
        }
      }
    }

	int round = 0;

    currentPlayer = &player1;
    Player* otherPlayer = &player2;

	while(!player1.my_ships.allShipsSunk() && !player2.my_ships.allShipsSunk())
	{
        if (round % 2) {
            currentPlayer = &player2;
            otherPlayer = &player1;
        } else {
            currentPlayer = &player1;
            otherPlayer = &player2;
        }
        int playerNum = (round % 2) + 1;

        if (playerNum == 2 && !humanOpponent) {
            if(machine.getDifficultyLevel() == 'E'){
				//call easy methods
				cout<<"pretend AI easy level shot\n";

			}
			else if (machine.getDifficultyLevel() == 'M'){
				//call medium methods
				cout<<"pretend AI medium level shot\n";

			}
      }
			else{
				//call hard methods
				cout<<"pretend AI hard level shot\n";

			}
		round++;
		}
  }

bool Executive::checkForBig()
{
  return m_big;
}


		/*---------------test code-----------------------------------------

        cout << "Player " << playerNum << "'s turn!\n";

        cout << "You have been hit " << currentShip->getHit() << " times\n";


        cout << "You have been hit " << currentPlayer->my_ships.getNumHits() << " times\n";

        //Print boards before fire
        if(big) {
        display.matchFrame(playerNum, currentPlayer->enemy_ships.m_boardXL, currentPlayer->my_ships.m_boardXL);
        }
        else {
          display.matchFrame(playerNum, currentPlayer->enemy_ships.m_board, currentPlayer->my_ships.m_board);
        }

        while (true) {
            if(big) {
            row = getInt("Input the row into which you wish to fire", 1, 20);
            c_col = getChar("Input the column into which you wish to fire", 'A', 'T');
            }
            else {
              row = getInt("Input the row into which you wish to fire", 1, 9);
              c_col = getChar("Input the column into which you wish to fire", 'A', 'I');
            }
            col = charToInt(c_col);
            row --;

            cout << "Player " << playerNum << "'s turn!\n";
            cout << "You have been hit " << currentPlayer->my_ships.getNumHits() << " times\n";
            //Print boards before fire
            display.matchFrame(playerNum, currentPlayer->enemy_ships, currentPlayer->my_ships);

            while (true) {
                row = getInt("Input the row into which you wish to fire", 1, 9);
                c_col = getChar("Input the column into which you wish to fire", 'A', 'I');
                col = charToInt(c_col);
                row --;

                if (otherPlayer->CheckHit(row, col))
                {
                    display.hit();
                    currentPlayer->UpdateEnemyBoard(row, col, true);
                    if (otherPlayer->my_ships.allShipsSunk()){
                        cout << "Player " << playerNum << " wins!\n";
                    }
                    break;
                }
                else if(otherPlayer->my_ships.getValue(row, col) == 'X' || currentPlayer->enemy_ships.getValue(row, col) == 'O')
                {
                    cout << "\n\nYou've already fired at that spot!\n";
                }
                else
                {
                    display.miss();
                    currentPlayer->UpdateEnemyBoard(row, col, false);
                    otherPlayer->my_ships.updateBoard(row, col, 'O');
                    break;
                }
                break;
            }
            else if(otherPlayer->my_ships.getValue(row, col) == 'X' || currentPlayer->enemy_ships.getValue(row, col) == 'O')
            {
                cout << "\n\nYou've already fired at that spot!\n";
            }
            else
            {
                display.miss();
                currentPlayer->UpdateEnemyBoard(row, col, false);
                otherPlayer->my_ships.updateBoard(row, col, 'O');
                break;
            }
            round++;
            WaitEnter();
        }
	}
}*/
