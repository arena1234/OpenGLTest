#ifndef GL_PIC_LIGHT_H
#define GL_PIC_LIGHT_H

#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gl/gl_base.h"
#include "bean/bean_base.h"
#include "log.h"


const char gCubeVertexShader[] =
        "#version 300 es                                                    \n"
        "layout (location = "STRV(SHADER_IN_POSITION)") in vec3 position;   \n"
        "layout (location = "STRV(SHADER_IN_TEX_COORDS)") in vec2 texCoord; \n"
        "layout (location = "STRV(SHADER_IN_NORMAL_VERTOR)") in vec3 normal;\n"
        "out vec2 TexCoord;                                                 \n"
        "out vec3 FragPos;                                                  \n"
        "out vec3 Normal;                                                   \n"
        "uniform mat4 projection;                                           \n"
        "uniform mat4 camera;                                               \n"
        "uniform mat4 transform;                                            \n"
        "void main() {                                                      \n"
        "  gl_Position = projection*camera*transform*vec4(position, 1.0);   \n"
        "  TexCoord = vec2(texCoord.s, 1.0-texCoord.t);                     \n"
        "  FragPos = vec3(transform * vec4(position, 1.0));                 \n"
        "  Normal = mat3(transpose(inverse(transform))) * normal;           \n"
        "}\n";

const char gCubeFragmentShader[] =
        "#version 300 es                        \n"
        "precision mediump float;               \n"
        "in vec2 TexCoord;                      \n"
        "in vec3 FragPos;                       \n"
        "in vec3 Normal;                        \n"
        "uniform sampler2D tTexture;            \n"
        "uniform vec3 objectPos;                \n"
        "out vec4 color;                        \n"
        "struct Material {                      \n"
        "  vec3 ambient;                        \n"
        "  vec3 diffuse;                        \n"
        "  vec3 specular;                       \n"
        "  float shininess;                     \n"
        "};                                     \n"
        "struct Light {                         \n"
        "  vec3 ambient;                        \n"
        "  vec3 diffuse;                        \n"
        "  vec3 specular;                       \n"
        "  vec3 position;                       \n"
        "};                                     \n"
        "uniform Material material;             \n"
        "uniform Light light;                   \n"
        "void main() {                          \n"
        // Ambient
        "  vec3 ambient = light.ambient * material.ambient;\n"
        // Diffuse
        "  vec3 norm = normalize(Normal);                               \n"
        "  vec3 lightDir = normalize(light.position - FragPos);         \n"
        "  float diff = max(dot(norm, lightDir), 0.0);                  \n"
        "  vec3 diffuse = light.diffuse * (diff * material.diffuse);    \n"
        // Specular
        "  vec3 objectDir = normalize(objectPos - FragPos);                             \n"
        "  vec3 reflectDir = reflect(-objectDir, norm);                                 \n"
        "  float spec = pow(max(dot(objectDir, reflectDir), 0.0), material.shininess);  \n"
        "  vec3 specular =  light.specular * (spec * material.specular);                \n"
        // result
        "  vec3 result = (ambient + diffuse + specular);                \n"
        "  color = vec4(result, 1) * texture(tTexture, TexCoord);       \n"
        "}                                                              \n";

const char gLightVertexShader[] =
        "#version 300 es                                                    \n"
        "layout (location = "STRV(SHADER_IN_POSITION)") in vec3 position;   \n"
        "uniform mat4 projection;                                           \n"
        "uniform mat4 camera;                                               \n"
        "uniform mat4 transform;                                            \n"
        "void main() {                                                      \n"
        "  gl_Position = projection*camera*transform*vec4(position, 1.0);   \n"
        "}\n";

const char gLightFragmentShader[] =
        "#version 300 es                        \n"
        "precision mediump float;               \n"
        "out vec4 color;                        \n"
        "void main() {                          \n"
        "  color = vec4(1.0);                   \n"
        "}\n";

