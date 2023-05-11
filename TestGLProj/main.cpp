#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

//internal classes
#include "Model.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "NoiseGenerator.h"
#include "Shader.h"
#include "ImGuiManager.h"

//OS classes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Shader shader; // loads our vertex and fragment shaders
Shader perlinShader;
Model *plane; //a plane

glm::mat4 projection; // projection matrix
glm::mat4 view; // where the camera is looking
glm::mat4 model; // where the model (i.e., the myModel) is located wrt the camera

float screenWidth = 1920.0f;
float screenHeight = 1080.0f;

int mouseX, mouseY;
int prevMouseX, prevMouseY;
bool middleMouseButtonPressed = false;
bool leftMouseButtonPressed = false;

CameraMode currentMode = CameraMode::THIRD_PERSON;
Camera camera(glm::vec3(-20.0f, 20.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool m_isWireframe = false;

const int terrain_width = 100;
const float terrain_scale = 10.0f;

bool m_usePerlinNoise = false;
bool m_showPerlinNoiseWindow = false;
int perlin_seed = 0;
int perlin_octaves = 4;
float perlin_persistence = 0.5f;
float perlin_amplitude = 1.0f;
float perlin_lacunarity = 2.0f;
float perlin_frequency = 0.03f;
float perlin_scale = 1.0f;
std::vector<float> perlinNoise;

bool m_useVoronoiNoise = false;
bool m_showVoronoiNoiseWindow = false;
int voronoi_points = 40;
int voronoi_octaves = 4;
float voronoi_lacunarity = 50.0f;
float voronoi_gain = 0.01f;
float voronoi_jitter = 0.01f;
float voronoi_scale = 100.0f;
float voronoi_amplitude = 4.0f;
int voronoi_seed = 0;
std::vector<float> voronoiNoise;

bool m_useThermalErosion = false;
bool m_showThermalErosionWindow = false;
int thermNumIterations = 0;
float thermalTalusAngle = 30.0f;
float dt = 0.001f;

//bool m_useHydroErosion = false;
//int hydroNumIterations = 0;
//float waterAmount = 0.1f;
//float hydroTalusAngle = 30.0f;
//float c = 0.1f;
//float m = 0.5f;


std::vector<float> result;

TerrainGenerator* terrain;
NoiseGenerator* noise;
ImGuiManager* im_gui;


/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Update the noise generator parameters
    im_gui->startFrame("Terrain Edditor");

	// Perlin Noise Window
	if (ImGui::Checkbox("Use Perlin Noise", &m_usePerlinNoise)) {
		// Toggle visibility of the Perlin Noise window
		m_showPerlinNoiseWindow = m_usePerlinNoise;
	}

	if (m_showPerlinNoiseWindow) {
		ImGui::Begin("Perlin Noise");
		ImGui::SliderInt("Seed", &perlin_seed, 0, 10000);
		ImGui::SliderInt("Octaves", &perlin_octaves, 1, 10);
		ImGui::SliderFloat("Persistence", &perlin_persistence, 0.0f, 1.0f);
		ImGui::SliderFloat("Amplitude", &perlin_amplitude, 0.0f, 10.0f);
		ImGui::SliderFloat("Lacunarity", &perlin_lacunarity, 1.0f, 10.0f);
		ImGui::SliderFloat("Frequency", &perlin_frequency, 0.001f, 0.999f, "%.3f");
		ImGui::SliderFloat("Scale", &perlin_scale, 0.0f, 10.0f);
		ImGui::End();
	}

	// Voronoi Noise Window
	if (ImGui::Checkbox("Use Voronoi Noise", &m_useVoronoiNoise)) {
		// Toggle visibility of the Voronoi Noise window
		m_showVoronoiNoiseWindow = m_useVoronoiNoise;
	}

	if (m_showVoronoiNoiseWindow) {
		ImGui::Begin("Voronoi Noise");
		ImGui::SliderInt("Voronoi Seed", &voronoi_seed, 0, 10000);
		ImGui::SliderInt("Voronoi Number Of Points", &voronoi_points, 0.0f, 50.0f);
		ImGui::SliderInt("Voronoi Octaves", &voronoi_octaves, 1, 10);
		ImGui::SliderFloat("Voronoi Lacunarity", &voronoi_lacunarity, 0.0f, 50.0f);
		ImGui::SliderFloat("Voronoi Gain", &voronoi_gain, 0.001f, 1.0f, "%.3f");
		ImGui::SliderFloat("Voronoi Jitter", &voronoi_jitter, 0.01f, 0.1f, "%.3f");
		ImGui::SliderFloat("Voronoi Scale", &voronoi_scale, 1.0f, 100.0f);
		ImGui::SliderFloat("Voronoi Amplitude", &voronoi_amplitude, 0.1f, 10.0f, "%.2f");

		if (ImGui::Button("Regenerate Voronoi")) {
			voronoiNoise = noise->generateVoronoi();
		}

		ImGui::End();
	}

	// Thermal Erosion Window
	if (ImGui::Checkbox("Use Thermal Erosion", &m_useThermalErosion)) {
		// Toggle visibility of the Thermal Erosion window
		m_showThermalErosionWindow = m_useThermalErosion;
	}

	if (m_showThermalErosionWindow) {
		ImGui::Begin("Thermal Erosion");
		if (ImGui::Button("Apply Erosion"))
		{
			terrain->thermalErosion(thermNumIterations, thermalTalusAngle, dt);
		}
		ImGui::SliderInt("Iterations", &thermNumIterations, 0, 10000);
		ImGui::SliderFloat("Talus Angle", &thermalTalusAngle, 30.0f, 45.0f);
		ImGui::SliderFloat("Amt of Mat Transported", &dt, 0.001f, 0.01f, "%.3f");

		ImGui::End();
	}

    im_gui->endFrame();

	im_gui->update();

	noise->setSeed(perlin_seed);
	noise->setOctaves(perlin_octaves);
	noise->setPersistence(perlin_persistence);
	noise->setAmplitude(perlin_amplitude);
	noise->setLacunarity(perlin_lacunarity);
	noise->setFrequency(perlin_frequency);
	noise->setScale(perlin_scale);

	noise->setVoronoiPointCount(voronoi_points);
	noise->setVoronoiOctaves(voronoi_octaves);
	noise->setVoronoiLacunarity(voronoi_lacunarity);
	noise->setVoronoiGain(voronoi_gain);
	noise->setVoronoiJitter(voronoi_jitter);
	noise->setVoronoiScale(voronoi_scale);
	noise->setVoronoiAmplitude(voronoi_amplitude);
	noise->setVoronoiSeed(voronoi_seed);

	result.clear(); // Clear previous noise data
	if (m_usePerlinNoise && m_useVoronoiNoise) {
		perlinNoise = noise->generatePerlinNoise(); // regenerate perlin noise
		//voronoiNoise = noise->generateVoronoi();
		result.resize(voronoiNoise.size()); // resize result to the same size as voronoiNoise
		for (int i = 0; i < voronoiNoise.size(); ++i) {
			result[i] = voronoiNoise[i] + perlinNoise[i]; // add corresponding elements together
		}
	}
	else if (m_usePerlinNoise) {
		perlinNoise = noise->generatePerlinNoise();
		result.insert(result.end(), perlinNoise.begin(), perlinNoise.end());
	}
	else if (m_useVoronoiNoise) {
		//voronoiNoise = noise->generateVoronoi();
		result.insert(result.end(), voronoiNoise.begin(), voronoiNoise.end());
	}

	terrain->applyHeightMap(result, terrain_scale);

	//if (m_useThermalErosion) {
	//	terrain->thermalErosion(thermNumIterations, thermalTalusAngle, dt);
	//}

	view = camera.getViewMatrix();

	// Define the scaling factors for the x and z directions
	float scaleX = 500.0f;
	float scaleZ = 500.0f;

	terrain->render(view, projection);

	// Flush the buffer and swap the back and front buffers
	glFlush();
	glutSwapBuffers();

	checkError ("display");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true; // Set the ImGui key state

	switch (key) {
	case 'w':
		camera.mvFw = true;

		break;
	case 's':
		camera.mvBw = true;
		break;
	case 'a':
		camera.mvLt = true;
		break;
	case 'd':
		camera.mvRt = true;
		break;
	case 32:
		if (camera.getMode() == CameraMode::THIRD_PERSON) {
			camera.mvUp = true;
		}
		break;
	case ']':
		m_isWireframe = !m_isWireframe;

		if (m_isWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;
	case 'c':
		// Cycle through the camera modes
		if (currentMode == CameraMode::DEFAULT) {
			currentMode = CameraMode::FIRST_PERSON;
		}
		else if (currentMode == CameraMode::FIRST_PERSON) {
			currentMode = CameraMode::THIRD_PERSON;
		}
		else {
			currentMode = CameraMode::DEFAULT;
		}
		camera.setMode(currentMode);
		break;
	case 27: // this is an ascii value
		exit(0);
		break;
	default:
		camera.stopMoving();
		break;
	}


	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;

	switch (key) {
	case 'w':
		camera.mvFw = false;
		break;
	case 's':
		camera.mvBw = false;
		break;
	case 'a':
		camera.mvLt = false;
		break;
	case 'd':
		camera.mvRt = false;
		break;
	case 32:
		if (camera.getMode() == CameraMode::THIRD_PERSON) {
			camera.mvUp = false;
		}
		break;
	}
}

void specialKeys(int key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	//io.KeyCtrl = io.KeysDown[GLUT_KEY_CTRL_L] || io.KeysDown[GLUT_KEY_CTRL_R]; // Set the ImGui Ctrl key state
	switch (key) {
	case GLUT_KEY_CTRL_L:
		camera.mvDw = true;
		break;

	}
}

void specialKeysUp(int key, int x, int y) {
	ImGuiIO& io = ImGui::GetIO();
	//io.KeyCtrl = nan;
	switch (key) {
	case GLUT_KEY_CTRL_L:
		camera.mvDw = false;
		break;
	}

}

void mouseButton(int button, int state, int x, int y) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[button] = (state == GLUT_DOWN); // Set the ImGui mouse button state

	if (!im_gui->isUsingMouse()) {
		// Pass input to Glut
		glutMouseFunc(mouseButton);
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			middleMouseButtonPressed = true;
			prevMouseX = x;
			prevMouseY = y;
		}
		else if (state == GLUT_UP) {
			middleMouseButtonPressed = false;
		}
	}
}

