#ifndef DECK_H
#define DECK_H

#include <QJsonObject>
#include <QJsonArray>

#include "Card.h"

#include <QString>

enum Card_Suit {  hearts, spades, diamonds , clubs };
enum Card_Value { ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king };

class TDeck
{
private:
protected:
public:
    TDeck();
    ~TDeck();

    /*
    QJsonObject AsJsonObject(void);
    void        FromJsonObject(QJsonObject *O);
    */

    int     CardsAvailable;
    TCard   **Card;

    QString *CVS;
    QString *CSS;

    void    Clear(void);
    int     SetCardLocation(int Location, int cardnum);

    int     GetCardIndex(int color, int value);
};

#endif // DECK_H
