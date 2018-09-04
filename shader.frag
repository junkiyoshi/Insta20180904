#version 150

const int number_of_targets = 2;

uniform float time;
uniform vec2 resolution;
uniform sampler2DRect tex;
uniform vec4 targets[number_of_targets];
uniform vec4 sizes[number_of_targets];

in vec2 texCoordVarying;
out vec4 outputColor;

void main() {

  vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

  for(int i = 0; i < number_of_targets; i++){

    vec2 t = vec2(targets[i].x, -targets[i].y) / min(resolution.x, resolution.y) * 2.0;
    t.xy += vec2(-resolution.x, resolution.y) / min(resolution.x, resolution.y);

    vec2 sz = vec2(sizes[i].x, -sizes[i].y) / min(resolution.x, resolution.y) * 2.0;

    vec2 q = p - t;
    float d = length(q);
    if(d < length(sz)){

      float a = atan(q.y, q.x);
      float x = t.x + cos(a + time * 5 + floor(d * 10) * 0.5) * d;
      float y = t.y + sin(a + time * 5 + floor(d * 10) * 0.5) * d;
      p = vec2(x, y);
    }
  }

  vec4 color = texture(tex, vec2((p.x * min(resolution.x, resolution.y) + resolution.x) * 0.5, resolution.y - (p.y * min(resolution.x, resolution.y) + resolution.y) * 0.5));
  outputColor = color;
}
