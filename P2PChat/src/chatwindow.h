#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QAbstractSocket>
#include <QStandardItemModel>
#include <QListWidget>
#include <QWidget>

class ChatClient;
class QListWidgetItem;

namespace Ui
{
	class ChatWindow;
}

class CQStandardItemModel : public QStandardItemModel
{
public:
	explicit CQStandardItemModel(QObject* parent = nullptr)
		: QStandardItemModel(parent)
	{ }

	using QStandardItemModel::setData;
	using QStandardItemModel::data;

	void setData(QVariant const& vtData)
	{
		m_vtData = vtData;
	}

	QVariant data() const
	{
		return m_vtData;
	}

private:
	QVariant m_vtData;
};

class CQListWidgetItem : public QListWidgetItem
{
public:
	explicit CQListWidgetItem(QListWidget* parent = nullptr)
		: QListWidgetItem(parent)
	{ }

	explicit CQListWidgetItem(const QString& text, QListWidget* parent = nullptr)
		: QListWidgetItem(text, parent)
	{ }

	using QListWidgetItem::setData;
	using QListWidgetItem::data;

	void setData(QVariant const& vtData)
	{
		m_vtData = vtData;
	}

	QVariant data() const
	{
		return m_vtData;
	}

private:
	QVariant m_vtData;
};

class ChatWindow : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(ChatWindow)

public:
	explicit ChatWindow(QWidget *parent = nullptr);
	~ChatWindow();

private:
	void updateUserChatView();

private:
	void closeEvent(QCloseEvent* pEvent) override;

private slots:
	void attemptConnection();
	void connectedToServer();
	void attemptLogin(QString const& sUserName);
	void loggedIn();
	void loginFailed(QString const& sReason);
	void messageReceived(QString const& sSender, QString const& sText);
	void sendMessage();
	void disconnectedFromServer();
	void userJoined(QString const& sUserName);
	void userLeft(QString const& sUserName);
	void error(QAbstractSocket::SocketError socketError);

	void onChatChanged();

private:
	Ui::ChatWindow* ui;
	ChatClient* m_pChatClient;
	QHash<QString, CQStandardItemModel*> m_mapChatModels;
};

#endif // CHATWINDOW_H