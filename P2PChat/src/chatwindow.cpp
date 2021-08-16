#include "chatwindow.h"

#include <QHostAddress>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>

#include "chatclient.h"
#include "serverdialog.h"
#include "ui_chatwindow.h"


ChatWindow::ChatWindow(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::ChatWindow),
	m_pChatClient(new ChatClient(this))
{
	ui->setupUi(this);
	
	connect(m_pChatClient, &ChatClient::connected, this, &ChatWindow::connectedToServer);
	connect(m_pChatClient, &ChatClient::loggedIn, this, &ChatWindow::loggedIn);
	connect(m_pChatClient, &ChatClient::loginError, this, &ChatWindow::loginFailed);
	connect(m_pChatClient, &ChatClient::messageReceived, this, &ChatWindow::messageReceived);
	connect(m_pChatClient, &ChatClient::disconnected, this, &ChatWindow::disconnectedFromServer);
	connect(m_pChatClient, &ChatClient::error, this, &ChatWindow::error);
	connect(m_pChatClient, &ChatClient::userJoined, this, &ChatWindow::userJoined);
	connect(m_pChatClient, &ChatClient::userLeft, this, &ChatWindow::userLeft);

	attemptConnection();

	connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
	connect(ui->messageEdit, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);

	connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &ChatWindow::onChatChanged);
}

ChatWindow::~ChatWindow()
{
	delete ui;
}

void ChatWindow::updateUserChatView()
{
	CQListWidgetItem* pItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
	if (!pItem)
		return;

	QString sUserName = pItem->data().toString();
	assert(!sUserName.isEmpty());

	QBrush brush = pItem->data(Qt::ForegroundRole).value<QBrush>();
	bool bUserActive = !(brush.color() == Qt::darkRed);

	ui->chatView->setEnabled(bUserActive);
	ui->sendButton->setEnabled(bUserActive);
	ui->messageEdit->setEnabled(bUserActive);
}

void ChatWindow::closeEvent(QCloseEvent* pEvent)
{
	disconnect(m_pChatClient, &ChatClient::disconnected, this, &ChatWindow::disconnectedFromServer);
	m_pChatClient->disconnectFromHost();
}

void ChatWindow::attemptConnection()
{
	// Ask user for the address of the server, 127.0.0.1 is used as default
	QString sHostAddress = "";
	int nPort = -1;
	ServerDialog::getInput(this, "127.0.0.1", 1967, sHostAddress, nPort);

	if (sHostAddress.isEmpty() || nPort == -1)
		return;

	m_pChatClient->connectToServer(QHostAddress(sHostAddress), nPort);
}

void ChatWindow::connectedToServer()
{
	// once connected to the server, ask the user for what username they would like to use
	QString sNewUsername = QInputDialog::getText(this, tr("Choose Username"), tr("Username"));

	QRegExp re("^[\\da-zA-Z_]*$");
	while (sNewUsername.isEmpty() || !re.exactMatch(sNewUsername))
	{
		if (sNewUsername.isEmpty())
			return m_pChatClient->disconnectFromHost();

		QMessageBox::warning(this, tr("Error"), tr("Invalid username. Please try another one"));
		sNewUsername = QInputDialog::getText(this, tr("Choose Username"), tr("Username"));
	}

	attemptLogin(sNewUsername);
}

void ChatWindow::attemptLogin(const QString& userName)
{
	m_pChatClient->login(userName);
}

void ChatWindow::loggedIn()
{
	ui->loginLabel->setText("Logged in as: <b>" + m_pChatClient->getName() + "</b>");
	// once successully logged in, enable the ui to display and send messages
	ui->sendButton->setEnabled(false);
	ui->messageEdit->setEnabled(false);
	ui->chatView->setEnabled(true);
	ui->listWidget->setEnabled(true);
}

void ChatWindow::loginFailed(QString const& sReason)
{
	ui->loginLabel->setText("Not logged in");
	// the server rejected the login attempt
	QMessageBox::critical(this, tr("Error"), sReason);
	// allow the user to retry, execute the same slot as when just connected
	connectedToServer();
}

