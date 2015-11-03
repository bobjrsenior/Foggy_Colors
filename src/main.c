#include <main.h>

#define GLSL(src) "#version 150 core\n" #src

const GLchar* squareVertexSource = GLSL(
	in vec2 position;
	in vec2 texcoord;
	out vec2 Texcoord;
	uniform float ratio;
	uniform vec2 scale;
	uniform vec2 translation;
	uniform float rotation;
	uniform int overrideTex;
	uniform vec2 texcoordoverride;
	void main(){
		if (overrideTex == 1){
			Texcoord = vec2(0.0001 + 0.0237 * texcoordoverride.x + 0.0236 * texcoord.x, 0.37 * texcoordoverride.y + 0.40 * texcoord.y);
		}
		else{
			Texcoord = texcoord;
		}
		gl_Position = vec4(translation.x + (scale.x * (position.x * cos(1.5 * rotation) - position.y * sin(1.5 * rotation))), translation.y + (scale.y * (position.y * ratio * cos(1.5 * rotation) + position.x * sin(1.5 * rotation))), 0.0, 1.0);
	}
);

const GLchar* squareFragmentSource = GLSL(
	in vec2 Texcoord;
	out vec4 outColor;
	uniform vec4 color;
	uniform int showTex;
	uniform sampler2D tex;
	void main(){
		if (showTex == 1){
			outColor = texture(tex, Texcoord).w * color;
		}
		else{
			outColor = color;
		}
	}
);

char keys[349];
char state = -1;

//Possible shape colors
const int numColors = 8;
const GLfloat colors[] = {
						1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.6471f, 0.16471f, 0.16471f, 1.0f,
						0.50196f, 0.0f, 0.50196f, 1.0f
						};

