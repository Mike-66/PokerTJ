#define InitHandCPP

#include "Hand.h"
#include "Game.h"

TPlayer *Player;

static int NUM_CARDS_ALL=NUM_PLAYER_CARDS+NUM_BOARD_CARDS;

//-----------------------------------------------

THand::THand(TDeck *Deck, TBoard *Board)
{
 this->Board=Board;
 this->Deck=Deck;

 Array_Size=NUM_PLAYER_CARDS+NUM_BOARD_CARDS;
 Array_ByValue     =new int[Array_Size];
 Array_ByValueColor=new int[Array_Size];
 InitArray();

 CountOfAColor=new int[NUM_COLORS];

 Name_2[POKER_HAND_HIGH_CARD]      = "high cards #1,#2";
 Name_2[POKER_HAND_PAIR]           = "a pair of #1s";

 Name[POKER_HAND_HIGH_CARD]      = "high cards #1,#2,#3,#4 and #5";
 Name[POKER_HAND_PAIR]           = "a pair of #1s, high cards #2,#3 and #4";
 Name[POKER_HAND_TWO_PAIR]       = "two pairs #1s and #2s, high card #3";
 Name[POKER_HAND_THREE_OF_A_KIND]= "three of a kind #1s, high cards #2 and #3";
 Name[POKER_HAND_STRAIGHT]       = "straight down from #1";
 Name[POKER_HAND_FLUSH]          = "flush with #1,#2,#3,#4,#5";
 Name[POKER_HAND_FULL_HOUSE]     = "full house , three of #1s, two of #2s";
 Name[POKER_HAND_FOUR_OF_A_KIND] = "four of a kind with #1s";
 Name[POKER_HAND_STRAIGHT_FLUSH] = "straight flush down from #1";
 Name[POKER_HAND_ROYAL_FLUSH]    = "royal Flush";
}

//------------------------------------------------------------

void THand::InitArray(void)
{
 for(int i=0;i<Array_Size;i++)
 {
  Array_ByValue[i]     =-1;
  Array_ByValueColor[i]=-1;
 }
 Array_ByValue_Num=0;
 Array_ByValueColor_Num=0;

 for(int i=0;i<NUM_POKER_HANDS;i++)
 {
  RealPokerHand[i]=-1;
  RealPokerResult[i]=0;
 }
}

//------------------------------------------------------------

void THand::Check(void *player)
{
 Player=(TPlayer*)player;
 NumCards=Player->validcards+Board->validcards;
 InitArray();
 SortByValue();
 SortByValueColor();
 CheckCountOfaKind();
 CheckCountOfaColor();
 CheckCountOfaRow();
}

//-----------------------------------------------

int THand::GetColor(int index, int *source)
{
 if(index<NUM_PLAYER_CARDS)
 {
  *source=SOURCE_PLAYER;
  return(Deck->Card[Player->iCard[index]]->Color);
 }
 else
 {
  *source=SOURCE_BOARD;
  index-=NUM_PLAYER_CARDS;
  return(Deck->Card[Board->iCard[index]]->Color);
 }
 return(-1);
}

//-----------------------------------------------

int THand::GetValue(int index, int *source)
{
 if(index<NUM_PLAYER_CARDS)
 {
  *source=SOURCE_PLAYER;
  return(Deck->Card[Player->iCard[index]]->Value);
 }
 else
 {
  *source=SOURCE_BOARD;
  index-=NUM_PLAYER_CARDS;
  return(Deck->Card[Board->iCard[index]]->Value);
 }
 return(-1);
}

//-----------------------------------------------

