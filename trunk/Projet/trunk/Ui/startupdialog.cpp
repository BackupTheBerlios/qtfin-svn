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
            this, SLOT(newEmptyProject()));
    connect(ui->imageProjectCmdButton, SIGNAL(clicked()),
            this, SLOT(imageProject()));
    connect(ui->openProjectCmdButton, SIGNAL(clicked()),
            this, SLOT(openProject()));

    /**
     * Event filters.
     */
    ui->newProjectCmdButton->installEventFilter(this);
    ui->imageProjectCmdButton->installEventFilter(this);
    ui->openProjectCmdButton->installEventFilter(this);
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

bool StartupDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->newProjectCmdButton) {
        if (event->type() == QEvent::MouseMove) {
            ui->label->setText(tr("Create a new empty project."));
            return true;
        } else
            return false;
    } else if (obj == ui->imageProjectCmdButton) {
        if (event->type() == QEvent::MouseMove) {
            ui->label->setText(tr("Create a new project from an image."));
            return true;
        } else
            return false;
    } else if (obj == ui->openProjectCmdButton) {
        if (event->type() == QEvent::MouseMove) {
            ui->label->setText(tr("Open an existing project."));
            return true;
        } else
            return false;
    } else
        return QDialog::eventFilter(obj, event);

}

void StartupDialog::retranslateUi()
{

    ui->retranslateUi(this);
}
