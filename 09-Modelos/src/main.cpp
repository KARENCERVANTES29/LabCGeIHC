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


Sphere sphereLamp(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);

Box boxMaterials;

Box  boxc1;
Box boxventana;
Box boxplanta;
Box boxalberca;
GLuint textureID1, textureID2, textureID3, textureID4, /*CASA*/ textureID5, textureID6, textureID7, textureID8, textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15;
// Descomentar;
//Models complex instances 
Model modelRailRoad;
Model modelAircraft;
Model modelRock;
Model modelarbol;
//CASA


GLuint skyboxTextureID;
////////////////////////////////////////////////////////////////////////////////////////////////////BOB ///////////////////////////////////////
Sphere sphere1(20, 20);//particiones de esfera, 20 horizontales, 20 veticales
Cylinder cylinder1(20, 20, 0.5, 0.5);// esfera de radio 0.5
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphere4(20, 20);
Sphere sphere5(20, 20);
Sphere sphere6(20, 20);
Sphere sphere7(20, 20);
Sphere sphere8(20, 20);
Sphere sphere9(20, 20);
Sphere sphere11(20, 20);

Box box1;
Box box2;
Box box3;
Box box4;
Box box5;
Cylinder cylinder2(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder3(20, 20, 0.1, 0.1);// esfera de radio 0.5
// R2-D2
Cylinder cylinder4(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder5(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder6(20, 20, 0.1, 0.1);// esfera de radio 0.5
Sphere sphere10(20, 20);
Sphere sphere12(20, 20);
Sphere sphere13(20, 20);
Sphere sphere14(20, 20);
Box box6;
Box box7;

Cylinder cylinderMaterials(20, 20, 0.5, 0.5);


float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0, rot5 = 0.0, rot6 = 0.0, rot7 = 0.0, rot8 = 0.0, rot9 = 0.0, rot10 = 0.0;
float rot0 = 0.0, dz = 0.0, rot01 = 0.0, dz2 = 0.0;



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



bool sentido = true;
bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;





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



	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));
	sphereLamp.init();
	sphereLamp.setShader(&shader);
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));


	/////////////////////////////////////////////CASA ////////////////////////////////////////////////////////////////////////////
	boxc1.init();// piso
	boxc1.setShader(&shaderMulLighting);

	boxventana.init();
	boxventana.setShader(&shaderTexture);



	boxplanta.init();
	boxplanta.setShader(&shaderTexture);


	//ALBERCA

	boxalberca.init();
	boxalberca.setShader(&shaderMulLighting);
	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	/////////////////////////////////////TERMINA CASA ////////////////////////7



	/////////////////////////////////////BOB/////////////////////////////
	sphere1.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere1.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere2.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere2.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere3.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere3.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere4.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere4.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere4.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere5.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere5.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere5.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	//inicializar los buffers VAO,VBO,EBO
	sphere6.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere6.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere6.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));


	//inicializar los buffers VAO,VBO,EBO
	sphere7.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere7.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere7.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0f));

	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0));
	//corbata
	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	//bob
	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0));
	//camisa blanca
	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4((1.0, 1.0, 1.0, 1.0)));
	// boca 
	//inicializar los buffers VAO,VBO,EBO
	sphere8.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere8.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere8.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	// dientes
	box4.init();
	box4.setShader(&shader);
	box4.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	// pestañas
	cylinder3.init();
	cylinder3.setShader(&shader);
	cylinder3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	//pantalones
	box3.init();
	box3.setShader(&shader);
	box3.setColor(glm::vec4((0.1f, 0.0f, 0.0f, 0.0f)));
	// R2-D2
	cylinder4.init();
	cylinder4.setShader(&shader);
	cylinder4.setColor(glm::vec4(0.70, 0.70, 0.70, 0.0));

	cylinder5.init();
	cylinder5.setShader(&shader);
	cylinder5.setColor(glm::vec4(0.50754, 0.50754, 0.50754, 0.0));

	cylinder6.init();
	cylinder6.setShader(&shader);
	cylinder6.setColor(glm::vec4(0.0f, 0.0f, 0.4f, 0.0f));
	sphere8.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere8.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere8.setColor(glm::vec4(0.50754, 0.50754, 0.50754, 0.0));
	//ojo 
	box5.init();
	box5.setShader(&shader);
	box5.setColor(glm::vec4((0.0f, 0.0f, 0.4f, 0.0f)));
	sphere10.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere10.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere10.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	sphere11.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere11.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere11.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	//detalles cuerpo
	box6.init();
	box6.setShader(&shader);
	box6.setColor(glm::vec4((0.329412, 0.329412, 0.329412, 0.329412)));

	box7.init();
	box7.setShader(&shader);
	box7.setColor(glm::vec4((0.70, 0.70, 0.70, 0.70)));

	sphere12.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere12.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere12.setColor(glm::vec4(0.70, 0.70, 0.70, 0.0));
	sphere13.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere13.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere13.setColor(glm::vec4(0.0f, 0.0f, 0.4f, 0.0f));

	sphere14.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere14.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere14.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));





	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);
	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);
	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	modelarbol.loadModel("../models/Generic_Trees_02_OBJ/Generic Trees 02_OBJ.obj");
	modelarbol.setShader(&shaderMulLighting);






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
	Texture texture2("../Textures/water2.jpg");
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

	Texture texture15("../Textures/puertas.jpg");
	bitmap = texture15.loadImage(true);
	data = texture15.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
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
	texture15.freeImage(bitmap);
	


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
		rot1 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.01;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && sentido)
		rot2 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !sentido)
		rot2 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !sentido)
		rot3 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !sentido)
		rot4 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && sentido)
		rot5 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !sentido)
		rot5 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && sentido)
		rot6 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !sentido)
		rot6 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		rot01 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rot01 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dz2 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dz2 -= 0.01;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz += 0.01;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && sentido)
		rot9 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && !sentido)
		rot9 -= 0.01;
	//r2d2
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && sentido)
		rot7 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !sentido)
		rot7 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && sentido)
		rot8 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && !sentido)
		rot8 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && sentido)
		rot10 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && !sentido)
		rot10 -= 0.01;
	sentido = true;
	glfwPollEvents();
	return continueApplication;
}


