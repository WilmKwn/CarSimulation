cbuffer constantBuffer : register(b0)
{
	row_major float4x4 projection;
}

float4 vsmain(float4 position: POSITION) : SV_POSITION
{
	position.y -= 3.5f;

	float scale = 0.15;
	position.x *= scale;
	position.y *= scale;
	position.z *= scale;
	
	position = mul(position, projection);

	return position;
}