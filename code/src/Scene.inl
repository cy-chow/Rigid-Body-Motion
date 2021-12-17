/**************************************************
Scene.inl contains the definition of the scene graph
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"


using namespace glm;

/*
 * Defines and returns a M_model matrix for calculation
 */
mat3 create_M(float u_x, float u_y, float u_z) {
  mat3 M_model;
  M_model[0][0] = u_x;
  M_model[1][1] = u_y;
  M_model[2][2] = u_z;

  return M_model;
}

void Scene::init(void){
    M_model = create_M(2.0f, 1.0, 1.5f);  // M_model of the teapot (others defined below)
    w = vec3(0.1f, 0.0f, 2.0f);           // initial angular velocity (world coordinates) 
    mat4 R(1.0f);                         // initial rotation matrix
    vec3 Omega(w);                        // initial angular velocity (model coordinates)

    // Create a geometry palette
    geometry["cube"] = new Cube;
    geometry["teapot"] = new Obj;
    geometry["bunny"] = new Obj;
    geometry["sphere"] = new Obj;
    geometry["cube"] -> init();
    geometry["teapot"] -> init("models/teapot.obj");
    geometry["bunny"] -> init("models/bunny.obj");
    geometry["sphere"] -> init("models/sphere.obj");
    
    // Create a material palette
    material["wood"] = new Material;
    material["wood"] -> ambient = vec4(0.1f,0.1f,0.1f,0.1f);
    material["wood"] -> diffuse = vec4(0.1f,0.1f,0.1f,0.1f);
    material["wood"] -> specular = vec4(0.1f,0.1f,0.1f,0.1f);
    material["wood"] -> shininess = 5.0f;
    
    material["ceramic"] = new Material;
    material["ceramic"] -> ambient = vec4(0.02f, 0.07f, 0.2f, 1.0f);
    material["ceramic"] -> diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
    material["ceramic"] -> specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["ceramic"] -> shininess = 150.0f;
 
    material["silver"] = new Material;
    material["silver"] -> ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["silver"] -> diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    material["silver"] -> specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["silver"] -> shininess = 50.0f;
    
    material["turquoise"] = new Material;
    material["turquoise"] -> ambient = vec4(0.1f, 0.2f, 0.17f, 1.0f);
    material["turquoise"] -> diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["turquoise"] -> specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["turquoise"] -> shininess = 100.0f;
    
    material["bulb"] = new Material;
    material["bulb"] -> ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material["bulb"] -> emision = vec4(1.0f,0.2f,0.1f,1.0f);
    material["bulb"] -> shininess = 200.0f;

    material["clear"] = new Material;
    material["clear"] -> ambient = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    material["clear"] -> diffuse = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    material["clear"] -> specular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    

    model["teapot1"] = new Model;
    model["teapot1"] -> geometry = geometry["teapot"];
    model["teapot1"] -> material = material["silver"];
    model["teapot1"] -> M_model = M_model;
    model["sphere"] = new Model;
    model["sphere"] -> geometry = geometry["sphere"];
    model["sphere"] -> material = material["turquoise"];
    model["sphere"] -> M_model = create_M(1.0f, 1.0f, 1.0f);
    model["sphere2"] = new Model;
    model["sphere2"] -> geometry = geometry["sphere"];
    model["sphere2"] -> material = material["ceramic"];
    model["sphere2"] -> M_model = create_M(1.0f, 1.0f, 1.0f);
    model["sphere3"] = new Model;
    model["sphere3"] -> geometry = geometry["sphere"];
    model["sphere3"] -> material = material["clear"];
    model["sphere3"] -> M_model = create_M(1.0f, 1.0f, 1.0f);

    model["table piece"] = new Model;
    model["table piece"] -> geometry = geometry["cube"];
    model["table piece"] -> material = material["wood"];
    model["table piece"] -> M_model = create_M(1.0f, 1.0f, 1.0f);
    model["bunny"] = new Model;
    model["bunny"] -> geometry = geometry["bunny"];
    model["bunny"] -> material = material["turquoise"];
    model["bunny"] -> M_model = create_M(4.0f, 1.0f, 2.0f); 
    model["bulb"] = new Model;
    model["bulb"] -> geometry = geometry["cube"];
    model["bulb"] -> material = material["bulb"];
    model["floor"] = new Model;
    model["floor"] -> geometry = geometry["cube"];
    model["floor"] -> material = material["turquoise"];
    
    // Create a light palette
    light["sun"] = new Light;
    light["sun"] -> position = vec4(3.0f,2.0f,1.0f,0.0f);
    light["sun"] -> color = 1.0f*vec4(1.0f,1.0f,1.0f,1.0f);
    
    light["bulb"] = new Light;
    light["bulb"] -> position = vec4(0.0f,2.0f,0.0f,0.0f);
    light["bulb"] -> color = 1.5f * vec4(1.0f,0.8f,0.5f,1.0f);
    
    // Build the scene graph
    node["spinny"] = new Node;
    node["EPoin"] = new Node;
    node["FPoin"] = new Node;
    node["polhode"] = new Node;
    
    node["EPoin"] -> models.push_back( model["sphere"] );
    float E_k = dot(Omega, (M_model * Omega));
    node["EPoin"] -> modeltransforms.push_back( scale(vec3(sqrt(E_k / M_model[0][0]), sqrt(E_k / M_model[1][1]), sqrt(E_k / M_model[2][2]))) );

    vec3 F_partial = M_model * M_model * Omega;
    float F = dot(Omega, F_partial);

    node["FPoin"] -> models.push_back( model["sphere2"] );
    node["FPoin"] -> modeltransforms.push_back( scale(vec3(sqrt(F) / M_model[0][0], sqrt(F) / M_model[1][1], sqrt(F) / M_model[2][2])) );

    node["polhode"] -> models.push_back( model["sphere3"] );
    node["polhode"] -> modeltransforms.push_back( scale(vec3(0.125f)) );
    
    node["spinny"] -> models.push_back( model["teapot1"] );
    node["spinny"] -> modeltransforms.push_back( scale(vec3(2.0f)) );

    node["world"] -> childnodes.push_back( node["spinny"] );
    node["world"] -> childtransforms.push_back(R);
    node["world"] -> childnodes.push_back( node["polhode"] );
    node["world"] -> childtransforms.push_back(translate(Omega));
    node["world"] -> childnodes.push_back( node["EPoin"] );
    node["world"] -> childtransforms.push_back(mat4(1.0f));
    node["world"] -> childnodes.push_back( node["FPoin"] );
    node["world"] -> childtransforms.push_back(mat4(1.0f));
    
    // Put a camera
    camera = new Camera;
    camera -> target_default = vec3( 0.0f, 0.0f, 0.0f );
    camera -> eye_default = vec3( 0.0f, 0.0f, 25.0f );
    camera -> up_default = vec3( 0.0f, 1.0f, 0.0f );
    camera -> reset();
    
    // Initialize shader
    shader = new SurfaceShader;
    shader -> read_source( "shaders/projective.vert", "shaders/lighting.frag" );
    shader -> compile();
    glUseProgram(shader -> program);
    shader -> initUniforms();
}