void THand::Sort(QString type,  // "ByValue, ByColor"
                 int itype,     //  byvalue, bycolor
                 int order,     //  asc,desc
                 int srccount,  // Player->validcards+Board->validcards
                 int *srcarray, // NULL for Player, Array_ByValue/ByColor Otherwise
                 int *dstarray, // Where to sort to
                 int *dstcount
                 )
{
 int source; //board,player

 int vc;

 for(int i=0;i<srccount;i++)
 {
  int idx=i;
  if(srcarray!=NULL)
  idx=srcarray[idx];

  switch(itype)
  {
   case 0:vc=GetValue(idx, &source );break;
   case 1:vc=GetColor(idx, &source );break;
  }

  if(dstcount==0)
  {
   dstarray[0]=idx;
   *dstcount=1;
  }
  else
  {
   int placed=0;
   for(int j=0;j<*dstcount;j++)
   {
    int vc1;
    switch(itype)
    {
     case 0:vc1=GetValue(dstarray[j], &source );break;
     case 1:vc1=GetColor(dstarray[j], &source );break;
    }

    if( vc <= vc1 )
    {
     for(int k=*dstcount-1;k>=j;k--)
     {
      int tmp=dstarray[k];
      dstarray[k+1]=tmp;
     }
     dstarray[j]=idx;
     placed=1;
    }
    if(placed==1)
    break;
   }
   if(placed==0)
   dstarray[*dstcount]=idx;
   dstcount[0]++;
  }
 }

 if(order==SORT_ORDER_REVERT)
 {
  int *tmp_array=new int[Array_Size];
  for(int i=0;i<srccount;i++)
  tmp_array[i]=dstarray[dstcount[0]-1-i];
  for(int i=0;i<srccount;i++)
  dstarray[i]=tmp_array[i];
  delete [] tmp_array;
 }

 QString msg=Player->GetNickName();
 msg.append(" ");
 for(int i=0;i<*dstcount;i++)
 {
  QString H;
  msg.append("c");
  H.setNum(GetColor(dstarray[i], &source));
  msg.append(H);
  msg.append(":");
  H.setNum(GetValue(dstarray[i], &source)+1);
  msg.append(H);
 }
 Log( type + " "+ msg);
}

//-----------------------------------------------

void THand::SortByValue(void)
{
 Sort("SortByValue",SORT_BYVALUE,SORT_ORDER_NORMAL,NumCards,NULL,Array_ByValue,&Array_ByValue_Num);
}

//-----------------------------------------------

void THand::SortByValueColor(void)
{
 Sort("SortByValueColor",SORT_BYCOLOR,SORT_ORDER_REVERT,NumCards,Array_ByValue,Array_ByValueColor,&Array_ByValueColor_Num);
}

//-----------------------------------------------

void THand::SetResult(int hand, int value, int position)
{
 unsigned long long exponent;
 switch(position)
 {
  case 1:exponent=10000000000;break;
  case 2:exponent=100000000;break;
  case 3:exponent=1000000;break;
  case 4:exponent=10000;break;
  case 5:exponent=100;break;
  case 6:exponent=1;break;
  default:exponent=0;
 }
 RealPokerResult[hand]+=value*exponent;
}

//-----------------------------------------------

QString THand::ExplainResult(unsigned long long result)
{
 QString msg="";
 unsigned long long exponent=10000000000;
 unsigned long long res=result/exponent;

 if(NumCards==2)
 msg=Name_2[res];
 else
 msg=Name[res];

 int imax=NumCards;
 if(imax>5)
 imax=5;

 for(int i=0;i<imax;i++)
 {
  result-=res*exponent;
  exponent/=100;
  res=result/exponent;
  int r=res;
  if(r==13)
  r=0;
  QString wild="#"+QString().setNum(i+1);
  msg=msg.replace(wild,Deck->CVS[r]);
 }
 return(msg);
}

//-----------------------------------------------
unsigned long long THand::GetResult(void)
{
 unsigned long long result=0;

 for(int i=NUM_POKER_HANDS-1;i>=0;i--)
 if(RealPokerResult[i]>0)
 {
  result=RealPokerResult[i];
  break;
 }

 QString msg=Player->GetNickName();
 msg.append(" ");
 QString H;
 H.setNum(result);
 msg.append(H);
 Log(" Result: " + msg);

 return(result);
}

//-----------------------------------------------

