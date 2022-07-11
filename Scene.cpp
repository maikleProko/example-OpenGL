#include <QMatrix4x4>
#include "Scene.h"
#include "Ring.h"
#include "Surface.h"
#include "Circle.h"
#include <math.h>
#include <qmath.h>

Scene::Scene( QWidget *parent ) :
    QGLWidget( parent )
{
    xAngle=0;
    yAngle=-90;
    zAngle=0;

    xPos=0.5;

    depthBufferHeight = depthBufferWidth = 1024;
    aspectRatio = {0.3, 0.3};

    rotation[0] = M_PI / 24;
    rotation[1] = 0;
    viewAngle = 3 * M_PI / 4;

    isOrtho = true;
    materialName = "Emerald";
    //lightType = DIRECTIONAL_LIGHT;

    setFocus();
}

Scene::~Scene()
{
    delete m_surface;
}

void Scene::initializeGL()
{
    // инициализируем vShader
    glClearColor( 0.1f, 0.1f, 0.2f, 1.0f );
    QGLShader vShader( QGLShader::Vertex );
    vShader.compileSourceFile( ":/Shaders/vShader.glsl" );

    // инициализируем fShader
    QGLShader fShader( QGLShader::Fragment );
    fShader.compileSourceFile( ":/Shaders/fShader.glsl" );

    m_program.addShader( &vShader );
    m_program.addShader( &fShader );
    if ( !m_program.link() )
    {
        qWarning( "Error: unable to link a shader program." );
        return;
    }

    // иницализируем поверхность линзы
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    glEnable(GL_DEPTH_TEST);
}

void Scene::initLight()
{
    //Устанавливаем параметры источника света
        static float ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        static float diffuse[] = {1.0, 1.0, 1.0, 1.0};
        static float specular[] = { 1.0, 1.0, 1.0, 1.0 };
        static float position[] = {100.0, 60.0, 150.0, 0.0};
        // Определяем свойства материала лицевой поверхности
        static float front_mat_shininess[] = {10.0};
        static float front_mat_specular[] = {0.5, 0.4, 0.4, 0.1};
        static float front_mat_diffuse[] = {0.0, 0.9, 0.3, 0.1};
        // Определяем свойства материала обратной поверхности
        static float back_mat_shininess[] = {3.2};
        static float back_mat_specular[] = {0.07568, 0.61424, 0.07568, 1.0};
        static float back_mat_diffuse[] = {0.633, 0.727811, 0.633, 1.0};
        static float lmodel_ambient[] = {1.0, 1.0, 0.0, 1.0};
        static float lmodel_twoside[] = {GL_TRUE};
        // Определяем цвет фона используемый по умолчанию
        glClearColor(1.0f, 0.96f, 0.866f, 1.0f);
        // Включаем различные тесты
        //glDepthFunc(GL_LEQUAL);
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_ONE, GL_SRC_COLOR);
        // Задаем источник света
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
        // Разрешаем освещение и включаем источник света
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        // Устанавливаем параметры материалов
        glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
        glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
        glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
        glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
}

