#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <time.h>

using namespace std;

#define MAX 52
bool gameOver = false;

// Stack implementation
class Stack
{
    int top;
    int n;

public:
    int stack[MAX];
    Stack()
    {
        top = -1;
        n = 0;
    }

    bool push(int x);
    bool isEmpty();
    int pop();
    int peek();
    int amount();
    void display();
};
bool Stack::push(int x)
{
    if (top >= (MAX - 1))
    {
        cout << "Stack Overflow";
        return false;
    }
    else
    {
        stack[++top] = x;
        n++;
        return true;
    }
}
int Stack::pop()
{
    if (top < 0)
    {
        cout << "Stack Underflow";
        return 0;
    }
    else
    {
        int x = stack[top--];
        n--;
        return x;
    }
}
int Stack::peek()
{
    if (top < 0)
    {
        cout << "Stack is Empty";
        return 0;
    }
    else
    {
        int x = stack[top];
        return x;
    }
}
bool Stack::isEmpty()
{
    return (top < 0);
}
int Stack::amount()
{
    return n;
}
void Stack::display()
{
    cout << "[ ";
    for (int i = 0; i < n; i++)
    {
        cout << stack[i] << " ";
    }
    cout << "]\n";
}

enum Suit
{
    Clubs,
    Spades,
    Diamonds,
    Hearts
};
enum Rank
{
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    king,
    queen,
    jack
};

struct card
{
    Suit suit;
    Rank rank;
};

// fun to check over played state (3 true plays on row).
bool overPlayed(char *plays)
{
    int n = 0;
    int size = (sizeof(plays) / sizeof(plays[0]));
    for (int i = 0; i < size - 1; i++)
    {
        n = 0;
        for (int j = i; j < i + 3; j++)
        {
            if (plays[j] == 't' || plays[j] == 'T')
                n++;
            if (n == 3)
                return true;
        }
    }
    return false;
}

// function to get correct input
// this function takes one arg .int to differntiate between truth,fake play and yes,no play. 0 for truth or fake and 1 for yes and no.
char readChar(int play)
{
    char ch;
    cin >> ch;
    if (play == 0)
    {
        while (ch != 't' && ch != 'T' && ch != 'f' && ch != 'F')
        {
            cout << "Error: Please enter valid character. \n";
            cin >> ch;
        }
        return ch;
    }
    else if (play == 1)
    {
        while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
        {
            cout << "Error: Please enter valid character. \n";
            cin >> ch;
        }
        return ch;
    }
}

// fun used to generate cards name out of their int number
string cardNameGen(int n)
{
    if (n == 0)
        return "one";
    if (n == 1)
        return "two";
    if (n == 2)
        return "three";
    if (n == 3)
        return "four";
    if (n == 4)
        return "five";
    if (n == 5)
        return "six";
    if (n == 6)
        return "seven";
    if (n == 7)
        return "eight";
    if (n == 8)
        return "nine";
    if (n == 9)
        return "ten";
    if (n == 10)
        return "king";
    if (n == 11)
        return "queen";
    if (n == 12)
        return "jack";
    return "nothing";
}

// fun used to print welcome screen
void printWelcomeScreen()
{
    cout << "  \n\nWelcome to IDoubt Game!\n----------------------------------------\n";
    cout << "  Game rules :\n- deck of cards is been shuffled then been distributed between two players.\n";
    cout << "- every player can peek only to the top card of his cards.";
    cout << "- you play your card on its back and decide play it with truth or lie(fake)." << endl;
    cout << "- every player can only play three true plays on row." << endl;
    cout << "- every player can only play 100 plays in the whole game. after that the game does end with tie." << endl;
    cout << "- once the card is been played the other can doubt about his play or don't." << endl;
    cout << "- if he did doubt. it's been checked if it was played with truth or lie and the one who got it wrong takes all the cards on the ground.\n";
    cout << "- if the other didn't doubt he plays the top of his cards with truth or lie(fake), and it keeps going. " << endl;
    cout << "- the first one who has no cards in his hand is the winner! \n";
    cout << "  ALL CLEAR ? LET'S PLAY!\n\n\n";
}

// Shuffle cards
void shuffleCards(int arr[], int n)
{

    // To obtain a time-based seed
    unsigned seed = 0;

    // Shuffling the array
    shuffle(arr, arr + n,
            default_random_engine(seed));
}

// Declaring the deck of cards
class deck
{
public:
    int cards[52];
    int index = 0;

