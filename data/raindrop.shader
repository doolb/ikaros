Shader "raindrop"{
    Properties{
        uTex ("Tex", 2D)
        _color ("color", vec4)
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
	            gl_Position = vec4(aPos.x,aPos.y,aPos.z, 1.0);	
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
	        void main () {		
            	vec4 col = texture(uTex, oUv); 
	            _frag_ = vec4(col.rgb, length(oColor) - 0.5);	
	        }

            #endif // here ends the definition of the fragment shader
            ENDGLSL
        }
    }
}