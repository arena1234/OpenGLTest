#include "gl/gl_shape.h"

Shape::Shape() {
    textureSize = POINT_X * (POINT_Y - 1) * 2 * POINT_SIZE_TEXTURE * sizeof(GLfloat);
    textureBuffer = (GLfloat *) malloc(textureSize);
    vertexSize = POINT_X * (POINT_Y - 1) * 2 * POINT_SIZE_VERTEX * sizeof(GLfloat);
    vertexBufferEnd = (GLfloat *) malloc(vertexSize);
    vertexBufferStart = (GLfloat *) malloc(vertexSize);
}

Shape::~Shape() {
    free(textureBuffer);
    free(vertexBufferEnd);
    free(vertexBufferStart);
}

void Shape::cylinder() {
    GLfloat w = 2;
    GLfloat h = w*9/16;
    GLfloat r = w / (2 * M_PI);
    GLfloat x_step_vertex = w / (POINT_X - 1);
    GLfloat y_step_vertex = h / (POINT_Y - 1);
    GLfloat x_step_vertex_degree = 360.0f / (POINT_X - 1);
    GLfloat x_step_texture = 1.0f / (POINT_X - 1);
    GLfloat y_step_texture = 1.0f / (POINT_Y - 1);

    GLuint64 time = getCurrentTime();
    for (int i = 0; i < POINT_X; i++) {
        for (int j = 0; j < POINT_Y - 1; j++) {
            int countVertex = POINT_X * POINT_SIZE_VERTEX * j * 2 + i * POINT_SIZE_VERTEX * 2;
            vertexBufferStart[countVertex + 0] = r * sinf(TO_RADIANS(i * x_step_vertex_degree));
            vertexBufferStart[countVertex + 1] = -h/2 + j * y_step_vertex;
            vertexBufferStart[countVertex + 2] = r * cosf(TO_RADIANS(i * x_step_vertex_degree));

            vertexBufferStart[countVertex + 3] = r * sinf(TO_RADIANS(i * x_step_vertex_degree));
            vertexBufferStart[countVertex + 4] = -h/2 + (j + 1) * y_step_vertex;
            vertexBufferStart[countVertex + 5] = r * cosf(TO_RADIANS(i * x_step_vertex_degree));

            vertexBufferEnd[countVertex + 0] = w/2 - i * x_step_vertex;
            vertexBufferEnd[countVertex + 1] = -h/2 + j * y_step_vertex;
            vertexBufferEnd[countVertex + 2] = 0;

            vertexBufferEnd[countVertex + 3] = w/2 - i * x_step_vertex;
            vertexBufferEnd[countVertex + 4] = -h/2 + (j + 1) * y_step_vertex;;
            vertexBufferEnd[countVertex + 5] = 0;


            int countTexture = POINT_X * POINT_SIZE_TEXTURE * j * 2 + i * POINT_SIZE_TEXTURE * 2;
            textureBuffer[countTexture + 0] = i * x_step_texture;
            textureBuffer[countTexture + 1] = j * y_step_texture;

            textureBuffer[countTexture + 2] = i * x_step_texture;
            textureBuffer[countTexture + 3] = (j + 1) * y_step_texture;
        }
    }
    LOGD("[Shape:cylinder]use time %d us", (GLuint) (getCurrentTime() - time));
}

GLuint64 Shape::getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}