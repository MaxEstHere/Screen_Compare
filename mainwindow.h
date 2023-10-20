#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "snapshotcomparator.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onStartStopButtonClicked();
    void onNewSnapshot(const QPixmap &snapshot, double similarity);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *currentSnapshotItem;
    QTimer *snapshotTimer;
    SnapshotComparator *comparator;
    bool isComparing;
    QPixmap snapshotToCompare;

    void setupUI();
    void startSnapshotComparison();
    void stopSnapshotComparison();
};

#endif // MAINWINDOW_H
