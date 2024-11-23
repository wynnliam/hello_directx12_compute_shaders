RWTexture2D<float4> buffer : register(u0);

[numthreads(8, 8, 1)]
void main( uint3 dispatch_thread_id : SV_DispatchThreadID )
{
    uint width;
    uint height;
    float2 uv;
    
    buffer.GetDimensions(width, height);
    uv = dispatch_thread_id.xy / float2(width, height);
    
    buffer[dispatch_thread_id.xy] = float4(uv.xy, 0.0f, 1.0f);
}