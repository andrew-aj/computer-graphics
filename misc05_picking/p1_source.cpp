// Include standard headers
#include "glm/detail/type_vec.hpp"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Include AntTweakBar
#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

// ATTN 1A is the general place in the program where you have to change the code base to satisfy a Task of Project 1A.
// ATTN 1B for Project 1B. ATTN 1C for Project 1C. Focus on the ones relevant for the assignment you're working on.

typedef struct Vertex {
	float Position[4];
	float Color[4];
	void SetCoords(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = coords[3];
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
};

// ATTN: use POINT structs for cleaner code (POINT is a part of a vertex)
// allows for (1-t)*P_1+t*P_2  avoiding repeat for each coordinate (x,y,z)
typedef struct point {
	float x, y, z;
	point(const float x = 0, const float y = 0, const float z = 0) : x(x), y(y), z(z){};
	point(float *coords) : x(coords[0]), y(coords[1]), z(coords[2]){};
	point operator -(const point& a) const {
		return point(x - a.x, y - a.y, z - a.z);
	}
	point operator +(const point& a) const {
		return point(x + a.x, y + a.y, z + a.z);
	}
	point operator *(const float& a) const {
		return point(x * a, y * a, z * a);
	}
	point operator /(const float& a) const {
		return point(x / a, y / a, z / a);
	}

	//function tries to return array allocated on stack so seg fault
	// float* toArray() {
	// 	float array[] = { x, y, z, 1.0f };
	// 	return array;
	// }

	void toArray(float* arr){
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		arr[3] = 1.0f;
	}
};

// Function prototypes
int initWindow(void);
void initOpenGL(void);
void createVAOs(Vertex[], GLushort[], size_t, size_t, int);
void createObjects(void);
void pickVertex(void);
void moveVertex(void);
void renderScene(void);
void cleanup(void);
static void mouseCallback(GLFWwindow*, int, int, int);

// GLOBAL VARIABLES
GLFWwindow* window;
const GLuint window_width = 1024, window_height = 768;

glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;
glm::mat4 shiftedView;
glm::mat4 sideViewMatrix;

// Program IDs
GLuint programID;
GLuint pickingProgramID;

// Uniform IDs
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorArrayID;
GLuint pickingColorID;

GLuint gPickedIndex;
std::string gMessage;

// ATTN: INCREASE THIS NUMBER AS YOU CREATE NEW OBJECTS
const GLuint NumObjects = 5; // Number of objects types in the scene

// Keeps track of IDs associated with each object
GLuint VertexArrayId[NumObjects];
GLuint VertexBufferId[NumObjects];
GLuint IndexBufferId[NumObjects];

size_t VertexBufferSize[NumObjects];
size_t IndexBufferSize[NumObjects];
size_t NumVerts[NumObjects];	// Useful for glDrawArrays command
size_t NumIdcs[NumObjects];	// Useful for glDrawElements command


// Initialize ---  global objects -- not elegant but ok for this project
const size_t IndexCount = 8;
Vertex Vertices[IndexCount];
GLushort Indices[IndexCount];
Vertex BBCoeff[IndexCount];
GLushort BBIndices[IndexCount];
std::vector<Vertex> Lines[NumObjects - 2];

int SubLevel = 0;
int NumSubVerts = IndexCount * std::exp2(5);
int tempSubSize = IndexCount;
const int sz = 50;

// ATTN: DON'T FORGET TO INCREASE THE ARRAY SIZE IN THE PICKING VERTEX SHADER WHEN YOU ADD MORE PICKING COLORS
float pickingColor[IndexCount];

float oldColor[4];
bool pressed = false;

bool shiftPressed = false;
bool fourPressed = false;
bool showSplit = false;

int initWindow(void) {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

	// ATTN: Project 1A, Task 0 == Change the name of the window
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "Knee,Andrew(3908-1329)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Initialize the GUI display
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Picking");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Last picked object", TW_TYPE_STDSTRING, &gMessage, NULL);

	// Set up inputs
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}