const Point3 A = {-0.5, 0.5, 0.5};
const Point3 B = {-0.5, -0.5, 0.5};
const Point3 C = {0.5, -0.5, 0.5};
const Point3 D = {0.5, 0.5, 0.5};
const Point3 E = {-0.5, 0.5, -0.5};
const Point3 F = {-0.5, -0.5, -0.5};
const Point3 G = {0.5, -0.5, -0.5};
const Point3 H = {0.5, 0.5, -0.5};
const GLfloat cubeVertex[][12] = {
        {// 左
                F.x, F.y, F.z,
                E.x, E.y, E.z,
                B.x, B.y, B.z,
                A.x, A.y, A.z,
        },
        {// 前
                B.x, B.y, B.z,
                A.x, A.y, A.z,
                C.x, C.y, C.z,
                D.x, D.y, D.z,
        },
        {// 上
                A.x, A.y, A.z,
                E.x, E.y, E.z,
                D.x, D.y, D.z,
                H.x, H.y, H.z,
        },
        {// 下
                F.x, F.y, F.z,
                B.x, B.y, B.z,
                G.x, G.y, G.z,
                C.x, C.y, C.z,
        },
        {//右
                C.x, C.y, C.z,
                D.x, D.y, D.z,
                G.x, G.y, G.z,
                H.x, H.y, H.z,
        },
        {// 后
                G.x, G.y, G.z,
                H.x, H.y, H.z,
                F.x, F.y, F.z,
                E.x, E.y, E.z,
        },
};
const GLfloat cubeTexCoords[][8] = {
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
};
const GLfloat normalVertex[][12] = {
        {// 左
                -1, 0, 0,
                -1, 0, 0,
                -1, 0, 0,
                -1, 0, 0,
        },
        {// 前
                0, 0, 1,
                0, 0, 1,
                0, 0, 1,
                0, 0, 1,
        },
        {// 上
                0, 1, 0,
                0, 1, 0,
                0, 1, 0,
                0, 1, 0,
        },
        {// 下
                0, -1, 0,
                0, -1, 0,
                0, -1, 0,
                0, -1, 0,
        },
        {//右
                1, 0, 0,
                1, 0, 0,
                1, 0, 0,
                1, 0, 0,
        },
        {// 后
                0, 0, -1,
                0, 0, -1,
                0, 0, -1,
                0, 0, -1,
        },
};

const GLfloat positions[][3] = {
        {
                0, 0, -2.4,
        },
        /*{
                3,  0, -8,
        },
        {
                -3,  0, -8,
        },
        {
                0,  3, -8,
        },
        {
                0,  -3, -8,
        },
        {
                -1, 1, -4,
        },
        {
                -1, -1, -4,
        },
        {
                1, 1, -4,
        },
        {
                1, -1, -4,
        },*/
};

const GLfloat lightPosition[3] = {8, 5, -12};

class PicLight : public GLBase {
public:
    PicLight(TransformBean *transformBean);

    ~PicLight();

protected:
    void updateFrame(Bitmap *bmp);

    void createTexture();

    void updateVertex();

    void updateTexCoord();

    GLuint loadShader();

    void prepareRenderer();

    void updateNormalVector();

private:
    GLboolean bFirstFrame;

    GLint mLightProjectionHandle;       // 投影矩阵的句柄
    GLint mLightCameraHandle;           // Camera矩阵的句柄
    GLint mLightTransformHandle;        // 变换矩阵的句柄
    GLuint mShaderLightHandle;          // 着色器句柄

    GLint mLightAmbient;                // 灯光颜色句柄
    GLint mLightDiffuse;                // 灯光颜色句柄
    GLint mLightSpecular;               // 灯光颜色句柄
    GLint mLightPosition;               // 灯光位置句柄
    GLint mObjectPosition;              // 物体位置句柄

    GLint mMaterialAmbient;             // 环境光
    GLint mMaterialDiffuse;             // 漫反射
    GLint mMaterialSpecular;            // 镜面反射
    GLint mMaterialShininess;           // 高光半径

    FloatBuffer *mNormalBuffer;     // 顶点坐标缓存

    void loadShaderForLight();
};

#endif //GL_PIC_LIGHT_H
