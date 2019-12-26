#include "controlutil.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QWidgetAction>

QCheckBox *ControlUtil::createCheckBox(bool checked,
                                       const std::function<void (bool checked)> &onToggled)
{
    auto c = new QCheckBox();
    c->setChecked(checked);

    c->connect(c, &QCheckBox::toggled, onToggled);

    return c;
}

QComboBox *ControlUtil::createComboBox(const QStringList &texts,
                                       const std::function<void (int index)> &onActivated)
{
    auto c = new QComboBox();
    c->addItems(texts);

    c->connect(c, QOverload<int>::of(&QComboBox::activated), onActivated);

    return c;
}

QPushButton *ControlUtil::createButton(const QString &iconPath,
                                       const std::function<void ()> &onClicked)
{
    auto c = new QPushButton(QIcon(iconPath), QString());

    c->connect(c, &QPushButton::clicked, onClicked);

    return c;
}

QToolButton *ControlUtil::createToolButton(const QString &iconPath,
                                           const std::function<void ()> &onClicked)
{
    auto c = new QToolButton();
    c->setIcon(QIcon(iconPath));

    c->connect(c, &QToolButton::clicked, onClicked);

    return c;
}

QPushButton *ControlUtil::createLinkButton(const QString &iconPath,
                                           const QString &linkPath,
                                           const QString &toolTip)
{
    auto c = new QPushButton(QIcon(iconPath), QString());
    c->setFlat(true);
    c->setCursor(Qt::PointingHandCursor);
    c->setWindowFilePath(linkPath);
    c->setToolTip(!toolTip.isEmpty() ? toolTip : linkPath);
    return c;
}

QMenu *ControlUtil::createMenuByLayout(QBoxLayout *layout, QWidget *parent)
{
    auto menu = new QMenu(parent);

    auto menuWidget = new QWidget();
    menuWidget->setLayout(layout);

    auto wa = new QWidgetAction(menu);
    wa->setDefaultWidget(menuWidget);
    menu->addAction(wa);

    return menu;
}

QBoxLayout *ControlUtil::createLayoutByWidgets(const QList<QWidget *> &widgets,
                                               Qt::Orientation o)
{
    auto layout = new QBoxLayout(o == Qt::Vertical ? QBoxLayout::TopToBottom
                                                   : QBoxLayout::LeftToRight);
    for (auto w : widgets) {
        if (!w) {
            layout->addStretch();
        } else {
            layout->addWidget(w);
        }
    }

    return layout;
}

QFrame *ControlUtil::createSeparator(Qt::Orientation o)
{
    auto c = new QFrame();
    c->setFrameShape(o == Qt::Horizontal ? QFrame::HLine : QFrame::VLine);
    return c;
}

QFont ControlUtil::createFont(int weight)
{
    QFont font;
    font.setWeight(weight);
    return font;
}

QStringList ControlUtil::getOpenFileNames(const QString &title,
                                          const QString &filter)
{
    return QFileDialog::getOpenFileNames(
                nullptr, title, QString(), filter,
                nullptr, QFileDialog::ReadOnly);
}

QColor ControlUtil::getColor(const QColor &initial)
{
    return QColorDialog::getColor(initial);
}