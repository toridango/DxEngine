

cbuffer CapsuleBuffer
{
    float3 cPos;
    float cLen;
    float3 cDir;
    float boundingCubeSide;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 wPos : TEXCOORD0;
	float3 normal : NORMAL;
    float3 camPos : SOULS0;
    float2 padding : SOULS1;
};

// SEND THIS THROUGH BUFFER
static const float3 centre = float3(0.0, 30.0, 0.0);
static const float radius = 1.0;

static const int STEPS = 64;
static const float root = sqrt(boundingCubeSide * boundingCubeSide * boundingCubeSide);
static const float STEP_SIZE = root / (float) STEPS;
static const float MIN_DIST = 0.01;

static const int MAX_STEPS = 100;
static const float MAX_DIST = 100.0;
static const float SURF_DIST = 0.01;


float sdSphere(float3 p, float4 s /*sphere*/)
{	   
    float sphereDist = length(p - s.xyz) - s.w;
    return sphereDist;
}

float sdCapsule(float3 p, float3 a, float3 b, float radius)
{
    float3 pa = p - a;
    float3 ba = b - a;
    float h = saturate(dot(pa, ba) / dot(ba, ba));
    return length(pa - ba * h) - radius;
}


float GetDist(float3 p)
{
    //float3 a = cPos - ((cLen / 2.0) * cDir); // float3(-1.0, 30, 0.0);
    //float3 b = cPos + ((cLen / 2.0) * cDir); // float3(1.0, 30, 0.0);
    //float3 a = float3(0.0, 30, 0.0) - cLen.x * float3(1.0, 0.0, 0.0); // float3(-1.0, 30, 0.0);
    //float3 b = float3(0.0, 30, 0.0) + cLen.x * float3(1.0, 0.0, 0.0); // float3(1.0, 30, 0.0);
    float3 a = cPos - ((cLen / 2.0) * cDir); // float3(-1.0, 30, 0.0);
    float3 b = cPos + ((cLen / 2.0) * cDir); // float3(1.0, 30, 0.0);
    return sdCapsule(p, a, b, 0.1);
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
        float distToScene = GetDist(p);
        distFromOrigin += distToScene;
        if (distToScene < SURF_DIST)
            density += /*smoothstep(radius, 0.0, length(p - centre))*/1.0 / (float) STEPS;

    /*if(distFromOrigin > MAX_DIST || distToScene < SURF_DIST)
	{
		end = true;
		close = distToScene < SURF_DIST;
	}*/

    }
    
	//return distFromOrigin * close;
	//return (1.f / (distFromOrigin / sqrt(27.))) - eps;// / GetDist(rayOrigin); //MAX_DIST;
    return density;
}



// TODO: Now make the sdf a capsule, blur its edges and make it red (optionally white core)

float4 VolumetricLaserPixelShader(PixelInputType input) : SV_TARGET
{
    float4 colour;
    float3 worldPosition = input.wPos.xyz;
    float3 viewDirection = normalize(worldPosition - input.camPos);
	

    //return float4(0.0, 0.0, 1.0, 1.0);

	//return float4((float3)raymarch2(worldPosition, viewDirection), 1.0);

    float d = RayMarch(worldPosition, viewDirection);
    //d /= GetDist(worldPosition);
    //colour = float4(1.0 - d, d, 0.0, 0.5*d);
    float c = smoothstep(0.0, 1.2, d);
    colour = float4(6.0, 0.8*(float2) c, 1.2*c);
    return colour;
 }
