#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";



static unsigned int CompileShader(unsigned int type ,const char* shaderSource)
{ 
	int success;
	char infoLog[512];

	// ������ɫ�����Ͳ�ָ��ID
	unsigned int shader = glCreateShader(type);

	// ȷ����ɫ��Դ��ָ��
	glShaderSource(shader, 1, &shaderSource, nullptr);

	// ִ�б���
	glCompileShader(shader);

	// ȷ����ɫ��״̬
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<< ( type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT" ) << "::COMPILATION_FAILED\n" << infoLog << std::endl;

		return -1;

		// TODO someAnthorErrorHandle
	}

	return shader;
};

static unsigned int CreateShader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int success;
	char infoLog[512];

	// ָ���ӳ���ID
	unsigned int shaderProgram = glCreateProgram();

	// 
	unsigned int VS = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int FS = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// 
	glAttachShader(shaderProgram, VS);
	glAttachShader(shaderProgram, FS);
	glLinkProgram(shaderProgram);

	// ȷ���ӳ���״̬
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return -1;

		// TODO someAnthorErrorHandle
	}

	// ����ڴ�( ���鲻ɾ ������ԣ� ) 
	glDeleteShader(VS);
	glDeleteShader(FS);

	return shaderProgram;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// ��������
void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// ��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow*  window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	// ����������
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ��ʼ��GL����ָ��
	glewInit();

	std::cout << glGetString(GL_VERSION) << std::endl;

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	unsigned int shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);


	// ���ö���
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	// ������
	unsigned int indices[] = { 
			0, 1, 3, // ��һ��������
			1, 2, 3  // �ڶ���������
	};

	// �����Դ滺��
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// �󶨶����������
	glBindVertexArray(VAO);

	// �󶨶��㻺�����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// �������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

		// ��Ⱦѭ��
		while (!glfwWindowShouldClose(window))
		{
			// �����������
			processInput(window);

			// ��Ⱦ
			
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			

			// ˫����
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	glfwTerminate();
	return 0;
}