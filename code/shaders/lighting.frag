#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {
        
        // HW3: You will compute the lighting here.
        fragColor = emision;
        vec4 color_sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);

        // normalized vector pointing at the viewer (camera)
        // always transform position first before resizing because 
        // position can be changed by affine transformation!
        vec3 v = -normalize(vec3(modelview * position));

        // normalized surface normal (recall the different transformation)
        mat3 mvit = inverse(transpose(mat3(modelview)));
        vec3 n = normalize(mvit * normal);

        for(int j = 0; j < nlights; j++) {
            // recall that light positions are in world space
            vec4 lp_eye = view * lightpositions[j];
            vec4 p_eye = modelview * position;

            vec3 l_j = normalize(p_eye.w * lp_eye.xyz - lp_eye.w * p_eye.xyz);
            vec3 h_j = normalize(v + l_j);

            vec4 c_diff = diffuse * max(dot(n, l_j), 0);
            vec4 c_spec = specular * pow(max(dot(n, h_j), 0), shininess);

            fragColor += (ambient + c_diff + c_spec) * lightcolors[j];
        }
    }
}
