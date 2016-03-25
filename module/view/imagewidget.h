#pragma once

#include <QWidget>
#include <QImage>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = 0);

    void setImage(const QString& path);
    void setImage(const QImage& image);
    QString imagePath() const;
    qreal scaleValue() const {return m_scale;}
    void setTransformOrigin(const QPoint& imageP, const QPoint& deviceP);
    QPoint mapToImage(const QPoint& p);
Q_SIGNALS:
    void scaleValueChanged(qreal);
public Q_SLOTS:
    void setScaleValue(qreal value);
    void rotateClockWise() {rotate(m_rot-90);}
    void rotateAntiClockWise() {rotate(m_rot+90);}
    void rotate(int deg);
    void flipX();
    void flipY();
protected:
    void paintEvent(QPaintEvent *);
    //void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    void updateTransform();

    int m_flipX = 1;
    int m_flipY = 1;
    int m_rot = 0;
    qreal m_scale = 0; // when an image is loaded to fit widget, m_scale is not 1.0
    qreal m_scale_requested = 0;
    QPoint m_o_img, m_o_dev;
    QString m_path;
    QImage m_image;
    QPixmap m_pixmap;
    QTransform m_mat;

    QPoint m_pos, m_posG;
};
