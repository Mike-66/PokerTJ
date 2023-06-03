#define InitDeckCPP

#include "Deck.h"

TDeck::TDeck()
{
 int value;
 Card=new TCard*[NUM_CARDSX];
 for(int i=0;i<NUM_COLORS;i++)
 for(int j=0;j<NUM_VALUES;j++)
 {
  int index=NUM_VALUES*i+j;
  value=j;
  if(value==0)
  value=13;
  Card[index]=new TCard(index, i, value);
 }

 CVS=new QString [NUM_VALUES];
 CVS[0] ="A";
 CVS[1] ="2";
 CVS[2] ="3";
 CVS[3] ="4";
 CVS[4] ="5";
 CVS[5] ="6";
 CVS[6] ="7";
 CVS[7] ="8";
 CVS[8] ="9";
 CVS[9] ="10";
 CVS[10]="J";
 CVS[11]="Q";
 CVS[12]="K";

 CSS=new QString [NUM_COLORS];
 CSS[0] =u8"\u2665";
 CSS[1] =u8"\u2660";
 CSS[2] =u8"\u2662";
 CSS[3] =u8"\u2663";

 Clear();
}
//-----------------------------------
TDeck::~TDeck()
{

}
/*
//-----------------------------------------------------
QJsonObject TDeck::AsJsonObject(void)
{
    QJsonObject O,o;
    QJsonArray a;
    O.insert("CardsAvailable",CardsAvailable);
    for(int i=0;i<NUM_CARDSX;i++)
    {
     o.empty();
     o.insert("iCard",iCard[i]);
     a.append(o);
    }
    O.insert("Cards",a);
    return(O);
}
//-----------------------------------------------------
void TDeck::FromJsonObject(QJsonObject *O)
{
    validcards=O->value("validcards").toInt();
    QJsonArray cardArray;
    cardArray = O[0]["Cards"].toArray();
    for(int i = 0; i < cardArray.size(); i++)
    iCard[i]=cardArray[i].toInt();
}
*/
//-----------------------------------
void::TDeck::Clear(void)
{
 for(int i=0;i<NUM_CARDSX;i++)
 Card[i]->Location=LOCATION_DECK;
 CardsAvailable=NUM_CARDSX;
}
//-----------------------------------
//returns card index of card
int TDeck::SetCardLocation(int Location, int cardnum)
{
//#define    LOCATION_DECK   -4
//#define    LOCATION_FLOP   -3
//#define    LOCATION_TURN   -2
//#define    LOCATION_RIVER  -1
//#define    LOCATION_PLAYER  0

 int index=0;
 for(int i=0;i<NUM_CARDSX;i++)
 if(Card[i]->Location<=LOCATION_DECK) //this card is available
 {
  if(index==cardnum)
  {
   Card[i]->Location=Location;
   CardsAvailable--;
   return(i);
  }
  index++;
 }
 return(-1);
}
//-------------------------------------
int TDeck::GetCardIndex(int color, int value)
{
 return(color*NUM_VALUES+value);
}
//-------------------------------------



