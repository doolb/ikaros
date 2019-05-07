Shader "raindrop"{
    Properties{
        uTex ("Tex", 2D)
        _color ("color", vec4)
        _Time ("_Time", float)
    }
    SubShader{
        Pass{
            GLSLPROGRAM
            #ifdef VERTEX // here begins the vertex shader
            #version 330 core
            layout (location = 0)in vec3 aPos;		
            layout (location = 1)in vec3 aColor;	
            layout (location = 2)in vec2 aUv;		
            out vec3 oColor;						
            out vec2 oUv;							
	        void main(){
	            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);	
	            oColor = aColor;	
	            oUv = aUv;					
	        }

            #endif // here ends the definition of the vertex shader


            #ifdef FRAGMENT // here begins the fragment shader
            #version 330 core            
            out vec4 _frag_;	
            uniform vec4 _color;
            in vec3 oColor;		
            in vec2 oUv;		
            uniform sampler2D uTex;
            uniform float _Time;

            float N21(vec2 p){
                p = fract(p*vec2(123.34, 345.45));
                p += dot(p, p + 34.345);
                return fract(p.x * p.y);
            }
	        void main () {		
                float t = _Time;
                vec4 col = vec4(0);
				vec2 aspect = vec2(2, 1);
                float size = 3;
				vec2 uv = oUv*size*aspect;
                float drop_speed = t * 0.25;
                uv.y += drop_speed; // move uv 
				vec2 gv = fract(uv) - 0.5;

                vec2 id = floor(uv);
                float n = N21(id);
                t += n*6.28631;

                float w = oUv.y * 10;
                // drop
                float x = (n-0.5)*0.8;
                x += (0.4-abs(x))*sin(3*w)*pow(sin(w),2) * 0.45;
                float y = -sin(t+sin(t+sin(t)*0.5))*0.45;
                y -= (gv.x -x) * (gv.x - x);
                vec2 drop_pos = (gv - vec2(x,y)) / aspect;
                float drop = smoothstep(0.05, 0.03, length(drop_pos));

                // drop trail
                vec2 drop_trail_pos = (gv - vec2(x, drop_speed)) / aspect;
                drop_trail_pos.y = (fract(drop_trail_pos.y * 8) / 8) - 0.03;
                float drop_trail = smoothstep(0.03, 0.02, length(drop_trail_pos));
                float fog_trail = smoothstep(-0.05, 0.05, drop_pos.y);
                fog_trail *= smoothstep(0.5, y, gv.y);
                drop_trail *= fog_trail;
                fog_trail *= smoothstep(0.05, 0.04, abs(drop_pos.x));

                col += drop;
                col += drop_trail;
                col += fog_trail * 0.5;

                vec2 offs = drop_pos * drop * drop_trail * drop_trail_pos;
                //col.rg = gv;
                if(gv.x > 0.49 || gv.y > 0.49 || gv.x < -0.49 || gv.y < -0.49)
                    _frag_ = vec4(1,0,0,1);
                else
                    _frag_ = col;
	        }

            #endif // here ends the definition of the fragment shader
            ENDGLSL
        }
    }
}