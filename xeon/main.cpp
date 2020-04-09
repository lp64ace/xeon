#include <iostream>
#include <fstream>
#include <gpu/texture.h>
#include <gpu/shader.h>
#include <wnd/wndlist.h>
#include <shapes/shape.h>
#include <math/math.h>
#include <thread>

WND *wnd;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 480;

void calculate_frame ( Texture& fb , Camera cam , ShapeList& world , int offset , int step ) {
	for ( int j = (SCREEN_HEIGHT-1)-offset; j >=0; j-=step ) {
		for ( int i = 0; i < SCREEN_WIDTH; i++ ) {
			glm::vec3 col ( 0.0f );

			for ( int s = 0; s < 32; s++ ) {
				float u = ( i + drand48 ( ) ) / ( float ) SCREEN_WIDTH;
				float v = ( j + drand48 ( ) ) / ( float ) SCREEN_HEIGHT;

				Ray ray = cam.get_ray ( u , v );

				glm::vec3 p = ray.point_at ( 2.0 );
				col += color ( ray , world );
			}

			col /= 100.0f;
			col = glm::vec3 ( glm::sqrt ( col.x ) , glm::sqrt ( col.y ) , glm::sqrt ( col.z ) );
			glm::ivec3 value = col * 255.99f;
			glm::u8vec4 final_color = glm::u8vec4 ( value.r , value.g , value.b , 255.0f );
			fb.write_pixel ( i , j , final_color );
		}
	}
}

int main ( )
{
	init_api ( );

	wnd = new WND ( );
	wnd->Create ( NULL , L"Xeon" , Box ( CW_USEDEFAULT , CW_USEDEFAULT , SCREEN_WIDTH , SCREEN_HEIGHT ) );
	wnd->Show ( );

	wnd->CreateContext ( );
	APP->setFpsCap ( 60.0f );

	ShapeList world;
	world.add_object ( new Sphere ( glm::vec3 ( 0.0 , -100.5 , -1.0 ) , 100.0 ) );
	world.add_object ( new Sphere ( glm::vec3 ( 0.0 , 0.0 , -1.0 ) , 0.5 ) );

	Shader shader;
	shader.load ( "shaders/basic_vertex.vert" , ShaderType::Vertex );
	shader.load ( "shaders/basic_fragment.frag" , ShaderType::Fragment );
	shader.build ( );

	Texture fb ( SCREEN_WIDTH , SCREEN_HEIGHT );
	fb.unbind ( );

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

	float aspect = float ( SCREEN_WIDTH ) / SCREEN_HEIGHT ;
	Camera cam ( glm::vec3 ( 0.0f ) , glm::vec3 ( -aspect , -1.0f , -1.0f ) );

	int thread_count = 8 ;

	for ( int i = 0 ; i < thread_count ; i++ ) {
		new std::thread ( calculate_frame , std::ref ( fb ) , cam , std::ref ( world ) , i , thread_count ) ;
	}

	while (APP->PollWindowsEvents()) {

		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		fb.bind();
		fb.upload_to_gpu();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		APP->appSwapBuffers();
	}
}
