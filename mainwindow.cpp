#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneEvent>
#include <QPixmap>
#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include    <QPainter>
#include  <QtMath>
#include   <QPainterPath>
#include<QPaintEvent>
#include<QMouseEvent>
#include<QMessageBox>
#include <QTextStream>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

void MainWindow::signInInitHandle()
{
    show();
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    //this->setMouseTracking(false);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    // 将场景添加到用户界面的 QGraphicsView 中
    // 在这里连接登录窗口的信号与主界面显示的槽

    connect(ui->graphicsView,&TGraphicsView::mouseClicked,this, &MainWindow::do_mouseClicked);



    QPixmap pixmap(":/res/png/Chongqing_Rail_Transit_logo.png");
    pixmap = pixmap.scaled(ui->label_logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_logo->setPixmap(pixmap);
    ui->label_logo->setAlignment(Qt::AlignCenter);



   // Q_ASSERT(m_sqlDataProcess != nullptr);
    showMaximized();
    draw_map();
    //draw_map_demo();
    sceneStatus = true;

    DrawSubwayMap(subwayMap);
    initializeVariables();

}




MainWindow::~MainWindow() {



    delete ui;
}

void MainWindow::draw_map_demo()
{

   //      QPen pen;
   // // int num = 0;
   //      pen.setWidth(5);
   //      for (int i = 0; i <m_sqlDataProcess->lineInfoList.size(); i++) {
   //           QPoint prePoint(0,0);
   //          pen.setColor(QColor(m_sqlDataProcess->lineInfoList.at(i).rgbR));
   //               QList<qint16> stationKeys = m_sqlDataProcess->lineInfoList.at(i).lineInfoMap.uniqueKeys();

   //            for (int j = 0; j < stationKeys.size(); j++){
   //                   qint16 currentKey = stationKeys.at(j);
   //                   QList<stationInfo> stations = m_sqlDataProcess->lineInfoList.at(i).lineInfoMap.values(currentKey);
   //                   QGraphicsItem* item;

   //                   for (int k = 0; k < stations.size(); k++) {
   //                   stationInfo tmpstationInfo = stations.at(k);
   //              if ( tmpstationInfo.isChange == 1) {

   //                  QRect rect(-5,-5,10,10);
   //                  QGraphicsRectItem* RectItem = new QGraphicsRectItem;
   //                  RectItem->setBrush(QBrush(Qt::white));
   //                  RectItem->setRect(rect);
   //                  RectItem->setPos(tmpstationInfo.coordinates);
   //                  item = RectItem;
   //                  item->setToolTip("这是"+QString(tmpstationInfo.stationName)+"\n"+QString::number(m_sqlDataProcess->lineInfoList.at(i).lineNumber)+"号线");

   //              } else {
   //                  // 如果不可以换乘，添加圆形
   //                  QGraphicsEllipseItem* ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10);
   //                  ellipseItem->setPos(tmpstationInfo.coordinates);
   //                  ellipseItem->setPen(Qt::NoPen);
   //                  ellipseItem->setBrush(QBrush(Qt::red));
   //                  //ellipseItem->setFlag(QGraphicsItem::ItemIsSelectable);
   //                  item = ellipseItem;
   //                  item->setToolTip("这是"+QString(tmpstationInfo.stationName)+"\n"+QString::number(m_sqlDataProcess->lineInfoList.at(i).lineNumber)+"号线");
   //              }

   //              item->setZValue(1);
   //              item->setFlag(QGraphicsItem::ItemIsFocusable);
   //              scene->addItem(item);
   //              if (prePoint.x() != 0 || prePoint.y() != 0) {
   //                  QPainterPath path;
   //                  path.moveTo(prePoint);
   //                  path.lineTo(tmpstationInfo.coordinates);
   //                  QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
   //                  pathItem->setPen(pen);
   //                  scene->addItem(pathItem);
   //              }
   //               QGraphicsTextItem* text = scene->addText(tmpstationInfo.stationName, QFont("宋体", 5));
   //      // 在这里设置每个文本项的位置等属性
   //                text->setZValue(3);
   //      int textX = tmpstationInfo.coordinates.x() - 15;
   //      int textY = tmpstationInfo.coordinates.y() + 5;
   //      text->setPos(textX, textY);


   //           prePoint = tmpstationInfo.coordinates;
   //                   }
   //     // num++;
   //          }


   //          }

   //      scene->clearSelection();


}

void MainWindow::DrawSubwayMap(QVector<QVector<SubwayConnection>>& subwayMap) {
    VerNum = 0;
    qint16 ContainID, EdgeLast;
    qint16 num = 0;
    subwayMap = initializeSubwayMap(M);
    for (qint16 i = 0; i < LineInfoList.size(); i++) {
        EdgeLast = -1;
        //qDebug() <<QString::asprintf("这是第%d轮",i);
        for (qint16 j = 0; j < LineInfoList.at(i).StationNum; j++) {
            ContainID = SearchMap(stationInfoList.at(j + num).stationName);

            if (ContainID == -1) {
                StationList.append(stationInfoList.at(j + num));

                if (EdgeLast != -1) {
                    subwayMap[EdgeLast][VerNum].lineID = subwayMap[VerNum][EdgeLast].lineID = LineInfoList.at(i).LineNumber;
                    subwayMap[EdgeLast][VerNum].weight = subwayMap[VerNum][EdgeLast].weight = 1;
                }
                EdgeLast = VerNum;
                VerNum++;
            }
            else {
                if (EdgeLast != -1 && subwayMap[EdgeLast][ContainID].lineID == 0) {
                    subwayMap[EdgeLast][ContainID].lineID = subwayMap[ContainID][EdgeLast].lineID = LineInfoList.at(i).LineNumber;
                    subwayMap[EdgeLast][ContainID].weight = subwayMap[ContainID][EdgeLast].weight = 1;
                }
                EdgeLast = ContainID;
            }
        }
        num += LineInfoList.at(i).StationNum;
    }

   // qDebug() << "不重复的站台数" << StationList.size();
//    qDebug() << "矩阵大小" <<subwayMap.size();
   // qDebug() << "Vernum" <<VerNum;
}




int MainWindow::SearchMap(const QString &s)
{
       // qDebug() << "##这是搜索结果" << s;
        for (int i = 0; i < StationList.size(); ++i) {
             if (StationList.at(i).stationName == s) {
               // qDebug() << "有一样的站" << s;
                return i; // 找到相同站名，返回其索引值 i
             }
        }
        return -1; // 遍历完列表仍未找到匹配站名，返回 -1
}


void MainWindow::dijkstra(int vs, int ve, const QVector<QVector<SubwayConnection>>& subwayMap) {
        int n = subwayMap.size(); // 顶点数量


        int i,j,k;
        int min;
        int flag1[n];
        for (i = 0; i < n; ++i){	// 初始化

             flag1[i] = 0;              	// 顶点i的最短路径还没获取到。
             prev1[i] = vs;             	 // 顶点i的前驱顶点为0。
             dist1[i] = subwayMap[vs][i].weight;	// 顶点i的最短路径为"顶点vs"到"顶点i"的权。
            // qDebug() << QString::asprintf("顶点vs到顶点%d的权",i)<<dist[i];
        }
        // dist[vs] = 0;
        // flag[vs] = 1;

        dist1[vs] = 0;
        flag1[vs] = 1;

        for ( i = 0; i < n-1 ; ++i) {
              min = INF;


             // 找到未标记顶点中距离起始点最近的顶点
             for ( j = 0; j < n; ++j) {

                if (flag1[j] == 0 && dist1[j] < min) {

                        min = dist1[j];
                        k = j;
                }
             }

             flag1[k] = 1;

              if ( k == ve) // 找不到可达顶点或者已经找到终点
                 return;



             // 更新与顶点k相连的顶点的最短路径和前驱节点
             for ( j = 0; j < n; ++j){    // 修正当前最短路径和前驱顶点:  即当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。

                  int tmp;
                // int tmp = (subwayMap[k][j].weight == INF ? INF :subwayMap[k][j].weight);

                 if (subwayMap[k][j].weight == INT_MAX) {
                                 tmp = INT_MAX;
                             }
                             else
                             {
                                 tmp = min + subwayMap[k][j].weight;
                             }
                if (flag1[j] == 0 && (tmp  < dist1[j]) && subwayMap[k][j].lineID>0){
                dist1[j] = tmp;
                prev1[j] = k;	//记录前驱
               // qDebug()<<"前驱值"<< prev1[j];
                //qDebug()<<QString("前驱站点名称：%1").arg(StationList.at(j).stationName);
                }
             }
        }

}

void MainWindow::tracePath(int start, int end, const QVector<int> &previous) {
        for (int t = end; t != start; t = previous[t]) {
             path.push(t);
             top++;

        }
        //qDebug() << "top值" << top;
}

void MainWindow::tracePath1(int start, int end, const int prev[])
{
    for (int t = end; t != start; t = prev[t]) {
        path.push(t);
        top++;

    }
   // qDebug() << "top值" << top;
}






void  MainWindow::PrintPath(int v1, int v2,consumeInfo &consumeinfo) {
   // QString pathload;
    QTextStream pathstream(&consumeinfo.path);

    //QTextStream moneystream(&consumeinfo.money);
    int i, EdgeLast = v1, pathLen = 1, u = path[top -1], prices = 0;
    //qDebug() <<"路径量"<<path.size();
    int lineId = subwayMap[EdgeLast][u].lineID;

    //qDebug() << StationList.at(v1).stationName << QString::asprintf("(%d号线)", lineId);
      pathstream <<QString("从 %1 到 %2 的最短路径：\n").arg(StationList.at(v1).stationName,StationList.at(v2).stationName);
    pathstream <<QString("%1 (%2号线)").arg(StationList.at(v1).stationName).arg(lineId) ;

    EdgeLast = u;
    for (i = top - 2; i >= 0; i--) {
        u = path[i];
        if (lineId != subwayMap[EdgeLast][u].lineID) {
            lineId = subwayMap[EdgeLast][u].lineID;

            //qDebug() << QString::asprintf("(价格：%d)->", pathLen)<<StationList.at(EdgeLast).stationName<< QString::asprintf("(%d号线)", lineId);

            pathstream << QString("-> %1 换乘( %2 号线)").arg(StationList.at(EdgeLast).stationName).arg(lineId);
            prices = prices + pathLen;
            pathLen = 0;
        }
        pathLen++;
        EdgeLast = u;
    }
    prices = prices + pathLen;

  //  qDebug() << QString::asprintf("(价格：%d)->", pathLen)<<StationList.at(v2).stationName;
   // qDebug() << StationList.at(v2).stationName;
 //   qDebug() << QString::asprintf("(价格：%d", prices);

    pathstream << QString("-> %1").arg(StationList.at(v2).stationName);
    //pathstream<<NULL;
    //moneystream << QString("(价格：%1\n").arg(prices);
    consumeinfo.money=prices;
    top=0;
    path.clear();
    //path.fill(0);
    //return pathload;
}







void MainWindow::draw_map() {

//    if (QSqlDatabase::contains("qt_sql_default_connection")) {
//        QSqlDatabase::removeDatabase("qt_sql_default_connection");
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    } else {
//        db = QSqlDatabase::addDatabase("QSQLITE");
//    }

    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("station.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库";
        return;
    }

    stationInfoList.clear();
    LineInfoList.clear();
    QSqlQuery queryinfo;
    queryinfo.exec("SELECT * FROM lineinfo");

    while (queryinfo.next()) {

        LineInfo lineinfo;

        lineinfo.LineNumber = queryinfo.value("路线号").toInt();

        //int snumber = queryinfo.value("路线号").toInt();
        QString routineColor = queryinfo.value("颜色").toString();
        lineinfo.StationNum = queryinfo.value("站点数量").toInt();

        //qDebug() << "颜色字符串：" << routineColor;

        lineinfo.rgbr = QColor(routineColor).rgb(); // 将字符串转换为有效的QRgb颜色值
        LineInfoList.append(lineinfo);
        //rgbr.append(color); // 将颜色添加到 rgbr 列表中
    }
    queryinfo.clear();

    QPen pen;
    pen.setWidth(5);


    for (int s = 0; s < LineInfoList.size(); s++) {
        QSqlQuery query;
        query.exec("SELECT * FROM lineid" +QString::number(LineInfoList.at(s).LineNumber) );
        pen.setColor(QColor(LineInfoList.at(s).rgbr));

        int prevX = 0;
        int prevY = 0;

        while (query.next()) {

            StationInfo stationInfo;

            int x = query.value("站点坐标x").toInt();
            int y = query.value("站点坐标y").toInt();
//            QString stationName = query.value("站点名称").toString();
//            int isChange1 = query.value("是否换乘").toInt();
           stationInfo.stationName = query.value("站点名称").toString();
           stationInfo.isChange = query.value("是否换乘").toInt();
           stationInfo.coordinates = QPoint(x, y);
            //coordinates.append(QPoint(x, y));
            //stationNames.append(stationName);
            //IsChange.append(isChange1);
            stationInfoList.append(stationInfo);
            //LineInfoList.at(s).StationList.append(stationInfo);

            QGraphicsItem* item;

            if ( stationInfo.isChange == 1) {
                // 如果可以换乘，添加自定义图片
//                QPixmap pixmap(":/new/prefix1/1.png");  // 替换为你的图片路径
//                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
//                //pixmap.setMask(pixmap.createMaskFromColor(Qt::white));
//                pixmapItem->setScale(0.02);
//                pixmapItem->setZValue(2);
//               // pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);

//                pixmapItem->setPos(x -pixmap.width()*0.01, y-pixmap.height()*0.01 );
//                item = pixmapItem;
                //scene->addItem(pixmapItem);
                QRect rect(-5,-5,10,10);
                QGraphicsRectItem* RectItem = new QGraphicsRectItem;
                RectItem->setBrush(QBrush(Qt::white));
                RectItem->setRect(rect);
                RectItem->setPos(x,y);
                item = RectItem;
                item->setToolTip("这是"+QString(stationInfo.stationName)+"\n"+QString::number(LineInfoList.at(s).LineNumber)+"号线");




            } else {
                // 如果不可以换乘，添加圆形
                QGraphicsEllipseItem* ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10);
                ellipseItem->setPos(x, y);
                ellipseItem->setPen(Qt::NoPen);
                ellipseItem->setBrush(QBrush(Qt::red));
                //ellipseItem->setFlag(QGraphicsItem::ItemIsSelectable);
                item = ellipseItem;
                 item->setToolTip("这是"+QString(stationInfo.stationName)+"\n"+QString::number(LineInfoList.at(s).LineNumber)+"号线");
            }

            item->setZValue(3);
            item->setFlag(QGraphicsItem::ItemIsFocusable);
            scene->addItem(item);

            if (prevX != 0 || prevY != 0) {
                QPainterPath path;
                path.moveTo(prevX, prevY);
                path.lineTo(x, y);
                QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
                pathItem->setPen(pen);
                scene->addItem(pathItem);
            }


    QGraphicsTextItem* text = scene->addText(stationInfo.stationName, QFont("宋体", 5));
    // 在这里设置每个文本项的位置等属性
    text->setZValue(2);
    int textX = x - 15;
    int textY = y + 5;
    text->setPos(textX, textY);


//            text->setZValue(3);
//            int textX = x - 10;
//            int textY = y + 2;
//            text->setPos(textX, textY);

            prevX = x;
            prevY = y;
        }

        //qDebug()<<StationNum;
        //StationNum +=stationNames.size();
        query.clear();
    }
    // 获取 stationName 的总数
    //int totalStationNames = stationInfoList.size();
   // qDebug() << "总站点数：" << totalStationNames;



    scene->clearSelection();
    db.close();
    QSqlDatabase::removeDatabase("QSQLITE");
    //LineNumber.clear();
    //rgbr.clear();
   // coordinates.clear();
}




