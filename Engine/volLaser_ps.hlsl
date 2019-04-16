

cbuffer LightBuffer
{
	float4 ambientColour;
    float4 diffuseColour;
    float3 lightDirection;
	float specularPower;
    float4 specularColour;
};

Texture2D shaderTexture;
SamplerState SampleType;


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
static const float STEP_SIZE = sqrt(27) / (float)STEPS;
static const float MIN_DIST = 0.01;

static const int MAX_STEPS = 100;
static const float MAX_DIST = 100.0;
static const float SURF_DIST = 0.01;

float GetDist(float3 p)
{
	float4 s = float4(centre, radius);
    
    float sphereDist =  length(p - s.xyz) - s.w;
    //float planeDist = p.y;
    
    //float d = min(sphereDist, planeDist);
    return sphereDist;
}

#define eps 0.1

float RayMarch(float3 rayOrigin, float3 rayDirection)
{
	float distFromOrigin = 0.0;
    
	bool end = false;
	float density = 0.;

	for(int i = 0; i < STEPS; ++i)
	{
    	float3 p = rayOrigin + rayDirection * distFromOrigin;
        float distToScene = GetDist(p);
        distFromOrigin += distToScene;
		if(distToScene < SURF_DIST)
			density += /*smoothstep(radius, 0.0, length(p - centre))*/1.0 / (float)STEPS;

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



float raymarch2 (float3 position, float3 direction)
{
	for (int i = 0; i < STEPS; i++)
	{
		float distance = GetDist(position);
		if (distance < MIN_DIST)
			return i / (float) STEPS;

		position += distance * direction;
	}
	return 0;
}


float4 VolumetricLaserPixelShader(PixelInputType input) : SV_TARGET
{
	float4 colour;
	float3 worldPosition = input.wPos.xyz;
	float3 viewDirection = normalize(worldPosition - input.camPos);

	/*if ( raymarchHit(worldPosition, viewDirection) )
	{
		float4 colour = lerp(
							float4(0.0, 0.0, 0.0, 0.0),
							float4(1.0, 0.0, 0.0, 1.0),
							distance(worldPosition, centre)/
							radius);
		return float4( 1.0, 0.0, 0.0, 1.0); // Red if hit the ball
	}
	else
		return float4( 0.0, 0.0, 0.0, 0.0); */// Transparent otherwise
		// return float4( 1.0, 1.0, 1.0, 1.0); // White otherwise

	//return float4((float3)raymarch2(worldPosition, viewDirection), 1.0);

    float d = RayMarch(worldPosition, viewDirection);
    //d /= GetDist(worldPosition);
    //colour = float4(1.0 - d, d, 0.0, 0.5*d);
	colour = (float4)smoothstep(0.0, 1.0, d);
	return colour;
}
