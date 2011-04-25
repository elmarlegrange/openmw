#include <QtGui>
#include <QDebug>

#include "maindialog.hpp"
#include "playpage.hpp"
#include "datafilespage.hpp"

MainDialog::MainDialog()
{
    /* TODO: Should be an install path
    QFile file("apps/launcher/resources/launcher.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qDebug() << styleSheet;
    setStyleSheet(styleSheet);*/

    mIconWidget = new QListWidget;
    mIconWidget->setViewMode(QListView::IconMode);
    mIconWidget->setWrapping(false);

    mIconWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Just to be sure

    //mIconWidget->setItemDelegate(new ListViewDelegate());
    mIconWidget->setAttribute(Qt::WA_MacShowFocusRect); // Show a focus frame around the icons on Mac
    //mIconWidget->setLayoutMode(QListView::SinglePass);
    mIconWidget->setUniformItemSizes(true);

    mIconWidget->setIconSize(QSize(48, 48));
    mIconWidget->setMovement(QListView::Static);

    mIconWidget->setMinimumWidth(400);
    mIconWidget->setFixedHeight(80);
    mIconWidget->setSpacing(4);
    mIconWidget->setCurrentRow(0);

    mIconWidget->setFlow(QListView::LeftToRight);

    QGroupBox *groupBox = new QGroupBox(this);
    QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);

    // TODO: TESTING

    /*mProfileModel = new QStringListModel();
    QStringList profileList;
    profileList << "Default" << "Warrior" << "Redemption" << "Cool stuff bro!";
    mProfileModel->setStringList(profileList);
    */

    // Various pages
    mPlayPage = new PlayPage(this);
    //mPlayPage->mProfileModel->setStringList(profileList);

    mDataFilesPage = new DataFilesPage(this);
    //mDataFilesPage->mProfileComboBox->setModel(mProfileModel);

    mPagesWidget = new QStackedWidget(groupBox);
    mPagesWidget->addWidget(mPlayPage);
    mPagesWidget->addWidget(new PlayPage);
    mPagesWidget->addWidget(mDataFilesPage);
    //mPagesWidget->addWidget(new QueryPage);

    groupLayout->addWidget(mPagesWidget);

    QPushButton *playButton = new QPushButton(tr("Play"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    buttonBox->addButton(playButton, QDialogButtonBox::AcceptRole);

    //QSpacerItem *vSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(mIconWidget);
    //dialogLayout->addItem(vSpacer1);
    dialogLayout->addWidget(groupBox);

    dialogLayout->addWidget(buttonBox);
    //mainLayout->addStretch(1);
    //mainLayout->addSpacing(12);

    setWindowTitle(tr("OpenMW Launcher"));
    setMinimumSize(QSize(550, 450));

    createIcons();

    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(play()));
}

void MainDialog::createIcons()
{
    //QSize itemSize(80, 66);

    QListWidgetItem *configButton = new QListWidgetItem(mIconWidget);
    configButton->setIcon(QIcon(":/images/openmw-icon.png"));
    configButton->setText(tr("Play"));
    configButton->setTextAlignment(Qt::AlignCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //configButton->setSizeHint(itemSize);

    QListWidgetItem *updateButton = new QListWidgetItem(mIconWidget);
    updateButton->setIcon(QIcon::fromTheme("video-display"));
    updateButton->setText(tr("Graphics"));
    updateButton->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom | Qt::AlignAbsolute);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //updateButton->setSizeHint(itemSize);

    QListWidgetItem *queryButton = new QListWidgetItem(mIconWidget);
    queryButton->setIcon(QIcon(":/images/openmw-plugin-icon.png"));
    queryButton->setText(tr("Data Files"));
    queryButton->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //queryButton->setSizeHint(itemSize);

    connect(mIconWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

}

void MainDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    mPagesWidget->setCurrentIndex(mIconWidget->row(current));

    if (previous) {
        QString previousPage = previous->data(Qt::DisplayRole).toString();
        QString currentPage = current->data(Qt::DisplayRole).toString();

        // The user switched from Data Files to Play
        if (previousPage == QString("Data Files") && currentPage == QString("Play")) {
            mPlayPage->mProfilesModel->setStringList(mDataFilesPage->mProfilesModel->stringList());
            mPlayPage->mProfilesComboBox->setCurrentIndex(mDataFilesPage->mProfilesComboBox->currentIndex());
        }

        // The user switched from Play to Data Files
        if (previousPage == QString("Play") && currentPage == QString("Data Files")) {
            mDataFilesPage->mProfilesComboBox->setCurrentIndex(mPlayPage->mProfilesComboBox->currentIndex());
        }
    }

}

void MainDialog::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event";
    mDataFilesPage->writeConfig();
    event->accept();

}

void MainDialog::play()
{

}