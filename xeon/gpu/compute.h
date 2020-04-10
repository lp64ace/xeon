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
__global__ void create_world(Hitable** objects, int count, ShapeList** world, Camera** cam, curandState* rand_state, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		objects[0] = new Sphere(glm::vec3(0.0, -100.5, -1.0), 100.0, new Diffuse(glm::vec3(0.8, 0.8, 0.0)));
		objects[1] = new Sphere(glm::vec3(0, 0, -1), 0.5, new Diffuse(glm::vec3(0.8, 0.3, 0.3)));
		objects[2] = new Sphere(glm::vec3(1, 0, -1), 0.5, new Metallic(glm::vec3(0.8, 0.6, 0.2)));
		objects[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5, new Metallic(glm::vec3(0.8, 0.8, 0.8)));

		*world = new ShapeList(objects, count);

		float aspect = width / (float)height;
		*(cam) = new Camera(glm::vec3(0.0f, 0.4f, 4.0f), glm::vec3(-aspect, -1.0f, -1.0f));
	}
}

__global__ void free_world(Hitable** d_list, ShapeList** d_world, Camera** d_camera) {
	for (int i = 0; i < (*d_world)->object_count; i++) {
		delete ((Sphere*)d_list[i])->surface_material;
		delete d_list[i];
	}

	delete* d_world;
	delete* d_camera;
}