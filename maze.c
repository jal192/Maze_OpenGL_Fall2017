/*
* maze.c
*
*  Created on: Oct 26, 2017
*      Author: Jason Ly
*/

#ifdef __APPLE__  // include Mac OS X versions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "vec4mat4headers.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#include "initShader.h"
#include "genMaze.h"
#include <string.h>
#include <time.h>
#include <windows.h>

// Function signatures
mat4 scale(float sx, float sy, float sz);
mat4 translate(float dx, float dy, float dz);
mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat atx, GLfloat aty, GLfloat atz, GLfloat upx, GLfloat upy, GLfloat upz);
mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near_val, GLfloat far_val);

// Number of vertices
int num_vertices = (36 * 79) + (36 * 81) + (36);	// Number of vertices for walls + number of vertices for posts + number of vertices for ground floor
int cube_num_vertices = 36;
int ground_num_vertices = 36;

vec4 maze_vertices[5796];
vec4 maze_colors[5796];
int maze_num_vertices = 5796;

// Vertices for ground/floor cube
vec4 ground_vertices[36] = {
	{ -100.0f, 0.0f, 100.0f, 1.0f }, { -100.0f, -2.0f, 100.0f, 1.0f }, { 100.0f, -2.0f, 100.0f, 1.0f },		// Front Side of Cube
	{ 100.0f, 0.0f, 100.0f, 1.0f }, { -100.0f, 0.0f, 100.0f, 1.0f }, { 100.0f, -2.0f, 100.0f, 1.0f }, 		// Front Side of Cube
	{ -100.0f, -2.0f, -100.0f, 1.0f }, { -100.0f, -2.0f, 100.0f, 1.0f }, { -100.0f, 0.0f, 100.0f, 1.0f },	// Left Side of Cube
	{ -100.0f, -2.0f, -100.0f, 1.0f }, { -100.0f, 0.0f, 100.0f, 1.0f }, { -100.0f, 0.0f, -100.0f, 1.0f },	// Left Side of Cube
	{ 100.0f, 0.0f, -100.0f, 1.0f }, { -100.0f, -2.0f,-100.0f, 1.0f }, { -100.0f, 0.0f, -100.0f, 1.0f },	// Back Sidde of Cube
	{ 100.0f, 0.0f, -100.0f, 1.0f }, { 100.0f, -2.0f, -100.0f, 1.0f }, { -100.0f, -2.0f, -100.0f, 1.0f },	// Back Side of Cube
	{ 100.0f, -2.0f, 100.0f, 1.0f }, { -100.0f, -2.0f, -100.0f, 1.0f }, { 100.0f, -2.0f, -100.0f, 1.0f },	// Bottom Side of Cube
	{ 100.0f, -2.0f, 100.0f, 1.0f }, { -100.0f, -2.0f, 100.0f, 1.0f }, { -100.0f, -2.0f, -100.0f, 1.0f },	// Bottom Side of Cube
	{ 100.0f, 0.0f, 100.0f, 1.0f }, { 100.0f, -2.0f,-100.0f, 1.0f }, { 100.0f, 0.0f, -100.0f, 1.0f },		// Right side of Cube
	{ 100.0f, -2.0f, -100.0f, 1.0f }, { 100.0f, 0.0f, 100.0f, 1.0f }, { 100.0f, -2.0f, 100.0f, 1.0f },		// Right Side of Cube
	{ 100.0f, 0.0f, 100.0f, 1.0f }, { -100.0f, 0.0f, -100.0f, 1.0f }, { -100.0f, 0.0f, 100.0f, 1.0f },		// Top Side of Cube
	{ 100.0f, 0.0f, 100.0f, 1.0f }, { 100.0f, 0.0f, -100.0f, 1.0f }, { -100.0f, 0.0f, -100.0f, 1.0f } 		// Top Side of Cube
};

// Color for ground cube
vec4 ground_colors[36] = {
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
	{ 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f }, { 0.0f, 0.588f, 0.0f, 1.0f },	// Dark Green
};

