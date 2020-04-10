#pragma once
#include <iostream>
#include <math/camera.h>
#include <math/math.h>
#include <shapes/shape.h>

#include <gpu/texture.h>
#include <gpu/material/diffuse.h>
#include <gpu/material/metallic.h>

#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <device_launch_parameters.h>

/* ------------------------------ CUDA functions ------------------------------ */
/* Check for erros in a CUDA call. */
#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const* const func, const char* const file, int const line) 
{
	if (result) {
		std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
			file << ":" << line << " '" << func << "' \n";

		std::cerr << "Error details: " << cudaGetErrorString(result) << '\n';

		cudaDeviceReset();
		exit(99);
	}
}

/* Render the current scene on the GPU. */
__global__ void render(uint8_t* fb, int max_x, int max_y, int ns, Camera** cam, ShapeList** world, curandState* rand_state) 
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	
	if ((i >= max_x) || (j >= max_y)) return;
	
	int pixel_index = j * max_x + i;
	curandState local_rand_state = rand_state[pixel_index];
	glm::vec3 col(0, 0, 0);
	
	for (int s = 0; s < ns; s++) {
		float u = float(i + curand_uniform(&local_rand_state)) / float(max_x);
		float v = float(j + curand_uniform(&local_rand_state)) / float(max_y);
		
		Ray r = (*cam)->get_ray(u, v);
		col += advance_ray(r, *world, 8, &local_rand_state);
	}
	
	rand_state[pixel_index] = local_rand_state;
	
	col /= float(ns);
	col = glm::sqrt(col);

	fb[pixel_index * 4 + 0] = col.r * 255.99;
	fb[pixel_index * 4 + 1] = col.g * 255.99;
	fb[pixel_index * 4 + 2] = col.b * 255.99;
	fb[pixel_index * 4 + 3] = 255;
}

/* Intialize the renderer here. */
__global__ void render_init(int max_x, int max_y, curandState* rand_state) 
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	
	if ((i >= max_x) || (j >= max_y)) return;
	
	/* Init the CUDA random number generator. */
	int pixel_index = j * max_x + i;
	curand_init(1984, pixel_index, 0, &rand_state[pixel_index]);
}

/* Creates the world and put objects in it. */
__global__ void create_world(Hitable** objects, int count, ShapeList** world, Camera** cam, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		objects[0] = new Sphere(glm::vec3(0.0, 0.0, -1.0), 0.5, new Diffuse(glm::vec3(0.8, 0.3, 0.3)));
		objects[1] = new Sphere(glm::vec3(0.0, -100.5, -1.0), 100.0, new Diffuse(glm::vec3(0.8, 0.8, 0.0)));
		objects[2] = new Sphere(glm::vec3(1.0, 0.0, -1.0), 0.5, new Metallic(glm::vec3(0.8, 0.6, 0.2)));
		objects[3] = new Sphere(glm::vec3(-0.5, 0.0, -1.0), 0.5, new Metallic(glm::vec3(0.8, 0.8, 0.8)));
		objects[4] = new Sphere(glm::vec3(0.5, 0.0, 0.2), 0.2, new Metallic(glm::vec3(0.8, 0.5, 0.2)));
		objects[5] = new Sphere(glm::vec3(0.7, 0.3,  0.1), 0.5, new Metallic(glm::vec3(1.0, 0.8, 0.8)));
		objects[6] = new Sphere(glm::vec3(1.0, 0.0, 0.3), 0.5, new Diffuse(glm::vec3(0.8, 0.8, 0.8)));
		objects[7] = new Sphere(glm::vec3(-0.8, 1.0, 0.0f), 0.5, new Metallic(glm::vec3(0.5, 0.8, 0.8)));
		objects[8] = new Sphere(glm::vec3(-1.0, 0.2, -2.0), 0.5, new Diffuse(glm::vec3(0.6, 0.8, 0.0)));
		objects[9] = new Sphere(glm::vec3(0.5, 0.0, -0.0), 0.5, new Metallic(glm::vec3(0.8, 0.8, 0.8)));

		*world = new ShapeList(objects, count);

		float aspect = width / (float)height;
		*(cam) = new Camera(glm::vec3(0.0f, 0.1f, 2.0f), glm::vec3(-aspect, -1.0f, -1.0f));
	}
}

/* ------------------------------ CPU functions ------------------------------ */
void launch_kernel(Texture& screen_tex, int object_count, int block_x = 16, int block_y = 16)
{
	int width = screen_tex.twidth;
	int height = screen_tex.theight;

	/* Allocate world objects on the GPU. */
	Hitable** objects;
	checkCudaErrors(cudaMalloc((void**)&objects, object_count * sizeof(Hitable*)));
	
	ShapeList** world;
	checkCudaErrors(cudaMalloc((void**)&world, sizeof(ShapeList*)));
	
	Camera** d_camera;
	checkCudaErrors(cudaMalloc((void**)&d_camera, sizeof(Camera*)));
	
	/* Init the world on the GPU. */
	create_world<<<1, 1>>>(objects, object_count, world, d_camera, width, height);
	cudaDeviceSynchronize();

	int fb_size = 4 * screen_tex.twidth * screen_tex.theight;
	cudaMallocManaged((void**)&screen_tex.buffer, fb_size);

	curandState* d_rand_state;
	dim3 blocks(width / block_x + 1, height / block_y + 1);
	dim3 threads(block_x, block_y);
	
	/* Set rendering parameters. */
	cudaMalloc((void**)&d_rand_state, width * height * sizeof(curandState));
	render_init<<<blocks, threads>>>(width, height, d_rand_state);
	cudaDeviceSynchronize();

	/* Start the ray tracer. */
	render<<<blocks, threads>>>(screen_tex.buffer, width, height, 100, d_camera, world, d_rand_state);
	cudaDeviceSynchronize();
}