#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>
// termianda 
//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Descomentar
#include "Headers/Texture.h"

int screenWidth;
int screenHeight;

GLFWwindow * window;

Shader shader;
//Descomentar El shader de texturizado
Shader shaderTexture;
// Descomentar El shader para iluminacion
Shader shaderColorLighting;
Shader shaderTextureLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());



Sphere sphere21(20, 20);
Sphere sphere22(20, 20);
Sphere sphere23(20, 20);
Sphere sphereLamp(20, 20);


Cylinder cylinder11(20, 20, 0.5, 0.5);
Cylinder cylinder12(20, 20, 0.5, 0.5);

Box box21;
Box box22;
Box box23;
Box boxC;//añadimos esto

//bob
Box box1;
Box box2;
Box box3;
Box box4;
Cylinder cylinder2(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder3(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder1(20, 20, 0.5, 0.5);// esfera de radio 0.5
Sphere sphere1(20, 20);
Sphere sphere3(20, 20);
Sphere sphere2(20, 20);
Sphere sphere4(20, 20);
Sphere sphere5(20, 20);
Sphere sphere6(20, 20);
Sphere sphere8(20, 20);



GLuint textureID1, textureID2, textureID3, textureID4;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderTexture.initialize("../Shaders/texturizado_res.vs", "../Shaders/texturizado_res.fs");
	// Descomentar
	shaderColorLighting.initialize("../Shaders/iluminacion_color.vs",
		"../Shaders/iluminacion_color.fs"); //ILUMINACION SHADER

	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/iluminacion_texture_res.fs"); //ILUMINACION SHADER

	// Inicializar los buffers VAO, VBO, EBO
	sphere21.init();
	// Método setter que colocar el apuntador al shader
	sphere21.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere21.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere22.init();
	// Método setter que colocar el apuntador al shader
	sphere22.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere22.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Descomentar
	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder11.init();
	cylinder11.setShader(&shaderColorLighting);
	cylinder11.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder12.init();
	cylinder12.setShader(&shaderTexture);

	

	box21.init();
	// Settea el shader a utilizar
	box21.setShader(&shaderTextureLighting);
	box21.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box22.init();
	box22.setShader(&shaderTexture);

	sphere23.init();
	sphere23.setShader(&shaderTexture);

	boxC.init();
	boxC.setShader(&shaderTexture);

	box23.init();
	box23.setShader(&shaderTextureLighting);
	box23.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
	// BOB
	box1.init();
	/*settea el shader a utilizar*/
	box1.setShader(&shaderTexture);
	box1.setShader(&shaderTextureLighting);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4((1.0, 1.0, 1.0, 1.0)));
	box2.setShader(&shaderColorLighting);
	box3.init();
	box3.setShader(&shader);
	box3.setColor(glm::vec4((0.1f, 0.0f, 0.0f, 0.0f)));

	cylinder1.init();
	cylinder1.setShader(&shaderTextureLighting);
	cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setShader(&shaderColorLighting);
	cylinder2.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	cylinder3.init();
	cylinder3.setShader(&shader);
	cylinder3.setShader(&shaderColorLighting);
	cylinder3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	sphere1.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere1.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere1.setShader(&shaderTexture);
	sphere1.setShader(&shaderTextureLighting);
	sphere1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	sphere2.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere2.setShader(&shader);
	 sphere2.setShader(&shaderColorLighting);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere3.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere3.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere3.setShader(&shaderColorLighting);
	sphere3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	sphere4.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere4.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere4.setShader(&shaderColorLighting);
	sphere4.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere5.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere5.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere5.setShader(&shaderColorLighting);
	sphere5.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	//inicializar los buffers VAO,VBO,EBO
	sphere6.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere6.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere6.setShader(&shaderColorLighting);
	sphere6.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	sphere8.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere8.setShader(&shader);
	sphere8.setShader(&shaderColorLighting);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere8.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	box4.init();
	box4.setShader(&shader);
	box4.setShader(&shaderColorLighting);
	box4.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere21.destroy();
	cylinder11.destroy();
	box21.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.0001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.0001;

	sentido = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al nuevo shader
		shaderTexture.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Descomentar
		shaderColorLighting.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderColorLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix, glm::vec3(-0.2f, 0.0f, -ratio));
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.render(lightModelmatrix);


		shaderTextureLighting.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderTextureLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		//Descomentar
		// Usamos la textura ID 1
		/*
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box21.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		// Articulacion 1
		glm::mat4 jJ1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		jJ1 = glm::rotate(jJ1, rot1, glm::vec3(0, 0, 1));
		jJ1 = glm::rotate(jJ1, rot2, glm::vec3(0, 1, 0));
		sphere21.enableWireMode();
		sphere21.render(glm::scale(jJ1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 lL1 = glm::translate(jJ1, glm::vec3(0.25f, 0.0, 0.0));
		lL1 = glm::rotate(lL1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder11.render(glm::scale(lL1, glm::vec3(0.1, 0.5, 0.1)));

		// Articulacion 2
		glm::mat4 jJ2 = glm::translate(jJ1, glm::vec3(0.5, 0.0f, 0.0f));
		jJ2 = glm::rotate(jJ2, rot3, glm::vec3(0.0, 0.0, 1.0));
		jJ2 = glm::rotate(jJ2, rot4, glm::vec3(1.0, 0.0, 0.0));
		sphere21.enableWireMode();
		sphere21.render(glm::scale(jJ2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 lL2 = glm::translate(jJ2, glm::vec3(0.25, 0.0, 0.0));
		lL2 = glm::rotate(lL2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder11.render(glm::scale(lL2, glm::vec3(0.1, 0.5, 0.1)));

		// Ojo
		glm::mat4 ojoO = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere21.render(glm::scale(ojoO, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 ojOo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere2.enableWireMode();
		sphere22.render(glm::scale(ojOo2, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		box21.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);

		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		sphere23.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder,
			glm::vec3(-3.0, 0.0, 0.0));
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		// Se usa la textura 1 ( Bon sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder12.render(0, cylinder12.getSlices() * cylinder12.getStacks() * 6,
			modelCylinder);
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder12.render(cylinder12.getSlices() * cylinder12.getStacks() * 6,
			cylinder12.getSlices() * 3, modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder12.render(
			cylinder12.getSlices() * cylinder12.getStacks() * 6
			+ cylinder12.getSlices() * 3, cylinder12.getSlices() * 3,
			modelCylinder);
		glBindTexture(GL_TEXTURE_2D, 0);
		**/
		// BOB
		//box1.enableWireMode();
		//Descomentar
		//usamos la textura ID 1 
		glBindTexture(GL_TEXTURE_2D, textureID1);
		shaderTexture.setFloat("offsetX", 0);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		//camisa
		//box2.enableWireMode();// DIMESION CAJA 1.0
		glm::mat4 c2 = glm::translate(model, glm::vec3(0.0, -0.5, 0.01));

		box2.render(glm::scale(c2, glm::vec3(1.0, 0.04, 0.1)));
		//pantalon
		glm::mat4 c3 = glm::translate(c2, glm::vec3(0.0, -0.11, 0.0));
		box3.render(glm::scale(c3, glm::vec3(1.0, 0.25, 0.1)));

		glm::mat4 c4 = glm::translate(c3, glm::vec3(-0.3, -0.1, 0.0));
		box3.render(glm::scale(c4, glm::vec3(0.11, 0.13, 0.0)));

		glm::mat4 c5 = glm::translate(c3, glm::vec3(0.3, -0.1, 0.0));
		box3.render(glm::scale(c5, glm::vec3(0.11, 0.13, 0.0)));

		// pie derecho
		glm::mat4 p1 = glm::translate(c5, glm::vec3(0.01, -0.19, 0.0));
		p1 = glm::rotate(p1, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(p1, glm::vec3(0.048, 0.25, 0.048)));
		// pie izquierdo
		glm::mat4 p2 = glm::translate(c4, glm::vec3(0.01, -0.19, 0.0));
		p2 = glm::rotate(p2, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(p2, glm::vec3(0.048, 0.25, 0.048)));




		//corbata
		glm::mat4 ci1 = glm::translate(model, glm::vec3(0.0, -0.5, 0.09));
		cylinder2.render(glm::scale(ci1, glm::vec3(0.1, 0.07, 0.1)));

		//articulacion mano derech hombro

		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere2.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));


		//para mano derecha
		glBindTexture(GL_TEXTURE_2D, textureID1);
		shaderTexture.setFloat("offsetX", 0);
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.1f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l1, glm::vec3(0.05, 0.4, 0.05)));


		//articulacion mano derecha codo
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.3, 0.0, 0.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1, 0, 0));
		//j2 = glm::rotate(j2, rot2, glm::vec3(0.0, 0.0, 1.0));
		sphere1.render(glm::scale(j2, glm::vec3(0.07, 0.07, 0.1)));


		//mano izquierda 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.01, -0.18, 0.0));
		l2 = glm::rotate(l2, glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0));
		cylinder1.render(glm::scale(l2, glm::vec3(0.048, 0.4, 0.048)));

		//articulacion mano izquierda hombro
		glm::mat4 j3 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		j3 = glm::rotate(j3, rot1, glm::vec3(1, 0, 0));
		j3 = glm::rotate(j3, rot2, glm::vec3(0.0, 0.0, 1.0));
		sphere2.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));


		//para mano izquierda
		glm::mat4 l3 = glm::translate(j3, glm::vec3(-0.1f, 0.0, 0.0));
		l3 = glm::rotate(l3, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l3, glm::vec3(0.05, 0.4, 0.05)));
		//articulacion mano izquierda codo3
		glm::mat4 j4 = glm::translate(j3, glm::vec3(-0.3, 0.0, 0.0));

		j4 = glm::rotate(j4, rot3, glm::vec3(0.0, 0.0, 1.0));
		sphere1.render(glm::scale(j4, glm::vec3(0.07, 0.07, 0.1)));
		//mano derecha 2
		glm::mat4 l4 = glm::translate(j4, glm::vec3(-0.01, -0.18, 0.0));
		l4 = glm::rotate(l4, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(l4, glm::vec3(0.048, 0.4, 0.048)));


		//ojo 2
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.15, 0.15, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo, glm::vec3(0.3, 0.3, 0.1)));

		//ojo2 pequeño parte negra
		glm::mat4 ojo1 = glm::translate(model, glm::vec3(0.15, 0.15, 0.09));
		//sphere3.enableWireMode();
		sphere3.render(glm::scale(ojo1, glm::vec3(0.08, 0.08, 0.05)));

		//ojo2 pequeño parte azul
		glm::mat4 ojo5 = glm::translate(model, glm::vec3(0.15, 0.15, 0.08));
		//sphere3.enableWireMode();
		sphere6.render(glm::scale(ojo5, glm::vec3(0.15, 0.15, 0.05)));

		//ojo1
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.05));
		//sphere1.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.3, 0.3, 0.1)));
		//ojo1 pequeño parte negra
		glm::mat4 ojo3 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.09));
		//sphere3.enableWireMode();
		sphere4.render(glm::scale(ojo3, glm::vec3(0.08, 0.08, 0.05)));

		//ojo1 pequeño parte azul
		glm::mat4 ojo4 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.08));
		//sphere3.enableWireMode();
		sphere5.render(glm::scale(ojo4, glm::vec3(0.15, 0.15, 0.05)));
		// boca
		//sphere1.enableWireMode();

		glm::mat4 boca = glm::translate(model, glm::vec3(0.0, -0.2, 0.08));
		sphere8.render(glm::scale(boca, glm::vec3(0.2, 0.09, 0.0)));
		// dientes
		glm::mat4 d1 = glm::translate(boca, glm::vec3(-0.02, 0.03, 0.0));
		box4.render(glm::scale(d1, glm::vec3(0.025, 0.03, 0.01)));

		glm::mat4 d2 = glm::translate(boca, glm::vec3(0.02, 0.03, 0.0));
		box4.render(glm::scale(d2, glm::vec3(0.025, 0.03, 0.01)));
		// Zapatos
		glm::mat4 z1 = glm::translate(p1, glm::vec3(0.0, -0.15, 0.0));
		box3.render(glm::scale(z1, glm::vec3(0.1, 0.05, 0.2)));

		glm::mat4 z2 = glm::translate(p2, glm::vec3(0.0, -0.15, 0.0));
		box3.render(glm::scale(z2, glm::vec3(0.1, 0.05, 0.2)));
		//nariz
		glm::mat4 n1 = glm::translate(boca, glm::vec3(0.0f, 0.2, 0.05));
		cylinder1.render(glm::scale(n1, glm::vec3(0.05, 0.05, 0.2)));

		//pestañas
		glm::mat4 pe1 = glm::translate(ojo2, glm::vec3(0.0f, 0.16, 0.05));
		cylinder3.render(glm::scale(pe1, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe2 = glm::translate(ojo2, glm::vec3(-0.06f, 0.155, 0.05));
		cylinder3.render(glm::scale(pe2, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe3 = glm::translate(ojo2, glm::vec3(0.06f, 0.155, 0.05));
		cylinder3.render(glm::scale(pe3, glm::vec3(0.05, 0.05, 0.2)));

		// pestañas 2
		glm::mat4 pe4 = glm::translate(ojo1, glm::vec3(0.0f, 0.16, 0.0));
		cylinder3.render(glm::scale(pe4, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe5 = glm::translate(ojo1, glm::vec3(-0.06f, 0.15, 0.0));
		cylinder3.render(glm::scale(pe5, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe6 = glm::translate(ojo1, glm::vec3(0.06f, 0.15, 0.0));
		cylinder3.render(glm::scale(pe6, glm::vec3(0.05, 0.05, 0.2)));

		// mano derecha
		glm::mat4 mm1 = glm::translate(l4, glm::vec3(0.0, -0.2, 0.0));
		sphere1.render(glm::scale(mm1, glm::vec3(0.08, 0.1, 0.1)));
		// mano inzquierda
		glm::mat4 mm2 = glm::translate(l2, glm::vec3(0.0, -0.2, 0.0));
		sphere1.render(glm::scale(mm2, glm::vec3(0.08, 0.1, 0.1)));

		//DEDOS MANO DERECHA
		glm::mat4 dd1 = glm::translate(mm1, glm::vec3(0.0, -0.04, 0.0));
		dd1 = glm::rotate(dd1, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd1, glm::vec3(0.009, 0.09, 0.009)));

		glm::mat4 dd2 = glm::translate(mm1, glm::vec3(-0.03, -0.035, 0.0));
		dd2 = glm::rotate(dd2, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd2, glm::vec3(0.009, 0.085, 0.009)));


		glm::mat4 dd3 = glm::translate(mm1, glm::vec3(0.03, -0.035, 0.0));
		dd3 = glm::rotate(dd3, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd3, glm::vec3(0.009, 0.085, 0.009)));

		glm::mat4 dd4 = glm::translate(mm1, glm::vec3(-0.02, 0.01, 0.0));
		dd4 = glm::rotate(dd4, glm::radians(130.0f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd4, glm::vec3(0.009, 0.12, 0.009)));

		glm::mat4 dd5 = glm::translate(mm1, glm::vec3(0.02, 0.01, 0.0));
		dd5 = glm::rotate(dd5, glm::radians(-130.0f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd5, glm::vec3(0.009, 0.12, 0.009)));
		// DEDOS MANO IZQUIERDA
		glm::mat4 dd6 = glm::translate(mm2, glm::vec3(0.0, -0.04, 0.0));
		dd6 = glm::rotate(dd6, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd6, glm::vec3(0.009, 0.09, 0.009)));

		glm::mat4 dd7 = glm::translate(mm2, glm::vec3(-0.03, -0.035, 0.0));
		dd7 = glm::rotate(dd7, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd7, glm::vec3(0.009, 0.085, 0.009)));


		glm::mat4 dd8 = glm::translate(mm2, glm::vec3(0.03, -0.035, 0.0));
		dd8 = glm::rotate(dd8, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd8, glm::vec3(0.009, 0.085, 0.009)));

		glm::mat4 dd9 = glm::translate(mm2, glm::vec3(-0.02, 0.01, 0.0));
		dd9 = glm::rotate(dd9, glm::radians(130.0f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd9, glm::vec3(0.009, 0.12, 0.009)));

		glm::mat4 dd10 = glm::translate(mm2, glm::vec3(0.02, 0.01, 0.0));
		dd10 = glm::rotate(dd10, glm::radians(-130.0f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(dd10, glm::vec3(0.009, 0.12, 0.009)));


		

		glm::mat4 cubeTextureModel = glm::mat4(1.0);
		cubeTextureModel = glm::translate(cubeTextureModel, glm::vec3(3.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		boxC.render(cubeTextureModel);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		shader.turnOff();

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