int main(void)
{
	GLfloat square[] = {
					-0.5, 0.5, 0.0, 0.0,
					0.5, 0.5, 1.0, 0.0,
					0.5, -0.5, 1.0, 1.0,
					0.5, -0.5, 1.0, 1.0,
					-0.5, -0.5, 0.0, 1.0,
					-0.5, 0.5, 0.0, 0.0
					};

	srand(glfwGetTime());

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1920, 1080, "OpenGL Fun", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	glewInit();
	
	glEnable(GL_BLEND);

	//Create a Vertex Array Object to store all program ect related info
	//Only things after creation are stored
	GLuint squarevao;
	glGenVertexArrays(1, &squarevao);
	//Bind it to use it
	glBindVertexArray(squarevao);

	//Create the Vertex Buffer Objects
	GLuint squarevbo;
	glGenBuffers(1, &squarevbo);

	//Make the vbo active
	glBindBuffer(GL_ARRAY_BUFFER, squarevbo);

	//Copy data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	
	//Create Vertex Shader
	GLuint squareVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(squareVertexShader, 1, &squareVertexSource, NULL);

	//Compile the shader
	glCompileShader(squareVertexShader);

	//Compilation Check
	GLint status;
	glGetShaderiv(squareVertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(squareVertexShader, 512, NULL, buffer);
		printf("Vertex: %s\n", buffer);
	}



	//Create the fragment shader
	GLuint squareFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(squareFragmentShader, 1, &squareFragmentSource, NULL);
	//Compile the shader
	glCompileShader(squareFragmentShader);

	//Compilation Check
	glGetShaderiv(squareFragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(squareFragmentShader, 512, NULL, buffer);
		printf("Fragment: %s\n", buffer);
	}

	//Attach the shaders to a program
	GLuint squareShaderProgram = glCreateProgram();
	glAttachShader(squareShaderProgram, squareVertexShader);
	glAttachShader(squareShaderProgram, squareFragmentShader);

	//Unneeded since default buffer is 0
	//glBindFragDataLocation(squareShaderProgram, 0, "outColor");

	//Link the shader program
	glLinkProgram(squareShaderProgram);

	//Use the new shader program
	glUseProgram(squareShaderProgram);


	//Create a link the the vertex shader's in variables
	GLint squarePosAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glVertexAttribPointer(squarePosAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//Enable the attribute
	glEnableVertexAttribArray(squarePosAttrib);

	GLint squareTexAttrib = glGetAttribLocation(squareShaderProgram, "texcoord");
	glVertexAttribPointer(squareTexAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	//Enable the attribute
	glEnableVertexAttribArray(squareTexAttrib);
	
	//Load texture
	GLuint textures[8];
	//0 = PlayPrompt; 1 = Circle; 2 = Level Label; 3 = Red; 4 = Green; 5 = Blue; 6 = Purple; 7 = alphabet
	glGenTextures(8, textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	int w, h;
	unsigned char* image = SOIL_load_image("PlayPrompt.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	image = SOIL_load_image("Circle.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	image = SOIL_load_image("Level.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[3]);

	image = SOIL_load_image("Red.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[4]);

	image = SOIL_load_image("Green.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[5]);

	image = SOIL_load_image("Blue.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, textures[6]);

	image = SOIL_load_image("Purple.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, textures[7]);

	image = SOIL_load_image("alphabet.png", &w, &h, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Get the Uniforms
	GLint squareuniRatio = glGetUniformLocation(squareShaderProgram, "ratio");

	GLint squareuniColor = glGetUniformLocation(squareShaderProgram, "color");

	GLint squareuniShowTex = glGetUniformLocation(squareShaderProgram, "showTex");

	GLint squareuniOverrideTex = glGetUniformLocation(squareShaderProgram, "overrideTex");
	glUniform1i(squareuniOverrideTex, 0);

	GLint squareuniTexOverride = glGetUniformLocation(squareShaderProgram, "texcoordoverride");
	glUniform2f(squareuniTexOverride, 0.0f, 0.0f);

	GLint squareuniScale = glGetUniformLocation(squareShaderProgram, "scale");
	glUniform2f(squareuniScale, 0.25f, 0.25f);

	GLint squareuniRotation = glGetUniformLocation(squareShaderProgram, "rotation");
	glUniform1f(squareuniRotation, 0.0f);

	GLint squareuniTranslation = glGetUniformLocation(squareShaderProgram, "translation");

	//Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//TImer variables
	double time = 0;
	double startTime = 0;
	double totalTime = 0;
	GLfloat colorTimer = 0.9f;
	GLfloat colorCountdown = colorTimer;
	int points = 0;
	int trys = 0;
	int pointCalc;
	int shapes;
	char buffer[30];

	//Hold the individual squares colors
	vec4* square1Color = makeNewVec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4* square2Color = makeNewVec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4* square3Color = makeNewVec4(0.0f, 1.0f, 0.0f, 1.0f);
	int tex1, tex2, tex3;
	#ifdef _WIN32
	PlaySound(TEXT("BRD-Teleport_Prokg.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	#endif
	while (!glfwWindowShouldClose(window))
	{
		GLfloat ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (GLfloat)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);


		//Letter Testing
		//drawText("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.9f, 0.9f, 0.5f);
		//drawText("abcdefghijklmnopqrstuvwxyz", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.9f, 0.8f, 0.5f);

		glUniform1f(squareuniRatio, ratio);

		switch (state){
			case -3://Win Screen
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					state = 1;
					trys = 0;
					points = 0;
					startTime = time;
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					glUniform4fv(squareuniColor, 1, &colors[rand() % 8]);
				}
				
				drawText("You Win", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.5f, 0.8f, 1.0f);
				strcpy(buffer, "Misses:");
				sprintf(&buffer[7], "%d", trys - points);
				drawText(buffer, squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.6f, 0.5f, 0.8f);
				strcpy(buffer, "Time Taken:");
				sprintf(&buffer[11], "%.3f", totalTime);
				drawText(buffer, squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.6f, 0.3f, 0.8f);
				drawText("Spacebar to play again", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.85f, -0.8f, 0.8f);
			break;
			case -1://Setup Main Menu
				state = -2;
				glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
				break;
			case -2://Display Main Menu
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE]){
					keys[GLFW_KEY_SPACE] = 1;
					state = 1;
					startTime = time;
					glUniform1i(squareuniShowTex, 0);
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					glUniform4fv(squareuniColor, 1, &colors[rand() % 8]);
				}

				
				drawText("Foggy Colors", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.6f, 0.9f, 1.0f);
				drawText("Instructions", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.5f, 0.3f, 0.7f);
				drawText("Press space when all\nthe objects are the\nsame color", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.60f, 0.1f, 0.5f);

				drawText("Spacebar to continue", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.87f, -0.8f, 0.9f);
				break;
			case 0://'Practice' Level
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					++trys;
					++points;
					colorCountdown = 0.0f;
					state = 1;
					colorTimer = .9f;
					break;
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
				}

				glUniform2f(squareuniScale, 0.25f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, -0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glUniform2f(squareuniScale, 0.075f, 0.075f);
				glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
				glUniform2f(squareuniTranslation, -0.66f, -0.89f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform2f(squareuniScale, 0.075f, 0.075f);
				glUniform4f(squareuniColor, 0.0f, 0.0f, 0.0f, 1.0f);
				drawText("0", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f, -0.91f, 1.0f);
				glUniform2f(squareuniScale, 0.075f, 0.075f);
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case 1://Main Game/Level 1
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					if (IS_VEC_4_EQUAL(square1Color, square2Color)){
						++points;
						++trys;
						colorCountdown = 0.0f;
						if (points == 10){
							state = 2;
							break;
						}
						colorTimer *= 0.99f;
					}
					else{
						++trys;
						points--;
						colorTimer /= 0.99f;
						if (points < 0){
							state = 0;
							break;
						}
					}
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
					if (rand() % 5 != 0){
						copyArrVec4(square2Color, &colors[rand() % 8]);
					}
					else{
						copyVec4(square2Color, square1Color);
					}
				}

				glUniform2f(squareuniScale, 0.25f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, -0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform4fv(squareuniColor, 1, square2Color->arr);
				glUniform2f(squareuniTranslation, 0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glUniform2f(squareuniScale, 0.075f, 0.075f);
				shapes = 0;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (pointCalc = points / 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 0.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("5", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.67f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				for (pointCalc = points % 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 1.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("1", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				glBlendFunc(GL_ONE, GL_ZERO);
				
				break;
			case 2://Level 2
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					if (IS_VEC_4_EQUAL(square1Color, square2Color) && IS_VEC_4_EQUAL(square1Color, square3Color)){
						++points;
						++trys;
						colorTimer *= .99f;
						colorCountdown = 0.0f;
						if (points == 15){
							state = 3;
							
							break;
						}
						colorTimer *= 0.95f;
					}
					else{
						--points;
						++trys;
						colorTimer /= 0.95f;
						if (points < 10){
							state = 1;
							break;
						}
					}
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
					if (rand() % 5 != 0){
						copyArrVec4(square2Color, &colors[rand() % 8]);
						copyArrVec4(square3Color, &colors[rand() % 8]);
					}
					else{
						copyVec4(square2Color, square1Color);
						copyVec4(square3Color, square1Color);
					}
				}

				glUniform2f(squareuniScale, 0.25f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, -0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform4fv(squareuniColor, 1, square2Color->arr);
				glUniform2f(squareuniTranslation, 0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform4fv(squareuniColor, 1, square3Color->arr);
				glUniform2f(squareuniTranslation, 0.0f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform2f(squareuniScale, 0.075f, 0.075f);
				shapes = 0;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (pointCalc = points / 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 0.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("5", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.67f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				for (pointCalc = points % 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 1.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("1", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case 3://Level 3
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					if (IS_VEC_4_EQUAL(square1Color, square2Color) && IS_VEC_4_EQUAL(square1Color, square3Color)){
						++points;
						++trys;
						colorCountdown = 0.0f;
						if (points == 20){
							state = 4;
							break;
						}
						colorTimer *= 0.95f;
						
					}
					else{
						--points;
						++trys;
						colorTimer /= 0.95f;
						if (points < 15){
							state = 2;
							break;
						}
					}
				}
				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
					if (rand() % 5 != 0){
						copyArrVec4(square2Color, &colors[rand() % 8]);
						copyArrVec4(square3Color, &colors[rand() % 8]);
					}
					else{
						copyVec4(square2Color, square1Color);
						copyVec4(square3Color, square1Color);
					}
				}

				glUniform2f(squareuniScale, 0.25f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, 0.8f * cosf(time * M_PI + M_PI), 0.8f * sinf(time * M_PI + M_PI));
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform4fv(squareuniColor, 1, square2Color->arr);
				glUniform2f(squareuniTranslation, 0.8f * cosf(time * M_PI), 0.8f * sinf(time * M_PI));
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform4fv(squareuniColor, 1, square3Color->arr);
				glUniform2f(squareuniTranslation, 0.0f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform2f(squareuniScale, 0.075f, 0.075f);
				shapes = 0;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (pointCalc = points / 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 0.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("5", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.67f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				for (pointCalc = points % 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 1.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("1", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;
				}
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case 4://Level 4
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					if (IS_VEC_4_EQUAL(square1Color, square2Color) && IS_VEC_4_EQUAL(square1Color, square3Color)){
						++points;
						++trys;
						colorCountdown = 0.0f;
						if (points == 25){
							state = 5;
							break;
						}
						colorTimer *= 0.95f;

					}
					else{
						--points;
						++trys;
						colorTimer /= 0.95f;
						if (points < 20){
							state = 3;
							break;
						}
					}
				}
				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
					if (rand() % 5 != 0){
						copyArrVec4(square2Color, &colors[rand() % 8]);
						copyArrVec4(square3Color, &colors[rand() % 8]);
					}
					else{
						copyVec4(square2Color, square1Color);
						copyVec4(square3Color, square1Color);
					}
				}

				glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), 1);
				glUniform1f(squareuniRotation, time - 2 * M_PI * ((int) (time / (2 * M_PI))));
				glUniform2f(squareuniScale, 0.25f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, 0.8f * cosf(time * M_PI + M_PI), 0.8f * sinf(time * M_PI + M_PI));
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform1f(squareuniRotation, time - 2.0f * M_PI * ((int)(time / (2 * M_PI))));
				glUniform4fv(squareuniColor, 1, square2Color->arr);
				glUniform2f(squareuniTranslation, 0.8f * cosf(time * M_PI), 0.8f * sinf(time * M_PI));
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform1f(squareuniRotation, 0);
				glUniform1i(squareuniShowTex, 1);
				glUniform4fv(squareuniColor, 1, square3Color->arr);
				glUniform2f(squareuniTranslation, 0.0f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glUniform1i(squareuniShowTex, 0);

				glUniform2f(squareuniScale, 0.075f, 0.075f);
				shapes = 0;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (pointCalc = points / 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 0.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("5", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.67f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				for (pointCalc = points % 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 1.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("1", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case 5://Level 5
				colorCountdown -= (glfwGetTime() - time);
				time = glfwGetTime();
				totalTime = time - startTime;

				if (keys[GLFW_KEY_SPACE] == 2){
					keys[GLFW_KEY_SPACE] = 1;
					if (IS_VEC_4_EQUAL(square1Color, square2Color) && IS_VEC_4_EQUAL(square1Color, square3Color)){
						++points;
						++trys;
						colorCountdown = 0.0f;
						if (points == 30){
							state = -3;
							colorTimer *= 1.05f;
							break;
						}
						colorTimer *= 0.95f;
					}
					else{
						--points;
						++trys;
						colorTimer /= 0.95f;
						if (points < 25){
							state = 4;
							break;
						}
					}
				}

				if (colorCountdown <= 0){
					colorCountdown = colorTimer;
					copyArrVec4(square1Color, &colors[rand() % 8]);
					if (rand() % 5 != 0){
						copyArrVec4(square2Color, &colors[rand() % 8]);
						copyArrVec4(square3Color, &colors[rand() % 8]);
					}
					else{
						copyVec4(square2Color, square1Color);
						copyVec4(square3Color, square1Color);
					}
					//Set random textures
					tex1 = rand() % 4 + 3;
					tex2 = rand() % 4 + 3;
					tex3 = rand() % 4 + 3;
				}
				glUniform1i(squareuniShowTex, 1);
				glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), tex1);
				glUniform2f(squareuniScale, 0.35f, 0.25f);
				
				glUniform4fv(squareuniColor, 1, square1Color->arr);
				glUniform2f(squareuniTranslation, -0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), tex2);
				glUniform4fv(squareuniColor, 1, square2Color->arr);
				glUniform2f(squareuniTranslation, 0.5f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), tex3);
				glUniform4fv(squareuniColor, 1, square3Color->arr);
				glUniform2f(squareuniTranslation, 0.0f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glUniform1i(squareuniShowTex, 0);
				glUniform2f(squareuniScale, 0.075f, 0.075f);
				shapes = 0;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (pointCalc = points / 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 0.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("5", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.67f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				for (pointCalc = points % 5; pointCalc > 0; --pointCalc){
					glUniform4f(squareuniColor, 0.0f, 1.0f, 1.0f, 1.0f);
					glUniform2f(squareuniTranslation, -0.67f + (shapes * .11f), -0.88f);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
					drawText("1", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.66f + (shapes * .11f), -0.89f, 1.0f);
					glUniform2f(squareuniScale, 0.075f, 0.075f);
					++shapes;

				}
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
		}
		//Level UI/Stats
		if (state != -1 && state != -2 && state != -3){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniform4f(squareuniColor, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText("Level:", squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.95f, -0.90f, 0.5f);
			strcpy(buffer, "Time:");
			sprintf(&buffer[5], "%.3f", totalTime);
			drawText(buffer, squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, -0.95f, 0.95f, 0.5f);
			strcpy(buffer, "FPS:");
			sprintf(&buffer[4], "%.3f", 1.0 / (glfwGetTime() - time));
			drawText(buffer, squareuniOverrideTex, squareuniShowTex, squareShaderProgram, squareuniTexOverride, squareuniScale, squareuniTranslation, squareuniColor, 0.30f, 0.95f, 0.5f);
			glBlendFunc(GL_ONE, GL_ZERO);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	//Clean up
	glDeleteTextures(8, textures);
	glDeleteProgram(squareShaderProgram);
	glDeleteShader(squareVertexShader);
	glDeleteShader(squareFragmentShader);
	glDeleteBuffers(1, &squarevao);
	glDeleteBuffers(1, &squarevbo);

	#ifdef _WIN32
	//End background song
	PlaySound(NULL, NULL, SND_FILENAME);
	#endif
	exit(EXIT_SUCCESS);
}

void drawText(char* text, GLuint squareuniOverrideTex, GLuint squareuniShowTex, GLuint squareShaderProgram, GLuint squareuniTexOverride,  GLuint squareuniScale, GLuint squareuniTranslation, GLuint squareuniColor, GLfloat xPos, GLfloat yPos, GLfloat scale){
	char* c = text;
	GLfloat initXPos = xPos;
	glUniform1i(squareuniOverrideTex, 1);
	glUniform1i(squareuniShowTex, 1);
	glUniform1i(glGetUniformLocation(squareShaderProgram, "tex"), 7);
	glUniform2f(squareuniScale, 0.10f * scale, 0.10f * scale);
		
	while (*c != '\0'){
		glUniform2f(squareuniTranslation, xPos, yPos);
		if (*c > 'Z'){
			glUniform2f(squareuniTexOverride, *c - 91.0f, 1.0f);
		}
		else if (*c == ':'){
			//glUniform2f(squareuniTexOverride, 32.0f, 1.0f);
		}
		else if(*c >= 'A'){
			glUniform2f(squareuniTexOverride, *c - 65 + 10.0f, 0.0f);
		}
		else if (*c == '\n'){
			xPos = initXPos - 0.1f * scale;
			yPos -= 0.2f * scale;
		}
		else if (*c == '.'){
			glUniform2f(squareuniTexOverride, 4.0f, 1.0f);
		}
		else if(*c != ' '){
			glUniform2f(squareuniTexOverride, *c - 48.0f, 0.0f);
		}

		if (!isspace(*c) && *c != ':'){
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		c++;
		xPos += 0.1f * scale;

	}
	glUniform1i(squareuniShowTex, 0);
	glUniform1i(squareuniOverrideTex, 0);
}


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == -1){
		return;
	}
	if (action == GLFW_PRESS){
		keys[key] = 2;
		if (key == GLFW_KEY_ESCAPE){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	else if (action == GLFW_RELEASE){
		keys[key] = 0;
	}
}
