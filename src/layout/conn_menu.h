#ifndef JOBMENU_H
#define JOBMENU_H
#include <QMenu>

class ConnectMenu : public QMenu
{
    Q_OBJECT
public:
    ConnectMenu(QWidget *parent = NULL);
    ~ConnectMenu();

    QAction  *edit;
    QAction  *display;
    QAction  *clean;
    QAction  *act;
    QAction  *refresh;

signals:

public slots:

private:

private slots:

};
#endif
