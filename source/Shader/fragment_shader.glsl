#version 410 core

#define PI 3.1415926535

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;


struct Ray
{
    vec3 origin;
    vec3 direction;
};

uniform vec3 lower_left_corner;
uniform vec3 horizontal;
uniform vec3 vertical;
uniform vec3 origin;

uniform vec3 sphereOrigin;
uniform float radius;

Ray RayCtor(vec3 o,vec3 d)
{   
    Ray r;
    r.origin=o;
    r.direction=d;
    return r;
}

Ray CameraGetRay(vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin, vec2 uv)
{
    Ray ray = RayCtor(origin, lower_left_corner + uv.x * horizontal + uv.y * vertical - origin);
    return ray;
}

bool SphereHit(vec3 sphereOrigin, float radius, Ray ray)
{
    vec3 oc = ray.origin - sphereOrigin;

    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;

    float delta = b * b - 4.0 * a * c;
  
    return delta > 0;
}


vec3 RayColor(Ray ray)
{
    if(SphereHit(sphereOrigin,radius,ray))
        return vec3(1,0,0);
    vec3 unit_direction=normalize(ray.direction);
    float t=unit_direction.y*0.5+0.5;
    return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
}

void main()
{
    Ray r = CameraGetRay(lower_left_corner, horizontal, vertical, origin, texcoord);
    fragColor = vec4(RayColor(r),1.0);
}