void Scene::paintGL()
{
    const float radius = 1.75;
    QVector3D cameraPosition = QVector3D(
        radius * qSin(rotation[0]),
        (radius * qCos(rotation[0])) * qSin(viewAngle),
            (radius * qCos(rotation[0])) * qCos(viewAngle)
    );

    qDebug() << viewAngle;
    //qDebug() << "X:" << camXPos << " Y: " << camYPos << " Z: " <<camZPos;

    QMatrix4x4 modelMatrix, viewMatrix, projectionMatrix;
    modelMatrix.rotate(rotation[1], {0.0, 0.0, 1.0});
    viewMatrix.lookAt(cameraPosition, {figureXPos, figureZPos, figureYPos}, {1.0, 0.0, 0.0});
    projectionMatrix.setToIdentity();
    if (!isOrtho)
        projectionMatrix.ortho(
            -3.3 * aspectRatio.x(), 3.3 * aspectRatio.x(),
            -3.3 * aspectRatio.y(), 3.3 * aspectRatio.y(),
            -20, 20
        );
    else
        projectionMatrix.frustum(
            -0.75 * aspectRatio.x(), 0.75 * aspectRatio.x(),
            -0.75 * aspectRatio.y(), 0.75 * aspectRatio.y(),
            0.3, 20
        );
    QMatrix4x4 lightViewMatrix, lightProjectionMatrix;
    lightViewMatrix.lookAt({(float) (3.0 * qCos(M_PI / 4)), (float) (3.0 * qSin(M_PI / 4)), 0.0}, {figureXPos, figureZPos, figureYPos}, {1.0, 0.0, 0.0});
    lightProjectionMatrix.ortho(-10, 10, -10, 10, -10, 20);

    /*QMatrix4x4 lightViewMatrix, lightProjectionMatrix;
    lightViewMatrix.lookAt(light->position(), {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});
    lightProjectionMatrix.ortho(-10, 10, -10, 10, -10, 20);*/

    /* drawing to buffer */
   // depthBuffer->bind();

    glViewport(0, 0, depthBufferWidth, depthBufferHeight);
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //depthShaderProgram->bind();

    //depthShaderProgram->setUniformValue("uLightModel", modelMatrix);
    //depthShaderProgram->setUniformValue("uLightView", lightViewMatrix);
    //depthShaderProgram->setUniformValue("uLightProjection", lightProjectionMatrix);

    /*drawObject(depthShaderProgram);
    float *floorVertex = floor->model();
    drawArrays(depthShaderProgram, {"aPosition"}, {floorVertex}, GL_POLYGON, floor->modelSize());*/

    //depthShaderProgram->release();
    //depthBuffer->release();

    /*GLuint shadowTexture = depthBuffer->texture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);*/

    /* drawing to screen */
    glViewport(0, 0, 1024, 780);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* clock drawing */
    //clockShaderProgram->bind();
    m_program.bind();

    m_program.setUniformValue("uModel", modelMatrix);
    m_program.setUniformValue("uView", viewMatrix);
    m_program.setUniformValue("uProjection", projectionMatrix);

    m_program.setUniformValue("uLightView", lightViewMatrix);
    m_program.setUniformValue("uLightProjection", lightProjectionMatrix);

    QVector3D lightColor = QVector3D(1.0, 1.0, 1.0);
    //m_program.setUniformValue("uLightType", lightType);
    m_program.setUniformValue("uLightInnerCut", (float) qCos(qDegreesToRadians(12.5)));
    m_program.setUniformValue("uLightOuterCut", (float) qCos(qDegreesToRadians(17.5)));
    m_program.setUniformValue("uLightColor", lightColor);
    m_program.setUniformValue("uLightPosition", QVector3D({(float) (3.0 * qCos(M_PI / 4)), (float) (3.0 * qSin(M_PI / 4)), 0.0}));
    m_program.setUniformValue("uCameraPosition", cameraPosition);

    m_program.setUniformValue("uMaterial.ambient", QVector3D(0.2125, 0.1275, 0.054));
    m_program.setUniformValue("uMaterial.diffuse", QVector3D(0.714, 0.4284, 0.18144));
    m_program.setUniformValue("uMaterial.specular", QVector3D(0.393548, 0.271906, 0.166721));
    m_program.setUniformValue("uMaterial.shininess", 0.2f * 128);


    /*//initLight();
    if (!isLightOn) glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    else glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();

    // выставление интенсивности света в программе
    intensityAttr = m_program.attributeLocation("intensity");
    m_program.setAttributeValue(intensityAttr, intensity);*/


    // вращение фигуры
    /*glPushMatrix();
    glRotated(xAngle,1,0,0);
    glRotated(yAngle,0,1,0);
    glRotated(zAngle,0,0,1);
    glScalef(xScale + 1.0f,yScale + 1.0f,zScale + 1.0f);*/
    if ( !m_program.bind() )
        return;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);

    // иницализируем поверхность линзы
    Circle *roofUp = new Circle( &m_program, this->partitionsNumber - 5, this->lcRadius + 0.1,
                             this->ucRadius + 0.1, this->height, vertex,
                             xPos + 0.02, yPos, zPos,
                             -0.8, yScale + 0.5, zScale + 0.5);

    // иницализируем поверхность линзы
    Circle *roofDown = new Circle( &m_program, this->partitionsNumber - 5, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos - 0.05, yPos, zPos,
                             -0.8, yScale + 0.5, zScale + 0.5);


    // иницализируем поверхность линзы
    Circle *floorUp = new Circle( &m_program, this->partitionsNumber - 5, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos - 0.45, yPos, zPos,
                             -0.8, yScale + 0.5, zScale + 0.5);

    // иницализируем поверхность линзы
    Circle *floorDown = new Circle( &m_program, this->partitionsNumber - 5, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos - 0.5, yPos, zPos,
                             -0.8, yScale + 0.5, zScale + 0.5);




    // иницализируем поверхность линзы
    Ring *floor = new Ring( &m_program, this->partitionsNumber - 5,
                             this->ucRadius, 0.02, 6, vertex,
                             xPos - 0.5, yPos, zPos,
                             -0.8, yScale + 0.5, zScale + 0.5);

    // иницализируем поверхность линзы
    Ring *ring = new Ring( &m_program, this->partitionsNumber - 5,
                             this->ucRadius,
                             0.05, 10, vertex,
                             xPos - 0.05, yPos, zPos,
                             xScale, yScale, zScale);


    // отрисовка поверхности
    m_surface->draw();

    roofUp->draw();
    ring->draw();
    roofDown->draw();

    floorUp->draw();
    floor->draw();
    floorDown->draw();


    m_program.setUniformValue("uMaterial.ambient", QVector3D(0.19225, 0.19225, 0.19225));
    m_program.setUniformValue("uMaterial.diffuse", QVector3D(0.50754, 0.50754, 0.50754));
    m_program.setUniformValue("uMaterial.specular", QVector3D(0.508273, 0.508273, 0.508273));
    m_program.setUniformValue("uMaterial.shininess", 0.4f * 128);

    for (int i = 0; i < 6; i++)
    {
        float alpha = (60 * i * M_PI) / 180;
        float x = yPos + 0.25 * cos(alpha);
        float y = zPos + 0.25 * sin(alpha);

        Ring *column = new Ring( &m_program, this->partitionsNumber - 5,
                               0.05,
                               0.235, 10, vertex,
                               xPos - 0.45, yPos + x, zPos + y,
                               xScale, yScale, zScale);
        column->draw();

    }


    m_program.release();

    //glPopMatrix();

    // отрисовка осей: X, Y, Z
    //drawAxes();
    glDisable(GL_BLEND);
}

