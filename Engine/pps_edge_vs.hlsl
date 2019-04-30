
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


cbuffer VariableBuffer
{
    float2 screenDimensions;
    int onlyEdges;
    float padding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;

    float2 sampleOffsets[8] : SOULS;
    int onlyEdges : EDGES;
};




PixelInputType PPSEdgeVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    

    output.tex = input.tex;

	// increasing the nominator makes the image blurry
    float2 uv = 1.0 / screenDimensions;

	// | 0 | 1 | 2 |
	// | 3 | X | 4 |
	// | 5 | 6 | 7 |
    output.sampleOffsets[0] = input.tex + float2(-uv.x, -uv.y);
    output.sampleOffsets[1] = input.tex + float2( 0.0,	-uv.y);
    output.sampleOffsets[2] = input.tex + float2( uv.x, -uv.y);
	
    output.sampleOffsets[3] = input.tex + float2(-uv.x,   0.0);
	// X    --------------------------------------------------;
    output.sampleOffsets[4] = input.tex + float2( uv.x,	  0.0);
	
    output.sampleOffsets[5] = input.tex + float2(-uv.x,  uv.y);
    output.sampleOffsets[6] = input.tex + float2( 0.0,	 uv.y);
    output.sampleOffsets[7] = input.tex + float2( uv.x,	 uv.y);
    
    output.onlyEdges = onlyEdges;

    return output;
}