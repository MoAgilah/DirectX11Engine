////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 4

#define DIRECTION 0
#define POINT 1
#define SPOT 2

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

struct LightProperties
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 direction;
	float specularPower;
	float spotAngle;
	float constantAtt;
	float linearAtt;
	float quadAtt;
	int lightType;
	bool IsEnabled;
	float padding;
};

cbuffer LightColorBuffer
{
	LightProperties lights[NUM_LIGHTS];
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	//float4 colour : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 lightPos[4] : TEXCOORD2;
};

float4 DoDiffuse(float3 lightDir, float3 normal, float4 diffuse)
{
	//L = lightDir is the normalized direction vector from the point we want to shade to the light source
		//L = normalize(lightPos, inputPos)
	//N = normal is the surface normal at the point we want to shade

	float NdotL = max(0, dot(normal, lightDir));

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	//diffuse = (L.N)Ld
	//where Ld = diffuse is the diffuse contribution from the light source
	float4 color = NdotL * diffuse;

	return color;
}

float4 DoSpecular(float3 eye, float3 lightDir, float3 normal, float4 specular, float specularPower)
{
	//phong lighting
	//L = lightDir is the normalized direction vector from the point we want to shade to the light source
		//L = normalize(lightPos, inputPos)
	//N = normal is the surface normal at the point we want to shade
	float3 reflection = normalize(reflect(-lightDir, normal));

	//R  is the reflection vector from the light source to the point we are shading about the surface normal N
		//R = 2(L.N)N-L
	//V = eye is the view vector and is the normalized direction vector from the point we want to shade to the eye
	float RdotV = max(0, dot(reflection, eye));

	//blinn-phong lighting
	//H = halfway vector is the half angle vector between the light vector and the view vector
		//H = normalize(L + V)
	float3 halfway = normalize(lightDir + eye);
	float NdotH = max(0, dot(reflection, halfway));

	//for phong lighting - specular = (R.V)^0 * Ls
	//0 = specularPower is the power of the specular component the higher the value, the smaller specular highlight
	//specular is the specular contribution of the light source
	float4 color = pow(RdotV, specularPower) * specular;

	//for blinn phong lighting - specular = (N.H) ^ 0 * Ls
	//float4 color = pow(NdotH, specularPower) * specular;

	return color;
}

//Attenuation is the falloff of the intensity of the light as the distance to the light souce increases
float DoAttenuation(float distance, int id)
{
	//kc = constant att is a constant value used to increase or decrease the intensity of the light independant of the distance to the light source
	//kl = linear att is the intensity of the light decreases linearly as the distance to the light source increases
	//kq = quadratic att is the intensity of the light decreases quadractically as the distance to the light source increases
	//attenuation = 1 / kc + kl*d + kq ^2

	return 1.0f / (lights[id].constantAtt + lights[id].linearAtt * distance + lights[id].quadAtt * distance * distance);
}

float4 DoPointLightCalculations(PixelInputType input, int id)
{
	float4 color;

	// Set the default output color to the ambient light value for all pixels.
	color = lights[id].ambientColor;

	float3 lightDir = input.lightPos[id];

	//the distance from the point being shaded to the light position is computed
	float distance = length(lightDir);

	//the light vector L is computed
	lightDir = lightDir / distance;

	float att = DoAttenuation(distance, id);

	//multiply the diffuse and specular contributions by att so the light intensity is redued as the distance to the light source increases
	color += DoDiffuse(lightDir, input.normal, lights[id].diffuseColor) * att;
	color += DoSpecular(input.viewDirection, lightDir, input.normal, lights[id].specularColor, lights[id].specularPower) * att;

	return color;
}

float4 DoDirectionalLightCalculations(PixelInputType input, int id)
{
	float4 color;

	// Set the default output color to the ambient light value for all pixels.
	color = lights[id].ambientColor;

	//negate the light's Direction property because we need the light vector (L) to point from the point being shaded (P) to the light source.
	float3 lightDir = -lights[id].direction.xyz;

	color += DoDiffuse(lightDir, input.normal, lights[id].diffuseColor);

	color += DoSpecular(input.viewDirection, lightDir, input.normal, lights[id].specularColor, lights[id].specularPower);

	return color;
}

float DoSpotCone(float3 L, int id)
{
	float minCos = cos(lights[id].spotAngle);
	float maxCos = (minCos + 1.f) / 2.f;
	float cosAngle = dot(lights[id].direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

float4 DoSpotLight(PixelInputType input, int id)
{
	float4 color;

	// Set the default output color to the ambient light value for all pixels.
	color = lights[id].ambientColor;

	float3 lightDir = input.lightPos[id];

	//the distance from the point being shaded to the light position is computed
	float distance = length(lightDir);

	//the light vector L is computed
	lightDir = lightDir / distance;

	float att = DoAttenuation(distance, id);
	float spotInt = DoSpotCone(lightDir, id);

	//multiply the diffuse and specular contributions by att so the light intensity is redued as the distance to the light source increases
	color += DoDiffuse(lightDir, input.normal, lights[id].diffuseColor) * att * spotInt;
	color += DoSpecular(input.viewDirection, lightDir, input.normal, lights[id].specularColor, lights[id].specularPower) * att * spotInt;

	return color;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 color = float4(0, 0, 0, 1);

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		if (lights[i].IsEnabled)
		{
			if (lights[i].lightType == POINT)
			{
				color += DoPointLightCalculations(input, i);
			}
			else if (lights[i].lightType == SPOT)
			{
				color += DoSpotLight(input, i);
			}
			else if (lights[i].lightType == DIRECTION)
			{
				color += DoDirectionalLightCalculations(input, i);
			}

		}

	}

	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	color = saturate(color) * textureColor;

	return color;
}
