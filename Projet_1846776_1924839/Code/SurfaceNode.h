#pragma once
// The code here is initially from:
// https://bitbucket.org/victorbush/ufl.cap5705.terrain/src/master/
void createTree(float x, float y, float z, float width, float height, glm::vec3 cam_position);
void renderSea(CNuanceurProg& progNuanceurGazon, glm::vec3 cam_position);
void surfaceInit();
void surfaceShutdown();