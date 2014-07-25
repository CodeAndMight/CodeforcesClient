#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QSet>

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

void MainWindow::fillTreeWidget(const QJsonValue &jsonValue, QTreeWidgetItem *parent)
{
    if (jsonValue.isObject()) {
        QJsonObject jsonObject = jsonValue.toObject();
        QStringList keys = jsonObject.keys();

        foreach (const QString &key, keys) {
            QJsonValue value = jsonObject.value(key);
            if (value.isArray()) {
                QTreeWidgetItem *item = this->createItemTreeWidget(parent, key);
                this->fillTreeWidget(value, item);
            } else {
                QTreeWidgetItem *item = this->createItemTreeWidget(parent, key);
                this->fillTreeWidget(value, item);
            }
        }

    } else if (jsonValue.isArray()) {
        QJsonArray jsonArray = jsonValue.toArray();

        foreach (const QJsonValue &subJsonValue, jsonArray) {
            if (subJsonValue.isObject()) {
                QJsonObject obj = subJsonValue.toObject();
                QTreeWidgetItem *node = this->createNodeTreeWidget(parent, obj.keys(), obj);
                this->fillTreeWidget(subJsonValue, node);
            } else {
                this->fillTreeWidget(subJsonValue, parent);
            }
        }

    } else if (jsonValue.isString()) {
        parent->setText(1, jsonValue.toString());
    } else if (jsonValue.isBool()) {
        parent->setText(1, QString::number(jsonValue.toBool()));
    } else if (jsonValue.isDouble()) {
        parent->setText(1, QString::number(jsonValue.toInt()));
    }
}

QTreeWidgetItem *MainWindow::createNodeTreeWidget(QTreeWidgetItem *parent, const QStringList &labels, const QJsonObject &jsonObject)
{
    QTreeWidgetItem *node = NULL;

    if (parent) {
        node = new QTreeWidgetItem(parent);
    } else {
        node = new QTreeWidgetItem;
        ui->treeWidget->addTopLevelItem(node);
    }

    QSet<QString> title = QSet<QString>::fromList(labels);
    QSet<QString> keyTitles = QSet<QString>::fromList(m_api.keyTitles());

    title.intersect(keyTitles);

    if (title.isEmpty()) {
        node->setText(0, "*");
    } else {
        QString k = title.values().at(0);
        if (jsonObject.value(k).isString()) {
            node->setText(0, jsonObject.value(k).toString());
        } else {
            node->setText(0, QString::number(jsonObject.value(k).toInt()));
        }
    }

    return node;
}

QTreeWidgetItem *MainWindow::createItemTreeWidget(QTreeWidgetItem *parent, const QString &label)
{
    QTreeWidgetItem *item = NULL;

    if (parent) {
        item = new QTreeWidgetItem(parent);
    } else {
        item = new QTreeWidgetItem;
        ui->treeWidget->addTopLevelItem(item);
    }

    item->setText(0, label);
    return item;
}

void MainWindow::readyRead()
{
    const QJsonDocument &jsonDocumnet = m_api.resultJsonDocument();

    ui->treeWidget->clear();

    QJsonObject result = jsonDocumnet.object();

    this->fillTreeWidget(result);

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
