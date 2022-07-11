#include "Circle.h"
#include <QtMath>

//нарисовать вершины
Circle::Circle(QGLShaderProgram *shader, int partitionsNumber, float lcRadius,
                 float ucRadius, float height, int vertex,
                 float xPosOffset, float yPosOffset, float zPosOffset,
                 float xScale, float yScale, float zScale) :
    Surface(),
     shader( shader )
{
    this->lcRadius = lcRadius;
    this->ucRadius = ucRadius;
    this->pNumber = partitionsNumber;
    this->height = height;
    this->circlesAround = (this->pNumber/10) - 1;
    if (this->circlesAround <= 0) this->circlesAround = 1;
    this->xPosOffset = xPosOffset;
    this->yPosOffset = yPosOffset;
    this->zPosOffset = zPosOffset;
    this->xScale = xScale;
    this->yScale = yScale;
    this->zScale = zScale;
    //draw();
}

// рассчет и отрисовка линий линзы
void Circle::calculateLinse()
{
    int sector = 1;
    QVector<QVector3D *> circles;
    QVector<QVector3D> points;

    for(int i = 0; i < pNumber/4; i++) {

        float theta = 2.0f * 3.1415926f * float(i) / float(pNumber); // получить текущий угол

        float x = this->xPosOffset; // рассчитываем x компоненту
        float z = this->ucRadius * sinf(theta) + this->zPosOffset; // рассчитываем y компоненту

        float rad = sqrt((z-this->zPosOffset)*(z-this->zPosOffset));

        circles.push_back(new QVector3D [pNumber]);


        // рисуем горизонтальные круги линзы
        for(int j = 0; j < pNumber * sector; j++)
        {

            float alpha = 2.0f * 3.1415926f * float(j) / float(pNumber); // получить текущий угол

            float y1 = rad * cosf(alpha)+this->yPosOffset; // рассчитываем x компоненту
            float z1 = rad * sinf(alpha)+this->zPosOffset; // рассчитываем y компоненту

            circles[i][j].setX(x);
            circles[i][j].setY(y1);
            circles[i][j].setZ(z1);
        }
    }

    // рисуем вертикальные линии линзы
    for(int i = 0; i < pNumber / 4; i++)
    {
        for (int j = 0; j < pNumber; j++)
        {
            points.append(circles[i][j]);
            if (i + 1 < pNumber / 4)
                points.append(circles[i + 1][j]);
            if (j + 1 < pNumber)
                points.append(circles[i][j + 1]);
            if((j + 1 < pNumber)&&(i + 1 < pNumber / 4))
                points.append(circles[i + 1][j + 1]);

        }
    }

    shader->setAttributeArray("aPosition", toArray(points), 3);
    shader->setAttributeArray("aNormal", toArray(points), 3);
    shader->enableAttributeArray("aPosition");
    shader->enableAttributeArray("aNormal");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, points.size());

    shader->disableAttributeArray("aPosition");
    shader->disableAttributeArray("aNormal");
}

void Circle::draw()
{
    calculateLinse();
}
