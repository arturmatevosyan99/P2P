#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class QHostAddress;
class QJsonDocument;

class ChatClient : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ChatClient)

	friend class ChatWindow;

public:
	explicit ChatClient(QObject *parent = nullptr);
	QString getName() const;

public slots:
	void connectToServer(QHostAddress const& address, quint16 port);
	void login(QString const& userName);
	void sendMessage(QString const& sText, QString const& sReceiver);
	void disconnectFromHost();

private slots:
	void onReadyRead();
signals:
	void connected();
	void loggedIn();
	void loginError(QString const& sReason);
	void disconnected();
	void messageReceived(QString const& sSender, QString const& sText);
	void error(QAbstractSocket::SocketError socketError);
	void userJoined(QString const& sUserName);
	void userLeft(QString const& sUserName);

private:
	QTcpSocket* m_pClientSocket;
	bool m_bLoggedIn;
	QString m_sName;
	void jsonReceived(QJsonObject const& doc);
};

#endif // CHATCLIENT_H