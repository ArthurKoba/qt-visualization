#include "visualization/surfaces/surfacegraph.h"
#include "visualization/surfaces/surfacegraphmodifier.h"

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qcommandlinkbutton.h>
#include <QtGui/qpainter.h>

using namespace Qt::StringLiterals;

static QPixmap gradientBtoYPB_Pixmap() {
    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.f, Qt::black);
    grBtoY.setColorAt(0.67f, Qt::blue);
    grBtoY.setColorAt(0.33f, Qt::red);
    grBtoY.setColorAt(0.f, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    return pm;
}

static QPixmap gradientGtoRPB_Pixmap() {
    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.f, Qt::darkGreen);
    grGtoR.setColorAt(0.5f, Qt::yellow);
    grGtoR.setColorAt(0.2f, Qt::red);
    grGtoR.setColorAt(0.f, Qt::darkRed);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    return pm;
}

static QPixmap highlightPixmap() {
    constexpr int height = 400;
    constexpr int width = 110;
    constexpr int border = 10;
    QLinearGradient gr(0, 0, 1, height - 2 * border);
    gr.setColorAt(1.f, Qt::black);
    gr.setColorAt(0.8f, Qt::darkGreen);
    gr.setColorAt(0.6f, Qt::green);
    gr.setColorAt(0.4f, Qt::yellow);
    gr.setColorAt(0.2f, Qt::red);
    gr.setColorAt(0.f, Qt::darkRed);

    QPixmap pmHighlight(width, height);
    pmHighlight.fill(Qt::transparent);
    QPainter pmpHighlight(&pmHighlight);
    pmpHighlight.setBrush(QBrush(gr));
    pmpHighlight.setPen(Qt::NoPen);
    pmpHighlight.drawRect(border, border, 35, height - 2 * border);
    pmpHighlight.setPen(Qt::black);
    int step = (height - 2 * border) / 5;
    for (int i = 0; i < 6; ++i) {
        int yPos = i * step + border;
        pmpHighlight.drawLine(border, yPos, 55, yPos);
        const int height = 550 - (i * 110);
        pmpHighlight.drawText(60, yPos + 2, QString::number(height) + u" m"_s);
    }
    return pmHighlight;
}


SurfaceGraph::SurfaceGraph(QWidget *parent) : QWidget(parent) {
    // todo qt.core.qobject.connect: QObject::disconnect: Unexpected nullptr parameter - self widget error
    m_surfaceGraph = new Q3DSurface(nullptr, this->windowHandle());
}

SurfaceGraph::~SurfaceGraph() = default;

