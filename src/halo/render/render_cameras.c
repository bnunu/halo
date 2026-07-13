/* re-implemented from cachebeta.xbe disassembly */

double render_camera_get_adjusted_field_of_view_tangent(float field_of_view)
{
  double result;
  float half_angle = field_of_view * 0.5f;
  __asm__("fptan\n\tfstp %%st(0)" : "=t"(result) : "0"(half_angle));
  return result * 0.85f;
}
