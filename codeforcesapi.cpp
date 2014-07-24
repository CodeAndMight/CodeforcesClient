#include "codeforcesapi.h"

#include <QNetworkRequest>

CodeforcesAPI::CodeforcesAPI(QObject *parent) : QObject(parent)
{
    m_apiBase = "http://codeforces.ru/api/";
    m_currentReply = NULL;
}

CodeforcesAPI::~CodeforcesAPI()
{
    delete m_currentReply;
}

void CodeforcesAPI::receivedJsonDocument()
{
    m_resultJsonDocument = QJsonDocument::fromJson(m_currentReply->readAll());

    delete m_currentReply;

    m_currentReply = NULL;

    emit readyJsonDocument();
}

void CodeforcesAPI::retrieveJsonDocument(const QString &methodName)
{
    if (m_currentReply != NULL) {
        return;
    }

    QString apiString = m_apiBase + methodName;
    QUrl apiUrl(apiString);
    QNetworkRequest request(apiUrl);

    m_currentReply = m_networkManager.get(request);

    connect(m_currentReply, SIGNAL(finished()), this, SLOT(receivedJsonDocument()));
}

const QString &CodeforcesAPI::apiBase() const
{
    return m_apiBase;
}

const QJsonDocument &CodeforcesAPI::resultJsonDocument() const
{
    return m_resultJsonDocument;
}
