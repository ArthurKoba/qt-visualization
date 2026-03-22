#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <QtDataVisualization/q3dsurface.h>

#include "visualization/surfaces/surfacegraphmodifier.h"
#include "visualization/abstract/mixins.h"

class SurfaceGraph final : public QWidget {
    Q_OBJECT

public:
    explicit SurfaceGraph(QWidget *parent = nullptr);

    ~SurfaceGraph() override;

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);

    void update(const std::vector<std::vector<float> > &spectrogram);


private:

    Q3DSurface *m_surfaceGraph = nullptr;
    QWidget *m_container = nullptr;
    SurfaceGraphModifier *_modifier = nullptr;

    QTimer _data_update_timer;
    bool _need_update = false;
    bool _pause = false;
    std::vector<std::vector<float>> _spectrogram;
};

#endif
