#include "gamemodel.hpp"

TGameModel::TGameModel(QObject *parent)
    : QAbstractTableModel(parent)
 {
    header[GM_COL_GAMENAME]="Game";
    header[GM_COL_PLAYERS]="Players";
    header[GM_COL_ACTIVEPLAYERS]="Active Players";
    header[GM_COL_SPOTTERS]="Spotters";
    header[GM_COL_BOTS]="Bots";
    header[GM_COL_STRENGTHOFBOTS]="Strength";
    header[GM_COL_CREDIT]="Credit";
    header[GM_COL_BIGBLIND]="Big Blind";
    header[GM_COL_BLINDRAISE]="Raise";
}

int TGameModel::rowCount(const QModelIndex & /*parent*/) const
{
   return stringList[0].count();
}

int TGameModel::columnCount(const QModelIndex & /*parent*/) const
{
    return GM_COLCOUNT;
}

QVariant TGameModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return header[section];
    else
        return QStringLiteral("%1").arg(section+1);
}

QVariant TGameModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
    //qDebug() << QString("row %1, col%2, role %3")
    //        .arg(row).arg(col).arg(role);

    QFont boldFont;

    switch (role)
    {
        case Qt::DisplayRole:
            return(stringList[index.column()].at(index.row()));
        break;
        case Qt::FontRole:
              boldFont.setBold(true);
              return boldFont;
        break;
        case Qt::BackgroundRole:
             if(ActiveRow==index.row())
             return QBrush(Qt::red);
             return QBrush(Qt::green);
        break;
        case Qt::TextAlignmentRole:
            if ( col > GM_COL_GAMENAME )
            return int(Qt::AlignRight| Qt::AlignVCenter);
        break;
        case Qt::CheckStateRole:
             //return Qt::Checked;
        break;
    }
    return QVariant();
}

void TGameModel::SetActiveRow(QString GameKey)
{
    for (int i=0;i<GameKeystringList.count();i++)
    if(GameKeystringList.value(i)==GameKey)
    ActiveRow=i;
}

void TGameModel::clearLists()
{
    for (int i=0;i<GM_COLCOUNT;i++)
    stringList[i].clear();
    GameKeystringList.clear();
    ActiveRow=-1;
}



