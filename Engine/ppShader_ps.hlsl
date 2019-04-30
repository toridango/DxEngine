

cbuffer VariableBuffer
{
    int sprinting;
    float fov;
    int aimAssist;
    float padding;
};


Texture2D shaderTexture;
SamplerState SampleType;


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

 
static const float pi = 3.14159265358979;
static const float exposure = 1.0;
static const float thickness = 0.0015;
static const float len = 0.02;

float4 PostProcessPixelShader (PixelInputType input) : SV_TARGET
{
    const float gamma = 2.2;
    /*float3 hdrColor = shaderTexture.Sample(SampleType, input.tex).rgb;      
    //float3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    //hdrColor += bloomColor; // additive blending

    // tone mapping
    float3 result = (float3)1.0  - exp(-hdrColor * exposure);
    // also gamma correct while we're at it   
    result = pow(result, (float3)1.0 / gamma);
    return float4(result, 1.0);*/
    // return float4(hdrColor, 1.0);

    //float mod = 1.0;
    float4 colour = (float4)0.0;
    if(!sprinting) 
    {
        colour = shaderTexture.Sample(SampleType, input.tex);
    }
    else
    {
        
        float2 Center   = {0.5, 0.5};
        float blurStart = 1.0;
        //float BlurWidth = -0.14;
        float blurWidth = -0.09;
        float samples   = 16.0;

        for(int i = 0; i < samples; i++)
        {
            float scale = blurStart + blurWidth * (i / (samples - 1));
            colour += shaderTexture.Sample(SampleType, (input.tex - 0.5) * scale + Center );
        }
        colour /= samples;

    }

	
    float2 d = abs(input.tex - float2(0.5, 0.5));
    float ld = length(d);
	// Threshold distance for the circle around the crosshairs
    float fovdeg = fov*(180.0 / pi);
    float th = (3.69 / (fovdeg / 2.0)) * 0.5;
	
    if (min(d.x, d.y) < thickness && max(d.x, d.y) < len)
    {
        colour = float4(1.0, 0.0, 0.0, 1.0);
        //colour = 1.0 - colour;
    }
    //if (ld > 0.081 && ld < 0.082)
    if (aimAssist)
        if (ld > (th - 0.001) && ld < th)
        {
            colour = float4(1.0, 0.0, 0.0, 0.3);
        }


        return colour;

    }
