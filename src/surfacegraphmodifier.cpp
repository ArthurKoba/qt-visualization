// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "surfacegraphmodifier.h"

#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dtheme.h>

#include <QtGui/qimage.h>
#include <QtGui/qquaternion.h>

#include <QtCore/qmath.h>

using namespace Qt::StringLiterals;

const int sampleCountX = 100;
const int sampleCountZ = 100;
const float sampleMin = 1.f;
const float sampleMax = 1.f;

SurfaceGraphModifier::SurfaceGraphModifier(Q3DSurface *surface, QLabel *label, QObject *parent) :
      QObject(parent),
      m_graph(surface),
      m_textField(label)
{

    updateElapsedTimer.start();

    m_graph->scene()->activeCamera()->setZoomLevel(85.f);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRight);
    m_graph->activeTheme()->setType(Q3DTheme::ThemeRetro);

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    //
    // Sqrt Sin
    //
    //! [0]
    m_sqrtSinProxy = new QSurfaceDataProxy();
    m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);
    //! [0]
    fillSqrtSinProxy();


    // Custom items and label
    connect(m_graph, &QAbstract3DGraph::selectedElementChanged,
            this, &SurfaceGraphModifier::handleElementSelected);

    m_selectionAnimation = new QPropertyAnimation(this);
    m_selectionAnimation->setPropertyName("scaling");
    m_selectionAnimation->setDuration(500);
    m_selectionAnimation->setLoopCount(-1);

    QFont titleFont = QFont("Century Gothic", 30);
    titleFont.setBold(true);
    m_titleLabel = new QCustom3DLabel("Oil Rigs on Imaginary Sea", titleFont,
                                      QVector3D(0.f, 1.2f, 0.f),
                                      QVector3D(1.f, 1.f, 0.f),
                                      QQuaternion());
    m_titleLabel->setPositionAbsolute(true);
    m_titleLabel->setFacingCamera(true);
    m_titleLabel->setBackgroundColor(QColor(0x66cdaa));
    m_graph->addCustomItem(m_titleLabel);
    m_titleLabel->setVisible(false);

}

void SurfaceGraphModifier::fillSqrtSinProxy()
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    //! [1]
    auto *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0 ; i < sampleCountZ ; ++i) {
        auto *newRow = new QSurfaceDataRow;
        newRow->reserve(sampleCountX);
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        for (int j = 0; j < sampleCountX; ++j) {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float R = qSqrt(z * z+ x * x) + 0.01f;
            float y = (qSin(R) / R + 0.24f) * 1.61f;
            newRow->append(QSurfaceDataItem({static_cast<float>(i - 8), static_cast<float>(sin(i * j)), static_cast<float>(j - 8) }));
            newRow->append(QSurfaceDataItem({x, y, z}));
        }
        dataArray->append(newRow);
    }

    m_sqrtSinProxy->resetArray(dataArray);
    //! [1]
}


void SurfaceGraphModifier::update(Spectrogram *spectrogram) {
    if (not spectrogram or spectrogram->channels() < 3) return;
    if (updateElapsedTimer.elapsed() < 25) return; // Пропускаем, если прошло < 25 мс
    updateElapsedTimer.restart();



    m_graph->axisX()->setRange(0, static_cast<float>(spectrogram->channels()/2));
    m_graph->axisZ()->setRange(0, static_cast<float>(spectrogram->history_size()/2));

    auto *dataArray = new QSurfaceDataArray();

    dataArray->reserve(static_cast<qsizetype>(spectrogram->channels()));

    for (int ch = 0; ch < spectrogram->channels(); ch++) {
        auto channel = spectrogram->get_channel_history_reversed(ch);
        auto *newRow = new QSurfaceDataRow;
        newRow->reserve(static_cast<qsizetype>(channel.size()));
        for (size_t j = 0; j < channel.size(); ++j) {
            newRow->append(QSurfaceDataItem({static_cast<float>(ch), static_cast<float>(channel[j]), static_cast<float>(j) }));
        }
        dataArray->append(newRow);
    }
    m_sqrtSinProxy->resetArray(dataArray);

}

void SurfaceGraphModifier::update(std::vector<float> &data) {



    if (updateElapsedTimer.elapsed() < 10) return; // Пропускаем, если прошло < 25 мс
    updateElapsedTimer.restart();

    m_graph->axisZ()->setRange(0, data.size());

    auto *dataArray = new QSurfaceDataArray;
    dataArray->reserve(8);
//
    for (int i = 0; i < 8; ++i) {
        auto *newRow = new QSurfaceDataRow;
        newRow->reserve(static_cast<qsizetype>(data.size()));
        for (size_t j = 0; j < data.size(); ++j) {
            newRow->append(QSurfaceDataItem({static_cast<float>(j), static_cast<float>(data[j]), static_cast<float>(i) }));
        }
        dataArray->append(newRow);
    }


//    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
//    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
//    auto *dataArray = new QSurfaceDataArray;
//    dataArray->reserve(sampleCountZ);
//    for (int i = 0 ; i < sampleCountZ ; ++i) {
//        auto *newRow = new QSurfaceDataRow;
//        newRow->reserve(sampleCountX);
//        float z = qMin(sampleMax, (i * stepZ + sampleMin));
//        for (int j = 0; j < sampleCountX; ++j) {
//            float x = qMin(sampleMax, (j * stepX + sampleMin));
//            float R = qSqrt(z * z+ x * x) + 0.01f;
//            float y = (qSin(R) / R + 0.24f) * 1.61f;
//            newRow->append(QSurfaceDataItem({x, y, z}));
//        }
//        dataArray->append(newRow);
//    }


    m_sqrtSinProxy->resetArray(dataArray);
}

