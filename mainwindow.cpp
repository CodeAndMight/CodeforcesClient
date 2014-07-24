#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("contest.list?gym=true");
    ui->comboBox->addItem("contest.list");

    ui->comboBox->addItem("contest.hacks?contestId=374");
    ui->comboBox->addItem("contest.standings?contestId=374&from=1&count=5&showUnofficial=true");
    ui->comboBox->addItem("contest.status?contestId=374&from=1&count=10");
    ui->comboBox->addItem("problemset.problems?tags=implementation");

    ui->comboBox->addItem("problemset.recentStatus?count=10");
    ui->comboBox->addItem("user.info?handles=DmitriyH;Fefer_Ivan");

    ui->comboBox->addItem("user.ratedList?activeOnly=true");
    ui->comboBox->addItem("user.rating?handle=Fefer_Ivan");

    ui->comboBox->addItem("user.status?handle=Fefer_Ivan&from=1&count=10");

    connect(&m_api, SIGNAL(readyJsonDocument()), this, SLOT(readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyRead()
{
    const QJsonDocument &jsonDocumnet = m_api.resultJsonDocument();

    ui->treeWidget->clear();

    QJsonObject result = jsonDocumnet.object();
    QStringList keys = result.keys();
    for (QStringList::Iterator key = keys.begin(); key != keys.end(); key++) {
        QTreeWidgetItem *root = new QTreeWidgetItem;

        root->setText(0, *key);
        root->setText(1, " ");

        QJsonValue value = result.take(*key);

        if (value.isString()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);

            item->setText(0, value.toString());
        } else if (value.isArray()) {
            QJsonArray array = value.toArray();

            int index = 0;

            for (QJsonArray::Iterator obj = array.begin(); obj != array.end(); obj++) {
                QJsonObject data = (*obj).toObject();

                QTreeWidgetItem *subitem = new QTreeWidgetItem(root);

                subitem->setText(0, QString::number(++index));

                QStringList labels = data.keys();

                for (QStringList::Iterator label = labels.begin(); label != labels.end(); label++) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(subitem);

                    item->setText(0, *label);

                    QJsonValue jv = data.value(*label);

                    if (jv.isString()) {
                        item->setText(1, jv.toString());
                    } else if (jv.isDouble()) {
                        item->setText(1, QString::number(jv.toDouble()));
                    } else if (jv.isBool()) {
                        item->setText(1, QString::number(jv.toBool()));
                    }
                }
            }

        }

        ui->treeWidget->addTopLevelItem(root);
    }

    ui->treeWidget->setCursor(Qt::ArrowCursor);
    ui->pushButton->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_pushButton_clicked()
{
    QString methodName = ui->comboBox->currentText();

    m_api.retrieveJsonDocument(methodName);

    ui->treeWidget->setCursor(Qt::WaitCursor);
    ui->pushButton->setCursor(Qt::WaitCursor);
}
