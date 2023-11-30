#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glut.h"
#include "Imgui/imgui_impl_opengl3.h"



#include <GL/glew.h>
#include <GL/freeglut.h>


#include <windows.h>
#include <thread>
#include "camera.h"




#define SW         1000                  //screen width
#define SH         1000                  //screen height
#define SW2        (SW/2)               //half of screen width
#define SH2        (SH/2)               //half of screen height
#define pixelScale 1                    //OpenGL pixel scale
#define GLSW       (SW*pixelScale)          //OpenGL window width
#define GLSH       (SH*pixelScale)          //OpenGL window height
#define GLW        (GetSystemMetrics(SM_CXSCREEN) - GLSW)
#define GLH        0 
#define PI			3.14158 


#define sSW         100                  //screen width
#define sSH         100                  //screen height
#define spixelScale 4                    //OpenGL pixel scale
#define sGLSW       (sSW*spixelScale)          //OpenGL window width
#define sGLSH       (sSH*spixelScale)          //OpenGL window height

//------------------------------------------------------------------------------
typedef struct
{
	int fr1, fr2;           //frame 1 frame 2, to create constant frame rate
}time1; time1 T,T2;

typedef struct
{
	int w, s, a, d,z,c;        
	int sl, sr;            
	int m;                 
}keys; keys K;
typedef struct
{
	float x, y, z,T12;

}k; k Move;

//------------------------------------------------------------------------------