void createAllVAOs() {
	createVAOs(Vertices, Indices, sizeof(Vertices), sizeof(Indices), 0);
	createVAOs(Lines[0].data(), nullptr, sizeof(Vertex) * Lines[0].size(), 0, 1);
	createVAOs(Lines[1].data(), nullptr, sizeof(Vertex) * Lines[1].size(), 0, 2);
	createVAOs(Lines[2].data(), nullptr, sizeof(Vertex) * Lines[2].size(), 0, 3);
	createVAOs(BBCoeff, BBIndices, sizeof(BBCoeff), sizeof(BBIndices), 4);
}

void initOpenGL(void) {
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for Project 1, use an ortho camera :
	gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	gViewMatrix = glm::lookAt(
		glm::vec3(0, 0, -5), // Camera is at (0,0,-5) below the origin, in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
	);

	shiftedView = glm::lookAt(glm::vec3(0, -1.5f, -5),
							  glm::vec3(0, -1.5f,  0),
							  glm::vec3(0,  1,  0));

	sideViewMatrix = glm::lookAt(glm::vec3(0, 5, 1.5f),
								 glm::vec3(0, 0, 1.5f),
								 glm::vec3(0, 0, 1));

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("p1_StandardShading.vertexshader", "p1_StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("p1_Picking.vertexshader", "p1_Picking.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	
	// Get a handle for our "pickingColorID" uniform
	pickingColorArrayID = glGetUniformLocation(pickingProgramID, "PickingColorArray");
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");

	// Define pickingColor array for picking program
	// use a for-loop here
	for(int i = 0; i < IndexCount; i++) {
		pickingColor[i] = i / 255.0f;
	}

	Lines[0].resize(IndexCount);
	Lines[1].resize(NumSubVerts);
	Lines[2].resize((sz + 1) * IndexCount);

	// Define objects
	createObjects();

	// ATTN: create VAOs for each of the newly created objects here:
	// for several objects of the same type use a for-loop
	int obj = 0;  // initially there is only one type of object 
	VertexBufferSize[obj] = sizeof(Vertices);
	IndexBufferSize[obj] = sizeof(Indices);
	NumIdcs[obj] = IndexCount;

	VertexBufferSize[4] = sizeof(BBCoeff);
	IndexBufferSize[4] = sizeof(BBIndices);
	NumIdcs[4] = IndexCount;

	VertexBufferSize[1] = sizeof(Vertex) * Lines[0].size();
	VertexBufferSize[2] = sizeof(Vertex) * Lines[1].size();
	VertexBufferSize[3] = sizeof(Vertex) * Lines[2].size();

	createAllVAOs();
}

// this actually creates the VAO (structure) and the VBO (vertex data buffer)
void createVAOs(Vertex Vertices[], GLushort Indices[], size_t BufferSize, size_t IdxBufferSize, int ObjectId) {
	NumVerts[ObjectId] = IdxBufferSize / (sizeof(GLubyte));
	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);

	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);
	glBindVertexArray(VertexArrayId[ObjectId]);

	// Create buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);

	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color

	// Disable our Vertex Buffer Object 
	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
		);
	}
}



void dcAlg() {
	float arr[4];
	for(int index = 0; index < IndexCount; index++) {
		for(int j = 0; j <= sz; j++) {
			float t = j / (double)sz;
			point cs[3][3];
			cs[2][0] = point(BBCoeff[index].Position);
			cs[1][1] = point(Vertices[index].Position);
			cs[0][2] = point(BBCoeff[(index + 1) % IndexCount].Position);

			for(int l = 1; l < 3; l++) {
				for(int i = 0; i < 3 - l; i++) {
					cs[2-l-i][i] = (cs[2-l+1-i][i] * (1 - t)) + (cs[2-l-i][i+1] * t);
				}
			}

			cs[0][0].toArray(arr);
			Lines[2][index * (sz + 1) + j].SetCoords(arr);
		}
	}
}

