// Domarys Correa (28/05/19)
// Faça os dois modelos (nanosuit e cyborg) pularem um por cima do outro até o infinito. Cada pulo deve durar 2 segundos.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float modBase1 = 0.0, modBase2 = 0.0;
int on = 0, idModel = 0;
int moved = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

    // load models
    // -----------
    Model ourModel1(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	Model ourModel2(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));

	// render the loaded model	
	double currentTime=0, firstTime=0, definedTime = 3;	// variaveis de controle temporal
	modBase1 = 0.0; // base fixa do modelo nanosuit
	modBase2 = 0.0; // base fixa do modelo cyborg
    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

		//definição inicial dos modelos, já rotando eles para olhar a direita e aumento o tamanho do cyborg
		glm::mat4 model1, model2; // model1 = nanosuit, model = cyborg
		model1 = glm::translate(model1, glm::vec3(modBase1, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model1 = glm::scale(model1, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(0.0,1.0,0.0));
		model2 = glm::translate(model2, glm::vec3(modBase2, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(0.8f, 0.8f, 0.8f));	// it's a bit too big for our scene, so scale it down
		model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

		//aciona a funcao
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			firstTime = glfwGetTime();
			on = 1;
			moved = 1;
		}

		// coleta tempoatual e gera o delta que ira controlar a animacao
		double currentTime = glfwGetTime();
		double deltaTimeAnimation = currentTime - firstTime;


		if ((deltaTimeAnimation > definedTime)&&(moved==1)) { // testa fim do pulo
			
			if (idModel == 0) {		// se acabou o pulo do model1, seta variavel para model2 e bases de ambos modelos, reinicia firstTime e delta				
				idModel = 1;
				modBase1 = modBase1 + (2.0);
				modBase2 = modBase2 + (2.0);
				firstTime = glfwGetTime();
				deltaTimeAnimation = currentTime - firstTime;
			}	
			else {					 // se acabou o pulo do model2, seta variavel para model1 e bases de ambos modelos, reinicia firstTime e delta
				idModel = 0;
				modBase1 = modBase1 + (2.0);
				modBase2 = modBase2 + (2.0);
				firstTime = glfwGetTime();
				deltaTimeAnimation = currentTime - firstTime;
				//on = 0;
			}
		}

		if (on == 1) { // verifica se a funcao pulo ainda esta ativa
			float rotationAngle = deltaTimeAnimation * 180 / definedTime;
			if (idModel == 0) { // pulo model1
				model1 = glm::rotate(model1, glm::radians(rotationAngle), glm::vec3(1.0, 0.0, 0.0));
				model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, -15));
				model1 = glm::rotate(model1, glm::radians(-rotationAngle), glm::vec3(1.0, 0.0, 0.0));			
			}	
			else if (idModel == 1) { // pulo model2
				model2 = glm::rotate(model2, glm::radians(rotationAngle), glm::vec3(1.0, 0.0, 0.0));
				model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -4));
				model2 = glm::rotate(model2, glm::radians(-rotationAngle), glm::vec3(1.0, 0.0, 0.0));
			}
		} 

		//desenha modelos
        ourShader.setMat4("model", model1);
        ourModel1.Draw(ourShader);
		ourShader.setMat4("model", model2);
		ourModel2.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