bool ReadF(const char* path, std::string& outFile) {
	using namespace std;
	ifstream fin(path);

	if (fin.is_open()) {
		string line;
		while (getline(fin, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		fin.close();
		return true;
	}
	else {
		return false;
	}

}

bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec2 clear_color2 = ImVec2(0, 0);
float t = 0.1f;
bool Mode = 0;
float v[2] = { 0.0f,0.0f };
float vec[30][2];
const char* mas[30];
int sizec = 0;

void f(int t, std::string& str)
{
	std::string s;
	while (t)
	{

		s.push_back((t % 10) + '0');
		t /= 10;
	}
	reverse(s.begin(), s.end());
	for (size_t i = 0; i < s.size(); i++)
		str.push_back(s[i]);
}
void MImGui() {

	std::string str[30];
	for (int i = 0; i < 30; i++) {

		str[i] = std::to_string(i);
		mas[i] = str[i].c_str();
	}




	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::Checkbox("Stop", &Mode);
	if (Mode) {
		t = 0;
	}
	

	ImGui::SliderFloat("Speed", &t, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	
	ImGui::DragFloat2("Sphere", v, 0.1f);

	if (ImGui::Button("Add source"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		sizec++;
	ImGui::SameLine();
	if (ImGui::Button("Clear"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		sizec=0;
	/*
	ImGui::Text("counter = %d", );*/
	
	for (int i = 0; i < sizec; i++)
	{
		
		ImGui::DragFloat2(mas[i], vec[i], 0.1f);
		
	}
	

	ImGui::End();
}
void movePlayer()
{
	if (GetKeyState('S') < 0) {
		Move.z -= 0.5f;
		//if (Move.z < 0)Move.z += 360;
	}
	if (GetKeyState('W') < 0) {
		Move.z += 0.5f;
		//if (Move.z > 360)Move.z -= 360;
	}
	if (GetKeyState('A') < 0) {
		Move.x -= 0.1f;
	}
	if (GetKeyState('D') < 0) {
		Move.x += 0.1f;
	}
	if (GetKeyState('Z') < 0) {
		Move.y -= 0.1f;
	}
	if (GetKeyState('C') < 0) {
		Move.y += 0.1f;
	}

}
unsigned int pr;

float fr = 0.0f;
float vec1[60];
void display()
{
	
	int x, y;
	if (T.fr1 - T.fr2 >= 50)                        //only draw 20 frames/second
	{
		movePlayer();
		int loc1 = glGetUniformLocation(pr, "u_Pos");
		glUniform3f(loc1, Move.x, Move.y,Move.z);


		int loc2 = glGetUniformLocation(pr, "f");
		glUniform1f(loc2, fr);

		int loc3 = glGetUniformLocation(pr, "Sp");
		glUniform2f(loc3, v[0],v[1]);

		int loc4 = glGetUniformLocation(pr, "M");
		glUniform1f(loc3,Mode );

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				vec1[i*2+j] = vec[i][j];
			}
		}

		int loc5 = glGetUniformLocation(pr, "array");
		glUniform1fv(loc5, 60, vec1);

		int loc6 = glGetUniformLocation(pr, "sc");
		glUniform1f(loc6, sizec);


		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGLUT_NewFrame();
		ImGui::NewFrame();
		fr = fr - t;
		
		glDrawArrays(GL_QUADS, 0, 4);
		
		
		T.fr2 = T.fr1;

		MImGui();

		// Rendering
		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glutSwapBuffers();
		glutReshapeWindow(GLSW, GLSH);             //prevent window scaling
		
	}

	T.fr1 = glutGet(GLUT_ELAPSED_TIME);          //1000 Milliseconds per second
	glutPostRedisplay();
}
static unsigned int CompileShader(unsigned int type, const std::string& sCode){
	unsigned int id = glCreateShader(type);
	const char* src = sCode.c_str();
	glShaderSource(id,1,&src,nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char* mes = (char*)alloca(len*sizeof(char));

		glGetShaderInfoLog(id, len, &len, mes);
		std::cout << "Failed to compile "<< (type==GL_VERTEX_SHADER?"vertex":"fragment") << " shader." << std::endl;
		std::cout << mes << std::endl;

		glDeleteShader(id);
		return 0;
	}
	return id;

}
unsigned int CreateShader(const std::string& vsh, const std::string& fsh) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsh);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsh);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
float pos[]{
	-1.0f, 1.0f, 
	1.0f, 1.0f, 
	1.0f,-1.0f, 
	-1.0f,-1.0f, 
};



void init()
{

	
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	
	std::string vF, fF;
	ReadF("vshader.vs", vF);
	ReadF("fshader.fs", fF);

	pr = CreateShader(vF,fF);
	glUseProgram(pr);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
}











HWND hwnd2;
void pixel(int x, int y, int r = 0, int g = 0, int b = 0)                  //draw a pixel at x/y with rgb
{
	glColor3ub(r, g, b);
	glBegin(GL_POINTS);
	glVertex2i(x * spixelScale + 2, y * spixelScale + 2);
	glEnd();
}

float RPoint(int x, int y, int x1, int y1) {

	int dx = abs(x1 - x);
	int dy = abs(y1 - y);
	float R = sqrt(dx * dx + dy * dy);

	return R;
}

int T12 = 10;
std::vector <k> ip;

bool s = 0;

void Wave() {



	int x, y, c = 0;
	float R;
	int A = 100;

	if (GetKeyState(VK_LBUTTON) >= 0 ) {
		s = 0;
	}


	if (GetKeyState(VK_LBUTTON) < 0 and s == 0) {
		s = 1;

		POINT Pnt;
		GetCursorPos(&Pnt);
		ScreenToClient(hwnd2, &Pnt);

		if (GetActiveWindow() == hwnd2) {
			if (Pnt.x > 0 and Pnt.y > 0 and Pnt.x < sGLSW and Pnt.y < sGLSH) {
				using namespace std;
				cout << Pnt.x << "\t" << Pnt.y << endl;

				ip.push_back(k());
				
				vec[sizec][0] = Pnt.x / spixelScale;
				vec[sizec][1] = sSH - Pnt.y / spixelScale;
				//vec[c][2] = T12;
				
				sizec++;
			}
		}
	}if (GetKeyState(VK_RBUTTON) < 0) {
		sizec = 0;
	}



	for (y = 0; y < sSH; y++) {
		for (x = 0; x < sSW; x++) {
			c = 0;
			for (int i = 0; i < sizec; i++) {
				R = RPoint(x, y, vec[i][0], vec[i][1]);
				c = c + (A * sin(((2 * 3.14) / T12) * R + fr));

			}

			A = A % 255;
			/*c = abs(c);*/

			if (c > 255) {
				c = 255;
			}
			if (c < 0) {
				c = 0;
			}
			if (GetKeyState(VK_SPACE) < 0) {
				pixel(x, y, c, c, c);
			}
			else {
				pixel(x, y, c, c, c);
			}
		}
	}


		for (int i = 0; i < sizec; i++) {

			pixel(vec[i][0], vec[i][1], 225, 0, 225);

		}

}
void display2()
{
	int x, y;
	if (T2.fr1 - T2.fr2 >= 50)                        //only draw 20 frames/second
	{


		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		Wave();

		T2.fr2 = T2.fr1;


		glutSwapBuffers();
		glutReshapeWindow(sGLSW, sGLSH);             //prevent window scaling

	}

	T2.fr1 = glutGet(GLUT_ELAPSED_TIME);          //1000 Milliseconds per second
	glutPostRedisplay();
}

void Test() {
	glutInitWindowSize(sGLSW, sGLSH);
	glutInitWindowPosition(GLW- sGLSW, GLH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutCreateWindow("Map");
	glewInit();

	glPointSize(4);                        //pixel size
	gluOrtho2D(0, sGLSW, 0, sGLSH);                      //origin bottom left
	hwnd2 = FindWindow(NULL, L"Map");
	glutDisplayFunc(display2);

}
int main(int argc, char* argv[])
{	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(GLW, GLH);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("Wave Interference");
	glewInit();

	glPointSize(pixelScale);                        //pixel size
	//gluOrtho2D(0, GLSW, 0, GLSH);                      //origin bottom left
	
	glutDisplayFunc(display);
	

	init();


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	
	ImGui::StyleColorsDark();

	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL3_Init();

	Test();
	glutMainLoop();

	glDeleteProgram(pr);

	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
