
void v_std_shadowmap
(
    in float3 modelPosition : POSITION,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
    uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));

    vertexTCoord0 = modelTCoord0;
}

sampler2D SampleBase = sampler_state
{
   MinFilter = Nearest;
   MagFilter = Nearest;
   WrapS     = Clamp;
   WrapT     = Clamp;
};

void p_std_shadowmap
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR
)
{
    // base
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	pixelColor = tex2D(SampleBase, texCoord);
}
