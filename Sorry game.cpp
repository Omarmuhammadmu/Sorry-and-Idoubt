#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <time.h>
#include <windows.h>
using namespace std;

//Global variables
bool win = true;
int drawnCard;
int usize = 4, csize = 4, dusize = 0, dcsize = 0;
#define MAX 4

//pawn implementation
struct pawn
{
    char s; //symbol
    int x, y; //Coordinates on the board
    bool red, safe;
};

//Queue implementation
struct Queue {
    int front, rear, capacity;
    int* queue;
    Queue(int c)
    {
        front = rear = 0;
        capacity = c;
        queue = new int;
    }
    ~Queue() { delete[] queue; }
    void Enqueue(int data)
    {
        // insert element at the rear
        queue[rear] = data;
        rear++;
    }
    int Dequeue()
    {
        int drawn = queue[front];
        // shift all the elements from index 2 till rear
        // to the left by one
        for (int i = 0; i < rear - 1; i++) {
            queue[i] = queue[i + 1];
        }
        // decrement rear
        rear--;
        return drawn;
    }
    bool isEmpty()
    {
        return (front == rear);
    }
};

//Stack implementation (stack of pawns)
class Stack {
    int top;
public:
    pawn stack[MAX];
    Stack() { top = -1; }

    bool push(pawn x);
    bool isEmpty();
    pawn pop();
    int length()
    {
        return top;
    }
};
bool Stack::push(pawn x)
{
    if (top >= (MAX - 1)) {
        cout << "Stack Overflow";
        return false;
    }
    else {
        stack[++top] = x;
        return true;
    }
}
pawn Stack::pop()
{
    pawn x = stack[top--];
    return x;
}
bool Stack::isEmpty()
{
    return (top < 0);
}

Stack user, computer, DU, DC;
Queue DOC(45);   //Deck of cards (DOC)

