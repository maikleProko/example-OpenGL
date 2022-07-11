#ifndef RING_H
#define RING_H

#include <vector>
#include <QGLShaderProgram>
#include "Surface.h"

class Ring : public Surface
{
public:

    int pNumber = 0;
    float radius = 0;
    float height = 0;
    int circlesAround = 3;
    int soverCirclesNumber;
    int vertex;
    float xPosOffset;
    float yPosOffset;
    float zPosOffset;
    float xScale;
    float yScale;
    float zScale;
    float fineness;
    Ring( QGLShaderProgram *shader, int pNumber,
             float radius, float height, float fineness, int vertex,
             float xPosOffset, float yPosOffset, float zPosOffset,
             float xScale, float yScale, float zScale);

    void draw();

    void counting();

    void calculateLinse();

private:

    QVector <QVector3D> lowCircle;
    QVector <QVector3D> uppCircle;
    QVector <QVector3D> lowCircleTwo;
    QVector <QVector3D> upperCircleTwo;
    QVector <QVector3D> soverCircleOne;
    QVector <QVector3D> soverCircleTwo;

    QVector <QPointF> originPoints;
    QVector <QVector3D> vertices;


    QGLShaderProgram *shader;
};

#endif // RING_H
