#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <form/windowtypes.h>
#include <util/window/widgetwindow.h>

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class ConfManager;
class FortSettings;
class HomeController;
class HomeMainPage;
class IniUser;
class WidgetWindowStateWatcher;
class WindowManager;

class HomeWindow : public WidgetWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);

    quint32 windowCode() const override { return WindowHome; }

    HomeController *ctrl() const { return m_ctrl; }
    FortSettings *settings() const;
    ConfManager *confManager() const;
    IniUser *iniUser() const;
    WindowManager *windowManager() const;

    void saveWindowState(bool wasVisible) override;
    void restoreWindowState() override;

public slots:
    void selectAboutTab();

private slots:
    void onActivated();

private:
    void setupController();
    void setupStateWatcher();

    void retranslateUi();

    void setupUi();
    QWidget *setupHeader();
    QLayout *setupLogoText();
    void setupPasswordButtons();
    QLayout *setupDialogButtons();

private:
    HomeController *m_ctrl = nullptr;
    WidgetWindowStateWatcher *m_stateWatcher = nullptr;

    HomeMainPage *m_mainPage = nullptr;
    QToolButton *m_btPasswordLock = nullptr;
    QToolButton *m_btPasswordUnlock = nullptr;
    QPushButton *m_btMenu = nullptr;

    QPushButton *m_btLogs = nullptr;
    QPushButton *m_btProfile = nullptr;
    QPushButton *m_btStat = nullptr;
    QPushButton *m_btReleases = nullptr;
};

#endif // HOMEWINDOW_H
