#include "Login.h"
#include "Message.h"
#include "ciphertext.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>




// 在构造函数中添加
LoginTab::LoginTab(QWidget *parent)
    : QWidget(parent)
     ,net_fd(new TCPConnection(this))
{
    setlayout();
    applyStyles();
    setConnect();

}

void LoginTab::applyStyles() {
    // 设置窗口样式
    setStyleSheet(R"(
        QWidget {
            background-color: #2D2D2D;
            font-family: 'Arial';
        }
        QLabel {
            color: #FFFFFF;
            font-size: 14px;
            min-width: 60px;
        }
        QLineEdit {
            background-color: #3A3A3A;
            color: #FFFFFF;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 5px;
            min-width: 200px;
        }
        QPushButton {
            background-color: #3498DB;
            color: white;
            border-radius: 4px;
            padding: 8px 16px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #2980B9;
        }
    )");

    // 设置窗口标题和大小
    setWindowTitle("用户登录");
    setFixedSize(400,200);
}

void LoginTab::setConnect()
{
    connect(Login_btn,&QPushButton::clicked,this,&LoginTab::UserLoginSection);
    connect(Registry_btn,&QPushButton::clicked,this,&LoginTab::UserRegistreSection);

    connect(net_fd,&TCPConnection::handleRegistre1_OK,this,&LoginTab::handleRegistre1_OK);
    connect(net_fd,&TCPConnection::handleRegistre1_ERROR,this,&LoginTab::handleRegistre1_ERROR);
    connect(net_fd,&TCPConnection::handleRegistre2_OK,this,&LoginTab::handleRegistre2_OK);
    connect(net_fd,&TCPConnection::handleRegistre2_ERROR,this,&LoginTab::handleRegistre2_ERROR);

    connect(net_fd,&TCPConnection::handleLogin1_OK,this,&LoginTab::handleLogin1_OK);
    connect(net_fd,&TCPConnection::handleLogin1_ERROR,this,&LoginTab::handleLogin1_ERROR);
    connect(net_fd,&TCPConnection::handleLogin2_OK,this,&LoginTab::handleLogin2_OK);
    connect(net_fd,&TCPConnection::handleLogin2_ERROR,this,&LoginTab::handleLogin2_ERROR);
}


LoginTab::~LoginTab() {

}

void LoginTab::setlayout() {
    // 1. 创建控件
    Login_btn = new QPushButton("登录", this);
    Registry_btn = new QPushButton("注册", this);
    username_line = new QLineEdit(this);
    password_line = new QLineEdit(this);
    password_line->setEchoMode(QLineEdit::Password); // 密码隐藏

    QLabel *username_label = new QLabel("用户名", this);
    QLabel *password_label = new QLabel("密码：", this);

    // 2. 创建布局（不指定父对象）
    QHBoxLayout *use_layout = new QHBoxLayout();
    use_layout->addWidget(username_label);
    use_layout->addWidget(username_line);

    QHBoxLayout *pass_layout = new QHBoxLayout();
    pass_layout->addWidget(password_label);
    pass_layout->addWidget(password_line);

    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addWidget(Login_btn);
    btn_layout->addWidget(Registry_btn);

    // 3. 创建主垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // 4. 添加子布局和间距
    mainLayout->addLayout(use_layout);
    mainLayout->addSpacing(15); // 添加间距
    mainLayout->addLayout(pass_layout);
    mainLayout->addSpacing(20); // 添加间距
    mainLayout->addLayout(btn_layout);

    // 5. 设置主布局
    this->setLayout(mainLayout);
}


void LoginTab::UserRegistreSection(){
    qDebug() << "UserRegistreSection";
    username = username_line->text();
    password = password_line->text();


    if(username == nullptr || password == nullptr){
        if(username == nullptr && password == nullptr){
            QMessageBox::critical(this,"错误","内容不能为空");
            return;
        }
        if(username == nullptr){
            QMessageBox::critical(this,"错误","用户名不能为空");
            return;
        }
        if(password == nullptr){
            QMessageBox::critical(this,"错误","密码不能为空");
            return;
        }
    }

    int type = TASK_TYPE_REGISTER_SECTION1;
    int len = username.length();
    net_fd->sendPacket(type, len, username);
}

void LoginTab::handleRegistre1_OK(const QByteArray& data)
{
    //根据盐值和密码计算密文
    QString text = password + QString::fromUtf8(data);
    CIPHERString encoded(text);
    QString encrpty = encoded.generateMD5();
    qDebug() << encrpty;

    //将密文发送给服务端
    int type = TASK_TYPE_REGISTER_SECTION2;
    int len = encrpty.length();
    net_fd->sendPacket(type, len, encrpty);
}

void LoginTab::handleRegistre1_ERROR()
{
    QMessageBox::warning(this,"注册","用户名已存在");
}

void LoginTab::handleRegistre2_OK()
{
    QMessageBox::information(this,"注册","注册成功");
}

void LoginTab::handleRegistre2_ERROR()
{
    QMessageBox::warning(this,"注册","请重新尝试");
}


void LoginTab::UserLoginSection(){
    qDebug() << "UserLoginSection";
    username = username_line->text();
    password = password_line->text();
    // qDebug() << "username" << username;
    // qDebug() << "password" << password;

    if(username == nullptr || password == nullptr){
        if(username == nullptr && password == nullptr){
            QMessageBox::critical(this,"错误","内容不能为空");
            return;
        }
        if(username == nullptr){
            QMessageBox::critical(this,"错误","用户名不能为空");
            return;
        }
        if(password == nullptr){
            QMessageBox::critical(this,"错误","密码不能为空");
            return;
        }
    }

    int type = TASK_TYPE_LOGIN_SECTION1;
    int len = username.length();
    net_fd->sendPacket(type, len, username);

}

void LoginTab::handleLogin1_OK(const QByteArray &data)
{
    //接收来自服务端的盐值，通过盐值和密码计算密文，发送给服务端
    QString text = password + QString::fromUtf8(data);
    CIPHERString encoded(text);
    QString encrpty = encoded.generateMD5();
    qDebug() <<  "encrpty"  << encrpty;

    //将密文发送给服务端
    int type = TASK_TYPE_LOGIN_SECTION2;
    int len = encrpty.length();
    net_fd->sendPacket(type, len, encrpty);
}

void LoginTab::handleLogin1_ERROR()
{
    QMessageBox::warning(this,"登录","用户名不存在");
}

void LoginTab::handleLogin2_OK()
{
    emit Login_end();
}

void LoginTab::handleLogin2_ERROR()
{
    QMessageBox::warning(this,"登录","密码错误");
}



