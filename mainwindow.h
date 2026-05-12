#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>

class Player;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override; // 处理键盘按下

private slots:
    void startGame(); // 开始新游戏
    void gameLoop();  // 游戏主循环，每帧执行

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;   // 游戏场景
    QGraphicsView *view;     // 场景视图
    QTimer *timer;           // 游戏循环定时器
    Player *player;          // 玩家对象
};

#endif // MAINWINDOW_H