#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <QGLShaderProgram>

class Surface
{
public:

    int pNumber = 0;
    float lcRadius = 0;
    float ucRadius = 0;
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
    Surface( QGLShaderProgram *shader, int pNumber,
             float lcRadius, float ucRadius, float height, int vertex,
             float xPosOffset, float yPosOffset, float zPosOffset,
             float xScale, float yScale, float zScale);
    Surface();

    void draw();

    void counting();

    void calculateLinse();

    void vectorToArray(float *array, QVector3D &vector, int index);

    QVector3D normalize(QVector3D point1, QVector3D point2, QVector3D point3);
    float *toArray(QVector<QVector3D> &vector);

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

#endif // SURFACE_H
