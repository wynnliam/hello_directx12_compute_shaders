RWTexture2D<float4> buffer : register(u0);

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint width;
    uint height;
    float2 uv;
    
    buffer.GetDimensions(width, height);
    uv = dispatchThreadID.xy / float2(width, height);
    
    buffer[dispatchThreadID.xy] = float4(uv.xy, 0.0f, 1.0f);
}