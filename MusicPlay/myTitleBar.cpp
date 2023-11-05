#include "myTitleBar.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QApplication>
#include <QPainter>
#include <QStyleOption>


myTitleBar::myTitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(50);

    //控件初始化
    m_iconLabel = new QLabel(this);
    m_titleLabel = new QLabel(this);
    m_minimizeButton = new QPushButton(this);
    m_maximizeButton = new QPushButton(this);
    m_closeButton = new QPushButton(this);

    //图片自适应控件大小
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setScaledContents(true);

    //设置控件在布局（layout）里面的大小变化的属性
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //设置控件的最小大小和最大大小
    m_minimizeButton->setFixedSize(30, 25);
    m_maximizeButton->setFixedSize(30, 25);
    m_closeButton->setFixedSize(30, 25);

    //设置控件唯一标识符
    m_titleLabel->setObjectName("whiteLabel");
    m_minimizeButton->setObjectName("minimizeButton");
    m_maximizeButton->setObjectName("maximizeButton");
    m_closeButton->setObjectName("closeButton");

    //设置图标
    int fontId = QFontDatabase::addApplicationFont(":/font/iconfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(17);

    m_minimizeButton->setFont(iconFont);
    m_minimizeButton->setText(QChar(0xe656));
    m_minimizeButton->setToolTip(tr(u8"最小化"));
    m_minimizeButton->installEventFilter(this);

    m_maximizeButton->setFont(iconFont);
    m_maximizeButton->setText(QChar(0xe600));
    m_maximizeButton->setToolTip(tr(u8"向下还原"));
    m_maximizeButton->installEventFilter(this);

    m_closeButton->setFont(iconFont);
    m_closeButton->setText(QChar(0xe646));
    m_closeButton->setToolTip(tr(u8"关闭"));
    m_closeButton->installEventFilter(this);

    //控件布局
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_iconLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_titleLabel);
    pLayout->addWidget(m_minimizeButton);
    pLayout->addWidget(m_maximizeButton);
    pLayout->addWidget(m_closeButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
    setLayout(pLayout);


    //信号槽绑定
    connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

myTitleBar::~myTitleBar()
{

}

/**
 * @brief MainWindow::mousePressEvent 鼠标双击：界面最大化或还原
 * @param event 鼠标事件
 */
void myTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    emit m_maximizeButton->clicked();
}

/**
 * @brief MainWindow::mousePressEvent 鼠标按下：准备移动
 * @param event 鼠标事件
 */
void myTitleBar::mousePressEvent(QMouseEvent* event)
{
    m_mousePosition = event->pos();

    m_isMousePressed = true;
}

/**
 * @brief MainWindow::mousePressEvent 鼠标移动
 * @param event 鼠标事件
 */
void myTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePressed == true)
    {
        QWidget* pWindow = this->window();

        if (pWindow->isMaximized()) //界面最大时，先还原再移动
        {
            pWindow->showNormal();

            //防止鼠标指针在界面之外
            m_mousePosition = QPoint(200, 10);
            pWindow->move(event->globalPos().x() - 200, event->globalPos().y());
        }
        else
        {
            QPoint movePot = event->globalPos() - m_mousePosition;
            pWindow->move(movePot);
        }
    }
}

/**
 * @brief MainWindow::mousePressEvent 鼠标抬起：移动结束
 * @param event 鼠标事件
 */
void myTitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    m_isMousePressed = false;
}

/**
 * @brief TitleBar::eventFilter 事件过滤器
 * @param obj 过滤对象
 * @param event 事件
 * @return
 */
bool myTitleBar::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:  //更改标题显示内容
    {
        QWidget* pWidget = qobject_cast<QWidget*>(obj);
        if (pWidget)
        {
            m_titleLabel->setText(pWidget->windowTitle());
        }
    }
    break;
    case QEvent::WindowIconChange:  //更改图标
    {
        QWidget* pWidget = qobject_cast<QWidget*>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
        }
    }
    break;
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();  //最大化/还原
        break;
    case QEvent::Enter:  //鼠标悬停在控件上，控件变色
    {
        if (obj == m_minimizeButton)
        {
            m_minimizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_selectedFontColor));
        }
        else if (obj == m_closeButton)
        {
            m_closeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_selectedFontColor));
        }
        else if (obj == m_maximizeButton)
        {
            m_maximizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_selectedFontColor));
        }
    }
    break;
    case QEvent::Leave:  //鼠标离开控件，颜色恢复
    {
        if (obj == m_minimizeButton)
        {
            m_minimizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
        }
        else if (obj == m_closeButton)
        {
            m_closeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
        }
        else if (obj == m_maximizeButton)
        {
            m_maximizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
        }

    }
    break;
    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}

/**
 * @brief TitleBar::onClicked 进行最小化、最大化/还原、关闭操作
 */
void myTitleBar::onClicked()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_minimizeButton)  //最小化
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)  //最大化、还原
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)  //关闭
        {
            pWindow->close();
        }
    }
}

/**
 * @brief TitleBar::updateMaximize 最大化/还原时，更改图标样式
 */
void myTitleBar::updateMaximize()
{
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr(u8"向下还原"));
            m_maximizeButton->setProperty("maximizeProperty",u8"向下还原");
            m_maximizeButton->setText(QChar(0xe600));
        }
        else
        {
            m_maximizeButton->setProperty("maximizeProperty", u8"最大化");
            m_maximizeButton->setToolTip(tr(u8"最大化"));
            m_maximizeButton->setText(QChar(0xe600));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

/**
 * @brief TitleBar::setColor 更改颜色
 * @param fontColor 控件颜色/字体颜色
 * @param selectedFontColor 鼠标悬停在控件上时的颜色
 */
void myTitleBar::setColor(QString fontColor, QString selectedFontColor)
{

    m_fontColor = fontColor;
    m_selectedFontColor = selectedFontColor;

    //按键背景透明，图标颜色为m_fontColor
    m_minimizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
    m_maximizeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
    m_closeButton->setStyleSheet(QString("QPushButton{background-color:rgba(0,0,0,0);color:%1}").arg(m_fontColor));


    m_titleLabel->setStyleSheet(QString("QLabel{background:rgba(0,0,0,0);color:%1}").arg(m_fontColor));
    m_iconLabel->setStyleSheet(QString("QLabel{background:rgba(0,0,0,0);color:%1}").arg(m_fontColor));

}

void myTitleBar::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter* painter = new QPainter(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,painter,this);

}
