#ifndef PROGRAMEDITDIALOG_H
#define PROGRAMEDITDIALOG_H

#include <QDialog>

#include <model/applistmodel.h>

QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QDateTimeEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QRadioButton)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class CheckSpinCombo;
class ConfManager;
class FirewallConf;
class FortManager;
class ProgramsController;

class ProgramEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgramEditDialog(ProgramsController *ctrl, QWidget *parent = nullptr);

    ProgramsController *ctrl() const { return m_ctrl; }
    FortManager *fortManager() const;
    ConfManager *confManager() const;
    FirewallConf *conf() const;
    AppListModel *appListModel() const;

    void initialize(const AppRow &appRow, const QVector<qint64> &appIdList);

    void activate();

private:
    void setupController();

    void retranslateUi();
    void retranslateAppBlockInHours();

    void setupUi();
    QLayout *setupAppLayout();
    QLayout *setupAppPathLayout();
    QLayout *setupAppNameLayout();
    void setupComboAppGroups();
    QLayout *setupLogLayout();
    QLayout *setupAllowLayout();
    QLayout *setupCheckDateTimeEdit();
    void setupAllowEclusiveGroup();

    bool save();
    bool saveApp(App &app);
    bool saveMulti(App &app);

    bool validateFields() const;
    void fillApp(App &app) const;

private:
    ProgramsController *m_ctrl = nullptr;

    QLabel *m_labelEditPath = nullptr;
    QLineEdit *m_editPath = nullptr;
    QToolButton *m_btSelectFile = nullptr;
    QLabel *m_labelEditName = nullptr;
    QLineEdit *m_editName = nullptr;
    QToolButton *m_btGetName = nullptr;
    QLabel *m_labelAppGroup = nullptr;
    QComboBox *m_comboAppGroup = nullptr;
    QCheckBox *m_cbUseGroupPerm = nullptr;
    QCheckBox *m_cbApplyChild = nullptr;
    QCheckBox *m_cbLanOnly = nullptr;
    QCheckBox *m_cbLogBlocked = nullptr;
    QCheckBox *m_cbLogConn = nullptr;
    QRadioButton *m_rbAllowApp = nullptr;
    QRadioButton *m_rbBlockApp = nullptr;
    QRadioButton *m_rbKillProcess = nullptr;
    CheckSpinCombo *m_cscBlockAppIn = nullptr;
    QCheckBox *m_cbBlockAppAt = nullptr;
    QDateTimeEdit *m_dteBlockAppAt = nullptr;
    QCheckBox *m_cbBlockAppNone = nullptr;
    QPushButton *m_btOk = nullptr;
    QPushButton *m_btCancel = nullptr;

    AppRow m_appRow;
    QVector<qint64> m_appIdList;
};

#endif // PROGRAMEDITDIALOG_H