void Scene::resizeGL( int width, int height )
{
    glViewport(0,0,width,height);
    this->wd = width;
    this->hg = height;
    GLdouble gldAspect = (GLdouble) width/ (GLdouble) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(10.0, gldAspect, 1.0, 100.0);
    // установка позиции камеры
    gluLookAt(this->camXPos, this->camYPos, this->camZPos, 0, 0, 0, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

// отрисовка осей: X, Y, Z
void Scene::drawAxes()
{
    double len = 2.0;

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(len,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,len,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,len);
    glEnd();

    renderText(len,0,0,"X");
    renderText(0,len,0,"Y");
    renderText(0,0,len,"Z");

}

// слот отвечающий за изменения X положение камеры
void Scene::camXPosChanged(int newX)
{
    this->camXPos = newX;
    rotation[0] += 0.1;
    this->resizeGL(wd,hg);
    this->updateGL();
}

// слот отвечающий за изменения Y положение камеры
void Scene::camYPosChanged(int newY)
{
    this->camYPos = newY;
    rotation[1] += 0.1;
    this->resizeGL(wd,hg);
    this->updateGL();

}

// слот отвечающий за изменения Z положение камеры
void Scene::camZPosChanged(int newZ)
{
    this->camZPos = newZ;
    viewAngle += M_PI / 32;

    if (viewAngle > 2 * M_PI)
        viewAngle -= 2 * M_PI;
    if (viewAngle < 0)
        viewAngle += 2 * M_PI;

    this->resizeGL(wd,hg);
    this->updateGL();
}

// слот отвечающий за сдвиг фигуры по оси X
void Scene::xPosChanged(int newX)
{
    //this->xPos = (float)newX/10;
    figureXPos = (float)newX/10;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за сдвиг фигуры по оси Y
void Scene::yPosChanged(int newY)
{
    //this->yPos = (float)newY/10;
    figureYPos = (float)newY/10;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за сдвиг фигуры по оси Z
void Scene::zPosChanged(int newZ)
{
    figureZPos = (float)newZ/10;
    //this->zPos = (float)newZ/10;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за масштабирование фигуры по X координате
void Scene::xScaleChanged(int newX)
{
    this->xScale = (float)newX/10;
    qDebug() << this->xScale;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за масштабирование фигуры по Y координате
void Scene::yScaleChanged(int newY)
{
    this->yScale = (float)newY/10;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за масштабирование фигуры по Z координате
void Scene::zScaleChanged(int newZ)
{
    this->zScale = (float)newZ/10;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    updateGL();
}

// слот отвечающий за изменение мелкости разбиения фигуры
void Scene::partitionsNumberChanged(int newPartitionNumber)
{
    this->partitionsNumber = newPartitionNumber;
    delete m_surface;
    m_surface = new Surface( &m_program, this->partitionsNumber, this->lcRadius,
                             this->ucRadius, this->height, vertex,
                             xPos, yPos, zPos,
                             xScale, yScale, zScale);
    this->updateGL();
}

// слот отвечающий за включение/выключение света
void Scene::lightOff(bool light)
{
    isLightOn = !light;
    updateGL();
}

// слот отвечающий за изменение интенсивности света
void Scene::intensityValueChanged(int newIntensity)
{
    this->intensity = newIntensity;
    updateGL();
}

// слот отвечающий за поворот фигуры относительно оси X
void Scene::xAngleChanged(int newXAngle)
{
    this->xAngle = newXAngle;
    updateGL();
}

// слот отвечающий за поворот фигуры относительно оси Y
void Scene::yAngleChanged(int newYAngle)
{
    this->yAngle = newYAngle;
    updateGL();
}

// слот отвечающий за поворот фигуры относительно оси Z
void Scene::zAngleChanged(int newZAngle)
{
    this->zAngle = newZAngle;
    updateGL();
}

