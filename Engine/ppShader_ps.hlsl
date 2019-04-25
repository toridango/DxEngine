

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
float gt(float x, float y)
{
    return max(sign(x - y), 0.0);
}

// x and y must be either 0 or 1.
float and(float x, float y)
{
    return x * y;
}

// x and y must be 0 or 1.
float or(float x, float y)
{
    return min(x + y, 1.0);
}

// x must be 0 or 1
float not_(float x)
{
    return 1.0 - x;
}


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

	
    float2 d = abs(input.tex - float2(0.5, 0.5));
	
    if (min(d.x, d.y) < thickness && max(d.x, d.y) < len)
    {
        colour = float4(1.0, 0.0, 0.0, 1.0);
        //colour = 1.0 - colour;
    }


    return colour;

}
