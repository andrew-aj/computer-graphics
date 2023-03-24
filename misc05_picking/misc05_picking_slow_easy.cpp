// Include standard headers
#include "glm/detail/type_mat.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <array>
#include <stack>   
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

const int window_width = 1024, window_height = 768;

typedef struct Vertex {
	float Position[4];
	float Color[4];
	float Normal[3];
	void SetPosition(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = 1.0;
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
	void SetNormal(float *coords) {
		Normal[0] = coords[0];
		Normal[1] = coords[1];
		Normal[2] = coords[2];
	}
};

struct Transform {
	glm::vec3 transform;
	glm::vec3 rotation;

	Transform* parent = nullptr;
};

// function prototypes
int initWindow(void);
void initOpenGL(void);
void createVAOs(Vertex[], GLushort[], int);
void loadObject(char*, glm::vec4, Vertex* &, GLushort* &, int);
void createObjects(void);
void pickObject(void);
void renderScene(void);
void cleanup(void);
static void keyCallback(GLFWwindow*, int, int, int, int);
static void mouseCallback(GLFWwindow*, int, int, int);

// GLOBAL VARIABLES
GLFWwindow* window;

glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;

GLuint gPickedIndex = -1;
std::string gMessage;

GLuint programID;
GLuint pickingProgramID;

const GLuint NumObjects = 6;	// ATTN: THIS NEEDS TO CHANGE AS YOU ADD NEW OBJECTS
GLuint VertexArrayId[NumObjects];
GLuint VertexBufferId[NumObjects];
GLuint IndexBufferId[NumObjects];

// TL
size_t VertexBufferSize[NumObjects];
size_t IndexBufferSize[NumObjects];
size_t NumIdcs[NumObjects];
size_t NumVerts[NumObjects];

GLuint MatrixID;
GLuint ModelMatrixID;
GLuint ViewMatrixID;
GLuint ProjMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorID;
GLuint LightID;

// Declare global objects
// TL
const size_t CoordVertsCount = 6;
Vertex CoordVerts[CoordVertsCount];

Vertex gridVerts[121];
GLushort gridIndices[440];

Vertex* outObjVertices[4];
GLushort* outObjIndices[4];

const float radius = 17.32;
float theta = 0.96;
float phi = 0.79;

glm::vec3 basePos(0);

bool moveCam = false;
bool cPressed = false;
bool tPressed = false;
bool rPressed = false;

bool translateBase = false;
bool rotateBase = false;
bool rotateArm1 = false;
bool rotateArm2 = false;

glm::vec3 baseToArm1(0, 0.8898, 0);
glm::vec3 arm1ToJoint(0, 1.537-0.8898, 0);
glm::vec3 jointToArm2(-.10272, 2.0653-1.537, .31095);
glm::vec3 arm2Rot(62.3, -9.79, 16.1);

Transform base {{0., 0., 0.}, {0., 0., 0.}};
Transform arm1 {{0, 0.8898, 0}, {0, 0, 0}, &base};
Transform joint {{0, 0.6472, 0}, {0,0,0}, &arm1};
Transform arm2 {{-.10272, 0.5283, 0.31095}, {glm::radians(62.3), glm::radians(-9.79), glm::radians(16.1)}, &joint};

glm::mat4 baseMat(1);
glm::mat4 arm1Mat(1);
glm::mat4 jointMat(1);
glm::mat4 arm2Mat(1);

void reset() {
	baseMat = glm::mat4(1.0);
	arm1Mat = glm::mat4(1.0);
	jointMat = glm::mat4(1.0);
	arm2Mat = glm::mat4(1.0);
}

void transform(glm::mat4& model, Transform* curr) {
	while(curr != nullptr) {
		glm::quat rot(curr->rotation);
		model = glm::translate(glm::mat4(1), curr->transform) * glm::toMat4(rot) * model;
		curr = curr->parent;
	}
}

void setModels() {
	transform(baseMat, &base);
	transform(arm1Mat, &arm1);
	transform(jointMat, &joint);
	transform(arm2Mat, &arm2);
}

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
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// FOR MAC

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "Knee,Andrew(39081329)", NULL, NULL);
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

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Picking");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Last picked object", TW_TYPE_STDSTRING, &gMessage, NULL);

	// Set up inputs
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}

