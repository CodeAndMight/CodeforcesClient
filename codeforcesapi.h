#ifndef CODEFORCESAPI_H
#define CODEFORCESAPI_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>

class CodeforcesAPI : public QObject
{
    Q_OBJECT

private:
    QString m_apiBase;
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_currentReply;
    QJsonDocument m_resultJsonDocument;

public slots:
    void receivedJsonDocument();

signals:
    void readyJsonDocument();

public:
    explicit CodeforcesAPI(QObject *parent = 0);
    ~CodeforcesAPI();

    void retrieveJsonDocument(const QString &methodName);

    const QJsonDocument &resultJsonDocument() const;
    const QString &apiBase() const;
};

#endif // CODEFORCESAPI_H