//Board implementation
class board
{
public:
    char b[17][17];
    board()
    {
        for (int i = 0; i < 16; i++) {
            b[i][0] = b[i][15] = b[0][i] = b[15][i] = '.';
        }
        for (int i = 1; i < 15; i++)
            for (int j = 1; j < 15; j++)
                b[i][j] = ' ';
        for (int i = 1; i < 7; i++)
        {
            b[i][2] = b[i + 8][13] = '.';
            b[2][15 - i] = b[13][i] = '.';
        }
        b[1][4] = 'R';
        b[14][11] = 'Y';
        b[0][1] = b[0][9] = '>';
        b[14][0] = b[6][0] = '^';
        b[15][14] = b[15][6] = '<';
        b[9][15] = b[1][15] = 'v';
        b[11][1] = 'G';
        b[4][14] = 'B';
        b[7][2] = b[8][13] = char(0 + 48);
        b[2][4] = b[13][11] = char(4 + 48);
    }
    void printBoard(char arr[17][17]);
};
void board::printBoard(char arr[17][17])
{
    b[7][2] = char(dusize + 48);
    b[8][13] = char(dcsize + 48);
    b[2][4] = char(usize + 48);
    b[13][11] = char(csize + 48);
    for (int i = 0; i < 16; i++)
    {
        cout << "                 ";
        for (int j = 0; j < 16; j++)
        {
            cout << arr[i][j] << ' ';
            if ((i == 0 && ((j < 4) || (j > 7 && j < 13))) || (i < 8 && (j == 1 || j == 3)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            else if ((j == 14 && ((i != 0 && i < 5) || (i > 8 && i < 14))) || ((i == 2 || i == 4) && (j > 7 && j < 15)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            else if ((i == 15 && ((j > 9 && j < 14) || (j > 0 && j < 6))) || (i > 7 && (j == 12 || j == 10)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
            else if ((j == 15 && ((i > 9 && i < 14) || (i > 0 && i < 6))) || ((i == 11 || i == 13) && (j < 7)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout << endl;
    }
    //Print the board for Online Compiler
    //     for(int i=0; i<16; i++)
    //         {
    //         cout<<"                 ";
    //         for(int j=0; j<16; j++)
    //         cout<< arr[i][j]<<' ';
    //         cout<<endl;}
}

// Shuffle cards
void shuffleCards(int arr[], int n)
{
    // To obtain a time-based seed
    unsigned seed = 0;
    // Shuffling our array
    shuffle(arr, arr + n, default_random_engine(seed));
}

//Deck of cards implementation
class deck
{
public:
    int cards[45];
    int index = 0;
    deck()
    {
        while (index < 5)
        {
            cards[index] = 1;
            index++;
        }

        for (int i = 0; i < 4; i++)
        {
            for (int card = 2; card < 13; card++)
            {

                if (card == 9 || card == 6)
                {
                    continue;
                }
                cards[index] = card;
                index++;
            }
            cards[index] = 0;
            index++;
        }
        //Shuffle the cards in the beginning of the game.
        int n = sizeof(cards) / sizeof(cards[0]);
        srand(time(0));
        int ranTimes = rand() % 10;
        for (int shuffles = 0; shuffles < ranTimes; shuffles++)
        {
            shuffleCards(cards, n);
        }
    }
    void queueDeck();
};
void deck::queueDeck()
{
    for (int i = 0; i < 45; i++)
    {
        DOC.Enqueue(cards[i]);
    }
    cout << endl << "The deck of cards is shuffles, You can draw now!\n" << endl;
}

//Game's rule
void gameRules()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    cout<<endl<<endl;
cout<<"  $$$$$$\\   $$$$$$\\  $$$$$$$\\  $$$$$$$\\ $$\\     $$\\        $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\\n" 
<<"$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\\\$$\\   $$  |      $$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|\n"
<<"$$ /  \\__|$$ /  $$ |$$ |  $$ |$$ |  $$ |\\$$\\ $$  /       $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      \n"
<<"\\$$$$$$\\  $$ |  $$ |$$$$$$$  |$$$$$$$  | \\$$$$  /        $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    \n"
 <<" \\____$$\\ $$ |  $$ |$$  __$$< $$  __$$<   \\$$  /         $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|   \n"
<<"$$\\   $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |   $$ |          $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |      \n"
<<"\\$$$$$$  | $$$$$$  |$$ |  $$ |$$ |  $$ |   $$ |          \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\ \n"
 <<" \\______/  \\______/ \\__|  \\__|\\__|  \\__|   \\__|           \\______/ \\__|  \\__|\\__|     \\__|\\________|\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    
    //Rules
    cout<<endl<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB);

    cout << " Rules of the game\n" ;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    cout<< "-------------------------------\n" << " - Each player has 4 pawns.\n";
    cout << " - If it's your first turn and you do not draw a card that lets you start a pawn out, you forfeit(skip) your turn.\n";
    cout << " - To move a pawn from your start out onto the track, you must draw either a 1 or a 2.\n" << " If it is a 2 put a pawn on the main track if it was on start and draw again. Otherwise, move pawn forward 2 steps and draw again. \n";
    cout << " - You may jump over your own or another player's pawn that's in your way, counting it as one space.\n" << " BUT... if you land on a space that's already occupied by an opponent's pawn, BUMP that pawn back to its own START space.\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB);
    cout << endl << " Notes:\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout<< "-------------------------------\n";
    cout << " - Two pawns of the same color may never occupy the same space. If your only possible move would make you land on a space already occupied\n" << " by another of your own pawns, you forfeit your turn.\n";
    cout << " - If at any time you cannot move, you forfeit your turn.\n";
    cout << " - But if at any time you can move, you must move, even if it's to your disadvantage.\n";
    cout << " - If the pawn is placed on (>), it will move either 3 or 4 steps foward (Clarified which is which on the board) on the main track bumping any opponent's pawn/s back to its/their own START space.\n";
    cout << " - Movement of pawns is in clockwise movement.\n";
    cout << " - If the drawn card was (0), then the player can choose any opponent's pawn on the main to replace its own and bump the opponet's pawn back to start \n";
    cout << " All CLEAR?!\n" << " Let's get started!\n";
    system("pause");
    cout << " -----------------------------------------------------------------------------------------------";

}
//Game functions 
//--------------------------------------------------------------------------------------------------
//Function to Draw cards and check the deck queue(if empty shuffle the cards and queue it again)
void Draw(deck deckOfcards)
{
    drawnCard = DOC.Dequeue();
    if (DOC.isEmpty())
    {
        DOC.front = DOC.rear = 0;
        int n = sizeof(deckOfcards.cards) / sizeof(deckOfcards.cards[0]);
        shuffleCards(deckOfcards.cards, n);
        deckOfcards.queueDeck();
    }
}

//Function to place pawn on the main track
void placeonTrack(pawn* placer, bool selector, board* brd)
{
    if (selector)
    {
        placer->x = 0;
        placer->y = 4;
        brd->b[placer->x][placer->y] = placer->s;
        cout << endl;
    }
    else
    {
        placer->x = 15;
        placer->y = 11;
        brd->b[placer->x][placer->y] = placer->s;
        cout << endl;
    }
}

// delete a pawn from the active pawn after pushing in stack
void delPawnfrmArr(char ch, pawn sentPawns[8]) {
    int i;
    for (i = 0; i < 8; i++)
        if (sentPawns[i].s == ch)
            break;
    sentPawns[i] = { '\0',-1,-1,false,false };
    return;
}

//Function to check whether there is a pawn on a particuDlar slot or not
//Return true if there is a slot is empty
bool slotChecker(board brd, int x, int y)
{
    if (brd.b[x][y] == '.' || brd.b[x][y] == '>' || brd.b[x][y] == '<' || brd.b[x][y] == '^' || brd.b[x][y] == 'v')
        return true;
    else
        return false;
}

//Function to return the index of pawn that will move
int whichtoMove(char diff, pawn active[8])
{
    while (true)
    {
        for (int i = 0; i < 8; i++)
        {
            if (active[i].s == diff)
                return i;
        }
        cout << "Please Enter a correct letter!\n";
        cin >> diff;
    }
}

// Function to bump the opponent's pawn to its start
bool trivialBump(pawn* check, board brd, pawn sentPawns[8])
{
   // cout << "bump is called\n";
    if (slotChecker(brd, check->x, check->y)) //do nothing if theres no char
        return false;
    char ch = brd.b[check->x][check->y];
    int i;
    for (i = 0; i < 8; i++)
        if (sentPawns[i].s == ch)
            break;
    brd.b[sentPawns[i].x][sentPawns[i].y] = '.';
    if ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') && !check->red)// user pawn will return to base
    {
        sentPawns[i]= {.s=sentPawns[i].s,.x=1, .y=4, .red=true, .safe =false };
        user.push(sentPawns[i]);
        usize++;
    }
    else if ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W') && check->red)// computer pawn will return to base
    {
        sentPawns[i]= {.s=sentPawns[i].s,.x=14, .y=11, .red=false, .safe =false };
        computer.push(sentPawns[i]);
        csize++;
    }
    else
        return true;
    cout << "Pawn " << ch << " was sent home\n";
    delPawnfrmArr(ch, sentPawns);
    return false;
}
void slideBump(pawn* check, board *brd, pawn sentPawns[8], bool sliding) {
    if (slotChecker(*brd, check->x, check->y)) //do nothing if theres no char
        return ;
    //Char of the pawn that should be bumped
    char ch = brd->b[check->x][check->y];
    //Get the index of the symbol
    int i;
    for (i = 0; i < 8; i++)
        {if (sentPawns[i].s == ch)
            {break;}}
    if ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D'))// user pawn will return to base
    {
        user.push(sentPawns[i]);
        usize++;
    }
    else if ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W'))// computer pawn will return to base
    {
        computer.push(sentPawns[i]);
        csize++;}
    brd->b[sentPawns[i].x][sentPawns[i].y] = '.';
    cout << "Pawn " << ch << " was sent home\n";
    delPawnfrmArr(ch, sentPawns);
    return;
}

void slide(pawn check, board* brd, pawn sentPawns[8], bool sm) {
    for (int i = 1; i < 4 - sm; i++)
    {
        pawn temp = check;
        if (check.x == 15) // slide left
            {temp.y -= i;}
        else if (check.x == 0)
            {temp.y += i;} //slide right
        else if (check.y == 15)
            {temp.x += i;} // slide down
        else
            {temp.x -= i;} // slide up

        slideBump(&temp, brd, sentPawns,true);
        
    }
}

void movePawn(pawn*, int, board*, pawn[]);
bool checker(char toCheck, pawn sentPawns[8]);
//To check whether the pawn is movable or not
bool isMovable(pawn mover, int steps, board* brd)
{
    if (mover.safe)
    {
            if ((mover.x + steps <= 6 && mover.red) || (mover.x - steps >= 9 && !mover.red))
                return true;
            else 
                return false;
    }
    if (mover.x == 0 && ((mover.y + steps) <= 15)) // upper horizontal
    {
        mover.y += steps;
    }
    else if (mover.x == 0 && ((mover.y + steps) > 15))
    {
        mover.x = (mover.y + steps - 15);
        mover.y = 15;
    }
    else if ((mover.x + steps) <= 15 && mover.y == 15) // right vertical
    {
        mover.x += steps;
    }
    else if ((mover.x + steps) > 15 && mover.y == 15)
    {
        mover.y = 15 - (mover.x + steps - 15);
        mover.x = 15;
    }
    else if (mover.x == 15 && ((mover.y - steps) >= 0)) // lower horizontal
    {
        mover.y -= steps;
    }
    else if (mover.x == 15 && ((mover.y - steps) < 0))
    {
        mover.x = 15 + (mover.y - steps);
        mover.y = 0;
    }
    else if ((mover.x - steps) >= 0 && mover.y == 0) // left vertical
    {
        mover.x -= steps;
    }
    else if ((mover.x - steps) < 0 && mover.y == 0)
    {
        mover.y = steps - mover.x;
        mover.x = 0;
    }
   
   char ch = brd->b[mover.x][mover.y];
   if(slotChecker(*brd, mover.x, mover.y) || (( ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') && !mover.red) || ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W') && mover.red))
   {
       return true;
   }
   else
   {
       return false;
   }
}

// Return number of movable pawns in the active pawns
int possibleMove(bool UOC, int steps, board* brd, pawn sentPawns[8])
{
    int num = 0;
    if (UOC)
    {
        for (int i = 0; i < 4; i++)
        {   if(sentPawns[i].s !='\0')
            {
                if (isMovable(sentPawns[i], steps, brd))
                {
                    num++;
                }
            }
            {continue;}
        }
    }
    else 
    {
        for (int i = 4; i < 8; i++) //return false if any computer pawn is not in safe road
        {   if(sentPawns[i].s !='\0')
            {   
                if (isMovable(sentPawns[i], steps, brd))
                {
                    num++;
                }
            }
        }    
    }
    return num;
}

void moveAnotherPawn(pawn* mover, int steps, board* brd, pawn sentPawns[8]) {// for user only **** needs to be adj. for computer
    
    if (possibleMove(mover->red, steps, brd, sentPawns))
        return;

    if (mover->red) 
    {
        if (user.length() >= 2)
            return; // no pawns to move
        else if (user.length() == 1) // two pawns
        {
            //Find another pawn to move
            int i;
            for (i = 0; i < 4; i++)
                if (sentPawns[i].s != mover->s)
                    break;
            //infinte loop when ther is two cons. pawns on the safe road****
            if(!isMovable(sentPawns[i],steps,brd))
                return;
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << "Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
        }
        else
        {
            cout << "Which Pawn will you move? \n";
            char ch;
            bool break1 = false;
            while (true)
            {
                cin >> ch;
                for (int i = 0; i < 4; i++)
                {
                    if (sentPawns[i].s == ch && sentPawns[i].s != mover->s && isMovable(sentPawns[i],steps,brd))
                        {
                            break1 = true;
                            break;
                        }
                }
                if(true)
                break;
                
                cout << "Please choonse another pawn \n";
            }
            movePawn(&sentPawns[whichtoMove(ch, sentPawns)], steps, brd, sentPawns);
            cout << " Pawn " << ch << " was moved by " << steps<<endl;  //Recursion happens (Pawn C was moved by 12
                                                                                            //Pawn A was moved by 12)
            return;
        }
    }
    else
    {
        if (computer.length() >= 2)
            return; // no pawns to move
        else if (computer.length() == 1) // two pawns
        {
            //Find another pawn to move
            int i;
            for (i = 4; i < 8; i++)
                if (sentPawns[i].s != mover->s)
                    break;
            if(!isMovable(sentPawns[i],steps,brd))
                return;
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << " Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
        }
        else
        {
            //More than two pawns
            //Find next pawn in the array
            int i;
            for (i = 4; i < 8; i++)
                {
                    if (sentPawns[i].s != mover->s && sentPawns[i].s != '\0' &&isMovable(sentPawns[i], steps,brd))
                        break;
                    if(i ==7)
                        return;
                }
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << "Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
        }
    }
}

void trivialSeq(pawn* check, board *brd, pawn sentPawns[8], int x,int y ,int steps,bool isSliding) {
   if(isSliding)
        {return;}
    if (trivialBump(check, *brd, sentPawns))
    {
        check->y = y;
        check->x = x;
        brd->b[check->x][check->y] = check->s; // new indicies
        if (check->red)cout << "Trivial move\n";
        moveAnotherPawn(check, steps, brd, sentPawns);
        return;
    }
}

//Function to move pawn on the board 
void movePawn(pawn* mover, int steps, board* brd, pawn sentPawns[8])
{ 
    bool isSliding = false;
    int x_cor = mover->x, y_cor = mover->y;
    //move inside the safe zone
    if (mover->safe)
    {
        if (mover->red) {
            if (mover->x + steps == 6)
            {
                if (mover->x == 0 && mover->y == 1 )
                {brd->b[mover->x][mover->y] = '>';}
                else
                {brd->b[mover->x][2] = '.';}
                DU.push(*mover);
                dusize++;
                delPawnfrmArr(mover->s, sentPawns);
            }
            else if ((mover->x + steps) < 6)
            {
                if (mover->x == 0 && mover->y == 1 )
                {brd->b[mover->x][mover->y] = '>';}
                else{
                brd->b[mover->x][2] = '.';}
                mover->x += steps;
                trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false);
                brd->b[mover->x][2] = mover->s;
            }
            else
                moveAnotherPawn(mover, steps, brd, sentPawns);
        }
        else {
            if (mover->x - steps == 9)
            {
                brd->b[mover->x][13] = '.';
                DC.push(*mover);
                dcsize++;
                delPawnfrmArr(mover->s, sentPawns);
            }
            else if ((mover->x - steps) > 9)
            {
                brd->b[mover->x][13] = '.';
                mover->x -= steps;
                trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false);
                brd->b[mover->x][13] = mover->s;
            }
            else
                moveAnotherPawn(mover, steps, brd, sentPawns);
        }
        return;
    }
    //*********************************************************************
    //enter the safe zone
    if (mover->red && mover->x < 11 && mover->y < 3 && ((-mover->x + mover->y + steps - 2) > -1) && ((-mover->x + mover->y + steps - 2) < 7) && !mover->safe) {
        brd->b[mover->x][mover->y] = '.';
        mover->x = -mover->x + mover->y + steps - 2;
        mover->y = 2;
        brd->b[mover->x][mover->y] = mover->s;
        //if(mover->x >0)
        mover->safe = true;
        if (mover->x == 6) {
            movePawn(mover, 0, brd, sentPawns);
        }
        return;
    }
    else if (!mover->red && mover->x > 4 && mover->y > 12 && ((47 - steps - mover->x - mover->y) < 16) && ((47 - steps - mover->x - mover->y) > 8) && !mover->safe) {
        brd->b[mover->x][mover->y] = '.';
        mover->x = 47 - steps - mover->x - mover->y;
        mover->y = 13;
        brd->b[mover->x][mover->y] = mover->s;
        if(mover->y == 13 && mover->x <15)
        {mover->safe = true;}
        if (mover->x == 9) {
            movePawn(mover, 0, brd, sentPawns);
        }
        return;
    }
    else if ((!mover->red && mover->x > 5 && mover->y > 12) || (mover->red && mover->x < 12 && mover->y < 3)) {
        if ((-mover->x + mover->y + steps -2 > 6) && mover->red)
        {
            moveAnotherPawn(mover, steps, brd, sentPawns);
            return;
        }
        else if ((47 - steps - mover->x - mover->y<9) && !mover->red) {
            moveAnotherPawn(mover, steps, brd, sentPawns);
            return;
        }
    }
    if (mover->x == 15 && mover->y == 14)
        brd->b[mover->x][mover->y] = '<';
    else if (mover->x == 0 && mover->y == 1 )
        brd->b[mover->x][mover->y] = '>';
    else
    brd->b[mover->x][mover->y] = '.';

    //move on board boundaries
    if (mover->x == 0 && ((mover->y + steps) <= 15)) // upper horizontal
    {
        mover->y += steps;
    }
    else if (mover->x == 0 && ((mover->y + steps) > 15))
    {
        mover->x = (mover->y + steps - 15);
        mover->y = 15;
    }
    else if ((mover->x + steps) <= 15 && mover->y == 15) // right vertical
    {
        mover->x += steps;
    }
    else if ((mover->x + steps) > 15 && mover->y == 15)
    {
        mover->y = 15 - (mover->x + steps - 15);
        mover->x = 15;
    }
    else if (mover->x == 15 && ((mover->y - steps) >= 0)) // lower horizontal
    {
        mover->y -= steps;
    }
    else if (mover->x == 15 && ((mover->y - steps) < 0))
    {
        mover->x = 15 + (mover->y - steps);
        mover->y = 0;
    }
    else if ((mover->x - steps) >= 0 && mover->y == 0) // left vertical
    {
        mover->x -= steps;
    }
    else if ((mover->x - steps) < 0 && mover->y == 0)
    {
        mover->y = steps - mover->x;
        mover->x = 0;
    }

    //char ch = brd->b[mover->x][mover->y];
    if (mover->x == 0 && mover->y == 1 && mover->red){
    brd->b[mover->x][mover->y] = mover->s; // new indicies
    return;
    }
    else if (mover->x == 15 && mover->y == 14 && !mover->red){
    brd->b[mover->x][mover->y] = mover->s; // new indicies
    return;
    }
    
    //slider
    //long slider (slide by 4)
    if ((mover->x == 0 && mover->y == 9) || (mover->x == 9 && mover->y == 15) || (mover->x == 15 && mover->y == 6) || (mover->x == 6 && mover->y == 0))
    {
        isSliding = true;
        slide(*mover, brd, sentPawns, 0);
        movePawn(mover, 4, brd, sentPawns);
        brd->b[0][9] = '>';
        brd->b[6][0] = '^';
        brd->b[15][6] = '<';
        brd->b[9][15] = 'v';
    }
    //short slider (slide by 3)
    if ((mover->x == 0 && mover->y == 1) || (mover->x == 1 && mover->y == 15) || (mover->x == 15 && mover->y == 14) || (mover->x == 14 && mover->y == 0))
    {
        if (((mover->x == 0 && mover->y == 1) && mover->red) || ((mover->x == 15 && mover->y == 14) && !mover->red)) // return if the smaller slider has the same colour
            return;
        
        isSliding = true;
        slide(*mover, brd, sentPawns, 1);
        movePawn(mover, 3, brd, sentPawns);
        brd->b[14][0] = '^';
        brd->b[15][14] = '<';
        brd->b[1][15] = 'v';
        brd->b[0][1] = '>';
    }
    trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,isSliding);
    brd->b[mover->x][mover->y] = mover->s; // new indicies
}

//Function to check active pawns[among computer pawns]
bool checker(char toCheck, pawn sentPawns[8])
{
    for (int i = 4; i < 8; i++)
    {
        if (sentPawns[i].s == toCheck)
            return true;
    }
    return false;
}

//Funcion that returns computer decision
char computerdesicison(board brd, pawn sentPawns[8])
{
    bool returner;
    while (true)
    {
        srand(time(NULL));              //Randomize seed initialization
        int randNum = (rand() % 40) % 4; // Generate a random number between 0 and 3

        if (randNum == 0)
        {
            returner = checker('W', sentPawns);
            if (returner)
            return 'W';
        }
        else if (randNum == 1)
        {
            returner = checker('X', sentPawns);
            if (returner)
            return 'X';
        }
        else if (randNum == 2)
        {
            returner = checker('Y', sentPawns);
            if (returner)
                return 'Y';
        }
        else
        {
            returner = checker('Z', sentPawns);
            if (returner)
                return 'Z';
        }
    }

}

//Function to get the only active pawn's index
int getActivepawnindex(bool UOC, pawn sentPawns[8])
{
    int onlyActivepawn;
    if(UOC)
    {
        for (int i = 0; i < 4; i++)
            {
                //Choose inside the safe road or outside
                if (sentPawns[i].s != '\0')
                    {
                        onlyActivepawn = i;
                        break;}
                
            }
    }
    else
    {
        for (int i = 4; i < 7; i++)
            {
                if (sentPawns[i].s != '\0')
                    {
                        onlyActivepawn = i;
                        break;}
            }
    }
    return onlyActivepawn;
}

//Function to return the number of active pawns
int numberofactivePawns(pawn sentPawns[8], bool UOC)
{
    int NOAP = 0; //Number of active pawns
    if (UOC)
    {
        for (int i = 4; i < 8; i++)
        {
            if (sentPawns[i].s == 'W' || sentPawns[i].s == 'X' || sentPawns[i].s == 'Y' || sentPawns[i].s == 'Z')
            {
                NOAP++;
            }
        }
        return NOAP;
    }
    else
    {
        //Computer
        for (int i = 0; i < 4; i++)
        {
            if (sentPawns[i].s == 'A' || sentPawns[i].s == 'B' || sentPawns[i].s == 'C' || sentPawns[i].s == 'D')
            {
                NOAP++;
            }
        }
        return NOAP;
    }
}

//Function to return the index of nearest replacable user pawn 
int moveACW(board brd, pawn sentPawns[8], bool UOC)
{
    if(UOC)
    {
        for (int i = 0; i < 4; i++)
            {
                for (int j = 15; j >=0; j--)
                {
                if(i == 0)      //Right vertical
                    {if (brd.b[j][15] == 'W' || brd.b[j][15] == 'X'|| brd.b[j][15] == 'Y'|| brd.b[j][15] == 'Z')
                        {return 1;}
                        continue;}
                else if(i == 1) //Upper horizontal
                        {if ((15 - j) != 3)
                            {
                                if (brd.b[0][j] == 'W' || brd.b[0][j] == 'X'|| brd.b[0][j] == 'Y'|| brd.b[0][j] == 'Z')
                                    {return 2;}
                                continue;
                            }
                            continue;}
                else if(i == 2) //Left vertical
                        {
                            if (brd.b[15- j][0] == 'W' || brd.b[15- j][0] == 'X'|| brd.b[15- j][0] == 'Y'|| brd.b[15- j][0] == 'Z')
                            {return 3;}
                            continue;}
                else            //Lower horizontal
                   {
                       if(brd.b[15][15- j] == 'W' || brd.b[15][15- j] == 'X'|| brd.b[15][15- j] == 'Y'|| brd.b[15][15- j] == 'Z')
                        {return 4;}
                        continue;}               
                }
            }
    }
    else
    { 
        for (int i = 0; i < 4; i++)
        {for (int j = 15; j >=0; j--)
            {
                if(i == 0)      //Right vertical
                    {
                        if (brd.b[j][15] == 'A' || brd.b[j][15] == 'B'|| brd.b[j][15] == 'C'|| brd.b[j][15] == 'D')
                            {return whichtoMove(brd.b[j][15],sentPawns);}
                            continue;}
                else if(i == 1) //Upper horizontal
                    {
                        if (brd.b[0][j] == 'A' || brd.b[0][j] == 'B'|| brd.b[0][j] == 'C'|| brd.b[0][j] == 'D')
                        {return whichtoMove(brd.b[0][j],sentPawns);}
                        continue;}
                else if(i == 2) //Left vertical
                    {
                        if (brd.b[15- j][0] == 'A' || brd.b[15- j][0] == 'B'|| brd.b[15- j][0] == 'C'|| brd.b[15- j][0] == 'D')
                        {return whichtoMove(brd.b[15- j][0],sentPawns);}
                        continue;} 
                else            //Lower horizontal
                    {
                        if (15- j != 12)
                        {
                            if(brd.b[15][15- j] == 'A' || brd.b[15][15- j] == 'B'|| brd.b[15][15- j] == 'C'|| brd.b[15][15- j] == 'D')
                                {return whichtoMove(brd.b[15][15- j],sentPawns);}
                                continue;
                        }
                        continue;}
            }               
        }
    }
    return -1;
}

//Move the chosen pawn
void movewhichPawn(board* brd, bool UOC, pawn sentPawns[8],int steps)
{
    if (UOC)
    {
        int i;        
        if(possibleMove(UOC,steps,brd,sentPawns)==1)
        {
            for(i=0;i<4;i++)
            {
                if(isMovable(sentPawns[i],steps,brd))
                {break;}
            }
        movePawn(&sentPawns[i], drawnCard, brd, sentPawns);
        cout<< "You have moved"<<sentPawns[i].s <<endl<<endl;
        }
        else if(possibleMove(UOC,steps,brd,sentPawns)==0)
        {
            return; //Skip the move
        }
        else
        {
        char selector;
        int index;
        cout << "Which Pawn will you move? \n";
        while(true)
        {   cin >> selector;
            if(selector=='A'||selector=='B' || selector=='C' ||selector=='D')
            {   index = whichtoMove(selector, sentPawns);
                if(isMovable(sentPawns[index],steps,brd))
                {break;}
                else
                {cout<< "Please enter a movable pawn: \n";}
            }
            else
            {cout<< "Please enter a correct pawn: \n";
                continue;}
        }
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
        cout<< "You have moved "<<sentPawns[index].s <<endl<<endl;}
    } 
    else
    {
        int i;
        if(possibleMove(UOC,steps,brd,sentPawns)==1)
            {
                for(i=4;i<8;i++)
                {
                    if(isMovable(sentPawns[i],steps,brd))
                    {break;}
                }
                movePawn(&sentPawns[i], drawnCard, brd, sentPawns);
                cout<<" The computer has moved " <<sentPawns[i].s <<endl<<endl;
            }
        else if(possibleMove(UOC,steps,brd,sentPawns)==0)
            {
                return; //Skip the move
            }
        else
        {
            char selector;
            int index;
        while(true)
        {
            selector = computerdesicison(*brd, sentPawns);
            index = whichtoMove(selector, sentPawns);
            if(isMovable(sentPawns[index],steps,brd))
            {break;}
        }
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
        cout << "The computer moved " << sentPawns[index].s << endl;}
    }

}

//Function to return the first free index of the activePawns
int nearfreeindex(bool UOC, pawn sentPawns[8])
{   int index;
    if(UOC)
    {
        for(int i=0; i<4; i++)
        {
            if(sentPawns[i].s == '\0')
            {index=i;
            break;}
        }
    }
    else
    for(int i=4; i<8; i++)
        {
            if(sentPawns[i].s == '\0')
            {index=i;
            break;}
        }
    return index;
}

//Function that check if the number of pawns is either 3 or 2 and are in the safe road
bool checkPawnsonsafe(int NOAP,pawn sentPawns[8])
{
    int terminator= 0; //Terminates the sorry card if all three are in safe road
    for(int i=0; i<4;i++)
    {
        if(sentPawns[i].safe)
            terminator++;
    }
    if(terminator == NOAP)
        return true;
    else 
        return false;
}

//Function of sorry card(Computer turn not implemented)
void sorryCard(bool UOC, pawn sentPawns[8],board* brd)
{   bool continuer = true;
    bool allSafe = true;
    if (UOC)//Cases of skipping
    {   //Cases of skipping 
        for(int i =0; i<=4; i++)
            {
                if(i==4)
                break;
                if(!sentPawns[i].safe)
                {
                    allSafe = false;
                    break;
                }
            }
        if (user.length() == -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
                return;
            }
        //Skip if there is no active opponent pawn (They are either unpoped from computer stack or they are all 4 in safe road) 
        else if (computer.length() == 3 || allSafe || moveACW(*brd,sentPawns,true) == -1 )
            {
                continuer = false;
                cout<<"The card is useless\n";
                return;
            }
    //Execute the sorry card rule
        if (continuer)
            {   int replacedPawnindex;
                bool pawnIsinsaferoad;
                int NOAP = 0; //Number of active pawns
                NOAP = numberofactivePawns(sentPawns, true);
                if (NOAP == 4 || NOAP == 3 || NOAP == 2)
                    {
                        //Handling the corner case of if the number of pawns is either 3 or 2 and are in the safe road
                        if(NOAP == 3)
                        {
                            if(checkPawnsonsafe(NOAP,sentPawns))
                            {cout<<"The sorry card is useless Sorry!\n";
                                return;} 
                        }
                        else if(NOAP == 2)
                        {
                            if(checkPawnsonsafe(NOAP,sentPawns))
                            {cout<<"The sorry card is useless Sorry!\n";
                                return;} 
                        }
                        //Read the pawn that will be replaced from the user and check if he enters a proper one
                        char replaced;
                        cout << "Which pawn would you like to replace?\n";
                        //Read replaced pawn and check if valid
                        while (true)
                            {
                                cin >> replaced;
                                if (checker(replaced, sentPawns))
                                {
                                    replacedPawnindex = whichtoMove(replaced, sentPawns);
                                    pawnIsinsaferoad = sentPawns[replacedPawnindex].safe; 
                                    if(!pawnIsinsaferoad)
                                        break;
                                    else
                                    cout << "Please enter a correct pawn\n";
                                }
                                else
                                    cout << "Please enter a correct pawn\n";
                            }
                    }
                else
                    {   //Find the only active paw
                        char OAP; //Only Active Pawn(OAP)
                        replacedPawnindex = getActivepawnindex(false, sentPawns);
                        OAP = sentPawns[replacedPawnindex].s;
                        pawnIsinsaferoad = sentPawns[replacedPawnindex].safe; 
                    }
                if(!pawnIsinsaferoad)
                    {   //Pop user stack
                        int firstFreeindex;
                        firstFreeindex = nearfreeindex(true, sentPawns);
                        sentPawns[firstFreeindex] = user.pop();
                        usize--;
                        //Assign user x and y coordinates and place on the board 
                        sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
                        sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
                        brd->b[sentPawns[replacedPawnindex].x][sentPawns[replacedPawnindex].y] = sentPawns[firstFreeindex].s;
                        //push computer
                        sentPawns[replacedPawnindex]= {.s=sentPawns[replacedPawnindex].s,.x=14, .y=11, .red=false, .safe =false };
                        computer.push(sentPawns[replacedPawnindex]);
                        csize++;
                        cout<<" You have replaced "<<sentPawns[replacedPawnindex].s<<" by "<< sentPawns[firstFreeindex].s<<" pawn\n"<<endl;
                        cout<<" Pawn "<<sentPawns[replacedPawnindex].s << " was sent home.\n";
                        //delete replaced from the activePawns array.
                        sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
                        movePawn(&sentPawns[firstFreeindex],0,brd,sentPawns);
                        brd->printBoard(brd->b);
                        return;
                    }
                    cout<<"Sorry card doesn't replace a pawn in the safe road\n";
                    return;
            }
        }
    else
    {
        //Computer sorry card
        //Cases of skipping 
        for(int i =4; i<=8; i++)
            {
                if(i==8)
                break;
                if(!sentPawns[i].safe)
                {
                    allSafe = false;
                    break;
                }
            }

        if (computer.length() == -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
            //Skip if there is no active opponent pawn (They are either unpoped from computer stack or they are all 4 in safe road)
        else if (user.length() == 3 || allSafe || moveACW(*brd,sentPawns,false)== -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
        //Execute the sorry card rule
        if (continuer)
        {
            int replacedPawnindex = moveACW(*brd,sentPawns,false);
            //cout<<"The computer replaced: \n"<<sentPawns[replacedPawnindex].s<<endl<<endl;
            //Pop user stack
            int firstFreeindex;
            firstFreeindex = nearfreeindex(false, sentPawns);
            sentPawns[firstFreeindex] = computer.pop();
            csize--;
            //Assign user x and y coordinates and place on the board 
            sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
            sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
            brd->b[sentPawns[replacedPawnindex].x][sentPawns[replacedPawnindex].y] = sentPawns[firstFreeindex].s;
            //push User 
            sentPawns[replacedPawnindex]= {.s=sentPawns[replacedPawnindex].s,.x=1, .y=4, .red=true, .safe =false };
            user.push(sentPawns[replacedPawnindex]);
            usize++;
            cout<<" The computer replaced "<<sentPawns[replacedPawnindex].s<<" by "<< sentPawns[firstFreeindex].s<<" pawn\n"<<endl;
            cout<<" Pawn "<<sentPawns[replacedPawnindex].s << " was sent home.\n";
            //delete replaced from the activePawns array.
            sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
            movePawn(&sentPawns[firstFreeindex],0,brd,sentPawns);
            brd->printBoard(brd->b);
            return;

        }
        
    }
}

bool isOpponentchar(char toCheck, bool UOC)
{
    char LUC; //Last user computer (LUC)
    if(UOC)
    {LUC = 'Z';}
    else
    {LUC = 'D';}
    for(int i=0; i<4; i++)
    {
        if(toCheck == LUC)
            {return true;}
        LUC--;
    }
    return false;
}

//Function to hand if the drawn card is 2
void twoCard (bool UOC,pawn sentPawns[8],board* brd, deck deckOfcards)
{
    cout<<" Drawing again due to the drawn card is 2. \n";
    Draw(deckOfcards);
    if(UOC)
    {
        cout << "You have drawn " << drawnCard << endl;}
    else
    {
        cout << "The computer has drawn " << drawnCard << endl;
    }
    if((drawnCard != 0) && (drawnCard != 2)||(drawnCard !=1))//Move the pawn function
        {
                if((user.length() == 2 && UOC)  || (computer.length() == 2 && !UOC) )
                {
                    int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(UOC,sentPawns);
                    movePawn(&sentPawns[onlyActivepawn], drawnCard, brd, sentPawns);
                    if(UOC)
                    {
                        cout<<" You have moved " << sentPawns[onlyActivepawn].s <<endl;}
                    else
                    {
                        cout<<" The computer has moved " << sentPawns[onlyActivepawn].s <<endl;
                    }
                    
                }
                else
                {
                    movewhichPawn(brd, UOC, sentPawns,drawnCard);
                }
                brd->printBoard(brd->b);
                return;
        }
    else if(drawnCard == 0)
        {
            sorryCard(UOC,sentPawns,brd);
        return;}
    else if(drawnCard == 2)
        {   twoCard(UOC,sentPawns,brd,deckOfcards);
        return;
        }
    else
    {           bool movePlace = false;
                if (((brd->b[0][4] == '.')|| isOpponentchar((brd->b[0][4]),true)) && (user.length() != -1))
                {
                    cout << "Do you want to move a pawn (Enter 1)  or to place a pawn(Enter 0)?\n";
                    int stopper;
                    char dummy;
                    cin >> dummy;
                    while (dummy != 48 && dummy != 49) 
                    {
                        cout << "Please enter either 1 (to move) or 0 (to place) : ";
                        cin >> dummy;
                    }
                    stopper = (int)dummy - 48;
                    if(stopper == 1)
                    {movePlace = true;}
                    else
                    {movePlace = false;}
                        //Move a pawn if the user chose to move and there must be a movable pawn on board for user 
                    if (movePlace && possibleMove(UOC,drawnCard,brd,sentPawns)!=0 )
                    {
                        //Move the pawn function;
                        //Check if there is more than a pawn on board and ask for which to move
                        if (user.length() < 2) //More than two pawns on board
                        {
                            movewhichPawn(brd, UOC, sentPawns, drawnCard);
                            brd->printBoard(brd->b);
                        }
                        else
                        {
                            int onlyActivepawn;
                            onlyActivepawn=getActivepawnindex(UOC,sentPawns);
                            movePawn(&sentPawns[onlyActivepawn], drawnCard, brd, sentPawns);
                            cout<<"You have moved" <<sentPawns[onlyActivepawn].s<<endl<<endl;
                            brd->printBoard(brd->b);
                        }
                        if(drawnCard == 2)
                            {
                                twoCard(UOC,sentPawns,brd,deckOfcards);
                            }
                    }
                }
    }
}

//Driver code
int main() {

    gameRules();
    //Initlaizing Board, the deck of cards and 4 pawns for each player.
    board brd;
    deck deckOfcards;
    deckOfcards.queueDeck();
    brd.printBoard(brd.b);
    pawn activePawns[8] = { NULL };
    char uc = 'D';
    char cc = 'Z';
    usize = 0, csize = 0;
    for (int i = 0; i < 4; i++)
    {
        pawn u = { uc--,  1,  4, true, false };
        pawn c = { cc--,  14, 11, false, false };
        user.push(u);
        computer.push(c);
        usize++;
        csize++;
    }
    //Start playing
    bool winner = true;
    while (win)
    {
        //Player turn
        //Asks the user to press sth to draw
        cout << "Press enter to draw\n"<<endl;
        cin.get();
        cin.ignore();

        Draw(deckOfcards);
        cout << "You have drawn " << drawnCard << endl;
        if (user.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[0] = user.pop();
            usize--;
            pawn dummy = { activePawns[0].s,  0,  4, true, false };
            trivialBump(&dummy,brd,activePawns);
            cout<<"You have added " <<activePawns[0].s<<" to the board\n"<<endl; 
            placeonTrack(&activePawns[0], true, &brd);
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {twoCard(true,activePawns,&brd,deckOfcards);}
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
                bool movePlace = false;
                if (((brd.b[0][4] == '.')|| isOpponentchar((brd.b[0][4]),true)) && (user.length() != -1))
                {
                    cout << "Do you want to move a pawn (Enter 1)  or to place a pawn(Enter 0)?\n";
                    int stopper;
                    char dummy;
                    cin >> dummy;
                    while (dummy != 48 && dummy != 49) 
                    {
                        cout << "Please enter either 1 (to move) or 0 (to place) : ";
                        cin >> dummy;
                    }
                    stopper = (int)dummy - 48;
                    if(stopper == 1)
                    {movePlace = true;}
                    else
                    {movePlace = false;}
                        //Move a pawn if the user chose to move and there must be a movable pawn on board for user 
                    if (movePlace && possibleMove(true,drawnCard,&brd,activePawns)!=0 )
                    {
                        //Move the pawn function;
                        //Check if there is more than a pawn on board and ask for which to move
                        if (user.length() < 2) //More than two pawns on board
                        {
                            movewhichPawn(&brd, true, activePawns, drawnCard);
                            brd.printBoard(brd.b);
                        }
                        else
                        {
                            int onlyActivepawn;
                            onlyActivepawn=getActivepawnindex(true,activePawns);
                            movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                            cout<<" You have moved" <<activePawns[onlyActivepawn].s<<endl<<endl;
                            brd.printBoard(brd.b);
                        }
                        if(drawnCard == 2)
                            {
                                twoCard(true,activePawns,&brd,deckOfcards);
                            }
                    }
                    else
                    {
                        //Need to make the locating be varying (pop in the first free array from 0 to 3)
                        int firstFreeindex;
                        //find first free index in user
                        firstFreeindex = nearfreeindex(true, activePawns);
                        activePawns[firstFreeindex] = user.pop();
                        usize--;
                        pawn dummy = { activePawns[firstFreeindex].s,  0,  4, true, false };
                        cout<<"You have added " <<activePawns[0].s<<" to the board\n"<<endl;
                        trivialBump(&dummy,brd,activePawns);
                        placeonTrack(&activePawns[firstFreeindex], true, &brd);
                        brd.printBoard(brd.b);
                        if (drawnCard == 2)
                            {
                                twoCard(true,activePawns,&brd,deckOfcards);
                            }
                    }
                }
                else if (user.length() == 2 && brd.b[0][4] != '.')
                {   int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(true,activePawns);
                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                    cout<<" You have moved "<<activePawns[onlyActivepawn].s<<endl<<endl;
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        twoCard(true,activePawns,&brd,deckOfcards);
                    }
                }
                else
                {
                    movewhichPawn(&brd, true, activePawns,drawnCard);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        twoCard(true,activePawns,&brd,deckOfcards);
                    }
                }
            }
            else if (drawnCard > 0)
            {
                if (user.length() < 2) //More than two pawns
                {
                    movewhichPawn(&brd, true, activePawns,drawnCard);
                    brd.printBoard(brd.b);
                }
                else
                {
                    if (user.length() == 2)
                    {
                        int onlyActivepawn;
                        onlyActivepawn=getActivepawnindex(true,activePawns);
                        if(isMovable(activePawns[onlyActivepawn],drawnCard,&brd))
                            {cout<<"You have moved "<<activePawns[onlyActivepawn].s<<endl<<endl;
                            movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);}
                        else {cout<<activePawns[onlyActivepawn].s<<" cannot be moved\n"<<endl;}
                        brd.printBoard(brd.b);
                    }
                }
            }
            else
            {sorryCard(true, activePawns,&brd);
}
        }
//To check if all pawns of the user are pushed to the destination stack
    if (DU.length() == 3)
    {
        winner = true;
        break;
    }

//Computer turn
        Draw(deckOfcards);
        cout << "The computer has drawn " << drawnCard << endl<<endl;
        if (computer.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[4] = computer.pop();
            activePawns[4].x =15;
            activePawns[4].y =11;
            csize--;
            pawn dummy = { activePawns[4].s,  15,  14, false, false };
            trivialBump(&activePawns[4],brd,activePawns);
            cout<<"The computer has added " <<activePawns[4].s<<" to the board\n"<<endl; 
            placeonTrack(&activePawns[4], false, &brd);
            //Bump user if exist**************************************************
            //Test if there is a user pawn on the start of the computer 
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {
                twoCard(false,activePawns,&brd,deckOfcards);
            }
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
                bool movePlace;
                if (((brd.b[15][11] == '.') || isOpponentchar((brd.b[15][11]),false)) && (computer.length() != -1))
                {
                    srand(time(NULL)); //Randomize seed initialization
                    movePlace = rand() % 2; // Generate a random number between 0 and 1

                    if (movePlace && possibleMove(false,drawnCard,&brd,activePawns)!=0)
                    {
                        //Move the pawn function;
                        //Check if there is more than a pawn on board and decide which to move
                        if (computer.length() < 2)
                        {
                            movewhichPawn(&brd, false, activePawns,drawnCard);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                               twoCard(false,activePawns,&brd,deckOfcards);
                            }
                        }
                        else
                        {
                            int onlyActivepawn;
                            onlyActivepawn=getActivepawnindex(false,activePawns);
                            cout<<" The computer has moved " <<activePawns[onlyActivepawn].s <<endl<<endl;
                            movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                                twoCard(false,activePawns,&brd,deckOfcards);
                            }
                        }
                    }
                    else
                    {
                        //Need to make the locating be varying (pop in the first free array from 4 to 7)
                        int firstFreeindex;
                        //find first free index in user
                        firstFreeindex = nearfreeindex(false, activePawns);
                        activePawns[firstFreeindex] = computer.pop();
                        csize--;
                        pawn dummy = { activePawns[firstFreeindex].s,  15, 11, false, false };
                        cout<<"The computer has added " <<activePawns[firstFreeindex].s<<" to the board\n"<<endl; 
                        trivialBump(&dummy,brd,activePawns);
                        placeonTrack(&activePawns[firstFreeindex], false, &brd);
                        brd.printBoard(brd.b);
                        if (drawnCard == 2)
                            {
                                twoCard(false,activePawns,&brd,deckOfcards);
                            }
                    }
                }
                else if (computer.length() == 2 && brd.b[15][11] != '.')
                {
                    int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(false,activePawns);
                    cout<<" The computer has moved " <<activePawns[onlyActivepawn].s <<endl<<endl;
                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        twoCard(false,activePawns,&brd,deckOfcards);
                    }
                }
                else
                {
                    movewhichPawn(&brd, false, activePawns,drawnCard);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        twoCard(false,activePawns,&brd,deckOfcards);
                    }
                }
            }
            else if (drawnCard > 0)
            {
                if (computer.length() < 2)
                {
                    movewhichPawn(&brd, false, activePawns,drawnCard);
                    brd.printBoard(brd.b);
                }
                else
                {
                    if (computer.length() == 2)
                    {
                        int onlyActivepawn;
                        onlyActivepawn=getActivepawnindex(false,activePawns);
                        cout<<" The computer has moved " <<activePawns[onlyActivepawn].s <<endl<<endl;
                        movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
            }
            else
            {
                sorryCard(false, activePawns,&brd);
            }
        }
        //To check if all pawns of the computer are pushed to the destination stack
        if (DC.length() == 3)
        {
            winner = false;
            break;
        }
    }
    //Annoucing the winner
    if (winner)
        {
            cout<<endl<<endl;
            cout<< " $$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$\\   $$$$$$\\ $$$$$$$$\\  $$$$$$\\            $$\\     $$\\  $$$$$$\\  $$\\   $$\\       $$\\      $$\\  $$$$$$\\  $$\\   $$\\ $$\\\n" 
            <<"$$  __$$\\ $$  __$$\\ $$$\\  $$ |$$  __$$\\ $$  __$$\\ $$  __$$\\\\__$$  __|$$  __$$\\           \\$$\\   $$  |$$  __$$\\ $$ |  $$ |      $$ | $\\  $$ |$$  __$$\\ $$$\\  $$ |$$ |\n"
            <<"$$ /  \\__|$$ /  $$ |$$$$\\ $$ |$$ /  \\__|$$ |  $$ |$$ /  $$ |  $$ |   $$ /  \\__|           \\$$\\ $$  / $$ /  $$ |$$ |  $$ |      $$ |$$$\\ $$ |$$ /  $$ |$$$$\\ $$ |$$ |\n"
            <<"$$ |      $$ |  $$ |$$ $$\\$$ |$$ |$$$$\\ $$$$$$$  |$$$$$$$$ |  $$ |   \\$$$$$$\\              \\$$$$  /  $$ |  $$ |$$ |  $$ |      $$ $$ $$\\$$ |$$ |  $$ |$$ $$\\$$ |$$ |\n"
            <<"$$ |      $$ |  $$ |$$ \\$$$$ |$$ |\\_$$ |$$  __$$< $$  __$$ |  $$ |    \\____$$\\              \\$$  /   $$ |  $$ |$$ |  $$ |      $$$$  _$$$$ |$$ |  $$ |$$ \\$$$$ |\\__|\n"
            <<"$$ |  $$\\ $$ |  $$ |$$ |\\$$$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |  $$ |   $$\\   $$ |              $$ |    $$ |  $$ |$$ |  $$ |      $$$  / \\$$$ |$$ |  $$ |$$ |\\$$$ |    \n"
            <<"\\$$$$$$  | $$$$$$  |$$ | \\$$ |\\$$$$$$  |$$ |  $$ |$$ |  $$ |  $$ |   \\$$$$$$  |$$\\           $$ |     $$$$$$  |\\$$$$$$  |      $$  /   \\$$ | $$$$$$  |$$ | \\$$ |$$\\\n" 
            <<" \\______/  \\______/ \\__|  \\__| \\______/ \\__|  \\__|\\__|  \\__|  \\__|    \\______/ $  |          \\__|     \\______/  \\______/       \\__/     \\__| \\______/ \\__|  \\__|\\__|\n"
            <<"                                                                               \\_/                                                                                  \n";
        }
    else
        cout << "Hard luck, the computer won\n";
}
