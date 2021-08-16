#include "serverworker.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

ServerWorker::ServerWorker(QObject* parent)
	: QObject(parent)
	, m_pServerSocket(new QTcpSocket(this))
{
	connect(m_pServerSocket, &QTcpSocket::readyRead, this, &ServerWorker::receiveJson);

	connect(m_pServerSocket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectFromClient);
	connect(m_pServerSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ServerWorker::error);
}


bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor)
{
	return m_pServerSocket->setSocketDescriptor(socketDescriptor);
}

void ServerWorker::sendJson(QJsonObject const& json)
{
	const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
	// notify the central server we are about to send the message
	emit logMessage(QLatin1String("Sending to ") + userName() + QLatin1String(" - ") + QString::fromUtf8(jsonData));
	
	QDataStream socketStream(m_pServerSocket);
	socketStream.setVersion(QDataStream::Qt_5_15);
	socketStream << jsonData;
}

void ServerWorker::disconnectFromClient()
{
	emit disconnectedFromClient();
	m_pServerSocket->disconnectFromHost();
}

QString ServerWorker::userName() const
{
	return m_sUserName;
}

void ServerWorker::setUserName(QString const& sUserName)
{
	m_sUserName = sUserName;
}

void ServerWorker::receiveJson()
{
	QByteArray jsonData;
	QDataStream socketStream(m_pServerSocket);
	socketStream.setVersion(QDataStream::Qt_5_15);

	for (;;) 
	{
		// start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
		socketStream.startTransaction();
		socketStream >> jsonData;
		if (socketStream.commitTransaction()) 
		{
			QJsonParseError parseError;
			const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
			if (parseError.error == QJsonParseError::NoError) 
			{
				if (jsonDoc.isObject())
					emit jsonReceived(jsonDoc.object());
				else
					emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData));
			} 
			else 
			{
				emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData));
			}
		} 
		else 
		{
			// just exit the loop and wait for more data to become available
			break;
		}
	}
}


