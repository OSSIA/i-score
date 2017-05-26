#include "ProcessFactory.hpp"
#include "ProcessList.hpp"
#include "StateProcessFactoryList.hpp"
#include <Process/LayerModelPanelProxy.hpp>
#include <Process/Process.hpp>
#include <Process/LayerView.hpp>
#include <iscore/model/path/PathSerialization.hpp>
#include <iscore/model/path/RelativePath.hpp>

namespace Process
{
ProcessModelFactory::~ProcessModelFactory() = default;
LayerFactory::~LayerFactory() = default;
ProcessFactoryList::~ProcessFactoryList() = default;
LayerFactoryList::~LayerFactoryList() = default;
StateProcessList::~StateProcessList() = default;

class DefaultLayerView final : public LayerView
{
public:
    DefaultLayerView(QGraphicsItem* parent): LayerView(parent) { }
    void paint_impl(QPainter* p) const override
    {
        QTextOption o;
        o.setAlignment(Qt::AlignCenter);
        p->setPen(Qt::white);
        p->drawText(boundingRect(), m_txt, o);
    }
    QString m_txt;
};

class DefaultLayerPresenter final : public LayerPresenter
{
    const Process::ProcessModel& m_model;
    Process::LayerView* m_view{};

    public:
      DefaultLayerPresenter(
              const Process::ProcessModel& model,
              Process::LayerView* v,
              const ProcessPresenterContext& ctx,
              QObject* parent)
          : LayerPresenter{ctx, parent}
          , m_model{model}
          , m_view{v}
      {
          auto vi = dynamic_cast<DefaultLayerView*>(v);
          vi->m_txt = m_model.metadata().getName();
          connect(&m_model.metadata(), &iscore::ModelMetadata::NameChanged,
                  this, [=] (auto t) { vi->m_txt = t; vi->update();});
      }

      ~DefaultLayerPresenter()
      {

      }

      void setWidth(qreal width) override { m_view->setWidth(width); }
      void setHeight(qreal height) override { m_view->setHeight(height); }

      void putToFront() override { m_view->setVisible(true); }
      void putBehind() override { m_view->setVisible(false); }

      void on_zoomRatioChanged(ZoomRatio) override { }
      void parentGeometryChanged() override {}

      const ProcessModel& model() const override { return m_model; }
      const Id<ProcessModel>& modelId() const override { return m_model.id(); }
};
LayerPresenter *LayerFactory::makeLayerPresenter(
        const ProcessModel& m,
        LayerView * v,
        const ProcessPresenterContext &context,
        QObject *parent)
{
    return new DefaultLayerPresenter{m, v, context, parent};
}

LayerView *LayerFactory::makeLayerView(const ProcessModel &view, QGraphicsItem *parent)
{
  return new DefaultLayerView{parent};
}

Process::MiniLayer* LayerFactory::makeMiniLayer(
    const ProcessModel& view,
    QGraphicsItem* parent)
{
  return nullptr;
}

LayerPanelProxy*
LayerFactory::makePanel(const ProcessModel& layer, QObject* parent)
{
  return new Process::GraphicsViewLayerPanelProxy{layer, parent};
}

bool LayerFactory::matches(const ProcessModel& p) const
{
  return matches(p.concreteKey());
}

bool LayerFactory::matches(
    const UuidKey<Process::ProcessModel>& p) const
{
  return false;
}

ProcessFactoryList::object_type* ProcessFactoryList::loadMissing(
    const VisitorVariant& vis, QObject* parent) const
{
  ISCORE_TODO;
  return nullptr;
}

StateProcessList::object_type*
StateProcessList::loadMissing(const VisitorVariant& vis, QObject* parent) const
{
  ISCORE_TODO;
  return nullptr;
}

LayerFactory*
LayerFactoryList::findDefaultFactory(const ProcessModel& proc) const
{
  return findDefaultFactory(proc.concreteKey());
}

LayerFactory* LayerFactoryList::findDefaultFactory(
    const UuidKey<ProcessModel>& proc) const
{
  for (auto& fac : *this)
  {
    if (fac.matches(proc))
      return &fac;
  }
  return nullptr;
}
}
