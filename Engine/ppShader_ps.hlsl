

Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PostProcessPixelShader(PixelInputType input) : SV_TARGET
{
    float4 colour = shaderTexture.Sample(SampleType, input.tex);

    /*float4 r = colour - float4(0.2, 0.0, 0.0, 1.0);
    float4 g = colour - float4(0.0, 0.2, 0.0, 1.0);
    float4 b = colour - float4(0.0, 0.0, 0.2, 1.0);
    

    colour = float4(r.r, g.g, b.b, 1.0); */

    return colour;
}