#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QWidget>

namespace Ui
{
	class ServerDialog;
}

class ServerDialog : public QDialog
{
	explicit ServerDialog(QWidget* parent = nullptr);
	explicit ServerDialog(QWidget* parent, QString const& sDefaultAddr, int nDefaultPort, QString& sAddress, int& nPort);
	~ServerDialog();

public:
	static void getInput(QWidget* parent, QString const& sDefaultAddr, int nDefaultPort, QString& sAddress, int& nPort);

private slots:
	void onAccepted();

private:
	Ui::ServerDialog* ui;
	QString* pAddress;
	int* pPort;
};

#endif