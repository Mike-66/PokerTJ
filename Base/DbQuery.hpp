#include "Helpers.hpp"

#ifdef extint
#undef extint
#endif
#ifdef DBQUERYInitC
#define extint
#else
#define extint extern
#endif
#undef DBQUERYInitC

#ifndef TDBQUERY_HPP
#define TDBQUERY_HPP

#include <QObject>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>

class TDbQuery : public QObject
{
    Q_OBJECT
public:
    explicit TDbQuery(QObject *parent = nullptr);

    QString        DB_User;
    QString        DB_Role;
    QString        DB_Password;
    QString        DB_Host;
    QString        DB_Name;

    bool OpenDatabase(void);

    bool CreateModel(QString ModelName, QString Filter);
    bool AddToModel(QString Name, int Value);
    bool AddToModelOnce(QString Name, int Value);
    bool AddToModelOnce(int round, int chair, qint64 result);
    bool UpdateModel(QString Name, int Value);
    void ShowModel(void);

protected:
    QSqlDatabase   Database;
    QSqlQuery      Query;
    QSqlTableModel *model;

    bool ModelSelect(QSqlTableModel *m);
    bool ModelSubmitAll(QSqlTableModel *m);

signals:

};

#endif // TDBQUERY_HPP

extint TDbQuery *DbQuery;
