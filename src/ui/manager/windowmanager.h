#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include <form/windowtypes.h>
#include <util/ioc/iocservice.h>

class GraphWindow;
class HomeWindow;
class MainWindow;
class OptionsWindow;
class PoliciesWindow;
class ProgramsWindow;
class ServicesWindow;
class StatisticsWindow;
class TrayIcon;
class WidgetWindow;
class ZonesWindow;

class WindowManager : public QObject, public IocService
{
    Q_OBJECT

public:
    enum TrayMessageType : qint8 {
        MessageOptions,
        MessageNewVersion,
        MessageZones,
        MessageAlert,
    };
    Q_ENUM(TrayMessageType)

    explicit WindowManager(QObject *parent = nullptr);

    MainWindow *mainWindow() const { return m_mainWindow; }
    HomeWindow *homeWindow() const { return m_homeWindow; }
    ProgramsWindow *progWindow() const { return m_progWindow; }
    PoliciesWindow *policiesWindow() const { return m_policiesWindow; }
    OptionsWindow *optWindow() const { return m_optWindow; }
    StatisticsWindow *connWindow() const { return m_statWindow; }
    ServicesWindow *servicesWindow() const { return m_servicesWindow; }
    ZonesWindow *zonesWindow() const { return m_zonesWindow; }
    GraphWindow *graphWindow() const { return m_graphWindow; }
    TrayIcon *trayIcon() const { return m_trayIcon; }

    void setUp() override;
    void tearDown() override;

    bool isWindowOpen(WindowCode code) const { return isAnyWindowOpen(code); }

    static QFont defaultFont();

signals:
    void windowVisibilityChanged(quint32 code, bool isVisible);

public slots:
    void setupAppPalette();

    void setupTrayIcon();
    void showTrayIcon();
    void closeTrayIcon();
    void showTrayMessage(
            const QString &message, WindowManager::TrayMessageType type = MessageOptions);

    void showHomeWindow();
    void closeHomeWindow();
    void quitHomeWindow(QEvent *event);

    void showHomeWindowAbout();

    void showProgramsWindow();
    void closeProgramsWindow();

    bool showProgramEditForm(const QString &appPath);

    void showOptionsWindow();
    void closeOptionsWindow();
    void reloadOptionsWindow(const QString &reason);

    void showPoliciesWindow();
    void closePoliciesWindow();

    void showStatisticsWindow();
    void closeStatisticsWindow();

    void showServicesWindow();
    void closeServicesWindow();

    void showZonesWindow();
    void closeZonesWindow();

    void showGraphWindow();
    void closeGraphWindow();
    void switchGraphWindow();

    void quit();
    void restart();

    bool checkWindowPassword(WindowCode code);
    bool checkPassword();

    virtual void showErrorBox(
            const QString &text, const QString &title = QString(), QWidget *parent = nullptr);
    virtual void showInfoBox(
            const QString &text, const QString &title = QString(), QWidget *parent = nullptr);
    void showConfirmBox(const std::function<void()> &onConfirmed, const QString &text,
            const QString &title = QString(), QWidget *parent = nullptr);
    void showQuestionBox(const std::function<void(bool confirmed)> &onFinished, const QString &text,
            const QString &title = QString(), QWidget *parent = nullptr);

    static bool activateModalWidget();

private:
    void setupMainWindow();
    void closeMainWindow();

    void setupHomeWindow();
    void setupProgramsWindow();
    void setupOptionsWindow();
    void setupPoliciesWindow();
    void setupServicesWindow();
    void setupZonesWindow();
    void setupGraphWindow();
    void setupStatisticsWindow();

    void closeAll();
    void quitApp();

    void onTrayMessageClicked();

    void showWindow(WidgetWindow *w, bool activate = true);
    void closeWindow(WidgetWindow *w);

    void windowOpened(quint32 code);
    void windowClosed(quint32 code);
    bool isAnyWindowOpen(quint32 codes) const;

private:
    bool m_isAppQuitting = false;

    quint32 m_openedWindows = 0;

    TrayMessageType m_lastMessageType = MessageOptions;

    TrayIcon *m_trayIcon = nullptr;

    MainWindow *m_mainWindow = nullptr;
    HomeWindow *m_homeWindow = nullptr;
    ProgramsWindow *m_progWindow = nullptr;
    OptionsWindow *m_optWindow = nullptr;
    PoliciesWindow *m_policiesWindow = nullptr;
    StatisticsWindow *m_statWindow = nullptr;
    ServicesWindow *m_servicesWindow = nullptr;
    ZonesWindow *m_zonesWindow = nullptr;
    GraphWindow *m_graphWindow = nullptr;
};

#endif // WINDOWMANAGER_H
