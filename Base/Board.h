#ifndef BOARD_H
#define BOARD_H

#include <QJsonObject>
#include <QJsonArray>

#define NUM_BOARD_CARDS   5  //FLOP,TURN,RIVER

class TBoard
{
private:
protected:
public:
   TBoard();
   ~TBoard();

   int validcards;
   int iCard[NUM_BOARD_CARDS];

   void Clear(void);
   void SetCard(int cardindex);

   QJsonObject AsJsonObject(void);
   void FromJsonObject(QJsonObject *O);
};

#endif // BOARD_H