void THand::SetKickers(int hand, int second, int start, int count)
{
 int source;
 int hc=0;
 for(int i=NumCards-1;i>=0;i--)
 {
  int addtokicker=1;
  int val=GetValue(Array_ByValue[i], &source);
  if(val==RealPokerHand[hand])
  addtokicker=0;
  if(second>=0 && val==RealPokerHand[second])
  addtokicker=0;

  if(addtokicker==1)
  {
   SetResult(hand,val,start+hc);
   hc++;
   if(hc==count)
   break;
  }
 }
}

//-----------------------------------------------

int THand::CheckCountOfaKind(void)
{
 //How many "pairs,threes.." do we have, may also be a full house (3,2) later
 CountOfAKind_Number=0;
 for(int i=0;i<MAX_COUNT_OF_A_KIND;i++)
 {
  //the number of identical values
  CountOfAKind_Count[i]=0;
  //and finally their value
  CountOfAKind_Value[i]=-1;
 }

 int source;
 int lastwaskind=0;
 int val_old=-1;

 for(int i=0;i<NumCards;i++)
 {
  int val=GetValue(Array_ByValue[i], &source);
  lastwaskind=0;
  if(val_old>=0)
  {
   if(val==val_old)
   {
    lastwaskind=1;
    if(CountOfAKind_Count[CountOfAKind_Number]==0)
    {
     CountOfAKind_Value[CountOfAKind_Number]=val;
     CountOfAKind_Count[CountOfAKind_Number]=2;
    }
    else
    {
     CountOfAKind_Count[CountOfAKind_Number]++;
    }
   }
   else if(CountOfAKind_Count[CountOfAKind_Number]>0)
   {
    CountOfAKind_Number++;
   }

  }
  val_old=val;
 }

 if(lastwaskind && CountOfAKind_Count[CountOfAKind_Number]>0)
 CountOfAKind_Number++;

 for(int i=0;i<CountOfAKind_Number;i++)
 Log("CountOfAKind_Number# " + QString().setNum(i) +" C "+QString().setNum(CountOfAKind_Count[i])+" V "+QString().setNum(CountOfAKind_Value[i]));


 int havesecondthree=-1;
 for(int i=0;i<CountOfAKind_Number;i++)
 {
  //----POKER_HAND_HIGH_CARD------------------------------------------------------------------
  if(CountOfAKind_Count[i]==1)
  {
   if(CountOfAKind_Value[i]>RealPokerHand[POKER_HAND_HIGH_CARD])
   RealPokerHand[POKER_HAND_HIGH_CARD]=CountOfAKind_Value[i];
  }
  //----POKER_HAND_PAIR-----------------------------------------------------------------------
  else if(CountOfAKind_Count[i]==2)
  {
   //wir haben schon ein Paar
   if(RealPokerHand[POKER_HAND_PAIR]>=0)
   {
    //welches ist das größere ?
    if(CountOfAKind_Value[i]>RealPokerHand[POKER_HAND_PAIR])
    {
     if(RealPokerHand[POKER_HAND_TWO_PAIR]>RealPokerHand[POKER_HAND_PAIR])
     RealPokerHand[POKER_HAND_PAIR]=RealPokerHand[POKER_HAND_TWO_PAIR];
     RealPokerHand[POKER_HAND_TWO_PAIR]=CountOfAKind_Value[i];
    }
    else
    {
     RealPokerHand[POKER_HAND_TWO_PAIR]=RealPokerHand[POKER_HAND_PAIR];
     if(CountOfAKind_Value[i]>RealPokerHand[POKER_HAND_PAIR])
     RealPokerHand[POKER_HAND_PAIR]=CountOfAKind_Value[i];
    }
   }
   else
   {
    RealPokerHand[POKER_HAND_PAIR]=CountOfAKind_Value[i];
   }
  }
  //----POKER_HAND_THREE_OF_A_KIND------------------------------------------------------------
  else if (CountOfAKind_Count[i]==3)
  {
   //es gibt schon einen Drilling
   if(RealPokerHand[POKER_HAND_THREE_OF_A_KIND]>=0)
   {
    //welchs ist das größere ?
    if(CountOfAKind_Value[i]>RealPokerHand[POKER_HAND_THREE_OF_A_KIND])
    {
     //Höheren Drilling tauschen
     havesecondthree=RealPokerHand[POKER_HAND_THREE_OF_A_KIND];
     RealPokerHand[POKER_HAND_THREE_OF_A_KIND]=CountOfAKind_Value[i];
    }
    else
    {
     //der zweite Drilling ist der niedere
     havesecondthree=CountOfAKind_Value[i];
    }
   }
   else
   {
    RealPokerHand[POKER_HAND_THREE_OF_A_KIND]=CountOfAKind_Value[i];
   }
  }
  //----POKER_HAND_FOUR_OF_A_KIND------------------------------------------------------------
  else if (CountOfAKind_Count[i]==4)
  {
   RealPokerHand[POKER_HAND_FOUR_OF_A_KIND]=CountOfAKind_Value[i];
  }
 }

 if(RealPokerHand[POKER_HAND_FOUR_OF_A_KIND]>=0)
 {
  //Set top Most Result
  SetResult(POKER_HAND_FOUR_OF_A_KIND,POKER_HAND_FOUR_OF_A_KIND,1);
  //Set Value of the Hand
  SetResult(POKER_HAND_FOUR_OF_A_KIND,RealPokerHand[POKER_HAND_FOUR_OF_A_KIND],2);
  //Get The Kicker
  SetKickers(POKER_HAND_FOUR_OF_A_KIND,-1,3,1);
 }
 else if(RealPokerHand[POKER_HAND_THREE_OF_A_KIND]>=0)
 {
  //Check Full Houses
  if(havesecondthree>=0)
  {
   RealPokerHand[POKER_HAND_FULL_HOUSE]=RealPokerHand[POKER_HAND_THREE_OF_A_KIND];
   RealPokerHand[POKER_HAND_PAIR]=havesecondthree;
  }
  else if(RealPokerHand[POKER_HAND_TWO_PAIR]>=0)
  {
   RealPokerHand[POKER_HAND_FULL_HOUSE]=RealPokerHand[POKER_HAND_THREE_OF_A_KIND];
   RealPokerHand[POKER_HAND_PAIR]=RealPokerHand[POKER_HAND_TWO_PAIR];
  }
  else if(RealPokerHand[POKER_HAND_PAIR]>=0)
  {
   RealPokerHand[POKER_HAND_FULL_HOUSE]=RealPokerHand[POKER_HAND_THREE_OF_A_KIND];
  }

  if(RealPokerHand[POKER_HAND_FULL_HOUSE]>=0) //we have a full hosue
  {
   //Set top Most Result
   SetResult(POKER_HAND_FULL_HOUSE,POKER_HAND_FULL_HOUSE,1);
   //Set Value of the Hand
   SetResult(POKER_HAND_FULL_HOUSE,RealPokerHand[POKER_HAND_FULL_HOUSE],2);
   //Set Value of Pair in Full House
   SetResult(POKER_HAND_FULL_HOUSE,RealPokerHand[POKER_HAND_PAIR],3);
  }
  else
  {
   //Set top Most Result
   SetResult(POKER_HAND_THREE_OF_A_KIND,POKER_HAND_THREE_OF_A_KIND,1);
   //Set Value of the Hand
   SetResult(POKER_HAND_THREE_OF_A_KIND,RealPokerHand[POKER_HAND_THREE_OF_A_KIND],2);
   //Get the Kickers
   SetKickers(POKER_HAND_THREE_OF_A_KIND,-1,3,2);
  }

 }
 else if(RealPokerHand[POKER_HAND_TWO_PAIR]>=0)
 {
  //Set top Most Result
  SetResult(POKER_HAND_TWO_PAIR,POKER_HAND_TWO_PAIR,1);
  //Set Value of the Hand
  SetResult(POKER_HAND_TWO_PAIR,RealPokerHand[POKER_HAND_TWO_PAIR],2);
  //Set Value of Second Pair
  SetResult(POKER_HAND_TWO_PAIR,RealPokerHand[POKER_HAND_PAIR],3);
  //Get Three Highest Cards
  SetKickers(POKER_HAND_TWO_PAIR,POKER_HAND_PAIR,4,1);
 }
 else if(RealPokerHand[POKER_HAND_PAIR]>=0)
 {
  //Set top Most Result
  SetResult(POKER_HAND_PAIR,POKER_HAND_PAIR,1);
  //Set Value of the Hand
  SetResult(POKER_HAND_PAIR,RealPokerHand[POKER_HAND_PAIR],2);
  //Get Three Highest Cards
  SetKickers(POKER_HAND_PAIR,-1,3,3);
 }
 else
 {
  //Set top Most Result
  SetResult(POKER_HAND_HIGH_CARD,POKER_HAND_HIGH_CARD,1);
  //Set Value of the Hand
  RealPokerHand[POKER_HAND_HIGH_CARD]=GetValue(Array_ByValue[NumCards-1], &source);
  SetResult(POKER_HAND_HIGH_CARD,RealPokerHand[POKER_HAND_HIGH_CARD],2);
  //Get For Next Highest Cards
  SetKickers(POKER_HAND_HIGH_CARD,-1,3,4);
 }

 QString msg=Player->GetNickName();
 msg.append(" ");
 for(int i=0;i<CountOfAKind_Number;i++)
 {
  QString H;
  H.setNum(CountOfAKind_Count[i]);
  msg.append(H);
  msg.append("x");
  H.setNum(CountOfAKind_Value[i]);
  msg.append(H);
  msg.append(":");
 }
 Log("CountOfAKind "+ msg);

 //RestChancen berechnen
 if(NUM_CARDS_ALL>NumCards)
 {

 }

 /*
 possible Results
 #define POKER_HAND_HIGH_CARD       0
 #define POKER_HAND_PAIR            1
 #define POKER_HAND_TWO_PAIR        2
 #define POKER_HAND_THREE_OF_A_KIND 3
 #define POKER_HAND_FULL_HOUSE      6
 #define POKER_HAND_FOUR_OF_A_KIND  7
 */

 return(0);
}

