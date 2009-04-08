#include "startupdialog.h"
#include "ui_startupDialog.h"

StartupDialog::StartupDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::StartupDialogClass)
{
    ui->setupUi(this);

    /**
     * Connections.
     */
    connect(ui->newProjectCmdButton, SIGNAL(clicked()),
            this, SIGNAL(newProject()));
    connect(ui->newProjectCmdButton, SIGNAL(clicked()),
            this, SIGNAL(accepted()));
    connect(ui->imageProjectCmdButton, SIGNAL(clicked()),
            this, SIGNAL(imageProject()));
    connect(ui->openProjectCmdButton, SIGNAL(clicked()),
            this, SIGNAL(openProject()));
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
