#include <stdlib.h>
#include <iostream>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "Scene.h"


static const int width = 800;
static const int height = 600;
static const char* title = "Tennis Racket Theorem";
static const glm::vec4 background(0.2f, 0.2f, 0.2f, 1.0f);
static Scene scene;
static bool enableAnimation;
static bool show_ellipsoid;

// constants for Buss's algorithm
static glm::vec3 w; 
static glm::mat3 M_model;
static glm::mat3 M_world;
static glm::vec3 L;

void printHelp(){
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'O' to save a screenshot.
      press the arrow keys to rotate camera.
      press 'A'/'Z' to zoom.
      press 'R' to reset camera
      press '['/']' to increase/decrease angular velocity
      press Space to start/stop the animation
      press '0' to toggle ellipsoids on and off
)";
}

void initialize(void){
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0,0,width,height);

    enableAnimation = false;
    show_ellipsoid = true;
    
    // Initialize scene
    scene.init();
    scene.shader->enablelighting = true;
    w = scene.w;
    M_model = scene.M_model;

    // compute initial values for Buss's algorithm constants
    glm::mat3 R_init(scene.node["world"]->childtransforms[0]);
    M_world = R_init * M_model * glm::transpose(R_init);
    L = M_world * w;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

// helper function to recompute Poinsot's ellipsoids when object changes
void recomputePoinsot() {
  float E_k = glm::dot(w, (M_model * w));
  glm::vec3 F_partial = M_model * M_model * w;
  float F = dot(w, F_partial);

  scene.node["EPoin"] -> modeltransforms[0] = scale(glm::vec3(glm::sqrt(E_k / M_model[0][0]), glm::sqrt(E_k / M_model[1][1]), glm::sqrt(E_k / M_model[2][2])));
  scene.node["FPoin"] -> modeltransforms[0] = scale(glm::vec3(glm::sqrt(F) / M_model[0][0], glm::sqrt(F) / M_model[1][1], glm::sqrt(F) / M_model[2][2]));
  if(!show_ellipsoid) {
    scene.node["FPoin"]->modeltransforms[0] *= scale(glm::vec3(0.001f));  
    scene.node["EPoin"]->modeltransforms[0] *= scale(glm::vec3(0.001f));  
  }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    scene.draw();
    
    glutSwapBuffers();
    glFlush();
    
}

void saveScreenShot(const char* filename = "test.png"){
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth,currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27: // Escape to quit
            exit(0);
            break;
        case 'h': // print help
            printHelp();
            break;
        case 'o': // save screenshot
            saveScreenShot();
            break;
        case 'r':
            scene.camera -> aspect_default = float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT));
            scene.camera -> reset();
            glutPostRedisplay();
            break;
        case 'a':
            scene.camera -> zoom(0.9f);
            glutPostRedisplay();
            break;
        case 'z':
            scene.camera -> zoom(1.1f);
            glutPostRedisplay();
            break;
        case ' ':
            enableAnimation = !enableAnimation;
            glutPostRedisplay();
            break;
        case '[':
            w *= 0.95f;
            L = M_world * w;
            std::cout << "|w| = " << glm::length(w) << std::endl;
            glutPostRedisplay();
            break;
        case ']':
            w *= 1.05f;
            L = M_world * w;
            std::cout << "|w| = " << glm::length(w) << std::endl;
            glutPostRedisplay();
            break;
        case 'l':
            scene.shader -> enablelighting = !(scene.shader -> enablelighting);
            glutPostRedisplay();
            break;
        case '7':
            scene.node["spinny"]->models[0] = scene.model["teapot1"];
            w = scene.w;
            M_model = scene.M_model;
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        case '8':
            scene.node["spinny"]->models[0] = scene.model["bunny"];
            w = scene.w;
            M_model = scene.model["bunny"]->M_model; 
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        case '9':
            scene.node["spinny"]->models[0] = scene.model["table piece"];
            w = scene.w;
            M_model = scene.model["table piece"]->M_model; 
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        case '0':
            show_ellipsoid = !show_ellipsoid;
            if(show_ellipsoid) {
              scene.node["FPoin"]->modeltransforms[0] *= scale(glm::vec3(1000.0f));  
              scene.node["EPoin"]->modeltransforms[0] *= scale(glm::vec3(1000.0f));  
              scene.node["polhode"]->modeltransforms[0] *= scale(glm::vec3(1000.0f));  
            } else {
              scene.node["FPoin"]->modeltransforms[0] *= scale(glm::vec3(0.001f));  
              scene.node["EPoin"]->modeltransforms[0] *= scale(glm::vec3(0.001f));  
              scene.node["polhode"]->modeltransforms[0] *= scale(glm::vec3(0.001f));  
            }
            glutPostRedisplay();
            break;
        case '1':
            w = glm::vec3(0.1f, 0.0f, 2.0f);
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        case '2':
            w = glm::vec3(2.0f, 0.0f, 0.1f);
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        case '3':
            w = glm::vec3(0.1f, 2.0f, 0.0f);
            M_world = M_model;
            L = M_world * w;
            recomputePoinsot();
            scene.node["world"]->childtransforms[0] = glm::mat4(1.0f);
            scene.node["world"]->childtransforms[1] = translate(w);
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
            break;
    }
}
void specialKey(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP: // up
            scene.camera -> rotateUp(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // down
            scene.camera -> rotateUp(10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            scene.camera -> rotateRight(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // left
            scene.camera -> rotateRight(10.0f);
            glutPostRedisplay();
            break;
    }
}

// Buss's method to better conserve angular momentum
glm::mat4 buss(glm::mat4 R, float dt, glm::mat3 M_model) {
  w = glm::inverse(M_world) * L;
  glm::vec3 alpha = -1.0f * glm::inverse(M_world) * glm::cross(w, L);
  glm::vec3 wbar = w + (dt / 2) * alpha + dt * dt * glm::cross(alpha, w) / 12.0f;

  R = rotate(dt * glm::length(wbar), glm::normalize(wbar)) * R;
  glm::mat3 R_3(R);
  M_world = R_3 * M_model * glm::transpose(R_3);

  // update angular velocity to check if it stays on polhode
  scene.node["world"]->childtransforms[1] = translate(glm::inverse(R_3) * w);

  return R;
}

// animation function to run once a frame
void animation(void) {
  static int prev_time = glutGet(GLUT_ELAPSED_TIME);
  int curr_time = glutGet(GLUT_ELAPSED_TIME);
  
  float dt = ((float) curr_time - prev_time) / 1000.0f;
  prev_time = curr_time; // update previous time

  // pause animation if needed
  if(!enableAnimation) {
    return;
  }

  // Buss's algorithm
  glm::mat4 R = scene.node["world"]->childtransforms[0];
  scene.node["world"]->childtransforms[0] = buss(R, dt, M_model); 
  glutPostRedisplay();
}


int main(int argc, char** argv)
{
    // BEGIN CREATE WINDOW
    glutInit(&argc, argv);
    
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#else
    glutInitContextVersion(3,1);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
#endif
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit() ;
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
#endif
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // END CREATE WINDOW
    
    initialize();
    glutDisplayFunc(display);
    glutIdleFunc(animation);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    
    glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}
