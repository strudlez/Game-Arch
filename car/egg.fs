#version 150
//The fragment shader needs to compute the attenuation.
//It also needs to normalize the interpolated light direction, since the direction is potentially different for every vertex
in vec2 v_tex;
in float d;
in vec3 HV;
in vec3 VP;
in vec3 tnorm;
in vec3 lightPos;
in vec3 ecPos;

uniform sampler2D s_tex;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec4 FragColor;

const vec3 ambient = vec3(.1, 0.1, 0.1);
const vec3 diffuse = vec3(0.7, 0.7, 0.9);
const vec3 specular = vec3(0.7, 0.7, 0.7);
const float constantAttenuation = 1.0;
const float linearAttenuation = 0.0;
const float quadraticAttenuation = 0.0;
const float shininess = 50; //can be as high as 128 - bigger number = more concentrated bright spot
const float spotCutoff = 0.984807753012208; //cos(radians(10)); //cosine of angle for spot cone
const vec3 LightDirection = vec3(0.0, 0.0, -1.0);
const vec3 LightForward = vec3(1.0, 0.0, 0.0);
const vec3 LightSide = vec3(0.0, 1.0, 0.0);
const float spotExponent = 1.0;

const float edgeWidth = 0.3;

const float round = 4;

void main(void)
{
    float pf=0.0; //power factor
    float attenuation=0.0;   
    float spotAttenuation = 0.0; //final spotlight attenuation factor
    float nDotVP = 0.0;
    float nDotHV = 0.0;
    //normals may have been denormalized by linear interpolation, so we'll normalize "again"
    vec3 n = normalize(tnorm);
    vec3 color = ambient;
    nDotVP = max(dot(n,normalize(VP)),0.0);
    if (nDotVP > 0)
    {
        float spotDot = dot(-VP, normalize(LightDirection));
        float twoD = 2/round;
        float xDot = dot(-VP, normalize(LightSide));
        float yDot = dot(-VP, normalize(LightForward));
        float dist = pow(abs(xDot)/spotCutoff, twoD) + pow(abs(yDot)/spotCutoff, twoD);
        float innerEdge = pow(spotCutoff/10, twoD);
        float outerEdge = pow((spotCutoff + edgeWidth)/10, twoD);
        //if (dist < outerEdge)
        {
            if(dist < innerEdge) {
              spotAttenuation = pow(spotDot, spotExponent);
            } else {
              spotAttenuation = pow((outerEdge - dist)/(outerEdge - innerEdge), spotExponent);
            }

            //compute attenuation
            attenuation = spotAttenuation / (constantAttenuation +
                                linearAttenuation * d +
                                quadraticAttenuation * d * d);
                                
            color += attenuation * (diffuse * nDotVP);
            
            nDotHV = max(0.0, dot(n, HV));
    
            color += attenuation * specular * pow(nDotHV, shininess);
        }
    }
	FragColor = texture(s_tex, v_tex) * vec4(color, 1);
}
