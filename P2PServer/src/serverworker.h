#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>
class QJsonObject;
class ServerWorker : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ServerWorker)
public:
	explicit ServerWorker(QObject *parent = nullptr);
	virtual bool setSocketDescriptor(qintptr socketDescriptor);
	QString userName() const;
	void setUserName(QString const& sUserName);
	void sendJson(QJsonObject const& jsonData);
signals:
	void jsonReceived(QJsonObject const& jsonDoc);
	void disconnectedFromClient();
	void error();
	void logMessage(QString const& msg);
public slots:
	void disconnectFromClient();
private slots:
	void receiveJson();
private:
	QTcpSocket* m_pServerSocket;
	QString m_sUserName;
};

#endif // SERVERWORKER_H