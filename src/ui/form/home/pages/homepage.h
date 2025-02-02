#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "homebasepage.h"

class HomePage : public HomeBasePage
{
    Q_OBJECT

public:
    explicit HomePage(HomeController *ctrl = nullptr, QWidget *parent = nullptr);

protected slots:
    void onRetranslateUi() override;

private:
    void retranslateDriverMessage();

    void setupUi();
    void setupDriverBox();
    void setupDriverIcon();

private:
    QGroupBox *m_gbDriver = nullptr;
    QLabel *m_iconDriver = nullptr;
    QLabel *m_labelDriverMessage = nullptr;
    QPushButton *m_btInstallDriver = nullptr;
    QPushButton *m_btRemoveDriver = nullptr;
};

#endif // HOMEPAGE_H
