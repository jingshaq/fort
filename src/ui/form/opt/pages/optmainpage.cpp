#include "optmainpage.h"

#include <QIcon>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QUrl>
#include <QVBoxLayout>

#include <conf/firewallconf.h>
#include <form/controls/controlutil.h>
#include <form/opt/optionscontroller.h>
#include <form/tray/trayicon.h>
#include <fortsettings.h>
#include <manager/windowmanager.h>
#include <util/iconcache.h>

#include "addressespage.h"
#include "applicationspage.h"
#include "graphpage.h"
#include "optionspage.h"
#include "schedulepage.h"
#include "statisticspage.h"

OptMainPage::OptMainPage(OptionsController *ctrl, QWidget *parent) : OptBasePage(ctrl, parent)
{
    setupUi();
}

void OptMainPage::onRetranslateUi()
{
    m_tabWidget->setTabText(0, tr("Options"));
    m_tabWidget->setTabText(1, tr("IP Addresses"));
    m_tabWidget->setTabText(2, tr("Application Groups"));
    m_tabWidget->setTabText(3, tr("Statistics"));
    m_tabWidget->setTabText(4, tr("Traffic Graph"));
    m_tabWidget->setTabText(5, tr("Schedule"));

    m_btOk->setText(tr("OK"));
    m_btApply->setText(tr("Apply"));
    m_btCancel->setText(tr("Cancel"));
}

void OptMainPage::setupUi()
{
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(6, 6, 6, 6);

    // Main Tab Bar
    setupTabBar();
    layout->addWidget(m_tabWidget);

    // Dialog buttons
    auto buttonsLayout = setupDialogButtons();
    layout->addLayout(buttonsLayout);

    this->setLayout(layout);
}

void OptMainPage::setupTabBar()
{
    auto optionsPage = new OptionsPage(ctrl());
    auto addressesPage = new AddressesPage(ctrl());
    auto applicationsPage = new ApplicationsPage(ctrl());
    auto statisticsPage = new StatisticsPage(ctrl());
    auto graphPage = new GraphPage(ctrl());
    auto schedulePage = new SchedulePage(ctrl());

    m_pages = { optionsPage, addressesPage, applicationsPage, statisticsPage, graphPage,
        schedulePage };

    m_tabWidget = new QTabWidget();
    m_tabWidget->addTab(ControlUtil::wrapToScrollArea(optionsPage),
            IconCache::icon(":/icons/cog.png"), QString());
    m_tabWidget->addTab(addressesPage, IconCache::icon(":/icons/ip.png"), QString());
    m_tabWidget->addTab(
            applicationsPage, IconCache::icon(":/icons/application_double.png"), QString());
    m_tabWidget->addTab(statisticsPage, IconCache::icon(":/icons/chart_bar.png"), QString());
    m_tabWidget->addTab(graphPage, IconCache::icon(":/icons/action_log.png"), QString());
    m_tabWidget->addTab(schedulePage, IconCache::icon(":/icons/clock.png"), QString());

    // Menu button
    m_btMenu = ControlUtil::createLinkButton(":/icons/large_tiles.png");
    m_btMenu->setMenu(windowManager()->trayIcon()->menu());

    m_tabWidget->setCornerWidget(m_btMenu);

    connect(m_tabWidget, &QTabWidget::currentChanged, this,
            [&](int tabIndex) { m_pages[tabIndex]->onPageActivated(); });
}

QLayout *OptMainPage::setupDialogButtons()
{
    auto layout = new QHBoxLayout();

    layout->addStretch();

    m_btOk = new QPushButton();
    m_btApply = new QPushButton();
    m_btCancel = new QPushButton();

    connect(m_btOk, &QAbstractButton::clicked, ctrl(), &OptionsController::saveChanges);
    connect(m_btApply, &QAbstractButton::clicked, ctrl(), &OptionsController::applyChanges);
    connect(m_btCancel, &QAbstractButton::clicked, ctrl(), &OptionsController::closeWindow);

    setupApplyCancelButtons();

    layout->addWidget(m_btOk);
    layout->addWidget(m_btApply);
    layout->addWidget(m_btCancel);

    return layout;
}

void OptMainPage::setupApplyCancelButtons()
{
    const auto refreshButtons = [&](bool anyEdited) {
        m_btApply->setEnabled(anyEdited);
        m_btCancel->setEnabled(anyEdited);
    };

    refreshButtons(ctrl()->anyEdited());

    connect(ctrl(), &OptionsController::editedChanged, this, refreshButtons);
}
