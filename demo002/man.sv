#version 330 core
precision highp float;

uniform float iTime;
uniform vec2 iPointer;
uniform int iCount;
uniform float iViewSize;
uniform float c1;
uniform float c2;
uniform float c3;
uniform float c4;
uniform float t1;
uniform float t2;
uniform float t3;
uniform float t4;
uniform int fractalOption;
uniform vec2 param1;
in vec4 viewSpace;
out vec4 color;

vec4 colors(float t){
    /*
    vec4 a = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 b = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    vec4 c = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    vec4 d = vec4(0.0f, 0.3333f, 0.6666f, 0.0f);
    return a + b * cos (6.28318f*(c*t+d));
    */
    return vec4(sin(c1*t + t1),sin(c2*t+t2),sin(c3*t+t3),sin(c4*t+t4));
}

vec4 mandel(in vec2 p, int count){
    vec2 uv = p;
    int i;
    for (i = 0; i < count;i++) {
        uv = vec2(uv.x * uv.x - uv.y * uv.y + p.x, 2.0f * uv.x * uv.y + p.y);
        if(length(uv) > 3.0f){
            return colors(float(i) / float(count));
            //return vec4(uv,0.5f,1.0f);
        }
    }
    //return colors(float(i) / float(count));
    return vec4(0.0f,0.0f,0.0f,1.0f);
}

vec4 julia(in vec2 p, in vec2 c, int count){
    vec2 uv = p;
    int i;
    for (i = 0; i < count;i++) {
        uv = vec2(uv.x * uv.x - uv.y * uv.y, 2.0f * uv.x * uv.y) + c;
        if(length(uv) > 3.0f){
            return colors(float(i) / float(count));
            //return vec4(uv,0.5f,1.0f);
        }
    }
    //return colors(float(i) / float(count));
    return vec4(0.0f,0.0f,0.0f,1.0f);
}

vec4 newton_fractal(in vec2 p, int count){
    vec2 uv = p;
    vec2 p0;
    vec2 p1;
    vec2 c;
    int i;
    for(i = 0; i < count; i++){
        p1 = vec2(uv.x * uv.x - uv.y * uv.y, 2.0f * uv.x * uv.y);
        p0 = vec2(uv.x * p1.x - uv.y*p1.y,uv.x * p1.y + uv.y * p1.x);
        p1 *= 3;
        c = vec2(((p0.x * p1.x + p0.y * p1.y) / (p1.x*p1.x + p1.y*p1.y)),(p0.x*p1.y - p0.y*p1.x)/(p1.x*p1.x + p1.y*p1.y));
        //c = vec2(c.x - c.y,c.x + c.y);

        uv = uv - c;

        vec2 diff = uv - vec2(1,0);
        if(abs(diff.x) < 0.5f && abs(diff.y) < 0.5f){
            return colors(1.0f);
        }
        diff = uv - vec2(-.5,sqrt(3)/2);
        if(abs(diff.x) < 0.5f && abs(diff.y) < 0.5f){
            return colors(2.0f);
        }
        diff = uv - vec2(-.5,-sqrt(3)/2);
        if(abs(diff.x) < 0.5f && abs(diff.y) < 0.5f){
            return colors(3.0f);
        }
    }
    return vec4(0.0f,0.0f,0.0f,1.0f);
}

//vec2 pointer= vec2(1.39694462981453571600448659850713f,-0.00368452841936819587933234026143f);

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    /*
    float t = 1.0f / (0.2f + pow(iTime - 5.0f,2.0f));
    if(iTime < 5.0f){
      t = 3.0f;
    }
    */

    float t = 1.0f / (pow(iViewSize, 2.0f));

    vec2 uv = viewSpace.xy * t - iPointer;
    vec4 mv;
    if(fractalOption == 1){
        mv = mandel(uv, iCount);
    } else{
        mv = julia(uv,param1, iCount);
    }

    // Output to screen
    color = mv;
}
