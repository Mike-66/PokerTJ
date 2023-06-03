#define InitBoardCPP

#include "Board.h"

//-----------------------------------------------

TBoard::TBoard()
{
 Clear();
}
//-----------------------------------------------
TBoard::~TBoard()
{

}

//-----------------------------------------------------

void TBoard::Clear(void)
{
 validcards=0;
}

//-----------------------------------------------------

void TBoard::SetCard(int cardindex)
{
 iCard[validcards++]=cardindex;
}

//-----------------------------------------------------

QJsonObject TBoard::AsJsonObject(void)
{
    QJsonObject O,o;
    QJsonArray a;
    O.insert("validcards",validcards);
    for(int i=0;i<validcards;i++)
    {
         o.insert("iCard",iCard[i]);
         a.append(o);
    }
    O.insert("Cards",a);
    return(O);
}

//-----------------------------------------------------

void TBoard::FromJsonObject(QJsonObject *O)
{
    validcards=O->value("validcards").toInt();
    QJsonArray cardArray;
    cardArray = O[0]["Cards"].toArray();
    for(int i = 0; i < cardArray.size(); i++)
    {
     QJsonObject o=cardArray[i].toObject();
     iCard[i]=o.value("iCard").toInt();
    }
}

//-----------------------------------------------------
