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
uniform float fixedPrecision;
uniform int fractalOption;
uniform vec2 param1;
uniform vec2 param2;
uniform vec2 param3;
uniform vec2 param4;
uniform vec2 param5;
uniform vec2 param6;
uniform vec2 param7;
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

vec2 imul(vec2 a,vec2 b){
    return vec2(a.x*b.x - a.y*b.y,b.x*a.y + a.x*b.y);
}

vec2 idiv(vec2 a,vec2 b){
    float m = b.x*b.x + b.y*b.y;
    return vec2((a.x*b.x+a.y*b.y)/m,(a.y*b.x - a.x*b.y)/m);
}

vec2 ipow(vec2 a, int count){
    int i;
    vec2 ans = vec2(1,0);
    for(i=0;i<count;i++){
        ans = imul(ans,a);
    }
    return ans;
}

vec4 newton_fractal_type1(in vec2 p, int count){
    vec2 uv = p;
    vec2 p0;
    vec2 p1;
    vec2 c;
    int i;
    for(i = 0; i < count; i++){
        p1 = vec2(uv.x * uv.x - uv.y * uv.y, 2.0f * uv.x * uv.y);
        p0 = vec2(uv.x * p1.x - uv.y*p1.y - 1.0f,uv.x * p1.y + uv.y * p1.x);
        p1 *= 3;
        c = vec2(((p0.x * p1.x + p0.y * p1.y) / (p1.x*p1.x + p1.y*p1.y)),(p0.y*p1.x - p0.x*p1.y)/(p1.x*p1.x + p1.y*p1.y));
        //c = vec2(c.x - c.y,c.x + c.y);

        uv = uv - c;

        vec2 diff = uv - vec2(1,0);
        float l1,l2,l3;
        l1 = length(diff);
        diff = uv - vec2(-.5f,sqrt(3)/2);
        l2 = length(diff);
        diff = uv - vec2(-.5f,-sqrt(3)/2);
        l3 = length(diff);

        if(l1 < fixedPrecision || l2 < fixedPrecision || l3 < fixedPrecision){
            if(l1 < l2 && l1 < l3){
                return vec4(0.140625f,0.0f,0.171875f,1.0f);
            }
            if(l2 < l1 && l2 < l3){
                return vec4(0.12109375f,0.171875f,0.296875f,1.0f);
            }
            if(l3 < l1 && l3 < l2){
                return vec4(0.171875f,0.35546875f,0.17578125f,1.0f);
            }
        }
    }
    return vec4(0.0f,0.0f,0.0f,1.0f);
}

vec4 newton_fractal_type2(in vec2 p, int count){
    vec2 uv = p;
    vec2 p0;
    vec2 p1;
    vec2 c;
    int i;
    for(i = 0; i < count; i++){
        p1 = vec2(uv.x * uv.x - uv.y * uv.y, 2.0f * uv.x * uv.y);
        p0 = vec2(uv.x * p1.x - uv.y*p1.y - 1.0f,uv.x * p1.y + uv.y * p1.x);
        p1 *= 3;
        c = vec2(((p0.x * p1.x + p0.y * p1.y) / (p1.x*p1.x + p1.y*p1.y)),(p0.y*p1.x - p0.x*p1.y)/(p1.x*p1.x + p1.y*p1.y));
        //c = vec2(c.x - c.y,c.x + c.y);

        uv = uv - c;
    }
    vec2 diff = uv - param1;
    float l1,l2,l3;
    l1 = length(diff);
    diff = uv - param2;
    l2 = length(diff);
    diff = uv - param3;
    l3 = length(diff);

    if(l1 < l2 && l1 < l3){
        return vec4(0.140625f,0.0f,0.171875f,1.0f);
    }
    if(l2 < l1 && l2 < l3){
        return vec4(0.12109375f,0.171875f,0.296875f,1.0f);
    }
    if(l3 < l1 && l3 < l2){
        return vec4(0.171875f,0.35546875f,0.17578125f,1.0f);
        //
    }
}

vec4 newton_fractal_type3(in vec2 p, int count){
    vec2 uv = p;
    int i;
    for(i = 0;i < count;i++){
        uv -= idiv(ipow(uv,5) + ipow(uv,2) - uv + 1.0, 5.0*ipow(uv,4) + 2.0*uv - 1.0);
    }

    vec2 diff = uv - param1;
    float l1,l2,l3,l4,l5;
    l1 = length(diff);
    diff = uv - param2;
    l2 = length(diff);
    diff = uv - param3;
    l3 = length(diff);
    diff = uv - param4;
    l4 = length(diff);
    diff = uv - param5;
    l5 = length(diff);

    float min_length = min(l1,min(l2,min(l3,min(l4,l5))));
    if(min_length == l1){
        return vec4(0.140625f,0.0f,0.171875f,1.0f);
    }else if(min_length == l2){
        return vec4(0.12109375f,0.171875f,0.296875f,1.0f);
    }else if(min_length == l3){
        return vec4(0.171875f,0.35546875f,0.17578125f,1.0f);
    }else if(min_length == l4){
        return vec4(0.140625f,0.3f,0.171875f,1.0f);
    }else if(min_length == l5){
        return vec4(0.11875f,0.2314f,0.237578125f,1.0f);
    }
}

vec4 newton_fractal_type4(in vec2 p, int count){
    vec2 uv = p;
    int i;
    for(i = 0;i < count;i++){
        uv -= idiv(ipow(uv,5) + sin(uv)  + ipow(uv,2) - uv + 1.0, 5.0*ipow(uv,4) + cos(uv) + 2.0*uv - 1.0);
    }

    vec2 diff = uv - param1;
    float l1,l2,l3,l4,l5;
    l1 = length(diff);
    diff = uv - param2;
    l2 = length(diff);
    diff = uv - param3;
    l3 = length(diff);
    diff = uv - param4;
    l4 = length(diff);
    diff = uv - param5;
    l5 = length(diff);

    float min_length = min(l1,min(l2,min(l3,min(l4,l5))));
    if(min_length == l1){
        return vec4(0.140625f,0.0f,0.171875f,1.0f);
    }else if(min_length == l2){
        return vec4(0.12109375f,0.171875f,0.296875f,1.0f);
    }else if(min_length == l3){
        return vec4(0.171875f,0.35546875f,0.17578125f,1.0f);
    }else if(min_length == l4){
        return vec4(0.140625f,0.3f,0.171875f,1.0f);
    }else if(min_length == l5){
        return vec4(0.11875f,0.2314f,0.237578125f,1.0f);
    }
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
    } else if(fractalOption == 2) {
        mv = julia(uv, param1, iCount);
    } else if(fractalOption == 3){
        mv = newton_fractal_type1(uv, iCount);
    } else if(fractalOption == 4){
        mv = newton_fractal_type2(uv, iCount);
    } else if(fractalOption == 5){
        mv = newton_fractal_type3(uv, iCount);
    } else {
        mv = newton_fractal_type4(uv, iCount);
    }

    // Output to screen
    color = mv;
}
