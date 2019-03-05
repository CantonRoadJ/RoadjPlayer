#ifndef PIXMAPIMAGE_H
#define PIXMAPIMAGE_H
#include <QQuickPaintedItem>
#include <QPainter>
class PixmapImage : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PixmapImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void setImage(QPixmap pixmap);

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

private:
    QPixmap myPixmap;
   // PixmapContainer m_pixmapContainer;
};

#endif // PIXMAPIMAGE_H
