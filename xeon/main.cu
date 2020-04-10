#include <gpu/texture.h>
#include <gpu/shader.h>
#include <glad/glad.h>
#include <gpu/compute.h>
//#include <wnd/wndlist.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
int tx = 8;
int ty = 8;

//WND* wnd;

Hitable** objects;
ShapeList** world;
Camera** d_camera;
curandState* d_rand_state;

void launch_kernel(Texture& screen_tex, int object_count, int block_x = 16, int block_y = 16)
{
	int width = screen_tex.twidth;
	int height = screen_tex.theight;

	/* Allocate world objects on the GPU. */
	checkCudaErrors(cudaMalloc((void**)&objects, object_count * sizeof(Hitable*)));
	checkCudaErrors(cudaMalloc((void**)&world, sizeof(ShapeList*)));
	checkCudaErrors(cudaMalloc((void**)&d_camera, sizeof(Camera*)));
	checkCudaErrors(cudaMalloc((void**)&d_rand_state, width * height * sizeof(curandState)));

	dim3 blocks(width / block_x + 1, height / block_y + 1);
	dim3 threads(block_x, block_y);

	render_init<<<blocks, threads>>>(width, height, d_rand_state);
	checkCudaErrors(cudaDeviceSynchronize());

	/* Init the world on the GPU. */
	create_world<<<1, 1>>>(objects, object_count, world, d_camera, d_rand_state, width, height);
	checkCudaErrors(cudaDeviceSynchronize());

	int fb_size = 4 * screen_tex.twidth * screen_tex.theight;
	checkCudaErrors(cudaMallocManaged((void**)&screen_tex.buffer, fb_size));

	/* Start the ray tracer. */
	render<<<blocks, threads>>>(screen_tex.buffer, width, height, 100, d_camera, world, d_rand_state);
	checkCudaErrors(cudaDeviceSynchronize());
}

int main ( )
{
	/*init_api();

	wnd = new WND();
	wnd->Create(NULL, "Xeon", Box(CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT));
	wnd->Show();

	wnd->CreateContext();
	APP->setFpsCap(60.0f);*/
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Xeon", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("[OpenGL] glfwGetProcAddress error!\n");
	}

	Texture tex(SCREEN_WIDTH, SCREEN_HEIGHT, false);
	launch_kernel(tex, 4, tx, ty);

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
	
	while (/*APP->PollWindowsEvents()*/!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		tex.bind();
		tex.upload_to_gpu();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//APP->appSwapBuffers();
		glfwSwapBuffers(window);
	}
	
	checkCudaErrors(cudaDeviceSynchronize());
	free_world<<<1, 1>>>(objects, world, d_camera);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaFree(d_camera));
	checkCudaErrors(cudaFree(world));
	checkCudaErrors(cudaFree(objects));
	checkCudaErrors(cudaFree(d_rand_state));
	checkCudaErrors(cudaFree(tex.buffer));
	checkCudaErrors(cudaDeviceReset());
}