void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);

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
			glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(1.2, 1.2, 1.2)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(1.4, 1.4, 1.4)));

		// Propiedades de la luz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//aqio le movimos para bajar la intensidad de la luz
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0))); //sentido del vector
		//ESTO ES PARA LA LUZ SPOTLIGHT LAMPARAAAAAAAAAS
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getFront()));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(1.5, 0.7, 1.1)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.difusse", glm::value_ptr(glm::vec3(1.0, 4.0, 6.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.5, 0.3, 0.2)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(16.5)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0); //AQUI LE MUEVO PARA LA ILUMINACION
		shaderMulLighting.setFloat("spotLights[0].linear", 0.7);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.1);

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







	glm::mat4 matrixModelAircraft = glm::mat4(1.0);
	matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(-7.0, 0.0, -9.0));
	modelAircraft.render(matrixModelAircraft);
		glActiveTexture(GL_TEXTURE0);
	
		glm::mat4 matrixModelRock = glm::mat4(1.0);
		matrixModelRock = glm::translate(matrixModelRock, glm::vec3(4, -1.0,-18));
		modelRock.render(matrixModelRock);
		glActiveTexture(GL_TEXTURE0);

		//ALBERCA
		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(17, -1, 2.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(6.0, 0.01, 10.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		boxalberca.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);
	
		glm::mat4 modelpiso = glm::mat4(1.0);
		modelpiso = glm::translate(modelpiso, glm::vec3(6.0, -1.0, 0.0));
		model2 = glm::translate(model2, glm::vec3(6.0, 2, -9.0));
		model2 = glm::rotate(model, rot01, glm::vec3(0, 1, 0));

		glm::mat4 modelpasto = glm::mat4(1.0);
		modelpasto = glm::translate(modelpasto, glm::vec3(6.0, -1.1, 0.0));
		modelpasto = glm::scale(modelpasto, glm::vec3(30.0, 0.1, 40.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setFloat("offsetX", offX);
		boxalberca.render(modelpasto);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);


		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(6.0, 6.0)));
		boxc1.render(glm::scale(modelpiso, glm::vec3(18.0, 0.1, 30.0)));





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





		glBindTexture(GL_TEXTURE_2D, textureID13);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 pared5b = glm::translate(pared5, glm::vec3(2.75, 0.0, 6.7));
		boxc1.render(glm::scale(pared5b, glm::vec3(0.1, 4.0, 0.5)));

		glBindTexture(GL_TEXTURE_2D, textureID6);
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