    deck()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                cards[index] = j;
                index++;
            }
        }

        int n = sizeof(cards) / sizeof(cards[0]);
        srand(time(0));
        int ranTimes = rand() % 10;
        for (int shuffles = 0; shuffles < ranTimes; shuffles++)
        {
            shuffleCards(cards, n);
        }
    }
    void printingCards();
};

// function used to print the cards generated in the deck
void deck::printingCards()
{
    for (int k = 0; k < 52; k++)
    {
        cout << cards[k] << ' ';
    }
    cout << '\n'
         << index << endl;
}

// used to rearrange the cards
void rearrangeCards(Stack &player1, Stack &cptr, Stack &restOfCards, bool isPlayer1)
{

    // isPlayer1 is true when he will take the cards

    // aray used for shuffling the stack
    int shuffle[52];

    int restOfCardsSize = restOfCards.amount();
    for (int i = 0; i < restOfCardsSize; i++)
    {
        if (isPlayer1)
        {
            player1.push(restOfCards.pop());
        }
        else
        {
            cptr.push(restOfCards.pop());
        }
    }

    // now need to shuffle this
    int amount;
    if (isPlayer1)
    {
        amount = player1.amount();
    }
    else
    {
        amount = cptr.amount();
    }

    for (int i = 0; i < amount; i++)
    {
        if (isPlayer1)
        {
            shuffle[i] = player1.pop();
        }
        else
        {
            shuffle[i] = cptr.pop();
        }
    }

    int n = sizeof(shuffle) / sizeof(shuffle[0]);
    shuffleCards(shuffle, amount);

    for (int i = 0; i < amount; i++)
    {
        if (isPlayer1)
        {
            player1.push(shuffle[i]);
        }
        else
        {
            cptr.push(shuffle[i]);
        }
    }
}

// fun used to check doubt
int checkDoubt(char x)
{
    if (x == 't' || x == 'T')
    {
        return 1;
    }
    else if (x == 'f' || x == 'F')
    {
        return 0;
    }
    return 0;
}

// function used to generate rando decision
int randDecision()
{
    unsigned int time_ui = static_cast<unsigned int>(time(NULL) % 1000);
    int x = rand() % 4;
    return x;
}

