// snapshotcomparator.h

#ifndef SNAPSHOTCOMPARATOR_H
#define SNAPSHOTCOMPARATOR_H

#include <QObject>
#include <QPixmap>

class SnapshotComparator : public QObject
{
    Q_OBJECT
public:
    explicit SnapshotComparator(QObject *parent = nullptr);

public slots:
    void setImages(const QPixmap &currentSnapshot, const QPixmap &snapshotToCompare);
    void compare();

signals:
    void finished(const QPixmap &snapshot, double similarity);

private:
    QPixmap currentSnapshot;
    QPixmap snapshotToCompare;

    double calculateSimilarity();
    double compareImages(const QImage &image1, const QImage &image2);
};

#endif // SNAPSHOTCOMPARATOR_H
