
SamplerState SampleType;


cbuffer LightBuffer
{
	float4 ambientColour;
    float4 diffuseColour;
    float3 lightDirection;
	float specularPower;
    float4 specularColour;
};




struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float delta : TAIMU;
    float3 surfaceDimensions: SOULS; // CAREFUL it's width and length and 0.0
};


// pseudo random
float random (float2 st)
{
    return frac(
        sin(dot(st.xy,float2(12.9898,78.233))) 
        * 43758.5453123);
}

float noise(float2 p)
{
    // integer part
    float2 i = floor(p);
    // fractional part
    float2 f = frac(p);
    
    float ul = random(i); // + vec2(0.0, 0.0)
    float ur = random(i + float2(1.0, 0.0));
    float ll = random(i + float2(0.0, 1.0));
    float lr = random(i + float2(1.0, 1.0));
    
    // mix(x, y, a): lerp between x and y using a: x*(1−a) + y*a
    float u = lerp(ul, ur, f.x);
    float l = lerp(ll, lr, f.x);
    
    float n = lerp(u, l, f.y);
            
    return n;
}


float brownian(float2 p)
{
    int numOctaves = 3;
    float frequency = 1.0;
    float amplitude = 1.0;

    float lacunarity = 0.02;
    float gain = 0.5;

    float result = 0.0;
    for(int i; i < numOctaves; i++)
    {
        result += noise(frequency * p) * amplitude;
        frequency *= lacunarity;
        amplitude *= gain;
    }
    return result;
}

float getHeight(float2 p, float iTime)
{
    iTime *= 0.25; // slowing it down if its under 1, else speeding it up
    float b1 = brownian(p)+iTime/1.3;
    float b2 = brownian(1.2*p);
	
    float fbm1 = brownian(float2(iTime+p.x, iTime+p.y));
    float fbm1slow = brownian(float2(iTime*0.3+p.x, iTime*0.3+p.y));
    float fbm2 = brownian(float2(b1, b2));

    
    return fbm1+fbm1slow+fbm2;
}

float3 getNormal(float2 fragCoord, float zoom, float3 surfaceDimensions, float iTime)
{
    float3 normal;
    float2 thisp = (fragCoord/surfaceDimensions.xy) * zoom;
    float2 p2    = ((fragCoord + float2(1.0,0.0))/surfaceDimensions.xy) * zoom;
    float2 p3    = ((fragCoord + float2(0.0,1.0))/surfaceDimensions.xy) * zoom;
    
    float3 thisv 	= float3(thisp.x, getHeight(thisp, iTime), thisp.y);
    float3 v2 	= float3(p2.x, getHeight(p2, iTime), p2.y);
    float3 v3 	= float3(p3.x, getHeight(p3, iTime), p3.y);
    
    float3 Vec1 = thisv - v3;
    float3 Vec2 = v3 - v2;
    
    normal = normalize(cross(Vec1, Vec2));
    
    return normal;
}



//float3 lightDirection = vec3(0.4, -0.5, 0.4);
//float3 viewDirection = vec3(0.35, -1.0, 0.5);
//float specularPower = 32.0;


float4 WaterPixelShader(PixelInputType input) : SV_TARGET
{
	float lightIntensity;
	float3 lightDir;
    float3 reflection;
	float4 colour;
    float4 specular;

    float iTime = input.delta;
    float2 uv = input.tex.xy;
    float2 fragCoord = uv * input.surfaceDimensions.xy;
    
    float zoom = 500.0;
	float2 p = zoom * uv;
    
    float3 dark = float3(0.1, 0.15, 0.3);
    float3 light = float3(0.1, 0.25, 0.75);
    
    float3 colour3 = lerp(dark, light, getHeight(p, iTime));

    
    //float3 lightDir = normalize(-lightDirection);
    //float3 normal = getNormal(fragCoord, zoom, input.surfaceDimensions, iTime);
    //float lightIntensity = saturate(dot(normal, lightDirection));
    //float3 reflection = normalize(2.0 * lightIntensity * normal - lightDir);
    //float specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    
    
    
    // Output to screen
    //float4 fragColour = float4(saturate(colour3 + /*vec3(0.1,0.1,0.1)*/specular), 0.75);
    

    colour = ambientColour;
    specular = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;
    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    colour += (diffuseColour * lightIntensity);
    // Saturate the ambient and diffuse colour.
    colour = saturate(colour);
    // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
    reflection = normalize(2 * lightIntensity * input.normal - lightDir);
    // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
    specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		
	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	float4 c = float4(colour.xyz * colour3, 0.75);


	c = saturate(c + specular);




    return c;
}