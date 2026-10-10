$input a_position, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_color0, v_texcoord0
#include <bgfx_shader.sh>
void main()
{
    float index = a_position.x;
    vec2 corner;
    vec2 packedColor;
    if (index < 0.5 || (index > 3.5 && index < 4.5 && i_data4.y < 0.5))
    { corner = vec2(0.0, 0.0); packedColor = i_data2.xy; }
    else if (index < 1.5 || (index > 3.5 && index < 4.5))
    { corner = vec2(0.0, 1.0); packedColor = i_data2.zw; }
    else if ((index > 2.5 && index < 3.5) || (index < 2.5 && i_data4.y > 0.5))
    { corner = vec2(1.0, 0.0); packedColor = i_data3.zw; }
    else
    { corner = vec2(1.0, 1.0); packedColor = i_data3.xy; }
    vec2 position = mix(i_data0.xy, i_data0.zw, corner);
    gl_Position = mul(u_modelViewProj, vec4(position, i_data4.x, 1.0));
    v_texcoord0 = mix(i_data1.xy, i_data1.zw, corner);
    v_color0 = vec4(mod(packedColor.x, 256.0), floor(packedColor.x / 256.0),
                    mod(packedColor.y, 256.0), floor(packedColor.y / 256.0)) / 255.0;
}
