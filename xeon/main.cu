#include <gpu/texture.h>
#include <gpu/shader.h>
#include <glad/glad.h>
#include <gpu/compute.h>
#include <wnd/wndlist.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
int tx = 32;
int ty = 32;

WND* wnd;

int main ( )
{
	init_api();

	wnd = new WND();
	wnd->Create(NULL, "Xeon", Box(CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT));
	wnd->Show();

	wnd->CreateContext();
	APP->setFpsCap(60.0f);

	Texture tex(SCREEN_WIDTH, SCREEN_HEIGHT, false);
	launch_kernel(tex, 10, tx, ty);

	Shader shader;
	shader.load ( "shaders/basic_vertex.vert" , ShaderType::Vertex );
	shader.load ( "shaders/basic_fragment.frag" , ShaderType::Fragment );
	shader.build ( );
	
	float quad_data [ 24 ] =
	{
		-1.f,  1.f, 0.0f, 1.0f,
		 1.f,  1.f, 1.0f, 1.0f,
		-1.f, -1.f, 0.0f, 0.0f,

		-1.f, -1.f, 0.0f, 0.0f,
		 1.f, -1.f, 1.0f, 0.0f,
		 1.f,  1.f, 1.0f, 1.0f
	};

	unsigned int VBO , VAO;
	glGenVertexArrays ( 1 , &VAO );
	glGenBuffers ( 1 , &VBO );
	glBindVertexArray ( VAO );

	glBindBuffer ( GL_ARRAY_BUFFER , VBO );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( quad_data ) , quad_data , GL_STATIC_DRAW );

	glVertexAttribPointer ( 0 , 2 , GL_FLOAT , GL_FALSE , 4 * 4 , ( void * ) 0 );
	glEnableVertexAttribArray ( 0 );

	glVertexAttribPointer ( 1 , 2 , GL_FLOAT , GL_FALSE , 4 * 4 , ( void * ) 8 );
	glEnableVertexAttribArray ( 1 );
	
	while (APP->PollWindowsEvents()) {

		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		tex.bind();
		tex.upload_to_gpu();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		APP->appSwapBuffers();
	}
	
	checkCudaErrors(cudaDeviceSynchronize());
	cudaDeviceReset();
}