void createObjects(void) {
	// ATTN: DERIVE YOUR NEW OBJECTS HERE:  each object has
	// an array of vertices {pos;color} and
	// an array of indices (no picking needed here) (no need for indices)
	// ATTN: Project 1A, Task 1 == Add the points in your scene
	float arc = 2 * 3.14159f / IndexCount;
	for(int i = 0; i < IndexCount; i++) {
		float offset = i * arc;
		float color = (float) i / IndexCount;
		Vertices[i] = { { std::cos(offset), std::sin(offset), 0.0f, 1.0f }, { 0, 1, 0, 1.0f } };
		Indices[i] = i;
	}

	for(int i = 0; i < IndexCount; i++) {
		point first(Vertices[i - 1 == -1 ? IndexCount - 1 : i - 1].Position);
		point second(Vertices[i].Position);

		float arr[4];
		((first + second) / 2.0f).toArray(arr);
		BBCoeff[i].SetCoords(arr);
		float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
		BBCoeff[i].SetColor(color);

		BBIndices[i] = i;
	}

	// ATTN: Project 1B, Task 1 == create line segments to connect the control points
	float color[] = {1.0f,1.0f,1.0f,1.0f};
	for(int i = 0; i < IndexCount; i++) {
		Lines[0][i].SetCoords(Vertices[i].Position);
		Lines[0][i].SetColor(color);
	}

	float subColor[] = {0.0f, 0.25f, 1.0f, 1.0f};
	for(int i = 0; i < IndexCount; i++) {
		Lines[1][i].SetCoords(Vertices[i].Position);
		Lines[1][i].SetColor(subColor);
	}

	for(int i = 0; i < Lines[2].size(); i++) {
		Lines[2][i].SetColor(color);
	}

	dcAlg();

	// ATTN: Project 1B, Task 2 == create the vertices associated to the smoother curve generated by subdivision

	// ATTN: Project 1B, Task 4 == create the BB control points and apply De Casteljau's for their corresponding for each piece

	// ATTN: Project 1C, Task 3 == set coordinates of yellow point based on BB curve and perform calculations to find
	// the tangent, normal, and binormal
}

std::vector<point> k1;

void recalculateSubdivde(int Level) {
	float subColor[] = {0.0f, 0.25f, 1.0f, 1.0f};
	if(Level == 0) {
		for(int i = 0; i < IndexCount; i++) {
			Lines[1][i].SetCoords(Vertices[i].Position);
			Lines[1][i].SetColor(subColor);
		}
		tempSubSize = IndexCount * sizeof(Vertex);


		return;
	}

	k1.clear();
	int tempSize = IndexCount * std::exp2(Level -1);
	for(int i = 0; i < tempSize; i++) {
		k1.push_back(Lines[1][i].Position);
	}

	tempSize = IndexCount * std::exp2(Level);
	tempSubSize = tempSize * sizeof(Vertex);


	int i_2 = 0;
	for(int i = 0; i < tempSize; i += 2) {
		float arr[4];
		((k1[i_2] * 3.0f + k1[(i_2+1) % k1.size()])/4.0f).toArray(arr);
		Lines[1][i].SetCoords(arr);
		Lines[1][i].SetColor(subColor);
		((k1[i_2] + k1[(i_2+1) % k1.size()] * 3.0f)/4.0f).toArray(arr);
		Lines[1][i+1].SetCoords(arr);
		Lines[1][i+1].SetColor(subColor);

		i_2++;
	}
}

void subdivide(void) {
	if(SubLevel > 5)
		SubLevel = 0;

	for(int i = 0; i <= SubLevel; i++) {
		recalculateSubdivde(i);
	}
}

