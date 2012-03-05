#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec3 in_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

const vec3 LightPosition = vec3(0.0, 0.0, 4.0); 
const float constantAttenuation = 1.0;
const float linearAttenuation = 0.0;
const float quadraticAttenuation = 0.0;
const float shininess = 50; //can be as high as 128 - bigger number = more concentrated bright spot
const float spotCutoff = 0.984807753012208; //cos(radians(10.0f)); //cosine of angle for spot cone
const vec3 LightDirection = vec3(0.0, 0.0, -1.0);
const float spotExponent = 20.0;

out vec2 v_tex;
out float nDotVP;
out float pf; //power factor
out float attenuation;

void main(void)
{
    float spotAttenuation; //final spotlight attenuation factor
    //eye coordinate position
    vec3 ecPos = (ViewMatrix * ModelMatrix * in_Position).xyz;
    vec3 eye = -normalize(ecPos);
    vec3 tnorm = normalize(NormalMatrix * in_Normal);
    
    //vector from surface to light position
    vec3 VP = vec3(LightPosition - ecPos);
    //distance from light to surface
    float d = length(VP);
    //normalize VP
    VP = normalize(VP);
    //compute attenuation
    attenuation = 1.0 / (constantAttenuation +
                        linearAttenuation * d +
                        quadraticAttenuation * d * d);
 
    //see if point on surface is inside cone of illumination
    float spotDot = dot(-VP, normalize(LightDirection));
    if (spotDot < spotCutoff)
        spotAttenuation = 0.0; //no contribution outside of cone
    else
        spotAttenuation = pow(spotDot, spotExponent);
        
    //combine attenuations
    attenuation *= spotAttenuation;
    
    vec3 HV = normalize(VP + eye);
    
    nDotVP = max(0.0, dot(tnorm, VP));
    float nDotHV = max(0.0, dot(tnorm, HV));
    
    if (nDotVP == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotHV, shininess);
    
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
	v_tex = in_Tex;
}