//puertas interior
			glBindTexture(GL_TEXTURE_2D, textureID15);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 puertas2 = glm::translate(modelpiso, glm::vec3(-1.7,1.50,0.9));
		boxc1.render(glm::scale(puertas2, glm::vec3(0.01, 3.3,1.8 )));

		glBindTexture(GL_TEXTURE_2D, textureID15);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 puertas1 = glm::translate(modelpiso, glm::vec3(1.7, 1.5, -0.24));
		boxc1.render(glm::scale(puertas1, glm::vec3(0.01, 3.0, 1.6)));







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


		techo = glm::translate(techo, glm::vec3(5.0, 3.0, -7.5));
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
		glm::mat4 techo2 = glm::translate(techo, glm::vec3(1.5,0.0, 15));
		boxc1.render(glm::scale(techo2, glm::vec3(20, 0.3, 15)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(9.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(4.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));


		lightModelmatrix = glm::translate(techo2, glm::vec3(-1.0, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

		lightModelmatrix = glm::translate(techo2, glm::vec3(-6, -0.15, 7.0));
		sphereLamp.render(glm::scale(lightModelmatrix, glm::vec3(0.2, 0.2, 0.2)));

	

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
		modelplanta = glm::translate(modelpiso, glm::vec3(-7.5, 1.0, 14.5));

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

		boxplanta.render(glm::scale(modelcama, glm::vec3(5.0, 4.0, 0.0001)));


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
		boxc1.render(glm::scale(cama1, glm::vec3(3, 0.1, 4.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID13);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		glm::mat4 cama2 = glm::translate(modelpiso, glm::vec3(-5.0, 0.5, -2.4));
		boxc1.render(glm::scale(cama2, glm::vec3(2.5, 0.25, 3.5)));
		glBindTexture(GL_TEXTURE_2D, 0);


		// CILINDROS DE TECHO A PISO 
		glm::mat4 cylinderMaterialModel = glm::mat4(1.0);
		cylinderMaterialModel = glm::translate(cylinderMaterialModel, glm::vec3(-5.0, 1.0, -2.0));
		cylinderMaterialModel = glm::scale(cylinderMaterialModel, glm::vec3(0.5, 4.0, 0.5));

		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.25, 0.20725, 0.20725)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(1, 0.829, 0.829)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.296648, 0.296648, 0.296648)));
		shaderMaterialLighting.setFloat("material.shininess", 0.088);
		cylinderMaterials.render(cylinderMaterialModel);

		// CILINDROS DE TECHO A PISO 
		glm::mat4 cylinderMaterialModel2 = glm::mat4(1.0);
		cylinderMaterialModel2 = glm::translate(cylinderMaterialModel2, glm::vec3(-5.0, 1.0, -14.0));
		cylinderMaterialModel2 = glm::scale(cylinderMaterialModel2, glm::vec3(0.5, 4.0, 0.5));

		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.25, 0.20725, 0.20725)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(1, 0.829, 0.829)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.296648, 0.296648, 0.296648)));
		shaderMaterialLighting.setFloat("material.shininess", 0.088);
		cylinderMaterials.render(cylinderMaterialModel2);
		/////////////////////////////////////////////////////////////termina casa///////////////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////////////////////BOB//////////////////////////////////////////////////////////////////



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
		j1 = glm::rotate(j1, rot5, glm::vec3(1, 0, 0));
		j1 = glm::rotate(j1, rot6, glm::vec3(0.0, 0.0, 1.0));
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));


		//para mano derecha
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.1f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l1, glm::vec3(0.05, 0.4, 0.05)));

		//articulacion mano derecha codo
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.3, 0.0, 0.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1, 0, 0));
		//j2 = glm::rotate(j2, rot2, glm::vec3(0.0, 0.0, 1.0));
		sphere7.render(glm::scale(j2, glm::vec3(0.07, 0.07, 0.1)));


		//mano izquierda 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.01, -0.18, 0.0));
		l2 = glm::rotate(l2, glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0));
		cylinder1.render(glm::scale(l2, glm::vec3(0.048, 0.4, 0.048)));

		//articulacion mano izquierda hombro
		glm::mat4 j3 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		j3 = glm::rotate(j3, rot1, glm::vec3(1, 0, 0));
		j3 = glm::rotate(j3, rot2, glm::vec3(0.0, 0.0, 1.0));
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));


		//para mano izquierda
		glm::mat4 l3 = glm::translate(j3, glm::vec3(-0.1f, 0.0, 0.0));
		l3 = glm::rotate(l3, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l3, glm::vec3(0.05, 0.4, 0.05)));
		//articulacion mano izquierda codo3
		glm::mat4 j4 = glm::translate(j3, glm::vec3(-0.3, 0.0, 0.0));

		j4 = glm::rotate(j4, rot3, glm::vec3(0.0, 0.0, 1.0));
		sphere7.render(glm::scale(j4, glm::vec3(0.07, 0.07, 0.1)));
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
		sphere1.render(glm::scale(ojo2, glm::vec3(0.3, 0.3, 0.1)));
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
		sphere7.render(glm::scale(mm1, glm::vec3(0.08, 0.1, 0.1)));
		// mano inzquierda
		glm::mat4 mm2 = glm::translate(l2, glm::vec3(0.0, -0.2, 0.0));
		mm2 = glm::rotate(mm2, rot9, glm::vec3(1, 0, 0));
		sphere7.render(glm::scale(mm2, glm::vec3(0.08, 0.1, 0.1)));

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
		// HASTA AQUÍ BOB 
		// R2-D2
			// CUERPO 
		glm::mat4 cuerpo = glm::translate(model2, glm::vec3(3.0, 0.5, 0.0));
		cylinder4.render(glm::scale(cuerpo, glm::vec3(6.0, 1.5, 6.0)));

		glm::mat4 cuerpo2 = glm::translate(cuerpo, glm::vec3(0.0, 0.75, 0.0));
		cylinder5.render(glm::scale(cuerpo2, glm::vec3(6.0, 0.07, 6.0)));
		glm::mat4 cuerpo3 = glm::translate(cuerpo2, glm::vec3(0.0, 0.06, 0.0));
		cylinder6.render(glm::scale(cuerpo3, glm::vec3(6.0, 0.07, 6.0)));
		// detalles cuerpo 
		glm::mat4 cuerpo4 = glm::translate(cuerpo, glm::vec3(0.0, -0.03, 0.0));
		cylinder6.render(glm::scale(cuerpo4, glm::vec3(0.02, 0.05, 0.6)));
		glm::mat4 cuerpo5 = glm::translate(cuerpo, glm::vec3(0.0, 0.5, 0.6));
		cylinder6.render(glm::scale(cuerpo5, glm::vec3(2.7, 0.09, 0.05)));
		glm::mat4 cuerpo6 = glm::translate(cuerpo, glm::vec3(0.0, 0.4, 0.6));
		cylinder6.render(glm::scale(cuerpo6, glm::vec3(2.7, 0.09, 0.05)));
		glm::mat4 cuerpo7 = glm::translate(cuerpo, glm::vec3(0.0, 0.3, 0.6));
		cylinder6.render(glm::scale(cuerpo7, glm::vec3(2.7, 0.09, 0.05)));

		glm::mat4 cuerpo8 = glm::translate(cuerpo, glm::vec3(-0.3, 0.2, 0.5));
		cylinder6.render(glm::scale(cuerpo8, glm::vec3(0.5, 0.5, 0.05)));
		glm::mat4 cuerpo9 = glm::translate(cuerpo, glm::vec3(0.3, 0.2, 0.5));
		cylinder6.render(glm::scale(cuerpo9, glm::vec3(0.5, 0.5, 0.05)));

		glm::mat4 cuerpo10 = glm::translate(cuerpo, glm::vec3(-0.3, -0.3, 0.5));
		cylinder6.render(glm::scale(cuerpo10, glm::vec3(0.5, 0.2, 0.05)));
		glm::mat4 cuerpo11 = glm::translate(cuerpo, glm::vec3(0.3, -0.3, 0.5));
		cylinder6.render(glm::scale(cuerpo11, glm::vec3(0.5, 0.2, 0.05)));

		glm::mat4 cuerpo12 = glm::translate(cuerpo, glm::vec3(0.0, -0.2, 0.6));// recuadro
		cylinder6.render(glm::scale(cuerpo12, glm::vec3(1.1, 0.5, 0.05)));
		glm::mat4 cuerpo13 = glm::translate(cuerpo, glm::vec3(0.0, -0.03, 0.6));
		box6.render(glm::scale(cuerpo13, glm::vec3(0.09, 0.12, 0.015)));
		glm::mat4 cuerpo14 = glm::translate(cuerpo, glm::vec3(0.0, -0.3, 0.6));
		box6.render(glm::scale(cuerpo14, glm::vec3(0.09, 0.19, 0.015)));

		glm::mat4 cuerpo15 = glm::translate(cuerpo, glm::vec3(0.0, -0.6, 0.6));// recuadro 2
		cylinder6.render(glm::scale(cuerpo15, glm::vec3(0.9, 0.2, 0.05)));
		glm::mat4 cuerpo16 = glm::translate(cuerpo15, glm::vec3(0.0, 0.0, 0.0));
		sphere12.render(glm::scale(cuerpo16, glm::vec3(0.16, 0.16, 0.05)));
		glm::mat4 cuerpo17 = glm::translate(cuerpo16, glm::vec3(0.0, 0.0, 0.01));
		sphere13.render(glm::scale(cuerpo17, glm::vec3(0.11, 0.11, 0.05)));

		glm::mat4 cuerpo18 = glm::translate(cuerpo16, glm::vec3(0.0, 0.0, 0.019));
		cylinder4.render(glm::scale(cuerpo18, glm::vec3(0.05, 0.096, 0.1)));
		glm::mat4 cuerpo19 = glm::translate(cuerpo17, glm::vec3(0.0, 0.0, 0.019));
		cylinder4.render(glm::scale(cuerpo19, glm::vec3(0.096, 0.05, 0.1)));
		//cabeza
		glm::mat4 cabeza = glm::translate(cuerpo3, glm::vec3(0.0, 0.06, 0.0));
		//sphere1.enableWireMode();
		sphere8.render(glm::scale(cabeza, glm::vec3(1.2, 1.2, 1.2)));
		cabeza = glm::rotate(cabeza, rot7, glm::vec3(0, 1, 0));
		cabeza = glm::rotate(cabeza, rot10, glm::vec3(1, 0, 0));


		//j3 = glm::rotate(j3, rot2, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 cabezaabajo = glm::translate(cuerpo, glm::vec3(0.0, -0.4, 0.0));
		//sphere1.enableWireMode();
		sphere8.render(glm::scale(cabezaabajo, glm::vec3(1.2, 1.2, 1.2)));


		// ojo
		glm::mat4 OC = glm::translate(cabeza, glm::vec3(0.0, 0.3, 0.48));
		cylinder6.render(glm::scale(OC, glm::vec3(1.3, 0.2, 1.0)));

		glm::mat4 OC1 = glm::translate(OC, glm::vec3(0.0, 0.0, 0.1));
		sphere10.render(glm::scale(OC1, glm::vec3(0.15, 0.15, 0.05)));

		glm::mat4 OC3 = glm::translate(cabeza, glm::vec3(0.0, 0.1, 0.55));
		cylinder6.render(glm::scale(OC3, glm::vec3(0.8, 0.1, 0.6)));

		glm::mat4 OC4 = glm::translate(OC3, glm::vec3(0.0, 0.0, 0.05));
		sphere11.render(glm::scale(OC4, glm::vec3(0.05, 0.05, 0.05)));
		glm::mat4 OC10 = glm::translate(cabeza, glm::vec3(0.2, 0.1, 0.55));
		cylinder6.render(glm::scale(OC10, glm::vec3(0.6, 0.1, 0.6)));
		glm::mat4 OC11 = glm::translate(OC10, glm::vec3(0.0, 0.0, 0.05));
		sphere14.render(glm::scale(OC11, glm::vec3(0.05, 0.05, 0.05)));

		// RECUADROS BAJO CABEZA
		glm::mat4 OC5 = glm::translate(cabeza, glm::vec3(-0.3, 0.02, 0.45));
		cylinder6.render(glm::scale(OC5, glm::vec3(0.85, 0.08, 0.6)));
		glm::mat4 OC6 = glm::translate(cabeza, glm::vec3(-0.5, 0.02, 0.2));
		cylinder6.render(glm::scale(OC6, glm::vec3(0.85, 0.08, 0.6)));


		glm::mat4 OC8 = glm::translate(cabeza, glm::vec3(0.3, 0.02, 0.45));
		cylinder6.render(glm::scale(OC8, glm::vec3(0.85, 0.08, 0.6)));
		glm::mat4 OC9 = glm::translate(cabeza, glm::vec3(0.5, 0.02, 0.2));
		cylinder6.render(glm::scale(OC9, glm::vec3(0.85, 0.08, 0.6)));
		//BRAZOS
		glm::mat4 brazoizq = glm::translate(cuerpo, glm::vec3(-0.6, 0.3, -0.2));
		brazoizq = glm::rotate(brazoizq, rot8, glm::vec3(1, 0, 0));
		//sphere1.enableWireMode();
		sphere8.render(glm::scale(brazoizq, glm::vec3(0.6, 0.6, 0.4)));

		glm::mat4 brazoder = glm::translate(cuerpo, glm::vec3(0.6, 0.3, -0.2));
		//sphere1.enableWireMode();
		brazoder = glm::rotate(brazoder, rot8, glm::vec3(1, 0, 0));
		sphere8.render(glm::scale(brazoder, glm::vec3(0.6, 0.6, 0.4)));

		glm::mat4 brazoizq1 = glm::translate(brazoizq, glm::vec3(-0.18, -0.7, 0.0));
		cylinder4.render(glm::scale(brazoizq1, glm::vec3(1, 1.3, 1.4)));

		glm::mat4 brazoder1 = glm::translate(brazoder, glm::vec3(0.18, -0.7, 0.0));
		cylinder4.render(glm::scale(brazoder1, glm::vec3(1, 1.3, 1.4)));

		glm::mat4 brazoizq2 = glm::translate(brazoizq1, glm::vec3(0.0, -0.62, 0.1));
		box6.render(glm::scale(brazoizq2, glm::vec3(0.3, 0.35, 0.3)));

		glm::mat4 brazoder2 = glm::translate(brazoder1, glm::vec3(0.0, -0.62, 0.1));
		box6.render(glm::scale(brazoder2, glm::vec3(0.3, 0.35, 0.3)));

		glm::mat4 brazoder3 = glm::translate(brazoder2, glm::vec3(0.0, -0.21, 0.0));
		box6.render(glm::scale(brazoder3, glm::vec3(0.3, 0.13, 0.6)));

		glm::mat4 brazoizq3 = glm::translate(brazoizq2, glm::vec3(0.0, -0.21, 0.0));
		box6.render(glm::scale(brazoizq3, glm::vec3(0.3, 0.13, 0.6)));

		glm::mat4 brazoizq4 = glm::translate(brazoizq3, glm::vec3(0.0, -0.06, 0.0));
		box7.render(glm::scale(brazoizq4, glm::vec3(0.3, 0.05, 0.5)));

		glm::mat4 brazoder4 = glm::translate(brazoder3, glm::vec3(0.0, -0.06, 0.0));
		box7.render(glm::scale(brazoder4, glm::vec3(0.3, 0.05, 0.5)));

		glm::mat4 detallbraizq = glm::translate(brazoizq1, glm::vec3(-0.11, 0.1, 0.0));
		cylinder6.render(glm::scale(detallbraizq, glm::vec3(0.3, 0.6, 0.05)));
		glm::mat4 detallbraizq1 = glm::translate(brazoizq1, glm::vec3(0.0, 0.1, 0.14));
		cylinder6.render(glm::scale(detallbraizq1, glm::vec3(0.2, 0.3, 0.05)));
		glm::mat4 detallbrader1 = glm::translate(brazoder1, glm::vec3(0.0, 0.1, 0.14));
		cylinder6.render(glm::scale(detallbrader1, glm::vec3(0.2, 0.3, 0.05)));


		glm::mat4 detallbrader = glm::translate(brazoder1, glm::vec3(0.11, 0.1, 0.0));
		cylinder6.render(glm::scale(detallbrader, glm::vec3(0.08, 0.9, 0.05)));
		// ABAJO 
		glm::mat4 abajo = glm::translate(cuerpo, glm::vec3(0.0, -1.0, 0.0));
		box6.render(glm::scale(abajo, glm::vec3(0.2l, 0.3, 0.5)));
		glm::mat4 abajoc = glm::translate(abajo, glm::vec3(0.0, -0.2, 0.0));
		box6.render(glm::scale(abajoc, glm::vec3(0.4, 0.2, 0.5)));



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