#include "serverdialog.h"

#include "ui_serverdialog.h"

ServerDialog::ServerDialog(QWidget* parent)
	: QDialog(parent),
	ui(new Ui::ServerDialog),
	pAddress(nullptr),
	pPort(nullptr)
{
	ui->setupUi(this);

	connect(this, &QDialog::accepted, this, &ServerDialog::onAccepted);
}

ServerDialog::ServerDialog(QWidget* parent, QString const& sDefaultAddr, int nDefaultPort, QString& sAddress, int& nPort)
	: QDialog(parent),
	ui(new Ui::ServerDialog),
	pAddress(&sAddress),
	pPort(&nPort)
{
	ui->setupUi(this);

	connect(this, &QDialog::accepted, this, &ServerDialog::onAccepted);

	ui->serverLineEdit->setText(sDefaultAddr);
	ui->portSpinBox->setValue(nDefaultPort);
}

ServerDialog::~ServerDialog()
{
	// delete the elements created from the .ui file
	delete ui;
}

void ServerDialog::onAccepted()
{
	if (pAddress)
		*pAddress = ui->serverLineEdit->text();

	if (pPort)
		*pPort = ui->portSpinBox->value();
}

void ServerDialog::getInput(QWidget* parent, QString const& sDefaultAddr, int nDefaultPort, QString& sAddress, int& nPort)
{
	ServerDialog dialog(parent, sDefaultAddr, nDefaultPort, sAddress, nPort);
	dialog.exec();
}