bool SurfaceGraph::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize) {
    if (!m_surfaceGraph->hasContext())
        return false;
    auto *hLayout = new QHBoxLayout(this);
    m_container = QWidget::createWindowContainer(m_surfaceGraph, this);
    m_container->setMinimumSize(minimumGraphSize);
    m_container->setMaximumSize(maximumGraphSize);
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_container->setFocusPolicy(Qt::StrongFocus);
    hLayout->addWidget(m_container, 1);

    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    auto *signalGroupBox = new QGroupBox(u"Signal Settings"_s);

    auto *pauseButton = new QCheckBox(this);
    pauseButton->setCheckState(_pause ? Qt::Unchecked : Qt::Checked);
    pauseButton->setText(u"Pause"_s);
    pauseButton->setChecked(false);

    auto *pauseVBox = new QVBoxLayout;
    pauseVBox->addWidget(pauseButton);
    signalGroupBox->setLayout(pauseVBox);

    // Create control widgets
    auto *modelGroupBox = new QGroupBox(u"Model"_s);

    auto *toggle3DModelRB = new QRadioButton(this);
    toggle3DModelRB->setText(u"3D"_s);
    toggle3DModelRB->setChecked(false);

    auto *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(toggle3DModelRB);
    modelGroupBox->setLayout(modelVBox);

    auto *selectionGroupBox = new QGroupBox(u"Graph Selection Mode"_s);

    auto *modeNoneRB = new QRadioButton(this);
    modeNoneRB->setText(u"No selection"_s);
    modeNoneRB->setChecked(false);

    auto *modeItemRB = new QRadioButton(this);
    modeItemRB->setText(u"Item"_s);
    modeItemRB->setChecked(false);

    auto *modeSliceRowRB = new QRadioButton(this);
    modeSliceRowRB->setText(u"Time Slice"_s);
    modeSliceRowRB->setChecked(false);

    auto *modeSliceColumnRB = new QRadioButton(this);
    modeSliceColumnRB->setText(u"Frequency Slice"_s);
    modeSliceColumnRB->setChecked(false);

    auto *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    auto *axisGroupBox = new QGroupBox(u"Axis ranges"_s);

    auto *axisMinSliderX = new QSlider(Qt::Horizontal);
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    auto *axisMaxSliderX = new QSlider(Qt::Horizontal);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);

    auto *axisMinSliderZ = new QSlider(Qt::Horizontal);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    auto *axisMaxSliderZ = new QSlider(Qt::Horizontal);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

    auto *axisVBox = new QVBoxLayout;
    axisVBox->addWidget(new QLabel(u"Time range (x)"_s));
    axisVBox->addWidget(axisMinSliderX);
    axisVBox->addWidget(axisMaxSliderX);
    axisVBox->addWidget(new QLabel(u"Freq range (z)"_s));
    axisVBox->addWidget(axisMinSliderZ);
    axisVBox->addWidget(axisMaxSliderZ);
    axisGroupBox->setLayout(axisVBox);

    auto *colorGroupBox = new QGroupBox(u"Custom gradient"_s);

    QPixmap pixmap = gradientBtoYPB_Pixmap();
    auto *gradientBtoYPB = new QPushButton(this);
    gradientBtoYPB->setIcon(QIcon(pixmap));
    gradientBtoYPB->setIconSize(pixmap.size());

    pixmap = gradientGtoRPB_Pixmap();
    auto *gradientGtoRPB = new QPushButton(this);
    gradientGtoRPB->setIcon(QIcon(pixmap));
    gradientGtoRPB->setIconSize(pixmap.size());
    auto *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    // Multiseries heightmap


    auto *labelSelectedItem = new QLabel(u"Nothing"_s);
    labelSelectedItem->setVisible(false);


    auto *label = new QLabel(this);
    label->setPixmap(highlightPixmap());


    // Common
    vLayout->addWidget(signalGroupBox);
    vLayout->addWidget(modelGroupBox);
    vLayout->addWidget(selectionGroupBox);
    vLayout->addWidget(axisGroupBox);

    // Sqrt Sin
    vLayout->addWidget(colorGroupBox);

    // Multiseries heightmap
    vLayout->addWidget(labelSelectedItem);

    // Create the controller
    _modifier = new SurfaceGraphModifier(m_surfaceGraph, labelSelectedItem, this);


    connect(pauseButton, &QCheckBox::checkStateChanged, this, [this](const Qt::CheckState state) {
        _pause = not _pause;
        _need_update = true;
    });

    connect(toggle3DModelRB, &QRadioButton::toggled,
            _modifier, &SurfaceGraphModifier::enable3DModel);


    connect(modeNoneRB, &QRadioButton::toggled,
            _modifier, &SurfaceGraphModifier::toggleModeNone);
    connect(modeItemRB, &QRadioButton::toggled,
            _modifier, &SurfaceGraphModifier::toggleModeItem);
    connect(modeSliceRowRB, &QRadioButton::toggled,
            _modifier, &SurfaceGraphModifier::toggleModeSliceRow);
    connect(modeSliceColumnRB, &QRadioButton::toggled,
            _modifier, &SurfaceGraphModifier::toggleModeSliceColumn);

    connect(axisMinSliderX, &QSlider::valueChanged,
            _modifier, &SurfaceGraphModifier::adjustXMin);
    connect(axisMaxSliderX, &QSlider::valueChanged,
            _modifier, &SurfaceGraphModifier::adjustXMax);
    connect(axisMinSliderZ, &QSlider::valueChanged,
            _modifier, &SurfaceGraphModifier::adjustZMin);
    connect(axisMaxSliderZ, &QSlider::valueChanged,
            _modifier, &SurfaceGraphModifier::adjustZMax);

    connect(gradientBtoYPB, &QPushButton::pressed,
            _modifier, &SurfaceGraphModifier::setBlackToYellowGradient);
    connect(gradientGtoRPB, &QPushButton::pressed,
            _modifier, &SurfaceGraphModifier::setGreenToRedGradient);

    connect(toggle3DModelRB, &QRadioButton::toggled,
            colorGroupBox, &QGroupBox::setVisible);

    _modifier->setAxisMinSliderX(axisMinSliderX);
    _modifier->setAxisMaxSliderX(axisMaxSliderX);
    _modifier->setAxisMinSliderZ(axisMinSliderZ);
    _modifier->setAxisMaxSliderZ(axisMaxSliderZ);
    _modifier->setGreenToRedGradient();

    toggle3DModelRB->setChecked(true);
    modeItemRB->setChecked(true);

    connect(&_data_update_timer, &QTimer::timeout, this, [this]() {
        if (_pause or not _need_update or not _modifier) return;
        _modifier->update(_spectrogram);
        _need_update = false;
    });

    _data_update_timer.setInterval(1000 / 60);
    _data_update_timer.start();

    return true;
}

void SurfaceGraph::update(const std::vector<std::vector<float> > &spectrogram) {
    _spectrogram = spectrogram;
    _need_update = true;
}
