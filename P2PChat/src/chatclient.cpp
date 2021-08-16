#include "chatclient.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

ChatClient::ChatClient(QObject *parent)
	: QObject(parent),
	  m_pClientSocket(new QTcpSocket(this)),
	  m_bLoggedIn(false)
{
	connect(m_pClientSocket, &QTcpSocket::connected, this, &ChatClient::connected);
	connect(m_pClientSocket, &QTcpSocket::disconnected, this, &ChatClient::disconnected);
	connect(m_pClientSocket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
	connect(m_pClientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ChatClient::error);
	connect(m_pClientSocket, &QTcpSocket::disconnected, this, 
		[this]() -> void 
		{
			m_bLoggedIn = false;
		}
	);
}

QString ChatClient::getName() const
{
	return m_sName;
}

void ChatClient::login(QString const& sUserName)
{
	if (m_pClientSocket->state() == QAbstractSocket::ConnectedState) 
	{
		m_sName = sUserName;
		QDataStream clientStream(m_pClientSocket);
		clientStream.setVersion(QDataStream::Qt_5_15);

		QJsonObject message;
		message[QStringLiteral("type")] = QStringLiteral("login");
		message[QStringLiteral("username")] = sUserName;

		clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
	}
}

void ChatClient::sendMessage(QString const& sText, QString const& sReceiver)
{
	if (sText.isEmpty())
		return;
	
	QDataStream clientStream(m_pClientSocket);
	clientStream.setVersion(QDataStream::Qt_5_15);
	
	QJsonObject message;
	message[QStringLiteral("type")] = QStringLiteral("message");
	message[QStringLiteral("text")] = sText;
	message[QStringLiteral("receiver")] = sReceiver;
	
	clientStream << QJsonDocument(message).toJson();
}

void ChatClient::disconnectFromHost()
{
	m_pClientSocket->disconnectFromHost();
}

void ChatClient::jsonReceived(const QJsonObject &docObj)
{
	const QJsonValue typeVal = docObj.value(QLatin1String("type"));
	if (typeVal.isNull() || !typeVal.isString())
		return;
	if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0) 
	{
		if (m_bLoggedIn)
			return;
		// success field contains the result of login attempt
		const QJsonValue resultVal = docObj.value(QLatin1String("success"));
		if (resultVal.isNull() || !resultVal.isBool())
			return;
		const bool bLoginSuccess = resultVal.toBool();
		if (bLoginSuccess) 
		{
			emit loggedIn();
			return;
		}
		// login attempt failed, so extract the reason of the failure from the JSON
		const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
		emit loginError(reasonVal.toString());
	}
	else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) 
	{
		const QJsonValue textVal = docObj.value(QLatin1String("text"));
		const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
		if (textVal.isNull() || !textVal.isString())
			return;
		if (senderVal.isNull() || !senderVal.isString())
			return;
		
		emit messageReceived(senderVal.toString(), textVal.toString());
	} 
	else if (typeVal.toString().compare(QLatin1String("newuser"), Qt::CaseInsensitive) == 0) 
	{ 
		// A user joined the chat
		const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
		if (usernameVal.isNull() || !usernameVal.isString())
			return;
		
		emit userJoined(usernameVal.toString());
	} 
	else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) 
	{ 
		// A user left the chat
		const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
		if (usernameVal.isNull() || !usernameVal.isString())
			return;
		
		emit userLeft(usernameVal.toString());
	}
}

void ChatClient::connectToServer(QHostAddress const& address, quint16 port)
{
	m_pClientSocket->connectToHost(address, port);
}

void ChatClient::onReadyRead()
{
	QByteArray jsonData;
	QDataStream socketStream(m_pClientSocket);
	socketStream.setVersion(QDataStream::Qt_5_15);

	for (;;) 
	{
		// start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
		socketStream.startTransaction();
		socketStream >> jsonData;
		if (socketStream.commitTransaction()) 
		{
			// we successfully read some data
			QJsonParseError parseError;
			const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
			if (parseError.error == QJsonParseError::NoError) 
			{
				if (jsonDoc.isObject())
					jsonReceived(jsonDoc.object());
			}
		} 
		else 
		{
			// the read failed, the socket goes automatically back to the state it was in before the transaction started
			// just exit the loop and wait for more data to become available
			break;
		}
	}
}