//-----------------------------------------------

int THand::CheckCountOfaColor(void)
{
 Log("CheckCountOfaColor " + Player->GetNickName());

 for(int i=0;i<NUM_COLORS;i++)
 CountOfAColor[i]=0;

 int source;
 int flushcolor=-1;
 for(int i=0;i<NumCards;i++)
 {
  int col=GetColor(Array_ByValueColor[i], &source);
  CountOfAColor[col]++;
  if(CountOfAColor[col]>=5)
  flushcolor=col;
 }

 //for(int i=0;i<NUM_COLORS;i++)
 //{
 // Log("Color/Count" << i << "/" << CountOfAColor[i];
 //}
 Log("flushcolor is " + QString().setNum(flushcolor));

 int lastval=0;
 int is_straight=1;
 int haveace=0;
 int found=0;

 if(flushcolor>=0)
 for(int i=NumCards-1;i>=0;i--)
 {
  int col=GetColor(Array_ByValueColor[i],&source);
  int val=GetValue(Array_ByValueColor[i],&source);

  if( col == flushcolor )
  {
   if( val == 13 )
   haveace=1;

   found++;

   if(found==1)
   {
    RealPokerHand[POKER_HAND_FLUSH]=val;
    //Set top Most Result
    SetResult(POKER_HAND_FLUSH,POKER_HAND_FLUSH,1);
    //Set Value of the Hand
    SetResult(POKER_HAND_FLUSH,RealPokerHand[POKER_HAND_FLUSH],2);
   }
   else
   {
    if(found<=5)
    SetResult(POKER_HAND_FLUSH,val,1+found);

    if( lastval == val + 1 )
    {
     if(is_straight==1)
     RealPokerHand[POKER_HAND_STRAIGHT_FLUSH]=lastval;
     is_straight++;
    }
    else
    {
     is_straight=1;
    }

    if( is_straight == 5 )
    break;
   }
   lastval=val;

   //Log("found " << found;
   //Log("RealPokerResult[POKER_HAND_FLUSH] " << RealPokerResult[POKER_HAND_FLUSH];
   //Log("RealPokerResult[POKER_HAND_STRAIGHT_FLUSH] " << RealPokerResult[POKER_HAND_STRAIGHT_FLUSH];
   //Log("RealPokerResult[POKER_HAND_ROYAL_FLUSH] " << RealPokerResult[POKER_HAND_ROYAL_FLUSH];
  }
 } //NumCards

 if( lastval==1 && is_straight == 4 && haveace )
 is_straight++;

 if( is_straight == 5 )
 {
  //Set top Most Result
  SetResult(POKER_HAND_STRAIGHT_FLUSH,POKER_HAND_STRAIGHT_FLUSH,1);
  //Set Value of the Hand
  SetResult(POKER_HAND_STRAIGHT_FLUSH,RealPokerHand[POKER_HAND_STRAIGHT_FLUSH],2);
  //not necessary to remember the kickers since it is a straight

  if( RealPokerHand[POKER_HAND_STRAIGHT_FLUSH] == 13 )
  SetResult(POKER_HAND_ROYAL_FLUSH,POKER_HAND_ROYAL_FLUSH,1);
 }

 QString msg=Player->GetNickName();
 msg.append(" ");
 for(int i=0;i<NUM_COLORS;i++)
 {
  QString H;
  H.setNum(CountOfAColor[i]);
  msg.append(H);
  msg.append("x");
  H.setNum(i);
  msg.append(H);
  msg.append(":");
 }
 Log("CountOfAColor "+ msg);

 /*
 possible Results
 #define POKER_HAND_FLUSH           5
 #define POKER_HAND_STRAIGHT_FLUSH  8
 #define POKER_HAND_ROYAL_FLUSH     9
 */

 return(0);
}

