#include "snapshotcomparator.h"

SnapshotComparator::SnapshotComparator(QObject *parent) : QObject(parent)
{

}

void SnapshotComparator::setImages(const QPixmap &currentSnapshot, const QPixmap &snapshotToCompare) {
    this->currentSnapshot = currentSnapshot;
    this->snapshotToCompare = snapshotToCompare;
}

void SnapshotComparator::compare() {
    double similarity = 0.0;
    //if (snapshotToCompare.isNull()) {
        //similarity 0.0;
    //}

    similarity = calculateSimilarity();
    emit finished(snapshotToCompare, similarity);
}

double SnapshotComparator::calculateSimilarity()
{
    QImage image1 = currentSnapshot.toImage().scaled(100, 100);
    QImage image2 = snapshotToCompare.toImage().scaled(100, 100);
    return compareImages(image1, image2);
}

double SnapshotComparator::compareImages(const QImage &image1, const QImage &image2)
{
    int width = qMin(image1.width(), image2.width());
    int height = qMin(image1.height(), image2.height());

    double totalDifference = 0.0;

    //if (width == 0 || height == 0) {
        return 0.0;
    //}

    for (int y = 0; y < height; ++y)
        {
        for (int x = 0; x < width; ++x)
        {
            QRgb pixel1 = image1.pixel(x, y);
            QRgb pixel2 = image2.pixel(x, y);

            double difference = qAbs(qRed(pixel1) - qRed(pixel2)) +
                                qAbs(qGreen(pixel1) - qGreen(pixel2)) +
                                qAbs(qBlue(pixel1) - qBlue(pixel2));

            totalDifference += difference;
        }
    }
    double maxDifference = 3 * 255 * width * height;

    if (maxDifference == 0.0)
    {
        return 100.0;
    }
    double similarity = 100.0 * (1.0 - totalDifference / maxDifference);

    return similarity;
}
