


Texture2D shaderTexture;
SamplerState SampleType;


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;

    float2 sampleOffsets[8] : SOULS;
    bool onlyEdges : EDGES;
};

 
static const float pi = 3.14159265358979;

float4 PPSEdgePixelShader(PixelInputType input) : SV_TARGET
{

    float4 colour = (float4) 0.0;
    float4 cMatrix[9] =
    {
		// Row 1
        shaderTexture.Sample(SampleType, input.sampleOffsets[0]),
		shaderTexture.Sample(SampleType, input.sampleOffsets[1]),
		shaderTexture.Sample(SampleType, input.sampleOffsets[2]),
		// Row 2
		shaderTexture.Sample(SampleType, input.sampleOffsets[3]),
		shaderTexture.Sample(SampleType, input.tex),
		shaderTexture.Sample(SampleType, input.sampleOffsets[4]),
		// Row 3
		shaderTexture.Sample(SampleType, input.sampleOffsets[5]),
		shaderTexture.Sample(SampleType, input.sampleOffsets[6]),
		shaderTexture.Sample(SampleType, input.sampleOffsets[7])
    };

    float greyS[9];
    float bnw[9];
    float threshold = 0.5;
	
    for (int i = 0; i < 9; ++i)
    {
        greyS[i] = (cMatrix[i].x + cMatrix[i].x + cMatrix[i].x) / 3.0;
        if (greyS[i] > threshold)
            bnw[i] = 1.0;
        else
            bnw[i] = 0.0;
    }
	
    float vKernel[9] =
    {
        -1.0,  0.0,  1.0,
		-2.0,  0.0,  2.0,
		-1.0,  0.0,  1.0
    };
	
    float hKernel[9] =
    {
        -1.0, -2.0, -1.0,
		 0.0,  0.0,  0.0,
		 1.0,  2.0,  1.0
    };

    float4 h = (float4) 0.0;
    float4 v = (float4) 0.0;
	

    if (input.onlyEdges)
    {
		for (int i = 0; i < 9; ++i)
		{
			h += greyS[i] * hKernel[i];
			v += greyS[i] * vKernel[i];
		}
		float g = pow(pow(h, 2) + pow(v, 2), 0.5);
		//colour = float4((float3) greyS, 1.0);
		//colour = h;


		g = smoothstep(0.4, 0.6, g);
        float3 edgecolour = float3(1.0, 0.0, 0.2);
		//float3 edgecolour = float3(1.0, 1.0, 1.0);

        colour = float4((float3) lerp(colour.xyz, edgecolour, g), 1.0);
    }
    else
    {
        for (int i = 0; i < 9; ++i)
        {
            h += cMatrix[i] * hKernel[i];
            v += cMatrix[i] * vKernel[i];
        }
        colour = pow(pow(h, 2) + pow(v, 2), 0.5);
        //colour = smoothstep(0.4, 0.6, colour);

    }

    return colour;
}
