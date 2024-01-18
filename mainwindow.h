#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QMouseEvent>
#include <QSqlDatabase>
#include<QLabel>
#include<QPoint>
#include<QMouseEvent>
#include <QStack>
#include<QVector>
#include<pay.h>

#define M 618
#define INF    (~(0x1<<31))

enum lineid
{
    lineid1,
    lineid2,
    lineid3,
    lineid4,
    lineid5,
    lineid6,
    lineid7,

};

struct StationInfo {
    QString stationName;
    qint8 isChange;
    QPoint coordinates;
};

struct LineInfo {
    qint16 LineNumber;
    qint16 StationNum;
    QRgb rgbr;
    QList<StationInfo> StationList;
};
struct SubwayConnection {
    qint32 weight;    // 权重
    qint32 lineID;    // 路线序号

    qint32 getWeight() const {
        return weight;
    }

    qint32 getLineID() const {
        return lineID;
    }

};

typedef QVector<QVector<SubwayConnection>> SubwayMap;







struct  consumeInfo
{
    QString path;
    qint8 money;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void draw_map();
    void drawLine(int i);
    // algraph map;
    //tcpCommunicate *tcpCom;

    SubwayMap initializeSubwayMap(int size) {
        SubwayMap subwayMap(size, QVector<SubwayConnection>(size, {INF, 0}));
        return subwayMap;
    }


    ~MainWindow();
private:

    void draw_map_demo();
    // void iniGraphicsSystem();
    // void processTableData(const QString& tableName, const QPen& outlineColor);
    void DrawSubwayMap(QVector<QVector<SubwayConnection>>& subwayMap);
    int SearchMap(const QString &s);
    void dijkstra(int vs, int ve, const QVector<QVector<SubwayConnection>>& subwayMap);
    void PrintPath(int v1,int v2,consumeInfo &consumeinfo);
    //int dijkstra(int vs, int ve,  QVector<QVector<SubwayConnection>>& subwayMap,QVector<int> visit,QVector<int> pre);
    QString findStationName(const QList<StationInfo>& stationInfoList, const QPointF& searchCoordinate) {
        for (const StationInfo& station : stationInfoList) {
            if (station.coordinates == searchCoordinate) {
                return station.stationName;
            }
        }
        return QString(); // 如果找不到匹配的坐标，返回空字符串或者其他你认为合适的值
    }

signals:

    void targetStation(QString startStation,QString endStation);
    void showPayWidget();
    void sendPath(QString path);
    void sendCost(qint8 money);


private:

    QList<StationInfo> StationList;
    QList<StationInfo> stationInfoList;
    QList<LineInfo> LineInfoList;
    QStack<int> path;
    SubwayMap subwayMap ;
    int VerNum,top;
    QVector<int> prev;    // 前驱节点数组
    int prev1[M];
    QVector<int> dist; // 顶点到起始点的最短距离
    int dist1[M];
    QVector<int> flag;    // 标记顶点是否已获取最短路径
    consumeInfo ConsumeInfo;
    void initializeVariables() {
        prev = QVector<int>(M);
        dist = QVector<int>(M, INF);
        flag = QVector<int>(M);

    }

public slots:
    void signInInitHandle();

private slots:
    void on_all_line_pb_clicked();

    void on_line2_pb_clicked();

    //void on_line3_pb_clicked(bool checked);

    void do_mouseClicked(QPoint point);
    void on_line1_pb_clicked();

    void on_line3_pb_clicked();

    void on_line4_pb_clicked();

    void on_line5_pb_clicked();

    void on_line6_pb_clicked();

    void on_line7_pb_clicked();

    void on_line8_pb_clicked();

    void on_line9_pb_clicked();

    void on_ticket1_pb_clicked();

    void on_ticket2_pb_clicked();

    void on_ticket3_pb_clicked();

    void on_ticket4_pb_clicked();

    void on_ticket5_pb_clicked();

    void on_ticket6_pb_clicked();

    void on_ticket7_pb_clicked();

    void on_ticket8_pb_clicked();

    void on_ticket9_pb_clicked();

    void on_pushButton_jfb_clicked();

    void on_pushButton_ctm_clicked();

    void on_pushButton_hyd_clicked();

    void on_pushButton_cqk_clicked();

    void on_pushButton_nsyks_clicked();

protected:
    void wheelEvent(QWheelEvent *event);
private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QGraphicsScene *scene;

    QGraphicsScene *sceneMap;
    QGraphicsScene *sceneLine;
    QGraphicsPixmapItem *item;
    bool sceneStatus;


    //void do_mouseClicked(QPoint point);



    void tracePath(int start, int end, const QVector<int>& previous);
    void tracePath1(int start, int end, const int prev[]);
    void PrintSubwayMap(const QVector<QVector<SubwayConnection> > &subwayMap);
};
#endif // MAINWINDOW_H
