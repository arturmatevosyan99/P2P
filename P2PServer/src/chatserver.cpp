#include "chatserver.h"
#include "serverworker.h"
#include <QThread>
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

ChatServer::ChatServer(QObject *parent)
	: QTcpServer(parent)
{}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
	ServerWorker* worker = new ServerWorker(this);
	if (!worker->setSocketDescriptor(socketDescriptor)) 
	{
		worker->deleteLater();
		return;
	}

	connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&ChatServer::userDisconnected, this, worker));
	connect(worker, &ServerWorker::error, this, std::bind(&ChatServer::userError, this, worker));
	connect(worker, &ServerWorker::jsonReceived, this, std::bind(&ChatServer::jsonReceived, this, worker, std::placeholders::_1));
	connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);

	m_vecClients.append(worker);
	emit logMessage(QStringLiteral("New client Connected"));
}
void ChatServer::sendJson(ServerWorker* destination, const QJsonObject &message)
{
	Q_ASSERT(destination);
	destination->sendJson(message);
}
void ChatServer::broadcast(QJsonObject const& message, ServerWorker* exclude)
{
	for (ServerWorker *worker : m_vecClients) 
	{
		Q_ASSERT(worker);
		if (worker == exclude)
			continue;
		sendJson(worker, message);
	}
}

void ChatServer::jsonReceived(ServerWorker* sender, QJsonObject const& doc)
{
	Q_ASSERT(sender);
	emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
	if (sender->userName().isEmpty())
		return jsonFromLoggedOut(sender, doc);
	jsonFromLoggedIn(sender, doc);
}

void ChatServer::userDisconnected(ServerWorker* sender)
{
	m_vecClients.removeAll(sender);
	const QString userName = sender->userName();
	if (!userName.isEmpty()) 
	{
		QJsonObject disconnectedMessage;
		disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
		disconnectedMessage[QStringLiteral("username")] = userName;
		broadcast(disconnectedMessage, nullptr);
		emit logMessage(userName + QLatin1String(" disconnected"));
	}
	sender->deleteLater();
}

void ChatServer::userError(ServerWorker* sender)
{
	Q_UNUSED(sender)
	emit logMessage(QLatin1String("Error from ") + sender->userName());
}

void ChatServer::stopServer()
{
	for (ServerWorker* worker : m_vecClients) 
	{
		worker->disconnectFromClient();
	}
	close();
}

void ChatServer::jsonFromLoggedOut(ServerWorker* sender, QJsonObject const& docObj)
{
	Q_ASSERT(sender);
	const QJsonValue typeVal = docObj.value(QLatin1String("type"));
	if (typeVal.isNull() || !typeVal.isString())
		return;
	if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
		return;
	const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
	if (usernameVal.isNull() || !usernameVal.isString())
		return;
	const QString newUserName = usernameVal.toString().simplified();
	if (newUserName.isEmpty())
		return;
	for (ServerWorker* worker : qAsConst(m_vecClients)) 
	{
		if (worker == sender)
			continue;
		if (worker->userName().compare(newUserName, Qt::CaseInsensitive) == 0)
		{
			QJsonObject message;
			message[QStringLiteral("type")] = QStringLiteral("login");
			message[QStringLiteral("success")] = false;
			message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
			sendJson(sender, message);
			return;
		}
	}
	sender->setUserName(newUserName);
	QJsonObject successMessage;
	successMessage[QStringLiteral("type")] = QStringLiteral("login");
	successMessage[QStringLiteral("success")] = true;
	sendJson(sender, successMessage);
	
	QJsonObject connectedMessage;
	connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
	connectedMessage[QStringLiteral("username")] = newUserName;
	broadcast(connectedMessage, sender);

	for (ServerWorker* worker : qAsConst(m_vecClients)) 
	{
		if (worker == sender)
			continue;
		QString sUserName = worker->userName();
		{
			QJsonObject message;
			message[QStringLiteral("type")] = QStringLiteral("newuser");
			message[QStringLiteral("username")] = sUserName;
			broadcast(message, worker);
		}
	}
}

void ChatServer::jsonFromLoggedIn(ServerWorker* sender, QJsonObject const& docObj)
{
	Q_ASSERT(sender);
	const QJsonValue typeVal = docObj.value(QLatin1String("type"));
	if (typeVal.isNull() || !typeVal.isString())
		return;
	if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
		return;

	const QJsonValue textVal = docObj.value(QLatin1String("text"));
	if (textVal.isNull() || !textVal.isString())
		return;

	const QString text = textVal.toString().trimmed();
	if (text.isEmpty())
		return;

	const QJsonValue receiver = docObj.value(QLatin1String("receiver"));
	if (receiver.isNull() || !receiver.isString())
		return;

	QString const& sReceiver = receiver.toString();
	if (sReceiver.isEmpty())
		return;

	QJsonObject message;
	message[QStringLiteral("type")] = QStringLiteral("message");
	message[QStringLiteral("text")] = text;
	message[QStringLiteral("sender")] = sender->userName();

	for (ServerWorker* worker : qAsConst(m_vecClients)) 
	{
		if (worker == sender)
			continue;
		if (worker->userName().compare(sReceiver, Qt::CaseInsensitive) == 0) 
		{
			sendJson(worker, message);
			return;
		}
	}
}