void pickVertex(void) {
	// Clear the screen in white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pickingProgramID);
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0); // initialization
		// ModelMatrix == TranslationMatrix * RotationMatrix;
		glm::mat4 MVP;
		// if(shiftPressed)
		// 	MVP = gProjectionMatrix * sideViewMatrix * ModelMatrix;
		// else
		if(showSplit)
			MVP = gProjectionMatrix * shiftedView * ModelMatrix;
		else
			MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		// MVP should really be PVM...
		// Send the MVP to the shader (that is currently bound)
		// as data type uniform (shared by all shader instances)
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		// pass in the picking color array to the shader
		glUniform1fv(pickingColorArrayID, IndexCount, pickingColor);

		// --- enter vertices into VBO and draw
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBindVertexArray(VertexArrayId[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferSize[0], Vertices);	// update buffer data
		glDrawElements(GL_POINTS, NumIdcs[0], GL_UNSIGNED_SHORT, (void*)0);
		glBindVertexArray(0);
	}
	glUseProgram(0);
	glFlush();
	// --- Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// --- Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	unsigned char data[4];  // 2x2 pixel region
	glReadPixels(xpos, window_height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
       	// window_height - ypos;  
	// OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

	// Convert the color back to an integer ID
	gPickedIndex = int(data[0]);
	for(int i = 0; i < 4; i++) {
		oldColor[i] = Vertices[gPickedIndex].Color[i];
		Vertices[gPickedIndex].Color[i] = 1.0f;
	}

	pressed = true;

	// ATTN: Project 1A, Task 2
	// Find a way to change color of selected vertex and
	// store original color

	// Uncomment these lines if you wan to see the picking shader in effect
	// glfwSwapBuffers(window);
	// continue; // skips the visible rendering
}

void restoreVertex(void) {
	if(gPickedIndex < IndexCount) {
		for(int i = 0; i < 4; i++) {
			Vertices[gPickedIndex].Color[i] = oldColor[i];
		}
	}
	pressed = false;
}

void updateBBCoeff(void) {
	for(int i = 0; i < IndexCount; i++) {
		point first(Vertices[i - 1 == -1 ? IndexCount - 1 : i - 1].Position);
		point second(Vertices[i].Position);

		float arr[4];
		((first + second) / 2.0f).toArray(arr);
		BBCoeff[i].SetCoords(arr);
	}
}

// ATTN: Project 1A, Task 3 == Retrieve your cursor position, get corresponding world coordinate, and move the point accordingly

// ATTN: Project 1C, Task 1 == Keep track of z coordinate for selected point and adjust its value accordingly based on if certain
// buttons are being pressed
double prevX, prevY;

void moveVertex(void) {
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::vec4 vp = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (gPickedIndex >= IndexCount) { 
		// Any number > vertices-indices is background!
		gMessage = "background";
	}
	else {
		std::ostringstream oss;
		oss << "point " << gPickedIndex;
		gMessage = oss.str();

		if(pressed) {
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			glm::mat4 MV;
			if(showSplit)
				MV = shiftedView * ModelMatrix;
			else
				MV = gViewMatrix * ModelMatrix;
			glm::vec3 coords = glm::unProject(glm::vec3(xpos, window_height - ypos, 0.0), MV, gProjectionMatrix, glm::vec4(0.0, 0.0, window_width, window_height));
			if(!shiftPressed) {
				Vertices[gPickedIndex].Position[0] = coords[0];
				Vertices[gPickedIndex].Position[1] = coords[1];

				Lines[0][gPickedIndex].Position[0] = coords[0];
				Lines[0][gPickedIndex].Position[1] = coords[1];

				subdivide();
				updateBBCoeff();
				dcAlg();
			} else {
				glm::mat4 movement = glm::lookAt(glm::vec3(0, 5, 0),
								 glm::vec3(0, 0, 0),
								 glm::vec3(1, 0, 0));
				MV = gViewMatrix * ModelMatrix;
				coords = glm::unProject(glm::vec3(xpos, window_height - ypos, 0.0), MV, gProjectionMatrix, glm::vec4(0.0, 0.0, window_width, window_height));
				Vertices[gPickedIndex].Position[2] = -coords[0];
				Lines[0][gPickedIndex].Position[2] = -coords[0];
				subdivide();
				updateBBCoeff();
				dcAlg();
			}
		}
	}

	prevX = xpos;
	prevY = ypos;
}

bool hideBB = false;

