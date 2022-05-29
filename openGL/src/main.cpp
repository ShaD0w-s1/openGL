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

	// 定义着色器类型并指定ID
	unsigned int shader = glCreateShader(type);

	// 确定着色器源码指针
	glShaderSource(shader, 1, &shaderSource, nullptr);

	// 执行编译
	glCompileShader(shader);

	// 确定着色器状态
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

	// 指定子程序ID
	unsigned int shaderProgram = glCreateProgram();

	// 
	unsigned int VS = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int FS = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// 
	glAttachShader(shaderProgram, VS);
	glAttachShader(shaderProgram, FS);
	glLinkProgram(shaderProgram);

	// 确定子程序状态
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return -1;

		// TODO someAnthorErrorHandle
	}

	// 清空内存( 建议不删 方便调试？ ) 
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

// 监视输入
void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 启动窗口
	GLFWwindow*  window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	// 创建上下文
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 初始化GL函数指针
	glewInit();

	std::cout << glGetString(GL_VERSION) << std::endl;

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	unsigned int shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);


	// 设置顶点
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	// 面索引
	unsigned int indices[] = { 
			0, 1, 3, // 第一个三角形
			1, 2, 3  // 第二个三角形
	};

	// 创建显存缓冲
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// 绑定顶点缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 绑定索引缓冲对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

		// 渲染循环
		while (!glfwWindowShouldClose(window))
		{
			// 进程输入监视
			processInput(window);

			// 渲染
			
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			

			// 双缓冲
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	glfwTerminate();
	return 0;
}