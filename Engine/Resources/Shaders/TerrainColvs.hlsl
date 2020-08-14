////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 4


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 lightPos[4] : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
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
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	// Determine the light positions based on the position of the lights and the position of the vertex in the world.
	output.lightPos[0].xyz = lightPosition[0].xyz - worldPosition.xyz;
	output.lightPos[1].xyz = lightPosition[1].xyz - worldPosition.xyz;
	output.lightPos[2].xyz = lightPosition[2].xyz - worldPosition.xyz;
	output.lightPos[3].xyz = lightPosition[3].xyz - worldPosition.xyz;

	// Normalize the //-light position vectors.
	output.lightPos[0] = normalize(output.lightPos[0]);
	output.lightPos[1] = normalize(output.lightPos[1]);
	output.lightPos[2] = normalize(output.lightPos[2]);
	output.lightPos[3] = normalize(output.lightPos[3]);

	output.colour = input.colour;

	return output;
}