// main play fun
void play()
{
    char playerPlays[100];      // used to check over played state
    bool firstRound = true;     // used to check if it was first round
    char cptrPlays[100];        // used to check over played state
    int playerPlaysCounter = 0; // count player plays
    int cptrPlaysCounter = 0;   // count Ai plays
    deck deckOfcards;           // intitalize deck instance
    Stack restOfCards;          // represents the cards on the ground
    Stack player1;              // intialize player1 stack
    Stack cptr;                 // intialize cptr stack
    int decision1;              // var used in rand decision maker
    int decision2;              // var used in rand decision maker
    int turn = 0;               // var used to decide whose turn is it.
    char player1State;          // var used to check if cptr plays truth or fake
    char cptrState;             // var used to check if cptr plays truth or fake
    int playerPeek;             // used to get the top of player stack
    int cptrPeek;               // used to get the top of Ai stack

    // welcome screen
    printWelcomeScreen();

    // assigning values for players
    for (int i = 0; i < 26; i++)
    {
        player1.push(deckOfcards.cards[i]);
        cptr.push(deckOfcards.cards[51 - i]);
    }

    while (!gameOver)
    {
        if (restOfCards.amount() == 0 && firstRound)
        {
            firstRound = false; // so we don't acess here twice
            playerPeek = player1.peek();
            cout << "- Game Started! \n- you have " << cardNameGen(playerPeek) << " play it with truth or fake? enter t for truth , f for fake." << endl;
            player1State = readChar(0);
            // add the state to the array
            playerPlays[playerPlaysCounter] = player1State;
            playerPlaysCounter++;
            // push the top of player 1 to the ground
            cout << "- you have played " << cardNameGen(playerPeek) << "!\n";
            restOfCards.push(player1.pop());
            cout << "- You have " << player1.amount() << " Cards. cptr have " << cptr.amount() << endl;
            turn = 1;
        }
        else
        {
            if (turn == 0)
            {
                // player1 turn
                cout << "- do you wanna doubt about his play? enter y for yes n for no \n";
                char doubt; // player doubt or don't
                doubt = readChar(1);
                if (doubt == 'y' || doubt == 'Y')
                {
                    if (checkDoubt(cptrState) == 1)
                    {
                        // player takes restOfCards
                        rearrangeCards(player1, cptr, restOfCards, false);
                        cout << "- you was right. computer takes all the cards on the ground. you have " << player1.amount() << " cards. and computer have " << cptr.amount() << ".\n";
                    }
                    else
                    {
                        // ai takes restOfCards
                        rearrangeCards(player1, cptr, restOfCards, true);
                        cout << "- you wasn't right. you take all the cards on the ground. you have " << player1.amount() << " cards. and computer have " << cptr.amount() << ".\n";
                    }
                }
                playerPeek = player1.peek();
                cout << "- you have " << cardNameGen(playerPeek) << " play it with truth or fake? enter t for truth , f for fake." << endl;
                player1State = readChar(0);
                // add the state to the array
                playerPlays[playerPlaysCounter] = player1State;
                playerPlaysCounter++;
                if (overPlayed(playerPlays)) // check if over played
                {
                    cout << "**note you can only play 3 true states on row. so your play state changed to fake.\n";
                    player1State = 'f';
                    // loop made to reintialze plays array with all 'f'
                    for (int i = 0; i < 100; i++)
                    {
                        playerPlays[i] = 'f';
                    }
                }

                // push the top of player 1 to the ground
                cout << "- you have played " << cardNameGen(playerPeek) << "!\n";
                restOfCards.push(player1.pop());
                cout << "- You have " << player1.amount() << " Cards. cptr have " << cptr.amount() << endl;
                turn = 1;
            }
            else if (turn == 1)
            {
                // cptr  turn
                // computer decides to play or fake
                decision1 = randDecision(); // to doubt or not
                decision2 = randDecision(); // to play or fake
                if (decision1 == 1)
                {
                    // ai decides to doubt
                    cout << "- Ai decides to doubt!\n";
                    if (checkDoubt(player1State) == 1)
                    {
                        // ai takes restOfCards
                        rearrangeCards(player1, cptr, restOfCards, false);
                        cout << "- he didnt get it right computer takes all the cards on the ground. you have " << player1.amount() << " cards. and computer have " << cptr.amount() << ".\n";
                    }
                    else
                    {
                        // player takes restOfCards
                        rearrangeCards(player1, cptr, restOfCards, true);
                        cout << "- he got it right you take all the cards on the ground. you have " << player1.amount() << " cards. and computer have " << cptr.amount() << ".\n";
                    }
                }

                if (decision2 == 2 || decision2 == 3)
                {
                    // ai plays with fake
                    cptrState = 'f';
                    // keep tracking cptr states
                    cptrPlays[cptrPlaysCounter] = cptrState;
                    cptrPlaysCounter++;

                    cptrPeek = cptr.peek();
                    cout << "- cptr says he played " << cardNameGen(rand() % 13) << "!\n";
                    restOfCards.push(cptr.pop());
                    cout << "- You have " << player1.amount() << " Cards. cptr have " << cptr.amount() << endl;
                    turn = 0;
                }
                else
                {
                    // ai plays with truth
                    cptrState = 't';
                    // keep tracking player states
                    cptrPlays[cptrPlaysCounter] = cptrState;
                    cptrPlaysCounter++;
                    if (overPlayed(cptrPlays)) // check if over played
                    {
                        cptrState = 'f';
                        // loop made to reintialze plays array with all 'f'
                        for (int i = 0; i < 100; i++)
                        {
                            cptrPlays[i] = 'f';
                        }
                    }

                    cptrPeek = cptr.peek();
                    cout << "- cptr says he played " << cardNameGen(cptrPeek) << "!\n";
                    restOfCards.push(cptr.pop());
                    cout << "- You have " << player1.amount() << " Cards. cptr have " << cptr.amount() << endl;
                    turn = 0;
                }
            }
        }

        // check winning state
        if (player1.amount() == 0)
        {
            cout << "we have a winner!!!!!!!!!!!!!!. congrats you defated Ai\n";
            gameOver = true;
        }
        if (cptr.amount() == 0)
        {
            cout << "we have a winner!!!!!!!!!!!!!!. Ai has won. Good luck next time <3 \n";
            gameOver = true;
        }
        if (cptrPlaysCounter == 100 || playerPlaysCounter == 100)
        {
            cout << "Game ended in Tie.Good luck next Time\n";
            gameOver = true;
        }
    }
}

// Driver code
int main()
{
    char decision; // used to check if wants to play again.
    bool exit = false;

    while (!exit)
    {
        play();
        cout << " hope you enjoyed our game\n";
        cout << "do you wanna play again? enter y for yes n for no\n";
        cin >> decision;
        if (decision == 'y' || decision == 'Y')
        {
            gameOver = false;
            play();
        }
        else
        {
            exit = true;
        }
    }
    cout << "see you soon ^-^\n";
}
