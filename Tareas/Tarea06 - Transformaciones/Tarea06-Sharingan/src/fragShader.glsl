#version 330 core

out vec4 fColor;
uniform vec3 u_color; 
vec2 resize = vec2(800,800);

void main()
{  

	//Pinta el borde negro del circulo
	vec2 coor = gl_FragCoord.xy / resize;
	
	float Xcuadr = (coor.x - 0.5) * (coor.x - 0.5);
	float Ycuadr = (coor.y - 0.5) * (coor.y - 0.5);
	
	float radioCuadr = Ycuadr + Xcuadr;
	
	if(radioCuadr >= 0.23) {
		fColor = vec4 (0.0,0.0,0.0,1.0);
	}else {
		fColor = vec4(u_color,1.0);
	}
	
	
	
}
