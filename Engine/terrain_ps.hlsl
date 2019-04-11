////////////////////////////////////////////////////////////////////////////////
// Filename: terrain.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
    float3 lightDirection;
	float padding;
};


Texture2D shHighestTexture : register(t0);
Texture2D shHighTexture : register(t1);
Texture2D shLowTexture : register(t2);
Texture2D shLowestTexture : register(t3);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    // Color by height experiments
    //float4 color : COLOR;
    // ---------------------------

    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 blending : SOULS;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TerrainPixelShader(PixelInputType input) : SV_TARGET
{
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float4 textureColour1;
    float4 textureColour2;
    float4 textureColour3;
    float4 textureColour4;

    textureColour1 = shHighestTexture.Sample(SampleType, input.tex);
    textureColour2 = shHighTexture.Sample(SampleType, input.tex);
    textureColour3 = shLowTexture.Sample(SampleType, input.tex);
    textureColour4 = shLowestTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }

    // Saturate the final light color.
    color = saturate(color);


    
    // Apparently blending is in ARGB format, not RGBA // 1 is w, 2 is x, etc
    float4 c12 = lerp(textureColour2, textureColour1, input.blending.x);
    float4 c34 = lerp(textureColour4, textureColour3, input.blending.z);
    float blend = input.blending.x + input.blending.y;
    float4 c = lerp(c34, c12, blend);

    color = c;

    // Color by height experiments
    //color = input.color;
    // ---------------------------

    return color;
}