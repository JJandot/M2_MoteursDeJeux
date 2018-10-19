/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "geometryengine.h"
#include <vector>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D color;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    //initCubeGeometry();
    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initPlaneGeometry(){
    GLushort index = 0;
    std::vector<float> grayLevel;

    QImage img(257, 257, QImage::Format_RGB32);
    QImageReader reader(":/heightmap-1.png");

    int nbVertices = 16;

    VertexData vertices[nbVertices * nbVertices];

    int r = img.size().height() / nbVertices;

    if (reader.read(&img)) {
        for(int i = 0; i < nbVertices; ++i){
            for(int j = 0; j < nbVertices; ++j){
                int gray = qGray(img.pixel(i * r, j * r));
                grayLevel.push_back((double)gray / 255);
            }
        }
    }


    double incr = 2.0 / nbVertices;
    int grayCpt = 0;
    for(int j = 0; j < nbVertices; ++j){
        for(int i = 0; i < nbVertices; ++i){
            vertices[index].position = QVector3D(i * incr, j * incr, grayLevel[grayCpt]);
            vertices[index].texCoord = QVector2D(i * (1.0 / (nbVertices-1)), j * (1.0 / (nbVertices-1)));
            vertices[index++].color = QVector3D(grayLevel[grayCpt++], 0, 0);
        }
    }

    std::vector<GLushort> vectIndices;

    //init
    index = 0;
    for(int i = 0; i < 2*nbVertices; ++i){
        if(i % 2 == 0)
            vectIndices.push_back(index + nbVertices);
        else
            vectIndices.push_back(index++);
    }
    vectIndices.push_back(index-1);

    //fill
    for(int j = 0; j < nbVertices - 2; ++j){
        vectIndices.push_back(index);
        for(int i = 0; i < 2*nbVertices; ++i){
            if(i % 2 == 0)
                vectIndices.push_back(index + nbVertices);
            else
                vectIndices.push_back(index++);
        }
        vectIndices.push_back(index-1);
    }
    vectIndices.pop_back();


    int indicesSize = vectIndices.size();
    GLushort indices[indicesSize];

    for(GLushort i = 0; i < vectIndices.size(); ++i)
        indices[i] = vectIndices[i];

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbVertices * nbVertices * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, indicesSize * sizeof(GLushort));
}

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program){
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    //COLOR
    int colorLocation = program->attributeLocation("a_color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 1348, GL_UNSIGNED_SHORT, 0);
}
