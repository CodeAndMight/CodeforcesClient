#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QJsonValue>

#include "codeforcesapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readyRead();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    CodeforcesAPI m_api;

    void fillTreeWidget(const QJsonValue &jsonValue, QTreeWidgetItem *parent = 0);

    QTreeWidgetItem *createNodeTreeWidget(QTreeWidgetItem *parent, const QStringList &labels, const QJsonObject &jsonObject);
    QTreeWidgetItem *createItemTreeWidget(QTreeWidgetItem *parent, const QString &label);
};

#endif // MAINWINDOW_H
