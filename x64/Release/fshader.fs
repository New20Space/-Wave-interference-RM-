#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Pos;
uniform float f;
uniform vec2 Sp;
uniform float M;
uniform float array[60];
uniform float sc;




float noise(in vec2 p)
{
    float co = 0.0;
    for (int i = 0; i < sc; i++)
	{
    vec2 test = vec2(array[i*2],array[i*2+1]);
    //return sin(p[0])+sin(p[1]);
    float R = length(p-test);
    co += 1.0 * sin(((2 *  3.1415926535) / 10) * R+f);
 
    }

    return co;
}

float ter(in vec3 p, in vec3 c)
{
     vec3 d = p - c;
    // vec3 s = vec3(d.x,noise(d.xz),d.z);
   
    return d.y+noise(d.xz)+2;
    
}

float sphere(in vec3 p, in vec3 c,in float r)
{
    return length(p - c) - r;
}
vec3 sh = vec3(Sp.x,0.0,Sp.y);
bool id;
float map(vec3 p){
    float d_plane = ter(p, vec3(0.0,0.0,0.0));
    float d2 = ter(sh, vec3(0.0,0.0,0.5));
    sh = vec3(sh.x,sh.y-d2,sh.z);
    float d_sphere = sphere(p,sh,1.0);
    id = (d_plane < d_sphere ) ? false : true;
    return (d_plane < d_sphere ) ? d_plane : d_sphere ;
}
const int NUMBER_OF_STEPS = 32;
const float MIN_D = 0.000001;
const float MAX_D = 100.0;
vec2 ray_march(in vec3 ro, in vec3 rd)
{
    float total_distance_traveled = 0.0;
    float d_closest;


    for (int i = 0; i < NUMBER_OF_STEPS; ++i)
    {


        vec3 p = ro + total_distance_traveled * rd;

        d_closest = map(p);
        
        total_distance_traveled += d_closest;

        if(abs(d_closest)<MIN_D || total_distance_traveled > MAX_D)break;
    }
    return vec2(total_distance_traveled,d_closest);
}
const float EPSILON = 0.1;
vec3 getNormal(in vec3 p){
    vec3 v = vec3(0.0,0.0,0.5);
    return normalize(vec3(
        map(vec3(p.x + EPSILON, p.y, p.z) ) - map(vec3(p.x , p.y, p.z)),
        map(vec3(p.x, p.y + EPSILON, p.z) ) - map(vec3(p.x, p.y , p.z)),
        map(vec3(p.x, p.y, p.z  + EPSILON)) - map(vec3(p.x, p.y, p.z ))
    ));
}

vec3 getLight(in vec3 p,in vec3 rd, in vec3 col){
    vec3 lPos = vec3(20.0,40.0,-30);
    vec3 L = normalize(lPos-p);
    vec3 N = getNormal(p);

    vec3 dif = col * vec3(dot(L,N),dot(L,N),dot(L,N));
    return dif;
}
vec3 render(in vec3 ro, in vec3 rd){
    vec3 col ;
    vec2 d = ray_march(ro, rd);
    if(d.x < MAX_D ){
        col=id? vec3(1.0,0.0,0.0) :vec3(0.27,0.51,0.71);
        vec3 p = ro + d.x*rd;
        col=getLight(p,rd,col);
        return mix(vec3(0.3,0.7,0.9),col,exp(-0.0009*d.x*d.x));
    }else{
        return vec3(0.3,0.7,0.9);
    }
}
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

void main(){
	vec2 Res = vec2(1000,1000);
    
    vec2 uv = (2.0*gl_FragCoord.xy-Res)/Res.y;

    vec3 ro = vec3(u_Pos.xy,u_Pos.z-3.0);
    vec3 rd = vec3(uv.xy,1.0);
    
    
    //float t = u_Pos.z/ 180 * 3.14159;
    //vec3 ro = vec3(0.0,u_Pos.y*sin(t),u_Pos.x*cos(t)-3.0);
    //vec3 rd = normalize(vec3(uv,1));
    
    //vec3 rd = vec3(uv.x,uv.y-sin(t),cos(t));


    vec3 end_color = render(ro,rd);

	color = vec4(end_color, 1.0);
    
}
/*
void main(){
	
   if(gl_FragCoord.x < array[0]*10){
        if(gl_FragCoord.y < array[1]*10)
            color = vec4(1.0, 0.0, 0.0, 1.0);
        else
            color = vec4(0.0, 1.0, 0.0, 1.0);
   }
   else
        color = vec4(0.0, 1.0, 0.0, 1.0);
    
}*/
