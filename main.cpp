#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QPaintEvent>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <math.h>
#include <QtMath>
#include <QTimer>

class BatteryChargeDisplay : public QWidget {
public:
    BatteryChargeDisplay(QWidget *parent = nullptr) : QWidget(parent), current(0) {
        setFixedSize(400, 400);

        // Set up the damping factor for smooth movement
        dampingFactor = 0.2;

        // Set up the timer for periodic updates
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &BatteryChargeDisplay::updateCharge);
        timer->start(100); // Update every 100 milliseconds
    }

    void updateCharge() {
        // Simulate current fluctuations within the given range
        current = (qrand() % 21) - 10; // Random value between -10 and 10

        // Normalize the current value to the range -212° to 0°
        int angle = (current + 212) * 10;

        // Apply the damping method for smooth movement
        int targetAngle = angle + dampingFactor * (angle - previousAngle);
        previousAngle = targetAngle;

        // Update the display
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Clear the background
        painter.setBrush(Qt::white);
        painter.drawRect(rect());

        // Draw the semicircular scale
        painter.setPen(Qt::black);
        //painter.draw
        //painter.drawArc(50, 50, 300, 300, -30 * 16, -120 * 16);

        // Draw the arrow pointer
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::red);
        painter.save();
        QPainterPath arrow;
     // Calculate the target angle in radians
        qreal targetAngleRad = (previousAngle - 93) * M_PI / 180.0;
        qreal targetAngleGrad = (previousAngle - 93);

        // Calculate the position of the arrow
        int radius = 150;
        int centerX = 200;
        int centerY = 200;
//        int x = centerX + radius * qCos(targetAngleRad);
//        int y = centerY - radius * qSin(targetAngleRad);

        painter.translate(centerX, centerY);
        painter.rotate(targetAngleGrad);
        arrow.addEllipse(-150.0, -150.0, 300.0, 300.0);
        arrow.addEllipse(-148.0, -148.0, 295.0, 295.0);
        arrow.moveTo(0.0, 0.0);
        arrow.lineTo(-20.0, -20.0);
        arrow.lineTo(0.0, -150.0);
        arrow.lineTo(20.0, -20.0);
        arrow.lineTo(0.0, 0.0);
        arrow.moveTo(0.0, 0.0);

        painter.fillPath(arrow, Qt::red);
        painter.drawPath(arrow);
        painter.restore();

      //  painter.drawPolygon(QPolygonF() << QPointF(centerX, centerY) << QPointF(x, y));
    }

private:
    QTimer *timer;
    qreal current;
    qreal previousAngle;
    qreal dampingFactor;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BatteryChargeDisplay display;
    display.show();

    return app.exec();
}