//-----------------------------------------------

int THand::CheckCountOfaRow(void)
{
 Log("CheckCountOfaRow" + Player->GetNickName());

 /*
 possible Results
 #define POKER_HAND_STRAIGHT        4
 */

 int source;
 int lastval=-1;
 int is_straight=1;
 int haveace=0;


 for(int i=NumCards-1;i>=0;i--)
 {
  int val=GetValue(Array_ByValue[i],&source);
  if( val == 13 )
  haveace=1;

  if( lastval>=0 )
  {
   if( lastval == val + 1 )
   {
    if(is_straight==1)
    {
     RealPokerHand[POKER_HAND_STRAIGHT]=lastval;
     //Set top Most Result
     SetResult(POKER_HAND_STRAIGHT,POKER_HAND_STRAIGHT,1);
     //Set Value of the Hand
     SetResult(POKER_HAND_STRAIGHT,RealPokerHand[POKER_HAND_STRAIGHT],2);
    }
    is_straight++;
    if(is_straight<=5)
    SetResult(POKER_HAND_STRAIGHT,val,1+is_straight);
   }
   else
   {
    if(lastval!=val)
    {
     //this is no straight
     is_straight=1;
     //reset result
     RealPokerResult[POKER_HAND_STRAIGHT]=0;
     RealPokerHand[POKER_HAND_STRAIGHT]=-1;
    }
   }
  }

  lastval=val;

  if( is_straight == 5 )
  break;

 }

 if( lastval==1 && is_straight == 4 && haveace )
 is_straight++;

 if(is_straight!=5)
 {
  RealPokerResult[POKER_HAND_STRAIGHT]=0;
  RealPokerHand[POKER_HAND_STRAIGHT]=-1;
 }

 Log("+RealPokerResult[POKER_HAND_STRAIGHT] " +QString().setNum(RealPokerResult[POKER_HAND_STRAIGHT]));


 return(0);
}

//-----------------------------------------------