void MainWindow::drawLine(int k) {

    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QColor lineColor = QColor(140, 194, 34);  // 设置2号线的颜色
    QPen outlinePen;
    outlinePen.setColor(lineColor);
    outlinePen.setWidth(60);
    QPen outlinePen1;
    outlinePen1.setColor(Qt::red);
    outlinePen1.setWidth(20);
    QGraphicsTextItem *text;
//    if (QSqlDatabase::contains("qt_sql_default_connection")) {
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    } else {
//        db = QSqlDatabase::addDatabase("QSQLITE");
//    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("station.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return;
    }

    QSqlQuery query;

    // 构建查询语句，根据参数k来选择归属路线字段
    QString tableName = "LineID" + QString::number(k); // 构建表名
    QString queryStr = "SELECT 站点编号, 站点名称 FROM " + tableName;
    QString maxRowQueryStr = "SELECT count(*) FROM " + tableName;
    int maxrow = 0;
    if (!query.exec(maxRowQueryStr)) {
        qDebug() << "查询maxrow失败:" << query.lastError().text();
                                         db.close();
        return;
    }
    if (query.next()) {
        maxrow = query.value(0).toInt();
    }

    if (!query.exec(queryStr)) {
        qDebug() << "查询失败:" << query.lastError().text();
        db.close();
        return;
    }
    int nodesPerRow;
    if(maxrow>32)
    {
        nodesPerRow = 9;

    }
    else if(maxrow>10&&maxrow<=32) {
        nodesPerRow = 8;
    }
    int spacing = 160; // 节点之间的间距
    //nodesPerRow = maxrow/3; // 每行的节点数量
    int row = 0;
    int column = 0;
    bool forward = true; // 控制方向
    bool drawLine = true; // 控制是否绘制线
    while (query.next()) {
        int no = query.value(0).toInt();
        QString name = query.value(1).toString();

        int x, y;

        if (forward) {
            x = column * spacing;
        } else {
            x = (nodesPerRow - column - 1) * spacing;
        }

        y = row * spacing;

        text = sceneLine->addText(name, QFont("宋体", 15));
        text->setPos(x + 30, y + 30);
        if (no==maxrow) {
            drawLine = false; // 到达最后一个点后停止画线
        }
         if (drawLine) {
            if (column < nodesPerRow-1 && forward == 1) {
                sceneLine->addLine(x, y, x + spacing, y, outlinePen1);
            } else if (column < nodesPerRow-1 && forward == 0) {

                sceneLine->addLine(x, y, x - spacing, y, outlinePen1);
            }

         }

         column++;

         if (column == nodesPerRow) {
             if(drawLine){
             row++;
             column = 0;
             forward = !forward; // 切换方向
             if (no != maxrow) { // 如果不是最后一个节点
                 int startY = y ; // 从第八个节点的底部开始
                 int endY = startY + spacing; // 垂直方向距离
                 sceneLine->addLine(x, startY, x, endY, outlinePen1);
             }
             }
         }
        // scene->addEllipse(x-25, y-25, 50, 50,noPen,blueBrush);
          QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-25, -25, 50, 50);
         item->setPos(x,y);
         item->setPen(Qt::NoPen);
         item->setBrush(QBrush(Qt::blue));
         item->setFlag(QGraphicsItem::ItemIsSelectable);
         sceneLine->addItem(item);
    }
    query.clear();
    // 关闭数据库连接
    db.close();
    QSqlDatabase::removeDatabase("QSQLITE");

}
//鼠标单击事件
void MainWindow::do_mouseClicked(QPoint point) {

    QPointF pointScene = ui->graphicsView->mapToScene(point); // 转换到Scene坐标
    QGraphicsItem *item;
    if(sceneStatus)
    {
        item = scene->itemAt(pointScene, ui->graphicsView->transform()); // 获取光标下的图形项
    }
    else{

        item = nullptr;

    }

    qint16 v1,v2;
    QString endstationName;

    QString startStationName = "上新街";


    if (item != nullptr) {
        // 判断图形项的类型，根据类型显示坐标
        if (item->type() == QGraphicsEllipseItem::Type || item->type() == QGraphicsRectItem::Type) {
            QPointF itemPos = item->pos();

            //qDebug()<<itemPos;

            endstationName = findStationName(stationInfoList,itemPos);

            v1=SearchMap(startStationName);
           // qDebug()<<v1;

            //qDebug()<<StationList.at(v1).stationName;
            v2=SearchMap(endstationName);
            //qDebug()<<v2;
            //qDebug()<<StationList.at(v2).stationName;

            if(v1==v2)
            {
                QMessageBox::warning(this,"warning","please input unique station");
            }
            else{
                dijkstra(v1,v2,subwayMap);
                //tracePath(v1,v2,prev);
                 tracePath1(v1,v2,prev1);
                PrintPath(v1,v2,ConsumeInfo);
                emit showPayWidget();
                emit sendPath(ConsumeInfo.path);
                emit targetStation(startStationName,endstationName);
                emit sendCost(ConsumeInfo.money);

                //paywindow->setMoneyLabel(ConsumeInfo.money);
                ConsumeInfo={};
            }
            // QMessageBox::information(this, "这是", path);

        }
    }




}