// Vertices for cube
vec4 cube_vertices[36] = {
	{ -0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f },		// Front Side of Cube
	{ 0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f }, 		// Front Side of Cube
	{ -0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f },	// Left Side of Cube
	{ -0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f },	// Left Side of Cube
	{ 0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f,-0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f },	// Back Sidde of Cube
	{ 0.5f, 0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f },	// Back Side of Cube
	{ 0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f },	// Bottom Side of Cube
	{ 0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f },	// Bottom Side of Cube
	{ 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f,-0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f },		// Right side of Cube
	{ 0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f },		// Right Side of Cube
	{ 0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f },		// Top Side of Cube
	{ 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f }		// Top Side of Cube
};

// Color for cube
vec4 cube_colors[36] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },	// Red
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },	// Red
	{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, // Teal
	{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, // Teal
	{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, // Purple
	{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, // Purple
	{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },	// Green
	{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },	// Green
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, // Yellow
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, // Yellow
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, // Blue
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, // Blue
};

// Color for post
vec4 post_colors[36] = {
	{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, // Purple
	{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, // Purple
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, // Yellow
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, // Yellow
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },	// Red
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },	// Red
	{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },	// Green
	{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },	// Green
	{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, // Teal
	{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, // Teal
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, // Blue
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, // Blue
};

// Constants used for converting between radians and degrees
float radian_to_degree = 180.0 / M_PI;
float degree_to_radian = (M_PI / 180.0);

// Set the screen width and height
int screen_width = 512;
int screen_height = 512;

// Moves the window to the right side of the screen
int window_position_width = 900;
int window_position_height = 100;

// Stores the radius or what half the sides of the screen is
int mid_width_x;
int mid_height_y;

GLuint model_view_location;
GLuint projection_location;

// Model view matrix
mat4 model_view =
{ { 1.0, 0.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 0.0, 1.0 } };

// Projection matrix
mat4 projection =
{ { 1.0, 0.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 0.0, 1.0 } };

// Keep track of idle state
int enableIdle = 0;

// Check if a last rotation matrix has been stored
int last_rot_mat_exists = 0;

// Scaling factor to change size of 1x1x1 cube for the north and south walls
GLfloat ns_wall_scaling_factor_x = 1.6f;
GLfloat ns_wall_scaling_factor_y = 2.0f;
GLfloat ns_wall_scaling_factor_z = 0.2f;

// Scaling factor to change size of 1x1x1 cube for the west and east walls
GLfloat ew_wall_scaling_factor_x = 0.2f;
GLfloat ew_wall_scaling_factor_y = 2.0f;
GLfloat ew_wall_scaling_factor_z = 1.6f;

// Scaling factor to change size of 1x1x1 cube for the posts between walls
GLfloat post_scaling_factor_x = 0.4f;
GLfloat post_scaling_factor_y = 2.0f;
GLfloat post_scaling_factor_z = 0.4f;

// Keep track of the angle the eye is rotating
// Starting angle so eye point is in front of the entrance
GLfloat angle = -27.0f;
// Angle incremement sets the speed of the eye spinning around the maze
GLfloat angle_increment = 0.5f;

// Values for frustum
GLfloat fx_neg = -1.0f;
GLfloat fx_pos = 0.5f;
GLfloat fy_neg = 0.0f;
GLfloat fy_pos = 0.5f;
GLfloat fz_pos = -0.7f;
GLfloat fz_neg = -1000.0f;

// Initial eye point when the program is started
// Where the eye is initially located when flying around the maze
GLfloat start_eye_x = -12.1f;
GLfloat start_eye_y = 6.6f;
GLfloat start_eye_z = -7.6f;

// Eye point in front of the entrance
GLfloat entrance_eye_x = -9.0f;
GLfloat entrance_eye_y = 1.2f;
GLfloat entrance_eye_z = -7.0f;

// Point that the eye is looking at, looking at the other end of maze
// Provides smoother transition from flying down to the entrance of the maze
GLfloat entrance_at_x = 7.0f;
GLfloat entrance_at_y = 1.2f;
GLfloat entrance_at_z = -7.0f;

// Looking at the next position to move to
// First step inside the maze
GLfloat start_at_x = -7.0f;
GLfloat start_at_y = 1.2f;
GLfloat start_at_z = -7.0f;

// Keep track of the current eye location
GLfloat current_eye_x;
GLfloat current_eye_y;
GLfloat current_eye_z;
GLfloat current_at_x;
GLfloat current_at_y;
GLfloat current_at_z;

// Keep track of the next eye/target location
GLfloat next_eye_x;
GLfloat next_eye_y;
GLfloat next_eye_z;
GLfloat next_at_x;
GLfloat next_at_y;
GLfloat next_at_z;

// Set the turning speed and keep track of how much the eye turns
// This speed manages the transition of the eye moving from above the maze to the entrance
GLfloat turning_speed = 0.001f;
GLfloat curr_turning_angle = 0.001f;

// Set maze solving speed and keep track of how much the eye has moved to it's target
// curr_distance ranges from 0 - 1.
// 0 being the start point, 1 being the end point
GLfloat maze_solve_speed = 0.001f;
GLfloat curr_distance = 0.001f;

GLfloat eye_x;
GLfloat eye_z;

// Keep track of the current number of times that we looked at the maze
int fly_around_counter = 0;

// Set the number of times to fly around the maze
int num_times_fly_around_maze = 1;

// Array of the next locations that the eye has to move
vec4 next_eye_array[500] = { { 0 } };

// Array of the location that the eye looks at
vec4 next_at_array[500] = { { 0 } };

// Keep track of the number of moves, this includes turning and moving
int move_counter = 0;

// Keep track of the total number of moves
int num_moves = 0;

// Keep track of the progress of the idle function
int done_turn_to_entrance = 0; // 0 = eye hasn't been moved to front of maze, 1 = eye is in front of maze, done turning
int solve_maze = 0; // 0 = not ready to solve maze, 1 = start solving the maze

void init(void) {
	int vIndex = 0;
	
	// Copy the ground cube into the world vertices (maze vertices)
	for (int i = 0; i < ground_num_vertices; i++) {
		maze_vertices[vIndex] = ground_vertices[i];
		maze_colors[vIndex] = ground_colors[i];
		vIndex++;
	}
	
	int row, column;

	srand(time(0));

	cell *cells = (cell *)malloc(sizeof(cell) * 8 * 8);

	memset(cells, 0, sizeof(cells) * 8 * 8);

	gen_maze(8, 8, cells);

	print_maze(8, 8, cells);

	// Cast the one-dimensional array of cells into two-dimensional
	// array of cells

	cell(*cells2D)[8] = (cell(*)[8]) cells;

	// Solve the maze
	// Start of the maze is cells2D[0][0]
	// End of maze is cells2D[8][8]

	// Representing direction as 1 = North, 2 = East, 3 = South, 4 = West
	// Eye always starts by facing east
	int dir = 2;
	int curr_row = 0;
	int curr_col = 0;
	
	// Move the eye from the entrance of the maze into the first step inside of the maze 
	vec4 first_eye = { start_at_x, start_at_y, start_at_z, 1 };
	vec4 first_at = { start_at_x + 2.0f, start_at_y, start_at_z, 1 };
	next_eye_array[0] = first_eye;
	next_at_array[0] = first_at;
	num_moves++;

	// Keep track of the current location of the eye and where it's looking at
	vec4 curr_eye = first_eye;
	vec4 curr_at = first_at;
	
	// Keep going through maze until we reach the end
	while (curr_row != 8 && curr_col != 8) {
		// Eye is facing north
		if (dir == 1) {
			// Check left wall, turn left and move forward if there is no wall
			if (cells2D[curr_row][curr_col].west == 0) {
				// Turn left
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x - 2.0f, temp_at_1.y, temp_at_1.z, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col -= 1;
				dir = 4;
			}
			// Check front wall, move forward if there is no wall
			else if (cells2D[curr_row][curr_col].north == 0) {
				// Move forward
				next_eye_array[num_moves] = curr_at;
				vec4 temp_at_1 = { curr_at.x, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row -= 1;
				dir = 1;
			}
			// Check right wall, turn right and move forward if there is no wall
			else if (cells2D[curr_row][curr_col].east == 0) {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x + 2.0f, temp_at_1.y, temp_at_1.z, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col += 1;
				dir = 2;
			}
			// At a dead end, turn around and go back one spot
			else {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_2 = { temp_at_1.x - 2.0f, temp_at_1.y, temp_at_1.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_2;
				vec4 temp_at_3 = { temp_at_2.x, temp_at_2.y, temp_at_2.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_3;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row += 1;
				dir = 3;
			}
		}
		// Eye is facing east
		else if (dir == 2) {
			// Check left wall, turn left and move forward if there is no wall
			if (cells2D[curr_row][curr_col].north == 0) {
				// Turn left
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x, temp_at_1.y, temp_at_1.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row -= 1;
				dir = 1;
			}
			// Check front wall, move forward if there is no wall
			else if (cells2D[curr_row][curr_col].east == 0) {
				// Move forward
				next_eye_array[num_moves] = curr_at;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col += 1;
				dir = 2;
			}
			// Check right wall, turn right and move forward if there is no wall
			else if (cells2D[curr_row][curr_col].south == 0) {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x, temp_at_1.y, temp_at_1.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row += 1;
				dir = 3;
			}
			// At a dead end, turn around and go back one spot
			else {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_2 = { temp_at_1.x - 2.0f, temp_at_1.y, temp_at_1.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_2;
				vec4 temp_at_3 = { temp_at_2.x - 2.0f, temp_at_2.y, temp_at_2.z, 1 };
				next_at_array[num_moves] = temp_at_3;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col -= 1;
				dir = 4;
			}
		}
		// Eye is facing south
		else if (dir == 3) {
			// Check left wall, turn left and move forward if there is no wall
			if (cells2D[curr_row][curr_col].east == 0) {
				// Turn left
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x + 2.0f, temp_at_1.y, temp_at_1.z, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col += 1;
				dir = 2;
			}
			// Check front wall, move forward if there is no wall
			else if (cells2D[curr_row][curr_col].south == 0) {
				// Move forward
				next_eye_array[num_moves] = curr_at;
				vec4 temp_at_1 = { curr_at.x, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row += 1;
				dir = 3;
			}
			// Check right wall, turn right and move forward if there is no wall
			else if (cells2D[curr_row][curr_col].west == 0) {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x - 2.0f, temp_at_1.y, temp_at_1.z, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col -= 1;
				dir = 4;
			}
			// At a dead end, turn around and go back one spot
			else {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_2 = { temp_at_1.x + 2.0f, temp_at_1.y, temp_at_1.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_2;
				vec4 temp_at_3 = { temp_at_2.x, temp_at_2.y, temp_at_2.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_3;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row -= 1;
				dir = 1;
			}
		}
		// Eye is facing west
		else if (dir == 4) {
			// Check left wall, turn left and move forward if there is no wall
			if (cells2D[curr_row][curr_col].south == 0) {
				// Turn left
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x, temp_at_1.y, temp_at_1.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row += 1;
				dir = 3;
			}
			// Check front wall, move forward if there is no wall
			else if (cells2D[curr_row][curr_col].west == 0) {
				// Move forward
				next_eye_array[num_moves] = curr_at;
				vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col -= 1;
				dir = 4;
			}
			// Check right wall, turn right and move forward if there is no wall
			else if (cells2D[curr_row][curr_col].north == 0) {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_1;
				vec4 temp_at_2 = { temp_at_1.x, temp_at_1.y, temp_at_1.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_row -= 1;
				dir = 1;
			}
			// At a dead end, turn around and go back one spot
			else {
				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_1 = { curr_at.x + 2.0f, curr_at.y, curr_at.z - 2.0f, 1 };
				next_at_array[num_moves] = temp_at_1;
				num_moves++;

				// Turn right 
				next_eye_array[num_moves] = curr_eye;
				vec4 temp_at_2 = { temp_at_1.x + 2.0f, temp_at_1.y, temp_at_1.z + 2.0f, 1 };
				next_at_array[num_moves] = temp_at_2;
				num_moves++;

				// Move forward
				next_eye_array[num_moves] = temp_at_2;
				vec4 temp_at_3 = { temp_at_2.x + 2.0f, temp_at_2.y, temp_at_2.z, 1 };
				next_at_array[num_moves] = temp_at_3;
				num_moves++;

				// Update current eye point and at point
				curr_eye = next_eye_array[num_moves - 1];
				curr_at = next_at_array[num_moves - 1];

				// Update position in 2D array layout of maze and update direction the eye is facing
				curr_col += 1;
				dir = 2;
			}
		}
	}

	// Move outside of the maze
	next_eye_array[num_moves] = curr_at;
	vec4 temp_at_12 = { curr_at.x + 2.0f, curr_at.y, curr_at.z, 1 };
	next_at_array[num_moves] = temp_at_12;
	num_moves++;

	// Update current eye point and at point
	curr_eye = next_eye_array[num_moves - 1];
	curr_at = next_at_array[num_moves - 1];

	// Update position in 2D array layout of maze and update direction the eye is facing
	curr_col += 1;
	dir = 2;

	// These following moves are to turn the eye around to face the exit
	// Turn right 
	next_eye_array[num_moves] = curr_eye;
	vec4 temp_at_1 = { curr_at.x - 2.0f, curr_at.y, curr_at.z + 2.0f, 1 };
	next_at_array[num_moves] = temp_at_1;
	num_moves++;

	// Turn right 
	next_eye_array[num_moves] = curr_eye;
	vec4 temp_at_2 = { temp_at_1.x - 2.0f, temp_at_1.y, temp_at_1.z - 2.0f, 1 };
	next_at_array[num_moves] = temp_at_2;
	num_moves++;

	// Update current eye point and at point
	curr_eye = next_eye_array[num_moves - 1];
	curr_at = next_at_array[num_moves - 1];

	// Update position in 2D array layout of maze and update direction the eye is facing
	curr_col -= 1;
	dir = 4;

	// Display the number of moves, helps determined the length of the simulation
	printf("Number of moves: %d\n", num_moves);
	
	float x = -7;
	float z = -8;

	mat4 temp_tr_matrix = m4_identity();
	mat4 ns_scaling_matrix = scale(ns_wall_scaling_factor_x, ns_wall_scaling_factor_y, ns_wall_scaling_factor_z);

	// Draw the north walls, doesn't include the last wall at the bottom
	for (row = 0; row < 8; row++)
	{
		for (column = 0; column < 8; column++)
		{
			if (cells2D[row][column].north == 1) {
				temp_tr_matrix = m4_identity();
				temp_tr_matrix = m4_mult(ns_scaling_matrix, temp_tr_matrix);
				mat4 temp_mat4 = translate(x, 1.0, z);
				temp_tr_matrix = m4_mult(temp_mat4, temp_tr_matrix);
				
				for (int j = 0; j < 36; j++) {
					maze_vertices[vIndex] = m4_mult_vec(temp_tr_matrix, cube_vertices[j]);
					maze_colors[vIndex] = cube_colors[j];
					vIndex++;
				}
			}

			x += 2;
		}

		x = -7;
		z += 2;
	}

	// Draw the south wall
	for (column = 0; column < 8; column++)
	{
		if (cells2D[7][column].south == 1) {
			temp_tr_matrix = m4_identity();
			temp_tr_matrix = m4_mult(ns_scaling_matrix, temp_tr_matrix);
			mat4 temp_mat4 = translate(x, 1.0, 8);
			temp_tr_matrix = m4_mult(temp_mat4, temp_tr_matrix);

			for (int j = 0; j < 36; j++) {
				maze_vertices[vIndex] = m4_mult_vec(temp_tr_matrix, cube_vertices[j]);
				maze_colors[vIndex] = cube_colors[j];
				vIndex++;
			}
		}

		x += 2;
	}

	x = -8;
	z = -7;

	temp_tr_matrix = m4_identity();
	mat4 ew_scaling_matrix = scale(ew_wall_scaling_factor_x, ew_wall_scaling_factor_y, ew_wall_scaling_factor_z);

	// Draw all of the west walls, excludes the far east wall
	for (row = 0; row < 8; row++)
	{
		for (column = 0; column < 8; column++)
		{
			if (cells2D[row][column].west == 1) {
				temp_tr_matrix = m4_identity();
				temp_tr_matrix = m4_mult(ew_scaling_matrix, temp_tr_matrix);
				mat4 temp_mat4 = translate(x, 1.0, z);
				temp_tr_matrix = m4_mult(temp_mat4, temp_tr_matrix);

				for (int j = 0; j < 36; j++) {
					maze_vertices[vIndex] = m4_mult_vec(temp_tr_matrix, cube_vertices[j]);
					maze_colors[vIndex] = cube_colors[j];
					vIndex++;
				}
			}

			x += 2;
		}

		x = -8;
		z += 2;
	}

	z = -7;

	// Draw the far east wall
	for (row = 0; row < 8; row++)
	{
		if (cells2D[row][7].east == 1) {
			temp_tr_matrix = m4_identity();
			temp_tr_matrix = m4_mult(ew_scaling_matrix, temp_tr_matrix);
			mat4 temp_mat4 = translate(8, 1.0, z);
			temp_tr_matrix = m4_mult(temp_mat4, temp_tr_matrix);

			for (int j = 0; j < 36; j++) {
				maze_vertices[vIndex] = m4_mult_vec(temp_tr_matrix, cube_vertices[j]);
				maze_colors[vIndex] = cube_colors[j];
				vIndex++;
			}
		}

		z += 2;
	}

	// Draw the posts
	temp_tr_matrix = m4_identity();
	mat4 post_scaling_matrix = scale(post_scaling_factor_x, post_scaling_factor_y, post_scaling_factor_z);

	x = -8;
	z = -8;

	for (row = 0; row < 9; row++)
	{
		for (column = 0; column < 9; column++)
		{
			temp_tr_matrix = m4_identity();
			temp_tr_matrix = m4_mult(post_scaling_matrix, temp_tr_matrix);
			mat4 temp_mat4 = translate(x, 1.0, z);
			temp_tr_matrix = m4_mult(temp_mat4, temp_tr_matrix);

			for (int j = 0; j < 36; j++) {
				maze_vertices[vIndex] = m4_mult_vec(temp_tr_matrix, cube_vertices[j]);
				maze_colors[vIndex] = post_colors[j];
				vIndex++;
			}

			x += 2;
		}

		x = -8;
		z += 2;
	}
	
	// Calculate point on the circle that the camera will circle around
	GLfloat angle_radian = degree_to_radian*angle;

	// Calculate the distance from the center of the maze to the eye
	GLfloat r = sqrt(start_eye_x*start_eye_x + start_eye_y * start_eye_y + start_eye_z * start_eye_z);

	eye_x = r * -cos(angle_radian);
	eye_z = r * sin(angle_radian);

	model_view = look_at(eye_x, start_eye_y, eye_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	mat4 frustum_matrix = frustum(fx_neg, fx_pos, fy_neg, fy_pos, fz_pos, fz_neg);
	projection = frustum_matrix;

	// Calculate the center of the screen
	mid_width_x = floor(screen_width / 2.0);
	mid_height_y = floor(screen_height / 2.0);

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(maze_vertices) + sizeof(maze_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(maze_vertices), maze_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(maze_vertices), sizeof(maze_colors), maze_colors);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(maze_vertices));

	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	projection_location = glGetUniformLocation(program, "projection_matrix");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.58, 0.937, 1.0);
	glDepthRange(1, 0);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, maze_num_vertices);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	// Terminate program
	if (key == 'q')
		exit(0);

	// Press space bar start/stop animation
	if (key == ' ' && enableIdle == 0) {
		enableIdle = 1;
	}
	else if (key == ' ' && enableIdle == 1) {
		enableIdle = 0;
	}

	glutPostRedisplay();
}

void idle(void)
{
	if (enableIdle)
	{
		if (fly_around_counter < num_times_fly_around_maze) {

			// Continually change at point around circle
			// Calculate point on the circle that the camera will circle around
			GLfloat angle_radian = degree_to_radian*angle;

			GLfloat r = sqrt(start_eye_x * start_eye_x + start_eye_y * start_eye_y + start_eye_z * start_eye_z);

			eye_x = r * -cos(angle_radian);
			eye_z = r * sin(angle_radian);

			model_view = look_at(eye_x, start_eye_y, eye_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Slow down idle because it was spinning too fast
			Sleep(10);

			angle += angle_increment;

			if (angle >= 333) {
				angle = -27.0f;
				fly_around_counter++;
			}
		}
		
		if (fly_around_counter >= num_times_fly_around_maze && done_turn_to_entrance == 0) {
			vec4 starting_eye_point = { eye_x, start_eye_y, eye_z, 1 };
			vec4 starting_at_point = { 0, 0, 0, 1 };

			vec4 entrance_eye_point = { entrance_eye_x, entrance_eye_y, entrance_eye_z, 1 };
			vec4 entrance_at_point = { entrance_at_x, entrance_at_y, entrance_at_z, 1 };

			vec4 eye_vector = v4_sub(entrance_eye_point, starting_eye_point);
			vec4 at_vector = v4_sub(entrance_at_point, starting_at_point);

			vec4 current_eye_point = v4_add(v4_scalar_mult(eye_vector, curr_turning_angle), starting_eye_point);
			vec4 current_at_point = v4_add(v4_scalar_mult(at_vector, curr_turning_angle), starting_at_point);

			current_eye_x = current_eye_point.x;
			current_eye_y = current_eye_point.y;
			current_eye_z = current_eye_point.z;

			current_at_x = current_at_point.x;
			current_at_y = current_at_point.y;
			current_at_z = current_at_point.z;

			if (curr_turning_angle <= 1.0f) {

				model_view = look_at(current_eye_x, current_eye_y, current_eye_z, current_at_x, current_at_y, current_at_z, 0.0f, 1.0f, 0.0f);

				curr_turning_angle += turning_speed;

			}
			else {
				// Set that animation of moving the eye from above the maze to the entrance is done
				done_turn_to_entrance = 1;

				// Set value to indicate that maze is ready to be solve
				solve_maze = 1;
			}
		}

		if (solve_maze == 1 && move_counter < num_moves) {
			vec4 curr_eye_point = { current_eye_x, current_eye_y, current_eye_z, 1 };
			vec4 curr_at_point = { current_at_x, current_at_y, current_at_z, 1 };

			vec4 target_eye_point = { next_eye_array[move_counter].x, next_eye_array[move_counter].y, next_eye_array[move_counter].z, 1 };
			vec4 target_at_point = { next_at_array[move_counter].x, next_at_array[move_counter].y, next_at_array[move_counter].z, 1 };

			vec4 eye_vector = v4_sub(target_eye_point, curr_eye_point);
			vec4 at_vector = v4_sub(target_at_point, curr_at_point);

			vec4 turning_eye_point = v4_add(v4_scalar_mult(eye_vector, curr_distance), curr_eye_point);
			vec4 turning_at_point = v4_add(v4_scalar_mult(at_vector, curr_distance), curr_at_point);

			next_eye_x = turning_eye_point.x;
			next_eye_y = turning_eye_point.y;
			next_eye_z = turning_eye_point.z;

			next_at_x = turning_at_point.x;
			next_at_y = turning_at_point.y;
			next_at_z = turning_at_point.z;

			if (curr_distance <= 1.0f) {
				model_view = look_at(next_eye_x, next_eye_y, next_eye_z, next_at_x, next_at_y, next_at_z, 0.0f, 1.0f, 0.0f);

				curr_distance += maze_solve_speed;

			}
			else {
				curr_distance = 0.001f;
				move_counter++;
				
				current_eye_x = turning_eye_point.x;
				current_eye_y = turning_eye_point.y;
				current_eye_z = turning_eye_point.z;

				current_at_x = turning_at_point.x;
				current_at_y = turning_at_point.y;
				current_at_z = turning_at_point.z;
			}
		}

		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(window_position_width, window_position_height);
	glutCreateWindow("Project 2: Viewing by Jason Ly");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}

// Function to change the size of the cubes 
mat4 scale(float sx, float sy, float sz) {
	mat4 result;
	result.x.x = sx; result.y.x = 0;  result.z.x = 0;  result.w.x = 0;
	result.x.y = 0;  result.y.y = sy; result.z.y = 0;  result.w.y = 0;
	result.x.z = 0;	 result.y.z = 0;  result.z.z = sz; result.w.z = 0;
	result.x.w = 0;	 result.y.w = 0;  result.z.w = 0;  result.w.w = 1;
	return result;
}

mat4 translate(float dx, float dy, float dz) {
	mat4 result;
	result.x.x = 1; result.y.x = 0; result.z.x = 0;  result.w.x = dx;
	result.x.y = 0; result.y.y = 1; result.z.y = 0;  result.w.y = dy;
	result.x.z = 0;	result.y.z = 0; result.z.z = 1;  result.w.z = dz;
	result.x.w = 0;	result.y.w = 0; result.z.w = 0;  result.w.w = 1;
	return result;
}

mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat atx, GLfloat aty, GLfloat atz, GLfloat upx, GLfloat upy, GLfloat upz) {
	vec4 e = { eyex, eyey, eyez, 0 };
	vec4 a = { atx, aty, atz, 0 };
	vec4 vup = { upx, upy, upz, 0 };

	vec4 vpn = v4_sub(e, a);
	vec4 n = v4_unit_vec(vpn);
	vec4 u = v4_unit_vec(v4_cross_prod(vup, n));
	vec4 v = v4_unit_vec(v4_cross_prod(n, u));
	vec4 p = { eyex, eyey, eyez, 1 };

	mat4 m;
	m.x.x = u.x; m.y.x = u.y; m.z.x = u.z; m.w.x = u.w;
	m.x.y = v.x; m.y.y = v.y; m.z.y = v.z; m.w.y = v.w;
	m.x.z = n.x; m.y.z = n.y; m.z.z = n.z; m.w.z = n.w;
	m.x.w = p.x; m.y.w = p.y; m.z.w = p.z; m.w.w = p.w;

	mat4 m_transpose = m4_transpose(m);
	mat4 m_transpose_inverse = m4_inverse(m_transpose);
	
	return m_transpose_inverse;
}

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near_val, GLfloat far_val) {
	mat4 result;
	result.x.x = -(near_val / right); result.y.x = 0;				  result.z.x = 0;											result.w.x = 0;
	result.x.y = 0;					  result.y.y = -(near_val / top); result.z.y = 0;										    result.w.y = 0;
	result.x.z = 0;					  result.y.z = 0;				  result.z.z = (near_val + far_val) / (far_val - near_val); result.w.z = -(2 * near_val * far_val) / (far_val - near_val);
	result.x.w = 0;					  result.y.w = 0;				  result.z.w = -1.0f;									    result.w.w = 0;
	return result;
}