void ChatWindow::messageReceived(QString const& sSender, QString const& sText)
{
	assert(m_mapChatModels.contains(sSender));

	CQStandardItemModel* pModel = m_mapChatModels[sSender];

	int nRowCount = pModel->rowCount();
	if (pModel->data().toBool() || nRowCount == 0) 
	{
		// set the flag to false
		pModel->setData(false);
		QFont boldFont;
		boldFont.setBold(true);
		
		pModel->insertRows(nRowCount, 2);
		
		pModel->setData(pModel->index(nRowCount, 0), sSender + QLatin1Char(':'));
		
		pModel->setData(pModel->index(nRowCount, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
		
		pModel->setData(pModel->index(nRowCount, 0), boldFont, Qt::FontRole);
		pModel->setData(pModel->index(nRowCount, 0), QBrush(QColor("#66B2FF")), Qt::ForegroundRole);

		++nRowCount;
	}
	else 
	{
		pModel->insertRow(nRowCount);
	}

	pModel->setData(pModel->index(nRowCount, 0), sText);
	pModel->setData(pModel->index(nRowCount, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
	
	auto* pCurrentItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
	if (pCurrentItem)
	{
		QString sCurrentUser = pCurrentItem->data().toString();
		if (sCurrentUser.compare(sSender, Qt::CaseInsensitive) == 0)
		{
			ui->chatView->scrollToBottom();
			return;
		}
	}

	for (qint32 nIndex = 0; nIndex < ui->listWidget->count(); ++nIndex)
	{
		auto* pItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->item(nIndex));
		if (!pItem)
			continue;
		QString sUserName = pItem->data().toString();
		if (sUserName.compare(sSender, Qt::CaseInsensitive) == 0)
		{
			if (!pItem->text().endsWith("*"))
				pItem->setText(pItem->text() + "*");
			return;
		}
	}
}

void ChatWindow::sendMessage()
{
	if (ui->messageEdit->text().isEmpty())
		return;

	auto* pCurrentItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
	if (!pCurrentItem)
		return;
	QString sCurrentUser = pCurrentItem->data().toString();

	m_pChatClient->sendMessage(ui->messageEdit->text(), sCurrentUser);

	CQStandardItemModel* pModel = m_mapChatModels[sCurrentUser];
	if (!pModel)
		return;

	int nRowCount = pModel->rowCount();

	if (!pModel->data().toBool() || nRowCount == 0)
	{
		QFont boldFont;
		boldFont.setBold(true);

		pModel->insertRows(nRowCount, 2);
		
		pModel->setData(pModel->index(nRowCount, 0), tr("Me:"));
		
		pModel->setData(pModel->index(nRowCount, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
		pModel->setData(pModel->index(nRowCount, 0), boldFont, Qt::FontRole);
		pModel->setData(pModel->index(nRowCount, 0), QBrush(QColor("#66B2FF")), Qt::ForegroundRole);

		++nRowCount;
	}
	else
	{
		pModel->insertRow(nRowCount);
	}

	pModel->setData(pModel->index(nRowCount, 0), ui->messageEdit->text());
	pModel->setData(pModel->index(nRowCount, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);

	ui->messageEdit->clear();

	ui->chatView->scrollToBottom();

	// reset the flag for last printed username
	pModel->setData(true);
}

void ChatWindow::disconnectedFromServer()
{
	// client lost the connection to the server
	QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
	
	ui->sendButton->setEnabled(false);
	ui->messageEdit->setEnabled(false);
	ui->chatView->setEnabled(false);
	ui->listWidget->setEnabled(false);
}

void ChatWindow::userJoined(QString const& sUserName)
{
	if (m_mapChatModels.contains(sUserName))
	{
		for (qint32 nIndex = 0; nIndex < ui->listWidget->count(); ++nIndex)
		{
			auto* pItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->item(nIndex));
			if (!pItem)
				continue;

			QString sItemUserName = pItem->data().toString();

			if (sUserName.compare(sItemUserName, Qt::CaseInsensitive) == 0)
			{
				pItem->setData(Qt::ForegroundRole, QBrush(Qt::black));

				auto* pCurrentItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
				if (pCurrentItem)
				{
					QString sCurrentUser = pCurrentItem->data().toString();;
					if (sCurrentUser.compare(sUserName, Qt::CaseInsensitive) == 0)
						updateUserChatView();
				}

				break;
			}
		}

		return;
	}

	CQStandardItemModel* pModel = new CQStandardItemModel(this);
	pModel->insertColumn(0);
	pModel->setData(false);

	m_mapChatModels[sUserName] = pModel;

	CQListWidgetItem* pNewItem = new CQListWidgetItem(sUserName, ui->listWidget);
	pNewItem->setData(sUserName);
	ui->listWidget->addItem(pNewItem);
}

void ChatWindow::userLeft(QString const& sUserName)
{
	for (qint32 nIndex = 0; nIndex < ui->listWidget->count(); ++nIndex)
	{
		auto* pItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->item(nIndex));
		if (!pItem)
			continue;

		QString sItemUserName = pItem->data().toString();

		if (sUserName.compare(sItemUserName, Qt::CaseInsensitive) == 0)
		{
			pItem->setData(Qt::ForegroundRole, QBrush(Qt::darkRed));

			auto* pCurrentItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
			if (pCurrentItem)
			{
				QString sCurrentUser = pCurrentItem->data().toString();
				if (sCurrentUser.compare(sUserName, Qt::CaseInsensitive) == 0)
					updateUserChatView();
			}

			break;
		}
	}
}

void ChatWindow::error(QAbstractSocket::SocketError socketError)
{
	switch (socketError) 
	{
	case QAbstractSocket::RemoteHostClosedError:
	case QAbstractSocket::ProxyConnectionClosedError:
		return; // handled by disconnectedFromServer
	case QAbstractSocket::ConnectionRefusedError:
	{
		QMessageBox msgBox(QMessageBox::Critical, tr("Error"), tr("Server refused the connection"), QMessageBox::Retry);
		msgBox.setDefaultButton(QMessageBox::Retry);
		connect(&msgBox, &QMessageBox::buttonClicked, this, &ChatWindow::attemptConnection);
		msgBox.exec();
		break;
	}
	case QAbstractSocket::ProxyConnectionRefusedError:
		QMessageBox::critical(this, tr("Error"), tr("Proxy refused the connection"));
		break;
	case QAbstractSocket::ProxyNotFoundError:
		QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
		break;
	case QAbstractSocket::HostNotFoundError:
		QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
		break;
	case QAbstractSocket::SocketAccessError:
		QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
		break;
	case QAbstractSocket::SocketResourceError:
		QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
		break;
	case QAbstractSocket::SocketTimeoutError:
		QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
		return;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
		break;
	case QAbstractSocket::NetworkError:
		QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
		break;
	case QAbstractSocket::UnknownSocketError:
		QMessageBox::critical(this, tr("Error"), tr("An unknown error occured"));
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
		break;
	case QAbstractSocket::ProxyProtocolError:
		QMessageBox::critical(this, tr("Error"), tr("Proxy comunication failed"));
		break;
	case QAbstractSocket::TemporaryError:
	case QAbstractSocket::OperationError:
		QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
		return;
	default:
		Q_UNREACHABLE();
	}

	ui->sendButton->setEnabled(false);
	ui->messageEdit->setEnabled(false);
	ui->chatView->setEnabled(false);
}

void ChatWindow::onChatChanged()
{
	auto* pItem = dynamic_cast<CQListWidgetItem*>(ui->listWidget->currentItem());
	if (!pItem)
		return;

	QString sUserName = pItem->data().toString();
	QStandardItemModel* pModel = m_mapChatModels[sUserName];
	ui->chatView->setModel(pModel);

	QString sItemText = pItem->text();
	if (sItemText.endsWith("*"))
		pItem->setText(sItemText.left(sItemText.length() - 1));

	updateUserChatView();
}