void MainWindow::PrintSubwayMap(const QVector<QVector<SubwayConnection>>& subwayMap) {
    for (int i = 0; i < subwayMap.size(); ++i) {
        qDebug() << "Station" <<i<<StationList.at(i).stationName << "connections:";
        for (int j = 0; j < subwayMap[i].size(); ++j) {
            qDebug() << "To station" << StationList.at(j).stationName << ": lineID =" << subwayMap[i][j].lineID << ", weight =" << subwayMap[i][j].weight;
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        // 如果按住了Ctrl键
        int angle = event->angleDelta().y();

        if (angle > 0) {
            // 鼠标滚轮向上滚动，放大内容
            ui->graphicsView->scale(1.2, 1.2);  // 调整放大倍数

        } else {
            // 鼠标滚轮向下滚动，缩小内容
            ui->graphicsView->scale( 0.9,  0.9);  // 调整缩小倍数
        }
    }
}







void MainWindow::on_all_line_pb_clicked() {
    delete scene;
    delete sceneLine;
    sceneStatus = true;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resetTransform();

    draw_map();

}
void MainWindow::on_line1_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(1);
}

void MainWindow::on_line2_pb_clicked() {

    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(2);

}



void MainWindow::on_line3_pb_clicked()
{
    // delete scene;
    // delete sceneLine;
    delete scene;
    delete sceneLine;
       sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(3);
}


