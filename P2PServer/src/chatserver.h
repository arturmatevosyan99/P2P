#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QVector>

class QThread;
class ServerWorker;

class ChatServer : public QTcpServer
{
	Q_OBJECT
	Q_DISABLE_COPY(ChatServer)

public:
	explicit ChatServer(QObject *parent = nullptr);

protected:
	void incomingConnection(qintptr socketDescriptor) override;

signals:
	void logMessage(QString const& msg);

public slots:
	void stopServer();

private slots:
	void broadcast(QJsonObject const& message, ServerWorker *exclude);
	void jsonReceived(ServerWorker* sender, QJsonObject const& doc);
	void userDisconnected(ServerWorker* sender);
	void userError(ServerWorker* sender);

private:
	void jsonFromLoggedOut(ServerWorker *sender, QJsonObject const& doc);
	void jsonFromLoggedIn(ServerWorker *sender, QJsonObject const& doc);
	void sendJson(ServerWorker* destination, QJsonObject const& message);
	QVector<ServerWorker*> m_vecClients;
};

#endif // CHATSERVER_H