#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


GLfloat cameraX = 15.0f;
GLfloat cameraY = 10.0f;
GLfloat cameraZ = 15.0f;
GLfloat targetY = 2.0f;


struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
    
    
    aiColor4D diffuseColor;
    aiColor4D specularColor;
    aiColor4D ambientColor;
    float shininess;
};

std::vector<Mesh> meshes;


GLfloat rotationAngle = 0.0;
GLfloat zoomFactor = 1.0;
bool wireframeMode = false;


float g_aspect = 1.0f;


void loadGLTF(const std::string& filePath) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filePath, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_ImproveCacheLocality);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
        return;
    }

    
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = scene->mMeshes[i];

     
        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);
            
           
            if (aiMesh->HasNormals()) {
                mesh.normals.push_back(aiMesh->mNormals[j].x);
                mesh.normals.push_back(aiMesh->mNormals[j].y);
                mesh.normals.push_back(aiMesh->mNormals[j].z);
            }
            
          
            if (aiMesh->mTextureCoords[0]) {
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].x);
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].y);
            }
        }

        for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j) {
            const aiFace& face = aiMesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                mesh.indices.push_back(face.mIndices[k]);
            }
        }

        
        if (scene->mNumMaterials > 0) {
            const aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
            
           
            if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &mesh.diffuseColor)) {
                mesh.diffuseColor = aiColor4D(0.8f, 0.8f, 0.8f, 1.0f);
            }
            
           
            if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &mesh.specularColor)) {
                mesh.specularColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
            }
            
           
            if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &mesh.ambientColor)) {
                mesh.ambientColor = aiColor4D(0.2f, 0.2f, 0.2f, 1.0f);
            }
            
            float shininess = 0.0f;
            unsigned int max;
            if (AI_SUCCESS != aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max)) {
                mesh.shininess = 0.0f;
            } else {
                mesh.shininess = shininess;
            }
        }

        meshes.push_back(mesh);
    }
}


void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
    
    
    GLfloat lightPos0[] = {20.0f, 30.0f, 20.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    GLfloat ambientLight0[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    
    GLfloat diffuseLight0[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    
    GLfloat specularLight0[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);

    // Configurar luz secundaria (ambiente/relleno)
    GLfloat lightPos1[] = {-10.0f, 10.0f, -10.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    
    GLfloat diffuseLight1[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    
    GLfloat specularLight1[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);

    // Color de fondo (cielo)
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    // Cargar modelo GLTF
    loadGLTF("Resources/Models/House/House.gltf"); 
}


// Función de renderizado mejorada
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, g_aspect, 0.1f, 200.0f);

  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX * zoomFactor, cameraY * zoomFactor, cameraZ * zoomFactor,
              0.0, targetY, 0.0,
              0.0, 1.0, 0.0);

 
    glPushMatrix();
    glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    glScalef(5.5f, 0.6f, 5.0f);


    for (const auto& mesh : meshes) {
        
        aiColor4D vividDiffuse = mesh.diffuseColor;
        aiColor4D vividSpecular = mesh.specularColor;
        aiColor4D vividAmbient = mesh.ambientColor;
        float factor = 1.5f;
        vividDiffuse.r = std::min(vividDiffuse.r * factor, 1.0f);
        vividDiffuse.g = std::min(vividDiffuse.g * factor, 1.0f);
        vividDiffuse.b = std::min(vividDiffuse.b * factor, 1.0f);
        vividSpecular.r = std::min(vividSpecular.r * factor, 1.0f);
        vividSpecular.g = std::min(vividSpecular.g * factor, 1.0f);
        vividSpecular.b = std::min(vividSpecular.b * factor, 1.0f);
        vividAmbient.r = std::min(vividAmbient.r * factor, 1.0f);
        vividAmbient.g = std::min(vividAmbient.g * factor, 1.0f);
        vividAmbient.b = std::min(vividAmbient.b * factor, 1.0f);

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&vividDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&vividSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&vividAmbient);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mesh.shininess);

        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_LIGHTING);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_LIGHTING);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
        
        if (!mesh.normals.empty()) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, mesh.normals.data());
        }
        
        if (!mesh.texCoords.empty()) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.texCoords.data());
        }

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        
        if (wireframeMode) {
            glEnable(GL_LIGHTING);
        }
    }
    glPopMatrix();

    glutSwapBuffers();
}


void mouseMotion(int x, int y) {
    static int prevX = x;
    static int prevY = y;
    
    rotationAngle += (x - prevX) * 0.5f;
    targetY -= (y - prevY) * 0.01f;
    
    
    if (targetY > 5.0f) targetY = 5.0f;
    if (targetY < -2.0f) targetY = -2.0f;
    
    prevX = x;
    prevY = y;
    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        glutMotionFunc(mouseMotion);
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        glutMotionFunc(nullptr);
    } else if (button == 3) { 
        zoomFactor *= 0.9f;
        glutPostRedisplay();
    } else if (button == 4) { 
        zoomFactor *= 1.1f;
        glutPostRedisplay();
    }
}

// Función para manejo de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':
            zoomFactor *= 0.9f;
            break;
        case 's': case 'S':
            zoomFactor *= 1.1f;
            break;
        case 'a': case 'A':
            rotationAngle -= 5.0f; // Rota a la izquierda
            break;
        case 'd': case 'D':
            rotationAngle += 5.0f; // Rota a la derecha
            break;
        case 'r': case 'R':
            rotationAngle = 0.0f;
            targetY = 2.0f;
            zoomFactor = 1.0f;
            break;
    }
    glutPostRedisplay();
}



void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    g_aspect = (float)width / (float)height; 
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Casa 3D - Visualización con Materiales Originales");

    init();

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(nullptr);

    glutMainLoop();

    return 0;
}