void drawObjects(glm::mat4& MVP) {
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			if(showSplit)
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &sideViewMatrix[0][0]);
			else
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);

			glEnable(GL_PROGRAM_POINT_SIZE);

			glBindVertexArray(VertexArrayId[0]);	// Draw Vertices
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferSize[0], Vertices);		// Update buffer data
			glDrawElements(GL_POINTS, NumIdcs[0], GL_UNSIGNED_SHORT, (void*)0);


		// // If don't use indices
		// glDrawArrays(GL_POINTS, 0, NumVerts[0]);
			for(int i = 1; i < NumObjects - 1; i++) {
				if(i != 3 || !hideBB) {
					glBindVertexArray(VertexArrayId[i]);
					glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[i]);
					if(i == 2) {
						glBufferSubData(GL_ARRAY_BUFFER, 0, tempSubSize, Lines[i-1].data());
						glDrawArrays(GL_LINE_LOOP, 0, tempSubSize / sizeof(Vertex));
					}
					else {
						glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferSize[i], Lines[i-1].data());
						glDrawArrays(GL_LINE_LOOP, 0, Lines[i-1].size());
					}
				}
			}

			if(!hideBB){
				glBindVertexArray(VertexArrayId[4]);	// Draw Vertices
				glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[4]);
				glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferSize[4], BBCoeff);		// Update buffer data
				glDrawElements(GL_POINTS, NumIdcs[4], GL_UNSIGNED_SHORT, (void*)0);
			}
}

void renderScene(void) {    
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Re-clear the screen for visible rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
		// see comments in pick
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP;

		if(showSplit) {
			MVP = gProjectionMatrix * sideViewMatrix * ModelMatrix;
			drawObjects(MVP);
			MVP = gProjectionMatrix * shiftedView * ModelMatrix;
			drawObjects(MVP);
		} else {
			MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
			drawObjects(MVP);
		}

		glViewport(0, 0, window_width, window_height);

		// ATTN: OTHER BINDING AND DRAWING COMMANDS GO HERE
		// one set per object:
		// glBindVertexArray(VertexArrayId[<x>]); etc etc

		// ATTN: Project 1C, Task 2 == Refer to https://learnopengl.com/Getting-started/Transformations and
		// https://learnopengl.com/Getting-started/Coordinate-Systems - draw all the objects associated with the
		// curve twice in the displayed fashion using the appropriate transformations

		glBindVertexArray(0);
	}
	glUseProgram(0);
	// Draw GUI
	TwDraw();

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void cleanup(void) {
	// Cleanup VBO and shader
	for (int i = 0; i < NumObjects; i++) {
		glDeleteBuffers(1, &VertexBufferId[i]);
		glDeleteBuffers(1, &IndexBufferId[i]);
		glDeleteVertexArrays(1, &VertexArrayId[i]);
	}
	glDeleteProgram(programID);
	glDeleteProgram(pickingProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

// Alternative way of triggering functions on mouse click and keyboard events
static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickVertex();
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		restoreVertex();
	}
}

bool notPressed = true;
bool notPressed2 = true;

int main(void) {
	// ATTN: REFER TO https://learnopengl.com/Getting-started/Creating-a-window
	// AND https://learnopengl.com/Getting-started/Hello-Window to familiarize yourself with the initialization of a window in OpenGL

	// Initialize window
	int errorCode = initWindow();
	if (errorCode != 0)
		return errorCode;

	// ATTN: REFER TO https://learnopengl.com/Getting-started/Hello-Triangle to familiarize yourself with the graphics pipeline
	// from setting up your vertex data in vertex shaders to rendering the data on screen (everything that follows)

	// Initialize OpenGL pipeline
	initOpenGL();

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	createObjects();	// re-evaluate curves in case vertices have been moved
	do {
		// Timing 
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// DRAGGING: move current (picked) vertex with cursor
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			moveVertex();
		}

		// ATTN: Project 1B, Task 2 and 4 == account for key presses to activate subdivision and hiding/showing functionality
		if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && notPressed) {
			SubLevel++;
			subdivide();
			notPressed = false;
		}else if(glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
			notPressed = true;
		}

		if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && notPressed2) {
			hideBB = !hideBB;
			notPressed2 = false;
		} else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE){
			notPressed2 = true;
		}

		if((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) && !shiftPressed) {
			shiftPressed = true;
		} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) {
			shiftPressed = false;
		}

		if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !fourPressed) {
			fourPressed = true;
			showSplit = !showSplit;
		} else if(glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) {
			fourPressed = false;
		}
		// for respective tasks

		// DRAWING the SCENE
		renderScene();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}
