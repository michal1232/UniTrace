/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graph.h"

#include "noisynode.h"
#include "gridnode.h"
#include "linenode.h"

Graph::Graph()
    : m_samplesChanged(false)
    , m_geometryChanged(false)
{
    setFlag(ItemHasContents, true);
}


void Graph::appendSample(QList<qreal> value)
{
    for(qint32 i = 0; i < m_maxLine; i++) {
        m_samples[i] << value[i];
    }
    m_samplesChanged = true;
    //update();
}


void Graph::removeFirstSample()
{
    for(qint32 i = 0; i < m_maxLine; i++) {
        m_samples[i].removeFirst();
    }
    m_samplesChanged = true;
    //update();
}

void Graph::removeSamples(qint32 cntOfRemoved)
{
    for(qint32 i = 0; i < m_maxLine; i++) {
        while(cntOfRemoved != m_samples[i].size()) {
            m_samples[i].removeFirst();
        }
    }
    m_samplesChanged = true;
}

void Graph::updateGraph(qint32 gridSize, bool disGrid, bool disBck)
{
    _gridSize_ = gridSize;
    _disGrid_ = disGrid;
    _disBck_ = disBck;
    m_geometryChanged = true;
    update();
}

void Graph::updateArea()
{
    m_samplesChanged = true;
    update();
}

void Graph::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_geometryChanged = true;
    update();
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void Graph::setMaxLine(qint32 maxlLine)
{
    m_maxLine = maxlLine;
}


class GraphNode : public QSGNode
{
public:
    NoisyNode *background;
    GridNode *grid;
    LineNode *line[MAX_RENDER_LINE];
    LineNode *shadow;
};


QSGNode *Graph::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QColor colours[10] = {QColor("cyan"), QColor("magenta"), QColor("red"),
                          QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                          QColor("green"), QColor("darkGreen"), QColor("yellow"),
                          QColor("blue")};

    GraphNode *n= static_cast<GraphNode *>(oldNode);

    QRectF rect = boundingRect();

    if (rect.isEmpty()) {
        delete n;
        return nullptr;
    }

    if(_disBck_ == true) {
        n->removeAllChildNodes();

        n->appendChildNode(n->grid);

        for(qint32 i = 0; i < m_maxLine; i++) {
            n->line[i] = new LineNode(10, 0.5, colours[i]);
            n->appendChildNode(n->line[i]);
        //n->shadow = new LineNode(20, 0.2f, QColor::fromRgbF(0.2, 0.2, 0.2, 0.4));
        }

        _addBck_ = true;
    }
    if(_disBck_ == false && _addBck_ == true) {
        n->removeAllChildNodes();

        n->appendChildNode(n->background);
        n->appendChildNode(n->grid);

        for(qint32 i = 0; i < m_maxLine; i++) {
            n->line[i] = new LineNode(10, 0.5, colours[i]);
            n->appendChildNode(n->line[i]);
        //n->shadow = new LineNode(20, 0.2f, QColor::fromRgbF(0.2, 0.2, 0.2, 0.4));
        }

        _addBck_ = false;
    }
    if (!n) {
        n = new GraphNode();

        n->background = new NoisyNode(window());
        n->grid = new GridNode();

        n->appendChildNode(n->background);
        n->appendChildNode(n->grid);

        for(qint32 i = 0; i < m_maxLine; i++) {
            n->line[i] = new LineNode(10, 0.5, colours[i]);
            n->appendChildNode(n->line[i]);
        //n->shadow = new LineNode(20, 0.2f, QColor::fromRgbF(0.2, 0.2, 0.2, 0.4));
        }


        //n->appendChildNode(n->shadow);
        //n->appendChildNode(n->line);
    }

    if (m_geometryChanged) {
        n->background->setRect(rect, _disBck_);
        n->grid->setGrid(_gridSize_);
        n->grid->setRect(rect, _disGrid_);
    }

    if (m_geometryChanged || m_samplesChanged) {
        for(qint32 i = 0; i < m_maxLine; i++) {
            n->line[i]->updateGeometry(rect, m_samples[i]);
        }
        //n->shadow->updateGeometry(rect, k_samples);

        // We don't need to calculate the geometry twice, so just steal it from the other one...
        //n->shadow->setGeometry(n->line->geometry());
        //k->line->setGeometry(k->line->geometry());
    }

    m_geometryChanged = false;
    m_samplesChanged = false;

    return n;
}
