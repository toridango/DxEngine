

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float cpadding;
};

cbuffer VariableBuffer
{
    float delta;
	float3 padding;
};




struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 wPos : TEXCOORD0;
	float3 normal : NORMAL;
    float3 camPos : SOULS0;
    float2 padding : SOULS1;
};



PixelInputType VolumetricLaserVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.padding = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

	
    // Normalize the viewing direction vector.
    //output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
    //output.viewDirection = normalize(worldPosition.xyz - cameraPosition.xyz);
    output.camPos = cameraPosition.xyz;

    //output.wPos = input.position;
	output.wPos = worldPosition;


    return output;
}