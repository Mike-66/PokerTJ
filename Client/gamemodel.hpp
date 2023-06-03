#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include <QDebug>
#include <QBrush>
#include <QFont>
#include <QAbstractTableModel>

#define GM_COLCOUNT 9

#define GM_COL_GAMENAME          0
#define GM_COL_PLAYERS           1
#define GM_COL_ACTIVEPLAYERS     2
#define GM_COL_SPOTTERS          3
#define GM_COL_BOTS              4
#define GM_COL_STRENGTHOFBOTS    5
#define GM_COL_CREDIT            6
#define GM_COL_BIGBLIND          7
#define GM_COL_BLINDRAISE        8

class TGameModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TGameModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                           int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void SetActiveRow(QString GameKey);

    QString     header[GM_COLCOUNT];
    QStringList stringList[GM_COLCOUNT];
    QStringList GameKeystringList;
    int         ActiveRow;

    void clearLists(void);
};

#endif // GAMEMODEL_HPP
