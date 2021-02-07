#pragma once
#include <score/graphics/widgets/Constants.hpp>
#include <score/graphics/widgets/QGraphicsSliderBase.hpp>

#include <QObject>
#include <QGraphicsItem>
#include <verdigris>

#include <score_lib_base_export.h>

namespace score
{
class SCORE_LIB_BASE_EXPORT QGraphicsSlider final : public QObject,
        public QGraphicsSliderBase<QGraphicsSlider>
{
    W_OBJECT(QGraphicsSlider)
    friend struct DefaultGraphicsSliderImpl;
    friend struct QGraphicsSliderBase<QGraphicsSlider>;

    double m_value{};

public:
    double min{}, max{};

private:
    bool m_grab{};

public:
    QGraphicsSlider(QGraphicsItem* parent);

    double unmap(double v) const noexcept { return (v - min) / (max - min); }
    double map(double v) const noexcept { return (v * (max - min)) + min; }

    void setRange(double min, double max);
    void setValue(double v);
    double value() const;

    bool moving = false;

public:
    void sliderMoved() E_SIGNAL(SCORE_LIB_BASE_EXPORT, sliderMoved)
    void sliderReleased() E_SIGNAL(SCORE_LIB_BASE_EXPORT, sliderReleased)

private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
}