glm::vec3 toCartesian() {
	glm::vec3 val;
	val.x = radius * std::sin(phi) * std::cos(theta);
	val.z = radius * std::sin(phi) * std::sin(theta);
	val.y = radius * std::cos(phi);
	return val;
}

void initOpenGL(void) {
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	gProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	gViewMatrix = glm::lookAt(toCartesian(),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("Picking.vertexshader", "Picking.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ProjMatrixID = glGetUniformLocation(programID, "P");

	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	// Get a handle for our "pickingColorID" uniform
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// TL
	// Define objects
	createObjects();

	setModels();

	// ATTN: create VAOs for each of the newly created objects here:
	VertexBufferSize[0] = sizeof(CoordVerts);
	NumVerts[0] = CoordVertsCount;

	VertexBufferSize[1] = sizeof(gridVerts);
	NumVerts[1] = 121;
	IndexBufferSize[1] = sizeof(gridIndices);
	NumIdcs[1] = 440;

	loadObject((char*)"../models/base.obj", {1.0, 0.0, 0.0, 1.0}, outObjVertices[0], outObjIndices[0], 2);
	loadObject((char*)"../models/arm1.obj", {0.0, 0.0, 1.0, 1.0}, outObjVertices[1], outObjIndices[1], 3);
	loadObject((char*)"../models/joint.obj", {1.0, 0.0, 1.0, 1.0}, outObjVertices[2], outObjIndices[2], 4);
	loadObject((char*)"../models/arm2.obj", {0.0, 0.5, 1.0, 1.0}, outObjVertices[3], outObjIndices[3], 5);

	createVAOs(CoordVerts, NULL, 0);
	createVAOs(gridVerts, gridIndices, 1);
	createVAOs(outObjVertices[0], outObjIndices[0], 2);
	createVAOs(outObjVertices[1], outObjIndices[1], 3);
	createVAOs(outObjVertices[2], outObjIndices[2], 4);
	createVAOs(outObjVertices[3], outObjIndices[3], 5);
}

void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId) {
	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);
	const size_t Normaloffset = sizeof(Vertices[0].Color) + RgbOffset;

	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);
	glBindVertexArray(VertexArrayId[ObjectId]);

	// Create Buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create Buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)Normaloffset);	// TL

	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);	// normal

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

// Ensure your .obj files are in the correct format and properly loaded by looking at the following function
void loadObject(char* file, glm::vec4 color, Vertex* &out_Vertices, GLushort* &out_Indices, int ObjectId) {
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(file, vertices, normals);

	std::vector<GLushort> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, normals, indices, indexed_vertices, indexed_normals);

	const size_t vertCount = indexed_vertices.size();
	const size_t idxCount = indices.size();

	// populate output arrays
	out_Vertices = new Vertex[vertCount];
	for (int i = 0; i < vertCount; i++) {
		out_Vertices[i].SetPosition(&indexed_vertices[i].x);
		out_Vertices[i].SetNormal(&indexed_normals[i].x);
		out_Vertices[i].SetColor(&color[0]);
	}
	out_Indices = new GLushort[idxCount];
	for (int i = 0; i < idxCount; i++) {
		out_Indices[i] = indices[i];
	}

	// set global variables!!
	NumIdcs[ObjectId] = idxCount;
	VertexBufferSize[ObjectId] = sizeof(out_Vertices[0]) * vertCount;
	IndexBufferSize[ObjectId] = sizeof(GLushort) * idxCount;
}

