float4 psmain(float4 position: POSITION) : SV_TARGET
{
	float4 color = {1.0f - position.z, 1.0f - position.z, 1.0f - position.z, 1.0f};
	return color;
}