void SurfaceGraphModifier::enableSqrtSinModel(bool enable)
{
    if (enable) {
        //! [3]
        m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_sqrtSinSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(sampleMin, sampleMax);
        m_graph->axisY()->setRange(0.f, 2.f);
        m_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_graph->axisX()->setLabelAutoRotation(30.f);
        m_graph->axisY()->setLabelAutoRotation(90.f);
        m_graph->axisZ()->setLabelAutoRotation(30.f);


        m_graph->addSeries(m_sqrtSinSeries);
        //! [3]

        m_titleLabel->setVisible(false);
        m_graph->axisX()->setTitleVisible(false);
        m_graph->axisY()->setTitleVisible(false);
        m_graph->axisZ()->setTitleVisible(false);

        m_graph->axisX()->setTitle({});
        m_graph->axisY()->setTitle({});
        m_graph->axisZ()->setTitle({});

        m_graph->setActiveInputHandler(m_defaultInputHandler);

        //! [6]
        // Reset range sliders for Sqrt & Sin
        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMinimum(0);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMinimum(1);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMinimum(0);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMinimum(1);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);
        //! [6]
    }
}





void SurfaceGraphModifier::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraphModifier::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraphModifier::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraphModifier::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

//! [7]
void SurfaceGraphModifier::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}
//! [7]

void SurfaceGraphModifier::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraphModifier::setBlackToYellowGradient()
{
    //! [8]
    QLinearGradient gr;
    gr.setColorAt(0.f, Qt::black);
    gr.setColorAt(0.33f, Qt::blue);
    gr.setColorAt(0.67f, Qt::red);
    gr.setColorAt(1.f, Qt::yellow);

    m_sqrtSinSeries->setBaseGradient(gr);
    m_sqrtSinSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //! [8]
}

void SurfaceGraphModifier::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.f, Qt::darkGreen);
    gr.setColorAt(0.5f, Qt::yellow);
    gr.setColorAt(0.8f, Qt::red);
    gr.setColorAt(1.f, Qt::darkRed);

    m_sqrtSinSeries->setBaseGradient(gr);
    m_sqrtSinSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


void SurfaceGraphModifier::handleElementSelected(QAbstract3DGraph::ElementType type)
{
    resetSelection();
    if (type == QAbstract3DGraph::ElementCustomItem) {
        QCustom3DItem *item = m_graph->selectedCustomItem();
        QString text;
        if (qobject_cast<QCustom3DLabel *>(item) != 0) {
            text.append("Custom label: ");
        } else {
            QStringList split = item->meshFile().split("/");
            text.append(split.last());
            text.append(": ");
        }
        int index = m_graph->selectedCustomItemIndex();
        text.append(QString::number(index));
        m_textField->setText(text);
        m_previouslyAnimatedItem = item;
        m_previousScaling = item->scaling();
        m_selectionAnimation->setTargetObject(item);
        m_selectionAnimation->setStartValue(item->scaling());
        m_selectionAnimation->setEndValue(item->scaling() * 1.5f);
        m_selectionAnimation->start();
    } else if (type == QAbstract3DGraph::ElementSeries) {
        QString text = "Surface (";
        QSurface3DSeries *series = m_graph->selectedSeries();
        if (series) {
            QPoint point = series->selectedPoint();
            QString posStr;
            posStr.setNum(point.x());
            text.append(posStr);
            text.append(", ");
            posStr.setNum(point.y());
            text.append(posStr);
        }
        text.append(")");
        m_textField->setText(text);
    } else if (type > QAbstract3DGraph::ElementSeries
               && type < QAbstract3DGraph::ElementCustomItem) {
        int index = m_graph->selectedLabelIndex();
        QString text;
        if (type == QAbstract3DGraph::ElementAxisXLabel)
            text.append("Axis X label: ");
        else if (type == QAbstract3DGraph::ElementAxisYLabel)
            text.append("Axis Y label: ");
        else
            text.append("Axis Z label: ");
        text.append(QString::number(index));
        m_textField->setText(text);
    } else {
        m_textField->setText("Nothing");
    }
}

void SurfaceGraphModifier::resetSelection()
{
    m_selectionAnimation->stop();
    if (m_previouslyAnimatedItem)
        m_previouslyAnimatedItem->setScaling(m_previousScaling);
    m_previouslyAnimatedItem = nullptr;
}