void mouseMotion(int x, int y) {
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y); // Set the ImGui mouse position

	if (!im_gui->isUsingMouse()) {
		// Pass input to Glut
		glutMouseFunc(mouseButton);
		glutMotionFunc(mouseMotion);
		glutPassiveMotionFunc(mouseMotion);
	}
	// Calculate the change in mouse position
	int dx = x - prevMouseX;
	int dy = y - prevMouseY;

	// Update the previous mouse position
	prevMouseX = x;
	prevMouseY = y;

	// Only rotate the camera when the middle mouse button is held down
	if (middleMouseButtonPressed) {
		if (camera.getMode() == CameraMode::THIRD_PERSON) {
			camera.rotateYaw(dx * 0.1f);
			camera.rotatePitch(-dy * 0.1f);
		}
	}
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	camera.updateCameraPosition(0.5f);
	glutPostRedisplay();
}

/*Called when the window is resized*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	checkError ("reshape");
	ImGui_ImplGLUT_ReshapeFunc(w, h);
}

void initCallbacks(void)
{
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeys);
	glutSpecialUpFunc(specialKeysUp);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);
}

void initShader(void)
{
	glm::vec3 lightPosition = glm::vec3(50.0f, 50.0f, 50.0f);
	glm::vec3 lightColor = glm::vec3(5.0f, 5.0f, 5.0f);
	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);

	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");
	glGetError();

	perlinShader.InitializeFromFile("shaders/perlin.vert", "shaders/perlin.frag");
	perlinShader.AddAttribute("vertexPosition");

	// Set the values of the uniform variables used in the fragment shader
	shader.Activate();
	// Set the uniform variables used in the fragment shader
	shader.SetUniform("lightPosition", lightPosition);
	shader.SetUniform("lightColor", lightColor);
	shader.SetUniform("ambientColor", ambientColor);

	// Set the uniform variables used in the vertex shader
	shader.SetUniform("Projection", projection);
	shader.SetUniform("ModelView", model);
	shader.SetUniform("heightScale", terrain_scale);
	shader.SetUniform("terrainSize", terrain_width);

	glGetError();
	checkError("initShader");
}

void initRendering(void)
{
	glClearColor(0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	checkError("initRendering");
}

void init(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Terrain Generation");

	glewInit();
	dumpInfo();

	// Perspective projection matrix.
	projection = glm::perspective(45.0f, screenWidth / screenHeight, 1.0f, 1000.0f);

	// Set the initial mouse position
	mouseX = prevMouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	mouseY = prevMouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	terrain = new TerrainGenerator(&shader, terrain_width, &perlinShader);
	terrain->generateLevelPlane();

	noise = new NoiseGenerator(terrain_width);
	voronoiNoise = noise->generateVoronoi();

	initShader();
	initRendering();
	initCallbacks();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init ();

	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glGetString(GL_VERSION);
	
	im_gui = new ImGuiManager();

	// Enter the GLUT main loop
	glutMainLoop();

	im_gui->shutdown();
	return 0;
}