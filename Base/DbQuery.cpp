#define DBQUERYInitC

#include "DbQuery.hpp"

//----------------------------------------------------
TDbQuery::TDbQuery(QObject *parent) : QObject(parent)
{
 Log("Constructor of TDbQuery");

 DB_User="pokertj";
 DB_Role="role_poker_server";
 DB_Password="<password>";
 DB_Host="localhost";
 DB_Name="pokertjdb";

 //sudo apt-get install libqt5sql5-mysql
 Database = QSqlDatabase::addDatabase("QMYSQL");
 Query=QSqlQuery(Database);

 Log("DB_User is "+DB_User);
 Log("DB_Role is "+DB_Role);
 Log("DB_Password is "+DB_Password);
 Log("DB_Host is "+DB_Host);
 Log("DB_Name is "+DB_Name);
}
//----------------------------------------------------
bool TDbQuery::OpenDatabase(void)
{
 Database.setHostName(DB_Host);
 Database.setDatabaseName(DB_Name);
 Database.setUserName(DB_User);
 Database.setPassword(DB_Password);

 Database.close();
 bool ok = Database.open();
 QString Message="ERROR ";
 if( ok )
 Message="SUCCESS ";
 Log(Message + "conecting "+ DB_User+" to db "+DB_Host+"."+DB_Name);

 return(ok);
}
//----------------------------------------------------
bool TDbQuery::ModelSelect(QSqlTableModel *m)
{
 if(m->select())
 return(true);
 if(m->lastError().isValid())
 Log("SQLError ["+m->lastError().nativeErrorCode()+"] "+m->lastError().databaseText());
 return(false);
}
//----------------------------------------------------
bool TDbQuery::ModelSubmitAll(QSqlTableModel *m)
{
 if(m->submitAll())
 return(true);
 if(m->lastError().isValid())
 Log("SQLError ["+m->lastError().nativeErrorCode()+"] "+m->lastError().databaseText());
 return(false);
}
//----------------------------------------------------
bool TDbQuery::CreateModel(QString ModelName, QString Filter)
{
 model = new QSqlTableModel;
 model->setTable(ModelName);
 model->setEditStrategy(QSqlTableModel::OnManualSubmit);
 model->setFilter(Filter);
 return(ModelSelect(model));
}
//----------------------------------------------------
bool TDbQuery::AddToModel(QString Name, int Value)
{
 QSqlRecord r=model->record();
 r.setValue("name",Name);
 r.setValue("value",Value);
 model->insertRecord(-1, r);
 if(ModelSubmitAll(model))
 if(ModelSelect(model))
 return(true);
 return(false);
}
//----------------------------------------------------
bool TDbQuery::AddToModelOnce(QString Name, int Value)
{
 QSqlRecord r=model->record();
 r.setValue("name",Name);
 r.setValue("value",Value);
 model->insertRecord(-1, r);
 if(ModelSubmitAll(model))
 {
  model->clear();
  return(true);
 }
 return(false);
}
//----------------------------------------------------
bool TDbQuery::AddToModelOnce(int round, int chair, qint64 result)
{
 QSqlRecord r=model->record();
 r.setValue("round",round);
 r.setValue("chair",chair);
 r.setValue("result",result);
 model->insertRecord(-1, r);
 if(ModelSubmitAll(model))
 {
  model->clear();
  return(true);
 }
 return(false);
}
//----------------------------------------------------
bool TDbQuery::UpdateModel(QString Name, int Value)
{
 QSqlRecord r=model->record(2);
 r.setValue("name",Name);
 r.setValue("value",Value);
 for(int i=0;i<r.count();i++)
 r.setGenerated(i,true);
 model->setRecord(4, r);
 if(ModelSubmitAll(model))
 if(ModelSelect(model))
 return(true);
 return(false);
}
//----------------------------------------------------
void TDbQuery::ShowModel(void)
{
 Log("---------------------------");
 for(int i=0;i<model->rowCount();i++)
 Log(model->record(i).value("name").toString()+"="+model->record(i).value("value").toString());
}



