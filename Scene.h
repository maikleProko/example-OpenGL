#ifndef SCENE_H
#define SCENE_H

#include <GL/glu.h>
#include <QGLWidget>
#include <QGLShaderProgram>
#include "Surface.h"
#include "Ring.h"
#include <QKeyEvent>

class Scene : public QGLWidget
{
    Q_OBJECT
public:
    int partitionsNumber = 80;
    float ucRadius = 0.35f;
    float lcRadius = 0.25f;
    float height = 0.9f;
    int camXPos = 3;
    int camYPos = 10;
    int camZPos = 5;
    int wd, hg;
    float xPos = 0.0f;
    float yPos = 0.0f;
    float zPos = 0.0f;
    float xScale = 0.0f;
    float yScale = 0.0f;
    float zScale = 0.0f;
    bool isLightOn = true;
    int intensityAttr;
    float intensity = 5;

    Scene( QWidget *parent = 0 );
    int vertex;
    ~Scene();

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );
    void initLight();

    void drawAxes();

    Surface *m_surface;
    QGLShaderProgram m_program;

    int xAngle;  /* azimuth of view angle */
    int yAngle;  /* elevation of view angle */
    int zAngle;


    // for shader
    //QOpenGLShaderProgram *clockShaderProgram, *floorShaderProgram, *lightShaderProgram, *depthShaderProgram;
    //Clock *clock;
    //Floor *floor;
    //Light *light;
    QImage textureImage;
    //QOpenGLFramebufferObject *depthBuffer;
    GLuint depthBufferHeight, depthBufferWidth;
    QVector2D aspectRatio;
    // for events
    QPoint mousePosition;
    float rotation[2];
    float viewAngle;
    // for settings
    bool isOrtho = true;
    QString materialName;
    //LightType lightType;


    float figureXPos;
    float figureYPos;
    float figureZPos;


private slots:
    void camXPosChanged(int newX);
    void camYPosChanged(int newY);
    void camZPosChanged(int newZ);
    void xPosChanged(int newX);
    void yPosChanged(int newY);
    void zPosChanged(int newZ);
    void xScaleChanged(int newX);
    void yScaleChanged(int newY);
    void zScaleChanged(int newZ);
    void partitionsNumberChanged(int newPartitionNumber);
    void lightOff(bool light);
    void intensityValueChanged(int newIntensity);
    void xAngleChanged(int newXAngle);
    void yAngleChanged(int newYAngle);
    void zAngleChanged(int newZAngle);
};

#endif // SCENE_H
