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

/**
 * @brief 使用三角函数（cosine波）生成彩虹色映射。
 *
 * @param t 输入值，范围通常是 [0.0, 1.0]。
 * @param num_cycles 颜色循环的次数/频率。例如，1.0 是一个完整彩虹，2.0 是两个。
 * @return vec4 映射后的 RGB 颜色 (Alpha 为 1.0)。
 */
vec4 rainbow_map(float t, float num_cycles) {
    // 定义常量 PI
    const float PI = 3.1415926535;

    // --- 标准配置参数 ---
    // 幅度 (Amplitude) 和 中心 (Center) 决定颜色范围在 [0, 1] 之间。
    // A = 0.5, C = 0.5 -> 0.5 + 0.5 * [-1, 1] = [0, 1]
    const vec3 CENTER = vec3(0.5); 
    const vec3 AMPLITUDE = vec3(0.5);

    // 相位差 (Phase Shift) 决定 RGB 三原色的波峰位置，这是彩虹色的关键。
    // 0°, 120°, 240° 的相位差。
    const vec3 PHASE = vec3(0.0, 2.0/3.0, 4.0/3.0) * 2.0 * PI; 
    // ---------------------

    // 1. 根据 num_cycles 调整频率，将 t 映射到 [0, 2*PI*num_cycles] 范围
    // num_cycles 相当于之前的 frequency
    float angle = t * 2.0 * PI * num_cycles; 
    
    // 2. 计算 RGB 分量
    // R = C + A * cos(angle + P.x)
    // G = C + A * cos(angle + P.y)
    // B = C + A * cos(angle + P.z)
    vec3 color_rgb = CENTER + AMPLITUDE * cos(angle + PHASE);
    
    // 3. 限制颜色值在 [0, 1] 范围内 (通常是多余的，但能防止浮点误差)
    color_rgb = clamp(color_rgb, 0.0, 1.0);
    
    // 4. 返回带 Alpha 为 1.0 的 vec4 颜色
    return vec4(color_rgb, 1.0);
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

vec2 func1(vec2 uv,int i, int count) {
    int j;
    vec2 ans = vec2(0,0);
    for(j=0;j<count;j++){
        int mask = 1 << j;
        float sign = 1.0f;
        if((i & mask) == 0){
            sign = -1.0f;
        }
        ans = ans + sign * ipow(uv,j); 
    }
    return ans;
}

vec4 ploy(vec2 uv, int count){
    float PI = 3.1415926535;

    int i;
    vec4 final_color = vec4(0,0,0,0);
    for(i=0;i<(1 << count);i++) {
        vec2 complex_pos = func1(uv, i , count);
        // color = rainbow_map((atan(complex_pos.y, complex_pos.x) + PI) / (PI*2.0), 1.0);
        if(length(complex_pos) < 0.01f){
            final_color = vec4(1,1,1,1);
        }
    }

    // return rainbow_map((atan(uv.y, uv.x) + PI) / (PI*2.0), 1.0);
    return final_color;
}


void main() {
    // Normalized pixel coordinates (from 0 to 1)
    /*
    float t = 1.0f / (0.2f + pow(iTime - 5.0f,2.0f));
    if(iTime < 5.0f){
      t = 3.0f;
    }
    */

    float t = 1.0f / (pow(iViewSize, 2.0f));

    vec2 uv = viewSpace.xy * t - iPointer;
    // vec2 uv = viewSpace.xy * t;
    vec4 mv = ploy(uv, iCount);

    // Output to screen
    color = mv;
}
