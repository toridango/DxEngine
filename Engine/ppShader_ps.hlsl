

/*Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 PostProcessPixelShader(PixelInputType input) : SV_TARGET
{
    float4 colour = shaderTexture.Sample(SampleType, input.tex);

    return colour;
}*/


/*  // 256 Colouring
    float4 c = shaderTexture.Sample(SampleType, input.tex);	
	c=round(c*10)/10;
	c.a=1;
	return c;
*/


cbuffer VariableBuffer
{
    bool sprinting;
    float3 padding;
};


Texture2D shaderTexture;
SamplerState SampleType;


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

 
static const float exposure = 1.0;

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
        float BlurStart = 1.0;
        //float BlurWidth = -0.14;
        float BlurWidth = -0.09;
        float samples   = 16.0;

        for(int i = 0; i < samples; i++)
        {
            float scale = BlurStart + BlurWidth * (i / (samples - 1));
            colour += shaderTexture.Sample(SampleType, (input.tex - 0.5) * scale + Center );
        }
        colour /= samples;

    }


    return colour;

}
