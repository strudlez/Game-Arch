#version 150
//The fragment shader needs to compute the attenuation.
//It also needs to normalize the interpolated light direction, since the direction is potentially different for every vertex
in vec2 v_tex;
in float d;
in vec3 HV;
in vec3 VP;
in vec3 tnorm;

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
const float spotExponent = 1.0;

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
        //see if point on surface is inside cone of illumination
        float spotDot = dot(-VP, normalize(LightDirection));
        if (spotDot > spotCutoff)
        {
            spotAttenuation = pow(spotDot, spotExponent);
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
