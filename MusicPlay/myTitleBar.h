#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>
class QLabel;
class QPushButton;

class myTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit myTitleBar(QWidget* parent = nullptr);
    ~myTitleBar();

    void setColor(QString fontColor, QString selectedFontColor);  //������ɫ
protected:

    //�����϶�
    void mousePressEvent(QMouseEvent* event);  //��갴��

    void mouseMoveEvent(QMouseEvent* event);  //����ƶ�

    void mouseReleaseEvent(QMouseEvent* event);  //���̧��

    void mouseDoubleClickEvent(QMouseEvent* event);  //˫�����������н�������/��ԭ

    bool eventFilter(QObject* obj, QEvent* event);  //�¼�������

private slots:

    void onClicked();  //������С�������/��ԭ���رղ���

    void updateMaximize();  //���/��ԭ

private:
    QLabel* m_iconLabel;  //��ʾͼ��
    QLabel* m_titleLabel;  //��ʾ����
    QPushButton* m_minimizeButton;  //��С������
    QPushButton* m_maximizeButton;   //���/��ԭ����
    QPushButton* m_closeButton;   //�رհ���

    QPoint m_mousePosition;  //��갴��ʱ��λ��
    bool m_isMousePressed;  //����Ƿ�����

    QString m_fontColor; //����ɫ
    QString m_selectedFontColor; //��ѡ�е�����ɫ


protected:
    void paintEvent(QPaintEvent*);

};

#endif 


