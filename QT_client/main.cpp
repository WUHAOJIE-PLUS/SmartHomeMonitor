#include <QApplication>
#include <memory>
#include <QFile>
#include "cmainwindow.h"
#include "Login.h"

using std::unique_ptr;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建登录窗口（原始指针）
    LoginTab* loginWindow = new LoginTab;
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    // 登录成功处理
    QObject::connect(loginWindow, &LoginTab::Login_end, [loginWindow]() {
        CMainWindow* mainWindow = new CMainWindow;
        mainWindow->show();
        loginWindow->close();
    });


    return a.exec();
}

