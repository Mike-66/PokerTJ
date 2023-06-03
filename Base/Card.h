#ifndef CARD_H
#define CARD_H

#define    NUM_CARDSX    52
#define    NUM_VALUES    13
#define    NUM_COLORS     4

#define    LOCATION_DECK   -4
#define    LOCATION_FLOP   -3
#define    LOCATION_TURN   -2
#define    LOCATION_RIVER  -1
#define    LOCATION_PLAYER  0




class TCard
{
private:
protected:
public:
    TCard();
    TCard(int Index, int Color, int Value);
    ~TCard();

    int Index;
    int Color;
    int Value;

    int Location;

};


#endif // CARD_H
