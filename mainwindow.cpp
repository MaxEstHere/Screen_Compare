#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QOverload>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();

    snapshotTimer = new QTimer(this);
    connect(snapshotTimer, &QTimer::timeout, this, &MainWindow::startSnapshotComparison);

    comparator = new SnapshotComparator;
    connect(comparator, &SnapshotComparator::finished, this, &MainWindow::onNewSnapshot);

    isComparing = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    currentSnapshotItem = new QGraphicsPixmapItem;
    scene->addItem(currentSnapshotItem);

    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopButtonClicked);
}

void MainWindow::onStartStopButtonClicked()
{
    if (isComparing)
    {
        stopSnapshotComparison();
    }
    else
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty())
        {
            QPixmap snapshot(fileName);
            currentSnapshotItem->setPixmap(snapshot.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
            snapshotToCompare = snapshot;
            snapshotTimer->start(60000);
        }
    }
}

void MainWindow::startSnapshotComparison()
{
    QImage currentSnapshot = currentSnapshotItem->pixmap().toImage().scaled(100, 100);

    QThread *comparisonThread = new QThread;

    comparator->moveToThread(comparisonThread);
    connect(comparisonThread, &QThread::started, comparator, &SnapshotComparator::compare, Qt::DirectConnection);
    connect(comparator, &SnapshotComparator::finished, comparisonThread, &QThread::quit);
    connect(comparator, &SnapshotComparator::finished, comparator, &SnapshotComparator::deleteLater);
    connect(comparator, &SnapshotComparator::finished, comparisonThread, &QThread::deleteLater);

    comparisonThread->start();
    QMetaObject::invokeMethod(comparator, "setImages", Qt::QueuedConnection,
                              Q_ARG(QPixmap, QPixmap::fromImage(currentSnapshot)),
                              Q_ARG(QPixmap, snapshotToCompare));

    isComparing = true;
}

void MainWindow::stopSnapshotComparison()
{
    snapshotTimer->stop();
    isComparing = false;
}

void MainWindow::onNewSnapshot(const QPixmap &snapshot, double similarity)
{
    ui->similarityLabel->setText(QString("Similarity: %1%").arg(similarity));
    scene->clear();
    scene->addPixmap(snapshotToCompare);
    scene->addPixmap(snapshot);
}
