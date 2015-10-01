#pragma once
#include <QGraphicsItem>
#include <iscore/tools/SettableIdentifier.hpp>
class CurveSegmentModel;
class CurveSegmentView : public QGraphicsObject
{
        Q_OBJECT
    public:
        CurveSegmentView(
                const CurveSegmentModel* model,
                QGraphicsItem* parent);

        const Id<CurveSegmentModel>& id() const;
        int type() const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void setModel(const CurveSegmentModel*);
        const CurveSegmentModel& model() const
        { return *m_model; }

        void setRect(const QRectF& theRect);

        void setSelected(bool selected);

        void enable();
        void disable();

    signals:
        void contextMenuRequested(const QPoint&);

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    private:
        void updatePoints();
        // Takes a table of points and draws them in a square given by the boundingRect
        // QGraphicsItem interface
        QRectF m_rect;

        const CurveSegmentModel* m_model{};
        bool m_selected{};

        QPainterPath m_unstrockedShape;

        bool m_enabled{true};
};
