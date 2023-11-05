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

    //�ؼ���ʼ��
    m_iconLabel = new QLabel(this);
    m_titleLabel = new QLabel(this);
    m_minimizeButton = new QPushButton(this);
    m_maximizeButton = new QPushButton(this);
    m_closeButton = new QPushButton(this);

    //ͼƬ����Ӧ�ؼ���С
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setScaledContents(true);

    //���ÿؼ��ڲ��֣�layout������Ĵ�С�仯������
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //���ÿؼ�����С��С������С
    m_minimizeButton->setFixedSize(30, 25);
    m_maximizeButton->setFixedSize(30, 25);
    m_closeButton->setFixedSize(30, 25);

    //���ÿؼ�Ψһ��ʶ��
    m_titleLabel->setObjectName("whiteLabel");
    m_minimizeButton->setObjectName("minimizeButton");
    m_maximizeButton->setObjectName("maximizeButton");
    m_closeButton->setObjectName("closeButton");

    //����ͼ��
    int fontId = QFontDatabase::addApplicationFont(":/font/iconfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(17);

    m_minimizeButton->setFont(iconFont);
    m_minimizeButton->setText(QChar(0xe656));
    m_minimizeButton->setToolTip(tr(u8"��С��"));
    m_minimizeButton->installEventFilter(this);

    m_maximizeButton->setFont(iconFont);
    m_maximizeButton->setText(QChar(0xe600));
    m_maximizeButton->setToolTip(tr(u8"���»�ԭ"));
    m_maximizeButton->installEventFilter(this);

    m_closeButton->setFont(iconFont);
    m_closeButton->setText(QChar(0xe646));
    m_closeButton->setToolTip(tr(u8"�ر�"));
    m_closeButton->installEventFilter(this);

    //�ؼ�����
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


    //�źŲ۰�
    connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

myTitleBar::~myTitleBar()
{

}

/**
 * @brief MainWindow::mousePressEvent ���˫����������󻯻�ԭ
 * @param event ����¼�
 */
void myTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    emit m_maximizeButton->clicked();
}

/**
 * @brief MainWindow::mousePressEvent ��갴�£�׼���ƶ�
 * @param event ����¼�
 */
void myTitleBar::mousePressEvent(QMouseEvent* event)
{
    m_mousePosition = event->pos();

    m_isMousePressed = true;
}

/**
 * @brief MainWindow::mousePressEvent ����ƶ�
 * @param event ����¼�
 */
void myTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePressed == true)
    {
        QWidget* pWindow = this->window();

        if (pWindow->isMaximized()) //�������ʱ���Ȼ�ԭ���ƶ�
        {
            pWindow->showNormal();

            //��ֹ���ָ���ڽ���֮��
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
 * @brief MainWindow::mousePressEvent ���̧���ƶ�����
 * @param event ����¼�
 */
void myTitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    m_isMousePressed = false;
}

/**
 * @brief TitleBar::eventFilter �¼�������
 * @param obj ���˶���
 * @param event �¼�
 * @return
 */
bool myTitleBar::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:  //���ı�����ʾ����
    {
        QWidget* pWidget = qobject_cast<QWidget*>(obj);
        if (pWidget)
        {
            m_titleLabel->setText(pWidget->windowTitle());
        }
    }
    break;
    case QEvent::WindowIconChange:  //����ͼ��
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
        updateMaximize();  //���/��ԭ
        break;
    case QEvent::Enter:  //�����ͣ�ڿؼ��ϣ��ؼ���ɫ
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
    case QEvent::Leave:  //����뿪�ؼ�����ɫ�ָ�
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
 * @brief TitleBar::onClicked ������С�������/��ԭ���رղ���
 */
void myTitleBar::onClicked()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_minimizeButton)  //��С��
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)  //��󻯡���ԭ
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)  //�ر�
        {
            pWindow->close();
        }
    }
}

/**
 * @brief TitleBar::updateMaximize ���/��ԭʱ������ͼ����ʽ
 */
void myTitleBar::updateMaximize()
{
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr(u8"���»�ԭ"));
            m_maximizeButton->setProperty("maximizeProperty",u8"���»�ԭ");
            m_maximizeButton->setText(QChar(0xe600));
        }
        else
        {
            m_maximizeButton->setProperty("maximizeProperty", u8"���");
            m_maximizeButton->setToolTip(tr(u8"���"));
            m_maximizeButton->setText(QChar(0xe600));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

/**
 * @brief TitleBar::setColor ������ɫ
 * @param fontColor �ؼ���ɫ/������ɫ
 * @param selectedFontColor �����ͣ�ڿؼ���ʱ����ɫ
 */
void myTitleBar::setColor(QString fontColor, QString selectedFontColor)
{

    m_fontColor = fontColor;
    m_selectedFontColor = selectedFontColor;

    //��������͸����ͼ����ɫΪm_fontColor
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
