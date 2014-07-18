#ifndef CODEFORCESAPI_H
#define CODEFORCESAPI_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class CodeforcesAPI : public QObject
{
    Q_OBJECT

private:
    QString m_apiBase;
    QNetworkAccessManager m_networkManager;
    QUrl m_currentUrl;
    QNetworkRequest *m_currentRequest;
    QNetworkReply *m_currentReply;

    void releaseRequestAndReply();

public slots:
    void dataArrived();

public:
    explicit CodeforcesAPI(QObject *parent = 0);
    ~CodeforcesAPI();

    void loadData(QString apiMethod);
};

#endif // CODEFORCESAPI_H
