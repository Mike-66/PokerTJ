#ifndef THAND_H
#define THAND_H

#include <QString>
#include "Board.h"
#include "Deck.h"

#define NUM_POKER_HANDS           10

#define POKER_HAND_HIGH_CARD       0
#define POKER_HAND_PAIR            1
#define POKER_HAND_TWO_PAIR        2
#define POKER_HAND_THREE_OF_A_KIND 3
#define POKER_HAND_STRAIGHT        4
#define POKER_HAND_FLUSH           5
#define POKER_HAND_FULL_HOUSE      6
#define POKER_HAND_FOUR_OF_A_KIND  7
#define POKER_HAND_STRAIGHT_FLUSH  8
#define POKER_HAND_ROYAL_FLUSH     9

/* Result
X1: Hand
X2,3: Subresult of Hand
       -Value of
            - highest Card in HIGH_CARD
            - PAIR
            - Higher TWO_PAIR
            - THREE_OF_A_KIND (also in case of FULL_HOUSE)
            - FOUR_OF_A_KIND
       -Value of Highest Card of
            - STRAIGHT
            - FLUSH
            - STRAIGHT_FLUSH
X4,5:  -Value of
            - second highest Card in HIGH_CARD
            - highest card in PAIR
            - lower TWO_PAIR (also in case of FULL_HOUSE)
            - highest Card in THREE_OF_A_KIND
            - Kicker FOUR_OF_A_KIND
X6,7   -Value of
            - third highest Card in HIGH_CARD
            - second card in PAIR
            - highest Card in TWO_PAIR
X8,9   -Value of
            - fourth highest Card in HIGH_CARD
            - third card in PAIR
X10,11 -Value of
            - fifth highest Card in HIGH_CARD

Result  12233445566
*/

#define MAX_COUNT_OF_A_KIND        3

#define SOURCE_BOARD               0
#define SOURCE_PLAYER              1

#define SORT_BYVALUE 0
#define SORT_BYCOLOR 1

#define SORT_ORDER_NORMAL  0
#define SORT_ORDER_REVERT  1

class THand
{
protected:
     TBoard *Board;
     TDeck *Deck;

     int  *Array_ByValue;
     int  *Array_ByValueColor;
     int  Array_Size;
     int  Array_ByValue_Num;
     int  Array_ByValueColor_Num;

     int NumCards;

     int CountOfAKind_Number;
     int CountOfAKind_Count[MAX_COUNT_OF_A_KIND];
     int CountOfAKind_Value[MAX_COUNT_OF_A_KIND];

     int RealPokerHand[NUM_POKER_HANDS];
     unsigned long long RealPokerResult[NUM_POKER_HANDS];

     int *CountOfAColor;

     void InitArray(void);
     void Sort(QString type,  // "ByValue, ByColor"
               int itype,     //  byvalue, bycolor
               int order,     //  asc,desc
               int srccount,  // Player->validcards+Board->validcards
               int *srcarray, // NULL for Player, Array_ByValue/ByColor Otherwise
               int *dstarray, // Where to sort to
               int *dstcount);
     void SortByValue(void);
     void SortByValueColor(void);

     void SetKickers(int hand, int second, int start, int count);
     void SetResult(int hand, int value, int position);

     QString Name_2[2];
     QString Name[NUM_POKER_HANDS];


public:
    THand(TDeck *Deck, TBoard *Board);

    int GetColor(int index, int *source);
    int GetValue(int index, int *source);

    void Check(void *player);

    int CheckCountOfaKind(void);
    int CheckCountOfaColor(void);
    int CheckCountOfaRow(void);

    unsigned long long GetResult(void);

    QString ExplainResult(unsigned long long result);
};

#endif // THAND_H
