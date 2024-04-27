#include "cpsapplication.h"

namespace CPS {

Application::Application(QObject *parent)
    : QObject{parent},
    _window(new MainWindow),
    _history(new HistoryWindow),
    _webSocket(new WebSocketClient)
{
    setWindowsThemeToDark<MainWindow>(*_window);

    QObject::connect(_window, &MainWindow::historyuBtnClicked, _webSocket, &WebSocketClient::getHistory);
    QObject::connect(_webSocket, &WebSocketClient::historyReady, this, &Application::showHistoryWindow);
    //QObject::connect(_webSocket, &WebSocketClient::newUser, _window, &MainWindow::showUserDetails);
    QObject::connect(_window, &MainWindow::connectBtnClicked, _webSocket, &WebSocketClient::connectToServer);
    //QObject::connect(_webSocket, &WebSocketClient::connectionChanged, _window, &MainWindow::changeRightPanelEnabled);
}

Application::~Application()
{
    delete this->_window;
    delete this->_history;
    delete this->_webSocket;
}

void Application::show()
{
    this->_window->show();
}

void Application::showHistoryWindow(const QJsonArray &history)
{
    setWindowsThemeToDark<HistoryWindow>(*_history);
    _history->show(history);
}

} // end of CPS
