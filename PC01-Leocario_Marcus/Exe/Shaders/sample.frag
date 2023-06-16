#version 330 core


uniform vec4 rgba;

out vec4 FragColor;

void main(){
	//				  r     g     b     a
	FragColor = vec4(1.0f, 0.72f, 0.77f, 1.0f);
	// FragColor = rgba;
}