void MainWindow::on_line4_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(4);
}


void MainWindow::on_line5_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(5);
}


void MainWindow::on_line6_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(6);
}


void MainWindow::on_line7_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(7);
}


void MainWindow::on_line8_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(8);

}


void MainWindow::on_line9_pb_clicked()
{
    delete scene;
    delete sceneLine;
    sceneStatus = false;
    scene = new QGraphicsScene(this);
    sceneLine = new QGraphicsScene(this);
    ui->graphicsView->setScene(sceneLine);
    ui->graphicsView->resetTransform();
    drawLine(9);

}


void MainWindow::on_ticket1_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(2);
    emit sendPath(NULL);
}


void MainWindow::on_ticket2_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(3);
    emit sendPath(NULL);
}


void MainWindow::on_ticket3_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(4);
    emit sendPath(NULL);
}


void MainWindow::on_ticket4_pb_clicked()
{
    emit showPayWidget();
     emit sendCost(5);
}


void MainWindow::on_ticket5_pb_clicked()
{
    emit showPayWidget();
     emit sendCost(6);
}


void MainWindow::on_ticket6_pb_clicked()
{
    emit showPayWidget();
     emit sendCost(7);
}


void MainWindow::on_ticket7_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(8);
}


void MainWindow::on_ticket8_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(9);
    emit sendPath(NULL);
}


void MainWindow::on_ticket9_pb_clicked()
{
    emit showPayWidget();
    emit sendCost(10);
    emit sendPath(NULL);
}

void MainWindow::on_pushButton_jfb_clicked()
{

}


void MainWindow::on_pushButton_ctm_clicked()
{

}


void MainWindow::on_pushButton_hyd_clicked()
{

}


void MainWindow::on_pushButton_cqk_clicked()
{

}


void MainWindow::on_pushButton_nsyks_clicked()
{

}