void createObjects(void) {
	//-- COORDINATE AXES --//
	CoordVerts[0] = { { 0.0, 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	CoordVerts[1] = { { 5.0, 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	CoordVerts[2] = { { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	CoordVerts[3] = { { 0.0, 5.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	CoordVerts[4] = { { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	CoordVerts[5] = { { 0.0, 0.0, 5.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0 } };
	
	//-- GRID --//
	
	// ATTN: Create your grid vertices here!
	for(int z = 0; z <= 10; z++) {
		for(int x = 0; x <= 10; x++) {
			gridVerts[x + z * 11] = {{x - 5.0f, 0.0, z - 5.0f, 1.0}, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}};
		}
	}

	int x = -5;
	int z = -5;
	for(int index = 0; index < 220;) {
		if(x == 5) {
			z++;
			x = -5;
		}
		gridIndices[index++] = (x++ + 5) + (z + 5) * 11;
		gridIndices[index++] = (x + 5) + (z + 5) * 11;
	}

	x = -5;
	z = -5;
	for(int index = 220; index < 440;) {
		if(z == 5) {
			x++;
			z = -5;
		}
		gridIndices[index++] = (x + 5) + (z++ + 5) * 11;
		gridIndices[index++] = (x + 5) + (z + 5) * 11;
	}

	//-- .OBJs --//

	// ATTN: Load your models here through .obj files -- example of how to do so is as shown
	// Vertex* Verts;
	// GLushort* Idcs;
	// loadObject("models/base.obj", glm::vec4(1.0, 0.0, 0.0, 1.0), Verts, Idcs, ObjectID);
	// createVAOs(Verts, Idcs, ObjectID);
}

void pickObject(void) {
	// Clear the screen in white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pickingProgramID);
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
		glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		// ATTN: DRAW YOUR PICKING SCENE HERE. REMEMBER TO SEND IN A DIFFERENT PICKING COLOR FOR EACH OBJECT BEFOREHAND
		glBindVertexArray(0);
	}
	glUseProgram(0);
	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	unsigned char data[4];
	glReadPixels(xpos, window_height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

	// Convert the color back to an integer ID
	gPickedIndex = int(data[0]);

	if (gPickedIndex == 255) { // Full white, must be the background !
		gMessage = "background";
	}
	else {
		std::ostringstream oss;
		oss << "point " << gPickedIndex;
		gMessage = oss.str();
	}

	// Uncomment these lines to see the picking shader in effect
	//glfwSwapBuffers(window);
	//continue; // skips the normal rendering
}

void renderScene(void) {
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	// Re-clear the screen for real rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glm::mat4x4 ModelMatrix = glm::mat4(1.0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[0]);	// Draw CoordAxes
		glDrawArrays(GL_LINES, 0, NumVerts[0]);

		glBindVertexArray(VertexArrayId[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[1]);
		glDrawElements(GL_LINES, NumIdcs[1], GL_UNSIGNED_SHORT, (void*)0);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &baseMat[0][0]);
		glBindVertexArray(VertexArrayId[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[2]);
		glDrawElements(GL_TRIANGLES, NumIdcs[2], GL_UNSIGNED_SHORT, (void*)0);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &arm1Mat[0][0]);
		glBindVertexArray(VertexArrayId[3]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[3]);
		glDrawElements(GL_TRIANGLES, NumIdcs[3], GL_UNSIGNED_SHORT, (void*)0);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &jointMat[0][0]);
		glBindVertexArray(VertexArrayId[4]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[4]);
		glDrawElements(GL_TRIANGLES, NumIdcs[4], GL_UNSIGNED_SHORT, (void*)0);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &arm2Mat[0][0]);
		glBindVertexArray(VertexArrayId[5]);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[5]);
		glDrawElements(GL_TRIANGLES, NumIdcs[5], GL_UNSIGNED_SHORT, (void*)0);

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

const float tau = 2 * 3.14159;
const float mpi = 3.14159;

uint8_t heldStatus = 0;

glm::vec3 up(0, 1, 0);

void handleArrow() {
	if(heldStatus & 1) {
		theta += 0.01;
	} else if(heldStatus & 2) {
		theta -= 0.01;
	} else if(heldStatus & 4) {
		phi -= 0.01;
	} else if(heldStatus & 8){
		phi += 0.01;
	}

	if(phi < -mpi)
		phi = mpi;
	else if(phi > mpi)
		phi = -mpi;

	if(phi < 0)
		up = glm::vec3(0, -1, 0);
		// phi = 0.001;
	else if(phi > mpi)
		up = glm::vec3(0, -1, 0);
		// phi = mpi - 0.001;
	else
		up = glm::vec3(0, 1, 0);

	if(theta < 0)
		theta = tau;
	else if(theta > tau)
		theta = 0;


	gViewMatrix = glm::lookAt(toCartesian(),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		up);	// up
}

void moveBase() {
	if(heldStatus & 1) {
		base.transform.z += 0.01;
	} else if(heldStatus & 2) {
		base.transform.z -= 0.01;
	} else if(heldStatus & 4) {
		base.transform.x -= 0.01;
	} else if(heldStatus & 8){
		base.transform.x += 0.01;
	}

	reset();
	setModels();
}

void rotateBaseFunc() {
	if(heldStatus & 1) {
		base.rotation.z += 0.01;
	} else if(heldStatus & 2) {
		base.rotation.z -= 0.01;
	}

	reset();
	setModels();
}

// Alternative way of triggering functions on keyboard events
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// ATTN: MODIFY AS APPROPRIATE
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_A:
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_C:
			if(!cPressed) {
				moveCam = !moveCam;
				cPressed = true;
			}
			break;
		case GLFW_KEY_T:
			if(!tPressed) {
				translateBase = !translateBase;
				tPressed = true;
			}
			break;
		case GLFW_KEY_R:
			if(!rPressed) {
				rotateBase = !rotateBase;
				rPressed = true;
			}
			break;
		case GLFW_KEY_LEFT:
			heldStatus |= 1;
			break;
		case GLFW_KEY_RIGHT:
			heldStatus |= 2;
			break;
		case GLFW_KEY_UP:
			heldStatus |= 4;
			break;
		case GLFW_KEY_DOWN:
			heldStatus |= 8;
			break;
		case GLFW_KEY_SPACE:
			break;
		default:
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_C:
				if(cPressed) {
					cPressed = false;
				}
				break;
			case GLFW_KEY_T:
				if(tPressed) {
					tPressed = false;
				}
				break;
			case GLFW_KEY_R:
				if(rPressed) {
					rPressed = false;
				}
			break;
			case GLFW_KEY_LEFT:
				heldStatus &= 0x1110;
				break;
			case GLFW_KEY_RIGHT:
				heldStatus &= 0x1101;
				break;
			case GLFW_KEY_UP:
				heldStatus &= 0x1011;
				break;
			case GLFW_KEY_DOWN:
				heldStatus &= 0x0111;
				break;
			default:
				break;
		}
	}
}

// Alternative way of triggering functions on mouse click events
static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickObject();
	}
}

int main(void) {
	// TL
	// ATTN: Refer to https://learnopengl.com/Getting-started/Transformations, https://learnopengl.com/Getting-started/Coordinate-Systems,
	// and https://learnopengl.com/Getting-started/Camera to familiarize yourself with implementing the camera movement

	// ATTN (Project 3 only): Refer to https://learnopengl.com/Getting-started/Textures to familiarize yourself with mapping a texture
	// to a given mesh

	// Initialize window
	int errorCode = initWindow();
	if (errorCode != 0)
		return errorCode;

	// Initialize OpenGL pipeline
	initOpenGL();

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// DRAWING POINTS
		if(moveCam)
			handleArrow();
		if(translateBase)
			moveBase();
		renderScene();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}
