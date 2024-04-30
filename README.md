# Entrance Control Cloud

This project is about to implement a cloud-based entrance control system. The system consists of a cloud server and a client. The client is a microcontroller that is connected to a RFID reader and a DC motor. The server is a cloud server that is responsible for authenticating the users and controlling the entrance. The server is also responsible for monitoring the entrance and keeping the entrance history.

- [Requirements](#requirements)
- [Structure and Implementation](#structure-and-implementation)
  - [Client](#client)
    - [Application](#application)
    - [MainWindow](#mainwindow)
    - [HistoryWindow](#historywindow)
    - [WebSocketClient](#websocketclient)
    - [WebSocketRequest](#websocketrequest)
    - [Button](#button)
    - [Label](#label)
    - [TextField](#textfield)
  - [Control Cloud](#control-cloud)
    - [HttpServer](#httpserver)
    - [WebSocketServer](#websocketserver)
    - [EntranceRecord](#entrancerecord)
    - [EntranceHistory](#entrancehistory)
    - [Authenticator](#authenticator)
    - [MonitoringSystemUser](#monitoringsystemuser)
    - [MonitoringSystemAuthenticator](#monitoringsystemauthenticator)
- [Questions](#questions)

- [How to run](#how-to-run)

- [Contributions](#contributions)

## Requirements

For this project, Proteus simulation software, Arduino IDE, and QT Creator are required to be installed on your system. To communicate with the Internet and send data, we need a hardware to send and receive data wirelessly, which is done with the help of ESP8266 wifi module. To control the modules we need the Arduino board.

## Client

In the client section, several essential tasks are performed. Firstly, an observer can send a request to authenticate themselves to the server. Upon successful authentication, he can view the login history of users or real-time information about the logged-in user. Additionally, whenever someone enters through RFID, a message is received via the server, displaying the user's information to the observer. Moreover, the observer can retrieve the login history of users.

## Application

The `CPS::Application` class oversees critical operations such as initializing the main application window, managing WebSocket communications with the server, handling the display of historical data and receiving user's data from the server.
In the constructor of the `Application` class, new instances of `MainWindow`, `HistoryWindow`, and `WebSocketClient` are created, and connections are established between various signals and slots to handle user interactions and WebSocket events within the application. The `show` method displays the main window of the application. The `showHistoryWindow` method presents the received historical data within the history window.

```cpp
namespace CPS {

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    void show();

Q_SIGNALS:

private Q_SLOTS:
    void showHistoryWindow(const QJsonArray &history);

private:

private:
    MainWindow    *_window;
    HistoryWindow *_history;
    WebSocketClient *_webSocket;
};
}
```

## MainWindow

The `CPS::MainWindow` class is responsible for creating and managing the main window of the CPS application. It sets up the UI components such as text fields, labels, and buttons, establishes connections for user interactions, and manages layout and styling for controlling and monitoring the system.
The constructor initializes various UI components such as text fields, labels, and buttons, sets up layout managers for organizing these components, and applies global styling to ensure a consistent appearance. `setTime`, `setDate` and `setUsername` methods update the displayed time, date, and username within the main window of the application. This class has two signals, one of which is for the button to connect to the server and the other is to show the history of users' logins. `changeRightPanelEnabled` method enables or disables all widgets contained within the right panel layout based on the provided boolean value. `showUserDetails` method updates the main window with user details by setting the displayed date, time, and username using the provided data. `setupConnections` is used for button clicks, emitting signals to perform actions such as attempting a server connection or displaying the history. `setupGlobalStyle` configures the global style of the main window by setting its size limits, background color, window icon, title, and palette colors. `setupWindowLayout` organizes the layout of the main window by adding the right panel and left panel layouts to the main layout manager. `setupHeader` method sets up the header label of the main window. `setupRightPanel` and `setupLeftPanel` position and configure the widgets within the left and right panels of the main window, adjusting their layout based on window dimensions and margins. `closeEvent` method overrides the default close event handling, hiding the window instead of closing it, and then exiting the application.

```cpp
namespace CPS {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTime(const QString& time);
    void setDate(const QString& date);
    void setUsername(const QString& username);

Q_SIGNALS:
    void historyuBtnClicked();
    void connectBtnClicked(const QString &address, const QString &username, const QString &password);

public Q_SLOTS:
    void changeRightPanelEnabled(bool enabled);
    void showUserDetails(const QString &username, const QString &date, const QString &time);

private:
    void setupConnections();
    void setupGlobalStyle();
    void setupWindowLayout();
    void setupHeader();
    void setupRightPanel();
    void setupLeftPanel();
    void closeEvent(QCloseEvent *event) override;

private:
    Button      *_connectBtn;
    TextField   *_addressInput;
    TextField   *_usernameInput;
    TextField   *_passwordInput;

    Label       *_date;
    Label       *_time;
    Label       *_username;
    Button      *_historyBtn;

    QVBoxLayout *_leftPanel;
    QHBoxLayout *_mainLayout;
    QVBoxLayout *_rightPanel;

    const int _windowMargin = 30;
};
}
```

## HistoryWindow

The `CPS::HistoryWindow` class creates a window widget to display historical data within the CPS application, formatting and presenting the data in a list format, and clearing the list upon window closure.
In the constructor of the `HistoryWindow` class, a widget is created to display historical data, with specific styling and dimensions set for the window. Additionally, a list widget is created and positioned within the window to present the historical data in a formatted manner. `show` method displays data from the provided JSON array in the history window, formatting and presenting each entry in a list format. `closeEvent` method clears the list of entries in the history window upon closure.

```cpp
namespace CPS {

class HistoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryWindow(QWidget *parent = nullptr);
    ~HistoryWindow();

    void show(const QJsonArray &array);

private:
    void closeEvent(QCloseEvent * event) override;

private:
    QListWidget *_list;
};
}
```

## WebSocketClient

The `CPS::WebSocketClient` class manages WebSocket communication within the CPS application, handling authentication, retrieving historical data, and processing real-time RFID events from the server. It emits signals to notify UI components of new users, changes in connection status, and the availability of historical data, facilitating real-time monitoring and control of the system.
In the constructor, the `WebSocketClient` class establishes connections to handle WebSocket events such as connection establishment, error occurrences, and reception of text messages. Signals in this class convey various events: `newUser` signals a new user event with username, date, and time; `connectionChanged` indicates changes in connection status; `historyReady` signifies availability of historical data in a QJsonArray. `errorOccurred` method handles WebSocket errors by logging the error and emitting a signal to indicate a change in connection status, informing the application of the error condition. After successfully connecting to the server, `connected` method sends an authentication request with the username and password, logging the process. `findRequest` method handles incoming WebSocket data, determining its type, and triggering appropriate actions like notifying about historical data, authentication status, or new RFID events, enabling event handling in the application. `findResponseType` method categorizes the type of response from the server based on the JSON document's structure, helping in handling server responses in the application. `connectToServer` method initiates a connection to the server using the provided address, username, and password, emitting a signal to indicate the connection status. `getHistory` method requests historical data from the server by sending a message through the WebSocket, utilizing a predefined request format to retrieve the history of events or actions.

```cpp
namespace CPS {

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    enum class ResponseType {
        Authenticate,
        History,
        RFID
    };

    explicit WebSocketClient(QObject *parent = nullptr);

Q_SIGNALS:
    void newUser(const QString &username, const QString &date, const QString &time);
    void connectionChanged(bool enabled);
    void historyReady(const QJsonArray &history);

public Q_SLOTS:
    void connectToServer(const QString &address, const QString &username, const QString &password);
    void getHistory();

private Q_SLOTS:
    void connected();
    void errorOccurred(QAbstractSocket::SocketError error);
    void findRequest(const QString &data);

private:
    ResponseType findResponseType(const QJsonDocument &jsonDocument);

private:
    QWebSocket _webSocket;
    WebSocketRequest _request;

    QString _username;
    QString _password;
};

} // end of CPS
```

## WebSocketRequest

The `CPS::WebSocketRequest` class facilitates the creation of WebSocket requests for authentication and historical data retrieval. It generates JSON-formatted requests containing necessary authentication credentials or instructions for history data retrieval.
`authenticationRequest` method constructs a JSON-formatted authentication request by creating a JSON object with a specified type **authenticate** and including the provided username and password in the data object. `historyRequest` constructs a JSON request of type **history** with an empty data object. It's used to request historical data from the server.

```cpp
namespace CPS {

class WebSocketRequest
{
public:
    WebSocketRequest();
    QString authenticationRequest(const QString &username, const QString &password);
    QString historyRequest();
};

}
```

## Button

This class defines a customized QPushButton widget, which is designed for the CPS application. It ensures a consistent appearance and behavior for buttons throughout the application, with a specific text label, font size, and style defined in its constructor.

```cpp
namespace CPS {

class Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Button(const QString &text, QWidget *parent = nullptr);

signals:
};

}
```

## Label

The `Label` class defines a custom label widget in Qt. Its constructor initializes the label's appearance by setting its font size, minimum and maximum height, margin, and text content.

```cpp
namespace CPS {

class Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(const QString &text, QWidget *parent = nullptr);

signals:
};

} // end of CPS
```

## TextField

The `TextField` class defines a custom text field widget in Qt, inheriting from `QLineEdit`. Its constructor initializes the text field's appearance by setting properties such as font size, minimum and maximum height, text margins, placeholder text, and style sheet.

```cpp
namespace CPS {

class TextField : public QLineEdit
{
    Q_OBJECT
public:
    explicit TextField(const QString &placeholderText, QWidget *parent = nullptr);

signals:
};

}
```

## Control Cloud

It is the server-side that contains a list of valid users who can enter the company. Web server and Arduino board communicate with http protocol. The server is also responsible for authenticating the users, monitoring the entrance and keeping the entrance history. The server is implemented in C++ using the QT framework. The server has two main components: HttpServer and WebSocketServer.

## HttpServer

The `HttpServer` is responsible for handling the HTTP requests. Incoming requests are processed through the `handleRequest()` method, which parses incoming JSON payloads, particularly focusing on RFID data. Depending on the request type, the server verifies RFID authentication with the provided `Authenticator` instance and sends a response accordingly. The server also manages the entrance history by emitting signals for new entrance records and saving them to the history file. The `startServer()` method initializes the HTTP server and listens for incoming requests, while the `stopServer()` method stops the server.

```cpp
class HttpServer : public QObject
{
    Q_OBJECT

public:
    HttpServer(Authenticator* authenticator, const QHostAddress &address = QHostAddress::LocalHost, quint16 port = 5050, QObject *parent = nullptr);
    ~HttpServer();
    void startServer();
    void stopServer();
    QHttpServerResponse handleRequest(const QHttpServerRequest &req);

Q_SIGNALS:
    void serverStarted(const QHostAddress &address, quint16 port);
    void serverStopped();
    void requestReceived(const QHttpServerRequest *req, QHttpServerResponse *resp);
    void newEntraceRecord(EntranceRecord *record);

public slots:

private slots:

private:
    QHostAddress m_address;
    quint16 m_port;
    QHttpServer *m_server;
    Authenticator *m_authenticator;
};
```

## WebSocketServer

WebSocketServer is responsible for handling the websocket requests. It listens to the incoming requests and sends the response back. The server is responsible for authenticating users and showing the entrance history.

The `WebSocketServer` class sets up a WebSocket server using `QWebSocketServer` and listens on the specified address and port. The `onNewConnection` method is connected to the `newConnection` signal of the WebSocket server, which is triggered when a new WebSocket connection is established. The `processTextMessage` method handles incoming text messages from clients and performs various actions based on the message content. The class also includes methods for sending history, authentication responses, entrance records, and handling client disconnections. The `socketDisconnected` method handles client disconnections and resets the authentication status.

```cpp
class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    WebSocketServer(const QHostAddress &address, quint16 port = 5051, QObject *parent = nullptr);

Q_SIGNALS:
    void historyRequested();
    void authenticateRequested(const QString &username, const QString &password);

public slots:
    void sendHistory(const QJsonDocument &jsonDoc);
    void sendAuthenticate();
    void sendUnauthenticate();
    void sendEntranceRecord(EntranceRecord *record);

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketDisconnected();

private:
    QHostAddress m_address;
    quint16 m_port;
    QWebSocketServer *m_server;
    QWebSocket *m_client_socket;
    bool m_isAuthenticated = false;
};
```

## EntranceRecord

`EntranceRecord` is a class that contains the RFID tag, entrance time, and permission status of the user. The class provides methods to permit or deny access to the user and emits a signal when the permission status changes.

```cpp
class EntranceRecord : public QObject
{
    Q_OBJECT

public:
    EntranceRecord(QString rfidTag, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, QDateTime entranceTime, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, bool isPermitted, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, QString entranceTime, QString isPermitted, QObject *parent = nullptr); // (1
    ~EntranceRecord();
    QString rfidTag() const;
    QDateTime entranceTime() const;
    bool isPermitted() const;
    void permit();
    void deny();

Q_SIGNALS:
    void permissionChanged(bool isPermitted);

private:
    QString m_rfidTag;
    QDateTime m_entranceTime;
    bool m_isPermitted;
}
```

## EntranceHistory

`EntranceHistory` is a class that manages the history of entrance records. It loads the entrance records from a file, saves the records to the file, and provides methods to retrieve and add entrance records. The class also converts the entrance records to JSON format and emits a signal when the History is requested.

```cpp
class EntranceHistory : public QObject
{
    Q_OBJECT

public:
    EntranceHistory(QString entranceHistoryPath, QObject *parent = nullptr);
    ~EntranceHistory();
    QVector<EntranceRecord *> getEntranceRecords(int maxRecords = 10);
    QJsonDocument convertToJson(const QVector<EntranceRecord *> &records);

Q_SIGNALS:
    void getEntranceRecordsRequested(const QJsonDocument &jsonDo);

public slots:
    void saveEntranceRecords();
    void getEntranceRecordsHistory();
    void addEntranceRecord(EntranceRecord *record);

private:
    int m_lastRecordIndex = 0;
    QString m_entranceHistoryPath;
    QVector<EntranceRecord *> m_entranceRecords;
    void loadEntranceRecords();
};
```

## Authenticator

`Authenticator` is a class that manages the permitted RFID tags. It loads the permitted tags from a file and provides a method to check if a tag is in permitted tags or not..

```cpp
class Authenticator : public QObject
{
    Q_OBJECT

public:
    Authenticator(QString permittedTagsPath, QObject *parent = nullptr);
    ~Authenticator();
    bool isPermitted(const QString &tag);

private:
    QString m_permittedTagsPath;
    QVector<QString> m_permittedTags;
    void loadPermittedTags();
    void savePermittedTags();
};
```

## MonitoringSystemUser

`MonitoringSystemUser` is a class that represents a user of the monitoring system. It contains the username and password of the user and provides methods to get the username and validate the password.

```cpp
class MonitoringSystemUser : public QObject
{
    Q_OBJECT

public:
    MonitoringSystemUser(QString username, QString password, QObject *parent = nullptr);
    ~MonitoringSystemUser();
    QString getUsername() const;
    bool validatePassword(const QString &password) const;

Q_SIGNALS:

private:
    QString m_username;
    QString m_password;
};
```

## MonitoringSystemAuthenticator

`MonitoringSystemAuthenticator` is a class that is responsible for authenticating the users of the monitoring system. It contains the list of users and the path to the file that contains the users. By starting the server, the users are loaded from the file.

```cpp
class MonitoringSystemAuthenticator : public QObject
{
    Q_OBJECT
public:
    MonitoringSystemAuthenticator(const QString &systemUserPath, QObject *parent = nullptr);
    ~MonitoringSystemAuthenticator();

Q_SIGNALS:
    void authenticated();
    void unauthenticated();

public Q_SLOTS:
    void authenticate(const QString &username, const QString &password);

private:
    void loadMonitoringSystemUsers();

    QString m_systemUserPath;
    QVector<MonitoringSystemUser *> m_monitoringSystemUsers;
};
```

## How to Run
