#include "codeforcesapi.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

CodeforcesAPI::CodeforcesAPI(QObject *parent) : QObject(parent)
{
    m_apiBase = "http://codeforces.ru/api/";
    m_currentRequest = NULL;
    m_currentReply = NULL;
}

CodeforcesAPI::~CodeforcesAPI()
{
    this->releaseRequestAndReply();
}

void CodeforcesAPI::releaseRequestAndReply()
{
    if (m_currentRequest != NULL) {
        delete m_currentRequest;
    }
    if (m_currentReply != NULL) {
        delete m_currentReply;
    }
}

void CodeforcesAPI::loadData(QString apiMethod)
{
    QString urlString = m_apiBase;
    urlString.append(apiMethod);

    m_currentUrl = urlString;
    if (m_currentReply != NULL && !m_currentReply->isFinished()) {
        return;
    }
    this->releaseRequestAndReply();

    m_currentRequest = new QNetworkRequest(m_currentUrl);
    m_currentReply = m_networkManager.get(*m_currentRequest);

    QObject::connect(m_currentReply, SIGNAL(finished()), this, SLOT(dataArrived()));
}

void CodeforcesAPI::dataArrived()
{
    qDebug() << m_currentReply->readBufferSize();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_currentReply->readAll());
}
