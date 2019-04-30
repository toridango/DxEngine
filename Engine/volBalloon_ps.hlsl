

cbuffer CapsuleBuffer
{
    float3 cPos;
    float delta;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 wPos : TEXCOORD0;
    float3 normal : NORMAL;
    float3 camPos : SOULS0;
    float2 tex : TEXCOORD1;
};


static const int STEPS = 64;
static const float SURF_DIST = 0.01;



// Some useful functions
float3 mod289(float3 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float2 mod289(float2 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float3 permute(float3 x)
{
    return mod289(((x * 34.0) + 1.0) * x);
}

//(slightly modified version)
// Description : GLSL 2D simplex noise function 
//      Author : Ian McEwan, Ashima Arts
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License :
//  Copyright (C) 2011 Ashima Arts. All rights reserved.
//  Distributed under the MIT License. See LICENSE file.
//  https://github.com/ashima/webgl-noise
//
float snoise(float2 v)
{

    // Precompute values for skewed triangular grid
    const float4 C = float4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
                        // 1.0 / 41.0

    // First corner (x0)
    float2 i = floor(v + dot(v, C.yy));
    float2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    float2 i1 = (float2) 0.0;
    i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
    float2 x1 = x0.xy + C.xx - i1;
    float2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    float3 p = permute(
            permute(i.y + float3(0.0, i1.y, 1.0))
                + i.x + float3(0.0, i1.x, 1.0));

    float3 m = max(0.5 - float3(
                        dot(x0, x0),
                        dot(x1, x1),
                        dot(x2, x2)
                        ), 0.0);

    m = m * m;
    m = m * m;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    float3 x = 2.0 * frac(p * C.www) - 1.0;
    float3 h = abs(x) - 0.5;
    float3 ox = floor(x + 0.5);
    //float3 a0 = x - ox;
	// modification to animate the effect
    float3 a0 = x * (1 + sin(0.025 * delta)) - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);

    // Compute final noise value at P
    float3 g = (float3) 0.0;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * float2(x1.x, x2.x) + h.yz * float2(x1.y, x2.y);
    return 130.0 * dot(m, g);
}

#define OCTAVES 4

// Ridged multifractal
// See "Texturing & Modeling, A Procedural Approach", Chapter 12
float ridge(float h, float offset)
{
    h = abs(h); // create creases
    h = offset - h; // invert so creases are at top
    h = h * h; // sharpen creases
    return h;
}

float ridgedMF(float2 p)
{
    float lacunarity = 2.0;
    float gain = 0.5;
    float offset = 0.9;

    float sum = 0.0;
    float freq = 1.0, amp = 0.5;
    float prev = 1.0;
    for (int i = 0; i < OCTAVES; i++)
    {
        float n = ridge(snoise(p * freq), offset);
        sum += n * amp;
        sum += n * amp * prev; // scale by previous octave
        prev = n;
        freq *= lacunarity;
        amp *= gain;
    }
    return sum;
}




float sdSphere(float3 p, float4 s /*sphere*/)
{	   
    float sphereDist = length(p - s.xyz) - s.w;
    return sphereDist;
}



//#define eps 0.1
float RayMarch(float3 rayOrigin, float3 rayDirection)
{
    float distFromOrigin = 0.0;
    
    bool end = false;
    float density = 0.;

    for (int i = 0; i < STEPS; ++i)
    {
        float3 p = rayOrigin + rayDirection * distFromOrigin;
        float distToScene = sdSphere(p, float4(cPos, 2.0));
        distFromOrigin += distToScene;
        if (distToScene < SURF_DIST)
            density += 1.0 / (float) STEPS;
		
    }
    
    return density;
}



// TODO: Now make the sdf a capsule, blur its edges and make it red (optionally white core)

float4 VolumetricBalloonPixelShader(PixelInputType input) : SV_TARGET
{
    float4 colour;
    float3 worldPosition = input.wPos.xyz;
    float3 viewDirection = normalize(worldPosition - input.camPos);
	

    //return float4(0.0, 0.0, 1.0, 1.0);

	//return float4((float3)raymarch2(worldPosition, viewDirection), 1.0);

    float d = RayMarch(worldPosition, viewDirection);

    float c = smoothstep(0.0, 1.2, d);
	
    float rmf = ridgedMF(0.0002 * delta * input.tex * 3.0);
    //float rmf = ridgedMF(0.2 * (sin(20.0 + 0.0002 * delta) + cos(20.0 + 0.0002 * delta)) * input.tex * 3.0);
	
    //colour = float4(2.0 * c * ridgedMF(0.0002 * delta * input.tex * 3.0), 2.0 * c, 0.1 * c, 1.15 * c);
    // colour = float4(0.5 * c * rmf, 1.0 * c * rmf, 2.1 * c * rmf, 0.8 * c * rmf); // floating electricity
    colour = float4(0.5 * c * rmf , 1.0 * c * rmf, 2.1 * c * rmf, c * c * c);
    colour = smoothstep(0.0, 1.2, colour);
    return colour;
 }
