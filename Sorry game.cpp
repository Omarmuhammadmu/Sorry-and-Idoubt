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
    {   //cout<<cards[i]<<" ";
        DOC.Enqueue(cards[i]);
    }
    cout << endl << "queue enqueued\n" << endl;
}

//Game's rule
void gameRules()
{
    cout << endl << "                                         Sorry game!                                         \n";

    //Rules
    cout << " Rules of the game\n" << "-------------------------------\n" << " - Each player has 4 pawns.\n";
    cout << " - If it's your first turn and you do not draw a card that lets you start a pawn out, you forfeit(skip) your turn.\n";
    cout << " - To move a pawn from your start out onto the track, you must draw either a 1 or a 2.\n" << " If it is a 2 put a pawn on the main track if it was on start and draw again. Otherwise, move pawn forward 2 steps and draw again. \n";
    cout << " - You may jump over your own or another player's pawn that's in your way, counting it as one space.\n" << " BUT... if you land on a space that's already occupied by an opponent's pawn, BUMP that pawn back to its own START space.\n";
    cout << " - 4 and 10 cards move you backwards. If you have successfully moved a pawn backwards at least two spaces beyond your own START space,\n" << " you may, on a subsequent turn, move into your own SAFETY ZONE without moving all the way around the board.\n";
    cout << endl << " Notes:\n" << "-------------------------------\n";
    cout << " - Two pawns of the same color may never occupy the same space. If your only possible move would make you land on a space already occupied\n" << " by another of your own pawns, you forfeit your turn.\n";
    cout << " - If at any time you cannot move, you forfeit your turn.\n";
    cout << " - But if at any time you can move, you must move, even if it's to your disadvantage.\n";
    cout << " - If the pawn is placed on (>), it will move either 3 or 4 steps foward (Clarified which is which on the board) on the main track bumping any opponent's pawn/s back to its/their own START space.\n";
    cout << " - Movement of pawns is in clockwise movement.\n";
    cout << " - If the drawn card was (0), then the player can choose any opponent's pawn on the main to replace its own and bump the opponet's pawn back to start \n";

    cout << " All CLEAR?!\n" << " Let's get started!\n";
    system("pause");
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

//Function to check whether there is a pawn on a particuDlar slot or not
//Return true if there is a slot is empty
bool slotChecker(board brd, int x, int y)
{
    cout << brd.b[x][y];
    if (brd.b[x][y] == '.' || brd.b[x][y] == '>' || brd.b[x][y] == '<' || brd.b[x][y] == '^' || brd.b[x][y] == 'v')
        return true;
    else
        return false;
}

//Function to return the pawn that will move
int whichtoMove(char diff, pawn active[8])
{
    while (true)
    {
        for (int i = 0; i < 8; i++)
        {
            if (active[i].s == diff)
                return i;
        }
        cout << "Please Enter a correct letter!";
        cin >> diff;
    }
}




// Function to bump the opponent's pawn to its start
bool trivialBump(pawn* check, board brd, pawn sentPawns[8])
{
   // cout << "bump is called\n";
    if (slotChecker(brd, check->x, check->y)) //do nothing if theres no char
    {
        cout << "empty space\n";
        return false;
    }
    char ch = brd.b[check->x][check->y];
    int i;
    for (i = 0; i < 8; i++)
        if (sentPawns[i].s == ch)
            break;

    if ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') && !check->red)// user pawn will return to base
    {
        user.push(sentPawns[i]);
        usize++;
    }
    else if ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W') && check->red)// computer pawn will return to base
    {
        computer.push(sentPawns[i]);
        csize++;
    }
    else
        return true;
    cout << "Pawn " << ch << " was sent home\n";
    sentPawns[i] = { NULL };
    return false;
}
void slide(pawn* check, board brd, pawn sentPawns[8], bool sm) {
    for (int i = 1; i < 4 - sm; i++)
    {
        pawn* temp = check;

        if (check->x == 15) // slide left
            temp->y -= i;
        else if (check->x == 0)
            temp->y += i; //slide right
        else if (check->y == 15)
            temp->x += i; // slide down
        else
            temp->x -= i; // slide up
      //  cout << "\n===== " << temp->x << " " << temp->y << endl;
        trivialBump(temp, brd, sentPawns);
    }
}
void movePawn(pawn*, int, board*, pawn[]);
void moveAnotherPawn(pawn* mover, int steps, board* brd, pawn sentPawns[8]) {// for user only **** needs to be adj. for computer
    if (mover->red) {
        if (user.length() >= 2)
            return; // no pawns to move
        else if (user.length() == 1) // two pawns
        {
            int i;
            for (i = 0; i < 8; i++)
                if (sentPawns[i].red && sentPawns[i].s != mover->s)
                    break;
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << "Pawn " << sentPawns[i].s << " was moved by " << steps;
        }
        else
        {
            cout << "Which Pawn will you move? \n";
            char ch;
            cin >> ch;
            while (ch == mover->s)
            {
                cout << "Please choonse another pawn \n";
                cin >> ch;
            }
            movePawn(&sentPawns[whichtoMove(ch, sentPawns)], steps, brd, sentPawns);
            cout << "Pawn " << ch << " was moved by " << steps;
        }
    }
}
//Function to move pawn on the board 
void movePawn(pawn* mover, int steps, board* brd, pawn sentPawns[8])
{
    //move inside the safe zone
    if (mover->safe)
    {
        if (mover->red) {
            if (mover->x + steps == 6)
            {
                brd->b[mover->x][2] = '.';
                DU.push(*mover);
                dusize++;
                delete mover;
            }
            else if ((mover->x + steps) < 6)
            {
                brd->b[mover->x][2] = '.';
                mover->x += steps;
                brd->b[mover->x][2] = mover->s;
            }
            else
                moveAnotherPawn(mover, steps, brd, sentPawns);
        }
        else {
            if (mover->x - steps == 10)
            {
                brd->b[mover->x][13] = '.';
                DC.push(*mover);
                dcsize++;
                delete mover;
            }
            else if ((mover->x - steps) > 10)
            {
                brd->b[mover->x][13] = '.';
                mover->x -= steps;
                brd->b[mover->x][2] = mover->s;
            }
        }
        return;
    }
    
    //enter the safe zone
    if (mover->red && mover->x < 12 && mover->y < 3 && ((-mover->x + mover->y + steps-2) > 0) && ((-mover->x + mover->y + steps-2) < 7) && !mover->safe) {
        brd->b[mover->x][mover->y] = '.';
        mover->x = -mover->x + mover->y + steps-2;
        mover->y = 2;
        brd->b[mover->x][mover->y] = mover->s;
        mover->safe = true;
        if (mover->x == 6) {
            movePawn(mover, 0, brd, sentPawns);
        }
        return;
    }
    else if (!mover->red && mover->x > 5 && mover->y > 12 && ((47 - steps - mover->x - mover->y) < 16) && ((47 - steps - mover->x - mover->y) > 9) && !mover->safe) {
        brd->b[mover->x][mover->y] = '.';
        mover->x = 47 - steps - mover->x - mover->y;
        mover->y = 13;
        brd->b[mover->x][mover->y] = mover->s;
        mover->safe = true;
        if (mover->x == 9) {
            movePawn(mover, 0, brd, sentPawns);
        }
        return;
    }
    else if ((!mover->red && mover->x > 5 && mover->y > 12) || (mover->red && mover->x < 12 && mover->y < 3)) {
        if ((-mover->x + mover->y + steps > 6) && mover->red)
        {
            moveAnotherPawn(mover, steps, brd, sentPawns);
            return;
        }
    }
    int x_cor = mover->x, y_cor = mover->y;
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

    char ch = brd->b[mover->x][mover->y];
    if (trivialBump(mover, *brd, sentPawns))
    {
        mover->y = y_cor;
        mover->x = x_cor;
        brd->b[mover->x][mover->y] = mover->s; // new indicies
        if (mover->red)cout << "Trivial move\n"; 
        moveAnotherPawn(mover, steps, brd, sentPawns);
        return;
    }
    brd->b[mover->x][mover->y] = mover->s; // new indicies

    //slider
    if ((mover->x == 0 && mover->y == 9) || (mover->x == 9 && mover->y == 15) || (mover->x == 15 && mover->y == 6) || (mover->x == 6 && mover->y == 0))
    {
        cout << "heeey3";
        brd->b[0][9] = '>';
        brd->b[6][0] = '^';
        brd->b[15][6] = '<';
        brd->b[9][15] = 'v';
        slide(mover, *brd, sentPawns, 0);
        movePawn(mover, 4, brd, sentPawns);

    }
    if ((mover->x == 0 && mover->y == 1) || (mover->x == 1 && mover->y == 15) || (mover->x == 15 && mover->y == 14) || (mover->x == 14 && mover->y == 0))
    {
        if (((mover->x == 0 && mover->y == 1) && mover->red) || ((mover->x == 15 && mover->y == 14) && !mover->red)) // return if the smaller slider has the same colour
            return;
        cout << "heeey3";
        brd->b[14][0] = '^';
        brd->b[15][14] = '<';
        brd->b[1][15] = 'v';
        brd->b[0][1] = '>';
        slide(mover, *brd, sentPawns, 1);
        movePawn(mover, 3, brd, sentPawns);
    }
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
        srand(time(NULL)); //Randomize seed initialization
        int randNum = (rand() % 40) % 4; // Generate a random number between 0 and 3

        if (randNum == 0)
        {
            return 'W';
        }
        else if (randNum == 1)
        {
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

//Move the chosen pawn
void movewhichPawn(board* brd, bool UOC, pawn sentPawns[8])
{
    if (UOC)
    {
        char selector;
        cout << "Which Pawn will you move? \n";
        cin >> selector;
        cout << endl;
        int index;
        index = whichtoMove(selector, sentPawns);
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
    }
    else
    {
        char selector;
        selector = computerdesicison(*brd, sentPawns);
        int index;
        index = whichtoMove(selector, sentPawns);
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
        cout << "The computer moved " << sentPawns[index].s << endl;
    }

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
//Card of repeating drawing card2

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

//Function of sorry card
void sorryCard(bool UOC, pawn sentPawns[8],board* brd)
{
    if (UOC)//Cases of skipping
    {    bool continuer = true;;
        if (user.length() == -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
        else if (computer.length() == 4 || (!(checker('W', sentPawns)) && !(checker('X', sentPawns)) && !(checker('Y', sentPawns)) && !(checker('Z', sentPawns))))
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
    //Execute the sorry card rule
        if (continuer)
            {
                int NOAP = 0; //Number of active pawns
                NOAP = numberofactivePawns(sentPawns, true);
                cout<<NOAP<<endl;
                if (NOAP == 4 || NOAP == 3 || NOAP == 2)
                    {
                        //Read the pawn that will be replaced from the user and check if he enters a proper one
                        char replaced;
                        cout << "Which pawn would you like to replace?\n";
                        while (true)
                            {
                                bool breaker; //break the while loop
                                cin >> replaced;
                                breaker = checker(replaced, sentPawns);
                                if (breaker)
                                    break;
                                else
                                    cout << "Please enter a correct pawn\n";
                            }
                        //Get pawn of replaced 
                        int replacedPawnindex;
                        replacedPawnindex = whichtoMove(replaced, sentPawns);
                        //Pop from user stack
                        int firstFreeindex;
                        firstFreeindex = nearfreeindex(true, sentPawns);
                        sentPawns[firstFreeindex] = user.pop();
                        usize--;
                        //initialize user x y and place on the board
                        sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
                        sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
                        brd->b[sentPawns[firstFreeindex].x][sentPawns[firstFreeindex].y] = sentPawns[firstFreeindex].s;
                        //push computer
                        computer.push(sentPawns[replacedPawnindex]);
                        csize++;
                        //delete replaced from the sentPawns array.
                        sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
                        return;
                    }
                else
                    {   //Find the only active pawn
                        cout<<"The only active pawn is executing:\n";
                        int replacedPawnindex;
                        char OAP; //Only Active Pawn(OAP)
                        for (int i = 4; i < 8; i++)
                            {
                                if (sentPawns[i].s != '\0')
                                {
                                    OAP = sentPawns[i].s;
                                    replacedPawnindex = i;
                                    cout<<OAP<<endl;
                                    break;
                                }
                            }
                        //Pop user stack
                        int firstFreeindex;
                        firstFreeindex = nearfreeindex(true, sentPawns);
                        sentPawns[firstFreeindex] = user.pop();
                        usize--;
                        //initialize user x y 
                        sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
                        sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
                        brd->b[sentPawns[firstFreeindex].x][sentPawns[firstFreeindex].y] = sentPawns[firstFreeindex].s;
                        //push computer
                        computer.push(sentPawns[replacedPawnindex]);
                        csize++;
                        //delete replaced from the activePawns array.
                        sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
                    }
                        cout<<"Sorry card rules are not satisfied so the move is skipped\n";
                    }
    }
    else
    {
        //Computer sorry card
    }
}

int getActivepawnindex(bool UOC, pawn sentPawns[8])
{
    int onlyActivepawn;
    if(UOC)
    {
        cout<<"One pawn user is executing:\n";
        for (int i = 0; i < 1; i++)
            {
                if (sentPawns[i].s != '\0')
                    {
                        onlyActivepawn = i;
                        break;}
            }
    }
    else
    {
        cout<<"One pawn user is executing:\n";
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

//Driver code
int main() {

    //gameRules();
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

    //Check length of stack
    cout << user.length() << "  " << computer.length() << endl;
    //cout<<activePawns[0].s<<"$   "<<activePawns[0].x<<"   "<<activePawns[0].y<<endl;

    //Start playing
    bool winner = true;

    while (win)
    {
        //Player turn
        //Asks the user to press sth to draw
        cout << "Press enter to draw\n";
        cin.get();
        cin.ignore();

        Draw(deckOfcards);
        cout << "You have drawn " << drawnCard << endl;

        if (user.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[0] = user.pop();
            usize--;
            placeonTrack(&activePawns[0], true, &brd);
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {
                Draw(deckOfcards);
                cout << "You have drawn " << drawnCard << endl;

                if(drawnCard != 0)//Move the pawn function
                {
                    movePawn(&activePawns[0], drawnCard, &brd, activePawns);
                brd.printBoard(brd.b);}
                else
                sorryCard(true, activePawns,&brd);
            }
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
                bool movePlace;
                if ((brd.b[0][4] == '.') && (user.length() != -1))
                {
                    cout << "Do you want to move a pawn (Enter 1)  or to place a pawn(Enter 0)?\n";
                    //Corner Case(input is either 1 or 0 only) [if input letter inifinit loop (needs to be fixed)]
                    int stopper;
                    while (true)
                    {
                        cin >> stopper;
                        if (stopper == 1)
                        {
                            movePlace = true;
                            break;
                        }
                        else if (stopper == 0)
                        {
                            movePlace = false;
                            break;
                        }
                        cout << "Please enter either 1 (to move) or 0 (to place) : ";
                    }
                    if (movePlace)
                    {
                        //Move the pawn function;
                        //Check if there is more than a pawn on board and ask for which to move
                        if (user.length() < 2)
                        {
                            movewhichPawn(&brd, true, activePawns);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                                cout << "Here2" << endl;
                                Draw(deckOfcards);
                                cout << "You have drawn " << drawnCard << endl;
                                movewhichPawn(&brd, true, activePawns);
                                brd.printBoard(brd.b);
                            }
                        }
                        else
                        {
                            cout<<"One pawn user is executing:\n";
                            int onlyActivepawn;
                            onlyActivepawn=getActivepawnindex(true,activePawns);
                            movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                                cout << "Here2" << endl;
                                Draw(deckOfcards);
                                cout << "You have drawn " << drawnCard << endl;
                                movewhichPawn(&brd, true, activePawns);
                                brd.printBoard(brd.b);
                            }
                        }
                    }
                    else
                    {
                        //Need to make the locating be varying (pop in the first free array from 0 to 3)
                        cout << "Here\n";
                        int firstFreeindex;
                        //find first free index in user
                        firstFreeindex = nearfreeindex(true, activePawns);
                        activePawns[firstFreeindex] = user.pop();
                        usize--;
                        placeonTrack(&activePawns[firstFreeindex], true, &brd);
                        brd.printBoard(brd.b);
                        if (drawnCard == 2)
                            {
                                cout << "Here2" << endl;
                                Draw(deckOfcards);
                                cout << "You have drawn " << drawnCard << endl;
                                movewhichPawn(&brd, true, activePawns);
                                brd.printBoard(brd.b);
                            }
                    }
                }
                else if (user.length() == 2 && brd.b[0][4] != '.')
                {   int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(true,activePawns);
                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        cout << "Here2" << endl;
                        Draw(deckOfcards);
                        cout << "You have drawn " << drawnCard << endl;
                        movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
                else
                {
                    movewhichPawn(&brd, true, activePawns);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        cout << "Here2" << endl;
                        Draw(deckOfcards);
                        cout << "You have drawn " << drawnCard << endl;
                        movewhichPawn(&brd, true, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
            }
            else if (drawnCard > 0)
            {
                if (user.length() < 2)
                {
                    movewhichPawn(&brd, true, activePawns);
                    brd.printBoard(brd.b);
                }
                else
                {
                    if (user.length() < 3)
                    {
                        int onlyActivepawn;
                        onlyActivepawn=getActivepawnindex(true,activePawns);
                        movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
            }
            else
            {sorryCard(true, activePawns,&brd);}
        }
//To check if all pawns of the user are pushed to the destination stack
    if (DU.length() == 3)
    {
        winner = true;
        break;
    }

//Computer turn
        Draw(deckOfcards);
        cout << "The computer has drawn " << drawnCard << endl;
        if (computer.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[4] = computer.pop();
            csize--;
            placeonTrack(&activePawns[4], false, &brd);
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {
                Draw(deckOfcards);
                cout << "The computer has drawn " << drawnCard << endl;
                //Move the pawn function
                movePawn(&activePawns[4], drawnCard, &brd, activePawns);
                brd.printBoard(brd.b);
            }
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
                bool movePlace;
                if ((brd.b[15][11] == '.') && (computer.length() != -1))
                {
                    srand(time(NULL)); //Randomize seed initialization
                    movePlace = rand() % 2; // Generate a random number between 0 and 1

                    if (movePlace)
                    {
                        //Move the pawn function;
                        //Check if there is more than a pawn on board and decide which to move
                        //*********************************************************************
                        if (computer.length() < 2)
                        {
                            movewhichPawn(&brd, false, activePawns);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                                cout << "Here2" << endl;
                                Draw(deckOfcards);
                                cout << "The computer has drawn " << drawnCard << endl;
                                movewhichPawn(&brd, false, activePawns);
                                brd.printBoard(brd.b);
                            }
                        }
                        else
                        {
                            cout<<"One pawn computer is executing:\n";
                            int onlyActivepawn;
                            onlyActivepawn=getActivepawnindex(false,activePawns);
                            movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                            brd.printBoard(brd.b);
                            if (drawnCard == 2)
                            {
                                cout << "Here2" << endl;
                                Draw(deckOfcards);
                                cout << "The computer has drawn " << drawnCard << endl;
                                movewhichPawn(&brd, false, activePawns);
                                brd.printBoard(brd.b);
                            }
                        }
                    }
                    else
                    {
                        cout << "Here4\n";
                        //Need to make the locating be varying (pop in the first free array from 4 to 7)
                        cout << "Here\n";
                        int firstFreeindex;
                        //find first free index in user
                        firstFreeindex = nearfreeindex(false, activePawns);
                        activePawns[firstFreeindex] = computer.pop();
                        csize--;
                        placeonTrack(&activePawns[firstFreeindex], false, &brd);
                        brd.printBoard(brd.b);
                        if (drawnCard == 2)
                            {
                                cout << "Here3" << endl;
                                Draw(deckOfcards);
                                cout << "The computer has drawn " << drawnCard << endl;
                                movewhichPawn(&brd, false, activePawns);
                                brd.printBoard(brd.b);
                            }
                    }
                }
                else if (computer.length() == 2 && brd.b[15][11] != '.')
                {
                    int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(false,activePawns);
                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        cout << "Here3" << endl;
                        Draw(deckOfcards);
                        cout << "The computer has drawn " << drawnCard << endl;
                        movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                        brd.printBoard(brd.b);
                    }

                }
                else
                {
                    movewhichPawn(&brd, false, activePawns);
                    brd.printBoard(brd.b);
                    if (drawnCard == 2)
                    {
                        cout << "Here3" << endl;
                        Draw(deckOfcards);
                        cout << "The computer has drawn " << drawnCard << endl;
                        movewhichPawn(&brd, false, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
            }
            else if (drawnCard > 0)
            {
                if (computer.length() < 2)
                {
                    movewhichPawn(&brd, false, activePawns);
                    brd.printBoard(brd.b);
                }
                else
                {
                    if (computer.length() < 3)
                    {
                        int onlyActivepawn;
                        onlyActivepawn=getActivepawnindex(false,activePawns);
                        movePawn(&activePawns[4], drawnCard, &brd, activePawns);
                        brd.printBoard(brd.b);
                    }
                }
            }
            //else   drawn zero case
        }

        //Useless
        bool h;
        cout << "out?";
        cin >> h;
        if (h)
            break;
        //To check if all pawns of the computer are pushed to the destination stack
        if (DC.length() == 3)
        {
            winner = false;
            break;
        }
    }
    //Annoucing the winner
    if (winner)
        cout << "Congrats, You won\n";
    else
        cout << "Hard luck, the computer won\n";
    bool exit;
    cout << endl << "would you like to play again or to exit? (If again enter 1 if not enter 0)\n";
    cin >> exit;
}



//Functions that need to be written

//Sorry card
//Push in DU,DC & Entering Safe road
//test bump & bump while sliding






