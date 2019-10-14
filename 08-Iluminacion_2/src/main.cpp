#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

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

#include "Headers/Texture.h"
//Inlcude loader Model Class 
#include "Headers/Model.h"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box1;
Box box2;
Box box3;
Box  boxc1;
Box boxventana;
Box boxplanta;
GLuint textureID1, textureID2, textureID3, textureID4, /*CASA*/ textureID5, textureID6, textureID7, textureID8, textureID9, textureID10, textureID11, textureID12, textureID13, textureID14;
// Descomentar;
//Models complex instances 
Model modelRailRoad;
Model modelAircraft;
Model modelRock;
//CASA


GLuint skyboxTextureID;


GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/ame_nebula/purplenebula_bk.tga",
"../Textures/ame_nebula/purplenebula_bk.tga",
"../Textures/ame_nebula/purplenebula_ft.tga",
"../Textures/ame_nebula/purplenebula_lf.tga",
"../Textures/ame_nebula/purplenebula_rt.tga",
"../Textures/ame_nebula/purplenebula_up.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
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
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
		"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
		"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/iluminacion_texture_res.fs");
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
		"../Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
		"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderMulLighting); //solo se aplica a los objetos con tectura

	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderMulLighting); //modificamos esto
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderMulLighting);

	sphere3.init();
	sphere3.setShader(&shaderMulLighting);
	/////////////////////////////////////////////CASA ////////////////////////////////////////////////////////////////////////////
	boxc1.init();// piso
	boxc1.setShader(&shaderTextureLighting);

	boxventana.init();
	boxventana.setShader(&shaderTexture);



	boxplanta.init();
	boxplanta.setShader(&shaderTexture);




	/////////////////////////////////////TERMINA CASA ////////////////////////7


	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);
	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);
	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);





	box3.init();
	box3.setShader(&shaderMulLighting);

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

	//////////////////////////////////////////CASA//////////////////////////////
	Texture texture5("../Textures/piso.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture5.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID5);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID5);
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
	texture5.freeImage(bitmap);



	Texture texture6("../Textures/PARED.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture6.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture6.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID6);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID6);
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
	texture6.freeImage(bitmap);





	Texture texture9("../Textures/puerta.jpg");
	bitmap = texture9.loadImage(true);
	data = texture9.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture9.freeImage(bitmap);


	Texture texture8("../Textures/ventanas.PNG");
	bitmap = texture8.loadImage(true);
	data = texture8.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture8.freeImage(bitmap);



	Texture texture10("../Textures/planta.PNG");
	bitmap = texture10.loadImage(true);
	data = texture10.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID10);
	glBindTexture(GL_TEXTURE_2D, textureID10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture10.freeImage(bitmap);


	Texture texture7("../Textures/cesped.jpg");
	bitmap = texture7.loadImage(true);
	data = texture7.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture7.freeImage(bitmap);


	Texture texture11("../Textures/cocina.png");
	bitmap = texture11.loadImage(true);
	data = texture11.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture11.freeImage(bitmap);


	Texture texture12("../Textures/mueble1.png");
	bitmap = texture12.loadImage(true);
	data = texture12.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture12.freeImage(bitmap);



	Texture texture13("../Textures/colcha.jpg");
	bitmap = texture13.loadImage(true);
	data = texture13.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture13.freeImage(bitmap);


	Texture texture14("../Textures/maderas.jpg");
	bitmap = texture14.loadImage(true);
	data = texture14.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture14.freeImage(bitmap);




	///////////////////////////////////////////////////////////termina casa//////////////////////////77


	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
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

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con material
		shaderMaterialLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		// Propiedades de la luz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//aqio le movimos para bajar la intensidad de la luz
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0))); //sentido del vector
		// eSTO ES PARA LUZ CON SPOTLIGHT 
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getPosition()));

		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.1);
		shaderMulLighting.setFloat("spotLights[0].cuadratic", 0.1);
		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
			glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
			glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con materiales
		shaderMaterialLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		/*
				model = glm::translate(model, glm::vec3(0, 0, dz));
				model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
				//box1.enableWireMode();
				// Usamos la textura ID 1
				glBindTexture(GL_TEXTURE_2D, textureID1);
				box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
				// No utilizar ninguna textura
				glBindTexture(GL_TEXTURE_2D, 0);

				// Articulacion 1
				glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
				j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
				j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
				sphere1.enableWireMode();
				sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

				// Hueso 1
				glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
				l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
				//cylinder1.enableWireMode();
				cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

				// Articulacion 2
				glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
				j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
				j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
				sphere1.enableWireMode();
				sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

				// Hueso 2
				glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
				l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				//cylinder1.enableWireMode();
				cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

				// Ojo
				glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
				//sphere1.enableWireMode();
				sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));

				glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
				//sphere2.enableWireMode();
				sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

				glm::mat4 modelAgua = glm::mat4(1.0);
				modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
				modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
				// Se activa la textura del agua
				glBindTexture(GL_TEXTURE_2D, textureID2);
				shaderMulLighting.setFloat("offsetX", offX);
				box2.render(modelAgua);
				glBindTexture(GL_TEXTURE_2D, 0);
				shaderMulLighting.setFloat("offsetX", 0);

				glm::mat4 modelSphere = glm::mat4(1.0);
				modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
				glBindTexture(GL_TEXTURE_2D, textureID3);
				sphere3.render(modelSphere);
				glBindTexture(GL_TEXTURE_2D, 0);

				glm::mat4 modelCylinder = glm::mat4(1.0);
				modelCylinder = glm::translate(modelCylinder,
					glm::vec3(-3.0, 0.0, 0.0));
				// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
				// Se usa la textura 1 ( Bon sponja)
				glBindTexture(GL_TEXTURE_2D, textureID1);
				cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
					modelCylinder);
				// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
				// Se usa la textura 2 ( Agua )
				glBindTexture(GL_TEXTURE_2D, textureID2);
				cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
					cylinder2.getSlices() * 3, modelCylinder);
				// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
				// Se usa la textura 3 ( Goku )
				glBindTexture(GL_TEXTURE_2D, textureID3);
				cylinder2.render(
					cylinder2.getSlices() * cylinder2.getStacks() * 6
					+ cylinder2.getSlices() * 3, cylinder2.getSlices() * 3,
					modelCylinder);
				glBindTexture(GL_TEXTURE_2D, 0);

				// Render del cubo con textura de ladrillos y con repeticion en x
				glm::mat4 cubeTextureModel = glm::mat4(1.0);
				cubeTextureModel = glm::translate(cubeTextureModel,
					glm::vec3(3.0, 2.0, 3.0));
				glBindTexture(GL_TEXTURE_2D, textureID4);
				shaderTexture.setVectorFloat2("scaleUV",
					glm::value_ptr(glm::vec2(2.0, 1.0)));
				box3.render(cubeTextureModel);
				glBindTexture(GL_TEXTURE_2D, 0);

				// Render del cyindro con materiales
				glm::mat4 cylinderMaterialModel = glm::mat4(1.0);
				cylinderMaterialModel = glm::translate(cylinderMaterialModel, glm::vec3(3.0, 2.0, -3.0));
				shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
				shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
				shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
				shaderMaterialLighting.setFloat("material.shininess", 76.8f);
				cylinderMaterials.render(cylinderMaterialModel);
				//Models complex render
				glm::mat4 matrixModelRock = glm::mat4(1.0);
				matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 4.0));
				modelRock.render(matrixModelRock);
				//forze to anavle the unit texture to 9 always--------------------------IMPORTANT
				glActiveTexture(GL_TEXTURE0);
				//ESTO ES PARA LAS VIAS DEL TREN
				glm::mat4 matrixModelRailroad = glm::mat4(1.0);
				matrixModelRailroad = glm::translate(matrixModelRailroad, glm::vec3(3.0, 0.0, 4.0));
				modelRailRoad.render(matrixModelRailroad);
			// AIRCRAFT
				glm::mat4 matrixModelAircraft = glm::mat4(1.0);
				matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(-4.0, -1.0, 4.0));
				modelAircraft.render(matrixModelAircraft);
				glActiveTexture(GL_TEXTURE0);
				*/

				// Ak_47
		

		//ALBERCA
		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(17, -3.0, 2.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(6.0, 0.01, 10.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);
		// CAMA





		// PUERTAS 
		/*glm::mat4 matrixModelPuerta = glm::mat4(1.0);
		matrixModelPuerta = glm::translate(matrixModelPuerta, glm::vec3(-6.0, -4.3, -2.0));
		matrixModelPuerta = glm::scale(matrixModelPuerta, glm::vec3(0.1, 0.1, 0.1));
		modelPuerta.render(matrixModelPuerta);
		glActiveTexture(GL_TEXTURE0);*/






		/*glm::mat4 boxMaterialModel = glm::mat4(1.0f);
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-3.0, 2.0, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
		shaderMaterialLighting.setFloat("material.shininess", 76.8f);
		boxMaterials.render(boxMaterialModel);*/
		////////////////////////////////////////////////////// CASA ////////////////////////////////////////////////////////////////

		//	PISO 

		//boxc1.enableWireMode();

		glm::mat4 modelpiso = glm::mat4(1.0);
		modelpiso = glm::translate(modelpiso, glm::vec3(6.0, -3.0, 0.0));


		glm::mat4 modelpasto = glm::mat4(1.0);
		modelpasto = glm::translate(modelpasto, glm::vec3(6.0, -3.1, 0.0));



		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(6.0, 6.0)));
		boxc1.render(glm::scale(modelpiso, glm::vec3(18.0, 0.1, 30.0)));

		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(6.0, 6.0)));
		boxc1.render(glm::scale(modelpasto, glm::vec3(30.0, 0.1, 40.0)));




		//box1.enableWireMode();




		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));

		glm::mat4 pared1 = glm::translate(modelpiso, glm::vec3(-4.5, 2.0, 14));
		boxc1.render(glm::scale(pared1, glm::vec3(5.5, 4.0, 0.1)));


		glBindTexture(GL_TEXTURE_2D, textureID9);//PUERTA
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));

		glm::mat4 puerta = glm::translate(pared1, glm::vec3(4.0, 0.0, 0.0));
		boxc1.render(glm::scale(puerta, glm::vec3(4.5, 4.0, 0.1)));


		//6
		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared3 = glm::translate(modelpiso, glm::vec3(-4.5, 2.0, 8));
		boxc1.render(glm::scale(pared3, glm::vec3(5.5, 4.0, 0.1)));




		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared3a = glm::translate(pared3, glm::vec3(-2.75, -0.1, 3.0));
		boxc1.render(glm::scale(pared3a, glm::vec3(0.1, 4.2, 6.0)));









		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4 = glm::translate(modelpiso, glm::vec3(-4.5, 2.0, 2));
		boxc1.render(glm::scale(pared4, glm::vec3(5.5, 4.0, 0.1)));

		glBindTexture(GL_TEXTURE_2D, textureID8);

		glm::mat4 modelventana4 = glm::mat4(1.0); //textura PNG
		modelventana4 = glm::translate(pared4, glm::vec3(-2.75, 0.3, 4.6));
		modelventana4 = glm::scale(modelventana4, glm::vec3(0.1, 2.5, 2.8));
		boxventana.render(modelventana4);


		glm::mat4 modelventana4b = glm::mat4(1.0); //textura PNG
		modelventana4b = glm::translate(pared4, glm::vec3(-2.75, 0.3, 1.4));
		modelventana4b = glm::scale(modelventana4b, glm::vec3(0.1, 2.5, 2.8));
		boxventana.render(modelventana4b);




		glm::mat4 modelventana4c = glm::mat4(1.0); //textura PNG
		modelventana4c = glm::translate(pared4, glm::vec3(-2.75, 0.2, -1.1));
		modelventana4c = glm::scale(modelventana4c, glm::vec3(0.1, 2.8, 2.1));
		boxventana.render(modelventana4c);

		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4a = glm::translate(pared4, glm::vec3(-2.75, 1.7, 3.0));
		boxc1.render(glm::scale(pared4a, glm::vec3(0.1, 0.7, 6.0)));



		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4b = glm::translate(pared4, glm::vec3(-2.75, -1.4, 3.0));
		boxc1.render(glm::scale(pared4b, glm::vec3(0.1, 1.2, 6.0)));

		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4c = glm::translate(pared4, glm::vec3(-2.75, 0.0, 3.0));
		boxc1.render(glm::scale(pared4c, glm::vec3(0.1, 4.0, 0.6)));




		/*glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4a = glm::translate(pared4, glm::vec3(2.75, 0.0, 2.0));
		boxc1.render(glm::scale(pared4a, glm::vec3(0.01, 4.0, 4.0)));

		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared4b = glm::translate(pared4, glm::vec3(2.75, 0.0, 5.7));
		boxc1.render(glm::scale(pared4b, glm::vec3(0.01, 4.0, 0.5)));*/







		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5 = glm::translate(modelpiso, glm::vec3(-4.5, 2.0, -5));
		boxc1.render(glm::scale(pared5, glm::vec3(5.5, 4.0, 0.1)));



		// PRIMERA IZQUIERDA,AL LADO DE COMEDOR 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5a = glm::translate(pared5, glm::vec3(2.75, 0.0, 2.5));
		boxc1.render(glm::scale(pared5a, glm::vec3(0.1, 4.0, 5.0)));





		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5b = glm::translate(pared5, glm::vec3(2.75, 0.0, 6.7));
		boxc1.render(glm::scale(pared5b, glm::vec3(0.1, 4.0, 0.5)));

		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5c = glm::translate(pared5, glm::vec3(2.75, 1.50, 4.0));
		boxc1.render(glm::scale(pared5c, glm::vec3(0.1, 1.0, 6.0)));


		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5d = glm::translate(pared5, glm::vec3(-2.75, 1.8, 3.5));
		boxc1.render(glm::scale(pared5d, glm::vec3(0.1, 0.7, 7.0)));


		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5e = glm::translate(pared5, glm::vec3(-2.75, -1.5, 3.5));
		boxc1.render(glm::scale(pared5e, glm::vec3(0.1, 1.0, 7.0)));




		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5f = glm::translate(pared5, glm::vec3(-2.75, 0.0, 2.5));
		boxc1.render(glm::scale(pared5f, glm::vec3(0.1, 2.9, 5.0)));




		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared2 = glm::translate(modelpiso, glm::vec3(4.5, 2.0, 14));
		boxc1.render(glm::scale(pared2, glm::vec3(5.5, 4.0, 0.1)));


		glBindTexture(GL_TEXTURE_2D, textureID8);

		glm::mat4 modelventana2 = glm::mat4(1.0); //textura PNG
		modelventana2 = glm::translate(pared2, glm::vec3(2.75, 0.3, -3.0));
		modelventana2 = glm::scale(modelventana2, glm::vec3(0.1, 2.5, 6.0));
		boxventana.render(modelventana2);

		glBindTexture(GL_TEXTURE_2D, textureID8);

		glm::mat4 modelventana2a = glm::mat4(1.0); //textura PNG
		modelventana2a = glm::translate(pared2, glm::vec3(2.75, 0.3, -7.11));
		modelventana2a = glm::scale(modelventana2a, glm::vec3(0.1, 2.5, 2.5));
		boxventana.render(modelventana2a);

		glBindTexture(GL_TEXTURE_2D, textureID8);

		glm::mat4 modelventana2b = glm::mat4(1.0); //textura PNG
		modelventana2b = glm::translate(pared2, glm::vec3(2.75, 0.3, -9.9));
		modelventana2b = glm::scale(modelventana2b, glm::vec3(0.1, 2.5, 2.2));
		boxventana.render(modelventana2b);




		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared6 = glm::translate(modelpiso, glm::vec3(4.5, 2.0, 8));
		boxc1.render(glm::scale(pared6, glm::vec3(5.5, 4.0, 0.1)));



		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared6a = glm::translate(pared6, glm::vec3(2.75, 1.7, 3.0));
		boxc1.render(glm::scale(pared6a, glm::vec3(0.1, 0.7, 6.0)));


		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared6b = glm::translate(pared6, glm::vec3(2.75, -1.4, 3.0));
		boxc1.render(glm::scale(pared6b, glm::vec3(0.1, 1.2, 6.0)));





		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared7 = glm::translate(modelpiso, glm::vec3(4.5, 2.0, 3));
		boxc1.render(glm::scale(pared7, glm::vec3(5.5, 4.0, 0.1)));

		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared7a = glm::translate(pared7, glm::vec3(-2.71, -0.7, 2.0));
		boxc1.render(glm::scale(pared7a, glm::vec3(0.3, 2.5, 4.0)));


		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared7b = glm::translate(pared7, glm::vec3(2.75, -1.4, 3.0));
		boxc1.render(glm::scale(pared7b, glm::vec3(0.1, 1.2, 6.0)));

		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared7d = glm::translate(pared7, glm::vec3(2.75, 1.6, 2.5));
		boxc1.render(glm::scale(pared7d, glm::vec3(0.1, 0.5, 5.0)));

		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared7c = glm::translate(pared7, glm::vec3(2.75, 0.0, 2.5));
		boxc1.render(glm::scale(pared7c, glm::vec3(0.1, 4.0, 0.6)));


		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared8 = glm::translate(modelpiso, glm::vec3(4.5, 2.0, -1));
		boxc1.render(glm::scale(pared8, glm::vec3(5.5, 4.0, 0.1)));


		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared8c = glm::translate(pared8, glm::vec3(2.75, 0.0, 2.0));
		boxc1.render(glm::scale(pared8c, glm::vec3(0.1, 4.0, 4.0)));











		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared8a = glm::translate(pared8, glm::vec3(-2.75, 0.0, 2.8));
		boxc1.render(glm::scale(pared8a, glm::vec3(0.1, 4.0, 2.5)));




		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared9 = glm::translate(modelpiso, glm::vec3(4.5, 2.0, -7));
		boxc1.render(glm::scale(pared9, glm::vec3(5.5, 4.0, 0.1)));

		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared9b = glm::translate(pared9, glm::vec3(2.75, 0.0, 3.0));
		boxc1.render(glm::scale(pared9b, glm::vec3(0.1, 4.0, 6.0)));


		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared9a = glm::translate(pared9, glm::vec3(-2.75, 0.0, 2.5));
		boxc1.render(glm::scale(pared9a, glm::vec3(0.1, 4.0, 5.0)));


		//techo1




		glm::mat4 techo = glm::mat4(1.0);


		techo = glm::translate(techo, glm::vec3(5.0, 1.0, -7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(6.0, 6.0)));
		boxc1.render(glm::scale(techo, glm::vec3(23.0, 0.3, 19.0)));


		lightModelmatrix = glm::translate(techo, glm::vec3(-10.0, -0.15, -7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.3, 0.3, 0.3)));


		lightModelmatrix = glm::translate(techo, glm::vec3(-10.0, -0.15, -3.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.3, 0.3, 0.3)));

		lightModelmatrix = glm::translate(techo, glm::vec3(-10.0, -0.15, 1.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.3, 0.3, 0.3)));

		lightModelmatrix = glm::translate(techo, glm::vec3(-10.0, -0.15, 5.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.3, 0.3, 0.3)));



		//TECHO ENFRENTE

		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 techo2 = glm::translate(techo, glm::vec3(1.5, 0.0, 15));
		boxc1.render(glm::scale(techo2, glm::vec3(20, 0.3, 15)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(9.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(4.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(-1.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(-6, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		;

		lightModelmatrix = glm::translate(techo2, glm::vec3(9, -0.15, 1));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(9, -0.15, -5));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(9, -0.15, -11));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(9, -0.15, -16));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(9, -0.15, -22));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));


		//Parte arriba casa 
		/*glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 paredarriba = glm::translate(techo, glm::vec3(0.0, 2.8, -6.0));
		boxc1.render(glm::scale(paredarriba, glm::vec3(15, 5.0, 0.1)));
		*/


		//PAREDES ATRAS CASA 



		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared10 = glm::translate(modelpiso, glm::vec3(0.5, 2.0, -15));
		boxc1.render(glm::scale(pared10, glm::vec3(16.0, 4.0, 0.1)));


		glBindTexture(GL_TEXTURE_2D, textureID8);

		glm::mat4 modelventanatras = glm::mat4(1.0); //textura PNG
		modelventanatras = glm::translate(pared10, glm::vec3(-8.1, 0.0, 5));

		boxventana.render(glm::scale(modelventanatras, glm::vec3(0.1, 4.3, 10.2)));




		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(1.0, 1.0)));
		glm::mat4 pared10b = glm::translate(pared10, glm::vec3(8.0, 0.0, 4));
		boxc1.render(glm::scale(pared10b, glm::vec3(0.1, 4.0, 8.0)));


		//MUEBLE
		glBindTexture(GL_TEXTURE_2D, textureID10);//planta1
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(0, 1.0)));
		glm::mat4 modelplanta = glm::mat4(1.0); //textura PNG
		modelplanta = glm::translate(modelpiso, glm::vec3(-7.5, 1.0,14.5));

		boxplanta.render(glm::scale(modelplanta, glm::vec3(1.0, 2.0, 0.01)));

		glBindTexture(GL_TEXTURE_2D, textureID10);//planta 2
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(0, 1.0)));
		glm::mat4 modelplanta1 = glm::mat4(1.0); //textura PNG
		modelplanta1 = glm::translate(modelpiso, glm::vec3(-6.0, 0.65, -14.5));

		boxplanta.render(glm::scale(modelplanta1, glm::vec3(1.0, 2.0, 0.01)));

		glBindTexture(GL_TEXTURE_2D, textureID11);//concina
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(0, 1.0)));
		glm::mat4 modelcama = glm::mat4(1.0); //textura PNG
		modelcama = glm::translate(modelpiso, glm::vec3(4.8, 2.0, 3.2));

		boxplanta.render(glm::scale(modelcama, glm::vec3(5.0, 4.0,0.0001)));


		glBindTexture(GL_TEXTURE_2D, textureID12);//concina
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(0, 1.0)));
		glm::mat4 modelmueble1 = glm::mat4(1.0); //textura PNG
		modelmueble1 = glm::translate(modelpiso, glm::vec3(-5.0, 0.5, 1.8));

		boxplanta.render(glm::scale(modelmueble1, glm::vec3(3.0, 4.0, 0.0001)));



		//Cama



		glBindTexture(GL_TEXTURE_2D, textureID14);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 cama1 = glm::translate(modelpiso, glm::vec3(-5.0, 0.5, -2.4));
		boxc1.render(glm::scale(cama1, glm::vec3(3,0.1, 4.0)));

		glBindTexture(GL_TEXTURE_2D, textureID13);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 cama2 = glm::translate(modelpiso, glm::vec3(-5.0, 0.5, -2.4));
		boxc1.render(glm::scale(cama2, glm::vec3(2.5, 0.25, 3.5)));




		/////////////////////////////////////////////////////////////termina casa///////////////////////////////////////////////////////////////////////

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
